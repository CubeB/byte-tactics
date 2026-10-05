"""Carve what the tree has no source for out of the original exe, as
relocatable objects for an ordinary link (tools/link.py --carve).

    uv run tools/carve.py              # write build/link/gaps.obj and build/link/origdata.obj
    uv run tools/carve.py --verbose    # also list every address no symbol names

LEGO Island's decomp links the Smacker code it has no source for from a library
carved out of the retail DLL: the original bytes, with the relocations rebuilt
from the DLL's base relocations (its tools/gen_smacker_lib.py). TotalA.exe has
no base relocations, so here the relocation sites come from elsewhere:

  * gaps.obj holds the gap regions of data/functions.csv (hand-written
    assembly, code with no FPO record, the linker's import stubs) that have
    no matching source in src/gap/ yet (tools/gapcheck.py; a region whose
    source matches is linked from its own object), and the exception handler
    stubs of their functions, one section each. Its
    relocations come from disassembly: every rel32 branch that leaves its
    region, and every 32-bit immediate or displacement that holds an address
    in the image.
  * origdata.obj holds the original's .rdata and .data byte for byte, apart
    from the tables the linker builds itself (the import tables, the TLS and
    debug directories, the .CRT$X* initialiser tables), which are zeroed. Its
    relocations come from tools/place.py's layout: every pointer field of a
    placed piece of data; in data no object defines, every dword that holds
    the exact address of a function, a global, a placed piece or a string
    (unaligned too: packed records keep pointers anywhere), every element of
    a global data/globals.csv types as a pointer, and every address in the
    compiler's exception tables.

Every address a relocation points at is named from the same layout: a game
function by the symbol its object defines, a runtime library function or
global by its library name, an import by its __imp_ symbol, and the gap
regions and the original's data by the symbols these two objects define.

The layout also says what every reference in the tree's own objects means,
which tools/link.py --carve applies to copies of them (patch_objects):

  * a name no object defines is aliased to the symbol at the address the
    original's code holds wherever the name is used (in a placed function, or
    in a byte-identical copy of one another file keeps so that it inlines);
  * origdata.obj defines, at each global's address and each placed vtable's,
    every name a compiled object defines there, and is linked first, so LINK
    keeps one copy: the original's, at its full size and with every slot;
  * a placed function's references to file statics and to functions of its
    own file (copies kept so that they inline) point at the original's data
    and at the real function, where the original's code points;
  * every other file's definition of a game function becomes static, so the
    name binds to the annotated one;
  * a placed function the compiler made static (the `_$E<n>` initialisers of
    global objects and the destructors they register with atexit) gets a
    public name, __static_<address>, so that the references above can reach
    it: 0x4b2290 registers 0x4b2340, the destructor in another file, where
    its own object would register its own, which calls a stub.
"""

import argparse
import bisect
import re
import struct
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from pathlib import Path

import capstone

from check import ROOT
from linkcheck import IMPORT_LIBS, LIBDIR, address_of, archive_symbols, read_object
from place import (CODE, FUNCTIONS, GAPCODE, GLOBALDATA, GLOBALS, LIBCODE, LIBDATA, OBJDATA, PROGRESS,
                   REL_DIR32, REL_REL32, Image, Obj, Placer, Sec, layout, load_rows, undecorate)

OUT = ROOT / "build/link"
THIRD_PARTY_OBJS = sorted((ROOT / "toolchain/thirdparty/zlib-1.0.4").glob("*.obj"))
IMAGE_SYM_CLASS_EXTERNAL, IMAGE_SYM_CLASS_STATIC = 2, 3
SCN_MEM_WRITE = 0x80000000
TEXT_CHARS = 0x60500020          # code, execute/read, 16-byte aligned
RDATA_CHARS = 0x40D00040         # initialised data, read, 4096-byte aligned
DATA_CHARS = 0xC0D00040          # initialised data, read/write, 4096-byte aligned


# --- writing COFF objects ---------------------------------------------------------

