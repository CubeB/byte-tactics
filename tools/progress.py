"""Re-check every annotated function under src/ and report progress.

    uv run tools/progress.py            # check everything, update README and data/
    uv run tools/progress.py --quiet    # only print the summary
    uv run tools/progress.py --readme   # only redraw the README section from data/

Writes:
  data/progress.csv  status of every annotated function
  data/symbols.csv   name -> address map, rebuilt from scratch out of verified
                     matches only (plus the runtime library's names and the
                     names gap files' annotations give)
  README.md          the section between the progress markers
"""

import argparse
import csv
import hashlib
import re
import os
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from check import (COMPILE_SCHEME, DEFAULT_FLAGS, ROOT, SYMBOLS, Original, annotations, base_name, compare,
                   compile_source)
from coff import parse_object
from issues import BANDS
from sources import gap_annotations, sources as files_of_kind

README = ROOT / "README.md"
PROGRESS = ROOT / "data/progress.csv"
AREAS = ROOT / "data/areas.csv"
START, END = "<!-- progress:start -->", "<!-- progress:end -->"
NOT_LEARNED = ("$", "??_C@", "__real@", "??_G", "??_E")
# A static local to a function (`_?len@?CM@??FUN_...@4HA`, its guard `?$S1@?CM@??...`)
# is private to its function, so two files can each have one with the same short
# name at different addresses; never learn it as a global name.
LOCAL_STATIC = re.compile(r"@\?[0-9A-P]{1,4}@\?\?")


def compile_cached(src: Path, include_hash: str):
    key = hashlib.sha256(src.read_bytes() + include_hash.encode() + (DEFAULT_FLAGS + COMPILE_SCHEME).encode()).hexdigest()[:16]
    stamp = ROOT / "build/progress" / src.relative_to(ROOT / "src").with_suffix(".key")
    obj = stamp.with_suffix(".obj")
    if stamp.exists() and stamp.read_text() == key and obj.exists():
        return obj, ""
    obj, log = compile_source(src, out_dir="progress")
    if obj:
        stamp.write_text(key)
    return obj, log


def bar(pct: float, width: int) -> str:
    filled = round(pct * width / 100)
    return "#" * filled + "-" * (width - filled)


def breakdown(label: str, funcs: list[int], game: dict[int, int], status: dict[int, str]) -> str:
    """One table row: functions matched, share of the bytes matched, bytes left."""
    done = [a for a in funcs if status.get(a) == "matched"]
    size = sum(game[a] for a in funcs)
    done_bytes = sum(game[a] for a in done)
    pct = 100 * done_bytes / size
    return (f"| {label} | {len(done):,} of {len(funcs):,} | {pct:.1f}% | "
            f"{size - done_bytes:,} | `{bar(pct, 10)}` |")


