"""Compile a source file and compare one function against the original exe.

    uv run tools/check.py 0x401070                  # finds the file by its annotation
    uv run tools/check.py 0x401070 src/foo.cpp --sym Reset

A function is annotated with a comment on the line before its definition:

    // FUNCTION: 0x401070
    void PlayerRef::Reset(unsigned char playerIndex)

A function MATCHES when every byte the compiler controls is identical and every
reference the linker fills in points at the right thing:
  - strings and constants defined in the file must have the original's contents
  - jump tables must point back into the function at the right place
  - named functions and globals must agree with data/symbols.csv, the
    name -> address map learned from earlier matches
"""

import argparse
import csv
import difflib
import hashlib
import re
import struct
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

import capstone
import pefile

from coff import REL_I386_DIR32, REL_I386_REL32, CoffObject, parse_object

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_FLAGS = "/O2 /Ob2 /MT /Gz"
PADDING = (0x90, 0xCC)
SYMBOLS = ROOT / "data/symbols.csv"
ANNOTATION = re.compile(r"^\s*//\s*FUNCTION:\s*(0x[0-9a-fA-F]+)(?:\s+(\S+))?")
FORBIDDEN = re.compile(r"\b(__asm|_asm|_emit|__emit)\b|#\s*pragma\s+(optimize|code_seg)")


# --- the original exe -------------------------------------------------------

class Original:
    def __init__(self, path: Path = ROOT / "orig/TotalA.exe"):
        self.pe = pefile.PE(str(path))
        self.base = self.pe.OPTIONAL_HEADER.ImageBase
        self.end = self.base + self.pe.OPTIONAL_HEADER.SizeOfImage
        self.sizes = {}
        for d in getattr(self.pe, "DIRECTORY_ENTRY_DEBUG", []):
            if d.struct.Type == 3:  # IMAGE_DEBUG_TYPE_FPO
                raw = self.pe.__data__[d.struct.PointerToRawData:d.struct.PointerToRawData + d.struct.SizeOfData]
                for i in range(0, len(raw), 16):
                    rva, size = struct.unpack_from("<II", raw, i)
                    self.sizes[self.base + rva] = size

        # Bytes a later hand patch changed in orig/TotalA.exe (the GOG build),
        # put back to what the compiler emitted: data/exe_patches.csv.
        self.patches = []
        if PATCHES.exists():
            for row in csv.DictReader(PATCHES.open()):
                self.patches.append((int(row["address"], 16), bytes.fromhex(row["original"])))

    def read(self, va: int, size: int) -> bytes:
        try:
            data = self.pe.get_data(va - self.base, size)
        except Exception:
            return b""
        for at, orig in self.patches:
            lo, hi = max(va, at), min(va + len(data), at + len(orig))
            if lo < hi:
                data = data[:lo - va] + orig[lo - at:hi - at] + data[hi - va:]
        return data


# --- names ------------------------------------------------------------------

def base_name(sym: str) -> str:
    """Undecorate a symbol just enough to compare names: '?Reset@PlayerRef@@QAEXE@Z' -> 'PlayerRef::Reset'."""
    if re.search(r"@\?\d+\?\?", sym):
        # A function-local static (or its guard, $S1) is named inside its
        # function's scope; it is file-local like the compiler's _$E names.
        return "$local:" + sym
    if sym.startswith("??_C@"):
        return sym  # string literal, named after its contents
    if sym.startswith("??0") or sym.startswith("??1"):
        scopes = sym[3:].split("@@", 1)[0].split("@")
        cls = scopes[0]
        return "::".join(reversed(scopes)) + "::" + ("~" if sym[2] == "1" else "") + cls
    if sym.startswith("??"):
        return sym  # operators, vftables, compiler helpers: keep as is
    if sym.startswith("?"):
        parts = sym[1:].split("@@", 1)[0].split("@")
        return "::".join(reversed(parts[1:])) + ("::" if len(parts) > 1 else "") + parts[0]
    if sym.startswith("_"):
        return sym[1:].split("@", 1)[0]
    return sym


