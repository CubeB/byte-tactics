"""Link the source tree at the original exe's addresses (a placement link).

    uv run tools/place.py                # build build/place/TotalA.exe and compare it
    uv run tools/place.py --verbose      # also list every difference in full
    uv run tools/place.py --strict       # exit non-zero if any built byte differs

tools/link.py links the tree the ordinary way, so every function lands
wherever LINK puts it. That image cannot run as the game yet: the 29 gap
regions (code with no source) and much of the data hold absolute addresses, the original exe has no relocation table to move
them with, and callers that spell a callee differently need alias bridges.
place.py instead lays every piece out exactly where the original has it, the
layout LEGO Island's decomp checks its rebuilt binaries against (reccmp,
ReproBit), so code and data that are not rebuilt still find everything where
they expect it, and every call reaches the one function at its address:

  * every game function (data/progress.csv) is placed at its address from its
    object in build/progress, the objects tools/check.py compares;
  * every relocation in a placed piece is resolved by name: a placed function
    or global, a placeholder name (FUN_/DAT_<address>), a data/symbols.csv
    name (or one of its data/aliases.csv copies), an import (its slot in the
    original's import address table, or the linker's `jmp [slot]` stub), or a
    runtime library function data/functions.csv names;
  * data the compiler emits with the code (string literals, floating-point
    constants, jump tables, exception tables, vtables, file and function
    statics) is placed one symbol at a time where the original's code refers
    to it, the way a map file would say; its contents come from the object
    and are compared with the original's;
  * link/data.cpp and the globals tools/globals.py leaves out are placed at
    their addresses, one global at a time;
  * the runtime library comes from the members of LIBCMT.LIB, LIBCPMT.LIB and
    zlib that the original links, each where the original holds its bytes and
    refers to the same functions, imports and data contents;
  * what has no source yet is copied from the original and counted as copied:
    the gap regions, the few library functions no member matches, data no
    object defines, the import tables, the headers and the resources.

Every relocation is checked against the address the original uses at that
spot, and the image is compared with the original byte for byte (the
placement and data compare of #4869). The report counts where each byte came
from, so it says how much of the image is built from source and how much is
still copied. build/place/TotalA.map lists every placed piece.
"""

import argparse
import bisect
from array import array
import csv
import re
import struct
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from pathlib import Path

import pefile

from check import ROOT, base_name, load_symbols
from link import build_data, compile_all
from linkcheck import address_of

OUT_DIR = ROOT / "build/place"
PROGRESS = ROOT / "data/progress.csv"
FUNCTIONS = ROOT / "data/functions.csv"
GLOBALS = ROOT / "data/globals.csv"
ALIASES = ROOT / "data/aliases.csv"
PATCHES = ROOT / "data/exe_patches.csv"
DEF_FILES = sorted((ROOT / "link").glob("*.def"))

REL_DIR32, REL_DIR32NB, REL_REL32 = 0x06, 0x07, 0x14
SCN_CODE, SCN_UNINIT = 0x20, 0x80
SKIP_SECTIONS = (".drectve", ".debug")

# Where each byte of the image came from.
SOURCES = ["unset", "game code", "game data", "global data", "library (copied)", "gap (copied)",
           "data (copied)", "padding", "headers, imports, resources (copied)", "library code",
           "library data"]
(UNSET, CODE, OBJDATA, GLOBALDATA, LIBRARY, GAP, COPIED, PADDING, FIXED, LIBCODE,
 LIBDATA) = range(len(SOURCES))

# The libraries the original links statically: the VC5 SP3 C and C++ runtimes,
# and zlib 1.0.4 as tools/setup_toolchain.sh builds it with Cavedog's options.
LIBRARIES = [ROOT / "toolchain/msvc5-sp3/LIB/LIBCMT.LIB", ROOT / "toolchain/msvc5-sp3/LIB/LIBCPMT.LIB"]
THIRD_PARTY = ROOT / "toolchain/thirdparty/zlib-1.0.4"


# --- COFF objects -------------------------------------------------------------------

@dataclass
class Sym:
    index: int
    name: str
    value: int
    section: int      # 1-based; 0 undefined or common, negative absolute/debug
    sclass: int       # 2 external, 3 static
    is_func: bool


@dataclass
class Sec:
    index: int
    name: str
    chars: int
    data: bytes
    relocs: list[tuple[int, int, int]]   # (offset, symbol index, type)
    starts: list[int] = field(default_factory=list)   # offsets where a named symbol begins

    @property
    def is_code(self) -> bool:
        return bool(self.chars & SCN_CODE)

    def slice_at(self, offset: int) -> tuple[int, int]:
        """The symbol-to-symbol slice holding offset."""
        i = bisect.bisect_right(self.starts, offset) - 1
        lo = self.starts[i] if i >= 0 else 0
        hi = self.starts[i + 1] if i + 1 < len(self.starts) else len(self.data)
        return lo, hi


@dataclass
class Obj:
    path: Path
    secs: list[Sec]
    syms: dict[int, Sym]
    raw: bytes
    library: bool = False      # a member of a runtime or third-party library
    externals: dict[str, Sym] = field(default_factory=dict)
    absolutes: dict[str, int] = field(default_factory=dict)    # IMAGE_SYM_ABSOLUTE externals
    commons: dict[str, int] = field(default_factory=dict)      # communal (.bss) externals -> size


