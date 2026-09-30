"""Write explicit calling conventions where the source relied on the old default.

The original was built with /Gz (__stdcall as the default for free functions),
and tools/check.py compiles with /Gz too (issue #2290). Source written before
that switch left __cdecl implicit, so this adds it where the exe shows __cdecl:

  - game functions whose epilogue is a plain `ret` although they take stack
    arguments (the caller cleans up), and entry points the exe's function map
    does not list (hand-written code called as __cdecl);
  - hand-declared CRT functions, operator new/delete, and functions passed to
    atexit;
  - function-pointer declarators with parameters and no convention (they were
    __cdecl pointers under the old default).

Class members are left alone (__thiscall whatever the default). Run it only
on source written for the old default: tools/review.sh does so for pull
requests branched before the switch.

    uv run tools/fix_conventions.py [--dry-run] [src/unsorted/0x....cpp ...]
"""

import argparse
import csv
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from check import Original  # noqa: E402
from ctx import callee_pops  # noqa: E402

CONVENTIONS = ("__cdecl", "__stdcall", "__fastcall", "__thiscall", "WINAPI", "CALLBACK", "APIENTRY",
               "__pascal", "PASCAL", "WINAPIV", "_cdecl", "cdecl")
# CRT functions that sources sometimes declare by hand instead of including a header.
CRT = ("atexit", "malloc", "free", "calloc", "realloc", "memcpy", "memset", "memmove", "memcmp",
       "strlen", "strcpy", "strcat", "strcmp", "strncpy", "strncmp", "strchr", "strrchr", "strstr",
       "sprintf", "wsprintfA", "printf", "atoi", "atol", "qsort", "rand", "srand", "abs", "labs",
       "_strcmpi", "_stricmp", "stricmp", "strcmpi", "_strnicmp", "strnicmp", "toupper", "tolower",
       "fopen", "fclose", "fread", "fwrite", "fseek", "ftell", "exit", "_exit", "time", "_ftol",
       "_beginthread", "_beginthreadex", "_endthread", "rename", "remove", "_unlink", "unlink",
       "_chdrive", "_chdir", "chdir", "_getcwd", "getcwd", "_getdrive", "_findfirst", "_findnext",
       "_findclose", "_mkdir", "mkdir", "_rmdir", "_access", "_open", "_close", "_read", "_write",
       "_lseek", "getenv", "_splitpath", "_makepath", "_itoa", "_ltoa", "sscanf", "vsprintf",
       "_vsnprintf", "_snprintf", "strtok", "strtol", "strtoul", "_strlwr", "_strupr", "strspn",
       "strcspn", "strpbrk", "memchr", "floor", "ceil", "sqrt", "sin", "cos", "atan", "atan2",
       "fabs", "pow", "_hypot", "clock", "localtime", "gmtime", "mktime", "_stat", "fgets",
       "fputs", "fprintf", "fscanf", "_fullpath", "_strdup", "strdup", "signal", "raise", "abort")