OPERATOR_CODES = {"new": "2", "delete": "3", "=": "4", "==": "8", "!=": "9", "[]": "A",
                  "()": "R", "<": "M", "<=": "N", ">": "O", ">=": "P", "+": "H", "-": "G",
                  "*": "D", "/": "K"}


def mangled_prefixes(qualname: str) -> list[str]:
    """Symbol prefixes a C++ definition of `qualname` could compile to."""
    parts = qualname.split("::")
    last, scopes = parts[-1], parts[:-1]
    scope = "".join(s + "@" for s in reversed(scopes))
    if last.startswith("operator"):
        code = OPERATOR_CODES.get(last[len("operator"):])
        return [f"??{code}{scope}@"] if code else [qualname]
    if scopes and last == scopes[-1]:
        return [f"??0{last}@{''.join(s + '@' for s in reversed(scopes[:-1]))}@"]
    if last.startswith("~"):
        return [f"??1{last[1:]}@{''.join(s + '@' for s in reversed(scopes[:-1]))}@"]
    return [f"?{last}@{scope}@", f"_{last}@", f"_{last}"]


ALIASES = ROOT / "data/aliases.csv"
PATCHES = ROOT / "data/exe_patches.csv"
ALIAS_MAP: dict[str, set[int]] = {}


def load_aliases() -> dict[str, set[int]]:
    """Extra addresses a name may refer to, where the exe really contains
    duplicate copies of one function (e.g. two std::_Lockit)."""
    out: dict[str, set[int]] = {}
    if ALIASES.exists():
        with ALIASES.open() as fh:
            for row in csv.DictReader(fh):
                out.setdefault(row["name"], set()).add(int(row["address"], 16))
    return out


def load_symbols() -> dict[str, int]:
    if not SYMBOLS.exists():
        return {}
    with SYMBOLS.open() as fh:
        return {row["name"]: int(row["address"], 16) for row in csv.DictReader(fh)}


# --- source files -------------------------------------------------------------

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
        op = re.search(r"([\w:]*?)operator\s*(new|delete|==|!=|<=|>=|\[\]|\(\)|=|<|>|\+|-|\*|/)\s*\(", text)
        sig = text.split("(", 1)[0]
        names = [op.group(1) + "operator" + op.group(2)] if op else re.findall(r"[A-Za-z_~][\w:~]*", sig)
        # An explicit symbol after the address (for compiler-generated functions
        # such as dynamic initialisers, _$E1) is matched exactly, marked with "=".
        name = "=" + m.group(2) if m.group(2) else (names[-1] if names else "")
        out.append((int(m.group(1), 16), name))
    return out


def find_source(address: int) -> Path | None:
    for src in sorted((ROOT / "src").rglob("*.cpp")):
        if any(a == address for a, _ in annotations(src)):
            return src
    return None


def winpath(p: Path) -> str:
    return "Z:" + str(p).replace("/", "\\")


def compile_source(src: Path, flags: str = DEFAULT_FLAGS, out_dir: str = "obj") -> tuple[Path | None, str]:
    """Returns (object path, compiler output). Object path is None on failure.

    out_dir (under build/) keeps concurrent users, e.g. agents running check.py
    while progress.py re-verifies everything, from clobbering each other's objects.
    """
    bad = FORBIDDEN.search(src.read_text(errors="replace"))
    if bad:
        return None, f"{src}: '{bad.group(0)}' is not allowed; write the function in plain C++"
    if src.resolve().is_relative_to(ROOT / "src"):
        rel = src.resolve().relative_to(ROOT / "src")
    else:
        # A scratch file: keep objects of same-named files from different
        # folders (several agents at once) apart.
        tag = hashlib.sha256(str(src.resolve().parent).encode()).hexdigest()[:12]
        rel = Path("scratch") / tag / src.name
    out = ROOT / "build" / out_dir / rel.with_suffix(".obj")
    out.parent.mkdir(parents=True, exist_ok=True)
    out.unlink(missing_ok=True)
    cmd = [str(ROOT / "tools" / "wcl"), "/c", *flags.split(), f"/I{winpath(ROOT / 'include')}",
           f"/Fo{winpath(out)}", winpath(src.resolve())]
    proc = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT)
    log = (proc.stdout + proc.stderr).replace("\r", "")
    if proc.returncode != 0 or not out.exists():
        return None, log
    return out, log


