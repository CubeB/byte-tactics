"""Build the manifest of the game's global data: data/globals.csv, link/globals.h
and link/data.cpp.

    uv run tools/globals.py            # write the three files and summarise
    uv run tools/globals.py --check    # also compile link/data.cpp and compare
                                       # every definition with the original bytes

One row per global the source refers to by address (a DAT_<address> name, or a
name data/symbols.csv maps to an address in .rdata/.data), with:

  address, name   the name is data/symbols.csv's when it knows one
  section         .rdata, .data or .bss (the zero-filled tail of .data, from
                  the first known address after its last non-zero byte)
  size, size_from the size of the declared type when it has one ("type"), else
                  the distance to the next address the original, the source or
                  data/symbols.csv refers to ("gap"); "type>gap" marks a type
                  that overruns the next known address
  kind            data, string, vtable, float, library (owned by the CRT) or
                  template (a static member of an STL template)
  type            the most common type the source declares for it, the number
                  of files that declare exactly that (type_files) and the number
                  that declare something else (other_files), how many distinct
                  types there are, and tools/linkcheck.py's verdict on them
  files           files that refer to it
  max_offset      the largest offset from it the source uses (DAT_x + 8 -> 0x8)
  ghidra          the label Ghidra gives the address (s_ string, PTR_ pointer)
  pointers        dwords in its initial bytes that point into the exe
  init            its first bytes in the exe, in hex (none for .bss)

link/globals.h declares the globals whose type is settled (the views agree
up to struct names or signedness, or three quarters of the files agree on the
type or on its shape); a struct held by value is declared as a byte array of
the right size. link/data.cpp defines those globals with their initial values.
Neither is used by any file under src/ yet: they are the starting point for the
linkable build (docs/linking.md).
"""

import argparse
import bisect
import csv
import re
import struct
import sys
from collections import Counter
from dataclasses import dataclass
from pathlib import Path

import capstone

from check import ROOT, Original, compile_source, load_symbols
from sources import link_key, relative
from sources import sources as files_of_kind
from link import data_source_extents, inside
from coff import parse_object
from linkcheck import (CRT_LIBS, address_of, data_symbol, declare, global_table, library_symbols,
                       load_objects, records, shape, type_size)

GLOBALS = ROOT / "data/globals.csv"
LINK = ROOT / "link"
HEADER = LINK / "globals.h"
DATA = LINK / "data.cpp"
GHIDRA = ROOT / "build/ghidra/decomp"
INIT_CAP = 64
DATA_LO, DATA_HI = 0x4FC000, 0x52D000
COLUMNS = ["address", "name", "section", "size", "size_from", "kind", "type", "type_files", "other_files",
           "types", "verdict", "files", "max_offset", "ghidra", "pointers", "defined", "init"]


# --- the exe --------------------------------------------------------------------

class Image:
    """The exe's data sections. LINK merges .bss into the end of .data, and
    the raw bytes run on to the next file-alignment boundary, so the .bss
    start is estimated: the first known address after .data's last non-zero
    byte (set_bss_start)."""

    def __init__(self):
        self.orig = Original()
        self.sections = []
        for s in self.orig.pe.sections:
            lo = self.orig.base + s.VirtualAddress
            name = s.Name.rstrip(b"\0").decode()
            self.sections.append((lo, lo + s.Misc_VirtualSize, lo + s.SizeOfRawData, name))
        lo, _, raw_end, _ = next(s for s in self.sections if s[3] == ".data")
        raw = self.orig.read(lo, raw_end - lo)
        self.last_nonzero = lo + max(i for i, b in enumerate(raw) if b)
        self.bss_start = raw_end
        self.boundaries: list[int] = []     # every known address, sorted (set by build)

    def set_bss_start(self, known: list[int]) -> None:
        after = [a for a in known if a > self.last_nonzero]
        self.bss_start = min(min(after, default=self.bss_start), self.bss_start)

    def section(self, va: int) -> str:
        for lo, hi, raw_end, name in self.sections:
            if lo <= va < hi:
                return ".bss" if name == ".data" and va >= self.bss_start else name
        return ""

    def section_end(self, va: int) -> int:
        for lo, hi, raw_end, name in self.sections:
            if lo <= va < hi:
                return self.bss_start if name == ".data" and va < self.bss_start else hi
        return va

    def read(self, va: int, size: int) -> bytes:
        if self.section(va) in (".bss", ""):
            return b""
        return self.orig.read(va, size)


LIBRARY_REFS: set[int] = set()     # data addresses the runtime library's code refers to