def parse(path: Path, data: bytes | None = None, library: bool = False) -> Obj:
    data = path.read_bytes() if data is None else data
    _, nsects, _, symptr, nsyms, opthdr, _ = struct.unpack_from("<HHIIIHH", data, 0)
    strtab = data[symptr + nsyms * 18:]

    def name_of(raw: bytes) -> str:
        if raw[:4] == b"\0\0\0\0":
            (off,) = struct.unpack_from("<I", raw, 4)
            return strtab[off:strtab.index(b"\0", off)].decode("latin-1")
        return raw[:8].split(b"\0", 1)[0].decode("latin-1")

    secs = []
    for s in range(nsects):
        hdr = data[20 + opthdr + s * 40: 20 + opthdr + s * 40 + 40]
        sname = hdr[:8].split(b"\0", 1)[0].decode("latin-1")
        if sname.startswith("/"):
            sname = strtab[int(sname[1:]):].split(b"\0", 1)[0].decode("latin-1")
        size, rawptr, relptr, _, nrel, _, chars = struct.unpack_from("<IIIIHHI", hdr, 16)
        body = data[rawptr:rawptr + size] if rawptr else bytes(size)
        relocs = [struct.unpack_from("<IIH", data, relptr + r * 10) for r in range(nrel)]
        secs.append(Sec(s + 1, sname, chars, body, relocs))
    syms: dict[int, Sym] = {}
    i = 0
    while i < nsyms:
        raw = data[symptr + i * 18: symptr + i * 18 + 18]
        value, secnum, typ, sclass, naux = struct.unpack_from("<IhHBB", raw, 8)
        syms[i] = Sym(i, name_of(raw), value, secnum, sclass, (typ & 0x30) == 0x20)
        i += 1 + naux
    obj = Obj(path, secs, syms, data, library)
    for s in syms.values():
        if s.section > 0 and s.sclass in (2, 3) and not s.name.startswith("."):
            obj.secs[s.section - 1].starts.append(s.value)
            if s.sclass == 2:
                obj.externals[s.name] = s
        elif s.sclass == 2 and s.section == -1:
            obj.absolutes[s.name] = s.value
        elif s.sclass == 2 and s.section == 0 and s.value:
            obj.commons[s.name] = s.value
    for sec in secs:
        sec.starts = sorted(set(sec.starts))
    return obj


def read_library(path: Path) -> list[Obj]:
    """The object members of a COFF archive (.lib)."""
    data = path.read_bytes()
    longnames, members, pos = b"", [], 8
    while pos + 60 <= len(data):
        name = data[pos:pos + 16].decode("latin-1").rstrip()
        size = int(data[pos + 48:pos + 58])
        body = data[pos + 60:pos + 60 + size]
        pos += 60 + size + (size & 1)
        if name == "//":
            longnames = body
        elif name != "/" and body[:2] == b"\x4c\x01":
            if name.startswith("/"):
                name = longnames[int(name[1:]):].split(b"\0", 1)[0].decode("latin-1")
            members.append(parse(Path(f"{path.name}({name.rstrip('/')})"), body, library=True))
    return members


# --- the original -------------------------------------------------------------------

