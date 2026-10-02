# /// script
# requires-python = ">=3.11"
# dependencies = [
#     "capstone>=5",
#     "pefile>=2023.2.7",
#     "tree-sitter>=0.23",
#     "tree-sitter-cpp>=0.23",
# ]
# ///
"""Score a batch of proposed source variants in parallel (issue #4841, suggestion 1).

    uv run ~/.agents/projects/byte-tactics/scratch/propose.py 0x4c0a90 v1.cpp v2.cpp ...
    uv run tools/propose.py 0x4c0a90 --top 3 v*.cpp     # also write the best few copies

Every file is one whole-file variant of the function's source under src/. They
are compiled with check.py's compile_source (12 at a time) and scored with
permute.py's Scorer, so the numbers are the ones permute.py's log.txt prints.
check.py's own ratio and its "shape" ratio (jump targets masked) are added, so
the routing split of suggestion 2 can be read off the same table.

Output is one line per variant, best first:

    score  ratio  shape  bytes  status  file

It never writes to src/: the best few can be written with --top to
build/propose/<address>/ for the next round to start from.
"""

from __future__ import annotations

import argparse
import difflib
import json
import os
import sys
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))

from check import (DEFAULT_FLAGS, PADDING, ROOT, Original, annotations, compare,  # noqa: E402
                   compile_source, disasm, link_placeholders, load_symbols, normalise, select_function)
from coff import parse_object  # noqa: E402
import permute  # noqa: E402
from permute import INF, Score, Scorer  # noqa: E402
from permute_mutate import TargetSpec  # noqa: E402


class ShapeScorer(Scorer):
    """Scorer.score plus check.py's shape_ratio, the routing signal of #4841."""

    def __init__(self, address, qualname, guards):
        super().__init__(address, qualname, guards)
        in_image = self.in_image
        self.theirs_shape = [normalise(i, self.lo, self.hi, in_image, mask_targets=True)
                             for i in disasm(self.theirs, address)]

    def shape(self, obj) -> float:
        picked, err = select_function(obj, None, self.qualname)
        if not picked:
            return 0.0
        _, sec, start, end = picked
        data, mask = sec.data[start:end], sec.mask()[start:end]
        while data and data[-1] in PADDING and mask[-1]:
            data, mask = data[:-1], mask[:-1]
        theirs = self.theirs
        if len(data) == len(theirs) and all(not m or a == b for a, b, m in zip(data, theirs, mask)):
            return 1.0
        shown = disasm(link_placeholders(self.orig, sec, start, end, data, self.address, len(theirs)),
                       self.address)
        ours = [normalise(i, self.lo, self.hi, self.in_image, mask_targets=True) for i in shown]
        return difflib.SequenceMatcher(None, self.theirs_shape, ours, autojunk=False).ratio()


_W: dict = {}


def worker_init(address, qualname, guards, base_text, basename):
    try:
        os.nice(10)
    except OSError:
        pass
    work = ROOT / "build" / "propose" / f"{address:#x}" / "work" / str(os.getpid())
    work.mkdir(parents=True, exist_ok=True)
    _W.update(scorer=ShapeScorer(address, qualname, guards), path=work / basename,
              out_dir=f"propose/{address:#x}/obj/{os.getpid()}", flags=DEFAULT_FLAGS)


