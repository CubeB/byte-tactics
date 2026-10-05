"""Rename an identifier everywhere it is spelt, then check that nothing changed.

    uv run tools/rename.py OLD NEW [OLD NEW ...]   # rename, then run the checks
    uv run tools/rename.py --from FILE             # the pairs from a CSV (old,new[,evidence])
    uv run tools/rename.py OLD NEW --dry-run       # what would change, and what is refused
    uv run tools/rename.py OLD NEW --no-check      # rename only
    uv run tools/rename.py OLD NEW --full          # also link.py --carve and linkcmp.py

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
                              `// ENTRY:` annotation too
  data/aliases.csv            inside the names, decorated or not (the table is
  data/symbols.csv            kept by hand; symbols.csv is rebuilt by
                              tools/progress.py from the files, and rewritten
                              here only so that check.py agrees until then)
  data/modules.csv, docs/*.md, AGENTS.md
                              OLD as a whole word

What it refuses:

  - a NEW that is not an identifier, or is a C++ keyword;
  - a NEW that a file spelling OLD already spells: two things would share one
    name there (merging two types into one is phase 3, not a rename);
  - a NEW that data/symbols.csv gives another address after the rename;
  - a NEW that is already a type in other files, unless --join says OLD's
    views are views of that type (the evidence: tools/gametypes.py --explain);
  - an OLD declared as a local or a parameter in a function whose frame
    layout follows its locals' names: one with a `try` or `__try` block, or one
    built with `/Od` (docs/agent-guide.md; docs/linking.md, "The gap regions as
    source");
  - an OLD that names a gap entry label (`// ENTRY: 0x...`): its public symbol
    is made from the address.

The checks, unless --no-check: tools/progress.py (every function that
matched before still matches), tools/place.py --write-layout (the shipped
MD5, and data/layout.csv kept current), tools/place.py --no-orig (the same MD5
from data/layout.csv alone) and tools/globals.py (link/ follows the new
names); with --full also tools/link.py --carve and tools/linkcmp.py. On a
failure the files stay renamed for a look; `git checkout -- .` undoes it.
"""

import argparse
import csv
import re
import subprocess
import sys
from pathlib import Path

from sources import ROOT, SRC, function_addresses, source_files

WORD = r"[A-Za-z0-9_]"
IDENT = re.compile(r"[A-Za-z_][A-Za-z0-9_]*$")
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
ANNOTATED = re.compile(r"(//\s*(?:FUNCTION|ENTRY):\s*0x[0-9a-fA-F]+\s+)(\S+)")


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
        if kind == "code":
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


def word(name: str) -> re.Pattern:
    return re.compile(rf"(?<!{WORD}){re.escape(name)}(?!{WORD})")


def decorated(name: str) -> re.Pattern:
    """The name inside a decorated or undecorated symbol: after a non-word
    character, or after the code of a class (V), struct (U), union (T) or enum
    (W4) in a decorated name (`PAUUnit@@`, `IURect_0046e160::`)."""
    return re.compile(rf"(?:(?<!{WORD})|(?<=[UVT4])){re.escape(name)}(?!{WORD})")


def rewrite_source(text: str, pairs: list[tuple[str, str]]) -> str:
    out = []
    for kind, piece in segments(text):
        if kind != "literal":
            for old, new in pairs:
                if kind == "comment":
                    piece = ANNOTATED.sub(lambda m: m.group(1) + decorated(old).sub(new, m.group(2)), piece)
                piece = word(old).sub(new, piece)
        out.append(piece)
    return "".join(out)


def code_of(text: str) -> str:
    return "".join(piece if kind == "code" else " " * len(piece) for kind, piece in segments(text))


# --- what is refused ------------------------------------------------------------------------

def frame_bodies(path: Path, text: str) -> list[str]:
    """The bodies of the functions in a file whose frame follows their locals'
    names: every function of a file built with /Od, else those with a try block."""
    code = code_of(text)
    od = re.search(r"^//\s*FLAGS:.*/Od", text, re.M) is not None
    bodies = []
    for m in re.finditer(r"\)\s*(?:const\s*)?\{", code):
        depth, j = 0, m.end() - 1
        while j < len(code):
            depth += {"{": 1, "}": -1}.get(code[j], 0)
            j += 1
            if depth == 0:
                break
        # The parameter list too: parameters are locals of the frame.
        head = code.rfind("(", 0, m.start() + 1)
        body = code[head:j]
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


def entry_names() -> set[str]:
    out = set()
    for src in source_files():
        for m in re.finditer(r"//\s*ENTRY:\s*0x([0-9a-fA-F]+)(?:\s+(\S+))?", src.read_text(errors="replace")):
            out.add(f"FUN_{int(m.group(1), 16):08x}")
            if m.group(2):
                out.add(m.group(2).lstrip("_").split("@")[0])
    return out