class Image:
    """The original exe mapped at its addresses, the image being built over the
    same layout, and where each byte of it came from."""

    def __init__(self, path: Path):
        self.pe = pefile.PE(str(path))
        self.base = self.pe.OPTIONAL_HEADER.ImageBase
        self.size = self.pe.OPTIONAL_HEADER.SizeOfImage
        self.orig = bytearray(self.pe.get_memory_mapped_image()[:self.size])
        self.orig += bytes(self.size - len(self.orig))
        # The compiler's bytes under later hand patches (data/exe_patches.csv).
        self.pristine = bytearray(self.orig)
        self.patches = []
        if PATCHES.exists():
            for row in csv.DictReader(PATCHES.open()):
                at, original = int(row["address"], 16), bytes.fromhex(row["original"])
                self.pristine[at - self.base: at - self.base + len(original)] = original
                self.patches.append((at, len(original), row["note"]))
        self.copied_library: list[tuple[int, str]] = []   # library functions no member was placed for
        self.out = bytearray(self.size)
        self.src = bytearray(self.size)            # SOURCES index per byte
        self.owner = array("i", bytes(4 * self.size))   # 1 + the index of the piece that wrote it
        self.sections = [(s.Name.rstrip(b"\0").decode(), self.base + s.VirtualAddress,
                          s.Misc_VirtualSize, s.SizeOfRawData, s) for s in self.pe.sections]

    def inside(self, va: int) -> bool:
        return self.base + 0x1000 <= va < self.base + self.size

    def u32(self, va: int) -> int:
        return struct.unpack_from("<I", self.pristine, va - self.base)[0]

    def free(self, va: int) -> bool:
        return self.inside(va) and self.src[va - self.base] == UNSET

    def write(self, va: int, data: bytes, source: int, owner: int = 0) -> int:
        """Write where nothing is written yet; return how many bytes differed
        from what another piece had already written."""
        off = va - self.base
        clash = 0
        for i, b in enumerate(data):
            if not 0 <= off + i < self.size:
                break
            if self.src[off + i] == UNSET:
                self.out[off + i] = b
                self.src[off + i] = source
                self.owner[off + i] = owner
            elif self.out[off + i] != b:
                clash += 1
        return clash

    def copy(self, va: int, size: int, source: int) -> None:
        off = va - self.base
        for i in range(max(off, 0), min(off + size, self.size)):
            if self.src[i] == UNSET:
                self.out[i] = self.pristine[i]
                self.src[i] = source

    def imports(self) -> dict[tuple[str, str], int]:
        """(dll, name) -> the address of its slot in the import address table.
        Ordinal imports are named through link/*.def."""
        names: dict[tuple[str, int], str] = {}
        for path in DEF_FILES:
            dll = ""
            for line in path.read_text().splitlines():
                line = line.split(";", 1)[0].strip()
                m = re.match(r"LIBRARY\s+(\S+)", line, re.I)
                if m:
                    dll = m.group(1).lower().removesuffix(".dll")
                m = re.match(r"(\S+)\s+@(\d+)", line)
                if m and dll:
                    names[(dll, int(m.group(2)))] = m.group(1)
        out = {}
        for entry in self.pe.DIRECTORY_ENTRY_IMPORT:
            dll = entry.dll.decode().lower().removesuffix(".dll")
            for imp in entry.imports:
                name = imp.name.decode() if imp.name else names.get((dll, imp.ordinal), f"#{imp.ordinal}")
                out[(dll, undecorate(name))] = imp.address
        return out

    def fixed_ranges(self) -> list[tuple[int, int]]:
        """Tables the linker builds inside the sections: the import directory,
        its lookup and name tables, the import address table, and the debug and
        TLS directories."""
        out = []
        opt = self.pe.OPTIONAL_HEADER
        for idx in (1, 6, 9, 12):   # import, debug, TLS, IAT
            d = opt.DATA_DIRECTORY[idx]
            if d.VirtualAddress:
                out.append((self.base + d.VirtualAddress, d.Size))
        for entry in self.pe.DIRECTORY_ENTRY_IMPORT:
            for t in (entry.struct.OriginalFirstThunk, entry.struct.FirstThunk):
                out.append((self.base + t, 4 * (len(entry.imports) + 1)))
            out.append((self.base + entry.struct.Name, len(entry.dll) + 1))
            for imp in entry.imports:
                if imp.name:
                    out.append((self.base + imp.hint_name_table_rva, len(imp.name) + 3))
        return out


def undecorate(name: str) -> str:
    """_CreateFileA@28 -> CreateFileA, so import names compare across spellings."""
    return re.sub(r"@\d+$", "", name.lstrip("_"))


# --- placing ------------------------------------------------------------------------

@dataclass
class Piece:
    obj: Obj
    sec: Sec
    lo: int           # the part of the section placed
    hi: int
    va: int
    source: int
    label: str
    id: int
    why: str = ""     # for a piece placed where the original refers to it: the referring site


