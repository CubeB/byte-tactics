"""Build data/functions.csv: every function in the original exe.

Sources:
  - FPO debug records: start, size, argument/local counts, frame info
  - the VC5 SP3 runtime library (LIBCMT): which functions are library code;
    where several members have the same bytes, the one whose calls, imports
    and data agree with the exe's names the function (see References)
  - direct calls found by disassembly: the call graph

kind is one of:
  game     Cavedog code, to be decompiled
  library  statically linked runtime code, matched byte-for-byte already
  gap      code between FPO records (hand-written assembly, thunks, or data)
"""

import argparse
import csv
import struct
from collections import Counter, defaultdict
from pathlib import Path

import capstone
import pefile

from coff import REL_I386_DIR32, REL_I386_REL32, parse_object, read_archive
from crtmatch import MIN_SIZE, find_all_masked, find_masked

ROOT = Path(__file__).resolve().parent.parent
EXE = ROOT / "orig/TotalA.exe"
RUNTIME_LIB = ROOT / "toolchain/msvc5-sp3/LIB/LIBCMT.LIB"
CPP_LIB = ROOT / "toolchain/msvc5-sp3/LIB/LIBCPMT.LIB"
OUT = ROOT / "data/functions.csv"
SUBSTANTIAL = 64  # bytes; library matches this long are never coincidences
FIELDS = ["address", "size", "kind", "name", "params", "locals", "seh", "frame_pointer",
          "calls", "game_calls", "callers"]


def fpo_records(pe: pefile.PE) -> list[dict]:
    d = next(d for d in pe.DIRECTORY_ENTRY_DEBUG if d.struct.Type == 3)
    raw = pe.__data__[d.struct.PointerToRawData:d.struct.PointerToRawData + d.struct.SizeOfData]
    base = pe.OPTIONAL_HEADER.ImageBase
    out = []
    for i in range(0, len(raw), 16):
        start, size, locals_, params, bits = struct.unpack_from("<IIIHH", raw, i)
        out.append({"address": base + start, "size": size, "params": params, "locals": locals_,
                    "seh": (bits >> 11) & 1, "frame_pointer": (bits >> 12) & 1})
    return sorted(out, key=lambda f: f["address"])


# Byte-identical library functions: the name the exe really uses. memcpy and
# memmove are the same code in LIBCMT, but /O2 inlines memcpy, so an
# out-of-line call to that code is memmove (std::char_traits<char>::move).
PREFERRED = {"_memcpy": "_memmove"}


