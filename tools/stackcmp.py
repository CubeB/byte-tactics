"""Match a near-miss function's stack slots to the original's.

    uv run tools/stackcmp.py 0x405980
    uv run tools/stackcmp.py 0x405980 src/unsorted/0x405980.cpp
    uv run tools/stackcmp.py --verify-all          # is /Z7 code-neutral on the matched set?
    uv run tools/stackcmp.py --self-test           # does the depth tracking hold on the original?

When a function differs only in its stack frame, check.py's diff is a wall of
`[esp + 0x1c]` versus `[esp + 0x24]` and there is no way to tell which local is
which. This compiles the candidate once more with /Z7 to read the name and frame
offset of every local from CodeView, aligns our instructions with the original's
(ignoring frame operands and internal jump targets), and prints where the
original keeps each of our locals. The code being compared is the /Z7 build; it
warns when that differs from the normal build.

Every frame access is converted to an offset from the stack pointer at entry
(the return address is at 0, the first parameter at +4, locals below 0), the
convention CodeView uses for these functions. That needs the stack depth at each
instruction, which needs to know how many bytes each call pops: the callee's
`ret N` in the original, the import library's `@N`, the mangled calling
convention, or, failing all of those, the aligned call on the other side.

The last line is a `--stack` list for tools/permute.py, the locals whose slot is
wrong, so the permuter can aim its declaration moves at them.
"""

import argparse
import difflib
import re
import struct
import sys
from collections import Counter
from dataclasses import dataclass, field
from functools import cache
from pathlib import Path

import capstone
from capstone import x86

sys.path.insert(0, str(Path(__file__).resolve().parent))

from check import (DEFAULT_FLAGS, PADDING, ROOT, Original, annotations, compare,  # noqa: E402
                   compile_source, find_source, link_placeholders, load_symbols, normalise,
                   select_function)
from coff import parse_object  # noqa: E402
from linkcheck import (CALLCONV, IMPORT_LIBS, LIBDIR, Demangle, address_of,  # noqa: E402
                       archive_symbols, type_size)

DEBUG_FLAGS = DEFAULT_FLAGS + " /Z7"

# CodeView symbol records (VC5), little-endian.
S_REGISTER = 0x1001
S_BPREL32 = 0x1006
S_LPROC32 = 0x100a
S_GPROC32 = 0x100b
PROC_TYPES = (S_LPROC32, S_GPROC32)

# The stack probe: `mov eax, N; call __chkstk` lowers ESP by N itself.
PROBE_NAMES = ("__chkstk", "__alloca_probe")
SAVED = {x86.X86_REG_EBX, x86.X86_REG_ESI, x86.X86_REG_EDI, x86.X86_REG_EBP}
FRAME_OPERAND = re.compile(r"\[esp[^\]]*\]")


def disasm(code: bytes, va: int) -> list:
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.syntax = capstone.CS_OPT_SYNTAX_INTEL
    md.detail = True
    return list(md.disasm(code, va))


# --- FPO records ----------------------------------------------------------------

@dataclass
class Fpo:
    locals: int     # bytes
    params: int     # bytes
    saved: int      # bytes of saved registers


def fpo_records(orig: Original) -> dict[int, Fpo]:
    out: dict[int, Fpo] = {}
    for d in getattr(orig.pe, "DIRECTORY_ENTRY_DEBUG", []):
        if d.struct.Type != 3:
            continue
        raw = orig.pe.__data__[d.struct.PointerToRawData:d.struct.PointerToRawData + d.struct.SizeOfData]
        for i in range(0, len(raw), 16):
            start, _, locals_dw, params, packed = struct.unpack_from("<IIIHH", raw, i)
            out[orig.base + start] = Fpo(locals_dw * 4, params * 4, ((packed >> 8) & 0x7) * 4)
    return out


def object_fpo(obj, symname: str) -> Fpo | None:
    """Our own FPO record, from the /Z7 object's .debug$F section."""
    for sec in obj.sections:
        if sec.name == ".debug$F" and any(symname in r.symbol for r in sec.relocs) and len(sec.data) >= 16:
            locals_dw, params, packed = struct.unpack_from("<IHH", sec.data, 8)
            return Fpo(locals_dw * 4, params * 4, ((packed >> 8) & 0x7) * 4)
    return None


