"""Rename identifiers everywhere they are spelt, then check that nothing changed.

    uv run tools/rename.py OLD NEW [OLD NEW ...]   # rename, then run the checks
    uv run tools/rename.py --from FILE             # the pairs from a CSV (old,new[,evidence])
    uv run tools/rename.py OLD NEW --dry-run       # what would change, and what is refused
    uv run tools/rename.py OLD NEW --no-check      # rename only
    uv run tools/rename.py OLD NEW --full          # also link.py --carve and linkcmp.py
    uv run tools/rename.py --from FILE --join --keep-going
                                                   # several views to one type, skipping the
                                                   # pairs that are refused

Phase 2 of docs/tidy-up.md: placeholder names (`Class_<address>`,
`FUN_<address>`, `DAT_<address>`, the `X_<address>` views) become real ones
where the evidence gives them, and the several placeholder names one type
has across the files become that type's one name. A function's or a class's
name is part of the decorated names the linker matches, so every file that
spells it changes at once.

What it rewrites:

  src/**/*.cpp, include/*.h   OLD as a whole word in code and in comments,
                              never inside a string or character literal; in
                              the decorated symbol of a `// FUNCTION:` or
                              `// ENTRY:` annotation too. In the generated
                              headers only where it merges no two names: files
                              match only at those headers' exact symbol counts
                              (docs/c2-regalloc.md), and a second declaration
                              of one name counts nothing
  data/aliases.csv            inside the names, decorated or not (the table is
  data/symbols.csv            kept by hand; symbols.csv is rebuilt by
                              tools/progress.py from the files, and rewritten
                              here only so that check.py agrees until then)
  data/modules.csv, docs/*.md, AGENTS.md
                              OLD as a whole word, but not in a --join: there
                              the docs keep the view names they tell apart

What it refuses, pair by pair:

  - a NEW that is not an identifier, or is a C++ keyword;
  - two names of one file that would become one: a file spelling OLD that
    already spells NEW, or another OLD renamed to the same NEW (merging two
    types in one file is phase 3, not a rename);
  - a NEW that data/symbols.csv gives another address after the rename;
  - a NEW that is already a type in other files, unless --join says OLD's
    views are views of that type (the evidence: tools/gametypes.py --explain);
  - an OLD declared as a local or a parameter in a function whose frame
    layout follows its locals' names: one with a `try` or `__try` block, or one
    built with `/Od` (docs/agent-guide.md; docs/linking.md, "The gap regions as
    source");
  - an OLD that names a gap entry label (`// ENTRY: 0x...`): its public symbol
    is made from the address.

Any refusal stops the rename, unless --keep-going, which renames the pairs
that pass and lists the others.

The checks, unless --no-check: tools/progress.py (every function that
matched before still matches), tools/globals.py (link/ follows the new
names), tools/place.py --write-layout (the shipped MD5, and data/layout.csv
unchanged: it holds no names, so a change means a rename moved a piece within
its object, as renaming file statics can reorder an object's .bss) and
tools/place.py --no-orig (the same MD5 from data/layout.csv alone); with
--full also tools/link.py --carve and tools/linkcmp.py. On a
failure the files stay renamed for a look; `git checkout -- .` undoes it.
"""

import argparse
import csv
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

from sources import ROOT, source_files

WORD = r"[A-Za-z0-9_]"
IDENT = re.compile(r"[A-Za-z_][A-Za-z0-9_]*$")
TOKEN = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
KEYWORDS = set("""
    asm auto bool break case catch char class const const_cast continue default delete do double
    dynamic_cast else enum explicit export extern false float for friend goto if inline int long
    mutable namespace new operator private protected public register reinterpret_cast return short
    signed sizeof static static_cast struct switch template this throw true try typedef typeid
    typename union unsigned using virtual void volatile wchar_t while
    __asm __cdecl __stdcall __fastcall __thiscall __declspec __try __except __finally __leave __int64
""".split())
TEXT_FILES = ["data/modules.csv", "AGENTS.md", "docs/*.md"]
NAME_TABLES = ["data/aliases.csv", "data/symbols.csv"]
SYMBOLS = ROOT / "data/symbols.csv"
PROGRESS = ROOT / "data/progress.csv"
LAYOUT = ROOT / "data/layout.csv"
ANNOTATED = re.compile(r"(//\s*(?:FUNCTION|ENTRY):\s*0x[0-9a-fA-F]+\s+)(\S+)")
TRAILING = re.compile(r"^(.*?\S)(\s{2,})(//.*?)(\r?)$")


