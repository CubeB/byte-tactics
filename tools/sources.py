"""Find the sources under src/ by their annotations, wherever they live.

    uv run tools/sources.py 0x401070        # the file that defines a function
    uv run tools/sources.py 0x4fc490        # the file that defines a global
    uv run tools/sources.py --kinds         # how many files of each kind
    uv run tools/sources.py --list          # every file, its kind and its addresses

No tool depends on the folder or the name of a file: a file is found by the
annotations in it, and docs/tidy-up.md lays the folders out by subsystem.

    // FUNCTION: 0x401070    the function defined after it
    // GLOBAL: 0x4fc490      a global: defined after it in a data file, or
                             declared (extern) after it in a function's file

What a file is follows from the addresses of its functions, by the `kind`
column of data/functions.csv:

    game      game functions (the default)
    gap       a function inside a `gap` region: inline assembly and more
              per-file flags are allowed (tools/gapcheck.py checks it)
    library   only `library` functions: runtime library code built with the
              game's options, which tools/place.py places as library code
    data      no functions and at least one `// GLOBAL:`: the game's data as
              source (docs/linking.md), which tools/place.py places by address

The builds link the objects in a fixed order that does not depend on folders
(link_order): data first, then gap, library and game code, each kind by the
address in its file name, or by its lowest annotated address when the name
holds none. Files are named `<module>_<address>.cpp` (docs/tidy-up.md), so
moving a file between folders does not change the order.
"""

import argparse
import csv
import re
import sys
from functools import lru_cache
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src"
FUNCTIONS = ROOT / "data/functions.csv"
ANNOTATION = re.compile(r"^\s*//\s*FUNCTION:\s*(0x[0-9a-fA-F]+)(?:\s+(\S+))?")
GLOBAL_ANNOTATION = re.compile(r"^\s*//\s*GLOBAL:\s*(0x[0-9a-fA-F]+)")
# The address at the end of a file name: `0x401070.cpp`, `economy_401070.cpp`.
NAME_ADDRESS = re.compile(r"(?:^|_)(?:0x)?([0-9a-fA-F]{6,8})$")
# The order of the kinds in link_order (and of the folders the files had before
# docs/tidy-up.md: src/data, src/gap, src/lib, src/unsorted).
KINDS = ("data", "gap", "library", "game")


# --- annotations ----------------------------------------------------------------

def annotations(src: Path) -> list[tuple[int, str]]:
    """(address, qualified name) for every // FUNCTION: annotation in a file."""
    lines = src.read_text(errors="replace").splitlines()
    out = []
    for i, line in enumerate(lines):
        m = ANNOTATION.match(line)
        if not m:
            continue
        # The definition follows, possibly after more comments or blank lines.
        following = [l for l in lines[i + 1:i + 12] if l.strip() and not l.strip().startswith("//")]
        # Only the definition header counts (up to the opening brace), so an
        # `operator new(` call in the body is not mistaken for the definition.
        text = " ".join(following[:3]).split("{", 1)[0]
        text = re.sub(r"__declspec\s*\(\s*\w+\s*\)", " ", text)
        op = re.search(r"([\w:]*?)operator\s*(new|delete|==|!=|<=|>=|\[\]|\(\)|=|<|>|\+|-|\*|/)\s*\(", text)
        sig = text.split("(", 1)[0]
        names = [op.group(1) + "operator" + op.group(2)] if op else re.findall(r"[A-Za-z_~][\w:~]*", sig)
        # An explicit symbol after the address (for compiler-generated functions
        # such as dynamic initialisers, _$E1) is matched exactly, marked with "=".
        name = "=" + m.group(2) if m.group(2) else (names[-1] if names else "")
        out.append((int(m.group(1), 16), name))
    return out


