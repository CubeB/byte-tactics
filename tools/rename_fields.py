"""Rename the placeholder fields of one type's views, and only that type's.

    uv run tools/rename_fields.py <Type> --from pairs.csv [--dry-run]
    uv run tools/rename_fields.py <Type> --from pairs.csv --views Class_004ce260,Class_004cdb40
    uv run tools/rename_fields.py <Type> --from pairs.csv --no-explain-views
    uv run tools/rename_fields.py <Type> --from pairs.csv --jobs 8

Each row of pairs.csv is `offset,new[,evidence]`: the offset is hex (`0x4e`
or `4e`), `new` is the member's name, and `evidence` is kept for the pull
request (the tool ignores it).

Phase 4 of docs/tidy-up.md. A `field_<offset>` belongs to a type, not to a
file: the projectile's 0x4e and the weapon's 0x4e are different members, and
a whole-word name cannot tell them apart, which is why tools/rename.py
refuses the job. This tool edits the views of one type only.

The type's views are the views tools/gametypes.py extracts from each file
whose name is the type's, `<Type>` or `<Type>_<address>`: the rule its
base_name() strips the address with, and the one docs/tidy-up.md gives for a
file's own view of a type (`Unit_0041b2e0` is a Unit), plus every view
tools/gametypes.py's join (what `gametypes.py --explain` prints) puts in the
type's group: the `Entry_*` views of a Gadget, the `PlayerData_*` views of a
PlayerInfo. The join runs over every file this tool reads, so a view a gap
file has joins with the game files'. The joined views are listed before the
rename; `--no-explain-views` renames only the name family, and `--views` adds
view names neither finds, such as those of a type tools/gametypes.py leaves
out of the header. Re-running with the same pairs after a rebase is safe: a
renamed member is no longer `field_`, so there is nothing left to do, and a
name that is not found prints the extracted names that come closest.

The views are read from the same /Z7 objects tools/gametypes.py reads
(`--dump` writes them as JSON), plus the gap files its header leaves out and
the headers under src/ (each compiled through a dummy that includes it), so
the offsets are the compiler's, not a guess from the text. A member declared
in a header and its uses in the files that include the header (directly or
through another header) are renamed along with the .cpp views.

What it rewrites, in each view's file:

  - the member the view declares at the pair's offset, when it is still
    `field_<offset>` or `unknown_<offset>`. A member that already has a name
    stays, and a block of bytes (a `char` array, or any array of more than one
    element) keeps its `unknown_` name: those bytes are padding no view names.
  - the uses `x->field_<off>` and `x.field_<off>` where `x` is a view of the
    type, resolved from the declaration nearest the use that is in scope: a
    local or parameter of the same function, a member of the enclosing
    class, a member of the class the function belongs to, or a global.
    Arrays and member chains work (`entries[i].field_0` and
    `field_14->field_c` both take `field_14` as a PacketReceiver member).
    A use whose object is a cast to the type resolves to the cast's type
    (`((PacketChannel*)this)->field_18`), and a bare `field_<off>` inside a
    method resolves to that method's class, so an unrelated class's member of
    the same name is left alone.
  - the same word in comments, when the enclosing function's uses of it
    belong to the type (or no other type in the file declares the name). A
    comment never refuses the file: one that mentions another type's
    `field_<off>` is left as it is.
  - the trailing comment of a changed line keeps its column, so aligned
    blocks (`int field_42;   // +0x42`) stay aligned.

What it refuses, and lists:

  - a file where a use's object type is not plain, that is, another struct in
    the file, or in include/ta_types.h when the file includes it, declares
    `field_<off>` too and the use cannot be traced to a declaration in the
    same file. The file stays untouched; the others are renamed anyway.
  - a pair whose new name is not a C++ identifier, is a keyword, repeats
    another pair's new name, or is already a member of one of the views that
    has the placeholder (PR #6226 hit this with `owner` and used `ownerUnit`).
    A view that already carries the name at the same offset, as the other
    half of an anonymous union (`union { int active; int field_0; }`), agrees
    instead: it keeps both names and the pair proceeds for the other views.
  - an offset no view declares: printed as a note, since after a successful
    run that is also what a second run finds.

Data members cost no symbol ids (docs/c2-regalloc.md), so a correct run never
moves a match; a mismatch means the tool renamed the wrong thing. Check the
files it touched:

    uv run tools/checkall.py <every // FUNCTION: address in each touched file>
    uv run tools/place.py
"""

