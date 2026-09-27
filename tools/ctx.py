"""Print everything known about one original function, for decompiling it.

    uv run tools/ctx.py 0x4010b0

Shows the FPO facts, an annotated disassembly (names of callees and globals,
strings, float constants), what each callee expects, and Ghidra's pseudo-C.
"""

import csv
import re
import struct
import sys
from pathlib import Path

import capstone

from check import Original, base_name, load_symbols

ROOT = Path(__file__).resolve().parent.parent
GHIDRA_DIR = ROOT / "build/ghidra/decomp"


def load_functions() -> dict[int, dict]:
    with (ROOT / "data/functions.csv").open() as fh:
        return {int(r["address"], 16): r for r in csv.DictReader(fh)}


class Namer:
    def __init__(self, orig: Original, funcs: dict[int, dict]):
        self.orig, self.funcs = orig, funcs
        self.names = {addr: name for name, addr in load_symbols().items()}
        self.imports = {e.address: (d.dll.decode(), e.name.decode())
                        for d in orig.pe.DIRECTORY_ENTRY_IMPORT for e in d.imports if e.name}
        self.sections = [(orig.base + s.VirtualAddress, orig.base + s.VirtualAddress + s.Misc_VirtualSize,
                          s.Name.rstrip(b"\0").decode()) for s in orig.pe.sections]

    def section(self, va: int) -> str:
        return next((n for lo, hi, n in self.sections if lo <= va < hi), "")

    def function_label(self, va: int) -> str:
        if va in self.names:
            return self.names[va]
        f = self.funcs.get(va)
        if f and f["name"]:
            return base_name(f["name"])
        return f"FUN_{va:08x}" if f else ""

    def string_at(self, va: int) -> str | None:
        raw = self.orig.read(va, 96)
        if 0 not in raw:
            return None
        s = raw[:raw.index(0)]
        if s and all(32 <= c < 127 or c in (9, 10, 13) for c in s):
            text = '"' + s.decode().encode("unicode_escape").decode()[:70] + '"'
            # One printable byte then a zero may be a short string ("\\") or
            # just data, so say so.
            return text if len(s) >= 2 else text + " (if this is a string)"
        return None

    def describe(self, va: int, ins) -> str:
        if va in self.imports:
            dll, name = self.imports[va]
            return f"import {name} from {dll}: include <windows.h> (or the API's header) and call {name}()"
        if va in self.funcs or va in self.names and self.section(va) == ".text":
            return self.function_label(va)
        sec = self.section(va)
        if sec not in (".rdata", ".data"):
            return ""
        label = self.names.get(va, f"DAT_{va:08x}")
        if ins.mnemonic.startswith("f") and "ptr [" in ins.op_str:
            if "qword" in ins.op_str:
                (v,) = struct.unpack("<d", self.orig.read(va, 8) or b"\0" * 8)
                return f"{label} = {v!r} (double)"
            if "dword" in ins.op_str:
                (v,) = struct.unpack("<f", self.orig.read(va, 4) or b"\0" * 4)
                return f"{label} = {v!r} (float)"
        s = self.string_at(va)
        if s:
            return f"{label} {s}"
        vt = self.vtable_at(va)
        return f"{label} = vtable? [{vt}]" if vt else label

    def vtable_starts(self) -> set[int]:
        """Addresses some code stores into [reg] (`mov dword ptr [reg], imm32`):
        vtable starts. MSVC 5 places vtables back to back with no RTTI between
        them, so a run of function pointers must stop at the next one."""
        if not hasattr(self, "_vtable_starts"):
            text = next(s for s in self.orig.pe.sections if s.Name.startswith(b".text"))
            code = text.get_data()
            lo, hi = next((lo, hi) for lo, hi, n in self.sections if n == ".rdata")
            starts = set()
            for i in range(len(code) - 6):
                if code[i] == 0xC7 and code[i + 1] in (0, 1, 2, 3, 6, 7):
                    imm = int.from_bytes(code[i + 2:i + 6], "little")
                    if lo <= imm < hi:
                        starts.add(imm)
            self._vtable_starts = starts
        return self._vtable_starts

    def vtable_at(self, va: int) -> str:
        """A run of pointers to known function starts in .rdata looks like a vtable."""
        if self.section(va) != ".rdata":
            return ""
        entries = []
        for i in range(32):
            if i and va + 4 * i in self.vtable_starts():
                break
            raw = self.orig.read(va + 4 * i, 4)
            if len(raw) < 4:
                break
            (p,) = struct.unpack("<I", raw)
            if p not in self.funcs:
                break
            entries.append(self.function_label(p))
        if len(entries) < 2:
            return ""
        return ", ".join(entries[:6]) + (f", ... ({len(entries)} entries)" if len(entries) > 6 else "")


