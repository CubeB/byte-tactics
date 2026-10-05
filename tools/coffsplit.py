"""Split an object's data sections into one section per placed piece, named
for the original's address, so that LINK lays the game's data out in the
original's order.

LINK sorts the parts of a grouped section (`.data$xxx`) by the part after the
`$` and merges them into `.data`, which is how the C runtime orders its
`.CRT$XCA`..`.CRT$XCZ` tables. Each global the layout (tools/place.py) puts at
an address becomes a section `.data$<address>` (`.rdata$`, `.bss$`), so the
linked image keeps the original's neighbours: code that clears 44 ints with
one memset (0x451fd0) or reads past a global's declared end finds what the
original had there, not whatever the compiler's order put next.

    rewrite(path, pieces, out)    # pieces: (section index, lo, hi, address)
"""

import struct
from dataclasses import dataclass, field
from pathlib import Path

IMAGE_SCN_CNT_CODE = 0x20
IMAGE_SCN_CNT_UNINIT = 0x80
IMAGE_SCN_LNK_COMDAT = 0x1000
IMAGE_SCN_MEM_WRITE = 0x80000000
REL_DIR32, REL_DIR32NB, REL_REL32 = 0x06, 0x07, 0x14
IMAGE_SYM_CLASS_STATIC = 3


@dataclass
class Section:
    name: str
    chars: int
    data: bytes                      # empty for uninitialised data
    size: int
    relocs: list = field(default_factory=list)   # [offset, symbol index, type]


@dataclass
class Symbol:
    name: str
    value: int
    section: int
    type: int
    sclass: int
    aux: list = field(default_factory=list)      # raw 18-byte auxiliary records


def read(data: bytes) -> tuple[int, list[Section], list[Symbol]]:
    machine, nsects, stamp, symptr, nsyms, opthdr, chars = struct.unpack_from("<HHIIIHH", data, 0)
    strtab = data[symptr + nsyms * 18:]

    def long_name(raw: bytes) -> str:
        if raw[:4] == b"\0\0\0\0":
            off = struct.unpack_from("<I", raw, 4)[0]
            return strtab[off:strtab.index(b"\0", off)].decode("latin-1")
        return raw[:8].split(b"\0", 1)[0].decode("latin-1")

    secs = []
    for s in range(nsects):
        hdr = data[20 + opthdr + s * 40: 60 + opthdr + s * 40]
        name = hdr[:8].split(b"\0", 1)[0].decode("latin-1")
        if name.startswith("/"):
            off = int(name[1:])
            name = strtab[off:strtab.index(b"\0", off)].decode("latin-1")
        size, rawptr, relptr, _, nrel, _, schars = struct.unpack_from("<IIIIHHI", hdr, 16)
        body = data[rawptr:rawptr + size] if rawptr else b""
        relocs = [list(struct.unpack_from("<IIH", data, relptr + r * 10)) for r in range(nrel)]
        secs.append(Section(name, schars, body, size, relocs))
    syms: list[Symbol] = []
    i = 0
    while i < nsyms:
        raw = data[symptr + i * 18: symptr + i * 18 + 18]
        value, secnum, typ, sclass, naux = struct.unpack_from("<IhHBB", raw, 8)
        aux = [data[symptr + (i + 1 + k) * 18: symptr + (i + 2 + k) * 18] for k in range(naux)]
        syms.append(Symbol(long_name(raw), value, secnum, typ, sclass, aux))
        i += 1 + naux
    return chars, secs, syms


def write(chars: int, secs: list[Section], syms: list[Symbol], out: Path) -> None:
    strings = bytearray()

    def string(name: str) -> int:
        off = 4 + len(strings)
        strings.extend(name.encode("latin-1") + b"\0")
        return off

    # Symbol indices: each symbol takes 1 + its auxiliary records.
    index, n = [], 0
    for s in syms:
        index.append(n)
        n += 1 + len(s.aux)
    pos = 20 + 40 * len(secs)
    headers, bodies = bytearray(), bytearray()
    for sec in secs:
        raw = sec.name.encode("latin-1")
        name_field = raw.ljust(8, b"\0") if len(raw) <= 8 else f"/{string(sec.name)}".encode().ljust(8, b"\0")
        relocs = b"".join(struct.pack("<IIH", off, index[symidx], rtype) for off, symidx, rtype in sec.relocs)
        rawptr = pos if sec.data else 0
        bodies += sec.data
        pos += len(sec.data)
        relptr = pos if relocs else 0
        bodies += relocs
        pos += len(relocs)
        headers += name_field + struct.pack("<IIIIIIHHI", 0, 0, sec.size, rawptr, relptr, 0,
                                            len(sec.relocs), 0, sec.chars)
    symtab = bytearray()
    for s in syms:
        raw = s.name.encode("latin-1")
        name_field = raw.ljust(8, b"\0") if len(raw) <= 8 else b"\0\0\0\0" + struct.pack("<I", string(s.name))
        symtab += name_field + struct.pack("<IhHBB", s.value, s.section, s.type, s.sclass, len(s.aux))
        for a in s.aux:
            symtab += a
    head = struct.pack("<HHIIIHH", 0x14C, len(secs), 0, pos, n, 0, chars)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_bytes(head + bytes(headers) + bytes(bodies) + bytes(symtab)
                    + struct.pack("<I", 4 + len(strings)) + bytes(strings))


def kind(sec: Section) -> str:
    if sec.chars & IMAGE_SCN_CNT_UNINIT:
        return ".bss"
    return ".data" if sec.chars & IMAGE_SCN_MEM_WRITE else ".rdata"