import argparse
import bisect
import csv
import hashlib
import os
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import gametypes as gt  # noqa: E402
from rename import IDENT, KEYWORDS  # noqa: E402
from sources import ROOT, sources  # noqa: E402

PLACEHOLDER = re.compile(r"^(?:field|unknown)_(?:0x)?([0-9a-fA-F]+)$")
CAST_EXPR = re.compile(
    r"^\s*\(\s*([A-Za-z_]\w*(?:\s*::\s*[A-Za-z_]\w*)*)\s*([*&]+)\s*\)\s*(\S.*?)\s*$")
DEF = re.compile(r"\b(?:struct|class|union)\s+([A-Za-z_]\w*)\s*(?::[^{;]*)?\{")
FUNC = re.compile(r"([A-Za-z_~][A-Za-z0-9_:~]*)\s*\(([^;{}()]*)\)\s*(?:const\s*)?(?::[^{;]*)?\{")
NOT_FUNCTION = {"if", "while", "for", "switch", "catch", "do", "return", "sizeof"}
SKIP_TYPES = set(KEYWORDS) | {
    "return", "case", "delete", "new", "sizeof", "throw", "goto", "else", "do", "while",
    "if", "switch", "for", "default", "using", "typedef", "template", "friend", "operator",
}
TRAILING = re.compile(r"^(.*?\S)(\s{2,})(//.*?)(\r?)$")


# --- reading the text -------------------------------------------------------------------

def segments(text: str) -> list[tuple[str, int, int]]:
    """The text cut into ("code" | "comment" | "literal", start, end) pieces, in order."""
    out, i, n, start, kind = [], 0, len(text), 0, "code"

    def cut(at: int, new_kind: str) -> None:
        nonlocal start, kind
        if at > start:
            out.append((kind, start, at))
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


def mask(text: str, segs: list[tuple[str, int, int]]) -> str:
    """The text with comments and literals blanked (newlines kept, so positions hold)."""
    chars = list(text)
    for kind, a, b in segs:
        if kind != "code":
            for i in range(a, b):
                if chars[i] != "\n":
                    chars[i] = " "
    return "".join(chars)


def close_brace(code: str, open_index: int) -> int | None:
    depth = 0
    for i in range(open_index, len(code)):
        if code[i] == "{":
            depth += 1
        elif code[i] == "}":
            depth -= 1
            if depth == 0:
                return i
    return None


def word_pattern(name: str) -> re.Pattern:
    return re.compile(rf"(?<![A-Za-z0-9_]){re.escape(name)}(?![A-Za-z0-9_])")


def char_array(t) -> bool:
    """A field's type is a char array: a block of bytes, not a member (Views' JSON form)."""
    if not (isinstance(t, dict) and "arr" in t):
        return False
    while isinstance(t, dict) and "arr" in t:
        t = t["arr"]
    return t in ("char", "unsigned char", "signed char")


def block_array(t) -> bool:
    """A field's type is any array of more than one element: a block of bytes a
    view did not split, so the name stays whatever its element type is."""
    n = 1
    while isinstance(t, dict) and "arr" in t:
        n *= t.get("n", 1)
        t = t["arr"]
    return n > 1


def declares_member(code: str, struct: tuple[str, int, int], old: str) -> bool:
    """Whether `old` appears anywhere in the struct's body. A member of a nested
    anonymous union is one of the struct's own, and a use at any depth still
    makes the name one to resolve use by use, so either way it is not safe to
    rename blindly."""
    _, o, e = struct
    return bool(word_pattern(old).search(code[o + 1:e]))


