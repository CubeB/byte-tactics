"""Compare an ordinary link with the original: does every reference reach
what the original's does?

    uv run tools/link.py --carve --map   # build/link/TotalA.exe and its map
    uv run tools/linkcmp.py              # compare it with orig/TotalA.exe
    uv run tools/linkcmp.py --verbose    # list every difference

In an ordinary link every function sits where LINK puts it, so comparing
addresses with the original says little. What must agree is where each
reference leads. For every game function tools/place.py places, this reads
each relocated field in the linked exe, maps the address it holds back to the
original through the map file (a public symbol of the link covering it, and
that symbol's address in the original), and compares the result with the
address the original's code holds in the same field. A difference means the
link bound a name to something other than what the original calls or uses: a
wrong alias, a copy kept so that it inlines, a stub, or a second copy of a
global.

References to a function's own constants and string literals are skipped
(each object keeps its own copies, with the same contents), and so are the
import slots (LINK builds its own table). Calls to std::_Lockit, which the
original links twice (data/aliases.csv), reach one copy instead of two.
"""

import argparse
import bisect
import re
import struct
from collections import Counter
from pathlib import Path

import pefile

from carve import Namer, linker_tables, static_alias
from check import ROOT, load_symbols
from linkcheck import address_of
from place import CODE, FUNCTIONS, GAPCODE, LIBDATA, REL_DIR32, REL_REL32, layout, load_rows, parse

LINKED = ROOT / "build/link/TotalA.exe"
SCN_MEM_WRITE = 0x80000000