class Coff:
    """A minimal COFF object writer: sections with relocations against named
    symbols, external definitions, and undefined externals added on use."""

    def __init__(self):
        self.sections: list[dict] = []
        self.defs: dict[str, tuple[int, int]] = {}      # name -> (section index from 1, value)

    def section(self, name: str, data: bytes, chars: int) -> int:
        self.sections.append({"name": name, "data": bytearray(data), "chars": chars, "relocs": []})
        return len(self.sections)

    def define(self, name: str, section: int, value: int) -> None:
        self.defs.setdefault(name, (section, value))

    def reloc(self, section: int, offset: int, symbol: str, rtype: int, addend: int) -> None:
        sec = self.sections[section - 1]
        struct.pack_into("<i", sec["data"], offset, addend)
        sec["relocs"].append((offset, symbol, rtype))

    def write(self, path: Path) -> None:
        strings = bytearray()

        def name_field(name: str) -> bytes:
            raw = name.encode("latin-1")
            if len(raw) <= 8:
                return raw + b"\0" * (8 - len(raw))
            off = 4 + len(strings)
            strings.extend(raw + b"\0")
            return b"\0\0\0\0" + struct.pack("<I", off)

        # One static symbol per section, the definitions, then the undefined
        # names the relocations use.
        symtab = bytearray()
        index: dict[str, int] = {}
        n = 0
        for i, sec in enumerate(self.sections, start=1):
            symtab += name_field(sec["name"]) + struct.pack("<IhHBB", 0, i, 0, IMAGE_SYM_CLASS_STATIC, 0)
            n += 1
        for name, (sec, value) in self.defs.items():
            typ = 0x20 if self.sections[sec - 1]["chars"] & 0x20 else 0
            symtab += name_field(name) + struct.pack("<IhHBB", value, sec, typ, IMAGE_SYM_CLASS_EXTERNAL, 0)
            index[name] = n
            n += 1
        for sec in self.sections:
            for _, symbol, _ in sec["relocs"]:
                if symbol not in index:
                    symtab += name_field(symbol) + struct.pack("<IhHBB", 0, 0, 0, IMAGE_SYM_CLASS_EXTERNAL, 0)
                    index[symbol] = n
                    n += 1

        pos = 20 + 40 * len(self.sections)
        headers, bodies = bytearray(), bytearray()
        for sec in self.sections:
            data = bytes(sec["data"])
            relocs = b"".join(struct.pack("<IIH", off, index[sym], rtype) for off, sym, rtype in sec["relocs"])
            nrel = len(sec["relocs"])
            if nrel > 0xFFFF:
                raise SystemExit(f"{sec['name']}: more relocations than COFF allows")
            headers += sec["name"].encode()[:8].ljust(8, b"\0") + struct.pack(
                "<IIIIIIHHI", 0, 0, len(data), pos, pos + len(data) if nrel else 0, 0, nrel, 0, sec["chars"])
            bodies += data + relocs
            pos += len(data) + len(relocs)
        header = struct.pack("<HHIIIHH", 0x14C, len(self.sections), 0, pos, n, 0, 0)
        strtab = struct.pack("<I", 4 + len(strings)) + bytes(strings)
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(header + bytes(headers) + bytes(bodies) + bytes(symtab) + strtab)


# --- naming addresses ---------------------------------------------------------------

def import_symbols(img: Image) -> dict[int, str]:
    """Import address table slot -> the __imp_ symbol an import library
    defines for it: the toolchain's libraries by name, and for the DLLs the
    game imports by ordinal, the names tools/link.py gives its own libraries."""
    from link import DPLAYX_EXPORTS, SMACKW32_EXPORTS
    by_name: dict[str, str] = {}
    for name in IMPORT_LIBS:
        lib = LIBDIR / f"{name}.LIB"
        if lib.exists():
            for sym in archive_symbols(lib):
                if sym.startswith("__imp_"):
                    by_name.setdefault(undecorate(sym[len("__imp_"):]), sym)
    by_ordinal = {("smackw32", o): f"__imp__{n}@{a}" for n, a, o in SMACKW32_EXPORTS}
    by_ordinal |= {("dplayx", o): f"__imp__{n}@{a}" for n, a, o in DPLAYX_EXPORTS}
    out = {}
    for entry in img.pe.DIRECTORY_ENTRY_IMPORT:
        dll = entry.dll.decode().lower().removesuffix(".dll")
        for imp in entry.imports:
            sym = by_name.get(imp.name.decode()) if imp.name else by_ordinal.get((dll, imp.ordinal))
            if sym:
                out[imp.address] = sym
    return out


def gap_symbol(addr: int) -> str:
    return f"__gap_{addr:08x}"


def data_symbol(section: str) -> str:
    return "__orig" + section.replace(".", "_")


def static_alias(addr: int) -> str:
    """The public name patch_objects gives the placed function at addr when
    its object has only a static one."""
    return f"__static_{addr:08x}"


def external_at(piece, offset: int) -> str | None:
    """The external symbol of a piece's section that starts exactly at offset
    (slices of one section are placed apart, so no other offset is safe)."""
    for sym in piece.obj.syms.values():
        if sym.section == piece.sec.index and sym.sclass == IMAGE_SYM_CLASS_EXTERNAL and sym.value == offset:
            return sym.name
    return None