class Source:
    """One file parsed far enough for the rename: structs, functions, scopes."""

    def __init__(self, path: Path):
        self.path = path
        self.text = path.read_bytes().decode("latin-1")
        self.segs = segments(self.text)
        self.code = mask(self.text, self.segs)
        self._starts = [a for _, a, _ in self.segs]
        self.structs: list[tuple[str, int, int]] = []
        for m in DEF.finditer(self.code):
            o = m.end() - 1
            e = close_brace(self.code, o)
            if e is not None:
                self.structs.append((m.group(1), o, e))
        self.funcs: list[tuple[str, str | None, int, int, int]] = []
        for m in FUNC.finditer(self.code):
            name = m.group(1)
            if name in NOT_FUNCTION or name.split("::")[-1] in NOT_FUNCTION:
                continue
            line_start = self.code.rfind("\n", 0, m.start(1)) + 1
            if "#" in self.code[line_start:m.start(1)]:
                continue  # a preprocessor line, not a function (`#pragma pack(push, 1)`)
            o = m.end() - 1
            e = close_brace(self.code, o)
            if e is None:
                continue
            cls = name.rsplit("::", 1)[0] if "::" in name else None
            self.funcs.append((name, cls, m.start(1), o, e))
        self.includes_ta_types = bool(
            re.search(r'^\s*#\s*include\s*[<"]ta_types\.h[>"]', self.text, re.M))

    def kind_at(self, pos: int) -> str:
        i = bisect.bisect_right(self._starts, pos) - 1
        return self.segs[i][0]

    def struct_at(self, pos: int) -> tuple[str, int, int] | None:
        best = None
        for s in self.structs:
            if s[1] < pos < s[2] and (best is None or s[1] > best[1]):
                best = s
        return best

    def func_at(self, pos: int) -> tuple | None:
        best = None
        for f in self.funcs:
            if f[2] <= pos <= f[4] and (best is None or f[2] > best[2]):
                best = f
        return best

    def class_of(self, f: tuple) -> str | None:
        if f[1]:
            return f[1]
        s = self.struct_at(f[3])
        return s[0] if s else None

    def depth_in(self, open_index: int, pos: int) -> int:
        return self.code.count("{", open_index, pos) - self.code.count("}", open_index, pos)

    def decl_of(self, name: str, pos: int) -> str | None:
        """The type of the declaration of `name` nearest before pos that is in scope."""
        pattern = re.compile(
            r"(?:(?<=[;{}(),:])|^)\s*"
            r"(?:(?:const|volatile|unsigned|signed|static|extern|register|auto)\s+)*"
            r"(?:(?:struct|class|union)\s+)?"
            r"([A-Za-z_]\w*(?:\s*<[^;{}]*>)?(?:\s*::\s*[A-Za-z_]\w*)*)\s*([*&]*)\s*"
            + re.escape(name) + r"\b")
        fs = self.func_at(pos)
        best = None
        for m in pattern.finditer(self.code, 0, pos):
            t = m.group(1)
            if t in SKIP_TYPES:
                continue
            if m.group(2).count("&") > 1:
                continue  # `flag && name`: a logical and, not a declarator
            d = m.start(1)
            ds = self.struct_at(d)
            if ds is not None:
                if not (ds[1] < pos < ds[2]) and not (fs is not None and self.class_of(fs) == ds[0]):
                    continue  # a member of a sibling struct
            else:
                df = self.func_at(d)
                if df is not None and (fs is None or df[2] != fs[2]):
                    continue  # a local or parameter of another function
            if best is None or d > best[0]:
                best = (d, t)
        return best[1] if best else None

    def chain_before(self, pos: int) -> list[tuple[str, str]] | None:
        """The access chain an `x->member` / `x.member` at pos reads through, base
        first: `entry->field_0` is [('->', 'entry')], `g.member.field->field_c` is
        [('.', 'g'), ('.', 'member'), ('->', 'field')]. `None` when pos is no access.
        A cast in front of the access fixes the object's type and is the base:
        `((PacketChannel*)this)->field_18` is [('cast', 'PacketChannel')]. The
        cast's operand is skipped: the cast already says what it is."""
        code = self.code
        i = pos - 1
        while i >= 0 and code[i] in " \t\r\n":
            i -= 1
        parts: list[tuple[str, str]] = []
        pending_cast: str | None = None
        while True:
            if i >= 1 and code[i] == ">" and code[i - 1] == "-":
                sep, i = "->", i - 2
            elif i >= 0 and code[i] == ".":
                sep, i = ".", i - 1
            else:
                break
            while i >= 0 and code[i] in " \t\r\n":
                i -= 1
            if i < 0:
                return None
            if code[i] == "]":
                depth = 0
                k = i
                while k >= 0:
                    if code[k] == "]":
                        depth += 1
                    elif code[k] == "[":
                        depth -= 1
                        if depth == 0:
                            break
                    k -= 1
                if k < 0:
                    return None
                i = k - 1
                while i >= 0 and code[i] in " \t\r\n":
                    i -= 1
            elif code[i] == ")":
                depth = 0
                k = i
                while k >= 0:
                    if code[k] == ")":
                        depth += 1
                    elif code[k] == "(":
                        depth -= 1
                        if depth == 0:
                            break
                    k -= 1
                if k < 0:
                    return None
                inner = code[k + 1:i]
                m = CAST_EXPR.match(inner)
                if m:
                    # The nearest cast wins: later (further inside) casts only
                    # converted the same operand on the way here.
                    if pending_cast is None:
                        pending_cast = m.group(1)
                    i = k + m.end(3)  # skip the operand; the cast's type replaces it
                    continue
                i = k - 1
                while i >= 0 and code[i] in " \t\r\n":
                    i -= 1
                if i >= 0 and code[i] == "*":
                    i -= 1
            end = i + 1
            while i >= 0 and (code[i].isalnum() or code[i] == "_"):
                i -= 1
            name = code[i + 1:end]
            if not name:
                return None
            parts.append(("cast", pending_cast) if pending_cast is not None else (sep, name))
            pending_cast = None
            j = i
            while j >= 0 and code[j] in " \t\r\n":
                j -= 1
            if j >= 1 and code[j] == ">" and code[j - 1] == "-":
                i = j
            elif j >= 0 and code[j] == ".":
                i = j
            else:
                break
        if pending_cast is not None:
            parts.append(("cast", pending_cast))
        if not parts:
            return None
        parts.reverse()
        return parts

    def member_type(self, tname: str, member: str) -> str | None:
        """The declared type of `member` in this file's view named tname (or its base)."""
        for name, o, e in self.structs:
            if name != tname and gt.base_name(name) != tname:
                continue
            body = self.code[o + 1:e]
            for m in word_pattern(member).finditer(body):
                pos = m.start()
                depth = body.count("{", 0, pos) - body.count("}", 0, pos)
                if depth != 0:
                    continue
                stop = max(body.rfind(";", 0, pos), body.rfind("{", 0, pos), body.rfind("}", 0, pos))
                prefix = body[stop + 1:pos]
                decl = re.search(r"([A-Za-z_]\w*(?:\s*<[^;{}]*>)?)\s*[*&]*\s*$", prefix)
                if decl:
                    return decl.group(1)
            return None
        return None