def cdecl_addresses() -> set[int]:
    orig = Original()
    out = set()
    for r in csv.DictReader(open(ROOT / "data/functions.csv")):
        if r["kind"] != "game" or int(r["params"] or 0) <= 0:
            continue
        va, size = int(r["address"], 16), int(r["size"])
        pops = callee_pops(orig, va, size)
        if pops and 0 in pops:
            out.add(va)
    return out


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("files", nargs="*")
    args = ap.parse_args()

    cdecl = cdecl_addresses()
    print(f"{len(cdecl)} game functions clean up after the caller (__cdecl)")
    known = {int(r["address"], 16) for r in csv.DictReader(open(ROOT / "data/functions.csv"))}
    unlisted = set()
    for path in sorted((ROOT / "src").rglob("*.cpp")):
        for m in re.finditer(r"\bFUN_([0-9a-f]{8})\b", path.read_bytes().decode("latin-1")):
            if int(m.group(1), 16) not in known:
                unlisted.add(int(m.group(1), 16))
    print(f"{len(unlisted)} called entry points are not in the function map (hand-written, __cdecl)")
    cdecl |= unlisted
    # Functions whose epilogue does not show the convention (a tail jump) but
    # that are passed where a __cdecl pointer is expected.
    cdecl |= {0x4609a0}
    names = "|".join([f"FUN_{a:08x}" for a in sorted(cdecl)] + list(CRT))
    # A declaration or definition: a return type, then the name and "(", with
    # no convention written between them. Call sites have no type in front.
    decl = re.compile(
        r"^(?P<lead>\s*(?:extern\s+(?:\"C\"\s+)?)?(?:static\s+)?(?:inline\s+)?(?:__declspec\([^)]*\)\s+)?"
        r"(?:const\s+|unsigned\s+|signed\s+|struct\s+|class\s+)*[A-Za-z_][\w:<>]*[\s\*&]+)"
        r"(?P<name>" + names + r")\s*\(")
    newdel = re.compile(r"^(?P<lead>\s*(?:extern\s+)?(?:inline\s+)?void\s*(?:\*\s*)?)(?P<name>operator\s+(?:new|delete)(?:\[\])?)\s*\(")
    fptr = re.compile(r"(\b(?:void|int|char|short|long|unsigned|signed|float|double|bool|BOOL|DWORD|UINT|HRESULT|LRESULT|[A-Z][A-Za-z0-9_]*)\s*\**\s*\(\s*)\*(?=\s*\w*\s*\)\s*\((?!\s*\)|\s*void\s*\)))")
    keywords = ("return", "else", "case", "goto", "delete", "new", "throw", "sizeof", "do")

    files = [Path(f).resolve() for f in args.files] or sorted((ROOT / "src").rglob("*.cpp"))
    changed = 0
    for path in files:
        text = path.read_bytes().decode("latin-1")
        out, edits = [], 0
        # Brace depths that open a class/struct/union body: members there are
        # __thiscall whatever the default, so only static members are touched.
        depth, class_depths, pending = 0, set(), False
        for line in text.splitlines(keepends=True):
            code = line.split("//", 1)[0]
            in_class = depth in class_depths
            m = decl.match(code) or newdel.match(code)
            if m and in_class and "static" not in m.group("lead"):
                m = None
            if re.match(r"\s*(?:typedef\s+)?(?:class|struct|union)\s+\w*[^;]*$", code) and not re.search(r"\)\s*$", code):
                pending = True
            for ch in code:
                if ch == "{":
                    depth += 1
                    if pending:
                        class_depths.add(depth)
                        pending = False
                elif ch == "}":
                    class_depths.discard(depth)
                    depth -= 1
            if m and not any(c in m.group("lead") for c in CONVENTIONS):
                first = m.group("lead").split()[0] if m.group("lead").split() else ""
                # "default: f(" and "label: f(" are statements, not declarations.
                if first not in keywords and "=" not in m.group("lead") and ":" not in m.group("lead").replace("::", ""):
                    i = m.start("name")
                    line = line[:i] + "__cdecl " + line[i:]
                    edits += 1
            # A function-pointer declarator with no convention: "T (*name)(" or a
            # cast "T (*)(". Under the old default these were __cdecl pointers.
            code = line.split("//", 1)[0]
            line = fptr.sub(lambda m: m.group(1) + "__cdecl *", code) + line[len(code):] if fptr.search(code) else line
            if fptr.search(code):
                edits += 1
            out.append(line)
        # A function registered with atexit is called through a __cdecl pointer.
        text2 = "".join(out)
        for name in set(re.findall(r"\batexit\(\s*&?\s*(\w+)\s*\)", text2)):
            fix = re.compile(r"^(?P<lead>\s*(?:static\s+)?(?:extern\s+)?void\s+)(?P<name>" + re.escape(name) + r")\s*\(", re.M)
            text2, n = fix.subn(lambda m: m.group("lead") + "__cdecl " + m.group("name") + "(", text2)
            edits += n
        out = [text2]
        if edits:
            changed += 1
            if not args.dry_run:
                path.write_bytes("".join(out).encode("latin-1"))
            print(f"{edits:3d}  {path.relative_to(ROOT) if path.is_relative_to(ROOT) else path}")
    print(f"{changed} files {'would change' if args.dry_run else 'changed'}")


if __name__ == "__main__":
    main()
