"""List matched functions named as free functions that are probably methods.

    uv run tools/methods.py

A function whose callers load `ecx` just before every call to it is almost
always a `__thiscall` method, even when its own body never reads `ecx` (an
empty method, or one that ignores `this`). Naming it as a free function makes
every caller that is decompiled later fail the name check, so these are worth
renaming before anyone else trips over them. Prints each suspect with its
current name, how many call sites set `ecx`, and its file.
"""

import argparse
import csv
from collections import defaultdict

import capstone

from check import Original, find_source
from progress import ROOT



def main() -> None:
    argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter).parse_args()
    with (ROOT / "data/functions.csv").open() as fh:
        funcs = {int(r["address"], 16): int(r["size"]) for r in csv.DictReader(fh) if r["kind"] in ("game", "gap")}
    with (ROOT / "data/symbols.csv").open() as fh:
        names = {int(r["address"], 16): r["name"] for r in csv.DictReader(fh)}
    free = {a: n for a, n in names.items() if a in funcs and "::" not in n and not n.startswith(("?", "_", "$"))}

    orig = Original()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    ECX = capstone.x86.X86_REG_ECX
    sites = defaultdict(lambda: [0, 0])  # callee -> [calls, calls that receive ecx]
    for start, size in funcs.items():
        passed = False  # ecx written since the last call and not read since
        for ins in md.disasm(orig.read(start, size), start):
            if ins.mnemonic == "call" and ins.op_str.startswith("0x"):
                target = int(ins.op_str, 16)
                if target in free:
                    s = sites[target]
                    s[0] += 1
                    s[1] += passed
                passed = False
                continue
            if ins.mnemonic in ("ret", "jmp") or ins.mnemonic.startswith("j"):
                passed = False
                continue
            read, written = ins.regs_access()
            if ECX in read and not (ins.mnemonic == "xor" and ins.op_str == "ecx, ecx"):
                passed = False
            if ECX in written:
                passed = True

    suspects = [(a, n, c, e) for a, (c, e) in sites.items() for n in [free[a]] if e == c and c > 0]
    for a, n, c, e in sorted(suspects):
        src = find_source(a)
        where = src.relative_to(ROOT) if src else "no file"
        print(f"{a:#x}  {n:40s}  ecx set at {e} of {c} call site(s)  {where}")
    print(f"{len(suspects)} suspect(s)")


if __name__ == "__main__":
    main()