# --- CodeView locals --------------------------------------------------------------

def cv_records(data: bytes):
    """Yield (record offset, record type, proc name) for every CodeView record."""
    off = 4 if len(data) >= 4 and struct.unpack_from("<I", data, 0)[0] == 4 else 0
    while off + 4 <= len(data):
        reclen, rectyp = struct.unpack_from("<HH", data, off)
        if reclen == 0:
            break
        raw = data[off:off + 2 + reclen]
        name = raw[40:40 + raw[39]].decode("latin-1") if rectyp in PROC_TYPES and len(raw) >= 40 else ""
        yield off, rectyp, name
        off += 2 + reclen


def find_debug_section(obj, symname: str, short: str | None):
    for sec in obj.sections:
        if sec.name == ".debug$S" and any(symname in r.symbol for r in sec.relocs):
            return sec
    for sec in obj.sections:
        if sec.name == ".debug$S" and short and any(
                t in PROC_TYPES and n.split("::")[-1] == short for _, t, n in cv_records(sec.data)):
            return sec
    return None


def parse_locals(sec) -> tuple[list[tuple[str, int]], list[tuple[str, int]]]:
    """(stack locals, register locals): (name, frame offset) and (name, register)."""
    stack: list[tuple[str, int]] = []
    regs: list[tuple[str, int]] = []
    for off, rectyp, _ in cv_records(sec.data):
        raw = sec.data[off:off + 2 + struct.unpack_from("<H", sec.data, off)[0]]
        if rectyp == S_BPREL32 and len(raw) >= 13:
            stack.append((raw[13:13 + raw[12]].decode("latin-1"), struct.unpack_from("<i", raw, 4)[0]))
        elif rectyp == S_REGISTER and len(raw) >= 11:
            regs.append((raw[11:11 + raw[10]].decode("latin-1"), struct.unpack_from("<H", raw, 8)[0]))
    return stack, regs


# --- how many bytes a call pops ---------------------------------------------------

PROBE = "probe"   # the stack probe: lowers ESP by the EAX it was given


@cache
def import_pops() -> dict[str, int]:
    """Undecorated import name -> bytes its callee pops, from the import libraries."""
    out: dict[str, int] = {}
    for lib in IMPORT_LIBS:
        path = LIBDIR / f"{lib}.LIB"
        if not path.exists():
            continue
        for sym in archive_symbols(path):
            m = re.fullmatch(r"__imp__(\w+?)(?:@(\d+))?", sym)
            if m:
                out.setdefault(m.group(1), int(m.group(2) or 0))
    return out


class Callees:
    """Bytes popped by the functions of the original, from their own `ret N`."""

    def __init__(self, orig: Original, symbols: dict[str, int]):
        self.orig = orig
        self.probe = {symbols[n] for n in ("_alloca_probe", "__alloca_probe", "__chkstk", "_chkstk")
                      if n in symbols}
        self.iat: dict[int, str] = {}
        for entry in getattr(orig.pe, "DIRECTORY_ENTRY_IMPORT", []):
            for imp in entry.imports:
                if imp.name:
                    self.iat[imp.address] = imp.name.decode("latin-1")
        self.memo: dict[int, int | str | None] = {}

    def at(self, address: int, depth: int = 0) -> int | str | None:
        if address in self.probe:
            return PROBE
        if address in self.memo:
            return self.memo[address]
        self.memo[address] = None
        size = self.orig.sizes.get(address, 512)
        result = None
        for ins in disasm(self.orig.read(address, size), address):
            if ins.mnemonic == "ret":
                result = ins.operands[0].imm if ins.operands else 0
                break
            if ins.mnemonic == "jmp" and depth < 2 and ins.operands[0].type == x86.X86_OP_IMM \
                    and not address <= ins.operands[0].imm < address + size:
                result = self.at(ins.operands[0].imm, depth + 1)  # a thunk
                break
        self.memo[address] = result
        return result

    def imported(self, slot: int) -> int | None:
        name = self.iat.get(slot)
        return import_pops().get(name) if name else None