def referenced_addresses(img: Image) -> set[int]:
    """Every data address the original refers to directly: operands of every
    function's instructions, and pointers stored in .rdata and .data."""
    with (ROOT / "data/functions.csv").open() as fh:
        funcs = [(int(r["address"], 16), int(r["size"] or 0), r["kind"]) for r in csv.DictReader(fh)]
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = set()
    number = re.compile(r"0x([0-9a-f]{6,8})\b")
    for va, size, kind in funcs:
        for _, _, _, op in md.disasm_lite(img.orig.read(va, size), va):
            for m in number.finditer(op):
                v = int(m.group(1), 16)
                if DATA_LO <= v < DATA_HI:
                    out.add(v)
                    if kind == "library":
                        LIBRARY_REFS.add(v)
    for lo, hi, raw_end, name in img.sections:
        if name not in (".rdata", ".data"):
            continue
        raw = img.orig.read(lo, raw_end - lo)
        for i in range(0, len(raw) - 3, 4):
            (v,) = struct.unpack_from("<I", raw, i)
            if DATA_LO <= v < DATA_HI and not text_fragment(raw[i:i + 3]):
                out.add(v)
    return out


def text_fragment(low: bytes) -> bool:
    """Whether the low three bytes of a dword that looks like an address are
    text: four bytes of a string ("BAR" and its terminator are 0x524142), not
    a pointer. An address's low bytes are printable only by chance, and a
    false boundary cuts a .bss buffer short."""
    return all(0x20 <= c < 0x7F for c in low)


def ghidra_labels() -> dict[int, str]:
    """address -> the label Ghidra's pseudo-C uses most for it."""
    counts: dict[int, Counter] = {}
    if not GHIDRA.is_dir():
        return {}
    label = re.compile(r"\b([A-Za-z_]\w*?_00(?:4f[c-f]|5[0-2][0-9a-f])[0-9a-f]{3})\b")
    for path in GHIDRA.glob("*.c"):
        for m in label.finditer(path.read_text(errors="replace")):
            text = m.group(1)
            counts.setdefault(int(text[-8:], 16), Counter())[text] += 1
    return {a: c.most_common(1)[0][0] for a, c in counts.items()}


# --- choosing a type ---------------------------------------------------------------

def canonical(g: dict) -> tuple[str, tuple | None, int, int]:
    """(type text, type, files agreeing, files declaring something else).
    The most common declared type wins; an array without an extent agrees
    with the same array with one, and the extent is kept."""
    counts: Counter = g["types"]
    typed = g["typed"]

    def family(key: str) -> str:
        t = typed.get(key)
        if t is not None and t[0] == "arr":
            return "arr:" + declare(t[2])
        return key

    fam = Counter()
    for k, n in counts.items():
        fam[family(k)] += n
    total = sum(counts.values())
    best = max(fam, key=lambda f: (fam[f], not f.startswith(("?", "extern")), f))
    members = [k for k in counts if family(k) == best]
    # Within an array family, prefer a declaration with its extent.
    key = max(members, key=lambda k: (all((typed.get(k) or ("", [0]))[1]) if best.startswith("arr:") else 0,
                                      counts[k]))
    return key, typed.get(key), fam[best], total - fam[best]


def settled(g: dict, key: str, agree: int, other: int) -> bool:
    """Is the canonical type safe to declare once for everyone? Yes when the
    views differ at most in struct names or signedness, when three quarters
    of the files agree on it, or when three quarters agree on its shape (a
    pointer to some struct, say) and it is the commonest type of that shape."""
    if g["verdict"].startswith(("one type", "conflicting: struct names", "conflicting: signedness")):
        return True
    if agree >= 3 * other:
        return True
    shapes = Counter()
    for k, n in g["types"].items():
        shapes[shape(g["typed"].get(k))] += n
    top, n = shapes.most_common(1)[0]
    return n >= 3 * (sum(shapes.values()) - n) and shape(g["typed"].get(key)) == top


C_DECLARATION = r'extern\s+"C"\s+((?:const\s+|unsigned\s+|signed\s+)*(?:char|short|int|long|float|double))\s*(\**)\s*'


def c_linkage_type(g: dict, name: str) -> tuple | None:
    """The type of a global only ever declared extern "C" (whose symbol has
    no type in it), read from a declaration of a plain type."""
    pattern = re.compile(C_DECLARATION + r"(?:[A-Za-z_]\w*\s*,\s*)*" + re.escape(name) + r"\b\s*(\[\s*\d*\s*\])?")
    for src in sorted(g["files"]):
        m = pattern.search((ROOT / src).read_text(errors="replace"))
        if m:
            words = m.group(1).split()
            cv = "const" if "const" in words else ""
            base = " ".join(w for w in words if w != "const")
            t = ("prim", base) if base in PRIM_FORMAT else None
            if t is None:
                return None
            for _ in m.group(2):
                t = ("ptr", cv, t, "")
                cv = ""
            if cv:
                t = ("cv", cv, t)
            if m.group(3):
                d = m.group(3).strip("[] ")
                t = ("arr", [int(d) if d else 0], t)
            return t
    return None