# --- the scanner ------------------------------------------------------------------------

def segments(text: str) -> list[tuple[str, str]]:
    """The text cut into ("code" | "comment" | "literal", text) pieces, in order."""
    out, i, n, start, kind = [], 0, len(text), 0, "code"

    def cut(at: int, new_kind: str) -> None:
        nonlocal start, kind
        if at > start:
            out.append((kind, text[start:at]))
        start, kind = at, new_kind

    while i < n:
        c = text[i]
        if text.startswith("//", i):
            cut(i, "comment")
            end = text.find("\n", i)
            i = n if end < 0 else end
            cut(i, "code")
            continue
        if text.startswith("/*", i):
            cut(i, "comment")
            end = text.find("*/", i + 2)
            i = n if end < 0 else end + 2
            cut(i, "code")
            continue
        if c in "\"'":
            cut(i, "literal")
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            i = min(j + 1, n)
            cut(i, "code")
            continue
        i += 1
    cut(n, "code")
    return out


def code_of(text: str) -> str:
    return "".join(piece if kind == "code" else " " * len(piece) for kind, piece in segments(text))


class Renamer:
    """Every pair at once: one regex for whole words, one for names inside
    decorated symbols (after a non-word character, after the code of a
    class V, struct U, union T or enum W4: `PAUUnit@@`, `IURect_0046e160::`,
    or after the code of a special member: `??0` and `??1` the constructor and
    destructor, `??_G` and `??_E` the deleting destructors, `??_7` the vtable,
    `??_R2` to `??_R4` the RTTI records)."""

    def __init__(self, pairs: list[tuple[str, str]]):
        self.table = dict(pairs)
        names = "|".join(re.escape(o) for o in sorted(self.table, key=len, reverse=True))
        self.word = re.compile(rf"(?<!{WORD})(?:{names})(?!{WORD})")
        self.decorated = re.compile(rf"(?:(?<!{WORD})|(?<=[UVT4])|(?<=\?\?[01])|(?<=\?\?_[GE789DF])"
                                    rf"|(?<=\?\?_R[234]))(?:{names})(?!{WORD})")

    def words(self, text: str) -> str:
        return self.word.sub(lambda m: self.table[m.group(0)], text)

    def symbols(self, text: str) -> str:
        return self.decorated.sub(lambda m: self.table[m.group(0)], text)

    def source(self, text: str) -> str:
        out = []
        for kind, piece in segments(text):
            if kind == "comment":
                piece = ANNOTATED.sub(lambda m: m.group(1) + self.symbols(m.group(2)), piece)
            if kind != "literal":
                piece = self.words(piece)
            out.append(piece)
        new = "".join(out)
        if new == text:
            return text
        # Keep trailing comments in their column where a name got shorter or longer.
        old_lines, new_lines = text.split("\n"), new.split("\n")
        for i, (o, n) in enumerate(zip(old_lines, new_lines)):
            if o == n:
                continue
            mo, mn = TRAILING.match(o), TRAILING.match(n)
            if mo and mn and mo.group(3) == mn.group(3):
                column = len(mo.group(1)) + len(mo.group(2))
                new_lines[i] = mn.group(1) + " " * max(2, column - len(mn.group(1))) + mn.group(3) + mn.group(4)
        return "\n".join(new_lines)


# --- what is refused ------------------------------------------------------------------------