def mangled_pops(sym: str) -> int | None:
    """Bytes a callee pops, from its mangled or decorated name."""
    m = re.fullmatch(r"_(\w+)@(\d+)", sym)
    if m:
        return int(m.group(2))
    if not sym.startswith("?"):
        return 0 if re.fullmatch(r"_\w+", sym) else None   # a C function: __cdecl
    d = Demangle(sym[1:])
    try:
        d.qualified()
        access = d.take()
        if access not in "YZ" and access not in "CDKLST":
            d.take()   # `this` qualifiers
        cc = CALLCONV.get(d.take())
        if cc == "__cdecl":
            return 0
        if d.s[d.i] != "@":
            if d.s.startswith("?A", d.i):
                d.i += 2
            d.type()
        total = 0
        while d.i < len(d.s) and d.s[d.i] not in "@Z":
            if d.s[d.i] == "X" and total == 0:
                break
            size = type_size(d.arg())
            if size is None:
                return None
            total += (size + 3) & ~3
        if d.s[d.i:d.i + 2] == "ZZ":
            return 0   # variadic: the caller cleans
        return total
    except (ValueError, IndexError, KeyError):
        return None


def loaded_from(ins_list, i: int):
    """For `call reg`, the `mov reg, [address]` that loaded it (MSVC keeps an
    import called several times in a register), if nothing wrote reg since."""
    op = ins_list[i].operands[0]
    if op.type != x86.X86_OP_REG:
        return None
    for k in range(i - 1, max(-1, i - 200), -1):
        ins = ins_list[k]
        _, written = ins.regs_access()
        if op.reg in written:
            src = ins.operands[1] if ins.mnemonic == "mov" and len(ins.operands) == 2 else None
            if src is not None and src.type == x86.X86_OP_MEM and src.mem.base == 0 and src.mem.index == 0:
                return ins
            return None
    return None


def original_pops(ins_list, i: int, callees: Callees) -> int | str | None:
    ins = ins_list[i]
    op = ins.operands[0]
    if op.type == x86.X86_OP_IMM:
        return callees.at(op.imm)
    if op.type == x86.X86_OP_MEM and op.mem.base == 0 and op.mem.index == 0:
        return callees.imported(op.mem.disp)
    load = loaded_from(ins_list, i)
    return callees.imported(load.operands[1].mem.disp) if load is not None else None


def our_pops(ins_list, i: int, relocs: dict[int, str], callees: Callees,
             symbols: dict[str, int]) -> int | str | None:
    """Bytes a call in our object pops, from the symbol its relocation names."""
    ins = ins_list[i]
    if ins.operands[0].type == x86.X86_OP_REG:
        ins = loaded_from(ins_list, i)
        if ins is None:
            return None
    sym = next((relocs[a] for a in range(ins.address, ins.address + ins.size) if a in relocs), None)
    if sym is None:
        return None
    if sym.startswith("__imp_"):
        m = re.fullmatch(r"__imp__(\w+?)(?:@(\d+))?", sym)
        return int(m.group(2) or 0) if m else None
    if sym in PROBE_NAMES:
        return PROBE
    address = address_of(sym, symbols)
    if address is not None:
        known = callees.at(address)
        if known is not None:
            return known
    return mangled_pops(sym)


# --- stack depth --------------------------------------------------------------------

@dataclass
class Depth:
    delta: list[int | None]             # bytes below entry ESP before each instruction
    guessed: set[int] = field(default_factory=set)    # calls whose pops were inferred
    conflicts: set[int] = field(default_factory=set)  # joins reached at two depths


def _imm(op) -> int | None:
    return op.imm if op.type == x86.X86_OP_IMM else None


def _is_esp(op) -> bool:
    return op.type == x86.X86_OP_REG and op.reg == x86.X86_REG_ESP


def _branch_target(ins) -> int | None:
    if ins.group(capstone.CS_GRP_JUMP) and ins.operands and ins.operands[0].type == x86.X86_OP_IMM:
        return ins.operands[0].imm
    return None