def portable(t: tuple | None) -> bool:
    """Can a header declare this type without the struct's definition? Pointers
    to a struct need only a forward declaration; a struct by value cannot."""
    if t is None:
        return False
    k = t[0]
    if k == "prim":
        return t[1] != "void"
    if k == "cv":
        return portable(t[2])
    if k == "rec":
        return False
    if k == "arr":
        return portable(t[2])
    if k in ("ptr", "ref"):
        # A template's arguments would need declaring too; an enum can't be
        # forward-declared.
        return all(kw != "enum" and "<" not in name for kw, name in records(t))
    return False


# --- the manifest ---------------------------------------------------------------------

def row_element(t: tuple | None) -> int | None:
    """The size of one element of an array's outermost dimension
    (int[45][2] -> 8), or None for anything else."""
    if t is None or t[0] != "arr" or not t[1] or not all(t[1]):
        return None
    size = type_size(t[2])
    if not size:
        return None
    for d in t[1][1:]:
        size *= d
    return size


def build(objects, img: Image) -> list[dict]:
    symbols = load_symbols()
    extents = data_source_extents(symbols)
    table = global_table(objects, symbols)
    boundaries = set(table) | {a for a in symbols.values() if DATA_LO <= a < DATA_HI} | referenced_addresses(img)
    img.set_bss_start(sorted(boundaries))
    for lo, hi, raw_end, name in img.sections:
        boundaries |= {lo, hi}
    boundaries.add(img.bss_start)
    ordered = sorted(boundaries)
    img.boundaries = ordered
    labels = ghidra_labels()
    libs = library_symbols(set(CRT_LIBS))
    library_names = {n.lstrip("_") for n in libs}
    with (ROOT / "data/functions.csv").open() as fh:
        func_starts = {int(r["address"], 16) for r in csv.DictReader(fh)}

    rows = []
    for addr in sorted(table):
        g = table[addr]
        key, t, agree, other = canonical(g)
        lo, hi = g["addends"] or (0, 0)
        # The source uses DAT_x + hi, so the global runs past it, unless it is
        # an array and hi a whole number of elements: then it may be an end
        # pointer (`&a[10]`), and a known address exactly there starts the
        # next object.
        reach = addr + max(hi, 0)
        # Every file's view counts: an array of 4-byte entries in one file and
        # of bytes in another is reached at +1 for a field, not for an end.
        views = [v for v in g["typed"].values() if v is not None]
        elems = [type_size(v[2]) for v in views if v[0] == "arr"]
        elem = max((e or 0 for e in elems), default=0) if elems and all(elems) else None
        if elem and hi > 0 and hi % elem == 0:
            i = bisect.bisect_left(ordered, reach)
        else:
            i = bisect.bisect_right(ordered, reach)
        nxt = ordered[i] if i < len(ordered) else reach + 1
        end = min(nxt, img.section_end(addr))
        gap = max(end - addr, 1)
        tsize = type_size(t)
        # The largest view: a char in one file and an int in another is an int.
        sizes = [type_size(v) for v in views]
        if tsize and all(sizes) and max(sizes) > tsize:
            tsize = max(sizes)
        if re.match(r"(IID|CLSID|GUID)_", g["name"]):
            tsize = 16  # a COM interface or class id, declared through a macro
        section = img.section(addr)
        if tsize:
            size, size_from = tsize, ("type>gap" if tsize > gap and tsize > hi + 1 else "type")
            elem = row_element(t)
            if section == ".bss" and elem and gap > tsize and gap % elem == 0:
                # An uninitialised buffer owns the space up to the next thing the
                # image refers to: the source may declare it too small
                # (DAT_00528ae8 is declared char[0x1e8] in a 0x3e8-byte slot).
                size, size_from = gap, "gap>type"
            elif hi >= tsize and gap > tsize:
                # The source reaches past the type it declares (DAT_00529e00 is
                # an unsigned int in one file and a 16-byte entry in others).
                size, size_from = gap, "gap>type"
        else:
            size, size_from = gap, "gap"
        raw = img.read(addr, min(size, INIT_CAP))
        kind = "data"
        name = g["name"]
        if "?$" in name or "_Tree::" in name:
            kind = "template"
        elif name.lstrip("_") in library_names:
            kind = "library"
        elif section == ".rdata" and len(raw) >= 8 and all(
                struct.unpack_from("<I", raw, i)[0] in func_starts for i in range(0, 8, 4)):
            kind = "vtable"
        elif t is not None and shape(t) in ("float", "double", " float", " double", "const float", "const double"):
            kind = "float"
        elif raw and 0 in raw and raw.index(0) >= 2 and all(32 <= c < 127 or c in (9, 10, 13)
                                                            for c in raw[:raw.index(0)]):
            if t is None or shape(t, loose=True).startswith(("char", " char")) or "char" in shape(t, loose=True):
                kind = "string"
        whole = img.read(addr, size)
        pointers = sum(1 for i in range(0, len(whole) - 3, 4)
                       if 0x401000 <= struct.unpack_from("<I", whole, i)[0] < DATA_HI)
        # Where it is defined: a data file (the global itself, or one that holds
        # it: a field or an entry the code reaches by its address), or the tree
        # files that define it.
        container = inside(extents, addr)
        defined = ([container[3]] if container else []) or sorted(set(g["defined_in"]), key=link_key)
        rows.append({
            "address": f"{addr:#x}", "name": name, "section": section, "size": size,
            "defined": defined[0] if defined else "",
            "size_from": size_from, "kind": kind, "type": key, "type_files": agree, "other_files": other,
            "types": len(g["types"]), "verdict": g["verdict"], "files": len(g["files"]),
            "max_offset": f"{hi:#x}" if g["addends"] and hi > 0 else "",
            "ghidra": labels.get(addr, ""), "pointers": pointers or "",
            "init": raw.hex() if section != ".bss" else "",
            "_t": t, "_g": g, "_gap": gap,
        })
    # A global a tree file defines (a class object, a template's static
    # member) is as big as its largest definition: the offsets the source
    # uses can run past it (an end pointer) to globals of their own.
    data_files = {relative(s) for s in files_of_kind("data")}
    for row in rows:
        sizes = [definition_size(f, row["_g"]) for f in row["_g"]["defined_in"] if f not in data_files]
        sizes = [n for n in sizes if n]
        if sizes and row["defined"] not in data_files:
            row["size"], row["size_from"] = max(sizes), "definition"
    # A global that starts inside another (a field of a struct, an entry of an
    # array the code reaches by its address) is part of it, not a definition
    # of its own: linked separately, the two would be two variables.
    end, outer = 0, None
    for row in rows:
        addr = int(row["address"], 16)
        if outer is not None and addr < end and not row["defined"]:
            base = int(outer["address"], 16)
            row["defined"] = f"in {outer['name']}+{addr - base:#x}"
            if addr + int(row["size"]) > end and outer["size_from"] != "definition":
                # The part runs past the global that holds it: so does the global.
                end = addr + int(row["size"])
                outer["size"], outer["size_from"] = end - base, "parts"
            continue
        if addr + int(row["size"]) > end:
            end, outer = addr + int(row["size"]), row
    return sorted(rows + bss_tails(rows, img, ordered, labels), key=lambda r: int(r["address"], 16))


