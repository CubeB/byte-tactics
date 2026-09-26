"""Compile a source file and compare one function against the original exe.

    uv run tools/check.py 0x401070 src/pilot.cpp --sym Reset

Bytes the linker fills in (relocations) are ignored when comparing, and shown
next to the address the original uses so wrong globals or callees stand out.
"""

import argparse
import difflib
import re
import struct
import subprocess
import sys
from pathlib import Path

import capstone
import pefile

from coff import REL_I386_REL32, Section, parse_object

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_FLAGS = "/O2 /GX /MT"
PADDING = (0x90, 0xCC)


class Original:
    def __init__(self, path: Path):
        self.pe = pefile.PE(str(path))
        self.base = self.pe.OPTIONAL_HEADER.ImageBase
        self.end = self.base + self.pe.OPTIONAL_HEADER.SizeOfImage
        self.sizes = {}
        for d in getattr(self.pe, "DIRECTORY_ENTRY_DEBUG", []):
            if d.struct.Type == 3:  # IMAGE_DEBUG_TYPE_FPO
                raw = self.pe.__data__[d.struct.PointerToRawData:d.struct.PointerToRawData + d.struct.SizeOfData]
                for i in range(0, len(raw), 16):
                    rva, size = struct.unpack_from("<II", raw, i)
                    self.sizes[self.base + rva] = size

    def code(self, va: int, size: int) -> bytes:
        return self.pe.get_data(va - self.base, size)


def winpath(p: Path) -> str:
    return "Z:" + str(p).replace("/", "\\")


def compile_source(src: Path, flags: str) -> Path:
    out = ROOT / "build" / "check" / (src.stem + ".obj")
    out.parent.mkdir(parents=True, exist_ok=True)
    cmd = [str(ROOT / "tools" / "wcl"), "/c", *flags.split(), f"/Fo{winpath(out)}", winpath(src)]
    proc = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT)
    if proc.returncode != 0 or not out.exists():
        sys.exit(f"compile failed:\n{proc.stdout}{proc.stderr}")
    return out


def extract(obj_path: Path, sym: str | None) -> tuple[str, bytes, bytes, dict[int, str]]:
    obj = parse_object(obj_path.read_bytes(), obj_path.name)
    candidates = []
    for sec in obj.sections:
        if not sec.is_code:
            continue
        syms = sorted(obj.symbols_in(sec), key=lambda s: s.value)
        for i, s in enumerate(syms):
            end = syms[i + 1].value if i + 1 < len(syms) else len(sec.data)
            candidates.append((s.name, sec, s.value, end))
    if sym:
        candidates = [c for c in candidates if sym in c[0]]
    if len(candidates) != 1:
        names = ", ".join(c[0] for c in candidates) or "none"
        sys.exit(f"need exactly one function to compare, found: {names} (use --sym)")
    name, sec, start, end = candidates[0]
    data, mask = sec.data[start:end], sec.mask()[start:end]
    while data and data[-1] in PADDING:
        data, mask = data[:-1], mask[:-1]
    relocs = {r.offset - start: r.symbol for r in sec.relocs if start <= r.offset < end}
    rel32 = {r.offset - start for r in sec.relocs if r.type == REL_I386_REL32 and start <= r.offset < end}
    return name, data, mask, {k: v + (" (call/jmp)" if k in rel32 else "") for k, v in relocs.items()}


def disasm(code: bytes, va: int) -> list:
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.syntax = capstone.CS_OPT_SYNTAX_INTEL
    return list(md.disasm(code, va))


HEX = re.compile(r"0x[0-9a-f]+")


def normalise(ins, lo: int, hi: int, is_addr) -> str:
    """Replace addresses outside the function with <addr> so both sides compare equal."""
    def sub(m):
        v = int(m.group(), 16)
        return "<addr>" if is_addr(v) and not lo <= v < hi else m.group()
    return f"{ins.mnemonic} {HEX.sub(sub, ins.op_str)}".strip()


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("address", type=lambda s: int(s, 16))
    ap.add_argument("source", type=Path)
    ap.add_argument("--sym", help="substring of the (mangled) function name to compare")
    ap.add_argument("--flags", default=DEFAULT_FLAGS)
    ap.add_argument("--exe", type=Path, default=ROOT / "orig/TotalA.exe")
    args = ap.parse_args()

    orig = Original(args.exe)
    obj = compile_source(args.source.resolve(), args.flags)
    name, ours, mask, relocs = extract(obj, args.sym)
    size = orig.sizes.get(args.address, len(ours))
    theirs = orig.code(args.address, size)

    exact = len(ours) == len(theirs) and all(not m or a == b for a, b, m in zip(ours, theirs, mask))
    va, hi = args.address, args.address + size

    # Relocated operands are 0 (or an addend) in the .obj, so treat any reloc'd
    # instruction's operands as <addr>; in the exe, anything inside the image is.
    ours_ins = disasm(ours, va)
    reloc_ins = {i.address for i in ours_ins for off in relocs if i.address - va <= off < i.address - va + i.size}
    ours_txt = [normalise(i, va, hi, lambda v, i=i: i.address in reloc_ins) for i in ours_ins]
    theirs_ins = disasm(theirs, va)
    theirs_txt = [normalise(i, va, hi, lambda v: orig.base <= v < orig.end) for i in theirs_ins]
    ratio = difflib.SequenceMatcher(None, theirs_txt, ours_txt, autojunk=False).ratio()

    status = "MATCH" if exact else f"{ratio * 100:.1f}%"
    print(f"{args.address:#x}  {name}  original {size} bytes, ours {len(ours)} bytes  ->  {status}")

    # Pair each relocation with what the original has in the same place.
    if relocs:
        print("\nlinker-resolved references (ours -> original):")
        for off, sym in sorted(relocs.items()):
            if off + 4 <= len(theirs):
                (val,) = struct.unpack_from("<I", theirs, off)
                if "(call/jmp)" in sym:
                    val = (va + off + 4 + val) & 0xFFFFFFFF
                print(f"  +{off:#05x}  {sym:40s} {val:#x}")

    if not exact:
        print("\n" + "\n".join(difflib.unified_diff(theirs_txt, ours_txt, "original", "ours", lineterm="", n=3)))
    sys.exit(0 if exact else 1)


if __name__ == "__main__":
    main()