def frame_bodies(text: str, code: str) -> list[str]:
    """The bodies (with their parameter lists) of the functions in a file whose
    frame follows their locals' names: every function of a file built with /Od,
    else those with a try block."""
    od = re.search(r"^//\s*FLAGS:.*/Od", text, re.M) is not None
    if not od and not re.search(r"\btry\s*\{|\b__try\b", code):
        return []
    bodies = []
    for m in re.finditer(r"\)\s*(?:const\s*)?\{", code):
        depth, j = 0, m.end() - 1
        while j < len(code):
            depth += {"{": 1, "}": -1}.get(code[j], 0)
            j += 1
            if depth == 0:
                break
        body = code[code.rfind("(", 0, m.start() + 1):j]
        if od or re.search(r"\btry\s*\{|\b__try\b", body):
            bodies.append(body)
    return bodies


def declared_local(name: str, body: str) -> bool:
    decl = re.compile(rf"(?:[\w>]\s*[*&\s]\s*|[*&]){re.escape(name)}\s*(?:[;=,)\[]|$)", re.M)
    for m in decl.finditer(body):
        before = body[:m.start() + 1].rstrip()
        # `x = name;` or `f(a, name)` uses the name; a declaration has a type
        # word before it (not an operator, `return` or a comma alone).
        prev = re.search(r"([A-Za-z_]\w*|[>*&])\s*[*&\s]*$", before + " ")
        if prev and prev.group(1) not in ("return", "case", "goto", "delete", "throw", "sizeof", "new"):
            return True
    return False


def entry_names(texts: dict[Path, str]) -> set[str]:
    out = set()
    for text in texts.values():
        for m in re.finditer(r"//\s*ENTRY:\s*0x([0-9a-fA-F]+)(?:\s+(\S+))?", text):
            out.add(f"FUN_{int(m.group(1), 16):08x}")
            if m.group(2):
                out.add(m.group(2).lstrip("_").split("@")[0])
    return out


class Tree:
    """What the refusals need from every source file, read once."""

    def __init__(self, texts: dict[Path, str]):
        self.texts = texts
        self.headers = {p for p in texts if p.suffix == ".h"}
        self.codes = {p: code_of(t) for p, t in texts.items()}
        self.tokens = {p: set(TOKEN.findall(c)) for p, c in self.codes.items()}
        # A file that includes one of the generated headers sees its names too.
        self.included: dict[Path, set[str]] = defaultdict(set)
        for p, text in texts.items():
            for inc in re.findall(r'^\s*#\s*include\s*["<]([^">]+)[">]', text, re.M):
                header = ROOT / "include" / inc
                if header in self.tokens and header != p:
                    self.included[p] |= self.tokens[header]
        self.frames = {p: b for p, t in texts.items() if (b := frame_bodies(t, self.codes[p]))}
        self.entries = entry_names(texts)
        self.types: dict[str, list[Path]] = defaultdict(list)
        for p, c in self.codes.items():
            for m in re.finditer(r"\b(?:struct|class|union|enum)\s+([A-Za-z_]\w*)", c):
                if p not in self.types[m.group(1)]:
                    self.types[m.group(1)].append(p)