class References:
    """Tell apart library functions whose bytes are the same and whose
    references differ: the locking wrappers _read, _write and _lseek call
    _read_lk, _write_lk and _lseek_lk; _tell and _ismbbkalnum call _lseek and
    x_ismbbtype; _Xlen and _Xran throw different exceptions; zlibVersion
    returns a string where get_crc_table returns a table; remove and _rmdir
    import different functions. A member's code is scored by whether what it
    refers to is what the exe's code at that address refers to."""

    def __init__(self, pe: pefile.PE):
        self.base = pe.OPTIONAL_HEADER.ImageBase
        self.image = pe.get_memory_mapped_image()
        self.imports = {imp.address: imp.name.decode() for entry in pe.DIRECTORY_ENTRY_IMPORT
                        for imp in entry.imports if imp.name}
        # (object for a static, else "", symbol) -> addresses some member's code matched at
        self.at: dict[tuple[str, str], set[int]] = defaultdict(set)
        # address -> the names the chosen members' code uses for it (note_calls)
        self.called: dict[int, Counter] = defaultdict(Counter)

    @staticmethod
    def key(obj, name: str) -> tuple[str, str]:
        static = any(s.name == name and s.section > 0 and s.storage_class == 3 for s in obj.symbols)
        return (obj.name if static else "", name)

    def add(self, obj, sec, va: int) -> None:
        for s in obj.symbols_in(sec):
            self.at[self.key(obj, s.name)].add(va + s.value)

    def u32(self, va: int) -> int | None:
        o = va - self.base
        return struct.unpack_from("<I", self.image, o)[0] if 0 <= o <= len(self.image) - 4 else None

    def same_data(self, obj, sym, offset: int, va: int, depth: int = 3) -> bool | None:
        """Whether the exe holds sym's section's bytes from offset (up to 16, a
        string up to its terminator) at va, ignoring the fields the linker
        fills; None when there is nothing fixed to compare (uninitialised
        data, or a pointer). A field that points at the object's own data
        must lead to the same contents again: length_error's and failure's
        throw information differ only in the type names their tables lead
        to."""
        sec = obj.sections[sym.section - 1]
        o = va - self.base
        if sec.characteristics & 0x80:
            return None
        ours = sec.data[offset:offset + 16]
        if b"\0" in ours and sym.name.startswith(("??_C@", "$SG")):
            ours = ours[:ours.index(b"\0") + 1]
        mask = sec.mask()[offset:offset + len(ours)]
        result = None
        if any(mask):
            theirs = self.image[o:o + len(ours)] if 0 <= o else b""
            if len(theirs) != len(ours) or not all(not m or a == b for a, b, m in zip(ours, theirs, mask)):
                return False
            result = True
        for r in sec.relocs if depth else ():
            if r.type != REL_I386_DIR32 or not offset <= r.offset <= offset + len(ours) - 4:
                continue
            target = next((s for s in obj.symbols if s.name == r.symbol and s.section > 0), None)
            pointer = self.u32(va + r.offset - offset)
            if target is None or pointer is None or obj.sections[target.section - 1].is_code:
                continue
            (addend,) = struct.unpack_from("<I", sec.data, r.offset)
            same = self.same_data(obj, target, target.value + addend, pointer, depth - 1)
            if same is False:
                return False
            result = result or same
        return result

    def score(self, obj, sec, va: int) -> tuple[int, int]:
        """(references that disagree with the exe's, references that agree)
        for sec's code placed at va."""
        bad = good = 0
        syms = {}
        for sym in obj.symbols:
            if sym.section > 0:
                syms.setdefault(sym.name, sym)
        for r in sec.relocs:
            if r.type not in (REL_I386_DIR32, REL_I386_REL32) or r.offset + 4 > len(sec.data):
                continue
            site = va + r.offset
            theirs = self.u32(site)
            if theirs is None:
                continue
            (ours,) = struct.unpack_from("<I", sec.data, r.offset)
            target = (theirs - ours + (site + 4 if r.type == REL_I386_REL32 else 0)) & 0xFFFFFFFF
            if r.symbol.startswith("__imp_"):
                name = self.imports.get(target)
                if name is not None:
                    want = r.symbol[len("__imp_"):].lstrip("_").split("@")[0]
                    bad, good = (bad, good + 1) if name == want else (bad + 1, good)
                continue
            sym = syms.get(r.symbol)
            if sym is not None and not obj.sections[sym.section - 1].is_code:
                # Its own data: a literal, a table, an initial value.
                addend = ours if r.symbol.startswith(".") else 0
                same = self.same_data(obj, sym, sym.value + addend, theirs - addend)
                if same is not None:
                    bad, good = (bad, good + 1) if same else (bad + 1, good)
                continue
            known = self.at.get(self.key(obj, r.symbol))
            if known:
                bad, good = (bad, good + 1) if target in known else (bad + 1, good)
        return bad, good

    def note_calls(self, obj, sec, va: int) -> None:
        """Record the names sec's code (placed at va) calls or points at,
        where the exe's code there points: what callers call each address."""
        for r in sec.relocs:
            if r.type not in (REL_I386_DIR32, REL_I386_REL32) or r.offset + 4 > len(sec.data):
                continue
            site = va + r.offset
            theirs = self.u32(site)
            if theirs is None:
                continue
            (ours,) = struct.unpack_from("<I", sec.data, r.offset)
            target = (theirs - ours + (site + 4 if r.type == REL_I386_REL32 else 0)) & 0xFFFFFFFF
            if not va <= target < va + len(sec.data):
                self.called[target][r.symbol] += 1

    def called_by_name(self, obj, sec, va: int) -> int:
        """How often the chosen members' code calls sec's functions (at va)
        by their own names."""
        return sum(self.called.get(va + s.value, {}).get(s.name, 0) for s in obj.symbols_in(sec))

    def best(self, members: list) -> list:
        """members (obj, sec, address of sec, ...) that match at one address,
        the one whose references agree best first; then the one the chosen
        callers call by name (_strdup and _mbsdup are the same code, and
        only copy_environ's call says which is there); equals keep their
        order."""
        if len(members) < 2:
            return members
        scores = [self.score(m[0], m[1], m[2]) + (self.called_by_name(m[0], m[1], m[2]),) for m in members]
        return [m for _, m in sorted(zip(scores, members), key=lambda x: (x[0][0], -x[0][1], -x[0][2]))]

    def winners(self, found: list) -> set[int]:
        """The ids of the entries of found (obj, sec, address of sec, address,
        ...) that are the best at their address."""
        groups: dict[int, list] = defaultdict(list)
        for f in found:
            groups[f[3]].append(f)
        return {id(self.best(g)[0]) for g in groups.values()}

    def elsewhere(self, obj, sec, hay: bytes, text_va: int, taken) -> list[int]:
        """Where else sec's code matches with every reference agreeing (and at
        least one checked): the other half of a byte-identical pair that lost
        its first match to the half that is really there."""
        out = []
        for off in find_all_masked(hay, sec.data, sec.mask()):
            va = text_va + off
            if va not in taken:
                bad, good = self.score(obj, sec, va)
                if not bad and good:
                    out.append(va)
        return out


