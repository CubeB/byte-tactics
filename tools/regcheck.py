"""Per-region similarity for one big function (pilot for splitting huge functions).

    uv run tools/regcheck.py 0x43f0e0 [src/unsorted/0x43f0e0.cpp]

Compiles the file like check.py, aligns our instructions with the original's
over the whole function, and reports how many of the original's instructions in
each region (address range from data/regions/<addr>.csv) are matched, both
exactly and by shape (registers ignored). Register allocation is global, so
shape is the signal to chase first: a region is structurally right when its
shape score is 100 even if the registers still differ.
"""

import argparse
import csv
import difflib
import re
import sys

sys.path.insert(0, str(__import__("pathlib").Path(__file__).resolve().parent))
import check  # noqa: E402

HEXRE = re.compile(r"0x[0-9a-f]+")
REG = re.compile(r"\b(e?[abcd]x|e?[sd]i|e?[sb]p|[abcd][lh])\b")


def shape(text: str) -> str:
    return REG.sub("R", text)


def load_regions(addr: int, size: int):
    path = check.ROOT / "data" / "regions" / f"{addr:#x}.csv"
    rows = []
    if path.exists():
        for r in csv.DictReader(path.open()):
            rows.append((r["name"], int(r["start"], 16), int(r["end"], 16)))
    else:
        rows.append(("all", addr, addr + size))
    return rows


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("address", type=lambda s: int(s, 16))
    ap.add_argument("source", nargs="?", type=check.Path)
    ap.add_argument("--diff", metavar="REGION", help="print the exact diff of one region")
    args = ap.parse_args()
    src = args.source or check.find_source(args.address)
    obj_path, log = check.compile_source(src, out_dir="regcheck")
    if not obj_path:
        print(log)
        sys.exit(1)
    orig = check.Original()
    obj = check.parse_object(obj_path.read_bytes(), obj_path.name)
    picked, err = check.select_function(obj, None, None)
    if not picked:
        print(err)
        sys.exit(1)
    _, sec, start, end = picked
    data = sec.data[start:end]
    while data and data[-1] in check.PADDING:
        data = data[:-1]
    size = orig.sizes.get(args.address, len(data))
    theirs = check.disasm(orig.read(args.address, size), args.address)
    ours = check.disasm(bytes(data), args.address)
    lo, hi = args.address, args.address + size
    in_image = lambda v: orig.base <= v < orig.end
    t_txt = [check.normalise(i, lo, hi, in_image) for i in theirs]
    o_txt = [check.normalise(i, lo, hi, in_image) for i in ours]
    # jump targets differ whenever code moves; compare the mnemonic only
    def fold(t):
        m = t.split(" ", 1)
        return m[0] if m[0].startswith("j") else t
    def blocks(ins, txt):
        targets = {int(m.group(), 16) for i, t in zip(ins, txt) if t.startswith("j")
                   for m in HEXRE.finditer(i.op_str)}
        out, cur = [], []
        for k, i in enumerate(ins):
            if cur and i.address in targets:
                out.append(cur)
                cur = []
            cur.append(k)
            if i.mnemonic.startswith("j") or i.mnemonic.startswith("ret"):
                out.append(cur)
                cur = []
        if cur:
            out.append(cur)
        return out

    def score(t_lines, o_lines):
        """Per original instruction: ratio of its block's best match among our blocks."""
        tb, ob = blocks(theirs, t_txt), blocks(ours, o_txt)
        res = {}
        for blk in tb:
            a = [t_lines[k] for k in blk]
            best = 0.0
            for oblk in ob:
                b = [o_lines[k] for k in oblk]
                r = difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()
                if r > best:
                    best = r
                    if best == 1.0:
                        break
            for k in blk:
                res[k] = best
        return res

    exact = score([fold(t) for t in t_txt], [fold(t) for t in o_txt])
    shaped = score([shape(fold(t)) for t in t_txt], [shape(fold(t)) for t in o_txt])
    r_exact = sum(exact.values()) / len(exact)
    r_shape = sum(shaped.values()) / len(shaped)
    print(f"{args.address:#x}: original {size} bytes, ours {len(data)} bytes; "
          f"block-wise exact {r_exact*100:.1f}%, shape {r_shape*100:.1f}%\n")
    print(f"{'region':10} {'range':21} {'insns':>5} {'exact':>6} {'shape':>6}")
    for name, a, b in load_regions(args.address, size):
        idx = [k for k, i in enumerate(theirs) if a <= i.address < b]
        if not idx:
            continue
        e = sum(exact[k] for k in idx) / len(idx) * 100
        s = sum(shaped[k] for k in idx) / len(idx) * 100
        print(f"{name:10} {a:#x}-{b:#x} {len(idx):5} {e:5.0f}% {s:5.0f}%")
        if args.diff == name:
            for k in idx:
                print((f"{exact[k]*100:4.0f}% ") + f"{theirs[k].address:#x} {t_txt[k]}")


if __name__ == "__main__":
    main()