class Renamer:
    """One run: the type's view names and each file's verdicts."""

    def __init__(self, type_name: str, files: dict, extra_views: list[str] | None = None,
                 joined: dict[str, list[str]] | None = None):
        self.type_name = type_name
        self.exact = bool(gt.SUFFIX.search(type_name))
        self.extra = set(extra_views or ())
        self.joined = {stem: set(views) for stem, views in (joined or {}).items()}
        self.joined_names = {view for views in self.joined.values() for view in views}
        self.family: dict[str, list[str]] = defaultdict(list)
        for stem, data in files.items():
            joined_here = self.joined.get(stem, ())
            for view in data["views"]:
                if view == type_name or view in self.extra or view in joined_here \
                        or (not self.exact and gt.base_name(view) == type_name):
                    self.family[stem].append(view)

    def is_ours(self, tname: str | None, ours: set[str]) -> bool:
        if not tname:
            return False
        if tname in self.extra:
            return True
        # A joined view is the type only where the join put it: the name rule is
        # name-wide, but one name can be another type's view in another file, and
        # `ours` names the family views this file declares (or includes).
        if tname in self.joined_names and tname in ours:
            return True
        return tname == self.type_name if self.exact else gt.base_name(tname) == self.type_name

    def resolved(self, src: Source, pos: int) -> str | None:
        """The type name a use or declaration at pos resolves to, or None."""
        chain = src.chain_before(pos)
        if chain is None:
            f = src.func_at(pos)
            if f:
                return src.class_of(f)
            s = src.struct_at(pos)
            return s[0] if s else None
        if chain[0][0] == "cast":
            t, rest = chain[0][1], chain[1:]
        elif chain[0][1] == "this":
            f = src.func_at(pos)
            s = src.struct_at(pos)
            t = (src.class_of(f) if f else None) or (s[0] if s else None)
            rest = chain[1:]
        else:
            t = src.decl_of(chain[0][1], pos)
            rest = chain[1:]
        if t is None:
            return None
        for _, member in rest:
            t = src.member_type(t, member)
            if t is None:
                return None
        return t

    def use_verdict(self, src: Source, pos: int, ours: set[str]) -> str | None:
        """ours | other | None for one code use."""
        t = self.resolved(src, pos)
        if t is None:
            return None
        return "ours" if self.is_ours(t, ours) else "other"

    def code_verdict(self, src: Source, pos: int, ours: set[str]) -> str | None:
        s = src.struct_at(pos)
        if s:
            if self.is_ours(s[0], ours):
                return "ours"
            if src.depth_in(s[1], pos) == 0:
                return "other"  # a member declaration of the other struct
            return self.use_verdict(src, pos, ours)
        return self.use_verdict(src, pos, ours)

    def comment_verdict(self, src: Source, pos: int, old: str, ours: set[str]) -> str | None:
        f = src.func_at(pos)
        s = src.struct_at(pos)
        if f:
            region = (f[2], f[4])
        elif s:
            region = (s[1], s[2])
        else:
            # A comment block sitting on top of a function belongs to that function:
            # only comments, blanks and the return type may lie between. One with
            # includes or code in between belongs to the file.
            nxt = next((g for g in sorted(src.funcs, key=lambda g: g[2])
                        if g[2] > pos and re.fullmatch(r"[\sA-Za-z0-9_*&:~]*", src.code[pos:g[2]])),
                       None)
            region = (nxt[2], nxt[4]) if nxt else (0, len(src.code))
        seen = set()
        for m in word_pattern(old).finditer(src.code, region[0], region[1]):
            seen.add(self.code_verdict(src, m.start(), ours))
        if not seen:
            return None
        if seen == {"ours"}:
            return "ours"
        if seen == {"other"}:
            return "other"
        return None

    def ambiguous(self, src: Source, ours: set[str], old: str) -> bool:
        for s in src.structs:
            if s[0] not in ours and declares_member(src.code, s, old):
                return True
        if src.includes_ta_types:
            for s in header_structs():
                if s[0] not in ours and declares_member(header_code(), s, old):
                    return True
        return False

    def plan(self, src: Source, ours: set[str], old: str, new: str,
             agreed: frozenset[str] | set[str] = frozenset(),
             strict: bool = False) -> tuple[list[tuple[int, str, str]], str | None]:
        """The edits for one old name in one file, or the reason the file is refused.

        `agreed` names the views that already carry `new` at the pair's offset in
        an anonymous union; those views keep their placeholder, so their own
        declarations and uses are left alone. A comment never refuses the file.
        `strict` refuses when a use cannot be traced (a file planned because it
        includes a header, whose declaration is not in the file itself)."""
        ambiguous = self.ambiguous(src, ours, old)
        edits = []
        for m in word_pattern(old).finditer(src.text):
            pos = m.start()
            kind = src.kind_at(pos)
            if kind == "literal":
                continue
            if kind == "comment":
                v = self.comment_verdict(src, pos, old, ours)
                if v == "ours" or (v is None and not ambiguous):
                    edits.append((pos, old, new))
                continue
            v = self.code_verdict(src, pos, ours)
            if v == "other":
                continue  # another class's member, whatever the method says
            if v == "ours":
                if agreed and self.resolved(src, pos) in agreed:
                    continue  # the view already carries `new` at these bytes
                edits.append((pos, old, new))
                continue
            if ambiguous or strict:
                line = src.text.count("\n", 0, pos) + 1
                return [], f"line {line}: cannot tell which type `{old}` belongs to"
            edits.append((pos, old, new))
        return edits, None