def library_names(pe: pefile.PE, refs: References) -> dict[int, str]:
    """Map exe address -> runtime library symbol for every matched library function."""
    text = next(s for s in pe.sections if s.Name.startswith(b".text"))
    hay = text.get_data()
    text_va = pe.OPTIONAL_HEADER.ImageBase + text.VirtualAddress
    matches: dict[int, list] = defaultdict(list)    # section address -> [(order, obj, sec)]
    order = 0
    for obj in read_archive(RUNTIME_LIB):
        for sec in obj.sections:
            if not sec.is_code or len(sec.data) < MIN_SIZE:
                continue
            order += 1
            off = find_masked(hay, sec.data, sec.mask())
            if off < 0:
                continue
            matches[text_va + off].append((obj, sec, text_va + off, order))
            refs.add(obj, sec, text_va + off)
    # Where several members match, the one whose references agree is there,
    # and one that loses may be at another address of its own. A second round
    # also asks what the first round's choices call each address.
    for _ in range(2):
        chosen = {va: refs.best(members)[0] for va, members in matches.items()}
        moved: dict[int, list] = defaultdict(list)
        for members in matches.values():
            for obj, sec, _, order in refs.best(members)[1:]:
                for other in refs.elsewhere(obj, sec, hay, text_va, chosen):
                    moved[other].append((obj, sec, other, order))
        for va, members in moved.items():
            chosen[va] = refs.best(members)[0]
        refs.called.clear()
        for obj, sec, va, _ in chosen.values():
            refs.note_calls(obj, sec, va)
    names = {}
    for va, (obj, sec, _, _) in sorted(chosen.items(), key=lambda c: (c[1][3], c[0])):
        syms = obj.symbols_in(sec)
        for s in syms:
            names.setdefault(va + s.value, PREFERRED.get(s.name, s.name))
        if not syms:
            names.setdefault(va, f"{obj.name.split(chr(92))[-1]}:{sec.name}")
    return names


