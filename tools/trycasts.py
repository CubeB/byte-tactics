"""Remove redundant `((T*)x)->` casts from a file, one group at a time.

    uv run tools/trycasts.py src/weapons/weapon_types.cpp
    uv run tools/trycasts.py --dry-run src/orders/unit_orders.cpp

A group is one cast type and one cast operand: every `((T*)x)->` in the file
whose x is the same variable or member chain. For each group the casts come
out (`((T*)x)->` becomes `x->`), the file's functions are run through
tools/checkall.py, and the edit is kept only when every one still prints
MATCH; otherwise the file is restored. A local `void* x` whose every use is a
cast to one type T is tried as its own group too: the declaration becomes
`T* x` and the casts come out. Parameters are never retyped: their type is
part of the decorated name (docs/cleanup-roadmap.md, phase 2).

The file is restored to its last known-matching state when the run is
interrupted (SIGINT, SIGTERM or an error), so a killed run leaves a file that
still compiles and matches. Re-run the tool after a merge: a group kept
because its variable's type was wrong may work once the declarations around
it are fixed (docs/cleanup-roadmap.md, phase 3).

Only `((T*)x)->` casts are touched: a bare `(T*)x` used as a value, and
byte-offset access such as `*(T*)(p + 0x..)`, are other phases' input.
"""

import argparse
import os
import re
import signal
import subprocess
import sys
import tempfile
from pathlib import Path

from sources import ROOT, annotations, relative

CHECKALL = ROOT / "tools" / "checkall.py"

IDENT = r"[A-Za-z_]\w*"
# x: a variable, or a chain of member accesses (`g_game->sound`,
# `parser.current`) possibly taken by address (`&file`).
OPERAND = r"&?\s*" + IDENT + r"(?:\s*(?:->|\.)\s*" + IDENT + r"(?:\s*\[[^\[\]()]*\])?)*"
# The cast the issue is about: `((T*)x)` followed by `->`. The type is
# textually checked with TYPE below so expressions such as `((a+b)*c)->`
# (a multiplication) are not mistaken for casts.
CAST_ARROW = re.compile(r"\(\s*\((?P<type>[^()]*)\)\s*(?P<operand>" + OPERAND + r")\s*\)\s*->")
# A type as a cast spells it: `T`, `std::T`, `unsigned char`, `const T*`.
TYPE = re.compile(r"(?:const\s+)?(?:unsigned\s+|signed\s+|long\s+|short\s+)*" + IDENT +
                  r"(?:\s*::\s*" + IDENT + r")*(?:\s*<[^()<>]*>)?\s*\*+$")
# A declaration of `void* x`; `void** x` and a cast's `(void*)x` do not match.
DECL = re.compile(r"void\s*\*\s*(?P<name>" + IDENT + r")(?=\s*[=;,)\[])")


class Interrupted(Exception):
    """A signal asked the run to stop; the file in hand is put back."""


def same(text: str) -> str:
    """Whitespace-free form, for comparing types and operands."""
    return re.sub(r"\s+", "", text)


def tidy(text: str) -> str:
    """Single-spaced form, for writing a type back into the source."""
    return re.sub(r"\s+", " ", text).strip()


def canonical_type(text: str) -> str:
    """A type as source usually spells it: no space before the stars."""
    return re.sub(r"\s*\*", "*", tidy(text))