def restored_registers(ins_list) -> set[int]:
    """Callee-saved registers the epilogues pop: their first push on a path is a save."""
    out: set[int] = set()
    for i, ins in enumerate(ins_list):
        if ins.mnemonic != "ret":
            continue
        k = i - 1
        while k >= 0 and ins_list[k].mnemonic in ("pop", "add", "lea", "mov", "leave"):
            p = ins_list[k]
            if p.mnemonic == "pop" and p.operands[0].type == x86.X86_OP_REG and p.operands[0].reg in SAVED:
                out.add(p.operands[0].reg)
            k -= 1
    return out


@dataclass(frozen=True)
class State:
    delta: int
    args: tuple[int, ...] = ()          # sizes of the argument pushes not yet consumed
    saved: frozenset = frozenset()      # callee-saved registers already pushed
    eax: int | None = None              # constant last loaded into EAX (for the probe)


def _pop_args(args: tuple[int, ...], n: int) -> tuple[tuple[int, ...], bool]:
    """Remove n bytes of argument pushes; False if there were not that many."""
    out = list(args)
    while n > 0 and out:
        n -= out.pop()
    return tuple(out), n <= 0


def step(ins, s: State, pops, saves: set[int]) -> tuple[State, bool]:
    """The state after one instruction, and whether it consumed pushes it never saw."""
    m, ops = ins.mnemonic, ins.operands
    eax = s.eax
    if ops and ops[0].type == x86.X86_OP_REG and ops[0].reg == x86.X86_REG_EAX and m not in ("push", "cmp", "test"):
        eax = _imm(ops[1]) if m == "mov" and len(ops) == 2 else None
    if m == "push":
        if ops[0].type == x86.X86_OP_REG and ops[0].reg in saves and ops[0].reg not in s.saved:
            return State(s.delta + 4, s.args, s.saved | {ops[0].reg}, eax), True
        return State(s.delta + 4, s.args + (4,), s.saved, eax), True
    if m == "pop":
        args = s.args[:-1] if s.args else s.args
        return State(s.delta - 4, args, s.saved, eax), True
    if m in ("sub", "add") and len(ops) == 2 and _is_esp(ops[0]):
        n = _imm(ops[1])
        if n is None and ops[1].type == x86.X86_OP_REG and ops[1].reg == x86.X86_REG_EAX:
            n = s.eax
        if n is None:
            return State(s.delta, s.args, s.saved, eax), False
        if m == "sub":   # frame space: whatever was pushed before is part of the frame now
            return State(s.delta + n, (), s.saved, eax), True
        args, _ = _pop_args(s.args, n)
        return State(s.delta - n, args, s.saved, eax), True
    if m == "lea" and _is_esp(ops[0]) and ops[1].mem.base == x86.X86_REG_ESP and ops[1].mem.index == 0:
        n = ops[1].mem.disp
        if n < 0:
            return State(s.delta - n, (), s.saved, eax), True
        args, _ = _pop_args(s.args, n)
        return State(s.delta - n, args, s.saved, eax), True
    if m == "call":
        if pops == PROBE:
            return State(s.delta + (s.eax or 0), (), s.saved, None), s.eax is not None
        args, ok = _pop_args(s.args, pops or 0)
        return State(s.delta - (pops or 0), args, s.saved, None), ok
    return State(s.delta, s.args, s.saved, eax), True


def _ends_block(ins) -> bool:
    return ins.mnemonic == "ret" or ins.group(capstone.CS_GRP_JUMP)