# --- comparison ---------------------------------------------------------------

@dataclass
class Ref:
    offset: int      # within the function
    symbol: str
    target: int      # address the original uses (after removing our addend)
    status: str      # ok | new | mismatch | unverified
    note: str = ""


@dataclass
class Result:
    address: int
    symbol: str
    size: int
    ours_size: int
    bytes_match: bool
    ratio: float
    refs: list[Ref] = field(default_factory=list)
    diff: str = ""
    error: str = ""

    @property
    def matched(self) -> bool:
        return self.bytes_match and not any(r.status == "mismatch" for r in self.refs)

    @property
    def status(self) -> str:
        if self.error:
            return "error"
        return "MATCH" if self.matched else f"{self.ratio * 100:.1f}%"


def select_function(obj: CoffObject, want: str | None, qualname: str | None):
    cands = []
    for sec in obj.sections:
        if not sec.is_code:
            continue
        syms = sorted(obj.symbols_in(sec), key=lambda s: s.value)
        for i, s in enumerate(syms):
            end = syms[i + 1].value if i + 1 < len(syms) else len(sec.data)
            cands.append((s.name, sec, s.value, end))
    if want:
        picked = [c for c in cands if want in c[0]]
    elif qualname and qualname.startswith("="):
        picked = [c for c in cands if c[0] == qualname[1:]]
    elif qualname:
        prefixes = mangled_prefixes(qualname)
        picked = [c for c in cands if c[0].startswith(tuple(prefixes)) or c[0] == prefixes[-1]]
    else:
        picked = cands
    if len(picked) != 1:
        names = ", ".join(c[0] for c in picked or cands) or "none"
        return None, f"could not pick one function for {qualname or want or 'this file'} (candidates: {names})"
    return picked[0], ""


def disasm(code: bytes, va: int) -> list:
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.syntax = capstone.CS_OPT_SYNTAX_INTEL
    return list(md.disasm(code, va))


HEX = re.compile(r"0x[0-9a-f]+")


def normalise(ins, lo: int, hi: int, is_addr) -> str:
    def sub(m):
        v = int(m.group(), 16)
        return "<addr>" if is_addr(v) and not lo <= v < hi else m.group()
    return f"{ins.mnemonic} {HEX.sub(sub, ins.op_str)}".strip()


def link_placeholders(orig: Original, sec, start: int, end: int, data: bytes, address: int,
                      size: int) -> bytes:
    """Our function's bytes with every field the linker fills in pointed at a
    placeholder address in the image, so it disassembles as an address like the
    original's instead of `[0]`, `push 0` or a call to the next instruction."""
    placeholder = orig.base + 0x1000
    if address <= placeholder < address + size:
        placeholder = orig.end - 0x10
    patched = bytearray(data)
    for r in sec.relocs:
        off = r.offset - start
        if not start <= r.offset < end or off + 4 > len(patched):
            continue
        (field,) = struct.unpack_from("<I", patched, off)
        if r.type == REL_I386_REL32:
            struct.pack_into("<I", patched, off, (placeholder - (address + off + 4)) & 0xFFFFFFFF)
        elif r.type == REL_I386_DIR32:
            struct.pack_into("<I", patched, off, (placeholder + field) & 0xFFFFFFFF)
    return bytes(patched)