def refusals(pairs: list[tuple[str, str]], tree: Tree, join: bool) -> dict[tuple[str, str], list[str]]:
    """The reasons each refused pair is refused."""
    out: dict[tuple[str, str], list[str]] = defaultdict(list)
    olds = {o for o, _ in pairs}
    groups: dict[str, set[str]] = defaultdict(set)
    for old, new in pairs:
        groups[new].add(old)
    rel = lambda p: str(p.relative_to(ROOT))  # noqa: E731
    for old, new in pairs:
        if not IDENT.match(new) or new in KEYWORDS:
            out[(old, new)].append(f"{new!r} is not a name C++ allows")
        if not IDENT.match(old):
            out[(old, new)].append(f"{old!r} is not an identifier")
        if old in tree.entries:
            out[(old, new)].append(f"{old} names a gap entry label (// ENTRY:), whose symbol the annotation makes")
        others = (groups[new] - {old}) | ({new} if new not in olds else set())
        clash = [p for p, toks in tree.tokens.items()
                 if old in toks and (toks | tree.included[p]) & others and p not in tree.headers]
        if clash:
            seen_there = others & (tree.tokens[clash[0]] | tree.included[clash[0]])
            out[(old, new)].append(f"{old} and {' / '.join(sorted(seen_there))} "
                                   f"would both be {new} in {rel(clash[0])}"
                                   + (f" and {len(clash) - 1} more" if len(clash) > 1 else ""))
        for p, bodies in tree.frames.items():
            if old in tree.tokens[p] and any(declared_local(old, b) for b in bodies):
                out[(old, new)].append(f"{old} is a local in a function whose frame follows its locals' "
                                       f"names ({rel(p)})")
        types = [p for p in tree.types.get(new, []) if old not in tree.tokens[p]]
        if types and not join and new not in olds:
            out[(old, new)].append(f"{new} is already a type in {len(types)} file(s) ({rel(types[0])}, ...): "
                                   f"renaming {old} to it makes their views one type; pass --join if the "
                                   f"evidence says they are (tools/gametypes.py --explain {new})")
    # The name table after the rename: one address per name.
    if SYMBOLS.exists():
        renamer = Renamer(pairs)
        seen: dict[str, tuple[str, str]] = {}
        for r in csv.DictReader(SYMBOLS.open()):
            name = renamer.symbols(r["name"])
            if name in seen and seen[name][0] != r["address"] and (name != r["name"] or seen[name][1] != name):
                culprits = [pr for pr in pairs if pr[1] in name]
                for pr in culprits or pairs[:1]:
                    out[pr].append(f"data/symbols.csv would give {name} two addresses "
                                   f"({seen[name][0]}, {r['address']})")
            seen.setdefault(name, (r["address"], r["name"]))
    return out


# --- the rewrite and the checks ------------------------------------------------------------

def sources_and_headers() -> list[Path]:
    return source_files() + sorted((ROOT / "include").glob("*.h"))


def header_pairs(pairs: list[tuple[str, str]], tokens: set[str]) -> list[tuple[str, str]]:
    """The pairs a generated header takes: none that would make two of its
    names one (that would change its symbol count)."""
    groups: dict[str, set[str]] = defaultdict(set)
    for old, new in pairs:
        if old in tokens:
            groups[new].add(old)
    olds = {o for o, _ in pairs}
    return [(old, new) for old, new in pairs
            if len(groups[new]) + (new in tokens and new not in olds) <= 1]


def apply(pairs: list[tuple[str, str]], texts: dict[Path, str], dry_run: bool, docs: bool = True) -> list[str]:
    renamer = Renamer(pairs)
    changed: list[str] = []

    def save(path: Path, before: str, after: str) -> None:
        if after != before:
            changed.append(str(path.relative_to(ROOT)))
            if not dry_run:
                path.write_bytes(after.encode("latin-1"))

    for path, text in texts.items():
        if path.suffix == ".h":
            own = header_pairs(pairs, set(TOKEN.findall(code_of(text))))
            save(path, text, Renamer(own).source(text) if own else text)
            continue
        save(path, text, renamer.source(text))
    for pattern in NAME_TABLES:
        path = ROOT / pattern
        if path.exists():
            text = path.read_bytes().decode("latin-1")
            save(path, text, renamer.symbols(text))
    for pattern in TEXT_FILES if docs else []:
        for path in sorted(ROOT.glob(pattern)):
            text = path.read_bytes().decode("latin-1")
            save(path, text, renamer.words(text))
    return changed


def run(*cmd: str) -> tuple[int, str]:
    proc = subprocess.run(["uv", "run", *cmd], cwd=ROOT, capture_output=True, text=True)
    return proc.returncode, proc.stdout + proc.stderr


def matched() -> dict[str, str]:
    with PROGRESS.open() as fh:
        return {r["address"]: r["status"] for r in csv.DictReader(fh)}