def infer_pops(ins_list, i: int, s: State, known: dict[int, int | str | None], saves: set[int]) -> int:
    """Pops for a call nothing names (a virtual call, a function pointer).

    Try popping all the pending argument pushes, then fewer, and keep the
    largest count under which no later call in the same block pops pushes it
    never saw. That is what tells `push a; push x; call [g]; push eax; call f`
    (g takes x, f takes a and eax) from `push a; push x; call [g]` (g takes both)."""
    n = len(s.args)
    # `call reg; mov ...; add esp, N`: the caller cleans, so the callee popped nothing.
    for j in range(i + 1, min(i + 6, len(ins_list))):
        nxt = ins_list[j]
        if nxt.mnemonic in ("call", "push", "ret") or nxt.group(capstone.CS_GRP_JUMP):
            break
        if nxt.mnemonic == "add" and _is_esp(nxt.operands[0]) and _imm(nxt.operands[1]) is not None:
            if _imm(nxt.operands[1]) in {sum(s.args[n - k:]) for k in range(1, n + 1)}:
                return 0
            break
    for k in range(n, -1, -1):
        pops = sum(s.args[n - k:])
        t, ok = step(ins_list[i], s, pops, saves)
        j = i + 1
        while ok and j < len(ins_list) and not _ends_block(ins_list[j - 1]):
            p = known.get(j)
            if ins_list[j].mnemonic == "call" and p is None:
                break
            t, ok = step(ins_list[j], t, p, saves)
            j += 1
        if ok:
            return pops
    return sum(s.args)


def track_depth(ins_list, known: dict[int, int | str | None]) -> Depth:
    """Forward dataflow over the function's control flow."""
    n = len(ins_list)
    by_addr = {ins.address: i for i, ins in enumerate(ins_list)}
    saves = restored_registers(ins_list)
    states: list[State | None] = [None] * n
    depth = Depth([None] * n)
    if not n:
        return depth
    states[0] = State(0)
    todo = [0]
    while todo:
        i = todo.pop()
        s, ins = states[i], ins_list[i]
        pops = known.get(i)
        if ins.mnemonic == "call" and pops is None:
            pops = infer_pops(ins_list, i, s, known, saves)
            depth.guessed.add(i)
        after, _ = step(ins, s, pops, saves)
        succ = []
        if ins.mnemonic != "ret":
            target = _branch_target(ins)
            if target is not None and target in by_addr:
                succ.append(by_addr[target])
            if ins.mnemonic != "jmp" and i + 1 < n:
                succ.append(i + 1)
        for k in succ:
            if states[k] is None:
                states[k] = after
                todo.append(k)
            elif states[k].delta != after.delta:
                depth.conflicts.add(k)
    for i, s in enumerate(states):
        if s is not None:
            depth.delta[i] = s.delta
    return depth


def frame_offsets(ins, delta: int | None) -> list[int]:
    """Entry-relative offsets of the frame locations an instruction touches.

    No game function keeps an EBP frame (no /GX, no alloca), so every frame
    access is ESP-based."""
    if delta is None:
        return []
    return [op.mem.disp - delta for op in ins.operands
            if op.type == x86.X86_OP_MEM and op.mem.segment == 0 and op.mem.base == x86.X86_REG_ESP]


def frame_key(ins, lo: int, hi: int, in_image) -> str:
    """Instruction text with frame operands and code addresses masked, for alignment."""
    return FRAME_OPERAND.sub("[frame]", normalise(ins, lo, hi, in_image, mask_targets=True))


# --- one side ---------------------------------------------------------------------

@dataclass
class Side:
    ins: list
    fpo: Fpo | None
    depth: Depth | None = None
    offsets: list[list[int]] = field(default_factory=list)

    def valid(self, off: int) -> bool:
        """Inside the locals or the parameters: not the return address, a saved
        register, or an outgoing argument."""
        if self.fpo is None:
            return off != 0
        if 4 <= off < 4 + self.fpo.params:
            return True
        return -self.fpo.locals <= off < 0

    def resolve(self, known: dict[int, int | str | None]) -> None:
        self.depth = track_depth(self.ins, known)
        self.offsets = [frame_offsets(ins, self.depth.delta[i])
                        for i, ins in enumerate(self.ins)]


def align(ours: list[str], theirs: list[str]) -> list[tuple[int, int]]:
    """(ours index, theirs index) for instructions that line up on masked text."""
    pairs = []
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, theirs, ours, autojunk=False).get_opcodes():
        if tag == "equal" or (tag == "replace" and i2 - i1 == j2 - j1):
            pairs += [(j1 + k, i1 + k) for k in range(i2 - i1) if theirs[i1 + k] == ours[j1 + k]]
        elif tag == "replace":
            used: set[int] = set()
            for i in range(i1, i2):
                j = next((j for j in range(j1, j2) if j not in used and theirs[i] == ours[j]), None)
                if j is not None:
                    used.add(j)
                    pairs.append((j, i))
    return pairs