def write_readme(game: dict[int, int], rows: list[dict]) -> float:
    """Rewrite the progress section of README.md; return the share of bytes matched."""
    status = {int(r["address"], 16): r["status"] for r in rows if r["status"] in ("matched", "partial")}
    matched = [a for a, s in status.items() if s == "matched"]
    partial = [a for a, s in status.items() if s == "partial"]
    untouched = [a for a in game if a not in status]
    total_bytes = sum(game.values())
    done_bytes = sum(game[a] for a in matched)
    pct = 100 * done_bytes / total_bytes

    lines = [
        START,
        "## Progress",
        "",
        f"**{pct:.2f}% of Cavedog's code matched** ({done_bytes:,} of {total_bytes:,} bytes)",
        "",
        f"`[{bar(pct, 40)}]`",
        "",
        "| | Functions | Bytes |",
        "| --- | ---: | ---: |",
        f"| Matched byte-for-byte | {len(matched):,} | {done_bytes:,} |",
        f"| Attempted, not yet matching | {len(partial):,} | {sum(game[a] for a in partial):,} |",
        f"| Not attempted yet | {len(untouched):,} | {sum(game[a] for a in untouched):,} |",
        "",
        "### By function size",
        "",
        "The size bands are the `size:` labels on the issues.",
        "",
        "| Size | Functions matched | Bytes matched | Bytes left | |",
        "| --- | ---: | ---: | ---: | --- |",
    ]
    for name, (lo, hi) in BANDS.items():
        span = f"over {lo - 1:,}" if hi >= 1 << 30 else f"{lo:,} to {hi:,}"
        lines.append(breakdown(f"{name} ({span} bytes)",
                               [a for a, s in game.items() if lo <= s <= hi], game, status))
    lines += [
        "",
        "### By area",
        "",
        "The linker kept each part of the engine roughly together, so each 64 KB of the",
        "executable is mostly one subsystem. The area names are guesses from the strings",
        "and Windows calls each window's code uses (`data/areas.csv`).",
        "",
        "| Addresses | Area | Functions matched | Bytes matched | Bytes left | |",
        "| --- | --- | ---: | ---: | ---: | --- |",
    ]
    with AREAS.open() as fh:
        areas = {int(r["window"], 16): r["area"] for r in csv.DictReader(fh)}
    windows: dict[int, list[int]] = {}
    for a in sorted(game):
        windows.setdefault(a & ~0xFFFF, []).append(a)
    for window, funcs in sorted(windows.items()):
        lines.append(breakdown(f"`{window:#x}` | {areas.get(window, '?')}", funcs, game, status))
    lines += [
        "",
        "Generated by `uv run tools/progress.py`; per-function status is in `data/progress.csv`.",
        END,
    ]
    section = "\n".join(lines)
    text = README.read_text()
    if START in text:
        text = text[:text.index(START)] + section + text[text.index(END) + len(END):]
    else:
        marker = "\n## Target"
        text = text.replace(marker, "\n" + section + "\n" + marker, 1)
    README.write_text(text)
    return pct


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--quiet", action="store_true")
    ap.add_argument("--readme", action="store_true",
                    help="only rewrite the README section from data/progress.csv, without checking")
    args = ap.parse_args()

    with (ROOT / "data/functions.csv").open() as fh:
        funcs = {int(r["address"], 16): r for r in csv.DictReader(fh)}
    game = {a: int(r["size"]) for a, r in funcs.items() if r["kind"] == "game"}
    if args.readme:
        with PROGRESS.open() as fh:
            print(f"{write_readme(game, list(csv.DictReader(fh))):.2f}% matched")
        return

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
    # Gap regions are checked by tools/gapcheck.py, and runtime library code is
    # not game functions (tools/place.py places it): only game files count.
    for src in files_of_kind("game"):
        for address, qualname in annotations(src):
            work.append((address, qualname, src))
    work.sort()

    # Compiling is the slow part (one Wine process per file), so do it in parallel.
    sources = sorted({src for _, _, src in work})
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        compiled = dict(zip(sources, pool.map(lambda s: compile_cached(s, include_hash), sources)))

    conflicts: list[str] = []
    objects: dict[Path, object] = {}
    for src in sources:
        obj_path, log = compiled[src]
        objects[src] = parse_object(obj_path.read_bytes(), obj_path.name) if obj_path else log

    # Pass 1: every function whose bytes match contributes its own name (its
    # definition) before any references are learned, so a caller elsewhere in
    # the address order cannot claim that address under a different name.
    named = set(symbols.values())
    for address, qualname, src in work:
        obj = objects[src]
        if address not in game or isinstance(obj, str):
            continue
        res = compare(orig, obj, address, qualname=qualname, symbols=symbols, quick=True)
        own = base_name(res.symbol) if res.symbol else ""
        if res.bytes_match and own and not own.startswith("$") and address not in named:
            symbols.setdefault(own, address)
            named.add(address)

    # Pass 2: verify everything against those names and learn references.
    seen: dict[int, Path] = {}
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
                        and not LOCAL_STATIC.search(ref.symbol)
                        and not base_name(ref.symbol).startswith("$") and ref.target not in named):
                    symbols.setdefault(base_name(ref.symbol), ref.target)
                    named.add(ref.target)
                # A real name replaces a DAT_<address> placeholder for the same address.
                real = base_name(ref.symbol)
                placeholder = f"DAT_{ref.target:08x}"
                if (ref.status == "ok" and symbols.get(placeholder) == ref.target
                        and real != placeholder and real not in symbols
                        and not ref.symbol.startswith(NOT_LEARNED) and not real.startswith("$")):
                    del symbols[placeholder]
                    symbols[real] = ref.target

    # Names only gap sources spell, from the annotations beside their
    # definitions and declarations (tools/sources.py). Gap code is checked by
    # tools/gapcheck.py, so no match above learns them, and without a row the
    # tools that read the address out of a placeholder name (tools/globals.py,
    # tools/place.py, tools/linkcheck.py) lose the name on a rename. The
    # matches come first: a gap function can share a name with a game function
    # (FatalError, 0x4b6290 and 0x4d9ab0), and the learned one keeps it. One
    # name per address, as the table has it.
    for address, name in gap_annotations():
        if name not in symbols and address not in symbols.values():
            symbols[name] = address

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

    pct = write_readme(game, rows)
    matched = [r for r in rows if r["status"] == "matched"]
    partial = [r for r in rows if r["status"] == "partial"]
    done_bytes = sum(int(r["size"]) for r in matched)
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
