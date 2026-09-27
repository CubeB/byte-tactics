"""Check several functions at once and print one line each.

    uv run tools/checkall.py 0x401000 0x401070 0x4010b0

Compiles the files in parallel and prints the summary line tools/check.py
would print for each address. Run tools/check.py on one address for its
references and diff.
"""

import os
import sys
from concurrent.futures import ThreadPoolExecutor

from check import Original, annotations, compare, compile_source, find_source, report
from coff import parse_object


def main() -> None:
    if len(sys.argv) < 2 or sys.argv[1] in ("-h", "--help"):
        sys.exit(__doc__)
    addresses = [int(a, 16) for a in sys.argv[1:]]
    orig = Original()

    def one(address: int) -> str:
        src = find_source(address)
        if src is None:
            return f"{address:#x}  no file under src/ has '// FUNCTION: {address:#x}'"
        qualname = next((q for a, q in annotations(src) if a == address), None)
        obj, log = compile_source(src, out_dir=f"checkall/{address:#x}")
        if obj is None:
            return f"{address:#x}  compile failed ({src.name}); run tools/check.py {address:#x}"
        res = compare(orig, parse_object(obj.read_bytes(), obj.name), address, None, qualname)
        return report(res, verbose=False).splitlines()[0]

    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        for line in pool.map(one, addresses):
            print(line)


if __name__ == "__main__":
    main()
