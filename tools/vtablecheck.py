"""Check that a vtable a source file compiles names, in every slot, the
function the original's vtable holds there.

    uv run tools/vtablecheck.py src/unsorted/0x48e010.cpp            # every vtable it emits
    uv run tools/vtablecheck.py src/unsorted/0x48e010.cpp --vtable '??_7Class_0048fc70@@6BCondition_0048ff40@@@'

A class's vtable is emitted by every file that compiles its constructor or
destructor, and each slot is a relocation against a virtual function's
mangled name. For the linked game to call the right function, that name must
be the one the function's own file defines: an override named after the base
class's virtual it overrides (`Class_0048fc70::FUN_0048f840`), with the same
parameter types, declared virtual. tools/check.py accepts a slot that names a
placeholder, since names are provisional; this tool does not. For each slot it
looks up, through data/progress.csv, the file that defines the function at
the address the original's slot holds, compiles it and checks that it defines
the slot's exact name.
"""

import argparse
import csv
import struct
import sys
from pathlib import Path

from check import ROOT, Original, compile_source, load_symbols
from coff import parse_object

PROGRESS = ROOT / "data/progress.csv"


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("source", type=Path)
    ap.add_argument("--vtable", help="only this vtable symbol")
    args = ap.parse_args()
    src = args.source if args.source.is_absolute() else ROOT / args.source
    symbols = load_symbols()
    orig = Original()
    with PROGRESS.open() as fh:
        defined_in = {int(r["address"], 16): r["file"] for r in csv.DictReader(fh)}
    obj_path, log = compile_source(src)
    if obj_path is None:
        raise SystemExit(f"{src} does not compile:\n{log}")
    obj = parse_object(obj_path.read_bytes(), obj_path.name)
    compiled: dict[str, set[str]] = {}

    def defines(file: str) -> set[str]:
        if file not in compiled:
            path, flog = compile_source(ROOT / file)
            if path is None:
                raise SystemExit(f"{file} does not compile:\n{flog}")
            o = parse_object(path.read_bytes(), path.name)
            compiled[file] = {s.name for s in o.symbols if s.section > 0 and s.storage_class == 2}
        return compiled[file]

    bad = total = 0
    for sym in obj.symbols:
        if not sym.name.startswith("??_7") or sym.section <= 0 or (args.vtable and sym.name != args.vtable):
            continue
        addr = symbols.get(sym.name)
        if addr is None:
            print(f"{sym.name}: not in data/symbols.csv, so its address is unknown")
            continue
        sec = obj.sections[sym.section - 1]
        slots = sorted((r.offset - sym.value, r.symbol) for r in sec.relocs if r.offset >= sym.value)
        print(f"{sym.name} at {addr:#x}:")
        for off, name in slots:
            (fn,) = struct.unpack("<I", orig.read(addr + off, 4))
            file = defined_in.get(fn)
            total += 1
            if file is None:
                verdict = "the original's function there has no source file (a library function?)"
                ok = name.startswith("__purecall") or name == "__purecall"
            else:
                ok = name in defines(file)
                verdict = f"{file} defines it" if ok else f"{file} does not define this name"
            bad += not ok
            print(f"  slot {off // 4:2d}: {'ok ' if ok else 'BAD'} {name}  (the original has {fn:#x}; {verdict})")
    print(f"{total - bad} of {total} slots name the function the original's vtable holds")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