_HEADER: tuple[str, list[tuple[str, int, int]]] | None = None


def header_code() -> str:
    global _HEADER
    if _HEADER is None:
        path = ROOT / "include/ta_types.h"
        if path.exists():
            text = path.read_bytes().decode("latin-1")
            code = mask(text, segments(text))
            structs = []
            for m in DEF.finditer(code):
                o = m.end() - 1
                e = close_brace(code, o)
                if e is not None:
                    structs.append((m.group(1), o, e))
            _HEADER = (code, structs)
        else:
            _HEADER = ("", [])
    return _HEADER[0]


def header_structs() -> list[tuple[str, int, int]]:
    header_code()
    return _HEADER[1]


# --- the run ----------------------------------------------------------------------------

def header_stem(path: Path) -> str:
    """A header's file id: its path under the repository, safe as a cache name."""
    return "hdr_" + str(path.resolve().relative_to(ROOT)).replace("/", "_")


def extract_header(path: Path) -> dict | None:
    """One src header's views, extracted as gametypes extracts a file's.

    A header is not compiled on its own, so a dummy that includes it is, the
    same way gametypes --verify compiles the game types header. The dummy
    carries the header's digest, so the cache follows the header."""
    text = path.read_bytes()
    stem = header_stem(path)
    work = gt.CACHE / "hdr"
    work.mkdir(parents=True, exist_ok=True)
    dummy = work / (stem + ".cpp")
    wanted = {m.group(2) for m in gt.DEFINED.finditer(text.decode("latin-1"))}
    # /Z7 emits the types a translation unit uses, not only the ones it declares,
    # so probe each declared class with a pointer (an instance may be abstract).
    probes = "".join(f"{name}* rf_probe_{i};\n" for i, name in enumerate(sorted(wanted)))
    dummy.write_text(f'// {hashlib.sha256(text).hexdigest()[:16]}\n'
                     f'#include "{gt.winpath(path.resolve())}"\n{probes}')
    return gt.extract(dummy, stem, wanted)