class Namer:
    """What symbol, plus what offset, names an address of the original."""

    def __init__(self, img: Image, placer: Placer):
        self.img = img
        self.sections = {name: (start, max(vsize, rsize)) for name, start, vsize, rsize, _ in img.sections}
        self.spans: list[tuple[int, int, str]] = []          # code: (start, end, symbol)
        self.data_spans: list[tuple[int, int, str]] = []     # live data defined outside origdata
        self.gaps: list[tuple[int, int]] = []                # the regions to carve
        self.regions: list[tuple[int, int]] = []             # every gap region, with source or not
        self.library_starts: set[int] = set()
        # Object file name -> (section, offset, name): the public names to add
        # for placed functions that only have static ones (see static_alias).
        self.published: dict[str, list[tuple[int, int, str]]] = defaultdict(list)
        sizes = {}
        for row in load_rows(FUNCTIONS):
            addr, size = int(row["address"], 16), int(row["size"])
            sizes[addr] = size
            if row["kind"] == "gap":
                self.regions.append((addr, size))
                # Built from its source or from library members (the import
                # thunks): named by the objects placed there.
                if addr not in placer.built_regions:
                    self.gaps.append((addr, size))
                    self.spans.append((addr, addr + size, gap_symbol(addr)))
        for p in placer.pieces:
            if p.source == LIBCODE:
                self.library_starts.update(p.va + s.value for s in p.obj.syms.values()
                                           if s.section == p.sec.index and s.sclass == IMAGE_SYM_CLASS_EXTERNAL)
            if p.source == GAPCODE:
                # A gap function, and the asm labels gapcheck made public in it.
                syms = sorted((s.value, s.name) for s in p.obj.syms.values()
                              if s.section == p.sec.index and s.sclass == IMAGE_SYM_CLASS_EXTERNAL
                              and p.lo <= s.value < p.hi)
                for i, (value, name) in enumerate(syms):
                    end = syms[i + 1][0] if i + 1 < len(syms) else p.hi
                    self.spans.append((p.va + value - p.lo, p.va + end - p.lo, name))
                continue
            if p.source in (CODE, LIBCODE) and p.lo == 0:
                # A whole code section: every external symbol in it is where
                # the object puts it (an assembler member holds several).
                syms = sorted((s.value, s.name) for s in p.obj.syms.values()
                              if s.section == p.sec.index and s.sclass == IMAGE_SYM_CLASS_EXTERNAL and s.value < p.hi)
                for i, (value, name) in enumerate(syms):
                    end = syms[i + 1][0] if i + 1 < len(syms) else p.hi
                    self.spans.append((p.va + value, p.va + end, name))
                if p.source == CODE and not any(value == 0 for value, _ in syms):
                    self.publish(p)
                continue
            name = external_at(p, p.lo)
            if name and p.source in (CODE, LIBCODE):
                self.spans.append((p.va, p.va + p.hi - p.lo, name))
            elif p.source == CODE and not p.sec.name.startswith(".text$x"):
                self.publish(p)
            elif name and p.source == LIBDATA:
                self.data_spans.append((p.va, p.va + max(p.hi - p.lo, 1), name))
        for addr, name in img.copied_library:
            if name:
                self.spans.append((addr, addr + sizes.get(addr, 1), name))
        for name, addr in placer.common_at.items():
            self.data_spans.append((addr, addr + max(placer.commons.get(name, 4), 1), name))
        self.spans.sort()
        self.data_spans.sort()
        self.starts = [s for s, _, _ in self.spans]
        self.add_handler_stubs()
        self.data_starts = [s for s, _, _ in self.data_spans]
        self.imports = import_symbols(img)
        self.missing: Counter = Counter()

    def publish(self, p) -> None:
        """Name a placed function whose object has no public name for it."""
        name = static_alias(p.va)
        self.spans.append((p.va, p.va + p.hi - p.lo, name))
        self.published[p.obj.path.name].append((p.sec.index, p.lo, name))

    def add_handler_stubs(self) -> None:
        """The compiler's exception handler stubs for the gap regions' own
        functions (`mov eax, offset FuncInfo; jmp ___CxxFrameHandler`), which the
        original keeps with their unwind code after the runtime library: from
        the first of them to the end of .text, one more region."""
        text_start, text_size = self.sections[".text"]
        text_end = text_start + self.img.pe.sections[0].Misc_VirtualSize
        found = []
        for addr, size in self.gaps:
            code = bytes(self.img.pristine[addr - self.img.base: addr - self.img.base + size])
            for m in re.finditer(rb"\x68(....)", code):
                v = struct.unpack("<I", m.group(1))[0]
                if text_start <= v < text_end and not self.covered(v):
                    stub = bytes(self.img.pristine[v - self.img.base: v - self.img.base + 10])
                    if stub[0] == 0xB8 and stub[5] == 0xE9:
                        found.append(v)
        if found:
            start = min(found)
            if not any(start < s < text_end for s, _, _ in self.spans):
                self.gaps.append((start, text_end - start))
                self.spans.append((start, text_end, gap_symbol(start)))
            else:
                for v in sorted(set(found)):
                    self.gaps.append((v, 10))
                    self.spans.append((v, v + 10, gap_symbol(v)))
        self.gaps.sort()
        self.spans.sort()
        self.starts = [s for s, _, _ in self.spans]

    def covered(self, v: int) -> bool:
        i = bisect.bisect_right(self.starts, v) - 1
        return i >= 0 and self.spans[i][0] <= v < self.spans[i][1]

    def in_gap(self, v: int) -> bool:
        return any(a <= v < a + n for a, n in self.gaps + self.regions)

    def name(self, v: int, origin: str = "") -> tuple[str, int] | None:
        i = bisect.bisect_right(self.starts, v) - 1
        if i >= 0 and self.spans[i][0] <= v < self.spans[i][1]:
            return self.spans[i][2], v - self.spans[i][0]
        if v in self.imports:
            return self.imports[v], 0
        i = bisect.bisect_right(self.data_starts, v) - 1
        if i >= 0 and self.data_spans[i][0] <= v < self.data_spans[i][1]:
            return self.data_spans[i][2], v - self.data_spans[i][0]
        for sec in (".rdata", ".data"):
            start, size = self.sections[sec]
            if start <= v < start + size:
                return data_symbol(sec), v - start
        self.missing[f"{v:#x} from {origin}"] += 1
        return None