def compare(orig: Original, obj: CoffObject, address: int, want: str | None = None,
            qualname: str | None = None, symbols: dict[str, int] | None = None,
            quick: bool = False) -> Result:
    """Compare one function of our object with the original. `quick` stops once
    the bytes are compared: the result has the symbol and bytes_match but no
    references, score or diff (tools/progress.py's first pass needs no more)."""
    symbols = load_symbols() if symbols is None else symbols
    ALIAS_MAP.clear()
    ALIAS_MAP.update(load_aliases())
    picked, err = select_function(obj, want, qualname)
    if not picked:
        return Result(address, "", 0, 0, False, 0.0, error=err)
    name, sec, start, end = picked
    data, mask = sec.data[start:end], sec.mask()[start:end]
    while data and data[-1] in PADDING and mask[-1]:
        data, mask = data[:-1], mask[:-1]
    size = orig.sizes.get(address, len(data))
    theirs = orig.read(address, size)
    bytes_match = len(data) == len(theirs) and all(not m or a == b for a, b, m in zip(data, theirs, mask))
    if quick:
        return Result(address, name, size, len(data), bytes_match, 1.0 if bytes_match else 0.0)

    by_name = {s.name: s for s in obj.symbols}
    by_addr = {v: k for k, v in symbols.items()}
    refs = []
    for r in sec.relocs:
        if not start <= r.offset < end or r.offset - start + 4 > len(theirs):
            continue
        off = r.offset - start
        if not bytes_match:
            # Until the code lines up, the original's bytes at this offset belong
            # to some other instruction, so the address read there is meaningless.
            refs.append(Ref(off, r.symbol, 0, "unverified", "checked once the code matches"))
            continue
        (field_ours,) = struct.unpack_from("<I", data, off)
        (field_orig,) = struct.unpack_from("<I", theirs, off)
        if r.type == REL_I386_REL32:
            target = (address + off + 4 + field_orig - field_ours) & 0xFFFFFFFF
        elif r.type == REL_I386_DIR32:
            target = (field_orig - field_ours) & 0xFFFFFFFF
        else:
            refs.append(Ref(off, r.symbol, 0, "unverified", f"relocation type {r.type:#x}"))
            continue
        refs.append(check_ref(orig, obj, sec, start, address, off, r.symbol, target, field_ours,
                              by_name, symbols, by_addr))

    ours_ins = disasm(data, address)
    reloc_ins = {i.address for i in ours_ins for ref in refs
                 if i.address - address <= ref.offset < i.address - address + i.size}
    lo, hi = address, address + size

    # An address into the original image written as a plain number matches the
    # bytes but not the meaning: the linker could never move it. Require a symbol.
    if bytes_match:
        for i in ours_ins:
            if i.address in reloc_ins or i.mnemonic.startswith("j") or i.mnemonic == "call":
                continue
            for m in HEX.finditer(i.op_str):
                v = int(m.group(), 16)
                if 0x401000 <= v < orig.end:
                    refs.append(Ref(i.address - address, f"{v:#x}", v, "mismatch",
                                    "hard-coded address: declare the global/vtable/function and refer to it by name"))
        # Matching bytes score 1.0 with no diff, so skip the text comparison.
        return Result(address, name, size, len(data), True, 1.0, refs, "")
    shown_ins = disasm(link_placeholders(orig, sec, start, end, data, address, size), address)
    in_image = lambda v: orig.base <= v < orig.end
    ours_txt = [normalise(i, lo, hi, in_image) for i in shown_ins]
    theirs_txt = [normalise(i, lo, hi, in_image) for i in disasm(theirs, address)]
    ratio = difflib.SequenceMatcher(None, theirs_txt, ours_txt, autojunk=False).ratio()
    diff = "" if bytes_match else "\n".join(
        difflib.unified_diff(theirs_txt, ours_txt, "original", "ours", lineterm="", n=3))
    return Result(address, name, size, len(data), bytes_match, 1.0 if bytes_match else ratio, refs, diff)