def extract_all(jobs: int) -> tuple[dict, dict[str, Path]]:
    """gametypes' views, plus the files and headers its game header leaves out.

    tools/gametypes.py extracts the game files only; a gap file can define the same
    view a game file does (weapons_49a120.cpp declares Weapon_0049a120), so the two
    agree only when both are renamed. The same goes for the src headers, which have
    no object of their own. The objects are the same /Z7 ones.
    """
    files = gt.extract_all(jobs)
    paths = dict(gt.game_files())
    for kind in ("gap", "library"):
        for path in sources(kind):
            stem = gt.file_id(path)
            if stem in files:
                continue
            wanted = {m.group(2) for m in gt.DEFINED.finditer(path.read_text(errors="replace"))}
            data = gt.extract(path, stem, wanted)
            if data is not None:
                files[stem] = data
                paths[stem] = path
    for path in sorted((ROOT / "src").rglob("*.h")):
        stem = header_stem(path)
        if stem in files:
            continue
        data = extract_header(path)
        if data is not None:
            files[stem] = data
            paths[stem] = path
    return files, paths


def explained_views(files: dict, name: str) -> dict[str, list[str]]:
    """The views tools/gametypes.py's join puts in the type asked for.

    The join is gametypes' own: the Program, its edges and its layouts, which
    are what `gametypes.py --explain` prints. It runs over every file this tool
    reads, the game files plus the gap, library and header views its extraction
    adds, so a view only a gap file has joins with the game files' as well.
    `name` is the type's name or any view name in its group; the result is the
    group's views as {file id: [view name]}, empty when none joins to it.
    """
    definers = {r["symbol"]: gt.file_id(gt.ROOT / r["file"])
                for r in csv.DictReader(gt.PROGRESS.open())}
    program = gt.Program(files, definers, gt.declared_names(), gt.system_names())
    program.build()
    layouts = program.layouts()
    # The name is the type's own when a type has it; a view name only says which
    # group it joined (a file's `Gadget` view can be a Layer, so the group named
    # Gadget wins and the names are not mixed with it).
    group = [lay for lay in layouts if lay.name == name] \
        or [lay for lay in layouts if any(node[1] == name for node in lay.nodes)]
    out: dict[str, list[str]] = defaultdict(list)
    for lay in group:
        for stem, view in lay.nodes:
            out[stem].append(view)
    return dict(out)


INCLUDE = re.compile(r'^\s*#\s*include\s+[<"]([^">]+)[>"]', re.M)


def consumers_of(headers: set[Path]) -> dict[Path, set[Path]]:
    """Each header's consumers: every src file that includes it, directly or
    through another src header (the files whose uses move with it)."""
    includers: dict[Path, set[Path]] = defaultdict(set)
    for path in sorted((ROOT / "src").rglob("*.cpp")) + sorted((ROOT / "src").rglob("*.h")):
        text = path.read_bytes().decode("latin-1")
        for inc in INCLUDE.findall(text):
            for base in (path.parent, ROOT / "src", ROOT / "include", ROOT):
                cand = (base / inc).resolve()
                if cand in headers and cand != path.resolve():
                    includers[cand].add(path)
                    break
    out: dict[Path, set[Path]] = {h: set() for h in headers}
    for h in headers:
        seen: set[Path] = set()
        stack = [h]
        while stack:
            for f in includers.get(stack.pop(), ()):
                rf = f.resolve()
                if rf in seen or rf == h:
                    continue
                seen.add(rf)
                out[h].add(f)
                if rf in headers:
                    stack.append(rf)
    return out


def load_pairs(path: Path) -> tuple[list[tuple[int, str, str]], list[str]]:
    rows = []
    with path.open(newline="") as fh:
        for row in csv.DictReader(fh):
            off, new = (row.get("offset") or "").strip(), (row.get("new") or "").strip()
            ev = (row.get("evidence") or "").strip()
            rows.append((off, new, ev))
    pairs, problems = [], []
    seen_off: dict[int, str] = {}
    seen_new: dict[str, int] = {}
    for off, new, ev in rows:
        try:
            value = int(off, 16)
        except ValueError:
            problems.append(f"{off!r}: is not a hex offset")
            continue
        if not IDENT.match(new) or new in KEYWORDS:
            problems.append(f"{new!r}: is not a name C++ allows")
            continue
        if value in seen_off:
            problems.append(f"{off}: appears more than once")
            continue
        if new in seen_new:
            problems.append(f"{new!r}: two offsets cannot take one name")
            continue
        seen_off[value], seen_new[new] = new, value
        pairs.append((value, new, ev))
    return pairs, problems