def checks(full: bool, before: dict[str, str], layout: str) -> list[str]:
    failures = []
    print("tools/progress.py ...", flush=True)
    _, out = run("tools/progress.py", "--quiet")
    print("  " + out.strip().splitlines()[-1] if out.strip() else "  (no output)")
    after = matched()
    lost = sorted(a for a, s in before.items() if s == "matched" and after.get(a) != "matched")
    if lost:
        failures.append(f"{len(lost)} function(s) no longer match: {' '.join(lost[:12])}")
    # globals.py first: place.py lays out link/data.cpp, which it writes.
    print("tools/globals.py ...", flush=True)
    code, out = run("tools/globals.py")
    if code:
        failures.append("tools/globals.py failed:\n" + out[-2000:])
    for label, args in (("tools/place.py --write-layout", ("tools/place.py", "--write-layout")),
                        ("tools/place.py --no-orig", ("tools/place.py", "--no-orig"))):
        print(f"{label} ...", flush=True)
        _, out = run(*args)
        line = next((l for l in out.splitlines() if l.startswith("MD5 ")), "no MD5 line")
        print("  " + line[:100])
        if "the shipped exe" not in line or "NOT the shipped exe" in line:
            failures.append(f"{label}: not the shipped exe")
        if label.endswith("--write-layout") and LAYOUT.read_text() != layout:
            failures.append("data/layout.csv changed: a piece moved within its object "
                            "(renamed file statics can reorder .bss); `git diff data/layout.csv`")
    if full:
        print("tools/link.py --carve --map ...", flush=True)
        code, out = run("tools/link.py", "--carve", "--map")
        if code:
            failures.append("tools/link.py --carve failed:\n" + out[-2000:])
        print("tools/linkcmp.py ...", flush=True)
        code, out = run("tools/linkcmp.py")
        print("  " + "\n  ".join(out.strip().splitlines()[-4:]))
        if code or re.search(r"(?m)^references that differ", out):
            failures.append("tools/linkcmp.py found references that differ")
    return failures


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("names", nargs="*", help="OLD NEW pairs")
    ap.add_argument("--from", dest="table", type=Path, help="a CSV of old,new[,evidence] rows")
    ap.add_argument("--dry-run", action="store_true", help="print what would change and stop")
    ap.add_argument("--no-check", action="store_true", help="rename without running the checks")
    ap.add_argument("--full", action="store_true", help="also link.py --carve and linkcmp.py")
    ap.add_argument("--join", action="store_true",
                    help="allow a NEW that is already a type elsewhere: OLD's views join that type")
    ap.add_argument("--keep-going", action="store_true", help="rename the pairs that pass, list the others")
    args = ap.parse_args()
    if len(args.names) % 2:
        ap.error("names come in OLD NEW pairs")
    pairs = list(zip(args.names[::2], args.names[1::2]))
    if args.table:
        with args.table.open() as fh:
            pairs += [(r["old"], r["new"]) for r in csv.DictReader(fh)]
    if not pairs:
        ap.error("nothing to rename")
    if len({o for o, _ in pairs}) != len(pairs):
        sys.exit("an OLD name appears twice among the pairs")
    texts = {p: p.read_bytes().decode("latin-1") for p in sources_and_headers()}
    tree = Tree(texts)
    while True:
        refused = refusals(pairs, tree, args.join)
        if not refused:
            break
        print(f"refused {len(refused)} of {len(pairs)} pair(s):")
        for (old, new), why in sorted(refused.items()):
            print(f"  {old} -> {new}: {'; '.join(dict.fromkeys(why))}")
        if not args.keep_going:
            sys.exit(2)
        # Without the refused pairs, others may now pass (or clash): look again.
        pairs = [p for p in pairs if p not in refused]
        if not pairs:
            sys.exit(2)
    before = matched()
    layout = LAYOUT.read_text()
    # A join gives many views one name: the docs keep the names they tell apart.
    changed = apply(pairs, texts, args.dry_run, docs=not args.join)
    files = [f for f in changed if f.startswith("src/")]
    print(f"{len(pairs)} pair(s): {'would change' if args.dry_run else 'changed'} {len(changed)} file(s), "
          f"{len(files)} under src/")
    if args.dry_run or args.no_check:
        return
    failures = checks(args.full, before, layout)
    if failures:
        print("FAILED:\n  " + "\n  ".join(failures))
        sys.exit(1)
    print("every check passes")


if __name__ == "__main__":
    main()
