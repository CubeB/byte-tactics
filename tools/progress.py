"""Re-check every annotated function under src/ and report progress.

    uv run tools/progress.py            # check everything, update README and data/
    uv run tools/progress.py --quiet    # only print the summary

Writes:
  data/progress.csv  status of every annotated function
  data/symbols.csv   name -> address map, rebuilt from scratch out of verified
                     matches only (plus the runtime library's names)
  README.md          the section between the progress markers
"""

import argparse
import csv
import hashlib
import os
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from check import (DEFAULT_FLAGS, ROOT, SYMBOLS, Original, annotations, base_name, compare,
                   compile_source)
from coff import parse_object

README = ROOT / "README.md"
PROGRESS = ROOT / "data/progress.csv"
START, END = "<!-- progress:start -->", "<!-- progress:end -->"
NOT_LEARNED = ("$", "??_C@", "__real@", "??_7", "??_G", "??_E")


def compile_cached(src: Path, include_hash: str):
    key = hashlib.sha256(src.read_bytes() + include_hash.encode() + DEFAULT_FLAGS.encode()).hexdigest()[:16]
    stamp = ROOT / "build/progress" / src.relative_to(ROOT / "src").with_suffix(".key")
    obj = stamp.with_suffix(".obj")
    if stamp.exists() and stamp.read_text() == key and obj.exists():
        return obj, ""
    obj, log = compile_source(src, out_dir="progress")
    if obj:
        stamp.write_text(key)
    return obj, log


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args()

    with (ROOT / "data/functions.csv").open() as fh:
        funcs = {int(r["address"], 16): r for r in csv.DictReader(fh)}
    game = {a: int(r["size"]) for a, r in funcs.items() if r["kind"] == "game"}

    # Names from the runtime block at the end of the exe are known up front;
    # everything else is learned from matches. C++ library code found inside
    # the game region is left out: some of it exists twice (two copies of
    # std::_Lockit were linked in) and only a match shows which copy the
    # game's own code calls.
    ordered = sorted(funcs.items())
    runtime_start = next(a for a, r in reversed(ordered) if r["kind"] == "game")
    symbols: dict[str, int] = {}
    for a, r in ordered:
        if (a > runtime_start and r["kind"] == "library" and r["name"]
                and not r["name"].startswith(NOT_LEARNED)):
            symbols.setdefault(base_name(r["name"]), a)
    # Import slots carry their API's name: a call to a dllimport function
    # references __imp__Name@N, whose base name is _imp__Name.
    orig = Original()
    for d in orig.pe.DIRECTORY_ENTRY_IMPORT:
        for e in d.imports:
            if e.name:
                symbols.setdefault("_imp__" + e.name.decode(), e.address)

    include_hash = hashlib.sha256(b"".join(
        p.read_bytes() for p in sorted((ROOT / "include").rglob("*")) if p.is_file())).hexdigest()
    work = []
    for src in sorted((ROOT / "src").rglob("*.cpp")):
        for address, qualname in annotations(src):
            work.append((address, qualname, src))
    work.sort()

    # Compiling is the slow part (one Wine process per file), so do it in parallel.
    sources = sorted({src for _, _, src in work})
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        compiled = dict(zip(sources, pool.map(lambda s: compile_cached(s, include_hash), sources)))

    conflicts: list[str] = []
    seen: dict[int, Path] = {}
    objects: dict[Path, object] = {}
    rows = []
    for address, qualname, src in work:
        rel = src.relative_to(ROOT)
        row = {"address": f"{address:#x}", "size": game.get(address, ""), "file": str(rel),
               "symbol": "", "status": "", "similarity": ""}
        rows.append(row)
        if address in seen:
            row["status"] = f"duplicate of {seen[address].relative_to(ROOT)}"
            continue
        seen[address] = src
        if address not in game:
            row["status"] = "not a game function start"
            continue
        if src not in objects:
            obj_path, log = compiled[src]
            objects[src] = parse_object(obj_path.read_bytes(), obj_path.name) if obj_path else log
        obj = objects[src]
        if isinstance(obj, str):
            row["status"] = "compile error"
            continue
        res = compare(orig, obj, address, qualname=qualname, symbols=symbols)
        row["symbol"] = res.symbol
        row["similarity"] = f"{res.ratio * 100:.1f}"
        row["status"] = "matched" if res.matched else ("error" if res.error else "partial")
        if res.matched:
            # One name per address: the first one learned (or pre-loaded) wins.
            named = set(symbols.values())
            own = base_name(res.symbol)
            if not own.startswith("$"):  # skip compiler-generated _$E1...
                if address not in named:
                    symbols.setdefault(own, address)
                elif symbols.get(own) != address:
                    held = next(k for k, v in symbols.items() if v == address)
                    conflicts.append(f"{address:#x} {row['file']}: defines '{own}', but callers use '{held}'")
            for ref in res.refs:
                if (ref.status == "new" and not ref.symbol.startswith(NOT_LEARNED)
                        and not base_name(ref.symbol).startswith("$") and ref.target not in named):
                    symbols.setdefault(base_name(ref.symbol), ref.target)
                    named.add(ref.target)

    PROGRESS.parent.mkdir(exist_ok=True)
    with PROGRESS.open("w", newline="") as fh:
        w = csv.DictWriter(fh, ["address", "size", "file", "symbol", "status", "similarity"], lineterminator="\n")
        w.writeheader()
        w.writerows(rows)
    with SYMBOLS.open("w", newline="") as fh:
        w = csv.writer(fh, lineterminator="\n")
        w.writerow(["address", "name"])
        for name, a in sorted(symbols.items(), key=lambda kv: (kv[1], kv[0])):
            w.writerow([f"{a:#x}", name])

    matched = [int(r["address"], 16) for r in rows if r["status"] == "matched"]
    partial = [r for r in rows if r["status"] == "partial"]
    total_bytes = sum(game.values())
    done_bytes = sum(game[a] for a in matched)
    pct = 100 * done_bytes / total_bytes
    bar = "#" * round(pct / 2.5) + "-" * (40 - round(pct / 2.5))
    section = "\n".join([
        START,
        "## Progress",
        "",
        f"**{pct:.2f}% of Cavedog's code matched** ({done_bytes:,} of {total_bytes:,} bytes)",
        "",
        f"`[{bar}]`",
        "",
        "| | Functions | Bytes |",
        "| --- | ---: | ---: |",
        f"| Matched byte-for-byte | {len(matched):,} of {len(game):,} | {done_bytes:,} |",
        f"| Attempted, not yet matching | {len(partial):,} | {sum(int(r['size'] or 0) for r in partial):,} |",
        "",
        "Generated by `uv run tools/progress.py`; per-function status is in `data/progress.csv`.",
        END,
    ])
    text = README.read_text()
    if START in text:
        text = text[:text.index(START)] + section + text[text.index(END) + len(END):]
    else:
        marker = "\n## Target"
        text = text.replace(marker, "\n" + section + "\n" + marker, 1)
    README.write_text(text)

    print(f"{pct:.2f}% matched: {len(matched)} functions, {done_bytes} bytes; {len(partial)} partial")
    if not args.quiet:
        for r in rows:
            if r["status"] not in ("matched", "partial"):
                print(f"  {r['address']} {r['file']}: {r['status']}")
    for c in conflicts:
        print("  name conflict:", c)
    bad = [r for r in rows if r["status"] not in ("matched", "partial")]
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
