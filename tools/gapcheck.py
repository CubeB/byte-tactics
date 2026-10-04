"""Check the source of a gap region against the original exe, byte for byte.

    uv run tools/gapcheck.py 0x4e16b0       # one region: every function, and its coverage
    uv run tools/gapcheck.py                # every region: which have source and which match
    uv run tools/gapcheck.py --list         # the regions, their sizes and status

The 29 `gap` rows of data/functions.csv are code with no FPO record. Most of
it is compiled code the function finder could not see: functions with inline
assembly (cpuid, `int 3`), with __try/__except or try/catch frames, and
functions compiled with /Op, whose frames align the stack (`and esp, -8`).
Some is hand-written assembly. A region's source is src/gap/<address>.cpp:

  * every function in it is annotated `// FUNCTION: 0x...` as everywhere
    else, and tools/check.py checks one of them on its own;
  * inline assembly (`__asm`, `_emit` for instructions the compiler's
    assembler does not know) is allowed in these files only, and so are the
    per-file flags `// FLAGS: /Op` and `/GX` besides `/Gi`;
  * an entry point in the middle of a function (hand-written assembly falls
    from one routine into the next, or has entries that are not 16-byte
    aligned) is an `__asm` label annotated `// ENTRY: 0x... [symbol]` on the
    line before it. The compiler keeps asm labels private, so the checked
    object gets a public symbol there (`_FUN_<address>` unless one is named),
    at the label's offset from the function holding it.

A region MATCHES when every annotated function matches as tools/check.py
defines it (every byte the compiler controls, and every reference the linker
fills in), and together the functions cover every byte of the region apart
from the alignment padding between them. tools/place.py and
tools/link.py --carve then build the region from its object (with the entry
symbols added, under build/gap/) instead of the original's bytes; regions
without matching source are still copied or carved.
"""

import argparse
import csv
import hashlib
import re
import struct
import sys
from dataclasses import dataclass, field
from pathlib import Path

from check import GAP_DIR, ROOT, Original, Result, annotations, compare, disasm, load_symbols, report
from coff import CoffObject, parse_object

FUNCTIONS = ROOT / "data/functions.csv"
OUT = ROOT / "build/gap"
ENTRY = re.compile(r"^\s*//\s*ENTRY:\s*(0x[0-9a-fA-F]+)(?:\s+(\S+))?")
IMAGE_SYM_CLASS_EXTERNAL = 2


def regions() -> dict[int, int]:
    """Gap region address -> size (without its trailing padding)."""
    with FUNCTIONS.open() as fh:
        return {int(r["address"], 16): int(r["size"]) for r in csv.DictReader(fh) if r["kind"] == "gap"}


def source_of(region: int) -> Path:
    return GAP_DIR / f"{region:#x}.cpp"


def entries(src: Path) -> list[tuple[int, str]]:
    """(address, symbol or "") for every `// ENTRY:` label in a file."""
    out = []
    for line in src.read_text(errors="replace").splitlines():
        m = ENTRY.match(line)
        if m:
            out.append((int(m.group(1), 16), m.group(2) or ""))
    return out


@dataclass
class Alias:
    """A public symbol for an asm label: name, at section/offset."""
    address: int
    name: str
    section: int
    value: int


@dataclass
class RegionResult:
    address: int
    size: int
    source: Path
    obj: Path | None = None
    functions: list[Result] = field(default_factory=list)
    aliases: list[Alias] = field(default_factory=list)
    uncovered: list[tuple[int, int]] = field(default_factory=list)     # (start, end) not built
    problems: list[str] = field(default_factory=list)
    error: str = ""

    @property
    def matched(self) -> bool:
        return (not self.error and not self.problems and not self.uncovered and bool(self.functions)
                and all(f.matched for f in self.functions))

    @property
    def score(self) -> float:
        """Matched share of the region's bytes: each function's ratio over its size."""
        if self.error or not self.size:
            return 0.0
        done = sum(min(f.size, self.size) * (1.0 if f.matched else f.ratio) for f in self.functions if not f.error)
        return min(done / self.size, 1.0) if not self.matched else 1.0

    @property
    def status(self) -> str:
        if self.error:
            return "error"
        return "MATCH" if self.matched else f"{self.score * 100:.1f}%"


