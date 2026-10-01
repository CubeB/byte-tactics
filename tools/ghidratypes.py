"""Collect the types and signatures matched source declares, for Ghidra.

    uv run tools/ghidratypes.py              # write build/types.json
    uv run tools/ghidratypes.py --partial    # also take signatures from partials

Every file with a matched function is compiled again with /Z7, and the
CodeView records the compiler writes are read back: struct, union and enum
layouts exactly as VC5 laid them out, and each defined function's
calling convention, return type and parameters. tools/ghidra/BtImportTypes.java
applies the result to the Ghidra project before tools/ghidra.sh exports
pseudo-C, so unmatched functions decompile against the known layouts and
callee signatures instead of raw offsets.

Files disagree about some classes (see tools/unitmap.py). For each name the
layout the most files share wins, then the one with the most fields; the
losers are counted in the summary, not resolved.
"""

import argparse
import csv
import hashlib
import json
import os
import re
import struct
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from check import DEFAULT_FLAGS, ROOT, compile_source
from coff import parse_object

PROGRESS = ROOT / "data/progress.csv"
GLOBALS = ROOT / "data/globals.csv"
OUT = ROOT / "build/types.json"
FLAGS = DEFAULT_FLAGS + " /Z7"

# VC5 writes CodeView 4 with 32-bit type indices (the "_ST" leaves).
LF_MODIFIER, LF_POINTER, LF_ARRAY, LF_CLASS, LF_STRUCTURE = 0x1001, 0x1002, 0x1003, 0x1004, 0x1005
LF_UNION, LF_ENUM, LF_PROCEDURE, LF_MFUNCTION = 0x1006, 0x1007, 0x1008, 0x1009
LF_ARGLIST, LF_FIELDLIST, LF_BITFIELD = 0x1201, 0x1203, 0x1205
LF_BCLASS, LF_VBCLASS, LF_IVBCLASS, LF_ENUMERATE = 0x1400, 0x1401, 0x1402, 0x0403
LF_FRIENDFCN, LF_INDEX, LF_MEMBER, LF_STMEMBER, LF_METHOD = 0x1403, 0x1404, 0x1405, 0x1406, 0x1407
LF_NESTTYPE, LF_VFUNCTAB, LF_FRIENDCLS, LF_ONEMETHOD, LF_VFUNCOFF = 0x1408, 0x1409, 0x140a, 0x140b, 0x140c
S_END, S_UDT, S_BPREL32, S_LPROC32, S_GPROC32 = 0x0006, 0x1003, 0x1006, 0x100a, 0x100b
FWDREF = 0x80

CALLS = {0x00: "__cdecl", 0x04: "__fastcall", 0x07: "__stdcall", 0x0b: "__thiscall"}
PRIMS = {
    0x03: "void", 0x08: "int",  # HRESULT
    0x10: "char", 0x11: "short", 0x12: "long", 0x13: "longlong",
    0x20: "uchar", 0x21: "ushort", 0x22: "ulong", 0x23: "ulonglong",
    0x30: "bool", 0x31: "short", 0x32: "int",
    0x40: "float", 0x41: "double", 0x42: "longdouble",
    0x70: "char", 0x71: "wchar", 0x72: "short", 0x73: "ushort",
    0x74: "int", 0x75: "uint", 0x76: "longlong", 0x77: "ulonglong",
}
FILLER = re.compile(r"(unknown|pad|gap)_?[0-9a-fA-F]*$")
UNNAMED = ("<unnamed-tag>", "__unnamed", "")


def numeric(data: bytes, pos: int) -> tuple[int, int]:
    """A CodeView numeric leaf at pos: (value, position after it)."""
    v = struct.unpack_from("<H", data, pos)[0]
    if v < 0x8000:
        return v, pos + 2
    fmt = {0x8000: "<b", 0x8001: "<h", 0x8002: "<H", 0x8003: "<i", 0x8004: "<I",
           0x8009: "<q", 0x800a: "<Q"}[v]
    return struct.unpack_from(fmt, data, pos + 2)[0], pos + 2 + struct.calcsize(fmt)


def pstring(data: bytes, pos: int) -> tuple[str, int]:
    n = data[pos]
    return data[pos + 1:pos + 1 + n].decode("latin-1"), pos + 1 + n