def code_mask(text: str) -> bytearray:
    """1 for every character in code, 0 inside a string, char or comment."""
    mask = bytearray(b"\x01") * len(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c in "\"'":
            quote = c
            mask[i] = 0
            i += 1
            while i < n and text[i] != quote:
                mask[i] = 0
                if text[i] == "\\" and i + 1 < n:
                    i += 1
                    mask[i] = 0
                i += 1
            if i < n:
                mask[i] = 0
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            mask[i:j] = b"\x00" * (j - i)
            i = j - 1
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            mask[i:j] = b"\x00" * (j - i)
            i = j - 1
        i += 1
    return mask


def brace_depths(text: str, mask: bytearray) -> list[int]:
    """The brace depth just before each character; braces in strings and
    comments are text, not scope."""
    depths = [0] * (len(text) + 1)
    depth = 0
    for i, c in enumerate(text):
        depths[i] = depth
        if mask[i]:
            if c == "{":
                depth += 1
            elif c == "}":
                depth -= 1
    depths[len(text)] = depth
    return depths


def is_local(text: str, depths: list[int], start: int) -> bool:
    """True when the `void*` at `start` is a function's local: not a
    parameter (outside any brace) and not a class member (the enclosing
    class definition's `}` is followed by `;`)."""
    d = depths[start]
    if d < 1:
        return False
    j = start - 1
    while j >= 0 and not (text[j] == "{" and depths[j] == d - 1):
        j -= 1
    if j < 0:
        return False
    k = start + 1
    while k < len(text) and not (text[k] == "}" and depths[k] == d):
        k += 1
    if k >= len(text):
        return False
    return not text[k + 1:].lstrip().startswith(";")


def replacement(operand: str) -> str:
    """`x->` for a variable or member chain, `(&x)->` for an address: the
    arrow applies to the last member of a chain anyway, but `&x->` would
    parse as `&(x->..)`."""
    return "(" + operand + ")->" if operand.startswith("&") else operand + "->"


def group_key(match: re.Match) -> tuple[str, str]:
    return same(match.group("type")), same(match.group("operand"))


def arrow_groups(text: str, mask: bytearray) -> dict[tuple[str, str], int]:
    """The `((T*)x)->` groups in a file, keyed by (type, operand)."""
    groups: dict[tuple[str, str], int] = {}
    for m in CAST_ARROW.finditer(text):
        if not mask[m.start()] or not TYPE.match(m.group("type").strip()):
            continue
        key = group_key(m)
        groups[key] = groups.get(key, 0) + 1
    return groups


def remove_group(text: str, key: tuple[str, str]) -> tuple[str, int]:
    """Take the casts of one group out of a file."""
    removed = 0

    def sub(m: re.Match) -> str:
        nonlocal removed
        if group_key(m) != key:
            return m.group(0)
        removed += 1
        return replacement(m.group("operand").strip())

    return CAST_ARROW.sub(sub, text), removed


def use_cast_type(text: str, mask: bytearray, start: int, name: str) -> str | None:
    """The type a use of `name` at `start` is cast to, or None when the use
    is not `(T*)name` as the cast's operand (a `(T*)name->f` casts `name->f`,
    not `name`)."""
    if not mask[start]:
        return None
    after = text[start + len(name):]
    nxt = next((c for c in after if not c.isspace()), "")
    if nxt == "" or nxt == "-" or nxt == "." or nxt == "[" or nxt == "_" or nxt.isalnum():
        return None
    pre = text[:start]
    m = re.search(r"\(\s*\(([^()]*)\)\s*$", pre) or re.search(r"\(\s*([^()]*)\)\s*$", pre)
    if not m:
        return None
    t = m.group(1).strip()
    return canonical_type(t) if TYPE.match(t) else None


def retype_candidates(text: str, mask: bytearray, depths: list[int]) -> dict[str, str]:
    """Every local `void* x` all of whose uses in the file cast it to one
    type, as x -> that cast's type. Parameters are not declared at depth, and
    a name with a use that is not a cast cannot join: the declaration and
    that use would disagree once the casts came out."""
    candidates: dict[str, str] = {}
    decls: dict[str, list[int]] = {}
    for m in DECL.finditer(text):
        if not mask[m.start()] or not is_local(text, depths, m.start()):
            continue
        decls.setdefault(m.group("name"), []).append(m.start("name"))
    for name, starts in decls.items():
        used = [i for m in re.finditer(r"\b" + re.escape(name) + r"\b", text)
                if mask[i := m.start()] and i not in starts]
        types = {use_cast_type(text, mask, i, name) for i in used}
        if used and None not in types and len(types) == 1:
            candidates[name] = types.pop()
    return candidates


def retype(text: str, name: str, cast_type: str) -> tuple[str, int, int]:
    """Change every local `void* name` to `cast_type name` and take the
    group's `((cast_type)name)->` casts out. Returns (text, casts, decls)."""
    mask = code_mask(text)
    depths = brace_depths(text, mask)
    decls = 0

    def sub(m: re.Match) -> str:
        nonlocal decls
        if m.group("name") != name or not mask[m.start()] or not is_local(text, depths, m.start()):
            return m.group(0)
        decls += 1
        return cast_type + " " + m.group("name")

    text = DECL.sub(sub, text)
    text, casts = remove_group(text, (same(cast_type), name))
    return text, casts, decls


def checkall(addresses: list[int]) -> tuple[bool, int, str]:
    """Run a file's functions through tools/checkall.py. Returns (all
    matched, matched count, output)."""
    cmd = [sys.executable, str(CHECKALL), *(f"{a:#x}" for a in addresses)]
    proc = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    output = proc.stdout + proc.stderr
    matched = 0
    for line in output.splitlines():
        if re.match(r"^0x[0-9a-f]+\b", line) and re.search(r"\bMATCH\b", line):
            matched += 1
    return matched == len(addresses), matched, output


def write_atomic(path: Path, text: str) -> None:
    """Replace a file's contents in one step: an interrupted write never
    leaves a half-written file behind."""
    fd, tmp = tempfile.mkstemp(dir=path.parent, prefix=".trycasts-", suffix=".cpp")
    try:
        with os.fdopen(fd, "w", encoding="latin-1", newline="") as fh:
            fh.write(text)
        os.chmod(tmp, path.stat().st_mode)
        os.replace(tmp, path)
    except BaseException:
        Path(tmp).unlink(missing_ok=True)
        raise


def restore(path: Path, good: str) -> None:
    if path.read_text(encoding="latin-1") != good:
        write_atomic(path, good)


def run_file(path: Path, dry_run: bool) -> tuple[int, int, int, int]:
    """Try every group of one file. Returns (removed casts, removed groups,
    kept casts, kept groups)."""
    addresses = [a for a, _ in annotations(path)]
    text = path.read_text(encoding="latin-1")
    if not addresses:
        print(f"{relative(path)}: no functions to check")
        return 0, 0, 0, 0
    mask = code_mask(text)
    depths = brace_depths(text, mask)
    groups = arrow_groups(text, mask)
    retypes = retype_candidates(text, mask, depths)
    queue: list[tuple[str, tuple[str, str]]] = []
    for key in groups:
        if key[1] in retypes and same(retypes[key[1]]) == key[0]:
            continue  # `void* x`: retyping it is the group's edit
        queue.append(("group", key))
    for name, cast_type in retypes.items():
        queue.append(("retype", (cast_type, name)))

    good = text
    removed_casts = removed_groups = kept_casts = kept_groups = 0
    try:
        for kind, key in queue:
            if dry_run:
                if kind == "group":
                    print(f"{relative(path)}  (({key[0]}){key[1]})->  x{groups[key]}  would try")
                else:
                    print(f"{relative(path)}  void* {key[1]} -> {key[0]} {key[1]}  would try")
                continue
            if kind == "group":
                candidate, count = remove_group(good, key)
                label = f"(({key[0]}){key[1]})->"
            else:
                candidate, count, decls = retype(good, key[1], key[0])
                label = f"void* {key[1]} -> {key[0]} {key[1]}"
            if candidate == good:
                continue
            write_atomic(path, candidate)
            matched_all, matched, output = checkall(addresses)
            if matched_all:
                good = candidate
                removed_casts += count
                removed_groups += 1
                print(f"{relative(path)}  {label}  x{count}  REMOVED  ({matched} of {len(addresses)} MATCH)")
            else:
                kept_casts += count
                kept_groups += 1
                why = "compile failed" if "compile failed" in output else f"{matched} of {len(addresses)} MATCH"
                print(f"{relative(path)}  {label}  x{count}  kept  ({why})")
                restore(path, good)
    finally:
        if not dry_run and path.exists():
            restore(path, good)
    return removed_casts, removed_groups, kept_casts, kept_groups


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="+", type=Path, help="source files to try")
    ap.add_argument("--dry-run", action="store_true", help="list the groups that would be tried")
    args = ap.parse_args()

    def stop(signum, frame):
        raise Interrupted(signum)

    signal.signal(signal.SIGINT, stop)
    signal.signal(signal.SIGTERM, stop)
    removed = kept = 0
    try:
        for path in args.files:
            if not path.is_file():
                sys.exit(f"no such file: {path}")
        for path in args.files:
            r, g, kc, kg = run_file(path, args.dry_run)
            removed += r
            kept += kc
            if not args.dry_run:
                print(f"{relative(path)}: {r} casts removed in {g} groups; "
                      f"{kc} casts kept in {kg} groups")
        if not args.dry_run:
            print(f"total: {removed} casts removed, {kept} casts in kept groups")
    except (Interrupted, KeyboardInterrupt):
        print("\ninterrupted: the file in hand was restored to its last matching state",
              file=sys.stderr)
        sys.exit(130)


if __name__ == "__main__":
    main()