class Placer:
    def __init__(self, img: Image, objects: list[Obj], symbols: dict[str, int]):
        self.img = img
        self.objects = objects
        self.symbols = symbols
        self.pieces: list[Piece] = []
        self.placed: dict[tuple[int, int], list[Piece]] = defaultdict(list)   # (id(obj), sec) -> pieces
        self.globals: dict[str, int] = {}          # external code name -> address
        self.definers: dict[str, list[tuple[Obj, Sym]]] = defaultdict(list)
        self.absolutes: dict[str, int] = {}
        self.commons: dict[str, int] = {}
        for obj in objects:
            self.add_definer(obj)
        self.aliases: dict[str, set[int]] = defaultdict(set)
        if ALIASES.exists():
            for row in csv.DictReader(ALIASES.open()):
                self.aliases[row["name"]].add(int(row["address"], 16))
        self.import_slots = img.imports()
        # The linker's `jmp [slot]` stubs, for callers that call an import directly.
        slot_name = {a: n for (_, n), a in self.import_slots.items()}
        self.thunks: dict[str, int] = {}
        text = next(s for s in img.sections if s[0] == ".text")
        lo = text[1] - img.base
        for m in re.finditer(rb"\xff\x25(....)", bytes(img.pristine[lo:lo + text[2]])):
            (slot,) = struct.unpack("<I", m.group(1))
            if slot in slot_name and m.start() % 2 == 0:
                self.thunks.setdefault(slot_name[slot], img.base + lo + m.start())
        self.library: dict[str, list[int]] = defaultdict(list)   # runtime library symbol -> addresses
        self.stats = Counter()
        self.mismatches: list[str] = []
        self.kept: list[str] = []
        self.unresolved: Counter = Counter()
        self.clashes: list[str] = []
        self.renamed: list[str] = []
        self.todo: list[Piece] = []
        # Library function names by address, for refs_agree: data/functions.csv's,
        # corrected as members are placed.
        self.named_at: dict[int, str] = {}
        for row in load_rows(FUNCTIONS):
            if row["kind"] == "library" and row["name"]:
                self.named_at[int(row["address"], 16)] = row["name"].split(": ", 1)[-1]

    def add_definer(self, obj: Obj) -> None:
        for name, sym in obj.externals.items():
            self.definers[name].append((obj, sym))
        self.absolutes.update(obj.absolutes)
        self.commons.update(obj.commons)

    # -- pieces --

    def place(self, obj: Obj, sec: Sec, lo: int, hi: int, va: int, source: int, label: str) -> Piece:
        piece = Piece(obj, sec, lo, hi, va, source, label, len(self.pieces) + 1)
        clash = self.img.write(va, sec.data[lo:hi], source, piece.id)
        if clash:
            self.clashes.append(f"{va:#x} {label}: {clash} byte(s) differ from a piece already placed there")
        self.pieces.append(piece)
        self.placed[(id(obj), sec.index)].append(piece)
        if sec.is_code:
            for sym in obj.syms.values():
                if sym.section == sec.index and sym.sclass == 2 and lo <= sym.value < hi:
                    self.globals.setdefault(sym.name, va + sym.value - lo)
        self.todo.append(piece)
        return piece

    def address_in(self, obj: Obj, section: int, offset: int) -> int | None:
        for p in self.placed.get((id(obj), section), ()):
            if p.lo <= offset < p.hi:
                return p.va + offset - p.lo
        return None

    def place_at(self, obj: Obj, sym: Sym, offset: int, va: int, why: str = "") -> bool:
        """Place the part of sym's section that holds `offset` so that offset
        lands at va: a whole section for code, one symbol's slice for data.
        Nothing is placed over another piece; returns whether it was placed."""
        sec = obj.secs[sym.section - 1]
        if sec.name.startswith(SKIP_SECTIONS):
            return False
        lo, hi = (0, len(sec.data)) if sec.is_code else sec.slice_at(offset)
        start = va - (offset - lo)
        if self.address_in(obj, sec.index, offset) is not None or not self.img.free(start):
            return False
        if sec.is_code:
            # Code goes only where the original holds the same code: a library
            # member's neighbours can differ from the original's.
            hi = body_size(sec)
            if not masked_match(self.img, sec, start, hi):
                return False
            if obj.library and not self.refs_agree(obj, sec, start):
                return False
        if obj.library:
            source = LIBCODE if sec.is_code else LIBDATA
        else:
            source = CODE if sec.is_code else OBJDATA
        piece = self.place(obj, sec, lo, hi, start, source, f"{sec.name} of {obj.path.stem} ({sym.name})")
        piece.why = why
        self.stats["code pieces placed where the original refers to them" if sec.is_code
                   else "data pieces placed where the original refers to them"] += 1
        return True

    def refs_agree(self, obj: Obj, sec: Sec, start: int) -> bool:
        """Whether a library member's code placed at start refers to what the
        original's code there refers to: its calls reach functions of the same
        names, its imports the same slots, and its own data (literals, tables,
        initial values) the same contents. Many runtime functions differ only
        there: _read, _write and _lseek's locking wrappers, _Xlen and _Xran,
        zlib's get_crc_table and zlibVersion."""
        img = self.img
        for off, symidx, rtype in sec.relocs:
            if off + 4 > len(sec.data):
                continue
            site, sym = start + off, obj.syms[symidx]
            ours = struct.unpack_from("<I", sec.data, off)[0]
            if rtype == REL_REL32:
                target = (site + 4 + img.u32(site) - ours) & 0xFFFFFFFF
                if target in self.named_at:
                    if self.named_at[target] != sym.name:
                        return False
                    continue
                # An unnamed callee: some library member defining the name
                # must hold the code there (_lockexit calls _lock, _unlockexit
                # _unlock, and the two are otherwise identical).
                callees = [(o, t) for o, t in self.definers.get(sym.name, ())
                           if o.library and t.section > 0 and o.secs[t.section - 1].is_code]
                if callees and not any(masked_match(img, o.secs[t.section - 1], target - t.value,
                                                    body_size(o.secs[t.section - 1]))
                                       for o, t in callees):
                    return False
            elif rtype == REL_DIR32 and sym.name.startswith("__imp_"):
                slots = [a for (_, n), a in self.import_slots.items()
                         if n == undecorate(sym.name[len("__imp_"):])]
                # An import the original does not have cannot be what it calls.
                if len(slots) != 1 or (img.u32(site) - ours) & 0xFFFFFFFF != slots[0]:
                    return False
            elif rtype == REL_DIR32:
                item = (img.u32(site) - ours) & 0xFFFFFFFF
                offset = sym.value + (ours if sym.name.startswith(".") else 0)
                if sym.section > 0 and not obj.secs[sym.section - 1].is_code:
                    if not self.content_matches(obj, sym, offset, item + (offset - sym.value)):
                        return False
                elif sym.section == 0 and not sym.name.startswith("__imp_"):
                    # Data another member defines: one of its definers must hold
                    # the original's contents there.
                    held = [(o, t) for o, t in self.definers.get(sym.name, ())
                            if o.library and t.section > 0 and not o.secs[t.section - 1].is_code]
                    if held and not any(self.content_matches(o, t, t.value, item) for o, t in held):
                        return False
        return True

    def content_matches(self, obj: Obj, sym: Sym, offset: int, va: int, depth: int = 3) -> bool:
        """Whether the original holds, at va, the bytes of sym's section from
        offset to the end of its slice (at most 32 bytes): zeros for
        uninitialised data, a string up to its terminator, and for the fields
        the linker fills in, the same contents again where they point at data
        of the same object (an exception's throw info, down to the type name)."""
        sec = obj.secs[sym.section - 1]
        if not self.img.inside(va):
            return True
        lo, hi = sec.slice_at(offset)
        hi = min(hi, offset + 32)
        o = va - self.img.base
        if sec.chars & SCN_UNINIT:
            return not any(self.img.pristine[o:o + hi - offset])
        text = sec.data[offset:hi]
        if sym.name.startswith(("??_C@_0", "$SG")) and b"\0" in text:
            hi = offset + text.index(b"\0") + 1      # a string: up to its terminator
        fixed = bytearray(b"\1" * (hi - offset))
        for roff, symidx, rtype in sec.relocs:
            if not offset <= roff < hi:
                continue
            for i in range(roff, min(roff + 4, hi)):
                fixed[i - offset] = 0
            target = obj.syms[symidx]
            if (depth and rtype == REL_DIR32 and roff + 4 <= hi and target.section > 0
                    and not obj.secs[target.section - 1].is_code):
                (ours,) = struct.unpack_from("<I", sec.data, roff)
                at = target.value + ours          # the field holds the target's address plus ours
                theirs = struct.unpack_from("<I", self.img.pristine, o + roff - offset)[0]
                if not self.content_matches(obj, target, at, theirs, depth - 1):
                    return False
        theirs = self.img.pristine[o:o + hi - offset]
        return all(not f or a == b for a, b, f in zip(sec.data[offset:hi], theirs, fixed))

    # -- symbols --

    def resolve(self, obj: Obj, sym: Sym, offset: int) -> int | None:
        """The address a relocation against sym (at offset `offset` from its
        section's start, for section symbols) should hold, or None when only
        the original can say."""
        if sym.section > 0:
            sec = obj.secs[sym.section - 1]
            local = self.address_in(obj, sym.section, offset)
            if local is not None:
                return local - (offset - sym.value)
            if sec.is_code and sym.sclass == 2:
                # A copy of a function defined elsewhere (kept so it inlines):
                # the call goes to the one at its address.
                if sym.name in self.globals:
                    return self.globals[sym.name]
                addr = self.named_address(sym.name)
                if addr is not None:
                    return addr
            if not sec.is_code and sym.sclass == 2:
                addr = address_of(sym.name, self.symbols)
                if addr is not None:
                    self.place_at(obj, sym, sym.value, addr)
                    return addr
            return None
        name = sym.name
        if name in self.globals:
            return self.globals[name]
        if name in self.absolutes:
            return self.absolutes[name]
        if name.startswith("__imp_"):
            imp = undecorate(name[len("__imp_"):])
            hits = [a for (dll, n), a in self.import_slots.items() if n == imp]
            if not hits and re.fullmatch(r"DAT_[0-9a-f]{8}", imp):
                # An import the source has no name for yet, named by its slot.
                hits = [a for a in self.import_slots.values() if a == int(imp[4:], 16)]
            return hits[0] if len(hits) == 1 else None
        if undecorate(name) in self.thunks and name not in self.definers:
            return self.thunks[undecorate(name)]
        return self.named_address(name)

    def place_copy(self, name: str, va: int) -> bool:
        """Place a second copy of a library function at va when the original
        holds one there (LIBCMT's lseek is linked twice)."""
        for obj, sym in self.definers.get(name, ()):
            sec = obj.secs[sym.section - 1]
            if not obj.library or not sec.is_code:
                continue
            start = va - sym.value
            body = body_size(sec)
            if not self.img.free(start) or not masked_match(self.img, sec, start, body):
                continue
            copy = parse(obj.path, obj.raw, library=True)
            self.objects.append(copy)
            self.place(copy, copy.secs[sec.index - 1], 0, body, start, LIBCODE, f"{name} (second copy)")
            self.stats["library sections placed as a second copy"] += 1
            return True
        return False

    def copies(self, name: str) -> set[int]:
        """Every address one name may stand for: the copies data/aliases.csv
        lists and the runtime library code linked twice (std::_Lockit)."""
        base = base_name(name)
        out = set(self.aliases.get(base, ())) | set(self.library.get(name, ()))
        if base in self.symbols and self.aliases.get(base):
            out.add(self.symbols[base])
        return out

    def named_address(self, name: str) -> int | None:
        if name in self.library:
            return self.library[name][0]
        addr = address_of(name, self.symbols)
        if addr is not None:
            return addr
        base = base_name(name)
        for cand in (base, base[1:]):
            if len(self.aliases.get(cand, ())) == 1:
                # A name only data/aliases.csv knows (__chkstk is _chkstk).
                return next(iter(self.aliases[cand]))
        if name.startswith("?") and "@@Y" in name:
            # A C runtime function declared without extern "C" (?tolower@@YAHH@Z).
            base = base_name(name)
            if "::" not in base and "_" + base in self.library:
                return self.library["_" + base][0]
        return None

    # -- relocations --

    def relocate(self) -> None:
        while self.todo:
            piece = self.todo.pop()
            obj, sec = piece.obj, piece.sec
            for off, symidx, rtype in sec.relocs:
                if not piece.lo <= off <= piece.hi - 4:
                    continue
                site = piece.va + off - piece.lo
                sym = obj.syms[symidx]
                (ours,) = struct.unpack_from("<I", sec.data, off)
                theirs = self.img.u32(site)
                if rtype == REL_DIR32:
                    orig_target = (theirs - ours) & 0xFFFFFFFF
                elif rtype == REL_REL32:
                    orig_target = (site + 4 + theirs - ours) & 0xFFFFFFFF
                elif rtype == REL_DIR32NB:
                    orig_target = (theirs - ours + self.img.base) & 0xFFFFFFFF
                else:
                    self.stats[f"relocation type {rtype:#x} skipped"] += 1
                    continue
                # A section symbol plus an offset names whatever lies there.
                offset = sym.value + (ours if rtype == REL_DIR32 and sym.name.startswith(".") else 0)
                target = self.resolve(obj, sym, offset)
                how = "by name"
                if target is None:
                    # Nothing names the target: it is where the original's code
                    # points, and the piece that defines it goes there.
                    target = orig_target
                    definer = None
                    if sym.section > 0:
                        definer = (obj, sym)
                    elif sym.name in self.definers:
                        # Several members can define one name (crt0 and wincrt0
                        # both define __app_type): take the one whose contents
                        # the original holds.
                        found = [(o, s) for o, s in self.definers[sym.name] if s.section > 0]
                        definer = next(((o, s) for o, s in found
                                        if self.content_matches(o, s, s.value, orig_target)), None)
                        definer = definer or (found[0] if found else None)
                    if definer and self.img.inside(orig_target):
                        d_obj, d_sym = definer
                        at = offset if d_obj is obj else d_sym.value
                        self.place_at(d_obj, d_sym, at, orig_target + (at - d_sym.value),
                                      f"{site:#x} in {piece.label}")
                        how = "where the original refers to it"
                    elif sym.name in self.commons:
                        # Communal data the linker allocates in .bss.
                        how = "to common data where the original has it"
                    else:
                        how = "from the original's bytes"
                        self.unresolved[sym.name] += 1
                elif target != orig_target and (orig_target in self.copies(sym.name)
                                                or self.place_copy(sym.name, orig_target)):
                    target = orig_target       # another copy of the same function
                    how = "by name, to another copy"
                self.stats[f"relocations resolved {how}"] += 1
                if target != orig_target:
                    note = (f"{site:#x} in {piece.label}: {sym.name} resolves to {target:#x}, "
                            f"the original uses {orig_target:#x}")
                    if not sec.is_code:
                        # A vtable or table of the source's own (partial) view of
                        # a class: keep the original's entry.
                        self.kept.append(note)
                        o = site - self.img.base
                        if self.pieces_own(piece, site):
                            self.img.out[o:o + 4] = self.img.pristine[o:o + 4]
                        continue
                    self.mismatches.append(note)
                if rtype == REL_DIR32:
                    value = (target + ours) & 0xFFFFFFFF
                elif rtype == REL_DIR32NB:
                    value = (target + ours - self.img.base) & 0xFFFFFFFF
                else:
                    value = (target + ours - (site + 4)) & 0xFFFFFFFF
                o = site - self.img.base
                if self.pieces_own(piece, site):
                    self.img.out[o:o + 4] = struct.pack("<I", value)

    def pieces_own(self, piece: Piece, site: int) -> bool:
        """Whether this piece wrote the field at site (a piece placed partly
        over another must not patch the other's bytes)."""
        o = site - self.img.base
        return all(self.img.owner[o + i] == piece.id for i in range(4))