def global_annotations(src: Path) -> list[tuple[int, str]]:
    """(address, name) for every // GLOBAL: annotation in a file: the name of
    the variable the declaration after it declares or defines."""
    lines = src.read_text(errors="replace").splitlines()
    out = []
    for i, line in enumerate(lines):
        m = GLOBAL_ANNOTATION.match(line)
        if not m:
            continue
        following = [l for l in lines[i + 1:i + 12] if l.strip() and not l.strip().startswith("//")]
        text = " ".join(following[:3])
        # The declarator ends at its initialiser or at the end of the declaration;
        # array extents and a function pointer's parameter list are not names.
        head = re.split(r"[=;{]", text, maxsplit=1)[0]
        head = re.sub(r"\[[^\]]*\]", " ", head)
        head = re.sub(r"\)\s*\([^()]*\)\s*$", ")", head)
        names = [n for n in re.findall(r"[A-Za-z_]\w*", head)
                 if n not in ("const", "static", "extern", "volatile", "unsigned", "signed", "struct",
                              "class", "union", "enum", "__cdecl", "__stdcall", "__fastcall")]
        out.append((int(m.group(1), 16), names[-1] if names else ""))
    return out


def function_addresses(src: Path) -> list[int]:
    """The addresses of the functions a file defines, in file order."""
    return [int(m.group(1), 16) for m in map(ANNOTATION.match, src.read_text(errors="replace").splitlines()) if m]


def global_addresses(src: Path) -> list[int]:
    return [int(m.group(1), 16) for m in map(GLOBAL_ANNOTATION.match, src.read_text(errors="replace").splitlines())
            if m]


# --- what a file is ---------------------------------------------------------------

@lru_cache(maxsize=1)
def function_kinds() -> tuple[dict[int, str], list[tuple[int, int]]]:
    """data/functions.csv's kind of every function start, and the gap
    regions as (start, end)."""
    kinds, gaps = {}, []
    if FUNCTIONS.exists():
        with FUNCTIONS.open() as fh:
            for r in csv.DictReader(fh):
                a = int(r["address"], 16)
                kinds[a] = r["kind"]
                if r["kind"] == "gap":
                    gaps.append((a, a + int(r["size"])))
    return kinds, sorted(gaps)


def gap_region(address: int) -> int | None:
    """The gap region an address lies in, or None."""
    for lo, hi in function_kinds()[1]:
        if lo <= address < hi:
            return lo
    return None


_kinds: dict[tuple[Path, int], str] = {}


def kind_of(src: Path) -> str:
    """game, gap, library or data: see the module docstring."""
    src = src.resolve()
    key = (src, src.stat().st_mtime_ns)
    if key not in _kinds:
        _kinds[key] = _kind_of(src)
    return _kinds[key]


def _kind_of(src: Path) -> str:
    funcs = function_addresses(src)
    if not funcs:
        return "data" if global_addresses(src) else "game"
    if any(gap_region(a) is not None for a in funcs):
        return "gap"
    kinds = function_kinds()[0]
    if all(kinds.get(a) == "library" for a in funcs):
        return "library"
    return "game"


def is_gap_source(src: Path) -> bool:
    """A file with code of a gap region: inline assembly and the extra
    per-file flags are allowed in it."""
    return kind_of(src) == "gap"


def is_data_source(src: Path) -> bool:
    return kind_of(src) == "data"


# --- the files ----------------------------------------------------------------------

def source_files() -> list[Path]:
    """Every source file under src/, in path order."""
    return sorted(SRC.rglob("*.cpp"))


def sources(*kinds: str) -> list[Path]:
    """The source files of the given kinds (all when none are given), in link order."""
    return link_order([s for s in source_files() if not kinds or kind_of(s) in kinds])


def primary_address(src: Path) -> int:
    """The address a file is about: the one in its name, or else the lowest it annotates."""
    m = NAME_ADDRESS.search(src.stem)
    if m:
        return int(m.group(1), 16)
    return min(function_addresses(src) or global_addresses(src) or [0])


def link_key(src: Path | str) -> tuple:
    """Where a file goes in link_order."""
    src = Path(src) if Path(src).is_absolute() else ROOT / src
    kind = kind_of(src)
    return (KINDS.index(kind), src.stem if kind == "data" else "", primary_address(src), src.name)


