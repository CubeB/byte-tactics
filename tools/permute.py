# /// script
# requires-python = ">=3.11"
# dependencies = [
#     "capstone>=5",
#     "pefile>=2023.2.7",
#     "tree-sitter>=0.23",
#     "tree-sitter-cpp>=0.23",
# ]
# ///
"""Search source rewrites of a near-miss function for one that matches.

    uv run tools/permute.py 0x40d900                       # 15 minutes, 12 jobs
    uv run tools/permute.py 0x40d900 --minutes 30 --seed 7
    uv run tools/permute.py 0x40d900 --file build/scratch/0x40d900/try.cpp
    uv run tools/permute.py --batch 0x40d900 0x4c1ab0 --minutes 15
    uv run tools/permute.py --batch --partials 99 --minutes 15   # every partial at 99% or more

A permuter in the style of the decomp community's decomp-permuter. It applies
small meaning-preserving rewrites (tools/permute_mutate.py) to the annotated
function and the small inline helpers it calls, compiles each candidate with
check.py's compile_source, and scores it against the original with a finer
measure than check.py's similarity: register-only and stack-slot-only
differences cost a little, a reordered instruction more, an inserted or
deleted one most. A hill climb with random restarts keeps the best candidates
and accepts equal-score moves, since near misses usually sit on plateaus.

Output goes to build/permute/<address>/ (never to src/):
  best.cpp      the whole file with the best version found
  best.diff     that file against the starting file
  best.json     scores, status, and the mutations that led to the best
  log.txt       every improvement: time, score before and after, mutations
  stats.json    per mutation kind: tried, compiled, improved
  matches/      every candidate that MATCHes

Verify a result with `uv run tools/check.py <address> build/permute/<address>/best.cpp`.
See docs/permuter.md.
"""

from __future__ import annotations

import argparse
import bisect
import csv
import difflib
import hashlib
import json
import os
import random
import re
import struct
import sys
import time
from collections import Counter
from concurrent.futures import FIRST_COMPLETED, ProcessPoolExecutor, wait
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from check import (DEFAULT_FLAGS, PADDING, ROOT, Original, annotations, compare, compile_source, disasm,  # noqa: E402
                   find_source, link_placeholders, load_symbols, normalise, select_function)
from coff import parse_object  # noqa: E402
from permute_mutate import MUTATIONS, SIMPLIFY, Mutator, TargetSpec, helper_names  # noqa: E402

OUT = ROOT / "build" / "permute"
# /Zd adds COFF line numbers and leaves the code as it is (checked per run),
# which lets the search aim at the statements behind differing instructions.
FOCUS_FLAGS = DEFAULT_FLAGS + " /Zd"
INF = 10 ** 9

# --- scoring ---------------------------------------------------------------------

REG = re.compile(r"\b(?:e?[abcd]x|[abcd][lh]|e?[sd]i|e?bp)\b")
STACK = re.compile(r"\[(?:esp|ebp)(?: [+-] 0x[0-9a-f]+)?\]")
BRANCH = re.compile(r"^(j\w+|loop\w*|call|jmp) (0x[0-9a-f]+)$")

PENALTY_BRANCH = 1      # same branch, different target (code moved around it)
PENALTY_REG = 5         # same instruction, different registers
PENALTY_STACK = 5       # same instruction, different stack slot
PENALTY_OPERAND = 10    # same mnemonic, other operands
PENALTY_REORDER = 60    # an instruction that is in both, at different places
PENALTY_INSDEL = 100    # an instruction only one side has
PENALTY_BYTE = 2        # per byte of size difference


def instruction_penalty(x: str, y: str) -> int:
    bx, by = BRANCH.match(x), BRANCH.match(y)
    if bx and by and bx.group(1) == by.group(1):
        return PENALTY_BRANCH
    if REG.sub("R", x) == REG.sub("R", y):
        return PENALTY_REG
    if STACK.sub("[S]", x) == STACK.sub("[S]", y):
        return PENALTY_STACK
    return PENALTY_OPERAND


