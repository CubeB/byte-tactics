"""Find which MSVC runtime library build was statically linked into the original exe.

Every code section of every object in each candidate .lib is searched for in the
exe's .text, ignoring the bytes the linker patches (relocations). The library
with the most exact matches is the one the game was linked against.
"""

import argparse
from pathlib import Path

import pefile

from coff import read_archive

ROOT = Path(__file__).resolve().parent.parent
MIN_SIZE = 12  # shorter sections match by coincidence too easily


def find_masked(hay: bytes, needle: bytes, mask: bytes) -> int:
    # Anchor on the longest run of fixed bytes, then verify the rest.
    best, start = (0, 0), None
    for i, m in enumerate(mask + b"\0"):
        if m and start is None:
            start = i
        elif not m and start is not None:
            if i - start > best[1] - best[0]:
                best = (start, i)
            start = None
    a, b = best
    if b - a < 4:
        return -1
    anchor = needle[a:b]
    pos = hay.find(anchor)
    while pos != -1:
        base = pos - a
        if base >= 0 and base + len(needle) <= len(hay):
            window = hay[base:base + len(needle)]
            if all(not m or x == y for x, y, m in zip(window, needle, mask)):
                return base
        pos = hay.find(anchor, pos + 1)
    return -1


def find_all_masked(hay: bytes, needle: bytes, mask: bytes) -> list[int]:
    """Every offset where needle matches (find_masked only returns the first)."""
    out, start = [], 0
    while True:
        off = find_masked(hay[start:], needle, mask)
        if off < 0:
            return out
        out.append(start + off)
        start += off + 1


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--exe", default=ROOT / "orig/TotalA.exe", type=Path)
    ap.add_argument("--libs", nargs="+", default=["LIBC.LIB", "LIBCMT.LIB"])
    ap.add_argument("--toolchains", nargs="+", default=["msvc5-rtm", "msvc5-sp3"])
    args = ap.parse_args()

    pe = pefile.PE(str(args.exe), fast_load=True)
    text = next(s for s in pe.sections if s.Name.startswith(b".text"))
    hay = text.get_data()
    text_va = pe.OPTIONAL_HEADER.ImageBase + text.VirtualAddress

    results: dict[tuple[str, str], dict[str, int]] = {}
    for tc in args.toolchains:
        for lib in args.libs:
            found = {}
            for obj in read_archive(ROOT / "toolchain" / tc / "LIB" / lib):
                for sec in obj.sections:
                    if sec.is_code and len(sec.data) >= MIN_SIZE:
                        off = find_masked(hay, sec.data, sec.mask())
                        if off >= 0:
                            syms = obj.symbols_in(sec)
                            label = syms[0].name if syms else f"{obj.name}:{sec.name}"
                            found[f"{obj.name}|{label}|{sec.data.hex()}"] = text_va + off
            results[(tc, lib)] = found
            print(f"{tc:10s} {lib:11s} {len(found):4d} code sections found in exe, "
                  f"{sum(len(bytes.fromhex(k.split('|')[2])) for k in found):7d} bytes")

    # Sections present in one build's library but not the other are what tell them apart.
    for lib in args.libs:
        a, b = (results.get((tc, lib), {}) for tc in args.toolchains[:2])
        only_a = {k.split("|")[1] for k in a.keys() - b.keys()}
        only_b = {k.split("|")[1] for k in b.keys() - a.keys()}
        print(f"\n{lib}: only {args.toolchains[0]} matches: {len(only_a)}  {sorted(only_a)[:8]}")
        print(f"{lib}: only {args.toolchains[1]} matches: {len(only_b)}  {sorted(only_b)[:8]}")


if __name__ == "__main__":
    main()