def check_ref(orig, obj, sec, start, address, off, sym_name, target, addend, by_name, symbols, by_addr) -> Ref:
    sym = by_name.get(sym_name)
    # Defined in this object: compare what it points at.
    if sym is not None and sym.section > 0:
        target_sec = obj.sections[sym.section - 1]
        if target_sec is sec:  # jump tables and other self-references
            want = address + sym.value - start
            ok = target == want
            return Ref(off, sym_name, target, "ok" if ok else "mismatch",
                       "" if ok else f"should point into this function at {want:#x}")
        if target_sec.is_code:
            return lookup(off, sym_name, target, symbols, by_addr)
        if sym_name.startswith("??_7"):
            problem = check_vtable(orig, obj, target_sec, sym.value, target, symbols)
            if problem:
                return Ref(off, sym_name, target, "mismatch", problem)
        # Up to the next symbol in the same section: MSVC packs constants
        # (floats, doubles) back to back, and the original's neighbours differ.
        after = [s.value for s in obj.symbols if s.section == sym.section and s.value > sym.value
                 and s.storage_class in (2, 3)]
        ours = target_sec.data[sym.value:min(after, default=len(target_sec.data))]
        if not ours or not any(ours[:64]):
            return lookup(off, sym_name, target, symbols, by_addr)  # uninitialised data
        ours = ours[:ours.index(0) + 1] if sym_name.startswith(("$SG", "??_C@")) and 0 in ours else ours[:16]
        theirs = orig.read(target + addend, len(ours)) if sym_name.startswith("$SG") else orig.read(target, len(ours))
        if sym_name.startswith("$SG"):
            ours = target_sec.data[sym.value + addend:][:len(ours)]
        ok = ours == theirs
        return Ref(off, sym_name, target, "ok" if ok else "mismatch",
                   "" if ok else f"contents differ: ours {ours[:24]!r} original {theirs[:24]!r}")
    return lookup(off, sym_name, target, symbols, by_addr)


def same_slot_function(held: str, name: str, fn: int) -> bool:
    """Two names for one vtable slot that only differ where names are still
    provisional: the vector (??_E) and scalar (??_G) deleting destructors, which
    MSVC 5 links to one function, or two placeholder methods FUN_<address>.
    An override shares its base method's name, so a derived class's slot can
    hold FUN_<base address> while its own function was matched as
    FUN_<derived address>; both are provisional (docs/consolidation.md)."""
    if held[:4] in ("??_E", "??_G") and name[:4] in ("??_E", "??_G"):
        return held[4:] == name[4:]
    placeholder = re.compile(r"FUN_[0-9a-f]{8}$")
    return bool(placeholder.match(held.split("::")[-1]) and placeholder.match(name.split("::")[-1]))


def check_vtable(orig, obj, sec, start, target, symbols) -> str:
    """Compare a vtable defined in our object with the original one at target:
    every slot we declare must be a function in the original's vtable, and
    must not disagree with a name already established for that function."""
    slots = sorted((r.offset - start, r.symbol) for r in sec.relocs if r.offset >= start)
    other_vtables = {a: n for n, a in symbols.items() if n.startswith("??_7") and a != target}
    for off, sym in slots:
        raw = orig.read(target + off, 4)
        if len(raw) < 4:
            return f"vtable slot {off // 4} is past the end of the original data"
        (fn,) = struct.unpack("<I", raw)
        if target + off in other_vtables:
            return (f"vtable slot {off // 4} runs into another class's vtable "
                    f"('{other_vtables[target + off]}' starts at {target + off:#x}); declare fewer virtual functions")
        if fn not in orig.sizes:
            return (f"vtable slot {off // 4} does not exist in the original (it holds {fn:#x}); "
                    "the class declares more virtual functions than its vtable has")
        name = base_name(sym)
        known = symbols.get(name)
        if known is None and name.startswith("??_E"):
            known = symbols.get("??_G" + name[4:])  # one function for both (see same_slot_function)
        if known is not None and known != fn:
            return f"vtable slot {off // 4} is {fn:#x} in the original, but '{name}' is {known:#x}"
        # The reverse: the slot's function already has another name (a method
        # inherited from a base class keeps the base's name).
        held = [n for n, a in symbols.items() if a == fn]
        if (known is None and held and fn not in ALIAS_MAP.get(name, ())
                and not any(is_placeholder(h, fn) or same_slot_function(h, name, fn) for h in held)):
            return (f"vtable slot {off // 4} is {fn:#x}, already named '{held[0]}' in data/symbols.csv; "
                    "declare it under that name (a base class's method if it is inherited)")
    return ""


PLACEHOLDER = re.compile(r"(?:DAT|FUN|PTR|LAB)_([0-9a-f]{8})$")