def bss_tails(rows: list[dict], img: Image, ordered: list[int], labels: dict[int, str]) -> list[dict]:
    """The uninitialised space after a global, up to the next address the
    image or the source refers to: storage nothing names (padding, or a
    variable nothing uses), defined as a byte array DAT_<address> so that the
    data around it keeps its place and nothing of .bss is copied."""
    out = []
    covered = sorted((int(r["address"], 16), int(r["address"], 16) + int(r["size"])) for r in rows)
    for row in rows:
        addr = int(row["address"], 16)
        if row["section"] != ".bss" or row["defined"].startswith("in ") or row["kind"] == "library":
            continue
        end = addr + int(row["size"])
        i = bisect.bisect_right(ordered, end)
        nxt = ordered[i] if i < len(ordered) else img.section_end(addr)
        nxt = min(nxt, img.section_end(addr))
        # Not into another global (a row that starts before the boundary).
        j = bisect.bisect_left(covered, (end, end))
        if j < len(covered) and covered[j][0] < nxt:
            nxt = covered[j][0]
        # Not a stretch shorter than a dword (padding), nor one the runtime
        # library's code reaches (its own data, which its member defines).
        if nxt - end < 4 or any(a <= end < b for a, b in covered[max(0, j - 2):j + 1]) \
                or any(end <= a < nxt for a in LIBRARY_REFS):
            continue
        out.append({
            "address": f"{end:#x}", "name": f"DAT_{end:08x}", "section": ".bss", "size": nxt - end,
            "size_from": "gap", "kind": "unreferenced", "type": f"unsigned char[{nxt - end}]",
            "type_files": 0, "other_files": 0, "types": 0, "verdict": "", "files": 0, "max_offset": "",
            "ghidra": labels.get(end, ""), "pointers": "", "defined": "", "init": "",
            "_t": ("arr", [nxt - end], ("prim", "unsigned char")),
            "_g": {"verdict": "one type", "types": Counter(), "typed": {}, "files": set(), "defined_in": [],
                   "spellings": Counter()},
            "_gap": nxt - end,
        })
    return out


def definition_size(src: str, g: dict) -> int | None:
    """The size of the global a tree file defines: its symbol's slice of its
    section in the file's object."""
    from place import parse
    obj_path = ROOT / "build/progress" / Path(src).relative_to("src").with_suffix(".obj")
    if not obj_path.exists():
        return None
    obj = parse(obj_path)
    for name in g["spellings"]:
        sym = obj.externals.get(name)
        if sym is not None and sym.section > 0 and not obj.secs[sym.section - 1].is_code:
            lo, hi = obj.secs[sym.section - 1].slice_at(sym.value)
            return hi - lo
    return None


