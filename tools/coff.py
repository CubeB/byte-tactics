"""Minimal readers for MSVC COFF objects (.obj) and archives (.lib)."""

import struct
from dataclasses import dataclass, field
from pathlib import Path

IMAGE_SCN_CNT_CODE = 0x20
IMAGE_SCN_LNK_COMDAT = 0x1000

# i386 relocation types that patch a 4-byte field at link time.
REL_I386_DIR32 = 0x06
REL_I386_DIR32NB = 0x07
REL_I386_REL32 = 0x14
RELOC_WIDTH = {REL_I386_DIR32: 4, REL_I386_DIR32NB: 4, REL_I386_REL32: 4, 0x0A: 2, 0x0B: 4}


@dataclass
class Reloc:
    offset: int
    symbol: str
    type: int


@dataclass
class Section:
    index: int  # 1-based, as used by symbol SectionNumber
    name: str
    characteristics: int
    data: bytes
    relocs: list[Reloc] = field(default_factory=list)

    @property
    def is_code(self) -> bool:
        return bool(self.characteristics & IMAGE_SCN_CNT_CODE)

    def mask(self) -> bytes:
        """1 for bytes fixed at compile time, 0 for bytes the linker patches."""
        m = bytearray(b"\x01" * len(self.data))
        for r in self.relocs:
            for i in range(RELOC_WIDTH.get(r.type, 4)):
                if r.offset + i < len(m):
                    m[r.offset + i] = 0
        return bytes(m)


@dataclass
class Symbol:
    name: str
    value: int
    section: int
    storage_class: int


@dataclass
class CoffObject:
    name: str
    sections: list[Section]
    symbols: list[Symbol]

    def symbols_in(self, section: Section) -> list[Symbol]:
        return [s for s in self.symbols if s.section == section.index and s.storage_class in (2, 3) and not s.name.startswith((".", "$"))]


def _cstr(raw: bytes) -> str:
    return raw.split(b"\0", 1)[0].decode("latin-1")


def parse_object(data: bytes, name: str = "") -> CoffObject | None:
    if len(data) < 20:
        return None
    machine, nsects, _, symptr, nsyms, opthdr, _ = struct.unpack_from("<HHIIIHH", data, 0)
    if machine != 0x14C:  # also rejects short import objects (0x0000)
        return None
    strtab = data[symptr + nsyms * 18:] if symptr else b""

    def long_name(raw: bytes) -> str:
        if raw[:4] == b"\0\0\0\0":
            (off,) = struct.unpack_from("<I", raw, 4)
            return _cstr(strtab[off:])
        return _cstr(raw)

    symbols: list[Symbol] = []
    by_index: dict[int, str] = {}
    i = 0
    while i < nsyms:
        raw = data[symptr + i * 18: symptr + i * 18 + 18]
        value, secnum, _, sclass, naux = struct.unpack_from("<IhHBB", raw, 8)
        sym = Symbol(long_name(raw[:8]), value, secnum, sclass)
        by_index[i] = sym.name
        symbols.append(sym)
        i += 1 + naux

    sections = []
    base = 20 + opthdr
    for s in range(nsects):
        hdr = data[base + s * 40: base + s * 40 + 40]
        sname = _cstr(hdr[:8])
        if sname.startswith("/"):
            sname = _cstr(strtab[int(sname[1:]):])
        _, _, size, rawptr, relptr, _, nrel, _, chars = struct.unpack_from("<IIIIIIHHI", hdr, 8)
        body = data[rawptr:rawptr + size] if rawptr else b"\0" * size
        relocs = []
        for r in range(nrel):
            off, symidx, rtype = struct.unpack_from("<IIH", data, relptr + r * 10)
            relocs.append(Reloc(off, by_index.get(symidx, f"#{symidx}"), rtype))
        sections.append(Section(s + 1, sname, chars, body, relocs))
    return CoffObject(name, sections, symbols)


def read_archive(path: Path) -> list[CoffObject]:
    data = path.read_bytes()
    if data[:8] != b"!<arch>\n":
        raise ValueError(f"{path} is not a COFF archive")
    longnames = b""
    objects = []
    pos = 8
    while pos + 60 <= len(data):
        hdr = data[pos:pos + 60]
        name = hdr[:16].decode("latin-1").rstrip()
        size = int(hdr[48:58])
        body = data[pos + 60: pos + 60 + size]
        pos += 60 + size + (size & 1)
        if name == "//":
            longnames = body
        elif name == "/":
            continue
        else:
            if name.startswith("/"):
                off = int(name[1:])
                name = longnames[off:].split(b"\0", 1)[0].split(b"\n", 1)[0].decode("latin-1")
            obj = parse_object(body, name.rstrip("/"))
            if obj:
                objects.append(obj)
    return objects