# --- the build ----------------------------------------------------------------------

def load_rows(path: Path) -> list[dict]:
    with path.open() as fh:
        return list(csv.DictReader(fh))


def place_functions(placer: Placer, objects_by_src: dict[str, Obj]) -> None:
    sizes = {int(r["address"], 16): int(r["size"]) for r in load_rows(FUNCTIONS)}
    for row in load_rows(PROGRESS):
        addr, symbol = int(row["address"], 16), row["symbol"]
        obj = objects_by_src.get(row["file"])
        if obj is None or not symbol:
            placer.stats["game functions with no object"] += 1
            continue
        found = next((s for s in obj.syms.values() if s.name == symbol and s.section > 0
                      and obj.secs[s.section - 1].is_code), None)
        if found is None:
            placer.mismatches.append(f"{addr:#x}: {symbol} is not defined in {row['file']}")
            continue
        sec = obj.secs[found.section - 1]
        size = sizes.get(addr, len(sec.data) - found.value)
        placer.place(obj, sec, found.value, found.value + size, addr, CODE, symbol)
        placer.stats["game functions placed"] += 1
    # The alignment padding the compiler leaves after each function (every
    # function is its own COMDAT section, padded to 16 bytes with nops).
    for p in list(placer.pieces):
        if p.source == CODE and p.hi < len(p.sec.data):
            off = p.va + p.hi - p.lo - placer.img.base
            for i, b in enumerate(p.sec.data[p.hi:]):
                if placer.img.src[off + i] == UNSET:
                    placer.img.out[off + i], placer.img.src[off + i] = b, PADDING