# --- the gap regions ----------------------------------------------------------------

def carve_gaps(img: Image, namer: Namer, entries: set[int], learned: dict[str, int],
               out: Path) -> dict[int, str]:
    """gaps.obj: one section per gap region. Returns address -> the C name it
    defines for each entry point the tree calls (FUN_<address>)."""
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    coff = Coff()
    lo_img, hi_img = img.base + 0x1000, img.base + img.size
    defined: dict[int, str] = {}
    section_at: dict[int, int] = {}
    stats = Counter()
    for addr, size in namer.gaps:
        code = bytes(img.pristine[addr - img.base: addr - img.base + size])
        sec = coff.section(".text", code, TEXT_CHARS)
        section_at[addr] = sec
        coff.define(gap_symbol(addr), sec, 0)
        for entry in sorted(e for e in entries if addr <= e < addr + size):
            defined[entry] = f"_FUN_{entry:08x}"
            coff.define(defined[entry], sec, entry - addr)
        pos = 0
        while pos < size:
            ins = next(md.disasm(code[pos:], addr + pos, 1), None)
            if ins is None:
                pos += 1
                stats["undecodable bytes"] += 1
                continue
            b = bytes(ins.bytes)
            rel = 1 if b[0] in (0xE8, 0xE9) and ins.size == 5 else (
                2 if b[0] == 0x0F and 0x80 <= b[1] <= 0x8F and ins.size == 6 else None)
            if rel is not None:
                target = (ins.address + ins.size + struct.unpack_from("<i", b, rel)[0]) & 0xFFFFFFFF
                if not addr <= target < addr + size:
                    named = namer.name(target, f"gap {addr:#x}")
                    if named:
                        # REL32: the linker adds S + A - (P + 4); A is the offset into S.
                        coff.reloc(sec, pos + rel, named[0], REL_REL32, named[1])
                        stats["branches out of their region"] += 1
            else:
                for off, width in ((ins.imm_offset, ins.imm_size), (ins.disp_offset, ins.disp_size)):
                    if width != 4:
                        continue
                    v = struct.unpack_from("<I", b, off)[0]
                    if not lo_img <= v < hi_img:
                        continue
                    if addr <= v < addr + size:
                        coff.reloc(sec, pos + off, gap_symbol(addr), REL_DIR32, v - addr)
                        stats["addresses within their region"] += 1
                        continue
                    named = namer.name(v, f"gap {addr:#x}")
                    if named:
                        coff.reloc(sec, pos + off, named[0], REL_DIR32, named[1])
                        stats["addresses outside their region"] += 1
            pos += ins.size
    # Names the placed library code calls but no object defines, whose code is
    # a gap region: WinMainCRTStartup's _WinMain@16 is the game's WinMain.
    for name, target in learned.items():
        for addr, size in namer.gaps:
            if addr <= target < addr + size:
                coff.define(name, section_at[addr], target - addr)
                defined.setdefault(target, name)
    coff.write(out)
    print("gaps.obj: " + ", ".join(f"{k} {v:,}" for k, v in sorted(stats.items())), file=sys.stderr)
    return defined


# --- the original's data ------------------------------------------------------------

def linker_tables(img: Image) -> list[tuple[int, int]]:
    """Ranges of .rdata and .data the linker builds itself: the import, debug
    and TLS tables, and the .CRT$X* initialiser and terminator tables at the
    start of .data (from the pushes of _cinit and _doexit)."""
    out = list(img.fixed_ranges())
    data_start = next(start for name, start, *_ in img.sections if name == ".data")
    ends = []
    for row in load_rows(FUNCTIONS):
        if row["name"] in ("__cinit", "_doexit"):
            at, size = int(row["address"], 16), int(row["size"])
            code = img.pristine[at - img.base: at - img.base + size]
            for i in range(len(code) - 5):
                if code[i] == 0x68:
                    v = struct.unpack_from("<I", code, i + 1)[0]
                    if data_start <= v < data_start + 0x1000:
                        ends.append(v)
    if ends:
        out.append((data_start, max(ends) + 4 - data_start))
    return out