def fine_score(theirs: list[str], ours: list[str], hot: set | None = None) -> int:
    """decomp-permuter style distance between two normalised instruction lists.
    Identical instructions are aligned first; inside each differing block the
    rest are paired by mnemonic and cost by how much their operands differ.
    What is left counts as a move when the same instruction is left over on
    the other side, else as an insertion or deletion. `hot`, if given,
    collects the indexes of our instructions in the differing blocks."""
    score = 0
    deleted: Counter = Counter()
    inserted: Counter = Counter()
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, theirs, ours, autojunk=False).get_opcodes():
        if tag == "equal":
            continue
        if hot is not None:
            hot.update(range(j1, j2))
            if j1 == j2 and ours:
                hot.add(min(j1, len(ours) - 1))  # where the missing instructions belong
        a, b = theirs[i1:i2], ours[j1:j2]
        ma = [t.split(" ", 1)[0] for t in a]
        mb = [t.split(" ", 1)[0] for t in b]
        for t2, x1, x2, y1, y2 in difflib.SequenceMatcher(None, ma, mb, autojunk=False).get_opcodes():
            if t2 == "equal":
                for x, y in zip(a[x1:x2], b[y1:y2]):
                    if x != y:
                        score += instruction_penalty(x, y)
            else:
                deleted.update(a[x1:x2])
                inserted.update(b[y1:y2])
    # An unaligned instruction found on the other side is a move, not an
    # insertion plus a deletion; one moved across a push has another [esp+N],
    # and one moved with a register change another register.
    for extra, key in ((0, lambda s: s), (PENALTY_STACK, lambda s: STACK.sub("[S]", s)),
                       (PENALTY_STACK + PENALTY_REG, lambda s: REG.sub("R", STACK.sub("[S]", s)))):
        both = Counter(key(x) for x in deleted.elements()) & Counter(key(y) for y in inserted.elements())
        score += (PENALTY_REORDER + extra) * sum(both.values())
        deleted, inserted = unmatched_after(deleted, both, key), unmatched_after(inserted, both, key)
    score += PENALTY_INSDEL * (sum(deleted.values()) + sum(inserted.values()))
    return score


def unmatched_after(side: Counter, matched: Counter, key) -> Counter:
    """side without the instructions `matched` (counted by key) accounts for."""
    budget = Counter(matched)
    out: Counter = Counter()
    for x in side.elements():
        if budget[key(x)] > 0:
            budget[key(x)] -= 1
        else:
            out[x] += 1
    return out


def line_table(raw: bytes, sec_index: int, symbol: str) -> list[tuple[int, int]]:
    """(section offset, source line) for one function, from the COFF line
    numbers that /Zd adds (it leaves the code itself unchanged). Each
    function's entries start with its symbol index and a 0, and count lines
    from the line of the function's `.bf` symbol."""
    try:
        _, _, _, symptr, nsym, opt, _ = struct.unpack_from("<HHIIIHH", raw, 0)
        hdr = 20 + opt + 40 * (sec_index - 1)
        (plines,) = struct.unpack_from("<I", raw, hdr + 28)
        (nlines,) = struct.unpack_from("<H", raw, hdr + 34)
        strtab = symptr + 18 * nsym

        def sym(i):
            name, _, _, _, _, naux = struct.unpack_from("<8sIhHBB", raw, symptr + 18 * i)
            if name[:4] == b"\0\0\0\0":
                (o,) = struct.unpack_from("<I", name, 4)
                name = raw[strtab + o:raw.index(b"\0", strtab + o)]
            return name.rstrip(b"\0").decode("latin-1"), naux

        out, base = [], None
        for k in range(nlines):
            addr, line = struct.unpack_from("<IH", raw, plines + 6 * k)
            if line == 0:
                base = None
                name, naux = sym(addr)
                if name != symbol:
                    continue
                j = addr + 1 + naux
                while j < nsym:
                    n2, na2 = sym(j)
                    if n2 == ".bf":
                        (base,) = struct.unpack_from("<H", raw, symptr + 18 * (j + 1) + 4)
                        break
                    j += 1 + na2
            elif base is not None:
                out.append((addr, base + line))
        return sorted(out)
    except (struct.error, ValueError, IndexError):
        return []


@dataclass
class Score:
    value: float         # lower is better, 0 is a MATCH
    ratio: float         # check.py's similarity
    status: str          # match | bytes | partial | compile | error | guard
    size: int = 0
    note: str = ""
    hot: tuple = ()      # source lines (1-based) of the differing instructions

    @property
    def pct(self) -> str:
        return f"{self.ratio * 100:.1f}%"