def rewrite(path: Path, pieces: list[tuple[int, int, int, int]], out: Path) -> int:
    """Write `path` to `out` with every placed piece of data (section index
    from 1, lo, hi, address) in a section of its own named for its address.
    Returns how many sections were named."""
    chars, secs, syms = read(path.read_bytes())
    by_sec: dict[int, list[tuple[int, int, int]]] = {}
    for sidx, lo, hi, va in pieces:
        sec = secs[sidx - 1]
        # Only the ordinary data sections: not code, not the grouped sections
        # LINK orders by name already (.CRT$XCU, .xdata$x, .tls$), and not a
        # COMDAT in parts (its symbol and its partner must stay together).
        if sec.chars & IMAGE_SCN_CNT_CODE or sec.name not in (".data", ".rdata", ".bss"):
            continue
        if sec.chars & IMAGE_SCN_LNK_COMDAT and (lo or min(hi, sec.size) < sec.size):
            continue
        by_sec.setdefault(sidx, []).append((lo, hi, va))
    if not by_sec:
        return 0
    # Each section's parts: cut at every piece's start and end.
    new_secs: list[Section] = []
    remap: dict[int, list[tuple[int, int, int]]] = {}    # old index -> [(lo, hi, new index)]
    named = 0
    for i, sec in enumerate(secs, start=1):
        if i not in by_sec:
            new_secs.append(sec)
            remap[i] = [(0, max(sec.size, 1), len(new_secs))]
            continue
        cuts = {0, sec.size}
        at = {}
        for lo, hi, va in by_sec[i]:
            cuts |= {lo, min(hi, sec.size)}
            at[lo] = va
        cuts = sorted(c for c in cuts if 0 <= c <= sec.size)
        parts = []
        for lo, hi in zip(cuts, cuts[1:]):
            name = f"{kind(sec)}${at[lo]:08x}" if lo in at else sec.name
            named += lo in at
            body = sec.data[lo:hi] if sec.data else b""
            relocs = [[off - lo, s, t] for off, s, t in sec.relocs if lo <= off < hi]
            schars = sec.chars
            if lo in at:
                # No more alignment than the original's address has, or LINK
                # would leave gaps the original does not have.
                n = (schars >> 20) & 0xF
                align = 1 << (n - 1) if n else 16
                while at[lo] % align:
                    align //= 2
                schars = schars & ~0x00F00000 | (align.bit_length() << 20)
            new_secs.append(Section(name, schars, body, hi - lo, relocs))
            parts.append((lo, hi, len(new_secs)))
        remap[i] = parts or [(0, 1, len(new_secs))]

    def place(old: int, offset: int) -> tuple[int, int]:
        """(new section, offset in it) for an offset in an old section."""
        parts = remap[old]
        for lo, hi, new in parts:
            if lo <= offset < hi:
                return new, offset - lo
        lo, hi, new = parts[-1] if offset >= parts[-1][1] else parts[0]
        return new, offset - lo

    # Section symbols: one per new section, the old one standing for the
    # first part; aux records carry each section's size and relocations.
    section_symbol: dict[int, int] = {}
    new_syms: list[Symbol] = []
    old_to_new: dict[int, int] = {}
    old_index = 0
    for s in syms:
        old_to_new[old_index] = len(new_syms)
        old_index += 1 + len(s.aux)
        t = Symbol(s.name, s.value, s.section, s.type, s.sclass, list(s.aux))
        if s.section > 0:
            new, off = place(s.section, s.value)
            t.section, t.value = new, off
            if s.sclass == IMAGE_SYM_CLASS_STATIC and s.value == 0 and s.aux and s.name.startswith("."):
                section_symbol.setdefault(new, len(new_syms))
        new_syms.append(t)
    for new in range(1, len(new_secs) + 1):
        if new not in section_symbol:
            name = new_secs[new - 1].name
            new_syms.append(Symbol(name, 0, new, 0, IMAGE_SYM_CLASS_STATIC, [bytes(18)]))
            section_symbol[new] = len(new_syms) - 1
    # Every section symbol's aux record: its section's size and relocations,
    # and an associative COMDAT's partner, renumbered.
    for new, si in section_symbol.items():
        sym = new_syms[si]
        if not sym.aux:
            continue
        aux = bytearray(sym.aux[0])
        sec = new_secs[new - 1]
        number, selection = struct.unpack_from("<HB", aux, 12)
        if number:
            number = remap.get(number, [(0, 0, number)])[0][2]
        struct.pack_into("<IHH", aux, 0, sec.size, len(sec.relocs), 0)
        struct.pack_into("<H", aux, 12, number)
        sym.aux[0] = bytes(aux)
    # Relocations: by the new symbol indices; one against a split section's
    # symbol goes to the part its addend points into.
    sec_of_symbol = {old_to_new[i]: None for i in old_to_new}
    old_section_symbols = {}
    old_index = 0
    for s in syms:
        if s.section > 0 and s.sclass == IMAGE_SYM_CLASS_STATIC and s.value == 0 and s.aux \
                and s.name.startswith("."):
            old_section_symbols[old_to_new[old_index]] = s.section
        old_index += 1 + len(s.aux)
    del sec_of_symbol
    for sec in new_secs:
        for r in sec.relocs:
            r[1] = old_to_new[r[1]]
            old_sec = old_section_symbols.get(r[1])
            if old_sec in by_sec and r[2] in (REL_DIR32, REL_DIR32NB) and sec.data:
                (addend,) = struct.unpack_from("<i", sec.data, r[0])
                new, off = place(old_sec, addend)
                r[1] = section_symbol[new]
                body = bytearray(sec.data)
                struct.pack_into("<i", body, r[0], off)
                sec.data = bytes(body)
    write(chars, new_secs, new_syms, out)
    return named