def place_globals(placer: Placer, data_objs: list[Path], data_addr: dict[int, str]) -> None:
    """link/data.cpp and the generated extra globals, one global at a time."""
    sizes = {int(r["address"], 16): int(r["size"]) for r in load_rows(GLOBALS)}
    for path in data_objs:
        obj = parse(path)
        placer.objects.append(obj)
        placer.add_definer(obj)
        for sec in obj.secs:
            if sec.is_code or sec.name.startswith(SKIP_SECTIONS):
                continue
            for name, sym in obj.externals.items():
                if sym.section != sec.index:
                    continue
                addr = address_of(name, placer.symbols)
                if addr is None or data_addr.get(addr) != name:
                    continue
                lo, hi = sec.slice_at(sym.value)
                hi = min(hi, sym.value + sizes.get(addr, hi - sym.value))
                placer.place(obj, sec, sym.value, hi, addr, GLOBALDATA, name)
                placer.stats["globals placed from link/"] += 1


def masked_match(img: Image, sec: Sec, start: int, size: int) -> bool:
    """Whether the original holds sec's first `size` bytes at start, ignoring
    the fields the linker fills in."""
    fixed = bytearray(b"\1" * size)
    for off, _, rtype in sec.relocs:
        for i in range(off, min(off + 4, size)):
            fixed[i] = 0
    o = start - img.base
    if not img.inside(start) or o + size > img.size:
        return False
    return all(not f or a == b for a, b, f in zip(sec.data[:size], img.pristine[o:o + size], fixed))