# --- report -----------------------------------------------------------------------

REG_NAMES = {
    0: "ax", 1: "cx", 2: "dx", 3: "bx", 4: "sp", 5: "bp", 6: "si", 7: "di",
    8: "al", 9: "cl", 10: "dl", 11: "bl", 12: "ah", 13: "ch", 14: "dh", 15: "bh",
    17: "eax", 18: "ecx", 19: "edx", 20: "ebx", 21: "esp", 22: "ebp", 23: "esi", 24: "edi",
}


def signed(v: int) -> str:
    return f"+{v:#x}" if v >= 0 else f"-{-v:#x}"


@dataclass
class Local:
    names: list[str]
    start: int
    end: int                     # exclusive: the next local up, or the end of its area
    accesses: int = 0
    votes: Counter = field(default_factory=Counter)   # where the original's paired access puts our start


def build_locals(stack: list[tuple[str, int]], fpo: Fpo | None) -> list[Local]:
    by_offset: dict[int, list[str]] = {}
    for name, value in stack:
        by_offset.setdefault(value, []).append(name)
    starts = sorted(by_offset)
    params_end = 4 + (fpo.params if fpo else 0x100)
    out = []
    for k, start in enumerate(starts):
        nxt = starts[k + 1] if k + 1 < len(starts) else None
        if start < 0:
            end = min(nxt, 0) if nxt is not None else 0
        else:
            end = nxt if nxt is not None else max(params_end, start + 4)
        out.append(Local(by_offset[start], start, end))
    return out


def covering(locals_: list[Local], off: int) -> Local | None:
    return next((loc for loc in locals_ if loc.start <= off < loc.end), None)