def read_map(path: Path) -> dict[str, int]:
    """Public symbol -> address, from a LINK map file."""
    out: dict[str, int] = {}
    for line in path.read_text(errors="replace").splitlines():
        m = re.match(r"\s*[0-9a-f]{4}:[0-9a-f]{8}\s+(\S+)\s+([0-9a-f]{8})", line)
        if m:
            out.setdefault(m.group(1), int(m.group(2), 16))
    return out


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--exe", type=Path, default=LINKED)
    ap.add_argument("--verbose", "-v", action="store_true")
    args = ap.parse_args()
    map_path = args.exe.with_suffix(".map")
    if not map_path.exists():
        raise SystemExit(f"{map_path.relative_to(ROOT)} is missing: link with --map")

    img, placer = layout()
    namer = Namer(img, placer)
    symbols = load_symbols()
    linked = pefile.PE(str(args.exe))
    base = linked.OPTIONAL_HEADER.ImageBase
    image = linked.get_memory_mapped_image()
    link_at = read_map(map_path)

    # Where each public of the link sits in the original, and how far it runs.
    sizes = {int(r["address"], 16): int(r["size"]) for r in load_rows(FUNCTIONS)}
    orig_of: dict[str, tuple[int, int]] = {}
    for start, end, name in namer.spans + namer.data_spans:
        orig_of.setdefault(name, (start, end - start))
    # origdata.obj's runs of the original's data, one section each.
    runs = {}
    carved = ROOT / "build/link/origdata.obj"
    if carved.exists():
        obj = parse(carved)
        for name, sym in obj.externals.items():
            if name.startswith("__orig_"):
                runs[name] = (int(name[len("__orig_"):], 16), len(obj.secs[sym.section - 1].data))
    for name in link_at:
        if name in orig_of:
            continue
        if name in runs:
            orig_of[name] = runs[name]
        elif name.startswith("__static_"):
            # A static given a public name: the placed piece at its address.
            a = int(name[len("__static_"):], 16)
            pid = img.owner[a - img.base]
            p = placer.pieces[pid - 1] if pid else None
            orig_of[name] = (a, p.hi - p.lo if p else 4)
        else:
            a = address_of(name, symbols)
            if a is not None:
                orig_of[name] = (a, sizes.get(a, 4))
    pairs = sorted((link_at[n], n) for n in orig_of if n in link_at)
    link_starts = [a for a, _ in pairs]

    def translate(x: int) -> tuple[int | None, str]:
        i = bisect.bisect_right(link_starts, x) - 1
        while i >= 0 and x - pairs[i][0] < 0x40000:
            la, name = pairs[i]
            o, size = orig_of[name]
            if x - la < max(size, 1):
                return o + (x - la), name
            i -= 1
        return None, ""

    copies = {"??0_Lockit@std@@QAE@XZ", "??1_Lockit@std@@QAE@XZ"}
    stats = Counter()
    lines = []
    for p in placer.pieces:
        if p.source not in (CODE, GAPCODE) or p.obj.library:
            continue
        names = [s.name for s in p.obj.syms.values()
                 if s.section == p.sec.index and s.sclass == 2 and s.value == p.lo] + [static_alias(p.va)]
        at = next((link_at[n] for n in names if n in link_at), None)
        if at is None and p.sec.name.startswith(".text$x"):
            stats["exception handler stubs (static, not compared)"] += 1
            continue
        if at is None:
            stats["functions with no public name in the link"] += 1
            if args.verbose:
                lines.append(f"  {p.label}: no public name in the link")
            continue
        stats["functions compared"] += 1
        for off, symidx, rtype in p.sec.relocs:
            if not p.lo <= off <= p.hi - 4 or rtype not in (REL_DIR32, REL_REL32):
                continue
            sym = p.obj.syms[symidx]
            if sym.section > 0:
                target = p.obj.secs[sym.section - 1]
                if target is p.sec or (not target.is_code and not target.chars & SCN_MEM_WRITE
                                       and not sym.name.startswith("??_7")):
                    # Jump tables, its own constants and literals. A vtable
                    # its own object defines is compared: another file's copy
                    # of it may be the one LINK keeps.
                    continue
                if target.name.startswith(".text$x"):
                    continue                 # its own exception handler stub (a static label)
                if target.name.startswith(".data") and sym.name.startswith("??_C@"):
                    continue
            if sym.name.startswith("__imp_"):
                continue
            site_o, site_l = p.va + off - p.lo, at + off - p.lo
            want = img.u32(site_o)
            got = struct.unpack_from("<I", image, site_l - base)[0]
            if rtype == REL_REL32:
                want = (site_o + 4 + want) & 0xFFFFFFFF
                got = (site_l + 4 + got) & 0xFFFFFFFF
            if rtype == REL_DIR32 and got == want and got < 0x10000:
                # An absolute symbol, not an address: the offsets into the
                # thread information block __except_list (0) and __tls_array.
                stats["absolute values that agree"] += 1
                continue
            mapped, via = translate(got)
            if mapped is None and sym.section > 0 and sym.sclass == 3 and rtype == REL_DIR32:
                # Its own static, which the layout placed where the original's is.
                (ours,) = struct.unpack_from("<i", p.sec.data, off)
                at_ = sym.value + (ours if sym.name.startswith(".") else 0)
                own = placer.address_in(p.obj, sym.section, at_)
                if own is not None and own + (0 if sym.name.startswith(".") else ours) == want:
                    stats["references to the function's own statics"] += 1
                    continue
            if mapped != want and rtype == REL_DIR32:
                # An address just past (or before) the global it is computed
                # from, an array's end: what matters is that the global agrees.
                (ours,) = struct.unpack_from("<i", p.sec.data, off)
                if ours and translate((got - ours) & 0xFFFFFFFF)[0] == (want - ours) & 0xFFFFFFFF:
                    stats["references past the end of a global that agrees"] += 1
                    continue
            if mapped is None:
                stats["references the map cannot place"] += 1
                if args.verbose:
                    lines.append(f"  {p.label} +{off - p.lo:#x}: {sym.name} -> {got:#x}, not in the map")
                continue
            if mapped == want:
                stats["references that agree"] += 1
            elif sym.name in copies:
                stats["references to the other copy of std::_Lockit"] += 1
            else:
                stats["references that differ"] += 1
                lines.append(f"  {p.label} +{off - p.lo:#x}: {sym.name} -> {got:#x} = {mapped:#x} in the "
                             f"original ({via}), where the original's holds {want:#x}")
    for k, v in sorted(stats.items()):
        print(f"{k}: {v:,}")
    differ = [l for l in lines if "where the original's holds" in l]
    if lines:
        print("\n".join(lines if args.verbose else differ[:30]))

    # The data: every named piece of data in the link (a global from source,
    # a literal, a vtable, library data, a run of origdata.obj) against the
    # original's bytes at its address, pointer fields by where they lead.
    data_lo = namer.sections[".rdata"][0]
    data_hi = namer.sections[".data"][0] + namer.sections[".data"][1]
    tables = bytearray(img.size)            # what LINK builds itself: not compared
    for va, n in linker_tables(img):
        tables[va - img.base:va - img.base + n] = b"\1" * n

    def text(va: int) -> bytes | None:
        """The C string the original holds at va, if it is one."""
        if not data_lo <= va < data_hi:
            return None
        raw = bytes(img.pristine[va - img.base: va - img.base + 512])
        return raw.split(b"\0", 1)[0] if b"\0" in raw else None

    def linked_text(va: int) -> bytes | None:
        """The C string the link holds at va."""
        if not base <= va < base + len(image):
            return None
        raw = bytes(image[va - base: va - base + 512])
        return raw.split(b"\0", 1)[0] if b"\0" in raw else None

    def library(va: int) -> bool:
        """Whether the original's data at va is the runtime library's (whose
        vtables and throw information lead to its own functions where the
        original's lead to the copies Cavedog's objects instantiated)."""
        return img.src[va - img.base] == LIBDATA

    dstats = Counter()
    bad, notes = [], []
    for name, (start, size) in sorted(orig_of.items(), key=lambda kv: kv[1]):
        if name not in link_at or not data_lo <= start < data_hi or tables[start - img.base]:
            continue
        la = link_at[name]
        if not linked.get_section_by_rva(la - base):
            continue
        ours = image[la - base: la - base + size]
        theirs = bytes(img.pristine[start - img.base: start - img.base + size])
        if len(ours) < size:
            ours = ours.ljust(size, b"\0")
        dstats["pieces of data compared"] += 1
        i, wrong = 0, []
        while i < size:
            if tables[start - img.base + i]:
                i += 1
                continue
            if ours[i] == theirs[i]:
                if i % 4 == 0 and i + 4 <= size and ours[i:i + 4] == theirs[i:i + 4]:
                    # The same bytes, but an address of the original's code is
                    # not one in this image: a pointer left as a number.
                    v = struct.unpack_from("<I", ours, i)[0]
                    if (name.startswith("__orig_") and v in sizes and v not in img.patches
                            and translate(v)[0] != v):
                        dstats["raw addresses of functions in data"] += 1
                        bad.append(f"  {name} ({start:#x}) +{i:#x}: holds {v:#x}, the original's address of a "
                                   f"function, as a number")
                i += 1
                continue
            for j in range(max(0, i - 3), i + 1):
                if j + 4 > size:
                    continue
                mine, want = struct.unpack_from("<I", ours, j)[0], struct.unpack_from("<I", theirs, j)[0]
                mapped = translate(mine)[0]
                if mapped == want:
                    dstats["pointer fields that agree"] += 1
                    i = j + 4
                    break
                if text(want) is not None and linked_text(mine) == text(want):
                    # The same string at another address: the link folded the
                    # literal into another object's identical one, or keeps
                    # one of its own where the original shares a global's.
                    dstats["pointer fields that reach an identical string"] += 1
                    i = j + 4
                    break
            else:
                wrong.append(i)
                i += 1
        if wrong:
            line = (f"  {name} ({start:#x}, {size} bytes): {len(wrong)} byte(s) differ, first at +{wrong[0]:#x} "
                    f"(ours {ours[wrong[0]:wrong[0] + 4].hex()}, the original's {theirs[wrong[0]:wrong[0] + 4].hex()})")
            if library(start) or name.startswith(("__CT", "__TI")):
                dstats["pieces of the library's data and exception tables that differ"] += 1
                notes.append(line)
            else:
                dstats["pieces of data that differ"] += 1
                bad.append(line)
    # A stand-in the link defines for a global the source names by address
    # (link/data.cpp) must not lie inside a piece another file defines. The
    # original holds them as one thing (a field of a file static, like the
    # std::vector at 0x5122c0 whose _First and _Last the source names as
    # globals of their own), and apart they are two variables: the code writes
    # one and reads the other, and a null insert position crashed the battle
    # room. Two pieces of one file are not this (an int placed in an array's
    # padding), so only pieces of link/data.obj are looked at.
    standins, held = [], []
    for row in (ROOT / "data/layout.csv").read_text().splitlines():
        cols = row.split(",")
        if cols[0] == "piece" and len(cols) >= 7:
            piece = (int(cols[1], 16), int(cols[2]), cols[3])
            if cols[3] == "link/data.obj":
                standins.append(piece)
            elif cols[3].startswith("src/"):
                held.append(piece)
    for start, size, _ in standins:
        for lo, n, file in held:
            if lo < start < lo + n:
                dstats["stand-ins inside a piece another file defines"] += 1
                bad.append(f"  {start:#x}: link/data.cpp defines it ({size} bytes), but {file} places it inside "
                           f"the piece at {lo:#x}: {start - lo:#x} bytes into it, so the link has two variables")
    for k, v in sorted(dstats.items()):
        print(f"{k}: {v:,}")
    print("\n".join(bad if args.verbose else bad[:30]))
    if args.verbose and notes:
        print("the library's data and exception tables (not counted as differences):")
        print("\n".join(notes))
    raise SystemExit(1 if differ or bad else 0)


if __name__ == "__main__":
    main()