class Scorer:
    """Scores compiled objects for one address, the way check.py compares."""

    def __init__(self, address: int, qualname: str | None, guards: list[tuple[int, str]]):
        self.address = address
        self.qualname = qualname
        self.guards = guards
        self.orig = Original()
        self.symbols = load_symbols()
        self.size = self.orig.sizes.get(address)
        self.theirs = self.orig.read(address, self.size) if self.size else b""
        in_image = self.in_image = lambda v: self.orig.base <= v < self.orig.end
        self.lo, self.hi = address, address + (self.size or 0)
        self.theirs_txt = [normalise(i, self.lo, self.hi, in_image) for i in disasm(self.theirs, address)]

    def score(self, obj, raw: bytes | None = None) -> Score:
        for g_addr, g_q in self.guards:
            res = compare(self.orig, obj, g_addr, None, g_q, self.symbols, quick=True)
            if not res.bytes_match:
                return Score(INF, 0.0, "guard", note=f"{g_addr:#x} no longer matches")
        picked, err = select_function(obj, None, self.qualname)
        if not picked:
            return Score(INF, 0.0, "error", note=err)
        name, sec, start, end = picked
        data, mask = sec.data[start:end], sec.mask()[start:end]
        while data and data[-1] in PADDING and mask[-1]:
            data, mask = data[:-1], mask[:-1]
        theirs = self.theirs
        bytes_match = len(data) == len(theirs) and all(not m or a == b for a, b, m in zip(data, theirs, mask))
        if bytes_match:
            res = compare(self.orig, obj, self.address, None, self.qualname, self.symbols)
            if res.matched:
                return Score(0, 1.0, "match", len(data))
            bad = [r for r in res.refs if r.status == "mismatch"]
            return Score(0.5, 1.0, "bytes", len(data), "; ".join(f"{r.symbol}: {r.note}" for r in bad)[:300])
        shown = disasm(link_placeholders(self.orig, sec, start, end, data, self.address, len(theirs)),
                       self.address)
        ours_txt = [normalise(i, self.lo, self.hi, self.in_image) for i in shown]
        ratio = difflib.SequenceMatcher(None, self.theirs_txt, ours_txt, autojunk=False).ratio()
        hot_idx: set = set()
        value = fine_score(self.theirs_txt, ours_txt, hot_idx) + PENALTY_BYTE * abs(len(data) - len(theirs))
        hot: tuple = ()
        if raw is not None and hot_idx:
            table = line_table(raw, sec.index, name)
            if table:
                offs = [a for a, _ in table]
                lines = set()
                for i in hot_idx:
                    k = bisect.bisect_right(offs, start + shown[i].address - self.address) - 1
                    if k >= 0:
                        lines.add(table[k][1])
                hot = tuple(sorted(lines))
        return Score(max(value, 1), ratio, "partial", len(data), hot=hot)


# --- worker processes ---------------------------------------------------------------

_W: dict = {}


def worker_init(address: int, qualname: str | None, guards, spec: TargetSpec, base_text: str,
                basename: str, weights: dict | None, focus: bool = False):
    try:
        os.nice(10)
    except OSError:
        pass
    work = OUT / f"{address:#x}" / "work" / str(os.getpid())
    work.mkdir(parents=True, exist_ok=True)
    simplifier = Mutator(base_text, spec, {k: SIMPLIFY.get(k, 0) for k in MUTATIONS})
    simplifier.simplify = True
    _W.update(scorer=Scorer(address, qualname, guards), mutator=Mutator(base_text, spec, weights),
              simplifier=simplifier, base_lines=base_text.splitlines(),
              path=work / basename, out_dir=f"permute/{address:#x}/obj/{os.getpid()}", cache={},
              flags=FOCUS_FLAGS if focus else DEFAULT_FLAGS)


WORD = re.compile(r"\w+|->|::|\+\+|--|&&|\|\||<<=|>>=|[-+*/%&|^!<>=]=|<<|>>|\S")


def distance(base_lines: list[str], text: str) -> int:
    """How far text is from the starting file: tokens changed, ignoring spacing."""
    b = text.splitlines()
    d = 0
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, base_lines, b, autojunk=False).get_opcodes():
        if tag == "equal":
            continue
        ta = WORD.findall("\n".join(base_lines[i1:i2]))
        tb = WORD.findall("\n".join(b[j1:j2]))
        for t, x1, x2, y1, y2 in difflib.SequenceMatcher(None, ta, tb, autojunk=False).get_opcodes():
            if t != "equal":
                d += max(x2 - x1, y2 - y1)
    return d


def compile_and_score(text: str) -> Score:
    path: Path = _W["path"]
    path.write_text(text, encoding="latin-1")
    obj_path, log = compile_source(path, _W["flags"], out_dir=_W["out_dir"])
    if obj_path is None:
        err = next((l for l in log.splitlines() if "error" in l), log.strip()[-200:])
        return Score(INF, 0.0, "compile", note=err[-200:])
    try:
        raw = obj_path.read_bytes()
        return _W["scorer"].score(parse_object(raw, obj_path.name), raw)
    except Exception as exc:  # a broken object must not stop the search
        return Score(INF, 0.0, "error", note=repr(exc)[:200])


def evaluate(parent: str, n_mut: int, seed: int, simplify: bool = False, hot: tuple = ()) -> dict:
    """Mutate parent n_mut times (0: score it as is), compile and score.
    `simplify` draws from the mutations that can undo others (SIMPLIFY);
    `hot` lists the parent's source lines behind differing instructions,
    which the mutations then prefer."""
    rng = random.Random(seed)
    cache: dict = _W["cache"]
    mutator = _W["simplifier"] if simplify else _W["mutator"]
    text, names = parent, []
    if n_mut:
        for _ in range(4):
            text, names = mutator.mutate(parent, rng, n_mut, hot=hot)
            if names and digest(text) not in cache:
                break
        if not names:
            return {"text": None, "names": [], "score": None}
    h = digest(text)
    dist = distance(_W["base_lines"], text)
    if h in cache:
        return {"text": text, "names": names, "score": cache[h], "dup": True, "dist": dist}
    score = compile_and_score(text)
    cache[h] = score
    return {"text": text, "names": names, "score": score, "dup": False, "dist": dist}