def compile_gap(src: Path) -> tuple[Path | None, str]:
    """The region's object, through tools/progress.py's cache (build/progress/gap/)."""
    from linkcheck import include_hash
    from progress import compile_cached
    return compile_cached(src, include_hash())


def symbol_value(obj: CoffObject, name: str) -> tuple[int, int] | None:
    for s in obj.symbols:
        if s.name == name and s.section > 0:
            return s.section, s.value
    return None


def check_region(region: int, size: int, orig: Original | None = None,
                 symbols: dict[str, int] | None = None) -> RegionResult:
    src = source_of(region)
    res = RegionResult(region, size, src)
    if not src.exists():
        res.error = f"no source: {src.relative_to(ROOT)}"
        return res
    obj_path, log = compile_gap(src)
    if obj_path is None:
        res.error = "compile failed:\n" + log
        return res
    res.obj = obj_path
    orig = orig or Original()
    symbols = load_symbols() if symbols is None else symbols
    obj = parse_object(obj_path.read_bytes(), obj_path.name)

    funcs = annotations(src)
    if not funcs:
        res.error = f"{src.relative_to(ROOT)} has no '// FUNCTION:' annotation"
        return res
    extents: list[tuple[int, int, Result]] = []
    starts = sorted(a for a, _ in funcs)
    for address, qualname in sorted(funcs):
        # No FPO record gives the function's size: it runs to the next annotated
        # function (or the region's end), less the padding before it.
        later = [a for a in starts if a > address] + [region + size]
        end = max(min(later), address)
        orig.sizes[address] = len(orig.read(address, end - address).rstrip(b"\x90\xcc")) or end - address
        r = compare(orig, obj, address, qualname=qualname, symbols=symbols)
        res.functions.append(r)
        if not region <= address < region + size or (not r.error and address + r.ours_size > region + size):
            res.problems.append(f"{address:#x} runs outside the region {region:#x}..{region + size:#x}")
        if not r.error:
            extents.append((address, address + r.ours_size, r))

    # Asm labels that are entry points: a public symbol at the label's offset
    # in the matched function that holds it, on one of its instructions.
    for address, name in entries(src):
        holder = next(((lo, hi, r) for lo, hi, r in extents if lo < address < hi), None)
        if holder is None:
            res.problems.append(f"ENTRY {address:#x} is inside no annotated function")
            continue
        lo, hi, r = holder
        if address not in {i.address for i in disasm(orig.read(lo, hi - lo), lo)}:
            res.problems.append(f"ENTRY {address:#x} is not on an instruction of {r.symbol}")
            continue
        at = symbol_value(obj, r.symbol)
        if at is None:
            res.problems.append(f"ENTRY {address:#x}: {r.symbol} is not defined in the object")
            continue
        res.aliases.append(Alias(address, name or f"_FUN_{address:08x}", at[0], at[1] + address - lo))

    # Coverage: every byte of the region is in a function or is padding.
    built = bytearray(size)
    for lo, hi, _ in extents:
        for a in range(max(lo, region), min(hi, region + size)):
            built[a - region] = 1
    code = orig.read(region, size)
    run = None
    for i in range(size + 1):
        missing = i < size and not built[i] and code[i] not in (0x90, 0xCC)
        if missing and run is None:
            run = i
        elif not missing and run is not None:
            res.uncovered.append((region + run, region + i))
            run = None
    return res


def describe(res: RegionResult, verbose: bool = True) -> str:
    lines = [f"{res.address:#x}  region of {res.size:,} bytes  ->  {res.status}"]
    if res.error:
        return lines[0] + "\n  " + res.error
    for f in res.functions:
        if f.error:
            lines.append(f"  {f.address:#x}  ERROR  {f.error}")
        else:
            # A gap function has no FPO record to give the original's size: ours
            # is compared, and the coverage below shows what it leaves out.
            lines.append(f"  {f.address:#x}  {f.symbol}  {f.ours_size:,} bytes  ->  {f.status}"
                         + ("  (bytes match, but a reference is wrong)" if f.bytes_match and not f.matched else ""))
    for a in res.aliases:
        lines.append(f"  entry {a.address:#x}  {a.name}")
    for lo, hi in res.uncovered:
        lines.append(f"  not covered by any function: {lo:#x}..{hi:#x} ({hi - lo} bytes)")
    for p in res.problems:
        lines.append("  " + p)
    if verbose:
        for f in res.functions:
            if not f.matched:
                lines.append("\n" + report(f))
    return "\n".join(lines)