def field_at(view: dict, off: int) -> dict | None:
    """The view's member at the offset, when it is still a placeholder for it.

    A member whose type is a block (a char array or any array of more than one
    element) keeps its `unknown_` name: the block covers bytes no single pair
    names."""
    for f in view["fields"]:
        m = PLACEHOLDER.match(f["name"])
        if not m or int(m.group(1), 16) != off or f["off"] != off:
            continue
        if "bits" in f:
            return f
        if char_array(f["type"]) or block_array(f["type"]):
            continue
        return f
    return None


def has_member(view: dict, name: str) -> bool:
    return (any(f["name"] == name for f in view["fields"])
            or any(m["name"] == name for m in view["methods"])
            or any(s["name"] == name for s in view["statics"]))


def has_field_at(view: dict, name: str, off: int) -> bool:
    """Whether the view carries `name` at the offset: a union alternative is
    the same bytes under another spelling, so the pair is already half done."""
    return any(f["name"] == name and f["off"] == off for f in view["fields"])


def apply_edits(text: str, edits: list[tuple[int, str, str]]) -> str:
    # Back to front: replacing a name shifts every later offset.
    out = text
    for pos, old, new in sorted(set(edits), key=lambda e: -e[0]):
        out = out[:pos] + new + out[pos + len(old):]
    # A changed line keeps its trailing comment's column, as tools/rename.py does.
    old_lines, new_lines = text.split("\n"), out.split("\n")
    for i, (o, n) in enumerate(zip(old_lines, new_lines)):
        if o == n:
            continue
        mo, mn = TRAILING.match(o), TRAILING.match(n)
        if mo and mn and mo.group(3) == mn.group(3):
            column = len(mo.group(1)) + len(mo.group(2))
            new_lines[i] = mn.group(1) + " " * max(2, column - len(mn.group(1))) + mn.group(3) + mn.group(4)
    return "\n".join(new_lines)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("type", metavar="Type", help="the type whose fields to rename")
    ap.add_argument("--from", dest="table", type=Path, required=True, metavar="pairs.csv",
                    help="a CSV of offset,new[,evidence] rows")
    ap.add_argument("--dry-run", action="store_true", help="print what would change and stop")
    ap.add_argument("--views", metavar="Name,Name", default="",
                    help="further view names of the type, which neither its name rule nor "
                         "gametypes' join finds")
    ap.add_argument("--no-explain-views", action="store_true",
                    help="rename only the name rule's views, not the ones tools/gametypes.py's "
                         "join adds")
    ap.add_argument("--jobs", type=int, default=min(8, os.cpu_count() or 4))
    args = ap.parse_args()

    pairs, problems = load_pairs(args.table)
    if problems:
        for p in problems:
            print(f"bad pair: {p}")
        return 2

    extra = [v for v in re.split(r"[,\s]+", args.views) if v]
    files, paths = extract_all(args.jobs)
    joined: dict[str, list[str]] = {}
    if not args.no_explain_views:
        joined = explained_views(files, args.type)
    rename = Renamer(args.type, files, extra, joined)
    if not rename.family:
        names = sorted({v for data in files.values() for v in data["views"]})
        close = [n for n in names if args.type.lower() in n.lower()]
        print(f"no view of `{args.type}`: no struct or class named `{args.type}` or "
              f"`{args.type}_<address>` is declared under src/")
        if close:
            print("  closest extracted views: " + ", ".join(close[:20]))
        return 1

    # Pair by pair: the views that still hold the placeholder, the views that
    # already agree through a union, and the refusals.
    candidates: dict[tuple[str, str], dict[int, tuple[str, str]]] = defaultdict(dict)
    agreed: dict[str, dict[str, set[str]]] = defaultdict(lambda: defaultdict(set))
    notes, refused_pairs = [], []
    for off, new, _ev in pairs:
        places = []
        for stem, views in rename.family.items():
            for view in views:
                f = field_at(files[stem]["views"][view], off)
                if f is not None:
                    places.append((stem, view, f["name"]))
        if not places:
            notes.append(f"{off:#x}: no view of {args.type} declares a placeholder there")
            continue
        pair_agreed, refuse = set(), None
        for stem, view, old in places:
            if not has_member(files[stem]["views"][view], new):
                continue
            if has_field_at(files[stem]["views"][view], new, off):
                pair_agreed.add((stem, view))
                agreed[stem][old].add(view)
                continue
            refuse = (stem, view)
            break
        if refuse is not None:
            stem, view = refuse
            refused_pairs.append(f"{off:#x} -> {new}: {view} ({paths[stem].name}) "
                                 f"already has a member named {new}")
            continue
        for stem, view, old in places:
            if (stem, view) not in pair_agreed:
                candidates[(stem, view)][off] = (old, new)

    # File by file: the olds every planned file owes. A member renamed in a
    # header is owed by the files that include it as well.
    planned: dict[str, dict[str, str]] = defaultdict(dict)
    for (stem, view), offs in candidates.items():
        for _off, (old, new) in offs.items():
            planned[stem][old] = new

    headers = {p.resolve(): stem for stem, p in paths.items() if p.suffix == ".h"}
    consumers = consumers_of(set(headers))
    consumer_paths = {p.resolve() for group in consumers.values() for p in group}
    stem_of = {p.resolve(): stem for stem, p in paths.items()}
    for h, hstem in headers.items():
        for consumer in consumers.get(h, ()):
            cstem = stem_of.get(consumer.resolve(), str(consumer.resolve().relative_to(ROOT.resolve())))
            planned[cstem].update(planned.get(hstem, {}))
            paths.setdefault(cstem, consumer)
            for old, views in agreed.get(hstem, {}).items():
                agreed[cstem][old].update(views)

    # The file's verdicts, or the reason the whole file is refused. A file that
    # is planned because it includes a header has no view of its own, so its
    # uses are resolved strictly: one that cannot be traced refuses the file.
    sources = {stem: Source(paths[stem]) for stem in planned if planned[stem]}
    ours_by_stem = {stem: set(rename.family.get(stem, [])) for stem in sources}
    for h, hstem in headers.items():
        for consumer in consumers.get(h, ()):
            cstem = stem_of.get(consumer.resolve())
            if cstem in ours_by_stem:
                ours_by_stem[cstem].update(rename.family.get(hstem, []))
    edits_by_file: dict[str, list[tuple[int, str, str]]] = {}
    refused_files = []
    for stem in sorted(sources):
        src = sources[stem]
        strict = paths[stem].suffix == ".h" or paths[stem].resolve() in consumer_paths
        edits, why = [], None
        for old, new in planned[stem].items():
            got, why = rename.plan(src, ours_by_stem[stem], old, new,
                                   agreed=agreed.get(stem, {}).get(old, set()), strict=strict)
            if why:
                break
            edits += got
        if why:
            refused_files.append(f"{paths[stem].relative_to(ROOT)}: {why}")
            continue
        if edits:
            edits_by_file[stem] = edits

    count = sum(len(e) for e in edits_by_file.values())
    joined_names = sorted({v for views in joined.values() for v in views})
    if not args.no_explain_views:
        if joined_names:
            print(f"{args.type}: gametypes joins {len(joined_names)} view name(s): "
                  + ", ".join(joined_names))
        else:
            print(f"{args.type}: gametypes joins no view name, the name rule and --views apply")
    print(f"{args.type}: {len(rename.family)} file(s) with a view, {len(planned)} planned, "
          f"{len(pairs)} pair(s), {'would rename' if args.dry_run else 'renamed'} {count} "
          f"occurrence(s) in {len(edits_by_file)} file(s)")
    for stem in sorted(edits_by_file, key=lambda s: str(paths[s])):
        news = sorted({(old, new) for _, old, new in edits_by_file[stem]})
        print(f"  {paths[stem].relative_to(ROOT)}: "
              + ", ".join(f"{old} -> {new}" for old, new in news))
    for note in notes:
        print(f"  note: {note}")
    for r in refused_pairs:
        print(f"  refused pair: {r}")
    for r in refused_files:
        print(f"  refused file: {r}")

    if not args.dry_run:
        for stem, edits in edits_by_file.items():
            src = sources[stem]
            new = apply_edits(src.text, edits)
            if new != src.text:
                paths[stem].write_bytes(new.encode("latin-1"))
    if refused_pairs or refused_files:
        print("the refused work is listed above and was not applied")
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