def analyse(address: int, src: Path, want: str | None) -> int:
    qualname = next((q for a, q in annotations(src) if a == address), None)
    base_path, log = compile_source(src, out_dir="obj")
    if base_path is None:
        sys.exit(f"compile failed:\n{log}")
    dbg_path, dlog = compile_source(src, DEBUG_FLAGS, out_dir="dbg")
    if dbg_path is None:
        sys.exit(f"debug compile failed:\n{dlog}")
    base = parse_object(base_path.read_bytes(), base_path.name)
    dbg = parse_object(dbg_path.read_bytes(), dbg_path.name)

    picked, err = select_function(dbg, want, qualname)
    if not picked:
        sys.exit(f"cannot pick the function: {err}")
    name, sec, start, end = picked
    base_picked, _ = select_function(base, want, qualname)
    if not base_picked or base_picked[1].data[base_picked[2]:base_picked[3]] != sec.data[start:end]:
        print("warning: /Z7 changed this function's code; the table describes the /Z7 build")

    orig = Original()
    symbols = load_symbols()
    score = compare(orig, base, address, want, qualname, symbols)
    size = orig.sizes.get(address, end - start)
    in_image = lambda v: orig.base <= v < orig.end
    lo, hi = address, address + size

    data, mask = sec.data[start:end], sec.mask()[start:end]
    while data and data[-1] in PADDING and mask[-1]:
        data, mask = data[:-1], mask[:-1]
    ours = Side(disasm(link_placeholders(orig, sec, start, end, data, address, size), address),
                object_fpo(dbg, name))
    theirs = Side(disasm(orig.read(address, size), address), fpo_records(orig).get(address))

    callees = Callees(orig, symbols)
    relocs = {address + r.offset - start: r.symbol for r in sec.relocs if start <= r.offset < end}
    ours_known = {i: our_pops(ours.ins, i, relocs, callees, symbols)
                  for i, ins in enumerate(ours.ins) if ins.mnemonic == "call"}
    theirs_known = {i: original_pops(theirs.ins, i, callees)
                    for i, ins in enumerate(theirs.ins) if ins.mnemonic == "call"}

    keys_o = [frame_key(i, lo, hi, in_image) for i in ours.ins]
    keys_t = [frame_key(i, lo, hi, in_image) for i in theirs.ins]
    pairs = align(keys_o, keys_t)

    # A call one side cannot name takes its pops from the call it lines up with.
    for i, j in pairs:
        if ours.ins[i].mnemonic != "call":
            continue
        if ours_known.get(i) is None and theirs_known.get(j) is not None:
            ours_known[i] = theirs_known[j]
        elif theirs_known.get(j) is None and ours_known.get(i) is not None:
            theirs_known[j] = ours_known[i]
    ours.resolve(ours_known)
    theirs.resolve(theirs_known)

    stack, regs = [], []
    dbg_sec = find_debug_section(dbg, name, qualname.split("::")[-1] if qualname else None)
    if dbg_sec is not None:
        stack, regs = parse_locals(dbg_sec)
    locals_ = build_locals(stack, ours.fpo)

    for i, offs in enumerate(ours.offsets):
        for off in offs:
            loc = covering(locals_, off)
            if loc:
                loc.accesses += 1
    paired_theirs: set[int] = set()
    unplaced = 0
    for i, j in pairs:
        do, dt = ours.offsets[i], theirs.offsets[j]
        if len(do) != len(dt):
            continue
        for oo, ot in zip(do, dt):
            if not (ours.valid(oo) and theirs.valid(ot)):
                unplaced += 1
                continue
            paired_theirs.add(ot)
            loc = covering(locals_, oo)
            if loc:
                loc.votes[ot - (oo - loc.start)] += 1

    print(f"{address:#x}  {name}  original {size} bytes, ours {len(data)} bytes  ->  {score.status}")
    fmt = lambda f: f"{f.locals:#x} locals + {f.saved:#x} saved, {f.params:#x} params" if f else "unknown"
    print(f"frame: ours {fmt(ours.fpo)}   original {fmt(theirs.fpo)}")
    print(f"aligned frame accesses: {sum(sum(loc.votes.values()) for loc in locals_)}")

    rows, wrong = [], []
    for loc in sorted(locals_, key=lambda loc: -loc.start):
        names = "/".join(loc.names)
        if not loc.accesses:
            rows.append((names, signed(loc.start), "-", "unused"))
            continue
        if not loc.votes:
            rows.append((names, signed(loc.start), "-", "not paired"))
            continue
        target, count = loc.votes.most_common(1)[0]
        total = sum(loc.votes.values())
        if target == loc.start:
            status = "ok"
        elif loc.start > 0:
            status = "param differs"
        else:
            status = "moved"
            wrong.extend(loc.names)
        rows.append((names, signed(loc.start), f"{signed(target)} ({count}/{total})", status))

    if rows:
        width = max(len(r[0]) for r in rows + [("local",)])
        print(f"\n  {'local':<{width}}  {'our slot':>10}  {'original':>16}  status")
        for names, ours_s, orig_s, status in rows:
            print(f"  {names:<{width}}  {ours_s:>10}  {orig_s:>16}  {status}")
    else:
        print("\nno stack locals in CodeView (all in registers or optimised away)")

    unpaired = sorted({off for offs in theirs.offsets for off in offs
                       if theirs.valid(off) and off not in paired_theirs})
    if unpaired:
        print("\noriginal frame offsets no aligned access of ours reaches: "
              + ", ".join(signed(s) for s in unpaired[:24]) + (" ..." if len(unpaired) > 24 else ""))
    if regs:
        print("\nin registers: " + ", ".join(f"{n}={REG_NAMES.get(r, f'r{r}')}" for n, r in regs))
    notes = []
    for label, side in (("ours", ours), ("original", theirs)):
        if side.depth.guessed:
            notes.append(f"{label}: pops inferred for {len(side.depth.guessed)} call(s) at "
                         + ", ".join(f"{side.ins[i].address:#x}" for i in sorted(side.depth.guessed)[:6]))
        if side.depth.conflicts:
            notes.append(f"{label}: stack depth disagrees at join "
                         + ", ".join(f"{side.ins[i].address:#x}" for i in sorted(side.depth.conflicts)[:6]))
    if unplaced:
        notes.append(f"{unplaced} aligned access(es) fell outside the frame on one side and were skipped")
    if notes:
        print("\nnotes:\n  " + "\n  ".join(notes))
    if wrong:
        print("\npermute --stack " + ",".join(dict.fromkeys(wrong)))
    return 0