def refusals(pairs: list[tuple[str, str]], texts: dict[Path, str], join: bool = False) -> list[str]:
    problems = []
    for old, new in pairs:
        if not IDENT.match(new) or new in KEYWORDS:
            problems.append(f"{new!r} is not a name C++ allows")
        if not IDENT.match(old):
            problems.append(f"{old!r} is not an identifier")
    if problems:
        return problems
    olds = {o for o, _ in pairs}
    entries = entry_names()
    for old, new in pairs:
        if old in entries:
            problems.append(f"{old} names a gap entry label (// ENTRY:), whose symbol the annotation makes")
        w_old, w_new = word(old), word(new)
        both = []
        for path, text in texts.items():
            code = code_of(text)
            if not w_old.search(code):
                continue
            if new not in olds and w_new.search(code):
                both.append(path)
            for body in frame_bodies(path, text):
                if declared_local(old, body):
                    problems.append(f"{old} is a local in a function whose frame follows its locals' names "
                                    f"({path.relative_to(ROOT)})")
                    break
        types = [p for p, text in texts.items()
                 if re.search(rf"\b(?:struct|class|union|enum)\s+{re.escape(new)}\b", code_of(text))]
        if types and not join and new not in olds:
            problems.append(f"{new} is already a type in {len(types)} file(s) "
                            f"({', '.join(str(p.relative_to(ROOT)) for p in types[:3])}): renaming {old} to it "
                            f"makes their views one type; pass --join if the evidence says they are "
                            f"(tools/gametypes.py --explain {new})")
        if both:
            problems.append(f"{new} is already used where {old} is: "
                            + ", ".join(str(p.relative_to(ROOT)) for p in both[:5])
                            + (f" and {len(both) - 5} more" if len(both) > 5 else ""))
    # The name table after the rename: one address per name.
    if SYMBOLS.exists():
        rows = list(csv.DictReader(SYMBOLS.open()))
        seen: dict[str, tuple[str, bool]] = {}
        for r in rows:
            name = r["name"]
            for old, new in pairs:
                name = decorated(old).sub(new, name)
            renamed = name != r["name"]
            if name in seen and seen[name][0] != r["address"] and (renamed or seen[name][1]):
                problems.append(f"data/symbols.csv would give {name} two addresses "
                                f"({seen[name][0]}, {r['address']})")
            seen.setdefault(name, (r["address"], renamed))
    return problems


# --- the rewrite and the checks ------------------------------------------------------------

def sources_and_headers() -> list[Path]:
    return source_files() + sorted((ROOT / "include").glob("*.h"))


def apply(pairs: list[tuple[str, str]], dry_run: bool) -> dict[str, int]:
    changed: dict[str, int] = {}

    def save(path: Path, before: str, after: str) -> None:
        if after != before:
            changed[str(path.relative_to(ROOT))] = sum(len(word(o).findall(before)) for o, _ in pairs) or 1
            if not dry_run:
                path.write_bytes(after.encode("latin-1"))

    for path in sources_and_headers():
        text = path.read_bytes().decode("latin-1")
        save(path, text, rewrite_source(text, pairs))
    for pattern in NAME_TABLES:
        path = ROOT / pattern
        if path.exists():
            text = path.read_bytes().decode("latin-1")
            new = text
            for old, nw in pairs:
                new = decorated(old).sub(nw, new)
            save(path, text, new)
    for pattern in TEXT_FILES:
        for path in sorted(ROOT.glob(pattern)):
            text = path.read_bytes().decode("latin-1")
            new = text
            for old, nw in pairs:
                new = word(old).sub(nw, new)
            save(path, text, new)
    return changed


def run(*cmd: str) -> tuple[int, str]:
    proc = subprocess.run(["uv", "run", *cmd], cwd=ROOT, capture_output=True, text=True)
    return proc.returncode, proc.stdout + proc.stderr


def matched() -> dict[str, str]:
    with PROGRESS.open() as fh:
        return {r["address"]: r["status"] for r in csv.DictReader(fh)}


def checks(full: bool, before: dict[str, str]) -> list[str]:
    failures = []
    print("tools/progress.py ...", flush=True)
    _, out = run("tools/progress.py", "--quiet")
    print("  " + out.strip().splitlines()[-1] if out.strip() else "  (no output)")
    after = matched()
    lost = sorted(a for a, s in before.items() if s == "matched" and after.get(a) != "matched")
    if lost:
        failures.append(f"{len(lost)} function(s) no longer match: {' '.join(lost[:12])}")
    for label, args in (("tools/place.py --write-layout", ("tools/place.py", "--write-layout")),
                        ("tools/place.py --no-orig", ("tools/place.py", "--no-orig"))):
        print(f"{label} ...", flush=True)
        _, out = run(*args)
        line = next((l for l in out.splitlines() if l.startswith("MD5 ")), "no MD5 line")
        print("  " + line[:100])
        if "the shipped exe" not in line or "NOT the shipped exe" in line:
            failures.append(f"{label}: not the shipped exe")
    print("tools/globals.py ...", flush=True)
    code, out = run("tools/globals.py")
    if code:
        failures.append("tools/globals.py failed:\n" + out[-2000:])
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
    args = ap.parse_args()
    if len(args.names) % 2:
        ap.error("names come in OLD NEW pairs")
    pairs = list(zip(args.names[::2], args.names[1::2]))
    if args.table:
        with args.table.open() as fh:
            pairs += [(r["old"], r["new"]) for r in csv.DictReader(fh)]
    if not pairs:
        ap.error("nothing to rename")
    news = [n for _, n in pairs]
    if len(set(news)) != len(news) or len({o for o, _ in pairs}) != len(pairs):
        sys.exit("a name appears twice among the pairs")
    texts = {p: p.read_bytes().decode("latin-1") for p in sources_and_headers()}
    problems = refusals(pairs, texts, args.join)
    if problems:
        print("refused:\n  " + "\n  ".join(problems))
        sys.exit(2)
    before = matched()
    changed = apply(pairs, args.dry_run)
    files = [f for f in changed if f.startswith("src/")]
    print(f"{'would change' if args.dry_run else 'changed'} {len(changed)} file(s), {len(files)} under src/")
    for f in sorted(changed)[:20] if args.dry_run else []:
        print("  " + f)
    if args.dry_run or args.no_check:
        return
    failures = checks(args.full, before)
    if failures:
        print("FAILED:\n  " + "\n  ".join(failures))
        sys.exit(1)
    print("every check passes")


if __name__ == "__main__":
    main()