def cpp_library_names(pe: pefile.PE, refs: References) -> dict[int, str]:
    """Code from the C++ runtime (LIBCPMT): std::string internals, _Lockit, std
    exceptions. Much of it is template code instantiated inside Cavedog's own
    objects, so it sits in the middle of game code rather than in the runtime
    block. Short matches are too often coincidences (any 16-byte getter looks
    like another), so only take 64+ byte matches and 32+ byte ones next to them."""
    text = next(s for s in pe.sections if s.Name.startswith(b".text"))
    hay = text.get_data()
    text_va = pe.OPTIONAL_HEADER.ImageBase + text.VirtualAddress
    found = []
    for obj in read_archive(CPP_LIB):
        for sec in obj.sections:
            if not sec.is_code or len(sec.data) < 32:
                continue
            syms = obj.symbols_in(sec)
            if not syms:
                continue
            # Some library objects were linked twice (two std::_Lockit copies).
            for off in find_all_masked(hay, sec.data, sec.mask()):
                found.append((obj, sec, text_va + off, text_va + off + syms[0].value, len(sec.data), syms[0].name))
                refs.add(obj, sec, text_va + off)
    strong = [f for f in found if f[4] >= SUBSTANTIAL]
    best = refs.winners(found)
    # Again, with what the winners call each address (the copy constructor
    # length_error::_Doraise calls is length_error's, not failure's).
    for f in found:
        if id(f) in best:
            refs.note_calls(f[0], f[1], f[2])
    best = refs.winners(found)
    names = {}
    for f in found:
        _, _, _, addr, size, name = f
        if id(f) in best and (size >= SUBSTANTIAL or any(abs(addr - s[3]) <= 0x200 for s in strong)):
            names.setdefault(addr, name)
    return names


THIRD_PARTY = [("zlib 1.0.4", ROOT / "toolchain/thirdparty/zlib-1.0.4")]
STRONG = 40  # bytes; third-party functions this long are never coincidences


def third_party_names(pe: pefile.PE, refs: References) -> dict[int, str]:
    """Third-party libraries built into the game from their own source (built by
    tools/setup_toolchain.sh with Cavedog's options). Short functions such as
    `return 1` match stubs all over the exe, so they only count inside the
    block the long ones occupy."""
    text = next(s for s in pe.sections if s.Name.startswith(b".text"))
    hay = text.get_data()
    text_va = pe.OPTIONAL_HEADER.ImageBase + text.VirtualAddress
    names = {}
    for label, folder in THIRD_PARTY:
        found = []
        for obj_path in sorted(folder.glob("*.obj")):
            obj = parse_object(obj_path.read_bytes(), obj_path.name)
            for sec in obj.sections:
                syms = obj.symbols_in(sec)
                if not sec.is_code or len(sec.data) < 8 or not syms:
                    continue
                for off in find_all_masked(hay, sec.data, sec.mask()):
                    found.append((obj, sec, text_va + off, text_va + off + syms[0].value, len(sec.data),
                                  syms[0].name))
                    refs.add(obj, sec, text_va + off)
        strong = [f for f in found if f[4] >= STRONG]
        if not strong:
            continue
        lo, hi = min(f[3] for f in strong), max(f[3] + f[4] for f in strong)
        best = refs.winners(found)
        for f in found:
            _, _, _, addr, size, name = f
            if id(f) in best and (size >= STRONG or lo <= addr < hi):
                names.setdefault(addr, f"{label}: {name}")
    return names


