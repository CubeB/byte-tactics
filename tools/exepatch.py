"""Apply the hand patches of data/exe_patches.csv to a built exe.

orig/TotalA.exe is the GOG/Steam build, and two runs of its bytes are not what
Cavedog's compiler produced: GOG's no-CD music patch replaced a `cmp`/`jne` in
0x4cda00 (the CD-music track count) with a `jmp` to code it wrote into the
zero padding past the end of .text. The source compiles to the compiler's
bytes, which is what tools/check.py compares against, so a built exe has the
unpatched code until this step puts the patch back after the link:

  * tools/place.py lays the image out at the original's addresses, so each
    row's patched bytes go where the row says (apply_placed).
  * tools/link.py --carve moves every function, so each row moves with what
    holds it (apply_linked): a row inside a game function goes to that
    function's address in the link's map plus the same offset, and a row in
    the zero padding past the end of .text goes the same distance past the
    end of the linked .text, whose virtual size then covers it. Every rel32
    branch in a row's bytes is retargeted the same way, and the bytes a row
    replaces are checked before it is written.
"""

import bisect
import csv
import struct
from dataclasses import dataclass
from pathlib import Path

import capstone
import pefile

from check import ROOT

PATCHES = ROOT / "data/exe_patches.csv"
FUNCTIONS = ROOT / "data/functions.csv"
PROGRESS = ROOT / "data/progress.csv"
ORIGINAL = ROOT / "orig/TotalA.exe"


@dataclass
class Patch:
    address: int
    original: bytes
    patched: bytes
    note: str


def load_patches() -> list[Patch]:
    if not PATCHES.exists():
        return []
    with PATCHES.open() as fh:
        return [Patch(int(r["address"], 16), bytes.fromhex(r["original"]), bytes.fromhex(r["patched"]),
                      r["note"]) for r in csv.DictReader(fh)]


def apply_placed(out: bytearray, base: int) -> int:
    """Write every row's patched bytes at its own address into an image laid
    out like the original's (out[0] is the image base). Returns the bytes written."""
    n = 0
    for p in load_patches():
        o = p.address - base
        out[o:o + len(p.patched)] = p.patched
        n += len(p.patched)
    return n


# --- an ordinary link -------------------------------------------------------------

def text_end(pe: pefile.PE) -> tuple[pefile.SectionStructure, int]:
    text = next(s for s in pe.sections if s.Name.rstrip(b"\0") == b".text")
    return text, pe.OPTIONAL_HEADER.ImageBase + text.VirtualAddress + text.Misc_VirtualSize


class Mover:
    """Where an address of the original is in the linked exe."""

    def __init__(self, map_path: Path, orig_end: int, linked_end: int):
        from linkcmp import read_map
        link_at = read_map(map_path)
        with PROGRESS.open() as fh:
            symbol = {int(r["address"], 16): r["symbol"] for r in csv.DictReader(fh)}
        with FUNCTIONS.open() as fh:
            rows = [(int(r["address"], 16), int(r["size"])) for r in csv.DictReader(fh) if r["kind"] == "game"]
        self.functions = sorted((a, size, link_at[symbol[a]]) for a, size in rows
                                if symbol.get(a) in link_at)
        self.starts = [a for a, _, _ in self.functions]
        self.orig_end, self.linked_end = orig_end, linked_end

    def __call__(self, va: int) -> int:
        if self.orig_end <= va < self.orig_end + 0x1000:
            return self.linked_end + (va - self.orig_end)     # the zero padding past .text
        i = bisect.bisect_right(self.starts, va) - 1
        if i >= 0 and va < self.functions[i][0] + self.functions[i][1]:
            start, _, linked = self.functions[i]
            return linked + (va - start)
        raise SystemExit(f"data/exe_patches.csv: {va:#x} is in no game function the link's map names")


def retarget(code: bytes, at: int, to: int, move: Mover) -> bytes:
    """code, which sits at `at` in the original, moved to `to`: every rel32
    branch points at where its target moved."""
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = bytearray(code)
    pos = 0
    while pos < len(code):
        ins = next(md.disasm(code[pos:], at + pos, 1), None)
        if ins is None:
            pos += 1          # zero padding: nothing to move
            continue
        b = bytes(ins.bytes)
        rel = 1 if b[0] in (0xE8, 0xE9) and ins.size == 5 else (
            2 if b[0] == 0x0F and 0x80 <= b[1] <= 0x8F and ins.size == 6 else None)
        if rel is not None:
            target = (ins.address + ins.size + struct.unpack_from("<i", b, rel)[0]) & 0xFFFFFFFF
            moved = move(target)
            struct.pack_into("<i", out, pos + rel, moved - (to + pos + ins.size))
        pos += ins.size
    return bytes(out)


def apply_linked(exe: Path, map_path: Path) -> list[str]:
    """Apply every row to an ordinary link of the tree, in place. Returns a
    line per row for the report."""
    patches = load_patches()
    if not patches:
        return []
    orig = pefile.PE(str(ORIGINAL), fast_load=True)
    _, orig_end = text_end(orig)
    pe = pefile.PE(str(exe), fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase
    text, linked_end = text_end(pe)
    move = Mover(map_path, orig_end, linked_end)
    raw = bytearray(exe.read_bytes())
    lines = []
    new_end = linked_end
    for p in patches:
        to = move(p.address)
        into = to - base - text.VirtualAddress
        if not 0 <= into <= text.SizeOfRawData - len(p.patched):
            raise SystemExit(f"data/exe_patches.csv: {p.address:#x} moves to {to:#x}, outside .text's raw data")
        off = text.PointerToRawData + into
        expect = retarget(p.original, p.address, to, move)
        have = bytes(raw[off:off + len(expect)])
        if have != expect:
            raise SystemExit(f"data/exe_patches.csv: at {to:#x} (the original's {p.address:#x}) the link has "
                             f"{have.hex()}, not the bytes the row replaces ({expect.hex()})")
        raw[off:off + len(p.patched)] = retarget(p.patched, p.address, to, move)
        new_end = max(new_end, to + len(p.patched))
        lines.append(f"  {p.address:#x} -> {to:#x}: {len(p.patched)} bytes")
    if new_end > linked_end:
        # The code past the end of .text becomes part of it.
        struct.pack_into("<I", raw, text.get_file_offset() + 8, new_end - base - text.VirtualAddress)
    exe.write_bytes(bytes(raw))
    return lines