def callee_pops(orig: Original, va: int, size: int) -> set[int]:
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    pops = set()
    for _, _, mnem, op in md.disasm_lite(orig.read(va, size), va):
        if mnem == "ret":
            pops.add(int(op, 16) if op else 0)
    return pops


def md_first(code: bytes, va: int):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.syntax = capstone.CS_OPT_SYNTAX_INTEL
    return md.disasm(code, va)


def main() -> None:
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    va = int(sys.argv[1], 16)
    orig, funcs = Original(), load_functions()
    f = funcs.get(va)
    if not f:
        sys.exit(f"{va:#x} is not the start of a known function")
    namer = Namer(orig, funcs)
    size = int(f["size"])
    code = orig.read(va, size)

    print(f"== {va:#x}  {namer.function_label(va)}  ({f['kind']}, {size} bytes)")
    print(f"FPO: {f['params']} dword(s) of stack arguments, {f['locals']} dword(s) of locals, "
          f"C++ exception frame: {'yes' if f['seh'] == '1' else 'no'}, "
          f"frame pointer (ebp): {'yes' if f['frame_pointer'] == '1' else 'no'}")
    pops = callee_pops(orig, va, size)
    print(f"returns with: {', '.join(f'ret {p:#x}' if p else 'ret' for p in sorted(pops)) or 'no ret (tail jump?)'}"
          "  (ret N = callee cleans N bytes: __stdcall or __thiscall; plain ret = __cdecl, or __thiscall with no args)")
    print(f"called from {f['callers']} place(s); calls {f['calls']} function(s)")
    head = [i for _, i in zip(range(5), md_first(code, va))]
    if any(i.mnemonic == "mov" and i.op_str == "eax, ecx" for i in head):
        print("hint: copies ecx (this) into eax up front and returns it: typical of a C++ constructor, "
              "or of a method returning *this / this")

    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.syntax = capstone.CS_OPT_SYNTAX_INTEL
    callees = {}
    print("\n-- disassembly --")
    for ins in md.disasm(code, va):
        notes = []
        for m in re.finditer(r"0x[0-9a-f]+", ins.op_str):
            v = int(m.group(), 16)
            if orig.base <= v < orig.end and not va <= v < va + size:
                d = namer.describe(v, ins)
                if d:
                    notes.append(d)
                if ins.mnemonic in ("call", "jmp") and v in funcs:
                    callees[v] = funcs[v]
        print(f"  {ins.address:#x}: {ins.mnemonic:6s} {ins.op_str:40s}" + (f" ; {'; '.join(notes)}" if notes else ""))

    if callees:
        print("\n-- callees --")
        for cva, cf in sorted(callees.items()):
            cp = callee_pops(orig, cva, int(cf["size"]))
            ret = ", ".join(f"ret {p:#x}" if p else "ret" for p in sorted(cp))
            status = "named in data/symbols.csv" if cva in namer.names else cf["kind"]
            print(f"  {cva:#x} {namer.function_label(cva):40s} {cf['params']} arg dword(s), {ret}  [{status}]")

    ghidra = GHIDRA_DIR / f"{va:#x}.c"
    if ghidra.exists():
        print("\n-- Ghidra pseudo-C (a starting point only: types, names and structure are guesses) --")
        print(ghidra.read_text().strip())


if __name__ == "__main__":
    main()