def body_size(sec: Sec) -> int:
    """A code section without its trailing alignment padding."""
    return len(sec.data.rstrip(b"\x90\xcc")) or len(sec.data)


def place_library(placer: Placer) -> None:
    """The runtime library and zlib code, from the members of the libraries
    the original links, at the addresses data/functions.csv gives.

    A member goes where the original holds its bytes (ignoring the fields the
    linker fills in) and its calls reach functions of the right names: the
    locking wrappers _read, _write and _lseek are byte-identical apart from
    the function they call, so the bytes alone cannot tell them apart."""
    members = [m for lib in LIBRARIES if lib.exists() for m in read_library(lib)]
    members += [parse(p, library=True) for p in sorted(THIRD_PARTY.glob("*.obj"))]
    by_name: dict[str, list[tuple[Obj, Sym]]] = defaultdict(list)
    by_size: dict[int, list[tuple[Obj, Sym]]] = defaultdict(list)
    for m in members:
        placer.objects.append(m)
        placer.add_definer(m)
        for name, sym in m.externals.items():
            sec = m.secs[sym.section - 1]
            if sec.is_code:
                by_name[name].append((m, sym))
                if sym.value == 0:
                    by_size[body_size(sec)].append((m, sym))
    img = placer.img
    rows = [r for r in load_rows(FUNCTIONS) if r["kind"] == "library"]
    named_at = placer.named_at

    pending = rows
    while pending:
        left = []
        for row in pending:
            addr, size = int(row["address"], 16), int(row["size"])
            if not img.free(addr):
                continue          # placed with an earlier function of the same section
            name = row["name"].split(": ", 1)[-1]
            for obj, sym in by_name.get(name, []) + by_size.get(size, []):
                sec = obj.secs[sym.section - 1]
                start, body = addr - sym.value, body_size(sec)
                if not masked_match(img, sec, start, max(body, sym.value + size)):
                    continue
                if not placer.refs_agree(obj, sec, start):
                    continue
                if (id(obj), sec.index) in placer.placed:
                    # Linked twice (std::_Lockit): a second copy of the member.
                    obj = parse(obj.path, obj.raw, library=True)
                    placer.objects.append(obj)
                    sec = obj.secs[sym.section - 1]
                placer.place(obj, sec, 0, body, start, LIBCODE, sym.name)
                placer.stats["library sections placed"] += 1
                if name and sym.name != name:
                    placer.renamed.append(f"{addr:#x}: data/functions.csv says {name}, the member placed "
                                          f"there defines {sym.name} ({obj.path.name})")
                # What is placed now names its address for the calls_agree of the rest.
                for other in obj.externals.values():
                    if other.section == sec.index:
                        named_at[start + other.value] = other.name
                break
            else:
                left.append(row)
        if len(left) == len(pending):
            break
        pending = left
    placer.stats["library functions with no matching member by name"] += len(pending)


def library_names(placer: Placer) -> None:
    for row in load_rows(FUNCTIONS):
        if row["kind"] == "library" and row["name"]:
            name = row["name"].split(": ", 1)[-1]
            placer.library[name].append(int(row["address"], 16))


def copy_unbuilt(img: Image) -> None:
    for row in load_rows(FUNCTIONS):
        addr, size = int(row["address"], 16), int(row["size"])
        size = ((addr + size + 15) & ~15) - addr      # with its alignment padding
        if row["kind"] == "gap":
            img.copy(addr, size, GAP)
        elif row["kind"] == "library":
            if img.free(addr):
                img.copied_library.append((addr, row["name"]))
            img.copy(addr, size, LIBRARY)
    for va, size in img.fixed_ranges():
        img.copy(va, size, FIXED)
    for name, start, vsize, rsize, _ in img.sections:
        if name in (".tls", ".rsrc"):
            img.copy(start, vsize, FIXED)
        # Past each section's virtual size: the linker's zero fill.
        for i in range(start + vsize - img.base, start + max(vsize, rsize) - img.base):
            if img.src[i] == UNSET:
                img.out[i], img.src[i] = 0, PADDING
    # Everything still unset is data or padding no object defines yet.
    for name, start, vsize, rsize, _ in img.sections:
        img.copy(start, max(vsize, rsize), COPIED)


def write_exe(img: Image, out: Path) -> None:
    raw = bytearray(img.pe.__data__[:img.pe.OPTIONAL_HEADER.SizeOfHeaders])
    for name, start, vsize, rsize, s in img.sections:
        need = s.PointerToRawData + rsize
        if len(raw) < need:
            raw += bytes(need - len(raw))
        raw[s.PointerToRawData:need] = img.out[start - img.base:start - img.base + rsize]
    raw += img.pe.__data__[len(raw):]       # anything past the last section
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_bytes(bytes(raw))


