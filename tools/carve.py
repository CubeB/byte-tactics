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
    no matching source yet (tools/gapcheck.py; a region whose
    source matches is linked from its own object), and the exception handler
    stubs of their functions, one section each. Its
    relocations come from disassembly: every rel32 branch that leaves its
    region, and every 32-bit immediate or displacement that holds an address
    in the image.
  * origdata.obj holds the runs of the original's .rdata and .data that no
    object defines (the bytes tools/place.py copies, but for runs of zeros
    nothing refers to) and what those runs point at that no symbol names,
    each run a section of its own named for its address. The game's data
    itself comes from source: link/data.cpp, the data files and the tree's own
    objects. Its
    relocations come from tools/place.py's layout: every pointer field of a
    placed piece of data; in data no object defines, every dword that holds
    the exact address of a function, a global, a placed piece or a string
    (unaligned too: packed records keep pointers anywhere), every element of
    a global data/globals.csv types as a pointer, and every address in the
    compiler's exception tables.

Every address a relocation points at is named from the same layout: a game
function by the symbol its object defines, a runtime library function or
global by its library name, an import by its __imp_ symbol, a global by the
symbol of what the layout placed there (a static by a public name its object
gets), and the gap regions and the original's data by the symbols these two
objects define.

The layout also says what every reference in the tree's own objects means,
which tools/link.py --carve applies to copies of them (patch_objects):

  * a name no object defines is aliased to the symbol at the address the
    original's code holds wherever the name is used (in a placed function, or
    in a byte-identical copy of one another file keeps so that it inlines);
  * a name that points inside a global (a field, an entry) has its
    references pointed at that global with the offset added;
  * every other definition of a placed global or vtable (another file's
    view of the same class, another spelling of a template's static) becomes
    a static nothing uses, its file's references going to the placed one;
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
from place import (CODE, COPIED, DATASRC, FUNCTIONS, GAPCODE, GLOBALDATA, GLOBALS, LIBCODE, LIBDATA,
                   OBJDATA, PROGRESS, REL_DIR32, REL_REL32, Image, Obj, Placer, Sec, layout, load_rows,
                   undecorate)

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

    def section(self, name: str, data: bytes, chars: int, size: int | None = None) -> int:
        """A section; an uninitialised one (chars has 0x80) has only a size."""
        self.sections.append({"name": name, "data": bytearray(data), "chars": chars, "relocs": [],
                              "size": len(data) if size is None else size})
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
            raw = sec["name"].encode()
            if len(raw) > 8:          # a long name lives in the string table: "/<offset>"
                raw = f"/{4 + len(strings)}".encode()
                strings.extend(sec["name"].encode() + b"\0")
            uninit = sec["chars"] & 0x80
            headers += raw.ljust(8, b"\0") + struct.pack(
                "<IIIIIIHHI", 0, 0, sec["size"], 0 if uninit else pos,
                pos + (0 if uninit else len(data)) if nrel else 0, 0, nrel, 0, sec["chars"])
            if not uninit:
                bodies += data
                pos += len(data)
            bodies += relocs
            pos += len(relocs)
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


def data_symbol(start: int) -> str:
    """The name origdata.obj gives the run of the original's data at start."""
    return f"__orig_{start:08x}"


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
    """What symbol, plus what offset, names an address of the original.

    Data is named by what the layout placed there: a global defined by source
    (link/, a data file, a tree file), a string literal or vtable by its
    external name, a static by a public name patch_objects adds to its
    object (static_alias), library data by its name. What none of those
    names is still the original's bytes: origdata.obj holds those runs
    (set_kept), and an address outside them is recorded in `wanted`, for
    carve() to add to the runs and try again."""

    def __init__(self, img: Image, placer: Placer):
        self.img = img
        self.placer = placer
        self.kept: list[tuple[int, int]] = []                # origdata.obj's runs: (start, end)
        self.kept_starts: list[int] = []
        self.wanted: set[int] = set()                        # data addresses no run or symbol covers
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
            elif name and p.source in (LIBDATA, OBJDATA, GLOBALDATA, DATASRC):
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

    def publish(self, p) -> str:
        """Name a placed function, or a placed piece of data, whose object has
        no public name for it."""
        name = static_alias(p.va)
        if p.sec.is_code:
            self.spans.append((p.va, p.va + p.hi - p.lo, name))
        if (p.sec.index, p.lo, name) not in self.published[p.obj.path.name]:
            self.published[p.obj.path.name].append((p.sec.index, p.lo, name))
        return name

    def set_kept(self, runs: list[tuple[int, int]]) -> None:
        self.kept = sorted(runs)
        self.kept_starts = [a for a, _ in self.kept]
        self.wanted = set()

    def in_kept(self, v: int) -> tuple[int, int] | None:
        i = bisect.bisect_right(self.kept_starts, v) - 1
        if i >= 0 and self.kept[i][0] <= v < self.kept[i][1]:
            return self.kept[i]
        return None

    def data_piece(self, v: int):
        """The placed piece of data that holds v in the layout, if any."""
        pid = self.img.owner[v - self.img.base]
        if not pid:
            return None
        p = self.placer.pieces[pid - 1]
        return None if p.sec.is_code else p

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
        run = self.in_kept(v)
        if run:
            return data_symbol(run[0]), v - run[0]
        p = self.data_piece(v) if self.img.inside(v) else None
        if p is not None:
            name = external_at(p, p.lo)
            if name:
                return name, v - p.va
            if p.source in (OBJDATA, DATASRC) and not p.obj.library:
                # A static (a file-scope `static` table, a function's static):
                # its object gets a public name for it.
                return self.publish(p), v - p.va
        i = bisect.bisect_right(self.data_starts, v) - 1
        if i >= 0 and self.data_spans[i][0] <= v < self.data_spans[i][1]:
            return self.data_spans[i][2], v - self.data_spans[i][0]
        for sec in (".rdata", ".data"):
            start, size = self.sections[sec]
            if start <= v < start + size:
                self.wanted.add(v)
                return None
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


def run_chars(section: str, start: int) -> int:
    """Section characteristics for a run of the original's data: its section's
    access, and the alignment its address has (up to 16 bytes)."""
    align = 16
    while start % align:
        align //= 2
    return (RDATA_CHARS if section == ".rdata" else DATA_CHARS) & ~0x00F00000 | (align.bit_length() << 20)


def carve_data(img: Image, placer: Placer, namer: Namer, more: set[int], out: Path) -> dict[int, str]:
    """origdata.obj: the runs of the original's .rdata and .data that nothing
    else defines (namer.kept), one section each. Returns address -> the C name
    it defines for each global data/globals.csv lists (and each address in
    `more`) that falls in a run."""
    coff = Coff()
    in_skip = bytearray(img.size)
    for va, size in linker_tables(img):
        for i in range(va - img.base, min(va - img.base + size, img.size)):
            in_skip[i] = 1
    runs = []
    rdata_end = namer.sections[".rdata"][0] + namer.sections[".rdata"][1]
    data_raw_end = next(start + rsize for name, start, vsize, rsize, _ in img.sections if name == ".data")
    for start, end in namer.kept:
        body = bytearray(img.pristine[start - img.base: end - img.base])
        for i in range(len(body)):
            if in_skip[start - img.base + i]:
                body[i] = 0
        section = ".rdata" if start < rdata_end else ".data"
        # Named for its address, so that LINK puts it among the game's data
        # in the original's order (tools/coffsplit.py does the same for the
        # objects); a run past .data's raw bytes is uninitialised.
        if start >= data_raw_end:
            idx = coff.section(f".bss${start:08x}", b"", (run_chars(section, start) | 0x80) & ~0x40,
                               end - start)
        else:
            idx = coff.section(f"{section}${start:08x}", body, run_chars(section, start))
        coff.define(data_symbol(start), idx, 0)
        runs.append((idx, start, end))

    def run_of(addr: int):
        for idx, start, end in runs:
            if start <= addr < end:
                return idx, addr - start
        return None

    # The pointer fields placed pieces declare, and the bytes whose pointers
    # their objects already account for.
    sites: set[int] = set()
    authoritative = bytearray(img.size)
    for p in placer.pieces:
        if p.source not in (OBJDATA, LIBDATA, GLOBALDATA, DATASRC):
            continue
        for off, _, rtype in p.sec.relocs:
            if rtype == REL_DIR32 and p.lo <= off <= p.hi - 4 and placer.pieces_own(p, p.va + off - p.lo):
                sites.add(p.va + off - p.lo)
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
    # globals data/globals.csv types as pointers, and the compiler's exception
    # tables (.xdata$x).
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
    for sec, start, end in runs:
        last = -4
        for va in range(start, end - 3):
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
    # Names: the C name of every global in a run.
    defined: dict[int, str] = {}
    for addr in sorted({int(r["address"], 16) for r in load_rows(GLOBALS)} | more):
        at = run_of(addr)
        if at is not None:
            defined[addr] = f"_DAT_{addr:08x}"
            coff.define(defined[addr], *at)
    coff.write(out)
    total = sum(end - start for _, start, end in runs)
    print(f"origdata.obj: {total:,} bytes in {len(runs):,} runs; "
          + ", ".join(f"{k} {v:,}" for k, v in sorted(stats.items())), file=sys.stderr)
    return defined


def kept_runs(img: Image, placer: Placer, wanted: set[int],
              runs: list[tuple[int, int]] | None = None) -> list[tuple[int, int]]:
    """The runs of the original's data origdata.obj holds: the bytes the
    layout copied (nothing defines them) but for runs of zeros, and the
    pieces holding the addresses in `wanted`, merged."""
    keep = bytearray(img.size)
    for start, end in runs or ():
        keep[start - img.base:end - img.base] = b"\1" * (end - start)
    lo = next(start for name, start, *_ in img.sections if name == ".rdata") - img.base
    data = next(s for s in img.sections if s[0] == ".data")
    hi = data[1] + max(data[2], data[3]) - img.base
    if runs is None:
        tables = bytearray(img.size)
        for va, size in linker_tables(img):
            tables[va - img.base:va - img.base + size] = b"\1" * size
        for i in range(lo, hi):
            if img.src[i] == COPIED and not tables[i]:
                keep[i] = 1
        # A run of zeros needs no definition until something refers to it
        # (`wanted`): LINK lays the library members' data out itself, and
        # the zeros between two pieces of the game's data hold nothing.
        i = lo
        while i < hi:
            if keep[i]:
                j = i
                while j < hi and keep[j]:
                    j += 1
                if not any(img.pristine[i:j]):
                    keep[i:j] = bytes(j - i)
                i = j
            else:
                i += 1
    for v in wanted:
        o = v - img.base
        pid = img.owner[o]
        if pid:
            p = placer.pieces[pid - 1]
            keep[p.va - img.base:p.va - img.base + p.hi - p.lo] = b"\1" * (p.hi - p.lo)
        else:
            # A run of bytes of one kind (copied, padding) around it.
            a, b = o, o + 1
            while a > lo and img.src[a - 1] == img.src[o] and not img.owner[a - 1]:
                a -= 1
            while b < hi and img.src[b] == img.src[o] and not img.owner[b]:
                b += 1
            keep[a:b] = b"\1" * (b - a)
    out = []
    i = lo
    while i < hi:
        if keep[i]:
            j = i
            while j < hi and keep[j]:
                j += 1
            out.append((img.base + i, img.base + j))
            i = j
        else:
            i += 1
    return out


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
                at = sym.value + (ours if sym.name.startswith(".") else 0)
                own = placer.address_in(obj, sym.section, at)
                if own is not None and own + (0 if sym.name.startswith(".") else ours) == img.u32(site):
                    continue             # its own static, placed where the original's is
                named = namer.name(img.u32(site), f"static {sym.name}")
                if named:
                    out[obj.path.name].append((sec.index, off, named[0], named[1]))
    return out


def second_definitions(img: Image, placer: Placer, namer: Namer
                       ) -> tuple[dict[str, set[int]], dict[str, list[tuple[int, int, str, int]]]]:
    """Every other definition of a global the layout placed from one file:
    the same name in another file (0x460f60's smaller view of DAT_00513000's
    class), or another spelling of it (the `_Nil` node of a std::map that two
    files instantiate with different views of its value type). Linked as they
    are, each would be a variable of its own, and a map whose `_Nil` one file
    set up would be empty in another. A vtable is one too: LINK would keep
    the first file's copy, which can be another file's view of the class.
    Returns object file name -> the symbols to make static, and -> the
    references to point at the placed definition."""
    hide: dict[str, set[int]] = defaultdict(set)
    sites: dict[str, list] = defaultdict(list)
    placed_ids = {(id(p.obj), p.sec.index, p.lo) for p in placer.pieces}
    vtables = {}
    for p in placer.pieces:
        name = external_at(p, p.lo) if p.source == OBJDATA else None
        if name and name.startswith("??_7"):
            vtables.setdefault(name, p.va)
    for obj in placer.objects:
        if obj.library or obj.data:
            continue
        mine = {}
        for name, sym in obj.externals.items():
            sec = obj.secs[sym.section - 1]
            if sec.is_code or name.startswith(("??_C@", "__real@", "__TI", "__CT", "__CTA")) \
                    or sec.name.startswith((".tls", ".CRT", ".xdata")):
                continue
            addr = vtables.get(name) if name.startswith("??_7") else address_of(name, placer.symbols)
            if addr is None or not namer.sections[".rdata"][0] <= addr:
                continue
            if (id(obj), sym.section, sec.slice_at(sym.value)[0]) in placed_ids:
                continue                       # the definition the layout placed
            named = namer.name(addr, f"the global {name}")
            if not named or named[1] or named[0].startswith("__orig_"):
                continue
            hide[obj.path.name].add(sym.index)
            mine[sym.index] = named[0]
        if not mine:
            continue
        for sec in obj.secs:
            for off, symidx, rtype in sec.relocs:
                if symidx in mine and rtype == REL_DIR32 and off + 4 <= len(sec.data):
                    (addend,) = struct.unpack_from("<i", sec.data, off)
                    sites[obj.path.name].append((sec.index, off, mine[symidx], addend))
    return hide, sites


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
    inner: dict[str, tuple[str, int]] = field(default_factory=dict)   # name -> (global, offset into it)
    references: dict[str, set[int]] = field(default_factory=dict)     # data definitions to make references
    # Object file name -> (section, lo, hi, address) of each piece of data the
    # layout placed from it: order_data names their sections by address.
    data_pieces: dict[str, list] = field(default_factory=dict)


def carve(objects: list[Path], verbose: bool = False) -> Carved:
    """Write gaps.obj and origdata.obj, and work out what the tree's own
    references mean (see the module docstring)."""
    img, placer = layout()
    namer = Namer(img, placer)
    gap_sources = [g.path for _, g in sorted(placer.gap_regions.items())]
    infos = [read_object(str(o), o.read_bytes()) for o in objects + gap_sources]
    entries = {a for a, _ in namer.gaps}
    for info in infos:
        for name in info.refs:
            a = address_of(name, placer.symbols)
            if a is not None and namer.in_gap(a):
                entries.add(a)

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
    # Data names no object defines that no placed code uses (an unplaced
    # copy's): what their address holds in the layout.
    data_refs = {n for info in infos for n, is_func in info.refs.items() if not is_func}

    # origdata.obj holds what no object defines (the bytes the layout
    # copied); whatever any of the references below still needs from the
    # original joins it, until nothing more is needed.
    runs = kept_runs(img, placer, set())
    for _ in range(8):
        namer.set_kept(runs)
        gap_names = carve_gaps(img, namer, entries, placer.learned, OUT / "gaps.obj")
        truth: dict[str, tuple[str, int] | int] = {}
        inner: dict[str, tuple[str, int]] = {}
        targets = {n: t.most_common(1)[0][0] for n, t in placer.ref_targets.items()}
        for name in data_refs:
            if name not in targets:
                a = address_of(name, placer.symbols)
                if a is not None and namer.sections[".rdata"][0] <= a:
                    targets[name] = a
        for name, target in targets.items():
            if name in defined_names:
                continue
            named = namer.name(target, f"the name {name}")
            if named and named[0].startswith("__orig_"):
                truth[name] = target             # data: origdata.obj names it
            elif named and named[0] in placer.commons:
                # A communal variable (the guard 0x463ba0 and string.obj's _$E50
                # share): its references are pointed at it, since LINK 5.10
                # crashes on a weak external whose default is a communal symbol.
                inner[name] = named
            elif named and named[1] == 0 and named[0] != name:
                truth[name] = named
            elif named and named[1] and not namer.covered(target):
                inner[name] = named              # a field or an entry of a global
        data_names = carve_data(img, placer, namer, {t for t in truth.values() if isinstance(t, int)},
                                OUT / "origdata.obj")
        moved = retargets(img, placer, namer, copies)
        if not namer.wanted:
            break
        runs = kept_runs(img, placer, namer.wanted, runs)
    aliases = {n: (t[0] if isinstance(t, tuple) else data_names[t]) for n, t in truth.items()
               if isinstance(t, tuple) or t in data_names}
    # One definition of each global: the others become statics nothing uses,
    # their files' references going to the placed one.
    private = private_copies(placer)
    hide, shared = second_definitions(img, placer, namer)
    for name, symbols in hide.items():
        private[name] = private.get(name, set()) | symbols
    for name, more in shared.items():
        moved[name] = moved.get(name, []) + more
    carved = Carved([OUT / "origdata.obj", OUT / "gaps.obj"], gap_sources, gap_names, data_names,
                    {start: name for start, _, name in namer.spans if start in namer.library_starts},
                    aliases, moved, private, dict(namer.published), inner, {}, data_pieces(placer))
    print(f"carve: {len(namer.gaps):,} regions carved, {len(gap_sources):,} gap regions built from source; "
          f"{len(copies):,} identical copies of placed functions, {len(aliases):,} names "
          f"resolved by the layout, {len(inner):,} as parts of globals", file=sys.stderr)
    if namer.wanted:
        print(f"carve: {len(namer.wanted):,} addresses of data still unnamed", file=sys.stderr)
    if namer.missing:
        print(f"carve: {sum(namer.missing.values()):,} references to {len(namer.missing):,} addresses no "
              f"symbol names (left as they are)", file=sys.stderr)
        for what, n in namer.missing.most_common(None if verbose else 10):
            print(f"  {what} x{n}", file=sys.stderr)
    return carved


def data_pieces(placer: Placer) -> dict[str, list[tuple[int, int, int, int]]]:
    out: dict[str, list] = defaultdict(list)
    for p in placer.pieces:
        if not p.sec.is_code and not p.obj.library:
            out[p.obj.path.name].append((p.sec.index, p.lo, p.hi, p.va))
    return dict(out)


def order_data(paths: list[Path], carved: Carved) -> list[Path]:
    """Copies, under build/link/ordered/, of the objects with each piece of
    data the layout placed in a section named for its address
    (tools/coffsplit.py): LINK then lays the game's data out in the
    original's order."""
    from coffsplit import rewrite
    out_dir = OUT / "ordered"
    result, named = [], 0
    for path in paths:
        pieces = carved.data_pieces.get(path.name)
        n = rewrite(path, pieces, out_dir / path.name) if pieces else 0
        result.append(out_dir / path.name if n else path)
        named += n
    print(f"data: {named:,} pieces of data in sections named for their addresses", file=sys.stderr)
    return result


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


def inner_sites(data: bytes, inner: dict[str, tuple[str, int]]
                ) -> tuple[list[tuple[int, int, str, int]], dict[int, str]]:
    """(section, offset, global, addend) for every DIR32 relocation of an
    object against a name `inner` maps to a part of a global: the field it
    holds plus the part's offset. Also symbol index -> that global, for the
    names themselves, which nothing refers to once the relocations go to the
    global."""
    if not inner:
        return [], {}
    _, nsects, _, symptr, nsyms, opthdr, _ = struct.unpack_from("<HHIIIHH", data, 0)
    strtab = data[symptr + nsyms * 18:]
    wanted = {}
    i = 0
    while i < nsyms:
        raw = data[symptr + i * 18: symptr + i * 18 + 18]
        value, secnum, _, sclass, naux = struct.unpack_from("<IhHBB", raw, 8)
        if secnum == 0 and sclass == IMAGE_SYM_CLASS_EXTERNAL and not value:
            if raw[:4] == b"\0\0\0\0":
                off = struct.unpack_from("<I", raw, 4)[0]
                name = strtab[off:strtab.index(b"\0", off)].decode("latin-1")
            else:
                name = raw[:8].split(b"\0", 1)[0].decode("latin-1")
            if name in inner:
                wanted[i] = inner[name]
        i += 1 + naux
    out = []
    if not wanted:
        return out, {}
    for sidx in range(nsects):
        hdr = 20 + opthdr + sidx * 40
        rawptr, relptr = struct.unpack_from("<II", data, hdr + 20)
        nrel = struct.unpack_from("<H", data, hdr + 32)[0]
        for r in range(nrel):
            off, symidx, rtype = struct.unpack_from("<IIH", data, relptr + r * 10)
            if symidx in wanted and rtype == REL_DIR32 and rawptr:
                name, delta = wanted[symidx]
                field_ = struct.unpack_from("<i", data, rawptr + off)[0]
                out.append((sidx + 1, off, name, field_ + delta))
    return out, {i: name for i, (name, _) in wanted.items()}


def patch_objects(paths: list[Path], carved: Carved) -> list[Path]:
    """Copies, under build/link/objs/, of the objects with carved.retargets,
    carved.private, carved.published, carved.inner and carved.references
    applied."""
    out_dir = OUT / "objs"
    out_dir.mkdir(parents=True, exist_ok=True)
    result, count, private, published, parts, refs = [], 0, 0, 0, 0, 0
    for path in paths:
        sites = list(carved.retargets.get(path.name, []))
        hidden = carved.private.get(path.name, set())
        names = carved.published.get(path.name, [])
        undefined = carved.references.get(path.name, set())
        data = bytearray(path.read_bytes())
        part_sites, part_names = inner_sites(bytes(data), carved.inner)
        if not sites and not hidden and not names and not undefined and not part_sites:
            result.append(path)
            continue
        taken = {(sec, off) for sec, off, _, _ in sites}
        for site in part_sites:
            if site[:2] not in taken:
                sites.append(site)
                parts += 1
        _, _, _, symptr, nsyms, opthdr, _ = struct.unpack_from("<HHIIIHH", data, 0)
        strtab = bytearray(data[symptr + nsyms * 18:])
        new_syms = bytearray()
        index: dict[str, int] = {}
        # Public names for its statics first, then the names its retargeted
        # references need (undefined, unless it defines them).
        for sec, value, name in names:
            raw = name.encode("latin-1")
            new_syms += b"\0\0\0\0" + struct.pack("<I", len(strtab))
            strtab += raw + b"\0"
            chars = struct.unpack_from("<I", data, 20 + opthdr + (sec - 1) * 40 + 36)[0]
            new_syms += struct.pack("<IhHBB", value, sec, 0x20 if chars & 0x20 else 0, IMAGE_SYM_CLASS_EXTERNAL, 0)
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
        for symidx, name in part_names.items():
            # The name of a part, which no relocation uses now: let it mean
            # the global, so that it needs no definition of its own.
            data[symptr + symidx * 18: symptr + symidx * 18 + 8] = b"\0\0\0\0" + struct.pack("<I", len(strtab))
            strtab += name.encode("latin-1") + b"\0"
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
        for symidx in undefined:
            # A definition of a global another object defines too: now a
            # reference to that one.
            struct.pack_into("<Ih", data, symptr + symidx * 18 + 8, 0, 0)
            refs += 1
        struct.pack_into("<I", data, 12, nsyms + len(index))
        target = out_dir / path.name
        target.write_bytes(bytes(data[:symptr + nsyms * 18]) + bytes(new_syms) + bytes(strtab))
        result.append(target)
    print(f"objects: {count:,} references pointed where the original's point ({parts:,} to parts of globals), "
          f"{private:,} copies of game functions made static, {published:,} statics given public names, "
          f"{refs:,} second definitions of globals made references", file=sys.stderr)
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