def lookup(off, sym_name, target, symbols, by_addr) -> Ref:
    name = base_name(sym_name)
    if target in ALIAS_MAP.get(name, ()):
        return Ref(off, sym_name, target, "ok", "duplicate copy (data/aliases.csv)")
    # A placeholder name carries its own address: it must be where the original points.
    m = PLACEHOLDER.search(name.split("::")[-1])
    if m and int(m.group(1), 16) != target:
        return Ref(off, sym_name, target, "mismatch",
                   f"'{name}' names {int(m.group(1), 16):#x}, but the original uses {target:#x}")
    if name.startswith("$"):  # compiler-generated, file-local (_$E1...): not in the global map
        return Ref(off, sym_name, target, "new", "file-local")
    known = symbols.get(name)
    if known is not None:
        ok = known == target
        return Ref(off, sym_name, target, "ok" if ok else "mismatch",
                   "" if ok else f"'{name}' is {known:#x} in data/symbols.csv, but the original uses {target:#x}"
                   + (f" ('{by_addr[target]}')" if target in by_addr else ""))
    if target in by_addr:
        held = by_addr[target]
        # A DAT_<address> placeholder only says "some data at this address", so
        # it agrees with any real name for the same address (e.g. a class's
        # compiler-generated vtable ??_7...), in either direction.
        if is_placeholder(held, target) or is_placeholder(name, target):
            return Ref(off, sym_name, target, "ok", f"same address as '{held}'")
        return Ref(off, sym_name, target, "mismatch",
                   f"{target:#x} is already named '{held}' in data/symbols.csv; use that name")
    return Ref(off, sym_name, target, "new")


def is_placeholder(name: str, address: int) -> bool:
    return name == f"DAT_{address:08x}"


def report(res: Result, verbose: bool = True) -> str:
    if res.error:
        return f"{res.address:#x}  ERROR  {res.error}"
    lines = [f"{res.address:#x}  {res.symbol}  original {res.size} bytes, ours {res.ours_size} bytes  ->  {res.status}"]
    if res.bytes_match and not res.matched:
        lines[0] += "  (bytes match, but a reference is wrong)"
    if re.match(r"\?[^@]+@@YI", res.symbol):
        lines.append("note: this is a __fastcall free function. If only ecx is an input (edx unused), "
                     "write it as a __thiscall method of a class instead; see docs/agent-guide.md.")
    if verbose and res.refs:
        lines.append("\nreferences the linker fills in (symbol -> address in the original):")
        for r in sorted(res.refs, key=lambda r: r.offset):
            flag = {"ok": "ok ", "new": "new", "mismatch": "BAD", "unverified": "?  "}[r.status]
            target = f"{r.target:#x}" if r.status != "unverified" else "-"
            lines.append(f"  {flag} +{r.offset:#05x}  {base_name(r.symbol)[:48]:48s} {target}  {r.note}")
    if verbose and res.diff:
        lines.append("\n" + res.diff)
    return "\n".join(lines)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("address", type=lambda s: int(s, 16))
    ap.add_argument("source", type=Path, nargs="?")
    ap.add_argument("--sym", help="substring of the mangled name, if the annotation can't be used")
    ap.add_argument("--flags", default=DEFAULT_FLAGS)
    args = ap.parse_args()

    src = args.source or find_source(args.address)
    if src is None:
        sys.exit(f"no file under src/ has '// FUNCTION: {args.address:#x}'")
    qualname = next((q for a, q in annotations(src) if a == args.address), None)
    obj_path, log = compile_source(src, args.flags)
    if obj_path is None:
        sys.exit(f"compile failed:\n{log}")
    warnings = [l for l in log.splitlines() if "warning" in l]
    res = compare(Original(), parse_object(obj_path.read_bytes(), obj_path.name), args.address, args.sym, qualname)
    print(report(res))
    # Calls from other files resolve through data/symbols.csv, so a definition
    # under another name breaks them once this function matches (0x40d900, #4596).
    named = sorted(n for n, a in load_symbols().items() if a == args.address)
    if qualname and named and qualname not in named:
        print(f"\nnote: data/symbols.csv names {args.address:#x} {' / '.join(repr(n) for n in named)}, "
              f"but this file defines {qualname!r}. Use that name, or calls to it from other "
              f"files stop resolving once it matches.")
    if warnings:
        print("\ncompiler warnings:\n  " + "\n  ".join(warnings[:10]))
    sys.exit(0 if res.matched else 1)


if __name__ == "__main__":
    main()