def digest(text: str) -> str:
    return hashlib.sha1(text.encode("latin-1", "replace")).hexdigest()


def diff_lines(a: str, b: str) -> list[str]:
    return [l for l in difflib.unified_diff(a.splitlines(), b.splitlines(), lineterm="", n=0)
            if l[:1] in "+-" and not l.startswith(("+++", "---"))]


# --- the search ------------------------------------------------------------------------


@dataclass
class Entry:
    text: str
    score: Score
    serial: int
    lineage: list[str] = field(default_factory=list)

    def key(self):
        # lower score first; on a tie the better ratio, then the newest (drift along plateaus)
        return (self.score.value, -self.score.ratio, -self.serial)


@dataclass
class Result:
    address: int
    file: str
    start: Score | None
    best: Score | None
    minutes: float
    evaluated: int
    out: Path
    error: str = ""


def acceptable(s: Score, target: Score) -> bool:
    """Is s at least as good as target, by both the score and check.py's ratio?"""
    if target.status == "match":
        return s.status == "match"
    if target.status == "bytes":
        return s.status in ("match", "bytes")
    return s.status in ("partial", "match", "bytes") and s.value <= target.value and s.ratio >= target.ratio - 1e-9


TOKEN = re.compile(r"\s+|\w+|->|::|\+\+|--|&&|\|\||<<=|>>=|[-+*/%&|^!<>=]=|<<|>>|.", re.S)


def minimize(pool, base_text: str, text: str, target: Score, jobs: int = 12, seconds: float = 120,
             say=print, use_ddmin: bool = False) -> tuple[str, Score]:
    """Undo every part of the change from base_text that the score does not need.

    The search drifts across plateaus, so a winning candidate carries many
    neutral rewrites. They are removed by a search that applies only
    meaning-preserving mutations (most kinds have an inverse) and keeps each
    one that brings the text closer to the start without losing score.

    With use_ddmin a MATCH is first cut down by delta debugging over the text
    diff. That is safe only because the result must still MATCH, and it finds
    smaller diffs, but they can be odd C++ (half of a `do { } while (0)`
    left as a bare `while (0);`), so it is off by default."""
    if use_ddmin and target.status in ("match", "bytes"):
        text, target = ddmin_text(pool, base_text, text, target)
    return simplify(pool, text, target, jobs, seconds, say)


def simplify(pool, text: str, target: Score, jobs: int, seconds: float, say=print) -> tuple[str, Score]:
    cur, cur_score = text, target
    cur_dist = pool.submit(evaluate, text, 0, 0).result()["dist"]
    start_dist = cur_dist
    rng = random.Random(0x5eed)
    t0 = time.time()
    fails = 0
    inflight: set = set()
    while time.time() - t0 < seconds and fails < 2000 and cur_dist > 0:
        while len(inflight) < jobs * 2:
            n = 1 if rng.random() < 0.7 else 2
            inflight.add(pool.submit(evaluate, cur, n, rng.getrandbits(62), True))
        done, _ = wait(list(inflight), timeout=5, return_when=FIRST_COMPLETED)
        for fut in done:
            inflight.discard(fut)
            r = fut.result()
            if r["text"] is not None and r["dist"] < cur_dist and acceptable(r["score"], target):
                cur, cur_score, cur_dist = r["text"], r["score"], r["dist"]
                fails = 0
            else:
                fails += 1
    for fut in inflight:
        fut.cancel()
    wait(list(inflight))
    say(f"  cleanup: {start_dist} -> {cur_dist} tokens changed from the start, in {time.time() - t0:.0f} s")
    return cur, cur_score


def ddmin_text(pool, base_text: str, text: str, target: Score) -> tuple[str, Score]:
    cur, cur_score = text, target
    for split in (lambda s: s.splitlines(keepends=True), TOKEN.findall):
        a, b = split(base_text), split(cur)
        hunks = [op for op in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes() if op[0] != "equal"]

        def build(subset, a=a, b=b):
            out, last = [], 0
            for _, i1, i2, j1, j2 in sorted(subset, key=lambda o: o[1]):
                out += a[last:i1] + b[j1:j2]
                last = i2
            return "".join(out + a[last:])

        cache: dict[str, Score] = {}

        def test_many(subsets):
            texts = [build(s) for s in subsets]
            todo = {t for t in texts if t not in cache}
            futs = {t: pool.submit(evaluate, t, 0, 0) for t in todo}
            for t, f in futs.items():
                cache[t] = f.result()["score"]
            return [cache[t] for t in texts]

        keep = ddmin(hunks, test_many, lambda s: acceptable(s, cur_score))
        new = build(keep)
        s = test_many([keep])[0]
        if acceptable(s, cur_score):
            cur, cur_score = new, s
    return cur, cur_score