def carve_data(img: Image, placer: Placer, namer: Namer, compiled_data: dict[int, list[str]],
               more: set[int], out: Path) -> dict[int, str]:
    """origdata.obj: the original's .rdata and .data. Returns address -> the C
    name it defines for each global data/globals.csv lists (and each address
    in `more`)."""
    coff = Coff()
    in_skip = bytearray(img.size)
    for va, size in linker_tables(img):
        for i in range(va - img.base, min(va - img.base + size, img.size)):
            in_skip[i] = 1
    secs = {}
    for name in (".rdata", ".data"):
        start, size = namer.sections[name]
        body = bytearray(img.pristine[start - img.base: start - img.base + size])
        for i in range(size):
            if in_skip[start - img.base + i]:
                body[i] = 0
        secs[name] = (coff.section(name, body, RDATA_CHARS if name == ".rdata" else DATA_CHARS), start, size)
        coff.define(data_symbol(name), secs[name][0], 0)

    # The pointer fields placed pieces declare, and the bytes whose pointers
    # their objects already account for.
    sites: set[int] = set()
    authoritative = bytearray(img.size)
    for p in placer.pieces:
        if p.source not in (OBJDATA, LIBDATA, GLOBALDATA):
            continue
        for off, _, rtype in p.sec.relocs:
            if rtype == REL_DIR32 and p.lo <= off <= p.hi - 4 and placer.pieces_own(p, p.va + off - p.lo):
                sites.add(p.va + off - p.lo)
        if p.source in (OBJDATA, LIBDATA):
            for i in range(p.va - img.base, p.va - img.base + p.hi - p.lo):
                if 0 <= i < img.size and img.owner[i] == p.id:
                    authoritative[i] = 1
    # Elsewhere: the exact start of something, or of a string.
    starts = {int(r["address"], 16) for r in load_rows(FUNCTIONS)}
    starts |= {int(r["address"], 16) for r in load_rows(GLOBALS)}
    starts |= {p.va for p in placer.pieces}
    starts |= set(placer.symbols.values())
    lo_img, hi_img = img.base + 0x1000, img.base + img.size
    data_lo = namer.sections[".rdata"][0]

    def string_start(v: int) -> bool:
        # After a NUL, at least two printable characters, then a NUL.
        if not data_lo <= v < hi_img or img.pristine[v - img.base - 1] != 0:
            return False
        text = bytes(img.pristine[v - img.base: v - img.base + 256]).split(b"\0", 1)[0]
        return len(text) >= 2 and all(32 <= c < 127 or c in (9, 10, 13) for c in text)

    # Aligned dwords that are pointers whatever they hold: the elements of the
    # globals data/globals.csv types as pointers (DAT_0050a788's GUID
    # pointers), and the compiler's exception tables (.xdata$x).
    pointer_slots: set[int] = set()
    for r in load_rows(GLOBALS):
        if "*" in r["type"] and "(" not in r["type"]:
            addr, size = int(r["address"], 16), int(r["size"])
            pointer_slots.update(range(addr, addr + size - 3, 4))
    xdata = [p for p in placer.pieces if p.sec.name.startswith(".xdata")]
    if xdata:
        lo = min(p.va for p in xdata)
        pointer_slots.update(range(lo & ~3, max(p.va + p.hi - p.lo for p in xdata), 4))

    stats = Counter()
    for name, (sec, start, size) in secs.items():
        last = -4
        for va in range(start, start + size - 3):
            o = va - img.base
            if in_skip[o] or va < last + 4:
                continue
            v = img.u32(va)
            if not lo_img <= v < hi_img:
                continue
            if va in sites:
                kind = "pointer fields of placed data"
            elif any(authoritative[o:o + 4]):
                continue
            elif va % 4 == 0 and (v in starts or namer.in_gap(v) or string_start(v) or va in pointer_slots):
                kind = "pointers found in data no object defines"
            elif va % 4 and (v in starts or string_start(v)):
                # Packed records (#pragma pack(1)) keep pointers anywhere:
                # 0x43bc90's 25-byte order records hold a callback at +4.
                kind = "unaligned pointers found in data no object defines"
            else:
                continue
            named = namer.name(v, f"data {va:#x}")
            if named:
                coff.reloc(sec, va - start, named[0], REL_DIR32, named[1])
                stats[kind] += 1
                last = va
    # Names: every global's C name, and every name a compiled object defines
    # at a global's or a placed vtable's address.
    defined: dict[int, str] = {}
    globals_at = {int(r["address"], 16) for r in load_rows(GLOBALS)} | more
    for addr in sorted(globals_at | set(compiled_data)):
        at = next(((sec, addr - start) for sec, start, size in secs.values() if start <= addr < start + size),
                  None)
        if at is None:
            continue
        if addr in globals_at:
            defined[addr] = f"_DAT_{addr:08x}"
            coff.define(defined[addr], *at)
        for other in compiled_data.get(addr, ()):
            coff.define(other, *at)
            stats["compiled definitions taken over"] += 1
    coff.write(out)
    print("origdata.obj: " + ", ".join(f"{k} {v:,}" for k, v in sorted(stats.items())), file=sys.stderr)
    return defined