def compile_and_measure(text: str) -> dict:
    path = _W["path"]
    path.write_text(text, encoding="latin-1")
    obj_path, log = compile_source(path, _W["flags"], out_dir=_W["out_dir"])
    if obj_path is None:
        err = next((l for l in log.splitlines() if "error" in l), log.strip()[-200:])
        return {"score": INF, "ratio": 0.0, "shape": 0.0, "status": "compile", "size": 0, "note": err[-200:]}
    try:
        obj = parse_object(obj_path.read_bytes(), obj_path.name)
        s = _W["scorer"].score(obj)
        return {"score": s.value, "ratio": s.ratio, "shape": _W["scorer"].shape(obj), "status": s.status,
                "size": s.size, "note": s.note, "hot": list(s.hot)}
    except Exception as exc:  # a broken object must not stop the batch
        return {"score": INF, "ratio": 0.0, "shape": 0.0, "status": "error", "size": 0, "note": repr(exc)[:200]}


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("address", type=lambda s: int(s, 16))
    ap.add_argument("files", type=Path, nargs="+")
    ap.add_argument("--jobs", type=int, default=12)
    ap.add_argument("--top", type=int, default=0, help="write the best N to build/propose/<address>/")
    ap.add_argument("--base", type=Path, help="file every variant is compared against (default src/)")
    ap.add_argument("--json", type=Path)
    args = ap.parse_args()

    src = args.base or permute.find_source(args.address)
    if src is None:
        sys.exit(f"no file under src/ has '// FUNCTION: {args.address:#x}'")
    ann = annotations(src)
    qualname = next((q for a, q in ann if a == args.address), None)
    base_text = src.read_text(errors="replace")

    # Guard: other annotated functions in the file that match now must keep matching.
    obj_path, log = compile_source(src, out_dir=f"propose/{args.address:#x}/base")
    if obj_path is None:
        sys.exit(f"the base file does not compile:\n{log}")
    base_obj = parse_object(obj_path.read_bytes(), obj_path.name)
    orig, symbols = Original(), load_symbols()
    guards = [(a, q) for a, q in ann
              if a != args.address and compare(orig, base_obj, a, None, q, symbols, quick=True).bytes_match]

    paths = [p for p in args.files if p.exists()]
    texts = {p: p.read_text(errors="replace") for p in paths}

    rows = []
    with ProcessPoolExecutor(max_workers=args.jobs, initializer=worker_init,
                             initargs=(args.address, qualname, guards, base_text, src.name)) as pool:
        futs = {p: pool.submit(compile_and_measure, texts[p]) for p in paths}
        for p, f in futs.items():
            r = f.result()
            r["file"] = str(p)
            rows.append(r)
    bad = ("compile", "error", "guard")
    rows.sort(key=lambda r: (r["score"] if r["status"] not in bad else INF, -r["ratio"]))
    base_row = next((r for r in rows if r["file"] in (str(src), str(args.base or ""))), None)

    print(f"{args.address:#x}  {len(rows)} variants of {src.name}"
          + (f", guarding {', '.join(f'{a:#x}' for a, _ in guards)}" if guards else ""))
    if base_row:
        print(f"base: score {base_row['score']:g}, ratio {base_row['ratio'] * 100:.1f}%, "
              f"shape {base_row['shape'] * 100:.1f}%, {base_row['size']} bytes")
    print(f"{'score':>7} {'ratio':>7} {'shape':>7} {'bytes':>7}  status   file")
    for r in rows:
        mark = ""
        if base_row and r["status"] not in bad:
            d = r["score"] - base_row["score"]
            if r["status"] == "match":
                mark = "  MATCH"
            elif d < 0:
                mark = f"  better by {-d:g}"
            elif d > 0:
                mark = f"  worse by {d:g}"
        print(f"{r['score']:7g} {r['ratio'] * 100:6.1f}% {r['shape'] * 100:6.1f}% {r['size']:7d}  "
              f"{r['status']:7s} {r['file']}{mark}"
              + (f"  [{r['note'][:80]}]" if r["status"] in bad and r["note"] else ""))

    if args.top:
        out = ROOT / "build" / "propose" / f"{args.address:#x}"
        out.mkdir(parents=True, exist_ok=True)
        keep = [r for r in rows if r["status"] in ("match", "partial")]
        for i, r in enumerate(keep[:args.top], 1):
            (out / f"best{i}.cpp").write_text(Path(r["file"]).read_text(errors="replace"), encoding="latin-1")
            (out / f"best{i}.txt").write_text(f"score {r['score']:g} ratio {r['ratio'] * 100:.1f}% "
                                             f"shape {r['shape'] * 100:.1f}% from {r['file']}\n", encoding="latin-1")
        (out / "base.cpp").write_text(base_text, encoding="latin-1")
        print(f"wrote the best {min(args.top, len(keep))} to {out}")
    if args.json:
        args.json.write_text(json.dumps(rows, indent=1))
        print(f"wrote {args.json}")


if __name__ == "__main__":
    main()