def write_csv(rows: list[dict]) -> None:
    with GLOBALS.open("w", newline="") as fh:
        w = csv.DictWriter(fh, COLUMNS, lineterminator="\n", extrasaction="ignore")
        w.writeheader()
        w.writerows(rows)


# --- link/globals.h and link/data.cpp ----------------------------------------------

def identifier(row: dict) -> str:
    """A name a header can declare: data/symbols.csv's when it is a plain
    identifier, else DAT_<address>."""
    name = row["name"]
    return name if re.fullmatch(r"[A-Za-z_]\w*", name) else f"DAT_{int(row['address'], 16):08x}"


@dataclass
class Entry:
    row: dict
    name: str      # the identifier declared
    t: tuple       # the type declared (a byte array for a struct by value)
    linkage: str   # '"C" ' or ''
    note: str

    @property
    def declaration(self) -> str:
        return f"extern {self.linkage}{declare(self.t, self.name)};"

    @property
    def definition(self) -> str:
        return declare(self.t, self.name)


def header_entry(row: dict) -> Entry | None:
    """The declaration of one global for globals.h, or None when its type is not settled."""
    g, t = row["_g"], row["_t"]
    if row["kind"] in ("template", "library", "vtable") or row["defined"]:
        return None
    if row["kind"] == "unreferenced":
        return Entry(row, identifier(row), t, "", f"{row['address']}, {row['size']} bytes; nothing refers to it")
    if not settled(g, row["type"], row["type_files"], row["other_files"]):
        return None
    name = identifier(row)
    size = int(row["size"])
    note = f"{row['address']}, {size} bytes"
    agree = f"{row['type_files']} of {row['type_files'] + row['other_files']} files"
    if g["verdict"] != "one type":
        agree += f" ({g['verdict']})"
    if name != row["name"]:
        note += f", {row['name']}"
    linkage = ""
    byte_array = ("arr", [size], ("prim", "unsigned char"))
    if row["type"].startswith('extern "C"'):
        # The symbol carries no type, so any declaration links; keep it C.
        linkage, t = '"C" ', c_linkage_type(g, name)
        if t is None:
            return Entry(row, name, byte_array, linkage, f"{note}; declared extern \"C\", type unknown")
        agree = "declared extern \"C\" in " + agree
        if type_size(t):
            note = f"{row['address']}, {type_size(t)} bytes"
    if t is not None and t[0] == "arr" and not all(t[1]):
        # An array declared without its extent everywhere: give it the size we know.
        elem = type_size(t[2])
        if elem and size % elem == 0 and portable(t[2]):
            t = ("arr", [size // elem] + list(t[1][1:]), t[2])
    elif row["size_from"] in ("gap>type", "parts") and t is not None and t[0] == "arr" and portable(t[2]) \
            and size % row_element(t) == 0:
        # An uninitialised buffer that runs on to the next known address, or
        # an array with globals of its own inside it that run past its end.
        t = ("arr", [size // row_element(t)] + list(t[1][1:]), t[2])
        note += f" (declared {row['type']})"
    elif row["size_from"] == "parts" and type_size(t) != size:
        return Entry(row, name, byte_array, linkage, f"{note}; {row['type']} with the globals inside it")
    if type_size(t) is not None and type_size(t) < size:
        # Smaller than what the source reaches inside it.
        return Entry(row, name, byte_array, linkage, f"{note}; {row['type']} in {agree}, but used past its end")
    if portable(t):
        return Entry(row, name, t, linkage, f"{note}; {agree}")
    return Entry(row, name, byte_array, linkage, f"{note}; {row['type']} by value in {agree}")


def forward_declarations(entries: list[Entry]) -> list[str]:
    out = set()
    for e in entries:
        for kw, name in records(e.t):
            if "<" not in name and "::" not in name:
                out.add(f"{kw} {name};")
    return sorted(out)


def write_header(rows: list[dict]) -> list[Entry]:
    entries, skipped = [], []
    for row in rows:
        e = header_entry(row)
        if e:
            entries.append(e)
        else:
            skipped.append(row)
    lines = [
        "// Generated by tools/globals.py from data/globals.csv: do not edit.",
        "//",
        "// The game's global data, declared once with the type most of the source",
        "// gives it. Not included by any file under src/ yet; see docs/linking.md.",
        "// A struct held by value is declared as a byte array of its size.",
        "",
        "#ifndef LINK_GLOBALS_H",
        "#define LINK_GLOBALS_H",
        "",
    ]
    fwd = forward_declarations(entries)
    lines += fwd + ([""] if fwd else [])
    width = max(len(e.declaration) for e in entries)
    for e in entries:
        lines.append(f"{e.declaration:{width}s}  // {e.note}")
    lines += ["", f"// Not declared: {len(skipped)} globals defined in a data file or whose type is not settled "
              "(see data/globals.csv)."]
    for row in sorted(skipped, key=lambda r: -r["files"]):
        if row["defined"].startswith("in "):
            why = f"part of another global: {row['defined'][3:]}"
        elif row["defined"]:
            why = f"defined in {row['defined']}"
        elif row["kind"] in ("template", "library", "vtable"):
            why = f"{row['kind']}"
        else:
            views = ", ".join(f"{k} ({n})" for k, n in row["_g"]["types"].most_common(4))
            more = len(row["_g"]["types"]) - 4
            why = views + (f", and {more} more" if more > 0 else "")
        lines.append(f"//   {row['address']} {row['name']}: {why}")
    lines += ["", "#endif", ""]
    HEADER.write_text("\n".join(lines))
    return entries


def c_string(raw: bytes) -> str:
    out = []
    for i, c in enumerate(raw):
        if c == 0x22 or c == 0x5C or (c == 0x3F and i and raw[i - 1] == 0x3F):
            out.append("\\" + chr(c))  # quote, backslash, and `??` (a trigraph)
        elif 32 <= c < 127:
            # A hex escape swallows following hex digits, so break the literal.
            if out and out[-1].startswith("\\x") and chr(c) in "0123456789abcdefABCDEF":
                out.append('" "')
            out.append(chr(c))
        elif c == 10:
            out.append("\\n")
        elif c == 9:
            out.append("\\t")
        elif c == 13:
            out.append("\\r")
        else:
            out.append(f"\\x{c:02x}")
    return '"' + "".join(out) + '"'


PRIM_FORMAT = {"signed char": "b", "char": "b", "unsigned char": "B", "short": "h", "unsigned short": "H",
               "int": "i", "unsigned int": "I", "long": "i", "unsigned long": "I", "bool": "B",
               "float": "f", "double": "d", "long double": "d", "__int64": "q", "unsigned __int64": "Q",
               "wchar_t": "H"}


class Values:
    """Initialisers for link/data.cpp, built from the exe's bytes."""

    def __init__(self, by_address: dict[int, str], arrays: set[str], image: Image, symbols: dict[str, int],
                 extents: list[tuple[int, int]]):
        self.by_address, self.arrays, self.img = by_address, arrays, image
        self.functions = {a: n for n, a in symbols.items() if a < 0x4FC000}
        self.extents = sorted(extents)      # (address, size) of every row
        # Data that a pointer in an initial value points at and no row names
        # (DAT_0050a788's GUIDs): address -> (size, what points at it).
        self.pointees: dict[int, tuple[int, str]] = {}
        self.current = ""
        self.todo = 0

    def pointee(self, v: int, t: tuple) -> str | None:
        """Name unnamed data in .rdata or .data that a pointer points at: a
        byte array from v to the next known address, defined in data.cpp."""
        if self.img.section(v) not in (".rdata", ".data") or v in self.functions:
            return None
        i = bisect.bisect_right(self.extents, (v, float("inf"))) - 1
        if i >= 0 and v < self.extents[i][0] + self.extents[i][1]:
            return None     # inside a row: not a start of its own
        ordered = self.img.boundaries
        j = bisect.bisect_right(ordered, v)
        end = min(ordered[j] if j < len(ordered) else v + 1, self.img.section_end(v))
        size, what = self.pointees.get(v, (end - v, ""))
        pointee = t[2] if t[2][0] != "cv" else t[2][2]
        what = what or f"a {declare(pointee)} {self.current} points at"
        self.pointees[v] = (size, what)
        return f"({declare(t)})DAT_{v:08x}"

    def scalar(self, t: tuple, raw: bytes) -> str:
        k = t[0]
        if k == "cv":
            return self.scalar(t[2], raw)
        if k == "prim":
            (v,) = struct.unpack("<" + PRIM_FORMAT[t[1]], raw)
            if t[1] == "float":
                return f"{v!r}f" if v == v and abs(v) != float("inf") else None
            if t[1] in ("double", "long double"):
                return repr(v) if v == v and abs(v) != float("inf") else None
            if v == -0x80000000:
                return "(-2147483647 - 1)"  # the literal 2147483648 would be unsigned
            return str(v) + ("u" if t[1].startswith("unsigned") and v > 0x7FFFFFFF else "")
        if k in ("ptr",):
            (v,) = struct.unpack("<I", raw)
            if v == 0:
                return "0"
            if v in self.by_address:
                target = self.by_address[v]
                cast = f"({declare(t)})"
                ref = target if target in self.arrays else f"&{target}"
                return f"{cast}{ref}"
            pointee = t[2] if t[2][0] != "cv" else t[2][2]
            if pointee[0] == "prim" and ("char" in pointee[1] or pointee[1] == "void"):
                s = self.img.read(v, 256)
                if s and 0 in s:
                    return f"({declare(t)}){c_string(s[:s.index(0)])}"
            named = self.pointee(v, t)
            if named:
                return named
            self.todo += 1
            what = self.functions.get(v, "")
            return f"0 /* TODO: {v:#x}{' ' + what if what else ''} */"
        return None

    def value(self, t: tuple, raw: bytes) -> str | None:
        if t[0] == "cv":
            return self.value(t[2], raw)
        if t[0] == "arr":
            elem = t[2] if len(t[1]) == 1 else ("arr", t[1][1:], t[2])
            esize = type_size(elem)
            if not esize:
                return None
            inner = elem[2] if elem[0] == "cv" else elem
            if inner[0] == "prim" and inner[1] in ("char", "signed char", "unsigned char") and 0 in raw:
                cut = raw.index(0)
                if cut > 0 and not any(raw[cut:]) and all(32 <= c < 127 or c in (9, 10, 13) for c in raw[:cut]):
                    return c_string(raw[:cut])
            if not any(raw):
                return "{0}"
            # Drop trailing zero elements: aggregate initialisation zeroes them.
            n = len(raw) // esize
            while n and not any(raw[(n - 1) * esize:n * esize]):
                n -= 1
            items = [self.value(elem, raw[i * esize:(i + 1) * esize]) for i in range(n)]
            if any(i is None for i in items):
                return None
            per = 16 if esize == 1 else 8 if esize <= 4 else 4
            rows = [", ".join(items[i:i + per]) for i in range(0, len(items), per)]
            return "{\n    " + ",\n    ".join(rows) + "\n}" if len(rows) > 1 else "{" + rows[0] + "}"
        return self.scalar(t, raw)


def write_data(entries: list[Entry], count: int, img: Image) -> list[Entry]:
    """Define the `count` most-referenced declared globals (all, if count is 0)."""
    symbols = load_symbols()
    chosen = sorted(entries, key=lambda e: (-e.row["files"], e.row["address"]))
    if count:
        chosen = chosen[:count]
    chosen.sort(key=lambda e: int(e.row["address"], 16))
    by_address = {int(e.row["address"], 16): e.name for e in entries}
    arrays = {e.name for e in entries if e.t[0] == "arr"}
    extents = [(int(e.row["address"], 16), int(e.row["size"])) for e in entries]
    vals = Values(by_address, arrays, img, symbols, extents)
    head = [
        "// Generated by tools/globals.py from data/globals.csv: do not edit.",
        "//",
        "// Definitions of the most-referenced globals in link/globals.h, with the",
        "// initial values the original exe holds. Pointers to strings become string",
        "// literals, pointers to other globals in this file become their address,",
        "// and pointers to data no row names (DAT_0050a788's GUIDs) point at a byte",
        "// array of that data defined here; any other pointer is marked TODO. Not",
        "// part of any build yet; see docs/linking.md.",
        "",
        '#include "globals.h"',
        "",
    ]
    body: list[tuple[int, list[str]]] = []
    for e in chosen:
        row, t, definition = e.row, e.t, e.definition
        addr = int(row["address"], 16)
        section = row["section"]
        vals.current = e.name
        lines = []
        body.append((addr, lines))
        size = type_size(t) or int(row["size"])
        note = f"// {row['address']} {section}"
        if row["pointers"] and not (t[0] == "ptr" or (t[0] == "arr" and t[2][0] == "ptr")):
            note += (f" (holds {row['pointers']} value(s) that look like addresses in the exe: "
                     "they need symbols before this can be relinked)")
        if row["size_from"] == "type>gap":
            note += f" (the type runs past the next known address, {row['address']}+{row['_gap']:#x})"
        if section in (".bss", ""):
            lines.append(f"{definition};  {note}")
        else:
            # A global can run past the raw bytes of .data into its zero-filled tail.
            raw = img.read(addr, size).ljust(size, b"\0")
            init = vals.value(t, raw) if t is not None else None
            if init is None:
                lines.append(f"{definition};  {note}: TODO initial value ({raw[:16].hex()}...)")
                vals.todo += 1
            elif "\n" in init:
                lines.append(f"{note}")
                lines.append(f"{definition} = {init};")
            else:
                lines.append(f"{definition} = {init};  {note}")
    # The data pointers in those initial values point at, which no row names.
    pointees = []
    for addr, (size, what) in sorted(vals.pointees.items()):
        section = img.section(addr)
        const = "const " if section == ".rdata" else ""
        t = ("arr", [size], ("prim", "unsigned char"))
        init = vals.value(t, img.read(addr, size).ljust(size, b"\0"))
        definition = f"{const}unsigned char DAT_{addr:08x}[{size}]"
        # extern: a const global would otherwise be internal to this file.
        body.append((addr, [f"extern {definition} = {init};  // {addr:#x} {section}, {what}"]))
        row = {"address": f"{addr:#x}", "size": size, "section": section, "files": 0}
        pointees.append(Entry(row, f"DAT_{addr:08x}", t, "", what))
    if pointees:
        # Declared first: a global may point at data at a higher address.
        head += ["// Data the initial values below point at, which no row names."]
        head += [f"extern {'const ' if e.row['section'] == '.rdata' else ''}{e.definition};" for e in pointees]
        head += [""]
    head.extend(line for _, block in sorted(body, key=lambda b: b[0]) for line in block)
    head.append("")
    DATA.write_text("\n".join(head))
    print(f"link/data.cpp: {len(chosen)} definitions and {len(pointees)} of data they point at, "
          f"{vals.todo} initial values left as TODO")
    return chosen + pointees


# --- checking link/data.cpp ----------------------------------------------------------

def check_data(objects, defined: list[Entry], img: Image) -> None:
    """Compile link/data.cpp and compare each definition with the exe, then
    count the references in the tree its spellings satisfy."""
    obj_path, log = compile_source(DATA, out_dir="link")
    if obj_path is None:
        sys.exit(f"link/data.cpp does not compile:\n{log}")
    obj = parse_object(obj_path.read_bytes(), obj_path.name)
    by_ident = {e.name: e.row for e in defined}
    ok, bad, pointers = 0, [], 0
    names = {}
    for s in obj.symbols:
        if s.storage_class != 2:
            continue
        qname, _ = data_symbol(s.name)
        row = by_ident.get(qname.split("::")[-1])
        if not row:
            continue
        names[s.name] = row
        if s.section <= 0:
            if s.section == 0 and s.value:  # communal: uninitialised
                ok += 1
            continue
        sec = obj.sections[s.section - 1]
        size = int(row["size"])
        if sec.name.startswith(".bss"):
            if row["section"] == ".bss" or not any(img.read(int(row["address"], 16), size)):
                ok += 1
            else:
                bad.append(f"{row['address']} {qname}: ours uninitialised, original "
                           f"{img.read(int(row['address'], 16), 16).hex()}")
            continue
        ours = bytearray(sec.data[s.value:s.value + size])
        theirs = img.read(int(row["address"], 16), size).ljust(size, b"\0")
        masked = [r.offset - s.value for r in sec.relocs if s.value <= r.offset < s.value + size]
        for off in masked:  # pointers: compared as "some address", not by value
            ours[off:off + 4] = theirs[off:off + 4]
            pointers += 1
        if bytes(ours) == theirs or (row["section"] == ".bss" and not any(ours)):
            ok += 1
        else:
            bad.append(f"{row['address']} {qname}: ours {bytes(ours[:16]).hex()} original {theirs[:16].hex()}")
    warnings = [l for l in log.splitlines() if "warning" in l]
    print(f"link/data.cpp compiles ({len(warnings)} warnings); {ok} of {len(names)} definitions hold the "
          f"original's bytes ({pointers} pointer fields compared as addresses)")
    for w in warnings[:10]:
        print("  ", w.split("data.cpp", 1)[-1])
    for b in bad[:20]:
        print("  differs:", b)
    # How far the definitions go towards resolving the tree's references: a
    # reference resolves only if it spells the global exactly as data.cpp does.
    symbols = load_symbols()
    declared = {int(e.row["address"], 16) for e in defined}
    matched = total = 0
    for o in objects:
        for n, is_func in o.refs.items():
            if is_func or address_of(n, symbols) not in declared:
                continue
            total += 1
            matched += n in names
    print(f"of the tree's {total} references to the globals it defines, {matched} spell them exactly "
          f"as data.cpp does; the rest declare another type")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true", help="compile link/data.cpp and compare it with the exe")
    ap.add_argument("--count", type=int, default=0,
                    help="define only the N most-referenced globals in link/data.cpp (default: all declared)")
    args = ap.parse_args()

    objects, failed = load_objects()
    if failed:
        print(f"warning: {len(failed)} files failed to compile and are left out", file=sys.stderr)
    img = Image()
    rows = build(objects, img)
    write_csv(rows)
    LINK.mkdir(exist_ok=True)
    entries = write_header(rows)
    defined = write_data(entries, args.count, img)
    kinds = Counter(r["kind"] for r in rows)
    sections = Counter(r["section"] for r in rows)
    print(f"data/globals.csv: {len(rows)} globals ({', '.join(f'{n} {k}' for k, n in kinds.most_common())}; "
          f"{', '.join(f'{n} in {s}' for s, n in sorted(sections.items()))})")
    print(f"link/globals.h: {len(entries)} declared, {len(rows) - len(entries)} left out (type not settled)")
    if args.check:
        check_data(objects, defined, img)


if __name__ == "__main__":
    main()