def link_order(paths: list[Path]) -> list[Path]:
    """Data first, then gap code, library code and game code, each in address
    order: the order the builds link the objects in (tools/link.py leaves the
    gap code out: tools/gapcheck.py builds it). Data files keep their names'
    order (their globals are placed by address anyway). Anything that counts
    or picks among the files goes through this order too, so that where a file
    lives never changes a result."""
    return sorted(paths, key=link_key)


def find_source(address: int) -> Path | None:
    """The file whose // FUNCTION: annotation names an address."""
    for src in source_files():
        if address in function_addresses(src):
            return src
    return None


def find_global(address: int) -> Path | None:
    """The data file that defines the global at an address."""
    for src in sources("data"):
        if address in global_addresses(src):
            return src
    return None


def gap_sources() -> dict[int, Path]:
    """Gap region -> the file holding its code: the one that annotates a
    function inside it. One file holds one region (tools/gapcheck.py)."""
    out: dict[int, Path] = {}
    for src in source_files():
        for a in function_addresses(src):
            region = gap_region(a)
            if region is not None:
                out.setdefault(region, src)
    return out


def gap_source(region: int) -> Path | None:
    return gap_sources().get(region)


_data_annotations: list[tuple[int, str]] | None = None


def data_annotations() -> list[tuple[int, str]]:
    """(address, name) for every global a data file defines."""
    global _data_annotations
    if _data_annotations is None:
        _data_annotations = [a for src in sources("data") for a in global_annotations(src)]
    return _data_annotations


_gap_annotations: list[tuple[int, str]] | None = None


def gap_annotations() -> list[tuple[int, str]]:
    """(address, name) for every name only gap sources spell: the function a
    `// FUNCTION:` annotation defines, and a global a `// GLOBAL:` annotation
    marks on the declaration of it. Gap code is checked by tools/gapcheck.py,
    not by tools/progress.py, so no match there learns these names; without
    them in data/symbols.csv a rename loses tools/globals.py's definition of
    the global and adds tools/place.py relocation rows."""
    global _gap_annotations
    if _gap_annotations is None:
        out: list[tuple[int, str]] = []
        for src in sources("gap"):
            for address, qualname in annotations(src):
                if qualname and not qualname.startswith("="):
                    out.append((address, qualname))
            out.extend((a, n) for a, n in global_annotations(src) if n)
        _gap_annotations = out
    return _gap_annotations


def object_source(obj: Path, cache: Path) -> Path:
    """The source file an object in a mirror of src/ (such as build/progress/) was compiled from."""
    return SRC / obj.relative_to(cache).with_suffix(".cpp")


def relative(src: Path) -> str:
    """A path as data/ and the tools print it: relative to the repository."""
    return str(src.resolve().relative_to(ROOT))


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("address", nargs="*", type=lambda s: int(s, 16))
    ap.add_argument("--kinds", action="store_true", help="count the files of each kind")
    ap.add_argument("--list", action="store_true", help="every file in link order: kind, path, addresses")
    args = ap.parse_args()
    if args.kinds:
        counts: dict[str, int] = {}
        for s in source_files():
            counts[kind_of(s)] = counts.get(kind_of(s), 0) + 1
        for k in KINDS:
            print(f"{k:8s} {counts.get(k, 0):5d}")
    if args.list:
        for s in sources():
            addrs = function_addresses(s) or global_addresses(s)
            print(f"{kind_of(s):8s} {relative(s)}  {' '.join(f'{a:#x}' for a in addrs[:8])}"
                  f"{' ...' if len(addrs) > 8 else ''}")
    status = 0
    for a in args.address:
        src = find_source(a) or find_global(a)
        if src is None:
            region = gap_region(a)
            src = gap_source(region) if region is not None else None
        if src is None:
            print(f"{a:#x}: no file under src/ annotates it")
            status = 1
        else:
            print(f"{a:#x}: {relative(src)} ({kind_of(src)})")
    sys.exit(status)


if __name__ == "__main__":
    main()
