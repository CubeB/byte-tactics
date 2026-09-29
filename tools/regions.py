"""Propose region seams for splitting one big function between agents.

    uv run tools/regions.py 0x4d8e60 [--min 250] [--max 500] [--write]

Cut points are basic-block starts. For a function with a jump-table switch the
cases are the seams (case start to next case start, the code before the first
case and after the last one become their own regions). For flat code the seams
are chosen greedily: after at least --min bytes, cut at the next block start
or the next instruction after a call that follows a call, ret or unconditional
jump, and is not the target of a jump from before the previous cut (so no
region is entered from the middle of an earlier one). A cut is forced at --max.

Prints the proposal; --write stores it as data/regions/<addr>.csv, which
tools/regcheck.py reads. Edit the file by hand where the seams look wrong.
"""

import argparse
import re
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check  # noqa: E402

HEX = re.compile(r"0x[0-9a-f]+")
TABLE = re.compile(r"\[\w+\*4 \+ (0x[0-9a-f]+)\]")


def read_u32(orig, va: int) -> int:
    return struct.unpack("<I", orig.read(va, 4))[0]


def jump_tables(orig, ins, lo: int, hi: int):
    """Yield the target lists of `jmp [reg*4 + table]` dispatches in the function."""
    for i in ins:
        if i.mnemonic != "jmp":
            continue
        m = TABLE.search(i.op_str)
        if not m:
            continue
        table, targets = int(m.group(1), 16), []
        while len(targets) < 512:
            t = read_u32(orig, table + 4 * len(targets))
            if not lo <= t < hi:
                break
            targets.append(t)
        if targets:
            yield table, targets


def propose(orig, address: int, size: int, lo_size: int, hi_size: int):
    ins = check.disasm(orig.read(address, size), address)
    lo, hi = address, address + size
    leaders, jumps = {address}, []
    for k, i in enumerate(ins):
        if i.mnemonic.startswith("j") and i.mnemonic != "jmp" or i.mnemonic == "jmp":
            m = HEX.search(i.op_str)
            if m and "[" not in i.op_str:
                t = int(m.group(), 16)
                if lo <= t < hi:
                    leaders.add(t)
                    jumps.append((i.address, t))
        if (i.mnemonic.startswith("j") or i.mnemonic.startswith("ret")) and k + 1 < len(ins):
            leaders.add(ins[k + 1].address)

    tables = list(jump_tables(orig, ins, lo, hi))
    if tables:
        table, targets = max(tables, key=lambda t: len(t[1]))
        cuts = sorted(set(targets))
        # the table itself follows the code; stop the last case before it
        end = min([table] + [a for a in cuts if a > max(cuts)] or [hi])
        names = {}
        for case, t in enumerate(targets, 1):
            names.setdefault(t, []).append(str(case))
        rows = [("head", address, cuts[0])]
        for a, b in zip(cuts, cuts[1:] + [min(end, hi)]):
            rows.append(("case" + "_".join(names[a]), a, b))
        if rows[-1][2] < hi:
            rows.append(("tail", rows[-1][2], hi))
        return rows, f"jump table at {table:#x} with {len(targets)} entries"

    after_break = {ins[k + 1].address for k, i in enumerate(ins[:-1])
                   if i.mnemonic in ("call", "jmp", "ret", "retn")}
    cuts, start = [], address
    for i in ins:
        a = i.address
        if a == address or (a not in leaders and a not in after_break):
            continue
        run = a - start
        if run < lo_size:
            continue
        entered_from_before = any(src < start and start < t <= a and t != a for src, t in jumps)
        if run >= hi_size or (a in after_break and not entered_from_before):
            cuts.append(a)
            start = a
    bounds = [address] + cuts + [hi]
    rows = [(f"r{n}", a, b) for n, (a, b) in enumerate(zip(bounds, bounds[1:]), 1)]
    return rows, "flat code, greedy cuts"


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("address", type=lambda s: int(s, 16))
    ap.add_argument("--min", type=int, default=250, dest="lo")
    ap.add_argument("--max", type=int, default=500, dest="hi")
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()
    orig = check.Original()
    size = orig.sizes[args.address]
    rows, how = propose(orig, args.address, size, args.lo, args.hi)
    print(f"{args.address:#x}: {size} bytes, {how}\n")
    for name, a, b in rows:
        print(f"{name:12} {a:#x}-{b:#x} {b - a:5} bytes")
    if args.write:
        out = check.ROOT / "data" / "regions" / f"{args.address:#x}.csv"
        out.parent.mkdir(exist_ok=True)
        out.write_text("name,start,end\n" + "".join(f"{n},{a:#x},{b:#x}\n" for n, a, b in rows))
        print(f"\nwrote {out.relative_to(check.ROOT)}")


if __name__ == "__main__":
    main()