# --- checks of the tool itself ------------------------------------------------------

def self_test() -> int:
    """Track the stack depth through every function of the original and count
    frame accesses that land somewhere no local can be (the return address, a
    saved register, past the parameters). Outgoing arguments written with
    `mov [esp], x` also land outside the frame, so the count is not zero."""
    import csv
    orig = Original()
    symbols = load_symbols()
    callees = Callees(orig, symbols)
    fpo = fpo_records(orig)
    game = [int(r["address"], 16) for r in csv.DictReader((ROOT / "data/functions.csv").open())
            if r["kind"] == "game"]
    bad, total, conflicted = [], 0, 0
    for address in game:
        if address not in fpo:
            continue
        ins = disasm(orig.read(address, orig.sizes[address]), address)
        side = Side(ins, fpo[address])
        side.resolve({i: original_pops(ins, i, callees) for i, x in enumerate(ins) if x.mnemonic == "call"})
        offs = [o for os in side.offsets for o in os]
        if not offs:
            continue
        total += 1
        conflicted += bool(side.depth.conflicts)
        # Below the locals is where outgoing arguments go; only these three are impossible.
        if any(0 <= o < 4 or o >= 4 + side.fpo.params for o in offs):
            bad.append(address)
    print(f"{total - len(bad)}/{total} functions place every frame access on a local, a parameter "
          f"or an outgoing argument; {conflicted} have a join reached at two depths")
    for address in bad[:20]:
        print(f"  {address:#x}")
    return 0


def verify_one(row) -> tuple[str, str]:
    address = int(row["address"], 16)
    src = ROOT / row["file"]
    qualname = next((q for a, q in annotations(src) if a == address), None)
    obj_path, log = compile_source(src, DEBUG_FLAGS, out_dir=f"dbgverify/{address:#x}")
    if obj_path is None:
        return row["address"], "compile failed"
    obj = parse_object(obj_path.read_bytes(), obj_path.name)
    res = compare(Original(), obj, address, None, qualname, load_symbols(), quick=True)
    return row["address"], "ok" if res.bytes_match else f"differs ({res.status})"


def verify_all(jobs: int) -> int:
    import csv
    from concurrent.futures import ProcessPoolExecutor
    rows = [r for r in csv.DictReader((ROOT / "data/progress.csv").open()) if r["status"] == "matched"]
    print(f"compiling {len(rows)} matched functions with /Z7 ...")
    bad = []
    with ProcessPoolExecutor(max_workers=jobs) as pool:
        for n, (address, result) in enumerate(pool.map(verify_one, rows), 1):
            if result != "ok":
                bad.append((address, result))
            if n % 250 == 0:
                print(f"  {n}/{len(rows)}")
    print(f"{len(rows) - len(bad)}/{len(rows)} unchanged by /Z7")
    for address, result in bad[:40]:
        print(f"  {address}  {result}")
    return 1 if bad else 0


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("address", nargs="?", help="function address, e.g. 0x405980")
    ap.add_argument("source", type=Path, nargs="?")
    ap.add_argument("--sym", help="substring of the mangled name, if the annotation can't be used")
    ap.add_argument("--verify-all", action="store_true",
                    help="check that /Z7 leaves every matched function's code bytes alone")
    ap.add_argument("--self-test", action="store_true",
                    help="check the stack-depth tracking against every function of the original")
    ap.add_argument("--jobs", type=int, default=8, help="parallel compiles for --verify-all")
    args = ap.parse_args()

    if args.verify_all:
        sys.exit(verify_all(args.jobs))
    if args.self_test:
        sys.exit(self_test())
    if not args.address:
        ap.error("an address is required unless --verify-all or --self-test is given")
    address = int(args.address, 16)
    src = args.source or find_source(address)
    if src is None:
        sys.exit(f"no file under src/ has '// FUNCTION: {address:#x}'")
    sys.exit(analyse(address, src, args.sym))


if __name__ == "__main__":
    main()