class TypeTable:
    """One object's .debug$T, turned into index-free type expressions.

    An expression is a primitive name ("int"), {"ptr": e}, {"arr": e, "n": k},
    {"ref": name} for a named struct, union or enum, {"fn": sig} for a
    function type, or {"raw": size} for what Ghidra has no type for.
    """

    def __init__(self, data: bytes):
        self.records: dict[int, tuple[int, bytes]] = {}
        pos, index = 4, 0x1000
        while pos + 4 <= len(data):
            length, leaf = struct.unpack_from("<HH", data, pos)
            self.records[index] = (leaf, data[pos + 4:pos + 2 + length])
            pos += 2 + length
            index += 1
        self.defs: dict[str, dict] = {}
        self.anon: dict[int, str] = {}
        self.full: dict[tuple[int, str], int] = {}

    def expr(self, ti: int, depth: int = 0):
        if ti < 0x1000:
            name = PRIMS.get(ti & 0xff, "undefined")
            return {"ptr": name} if (ti >> 8) & 7 else name
        if depth > 40 or ti not in self.records:
            return "undefined"
        leaf, d = self.records[ti]
        if leaf == LF_MODIFIER:
            return self.expr(struct.unpack_from("<I", d)[0], depth + 1)
        if leaf == LF_POINTER:
            utype, attr = struct.unpack_from("<II", d)
            if (attr >> 5) & 7 in (2, 3):  # pointer to member
                return {"raw": 4}
            return {"ptr": self.expr(utype, depth + 1)}
        if leaf == LF_ARRAY:
            elem = struct.unpack_from("<I", d)[0]
            size, _ = numeric(d, 8)
            esize = self.size_of(elem)
            return {"arr": self.expr(elem, depth + 1), "n": size // esize if esize else 0}
        if leaf in (LF_CLASS, LF_STRUCTURE, LF_UNION, LF_ENUM):
            return {"ref": self.define(ti)}
        if leaf in (LF_PROCEDURE, LF_MFUNCTION):
            return {"fn": self.signature(ti, depth + 1)}
        if leaf == LF_BITFIELD:
            return self.expr(struct.unpack_from("<I", d)[0], depth + 1)
        return "undefined"

    def size_of(self, ti: int) -> int:
        if ti < 0x1000:
            if (ti >> 8) & 7:
                return 4
            return {0x10: 1, 0x20: 1, 0x30: 1, 0x70: 1, 0x40: 4, 0x41: 8, 0x42: 10,
                    0x11: 2, 0x21: 2, 0x31: 2, 0x71: 2, 0x72: 2, 0x73: 2,
                    0x13: 8, 0x23: 8, 0x76: 8, 0x77: 8}.get(ti & 0xff, 4)
        leaf, d = self.records.get(ti, (0, b""))
        if leaf == LF_MODIFIER:
            return self.size_of(struct.unpack_from("<I", d)[0])
        if leaf == LF_POINTER:
            return 4
        if leaf == LF_ARRAY:
            return numeric(d, 8)[0]
        if leaf in (LF_CLASS, LF_STRUCTURE, LF_UNION, LF_ENUM):
            full = self.resolve(ti)
            leaf, d = self.records[full]
            if leaf == LF_ENUM:
                return self.size_of(struct.unpack_from("<I", d, 4)[0])
            return numeric(d, 16 if leaf != LF_UNION else 8)[0]
        return 4

    def resolve(self, ti: int) -> int:
        """The full definition of a forward reference, when this object has one."""
        if not self.full:
            for other, (leaf, d) in self.records.items():
                if leaf in (LF_CLASS, LF_STRUCTURE, LF_UNION, LF_ENUM) \
                        and not struct.unpack_from("<H", d, 2)[0] & FWDREF:
                    self.full.setdefault((leaf, self.composite_name(other)), other)
        leaf, d = self.records[ti]
        if not struct.unpack_from("<H", d, 2)[0] & FWDREF:
            return ti
        return self.full.get((leaf, self.composite_name(ti)), ti)

    def composite_name(self, ti: int) -> str:
        leaf, d = self.records[ti]
        if leaf == LF_ENUM:
            return pstring(d, 12)[0]
        _, pos = numeric(d, 16 if leaf != LF_UNION else 8)
        return pstring(d, pos)[0]

    def define(self, ti: int) -> str:
        """Record the composite or enum at ti in self.defs; return its name."""
        ti = self.resolve(ti)
        name = self.composite_name(ti)
        if name in UNNAMED:
            if ti in self.anon:
                return self.anon[ti]
            name = self.anon[ti] = f"anon_{ti:x}"  # renamed by layout hash below
        if name in self.defs:
            return name
        leaf, d = self.records[ti]
        count, prop = struct.unpack_from("<HH", d)
        if prop & FWDREF:
            self.defs[name] = {"kind": "union" if leaf == LF_UNION else "struct", "size": 0,
                               "fields": [], "opaque": True}
            return name
        if leaf == LF_ENUM:
            utype, flist = struct.unpack_from("<II", d, 4)
            entry = {"kind": "enum", "size": self.size_of(utype), "values": []}
            self.defs[name] = entry
            for sub, s, pos in self.fieldlist(flist):
                if sub == LF_ENUMERATE:
                    value, p = numeric(s, pos + 2)
                    entry["values"].append([pstring(s, p)[0], value])
            return name
        flist = struct.unpack_from("<I", d, 4)[0]
        size, _ = numeric(d, 16 if leaf != LF_UNION else 8)
        entry = {"kind": "union" if leaf == LF_UNION else "struct", "size": size, "fields": []}
        self.defs[name] = entry
        for sub, s, pos in self.fieldlist(flist):
            if sub == LF_BCLASS:
                base = struct.unpack_from("<I", s, pos + 2)[0]
                off, _ = numeric(s, pos + 6)
                entry["fields"].append({"off": off, "name": "base_" + self.define(base),
                                        "type": {"ref": self.define(base)}})
            elif sub == LF_VFUNCTAB:
                entry["fields"].append({"off": 0, "name": "vftable", "type": {"ptr": "void"}})
            elif sub == LF_MEMBER:
                mt = struct.unpack_from("<I", s, pos + 2)[0]
                off, p = numeric(s, pos + 6)
                field = {"off": off, "name": pstring(s, p)[0], "type": self.expr(mt)}
                if FILLER.match(field["name"]) and isinstance(field["type"], dict) \
                        and field["type"].get("arr") in ("char", "uchar"):
                    # Ghidra shows undefined bytes as p->field_0xb8, a filler array as p->unknown_0 + 0xb8.
                    continue
                bl, bd = self.records.get(mt, (0, b""))
                if bl == LF_BITFIELD:
                    field["bits"] = [bd[5], bd[4]]  # position, length
                    field["width"] = self.size_of(struct.unpack_from("<I", bd)[0])
                entry["fields"].append(field)
        return name

    def fieldlist(self, ti: int):
        """(subleaf, record bytes, position after the subleaf) for each entry,
        following LF_INDEX continuations."""
        seen = set()
        while ti in self.records and ti not in seen:
            seen.add(ti)
            leaf, d = self.records[ti]
            if leaf != LF_FIELDLIST:
                return
            pos, ti = 0, None
            while pos + 2 <= len(d):
                if d[pos] >= 0xf0:
                    pos += d[pos] & 0x0f
                    continue
                sub = struct.unpack_from("<H", d, pos)[0]
                p = pos + 2
                yield sub, d, p
                if sub == LF_ENUMERATE:
                    _, p = numeric(d, p + 2)
                    _, p = pstring(d, p)
                elif sub == LF_BCLASS:
                    _, p = numeric(d, p + 6)
                elif sub in (LF_VBCLASS, LF_IVBCLASS):
                    _, p = numeric(d, p + 10)
                    _, p = numeric(d, p)
                elif sub == LF_MEMBER:
                    _, p = numeric(d, p + 6)
                    _, p = pstring(d, p)
                elif sub in (LF_STMEMBER, LF_NESTTYPE, LF_FRIENDFCN):
                    _, p = pstring(d, p + 6)
                elif sub == LF_METHOD:
                    _, p = pstring(d, p + 6)
                elif sub == LF_ONEMETHOD:
                    attr = struct.unpack_from("<H", d, p)[0]
                    p += 6 + (4 if (attr >> 2) & 7 in (4, 6) else 0)
                    _, p = pstring(d, p)
                elif sub in (LF_VFUNCTAB, LF_FRIENDCLS):
                    p += 6
                elif sub == LF_VFUNCOFF:
                    p += 10
                elif sub == LF_INDEX:
                    ti = struct.unpack_from("<I", d, p + 2)[0]
                    break
                else:
                    return  # an entry this reader does not know; the rest is unreadable
                pos = p

    def signature(self, ti: int, depth: int = 0) -> dict:
        leaf, d = self.records[ti]
        if leaf == LF_MFUNCTION:
            rv, _, this, call, _, _, args = struct.unpack_from("<IIIBBHI", d)
        else:
            rv, call, _, _, args = struct.unpack_from("<IBBHI", d)
            this = 0
        params, varargs = [], False
        _, ad = self.records.get(args, (0, b"\0\0\0\0"))
        for i in range(struct.unpack_from("<I", ad)[0]):
            pt = struct.unpack_from("<I", ad, 4 + 4 * i)[0]
            if pt == 0:
                varargs = True
            else:
                params.append(self.expr(pt, depth + 1))
        sig = {"conv": CALLS.get(call, "__cdecl"), "ret": self.expr(rv, depth + 1),
               "params": params, "varargs": varargs}
        if this:
            sig["this"] = self.expr(this, depth + 1)
        return sig


def symbols_start(data: bytes) -> int:
    # Only the object's main .debug$S starts with the CodeView signature; the
    # per-function COMDAT ones start straight with records.
    return 4 if data[:4] == b"\x02\0\0\0" else 0


def functions(obj, table: TypeTable, symbol_address: dict[str, int]) -> list[dict]:
    """Each S_GPROC32/S_LPROC32 whose COFF symbol is a known function address."""
    out = []
    for sec in obj.sections:
        if sec.name != ".debug$S":
            continue
        relocs = {r.offset: r.symbol for r in sec.relocs}
        d, pos, current = sec.data, symbols_start(sec.data), None
        while pos + 4 <= len(d):
            length, kind = struct.unpack_from("<HH", d, pos)
            body = pos + 4
            if kind in (S_GPROC32, S_LPROC32):
                ti = struct.unpack_from("<I", d, body + 24)[0]
                name, _ = pstring(d, body + 35)
                address = symbol_address.get(relocs.get(body + 28, ""))
                current = None
                if address is not None and ti in table.records:
                    current = {"address": f"0x{address:x}", "name": name,
                               **table.signature(ti), "names": []}
                    out.append(current)
            elif kind == S_BPREL32 and current is not None:
                off = struct.unpack_from("<i", d, body)[0]
                name, _ = pstring(d, body + 8)
                if off > 0 and name != "this":
                    current["names"].append(name)
            elif kind == S_END:
                current = None
            pos += 2 + length
    for f in out:
        # Parameters come first; positive offsets after them are inlined
        # callees' parameters.
        names = f.pop("names")
        f["param_names"] = names[:len(f["params"])] if len(names) >= len(f["params"]) else []
    return out


def typedefs(obj, table: TypeTable) -> dict[str, object]:
    out = {}
    for sec in obj.sections:
        if sec.name != ".debug$S":
            continue
        d, pos = sec.data, symbols_start(sec.data)
        while pos + 4 <= len(d):
            length, kind = struct.unpack_from("<HH", d, pos)
            if kind == S_UDT:
                ti = struct.unpack_from("<I", d, pos + 4)[0]
                name, _ = pstring(d, pos + 8)
                out[name] = table.expr(ti)
            pos += 2 + length
    return out


def compile_cached(src: Path, include_hash: str) -> Path | None:
    key = hashlib.sha256(src.read_bytes() + include_hash.encode() + FLAGS.encode()).hexdigest()[:16]
    stamp = ROOT / "build/types" / src.relative_to(ROOT / "src").with_suffix(".key")
    obj = stamp.with_suffix(".obj")
    if stamp.exists() and stamp.read_text() == key and obj.exists():
        return obj
    obj, _ = compile_source(src, FLAGS, out_dir="types")
    if obj:
        stamp.write_text(key)
    return obj


def rename(e, names: dict[str, str]):
    if isinstance(e, dict):
        if "ref" in e:
            return {"ref": names.get(e["ref"], e["ref"])}
        return {k: rename(v, names) for k, v in e.items()}
    if isinstance(e, list):
        return [rename(v, names) for v in e]
    return e


def collect(table: TypeTable, udts: dict[str, object]) -> tuple[dict[str, dict], dict[str, str]]:
    """This object's definitions with anonymous composites given stable names:
    the typedef naming them when there is one, else a hash of the layout."""
    names = {}
    for alias, e in udts.items():
        if isinstance(e, dict) and e.get("ref", "").startswith("anon_"):
            names.setdefault(e["ref"], alias)
    for anon in table.anon.values():
        if anon not in names:
            body = json.dumps(table.defs[anon], sort_keys=True)
            names[anon] = "anon_" + hashlib.sha1(body.encode()).hexdigest()[:8]
    defs = {names.get(n, n): rename(v, names) for n, v in table.defs.items()}
    return defs, names


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--partial", action="store_true", help="also apply signatures of partial matches")
    ap.add_argument("--out", type=Path, default=OUT)
    args = ap.parse_args()

    rows = list(csv.DictReader(PROGRESS.open()))
    wanted = {"matched", "partial"} if args.partial else {"matched"}
    symbol_address = {r["symbol"]: int(r["address"], 16) for r in rows if r["status"] in wanted}
    sources = sorted({ROOT / r["file"] for r in rows if r["status"] in wanted})
    include = ROOT / "include"
    include_hash = hashlib.sha256(b"".join(
        p.read_bytes() for p in sorted(include.rglob("*")) if p.is_file())).hexdigest() if include.exists() else ""
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        objects = list(pool.map(lambda s: compile_cached(s, include_hash), sources))

    variants: dict[str, Counter] = defaultdict(Counter)
    funcs: dict[str, dict] = {}
    failed = 0
    for src, path in zip(sources, objects):
        if path is None:
            failed += 1
            continue
        obj = parse_object(path.read_bytes())
        debug_t = next((s for s in obj.sections if s.name == ".debug$T"), None)
        if debug_t is None:
            continue
        table = TypeTable(debug_t.data)
        udts = typedefs(obj, table)
        found = functions(obj, table, symbol_address)
        defs, names = collect(table, udts)
        for name, body in defs.items():
            variants[name][json.dumps(body, sort_keys=True)] += 1
        for f in found:
            funcs[f["address"]] = rename(f, names)

    types, disputed = {}, set()
    for name, seen in sorted(variants.items()):
        def rank(item):
            body = json.loads(item[0])
            return (bool(body.get("fields", body.get("values"))), item[1],
                    len(body.get("fields", body.get("values", []))), body["size"])
        best = max(seen.items(), key=rank)[0]
        real = [k for k in seen if json.loads(k).get("fields")]
        if len(set(real)) > 1:
            disputed.add(name)
        types[name] = json.loads(best)

    data = []
    for g in csv.DictReader(GLOBALS.open()):
        if g["verdict"].startswith("one type") and g["type"] and g["size"].isdigit():
            data.append({"address": g["address"], "type": g["type"], "size": int(g["size"])})

    # Keep what the signatures and globals use; the rest is mostly windows.h.
    words = {w for g in data for w in re.findall(r"[A-Za-z_]\w*", g["type"])}
    keep, todo = set(), [f for f in funcs.values()] + [{"ref": w} for w in words]
    while todo:
        e = todo.pop()
        if isinstance(e, dict):
            name = e.get("ref")
            if name in types and name not in keep:
                keep.add(name)
                todo.extend(types[name].get("fields", []))
            todo.extend(v for k, v in e.items() if k != "ref")
        elif isinstance(e, list):
            todo.extend(e)
    types = {n: t for n, t in types.items() if n in keep}
    disputed = len(keep & disputed)

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps({"types": types,
                                    "functions": sorted(funcs.values(), key=lambda f: int(f["address"], 16)),
                                    "globals": data}, indent=1))
    print(f"{len(types)} types ({disputed} with conflicting layouts), "
          f"{len(funcs)} function signatures, {len(data)} globals -> {args.out.relative_to(ROOT)}"
          + (f"; {failed} files failed to compile" if failed else ""))


if __name__ == "__main__":
    main()