# --- what the tree's own references mean --------------------------------------------

@dataclass
class Copy:
    """A copy of a placed game function in another file."""
    obj: Obj
    sec: Sec
    size: int


def find_copies(img: Image, placer: Placer) -> list[tuple[Copy, object]]:
    """Copies of placed game functions that other files keep so that they
    inline (docs/consolidation.md) and whose code is the placed function's
    byte for byte: (copy, placed piece) pairs."""
    placed_at = {p.va: p for p in placer.pieces if p.source == CODE and not p.obj.library}
    placed_ids = {(id(p.obj), p.sec.index) for p in placer.pieces}
    out = []
    for obj in placer.objects:
        if obj.library:
            continue
        for name, sym in obj.externals.items():
            sec = obj.secs[sym.section - 1]
            if not sec.is_code or sym.value or (id(obj), sec.index) in placed_ids:
                continue
            va = placer.globals.get(name)
            piece = placed_at.get(va) if va is not None else None
            if piece is None or piece.lo != 0:
                continue
            size = piece.hi
            if len(sec.data) < size or sec.data[size:].strip(b"\x90\xcc"):
                continue
            mine = sorted((off, rtype) for off, _, rtype in sec.relocs if off < size)
            if mine != sorted((off, rtype) for off, _, rtype in piece.sec.relocs if off < size):
                continue
            fixed = bytearray(b"\1" * size)
            for off, _ in mine:
                fixed[off:off + 4] = b"\0\0\0\0"
            if all(not f or a == b for a, b, f in zip(sec.data[:size], piece.sec.data[:size], fixed)):
                out.append((Copy(obj, sec, size), piece))
    return out


def site_targets(img: Image, sec: Sec, off: int, rtype: int, site: int) -> int:
    """The address the original's field at site points at, with sec's addend
    at off taken out."""
    (ours,) = struct.unpack_from("<I", sec.data, off)
    theirs = img.u32(site)
    return ((theirs - ours) if rtype == REL_DIR32 else (site + 4 + theirs - ours)) & 0xFFFFFFFF


def retargets(img: Image, placer: Placer, namer: Namer,
              copies: list) -> dict[str, list[tuple[int, int, str, int]]]:
    """Object file name -> [(section, relocation offset, symbol, addend)]: the
    references of placed functions (and of byte-identical copies of them) to
    file statics and to other functions of their own file, pointed where the
    original's code points.

    Many files keep a global as a file-scope `static` because that is what
    makes their function match (0x4223e0.cpp's `static FeatureList*
    DAT_00511fb4`), and function statics are file-local too: linked as they
    are, each object would get a copy of its own, and a function would never
    see what another one stored. And a call into another function of the same
    file reaches the file's own copy of it, kept so that it inlines, where the
    original calls the real one."""
    out: dict[str, list] = defaultdict(list)
    data_lo = namer.sections[".rdata"][0]
    data_hi = namer.sections[".data"][0] + namer.sections[".data"][1]
    work = [(p.obj, p.sec, p.lo, p.hi, p.va) for p in placer.pieces
            if p.source in (CODE, GAPCODE) and not p.obj.library]
    work += [(c.obj, c.sec, 0, c.size, piece.va - piece.lo) for c, piece in copies]
    for obj, sec, lo, hi, va in work:
        for off, symidx, rtype in sec.relocs:
            if rtype not in (REL_DIR32, REL_REL32) or not lo <= off <= hi - 4:
                continue
            sym = obj.syms[symidx]
            if sym.section <= 0 or sym.section == sec.index:
                continue
            target_sec = obj.secs[sym.section - 1]
            site = va + off - lo
            (ours,) = struct.unpack_from("<I", sec.data, off)
            if target_sec.is_code:
                target = site_targets(img, sec, off, rtype, site)
                if placer.address_in(obj, sym.section, sym.value) == target:
                    continue             # its own code, placed where the original's is (an EH stub)
                named = namer.name(target, f"call to {sym.name}")
                if named:
                    out[obj.path.name].append((sec.index, off, named[0], ours + named[1]))
            elif (sym.sclass == IMAGE_SYM_CLASS_STATIC and rtype == REL_DIR32
                  and target_sec.chars & SCN_MEM_WRITE and data_lo <= img.u32(site) < data_hi):
                named = namer.name(img.u32(site), f"static {sym.name}")
                if named:
                    out[obj.path.name].append((sec.index, off, named[0], named[1]))
    return out