def write_map(placer: Placer, out: Path) -> None:
    rows = sorted((p.va, p.hi - p.lo, SOURCES[p.source], p.label, p.obj.path.name) for p in placer.pieces)
    with out.open("w") as fh:
        fh.write("address,size,source,symbol,object\n")
        for va, size, source, label, obj in rows:
            fh.write(f"{va:#x},{size},{source},{label},{obj}\n")


# --- the report ---------------------------------------------------------------------

def compare(img: Image, placer: Placer, verbose: bool) -> tuple[int, list[str]]:
    """Bytes of the built image that differ from the original, by source."""
    def label(va: int) -> str:
        pid = img.owner[va - img.base]
        if not pid:
            return ""
        p = placer.pieces[pid - 1]
        return f"{p.label}+{va - p.va:#x}" + (f" (placed for {p.why})" if p.why else "")

    patched = set()
    for at, size, _ in img.patches:
        patched.update(range(at - img.base, at - img.base + size))
    lines, total, by_source, run = [], 0, Counter(), None
    for name, start, vsize, rsize, _ in img.sections:
        lo, hi = start - img.base, start - img.base + max(vsize, rsize)
        for i in range(lo, hi + 1):
            differs = i < hi and img.out[i] != img.orig[i] and i not in patched
            if differs:
                total += 1
                by_source[SOURCES[img.src[i]]] += 1
                if run is None:
                    run = i
            elif run is not None:
                va = img.base + run
                lines.append(f"  {va:#x}..{img.base + i:#x} ({i - run} bytes, {SOURCES[img.src[run]]}) {label(va)}")
                run = None
    out = [f"bytes that differ from the original, outside data/exe_patches.csv: {total:,}"]
    out += [f"  {k}: {v:,}" for k, v in by_source.most_common()]
    patch_diff = sum(1 for i in patched if img.out[i] != img.orig[i])
    if img.patches:
        out.append(f"bytes under data/exe_patches.csv (the compiler's bytes, not the patch): {patch_diff:,}")
    shown = lines if verbose else lines[:20] + (["  ..."] if len(lines) > 20 else [])
    return total, out + shown


def report(img: Image, placer: Placer, verbose: bool) -> int:
    print("placement:")
    for k, v in sorted(placer.stats.items()):
        print(f"  {k}: {v:,}")
    counts = Counter()
    for name, start, vsize, rsize, _ in img.sections:
        for i in range(start - img.base, start - img.base + max(vsize, rsize)):
            counts[(name, img.src[i])] += 1
    print("where each section's bytes come from:")
    for name, start, vsize, rsize, _ in img.sections:
        parts = [f"{SOURCES[s]} {counts[(name, s)]:,}" for s in range(len(SOURCES)) if counts[(name, s)]]
        print(f"  {name:6} {max(vsize, rsize):>9,}: " + ", ".join(parts))
    limit = None if verbose else 15
    for title, items in (("code relocations that disagree with the original", placer.mismatches),
                         ("table entries in compiled data that disagree with the original (the original's kept)",
                          placer.kept),
                         ("pieces that differ from a piece already placed there", placer.clashes),
                         ("library functions data/functions.csv names differently", placer.renamed)):
        if items:
            print(f"{title}: {len(items):,}")
            for m in items[:limit]:
                print("  " + m)
    if img.copied_library:
        print(f"library functions copied, with no member placed: {len(img.copied_library):,}")
        for addr, name in img.copied_library[:limit]:
            print(f"  {addr:#x} {name}")
    if placer.unresolved:
        print(f"names whose address only the original's bytes give: {len(placer.unresolved):,}")
        for name, n in placer.unresolved.most_common(limit):
            print(f"  {name} ({n})")
    total, lines = compare(img, placer, verbose)
    print("\n".join(lines))
    return total


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--output", type=Path, default=OUT_DIR / "TotalA.exe")
    ap.add_argument("--jobs", type=int, default=None, help="parallel compiles (default: all cores)")
    ap.add_argument("--strict", action="store_true", help="exit non-zero if any built byte differs")
    ap.add_argument("--verbose", "-v", action="store_true")
    args = ap.parse_args()
    output = args.output if args.output.is_absolute() else ROOT / args.output

    paths, failed = compile_all(args.jobs)
    if failed:
        raise SystemExit(f"{len(failed)} file(s) did not compile; tools/link.py lists them")
    symbols = load_symbols()
    objects = [parse(p) for p in paths]
    by_src = {}
    for p, obj in zip(paths, objects):
        by_src[str(Path("src") / p.relative_to(ROOT / "build/progress").with_suffix(".cpp"))] = obj

    img = Image(ROOT / "orig/TotalA.exe")
    placer = Placer(img, objects, symbols)
    library_names(placer)
    place_functions(placer, by_src)
    place_library(placer)
    data_objs, data_addr = build_data(symbols)
    place_globals(placer, data_objs, data_addr)
    placer.relocate()
    copy_unbuilt(img)
    write_exe(img, output)
    write_map(placer, output.with_suffix(".map"))
    total = report(img, placer, args.verbose)
    print(f"{output.relative_to(ROOT)}: {output.stat().st_size:,} bytes")
    if args.strict and total:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