def ddmin(items: list, test_many, ok) -> list:
    """Zeller's delta debugging: a 1-minimal sublist of items that passes ok().
    Every subset of one granularity level is tested in one parallel batch."""
    n = 2
    while len(items) >= 2:
        size = len(items)
        chunks = [items[i * size // n:(i + 1) * size // n] for i in range(n)]
        chunks = [c for c in chunks if c]
        comps = [[x for x in items if x not in c] for c in chunks]
        subsets = chunks + comps
        results = test_many(subsets)
        passed = [(len(subsets[i]), i) for i, s in enumerate(results) if ok(s)]
        if passed:
            _, i = min(passed)
            if i < len(chunks):
                items, n = chunks[i], 2
            else:
                items, n = comps[i - len(chunks)], max(n - 1, 2)
            continue
        if n >= size:
            break
        n = min(size, 2 * n)
    if len(items) == 1 and ok(test_many([[]])[0]):
        return []
    return items


def same_code_with_lines(src: Path, base_obj, address: int, qualname: str | None, guards) -> bool:
    """Does /Zd leave the scored functions' bytes alone in this file?"""
    obj_path, _ = compile_source(src, FOCUS_FLAGS, out_dir=f"permute/{address:#x}/base_zd")
    if obj_path is None:
        return False
    zd = parse_object(obj_path.read_bytes(), obj_path.name)
    for a, q in [(address, qualname)] + list(guards):
        x, _ = select_function(base_obj, None, q)
        y, _ = select_function(zd, None, q)
        if not x or not y or x[1].data[x[2]:x[3]] != y[1].data[y[2]:y[3]]:
            return False
    return True


def pick_mutation_count(rng: random.Random) -> int:
    r = rng.random()
    return 1 if r < 0.55 else 2 if r < 0.8 else 3 if r < 0.93 else 4


def permute(address: int, src: Path, minutes: float, jobs: int, seed: int | None,
            patience: float = 0.0, max_minutes: float | None = None, helpers: bool = True,
            elite_size: int = 12, keep_going: bool = False, weights: dict | None = None,
            quiet: bool = False, cleanup: float = 2.0, stall: float = 0.0, focus: bool = True,
            use_ddmin: bool = False) -> Result:
    say = (lambda *a: None) if quiet else (lambda *a: print(*a, flush=True))
    out = OUT / f"{address:#x}"
    (out / "matches").mkdir(parents=True, exist_ok=True)
    base_text = src.read_text(encoding="latin-1")
    ann = annotations(src)
    qualname = next((q for a, q in ann if a == address), None)
    if qualname is None:
        return Result(address, str(src), None, None, 0, 0, out, f"no // FUNCTION: {address:#x} in {src}")
    spec = TargetSpec(address, qualname[1:] if qualname.startswith("=") else None)
    spec.helpers = helper_names(base_text, spec) if helpers else []

    # Other annotated functions in the file that match now must keep matching.
    obj_path, log = compile_source(src, out_dir=f"permute/{address:#x}/base")
    if obj_path is None:
        return Result(address, str(src), None, None, 0, 0, out, "the starting file does not compile:\n" + log)
    base_obj = parse_object(obj_path.read_bytes(), obj_path.name)
    orig = Original()
    symbols = load_symbols()
    guards = []
    for a, q in ann:
        if a != address and compare(orig, base_obj, a, None, q, symbols, quick=True).bytes_match:
            guards.append((a, q))
    focus = focus and same_code_with_lines(src, base_obj, address, qualname, guards)

    rng = random.Random(seed)
    mutator = Mutator(base_text, spec, weights)
    from permute_mutate import Ctx
    ctx = Ctx(base_text, spec, mutator.fi, rng)
    if ctx.error or not ctx.funcs:
        return Result(address, str(src), None, None, 0, 0, out, ctx.error or "no function to mutate")
    say(f"{address:#x}: mutating {', '.join(f.name for f in ctx.funcs)} in {src}"
        + (f"; guarding {', '.join(f'{a:#x}' for a, _ in guards)}" if guards else "")
        + ("" if focus else "; no line numbers, so no focus"))

    stats: dict[str, Counter] = {k: Counter() for k in MUTATIONS}
    log_lines: list[str] = []
    t0 = time.time()
    evaluated = compile_errors = dups = 0
    serial = 0
    seen: dict[str, float] = {}
    elite: list[Entry] = []
    best: Entry | None = None
    top_ratio: Entry | None = None
    start: Score | None = None
    matches = 0
    last_gain = t0
    lowest = INF
    cap = max_minutes if max_minutes is not None else minutes

    def write_best(e: Entry):
        (out / "best.cpp").write_text(e.text, encoding="latin-1")
        diff = difflib.unified_diff(base_text.splitlines(), e.text.splitlines(), str(src), "best.cpp", lineterm="")
        (out / "best.diff").write_text("\n".join(diff) + "\n", encoding="latin-1")

    def summary(final: bool) -> dict:
        return {
            "address": f"{address:#x}", "file": str(src.relative_to(ROOT) if src.is_relative_to(ROOT) else src),
            "start_ratio": start.ratio if start else None, "start_score": start.value if start else None,
            "best_ratio": best.score.ratio if best else None, "best_score": best.score.value if best else None,
            "status": best.score.status if best else None, "note": best.score.note if best else "",
            "best_size": best.score.size if best else None,
            "top_ratio": top_ratio.score.ratio if top_ratio is not None else None,
            "evaluated": evaluated, "compile_errors": compile_errors, "duplicates": dups,
            "minutes": round((time.time() - t0) / 60, 2), "matches": matches, "final": final,
            "helpers": spec.helpers, "guards": [f"{a:#x}" for a, _ in guards],
            "lineage": best.lineage[-200:] if best else [],
        }

    def add_elite(e: Entry):
        elite.append(e)
        elite.sort(key=Entry.key)
        del elite[elite_size:]

    with ProcessPoolExecutor(max_workers=jobs, initializer=worker_init,
                             initargs=(address, qualname, guards, spec, base_text, src.name, weights,
                                       focus)) as pool:
        first = pool.submit(evaluate, base_text, 0, 0).result()
        start = first["score"]
        if start.status in ("compile", "error", "guard"):
            return Result(address, str(src), start, None, 0, 1, out, f"cannot score the starting file: {start.note}")
        best = Entry(base_text, start, serial)
        lowest = start.value
        top_ratio = best
        elite.append(best)
        with (out / "log.txt").open("a") as fh:
            fh.write(f"# {time.strftime('%Y-%m-%d %H:%M')} start {src} at {start.pct}, score {start.value:g}\n")
        seen[digest(base_text)] = start.value
        write_best(best)
        say(f"{address:#x}: start {start.pct} (score {start.value:g}, {start.size} bytes)")
        if start.status in ("match", "bytes"):
            (out / "best.json").write_text(json.dumps(summary(True), indent=1))
            return Result(address, str(src), start, start, 0, 1, out)

        inflight: dict = {}

        def submit():
            nonlocal serial
            r = rng.random()
            if r < 0.06:
                parent = Entry(base_text, start, 0, [])
                n = pick_mutation_count(rng) + 1
            elif r < 0.5:
                parent = elite[0]
                n = pick_mutation_count(rng)
            else:
                weights_ = [1.0 / (i + 1) for i in range(len(elite))]
                parent = rng.choices(elite, weights_)[0]
                n = pick_mutation_count(rng)
            fut = pool.submit(evaluate, parent.text, n, rng.getrandbits(62), False, parent.score.hot)
            inflight[fut] = parent

        def time_left() -> bool:
            el = (time.time() - t0) / 60
            idle = (time.time() - last_gain) / 60
            if el >= cap or (stall > 0 and idle >= stall):
                return False
            if el < minutes:
                return True
            return patience > 0 and idle < patience

        done_matching = False
        while time_left() and not done_matching:
            while len(inflight) < jobs * 2:
                submit()
            done, _ = wait(list(inflight), timeout=5, return_when=FIRST_COMPLETED)
            for fut in done:
                parent = inflight.pop(fut)
                try:
                    res = fut.result()
                except Exception as exc:  # a crashed worker task: note it and go on
                    say(f"{address:#x}: worker error {exc!r}")
                    continue
                if res["text"] is None:
                    continue
                names = res["names"]
                sc: Score = res["score"]
                if res.get("dup"):
                    dups += 1
                    continue
                h = digest(res["text"])
                if h in seen:
                    dups += 1
                    continue
                seen[h] = sc.value
                evaluated += 1
                for k in set(names):
                    stats[k]["tried"] += 1
                if sc.status == "compile":
                    compile_errors += 1
                    for k in set(names):
                        stats[k]["compile_error"] += 1
                    continue
                if sc.status in ("guard", "error"):
                    for k in set(names):
                        stats[k][sc.status] += 1
                    continue
                for k in set(names):
                    stats[k]["compiled"] += 1
                    if sc.value < parent.score.value:
                        stats[k]["improved"] += 1
                    elif sc.value == parent.score.value:
                        stats[k]["equal"] += 1
                serial += 1
                if sc.value < lowest:
                    lowest = sc.value  # search progress, even where best.cpp cannot follow
                    last_gain = time.time()
                    if sc.ratio < start.ratio:
                        (out / "best_search.cpp").write_text(res["text"], encoding="latin-1")
                e = Entry(res["text"], sc, serial, parent.lineage + names)
                worst = elite[-1].key() if len(elite) >= elite_size else None
                if sc.value <= parent.score.value or worst is None or e.key() < worst:
                    add_elite(e)
                if sc.status == "match":
                    matches += 1
                    (out / "matches" / f"match_{matches:03d}.cpp").write_text(res["text"], encoding="latin-1")
                if sc.ratio > top_ratio.score.ratio:
                    # check.py's percentage can disagree with the finer score; keep its best too
                    top_ratio = e
                    (out / "best_ratio.cpp").write_text(e.text, encoding="latin-1")
                # The search climbs the fine score, but best.cpp must never be
                # worse than the start by check.py's percentage (eight register
                # swaps score better than one moved instruction, yet read worse).
                if e.key() < best.key() and (sc.value < best.score.value or sc.ratio > best.score.ratio) \
                        and sc.ratio >= start.ratio - 1e-9:
                    for k in set(names):
                        stats[k]["new_best"] += 1
                    line = (f"{(time.time() - t0) / 60:6.2f} min  score {parent.score.value:g} -> {sc.value:g}"
                            f"  (best was {best.score.value:g}) {sc.pct}  {sc.size} bytes  {'+'.join(names)}")
                    log_lines.append(line)
                    with (out / "log.txt").open("a") as fh:
                        fh.write(line + "\n")
                    best = e
                    write_best(best)
                    (out / "best.json").write_text(json.dumps(summary(False), indent=1))
                    say(f"{address:#x}: {line}")
                    if sc.status in ("match", "bytes") and not keep_going:
                        done_matching = True
        for fut in inflight:
            fut.cancel()
        wait(list(inflight))
        if best.text != base_text and cleanup > 0:
            (out / "best_raw.cpp").write_text(best.text, encoding="latin-1")
            text, sc = minimize(pool, base_text, best.text, best.score, jobs, cleanup * 60, say, use_ddmin)
            say(f"{address:#x}: cleaned the best version to {len(diff_lines(base_text, text))} changed lines "
                f"(score {sc.value:g}, {sc.pct})")
            best = Entry(text, sc, best.serial, best.lineage)
            write_best(best)
        if top_ratio is not best and top_ratio.score.ratio > best.score.ratio and cleanup > 0:
            text, sc = minimize(pool, base_text, top_ratio.text, top_ratio.score, jobs, cleanup * 30, say,
                                use_ddmin)
            top_ratio = Entry(text, sc, top_ratio.serial, top_ratio.lineage)
            (out / "best_ratio.cpp").write_text(text, encoding="latin-1")
        pool.shutdown(wait=True, cancel_futures=True)

    (out / "best.json").write_text(json.dumps(summary(True), indent=1))
    (out / "stats.json").write_text(json.dumps({k: dict(v) for k, v in stats.items() if v}, indent=1))
    mins = (time.time() - t0) / 60
    say(f"{address:#x}: done after {mins:.1f} min, {evaluated} candidates ({compile_errors} did not compile, "
        f"{dups} duplicates): {start.pct} -> {best.score.pct} (score {start.value:g} -> {best.score.value:g})"
        + (" MATCH" if best.score.status == "match" else ""))
    return Result(address, str(src), start, best.score, mins, evaluated, out)


def minimize_file(address: int, src: Path, candidate: Path, jobs: int, seconds: float,
                  use_ddmin: bool = False) -> None:
    """Minimise an existing candidate against the starting file (--minimize)."""
    base_text = src.read_text(encoding="latin-1")
    text = candidate.read_text(encoding="latin-1")
    qualname = next((q for a, q in annotations(src) if a == address), None)
    spec = TargetSpec(address, qualname[1:] if qualname and qualname.startswith("=") else None)
    spec.helpers = helper_names(base_text, spec)
    with ProcessPoolExecutor(max_workers=jobs, initializer=worker_init,
                             initargs=(address, qualname, [], spec, base_text, src.name, None)) as pool:
        score = pool.submit(evaluate, text, 0, 0).result()["score"]
        print(f"{address:#x}: {candidate} scores {score.value:g} ({score.pct}, {score.status}), "
              f"{len(diff_lines(base_text, text))} changed lines")
        new, sc = minimize(pool, base_text, text, score, jobs, seconds, use_ddmin=use_ddmin)
    out = candidate.with_name(candidate.stem + "_min.cpp")
    out.write_text(new, encoding="latin-1")
    print(f"{address:#x}: wrote {out}: score {sc.value:g} ({sc.pct}, {sc.status}), "
          f"{len(diff_lines(base_text, new))} changed lines")


# --- command line -------------------------------------------------------------------------


def partials(minimum: float) -> list[int]:
    with (ROOT / "data/progress.csv").open() as fh:
        rows = [r for r in csv.DictReader(fh) if r["status"] == "partial" and float(r["similarity"] or 0) >= minimum]
    rows.sort(key=lambda r: -float(r["similarity"]))
    return [int(r["address"], 16) for r in rows]


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("addresses", nargs="*", type=lambda s: int(s, 16))
    ap.add_argument("--file", type=Path, help="source to start from (default: the file under src/ with the annotation)")
    ap.add_argument("--batch", action="store_true", help="work through several addresses and print a summary")
    ap.add_argument("--partials", type=float, metavar="PCT",
                    help="with --batch: every partial function at PCT%% or more in data/progress.csv, best first")
    ap.add_argument("--minutes", type=float, default=15.0, help="time budget per function (default 15)")
    ap.add_argument("--patience", type=float, default=0.0,
                    help="after the budget, keep going while the last gain is this recent (minutes)")
    ap.add_argument("--max-minutes", type=float, help="hard limit per function when --patience extends it")
    ap.add_argument("--stall", type=float, default=0.0,
                    help="stop a function early once it has gone this many minutes without a gain")
    ap.add_argument("--jobs", type=int, default=12, help="parallel compiles (default 12)")
    ap.add_argument("--seed", type=int)
    ap.add_argument("--no-helpers", action="store_true", help="only mutate the annotated function itself")
    ap.add_argument("--no-focus", action="store_true",
                    help="do not aim mutations at the source lines behind differing instructions")
    ap.add_argument("--resume", action="store_true", help="start from build/permute/<address>/best.cpp if present")
    ap.add_argument("--keep-going", action="store_true", help="do not stop at the first MATCH")
    ap.add_argument("--only", help="comma-separated mutation kinds to use (see docs/permuter.md)")
    ap.add_argument("--cleanup", type=float, default=2.0,
                    help="minutes to spend undoing neutral changes in the best version (default 2, 0: none)")
    ap.add_argument("--ddmin", action="store_true",
                    help="cut a MATCH down by delta debugging over the text first (smaller, sometimes odd diffs)")
    ap.add_argument("--minimize", type=Path, metavar="CPP",
                    help="only undo the parts of CPP's change that its score does not need; writes CPP_min.cpp")
    args = ap.parse_args()

    try:
        os.nice(10)
    except OSError:
        pass
    addresses = list(args.addresses)
    if args.partials is not None:
        addresses += [a for a in partials(args.partials) if a not in addresses]
    if not addresses:
        ap.error("give an address (or --batch --partials PCT)")
    if len(addresses) > 1 and not args.batch:
        ap.error("several addresses need --batch")
    if args.file and len(addresses) > 1:
        ap.error("--file works with one address")
    if args.minimize:
        src = args.file or find_source(addresses[0])
        if src is None:
            ap.error(f"no file under src/ has '// FUNCTION: {addresses[0]:#x}'")
        minimize_file(addresses[0], src.resolve(), args.minimize.resolve(), args.jobs, args.cleanup * 60,
                      args.ddmin)
        return
    weights = None
    if args.only:
        kinds = set(args.only.split(","))
        unknown = kinds - set(MUTATIONS)
        if unknown:
            ap.error(f"unknown mutation kinds: {', '.join(sorted(unknown))}")
        weights = {k: (w if k in kinds else 0) for k, (_, w) in MUTATIONS.items()}

    results = []
    for address in addresses:
        src = args.file or find_source(address)
        if src is None:
            print(f"{address:#x}: no file under src/ has '// FUNCTION: {address:#x}'")
            continue
        src = src.resolve()
        resumed = OUT / f"{address:#x}" / "best.cpp"
        if args.resume and resumed.exists():
            # keep the starting file out of the output folder, which best.cpp is rewritten in
            start_copy = OUT / f"{address:#x}" / "resume" / src.name
            start_copy.parent.mkdir(parents=True, exist_ok=True)
            start_copy.write_bytes(resumed.read_bytes())
            src = start_copy
        res = permute(address, src, args.minutes, args.jobs, args.seed, args.patience, args.max_minutes,
                      helpers=not args.no_helpers, keep_going=args.keep_going, weights=weights,
                      cleanup=args.cleanup, stall=args.stall, focus=not args.no_focus, use_ddmin=args.ddmin)
        if res.error:
            print(f"{address:#x}: {res.error}")
        results.append(res)

    if args.batch or len(results) > 1:
        print("\n| address | start | best | result | minutes | candidates | best.cpp |")
        print("| --- | ---: | ---: | --- | ---: | ---: | --- |")
        for r in results:
            if r.error:
                print(f"| {r.address:#x} | | | error: {r.error.splitlines()[0][:60]} | | | |")
                continue
            status = {"match": "MATCH", "bytes": "bytes match, reference wrong"}.get(r.best.status, "partial")
            if r.best.status == "partial" and r.best.value < r.start.value:
                status = "improved"
            print(f"| {r.address:#x} | {r.start.pct} | {r.best.pct} | {status} | {r.minutes:.1f} | {r.evaluated} "
                  f"| {r.out.relative_to(ROOT) / 'best.cpp'} |")


if __name__ == "__main__":
    main()