# --- the objects the builds use ------------------------------------------------------

@dataclass
class GapObject:
    region: int
    size: int
    path: Path                                   # build/gap/<region>.obj, with the entry symbols
    functions: list[tuple[int, str, int]]        # (address, symbol, size) of each function
    aliases: list[Alias]


def add_symbols(data: bytes, defs: list[tuple[str, int, int]]) -> bytes:
    """A COFF object with external symbols (name, section, value) appended to
    its symbol table."""
    data = bytearray(data)
    _, _, _, symptr, nsyms, _, _ = struct.unpack_from("<HHIIIHH", data, 0)
    strtab = bytearray(data[symptr + nsyms * 18:])
    new = bytearray()
    for name, section, value in defs:
        raw = name.encode("latin-1")
        if len(raw) <= 8:
            field_ = raw.ljust(8, b"\0")
        else:
            field_ = b"\0\0\0\0" + struct.pack("<I", len(strtab))
            strtab += raw + b"\0"
        new += field_ + struct.pack("<IhHBB", value, section, 0x20, IMAGE_SYM_CLASS_EXTERNAL, 0)
    struct.pack_into("<I", strtab, 0, len(strtab))
    struct.pack_into("<I", data, 12, nsyms + len(defs))
    return bytes(data[:symptr + nsyms * 18]) + bytes(new) + bytes(strtab)


def gap_objects(quiet: bool = False) -> dict[int, GapObject]:
    """Every region whose source matches -> its object for the builds."""
    out: dict[int, GapObject] = {}
    sizes = regions()
    sources = [r for r in sorted(sizes) if source_of(r).exists()]
    if not sources:
        return out
    orig, symbols = Original(), load_symbols()
    OUT.mkdir(parents=True, exist_ok=True)
    for region in sources:
        res = check_region(region, sizes[region], orig, symbols)
        if not res.matched:
            if not quiet:
                print(f"gap {region:#x}: source does not match ({res.status}); the original's bytes are used",
                      file=sys.stderr)
            continue
        path = OUT / f"{region:#x}.obj"
        path.write_bytes(add_symbols(res.obj.read_bytes(), [(a.name, a.section, a.value) for a in res.aliases]))
        out[region] = GapObject(region, sizes[region], path,
                                [(f.address, f.symbol, f.ours_size) for f in res.functions], res.aliases)
    return out


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("address", nargs="*", type=lambda s: int(s, 16))
    ap.add_argument("--list", action="store_true", help="list every region and its status")
    args = ap.parse_args()
    sizes = regions()
    wanted = args.address or sorted(sizes)
    unknown = [a for a in wanted if a not in sizes]
    if unknown:
        sys.exit("not a gap region: " + ", ".join(f"{a:#x}" for a in unknown)
                 + " (uv run tools/gapcheck.py --list shows them)")
    orig, symbols = Original(), load_symbols()
    done = done_bytes = 0
    failed = False
    for region in wanted:
        if not args.address and not source_of(region).exists():
            if args.list:
                print(f"{region:#x}  {sizes[region]:>6,} bytes  no source")
            continue
        res = check_region(region, sizes[region], orig, symbols)
        if args.list or not args.address:
            print(f"{region:#x}  {sizes[region]:>6,} bytes  {res.status}")
        else:
            print(describe(res))
        if res.matched:
            done += 1
            done_bytes += sizes[region]
        else:
            failed = True
    if not args.address:
        print(f"{done} of {len(sizes)} gap regions match their source "
              f"({done_bytes:,} of {sum(sizes.values()):,} bytes)")
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