def private_copies(placer: Placer) -> dict[str, set[int]]:
    """Object file name -> the symbols to make static: every other file's
    definition of an annotated game function, so that LINK binds the name to
    the annotated one (it keeps the first COMDAT copy it meets, and the copies'
    bodies and callees can differ). Each file still has its own copy."""
    annotated = {row["symbol"]: Path(row["file"]).stem for row in load_rows(PROGRESS)}
    out: dict[str, set[int]] = defaultdict(set)
    for obj in placer.objects:
        if obj.library:
            continue
        for name, sym in obj.externals.items():
            if name in annotated and annotated[name] != obj.path.stem and obj.secs[sym.section - 1].is_code:
                out[obj.path.name].add(sym.index)
    return out


# --- the whole step -----------------------------------------------------------------

@dataclass
class Carved:
    objects: list[Path]                          # origdata.obj, gaps.obj
    gap_sources: list[Path]                      # the objects of the gap regions built from source
    gap_names: dict[int, str]                    # gap entry point -> the name gaps.obj defines
    data_names: dict[int, str]                   # global's address -> the name origdata.obj defines
    library_at: dict[int, str]                   # runtime library function's address -> its name
    aliases: dict[str, str]                      # names no object defines -> what they mean
    retargets: dict[str, list] = field(default_factory=dict)
    private: dict[str, set[int]] = field(default_factory=dict)
    published: dict[str, list] = field(default_factory=dict)


def carve(objects: list[Path], verbose: bool = False) -> Carved:
    """Write gaps.obj and origdata.obj, and work out what the tree's own
    references mean (see the module docstring)."""
    img, placer = layout()
    namer = Namer(img, placer)
    gap_sources = [g.path for _, g in sorted(placer.gap_regions.items())]
    infos = [read_object(str(o), o.read_bytes()) for o in objects + gap_sources]
    entries = {a for a, _ in namer.gaps}
    compiled_data: dict[int, list[str]] = defaultdict(list)
    for info in infos:
        for name in info.refs:
            a = address_of(name, placer.symbols)
            if a is not None and namer.in_gap(a):
                entries.add(a)
        for name, d in info.defs.items():
            # Thread-local variables stay in the image's .tls section: code
            # reaches them by their offset in it (SECREL), not their address.
            if not d.code and not d.section.startswith(".tls"):
                a = address_of(name, placer.symbols)
                if a is not None and a >= namer.sections[".rdata"][0]:
                    compiled_data[a].append(name)
    # Vtables too: the tree's are partial views of their classes (some slots
    # name a base class's method where the original has the override).
    for p in placer.pieces:
        if p.source == OBJDATA:
            name = external_at(p, p.lo)
            if name and name.startswith("??_7") and name not in compiled_data.get(p.va, ()):
                compiled_data[p.va].append(name)

    gap_names = carve_gaps(img, namer, entries, placer.learned, OUT / "gaps.obj")
    copies = find_copies(img, placer)
    # What each name the game code uses means: the address the original's code
    # holds wherever it is used, in a placed function or an identical copy.
    for copy, piece in copies:
        for off, symidx, rtype in copy.sec.relocs:
            sym = copy.obj.syms[symidx]
            if (sym.sclass == IMAGE_SYM_CLASS_EXTERNAL or sym.section == 0) and off + 4 <= copy.size:
                site = piece.va + off - piece.lo
                placer.ref_targets[sym.name][site_targets(img, copy.sec, off, rtype, site)] += 1
    defined_names = {n for info in infos for n in info.defs}
    truth: dict[str, tuple[str, int] | int] = {}
    for name, targets in placer.ref_targets.items():
        if name in defined_names:
            continue
        target = targets.most_common(1)[0][0]
        named = namer.name(target, f"the name {name}")
        if named and named[0].startswith("__orig"):
            truth[name] = target                 # data: origdata.obj names it
        elif named and named[1] == 0 and named[0] != name:
            truth[name] = named
    data_names = carve_data(img, placer, namer, compiled_data,
                            {t for t in truth.values() if isinstance(t, int)}, OUT / "origdata.obj")
    aliases = {n: (t[0] if isinstance(t, tuple) else data_names[t]) for n, t in truth.items()
               if isinstance(t, tuple) or t in data_names}
    carved = Carved([OUT / "origdata.obj", OUT / "gaps.obj"], gap_sources, gap_names, data_names,
                    {start: name for start, _, name in namer.spans if start in namer.library_starts},
                    aliases, retargets(img, placer, namer, copies), private_copies(placer),
                    dict(namer.published))
    print(f"carve: {len(namer.gaps):,} regions carved, {len(gap_sources):,} gap regions built from source; "
          f"{len(copies):,} identical copies of placed functions, {len(aliases):,} names "
          f"resolved by the layout", file=sys.stderr)
    if namer.missing:
        print(f"carve: {sum(namer.missing.values()):,} references to {len(namer.missing):,} addresses no "
              f"symbol names (left as they are)", file=sys.stderr)
        for what, n in namer.missing.most_common(None if verbose else 10):
            print(f"  {what} x{n}", file=sys.stderr)
    return carved