def main() -> None:
    argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter).parse_args()
    pe = pefile.PE(str(EXE))
    base = pe.OPTIONAL_HEADER.ImageBase
    text = next(s for s in pe.sections if s.Name.startswith(b".text"))
    text_start = base + text.VirtualAddress
    text_end = text_start + text.Misc_VirtualSize

    funcs = fpo_records(pe)
    refs = References(pe)
    lib = library_names(pe, refs)
    for f in funcs:
        f["name"] = lib.get(f["address"], "")
        f["kind"] = "library" if f["name"] else "game"

    # The linker places library objects after all of Cavedog's objects, so the
    # runtime block starts at the first substantial library match. Everything
    # from there on is library code (including functions too small or too
    # compiler-specific to match); tiny "matches" before it are coincidences.
    runtime_start = min(f["address"] for f in funcs if f["kind"] == "library" and f["size"] >= SUBSTANTIAL)
    for f in funcs:
        if f["address"] >= runtime_start:
            f["kind"] = "library"
        elif f["kind"] == "library":
            f["kind"], f["name"] = "game", ""
    # C++ runtime code found inside the game region (see cpp_library_names).
    cpp = cpp_library_names(pe, refs)
    for f in funcs:
        if f["kind"] == "game" and f["address"] in cpp:
            f["kind"], f["name"] = "library", cpp[f["address"]]
    # Third-party libraries compiled into the game (zlib).
    third = third_party_names(pe, refs)
    for f in funcs:
        if f["kind"] == "game" and f["address"] in third:
            f["kind"], f["name"] = "library", third[f["address"]]
    starts = {f["address"] for f in funcs}

    # Everything in .text not covered by an FPO record (ignoring alignment padding).
    gaps = []
    cursor = text_start
    for f in funcs + [{"address": text_end, "size": 0}]:
        if f["address"] > cursor:
            chunk = pe.get_data(cursor - base, f["address"] - cursor)
            lead = len(chunk) - len(chunk.lstrip(b"\x90\xcc\x00"))
            body = chunk.rstrip(b"\x90\xcc\x00")
            # Keep the high byte of a final `ret N` (c2 NN 00), which the
            # padding strip would otherwise cut off.
            if len(body) >= 2 and body[-2] == 0xC2 and len(body) < len(chunk):
                body = chunk[:len(body) + 1]
            size = len(body) - lead
            if size > 0:
                gaps.append({"address": cursor + lead, "size": size, "kind": "gap", "name": lib.get(cursor + lead, ""),
                             "params": "", "locals": "", "seh": "", "frame_pointer": ""})
        cursor = max(cursor, f["address"] + f["size"])
    for g in gaps:
        if g["address"] >= runtime_start:
            g["kind"] = "library"

    everything = sorted(funcs + gaps, key=lambda f: f["address"])
    kind_at = {f["address"]: f["kind"] for f in everything}

    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    callers = defaultdict(set)
    for f in everything:
        targets = set()
        code = pe.get_data(f["address"] - base, f["size"])
        for ins in md.disasm_lite(code, f["address"]):
            _, _, mnemonic, op = ins
            if mnemonic in ("call", "jmp") and op.startswith("0x"):
                t = int(op, 16)
                if t in starts and not f["address"] <= t < f["address"] + f["size"]:
                    targets.add(t)
        f["_targets"] = targets
        for t in targets:
            callers[t].add(f["address"])
    for f in everything:
        f["calls"] = len(f["_targets"])
        f["game_calls"] = sum(1 for t in f["_targets"] if kind_at.get(t) == "game")
        f["callers"] = len(callers[f["address"]])

    OUT.parent.mkdir(exist_ok=True)
    with OUT.open("w", newline="") as fh:
        w = csv.DictWriter(fh, FIELDS, extrasaction="ignore", lineterminator="\n")
        w.writeheader()
        for f in everything:
            w.writerow({**f, "address": f"{f['address']:#x}"})

    by_kind = defaultdict(lambda: [0, 0])
    for f in everything:
        by_kind[f["kind"]][0] += 1
        by_kind[f["kind"]][1] += f["size"]
    for k, (n, size) in sorted(by_kind.items()):
        print(f"{k:8s} {n:5d} functions  {size:8d} bytes")
    game = [f for f in everything if f["kind"] == "game"]
    leaves = [f for f in game if f["game_calls"] == 0]
    print(f"game functions that call no other game function: {len(leaves)}")
    print(f"wrote {OUT.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