def library_aliases(objects: list[Path], symbols: dict[str, int], carved: Carved) -> dict[str, str]:
    """The layout's aliases, plus caller spellings of runtime library and zlib
    functions as game functions (FUN_004d1c80, ?_uncompress@@YGHPAEPAK0K@Z) ->
    the library's own name."""
    out = dict(carved.aliases)
    for o in objects:
        info = read_object(str(o), o.read_bytes())
        for name, is_func in info.refs.items():
            if not is_func or name in out:
                continue
            a = address_of(name, symbols)
            if a in carved.library_at and carved.library_at[a] != name:
                out[name] = carved.library_at[a]
    return out


def patch_objects(paths: list[Path], carved: Carved) -> list[Path]:
    """Copies, under build/link/objs/, of the objects with carved.retargets,
    carved.private and carved.published applied."""
    out_dir = OUT / "objs"
    out_dir.mkdir(parents=True, exist_ok=True)
    result, count, private, published = [], 0, 0, 0
    for path in paths:
        sites = carved.retargets.get(path.name, [])
        hidden = carved.private.get(path.name, set())
        names = carved.published.get(path.name, [])
        if not sites and not hidden and not names:
            result.append(path)
            continue
        data = bytearray(path.read_bytes())
        _, _, _, symptr, nsyms, opthdr, _ = struct.unpack_from("<HHIIIHH", data, 0)
        strtab = bytearray(data[symptr + nsyms * 18:])
        new_syms = bytearray()
        index: dict[str, int] = {}
        # Public names for its static functions first, then the names its
        # retargeted references need (undefined, unless it defines them).
        for sec, value, name in names:
            raw = name.encode("latin-1")
            new_syms += b"\0\0\0\0" + struct.pack("<I", len(strtab))
            strtab += raw + b"\0"
            new_syms += struct.pack("<IhHBB", value, sec, 0x20, IMAGE_SYM_CLASS_EXTERNAL, 0)
            index[name] = nsyms + len(index)
            published += 1
        for _, _, name, _ in sites:
            if name in index:
                continue
            raw = name.encode("latin-1")
            if len(raw) <= 8:
                name_field = raw.ljust(8, b"\0")
            else:
                name_field = b"\0\0\0\0" + struct.pack("<I", len(strtab))
                strtab += raw + b"\0"
            new_syms += name_field + struct.pack("<IhHBB", 0, 0, 0, IMAGE_SYM_CLASS_EXTERNAL, 0)
            index[name] = nsyms + len(index)
        struct.pack_into("<I", strtab, 0, len(strtab))
        for sec, off, name, addend in sites:
            hdr = 20 + opthdr + (sec - 1) * 40
            rawptr, relptr = struct.unpack_from("<II", data, hdr + 20)
            nrel = struct.unpack_from("<H", data, hdr + 32)[0]
            for r in range(nrel):
                roff = relptr + r * 10
                if struct.unpack_from("<I", data, roff)[0] == off:
                    struct.pack_into("<I", data, roff + 4, index[name])
                    struct.pack_into("<i", data, rawptr + off, addend)
                    count += 1
                    break
        for symidx in hidden:
            data[symptr + symidx * 18 + 16] = IMAGE_SYM_CLASS_STATIC
            private += 1
        struct.pack_into("<I", data, 12, nsyms + len(index))
        target = out_dir / path.name
        target.write_bytes(bytes(data[:symptr + nsyms * 18]) + bytes(new_syms) + bytes(strtab))
        result.append(target)
    print(f"objects: {count:,} references pointed where the original's point, {private:,} copies of game "
          f"functions made static, {published:,} static game functions given public names", file=sys.stderr)
    return result


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--verbose", "-v", action="store_true")
    args = ap.parse_args()
    from link import compile_all
    objects, failed = compile_all(None)
    if failed:
        raise SystemExit(f"{len(failed)} file(s) did not compile")
    carved = carve(objects, args.verbose)
    print(f"{len(carved.gap_names):,} gap entry points, {len(carved.data_names):,} globals; "
          + ", ".join(str(p.relative_to(ROOT)) for p in carved.objects))


if __name__ == "__main__":
    main()
