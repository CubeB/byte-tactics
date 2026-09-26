"""Build data/functions.csv: every function in the original exe.

Sources:
  - FPO debug records: start, size, argument/local counts, frame info
  - the VC5 SP3 runtime library (LIBCMT): which functions are library code
  - direct calls found by disassembly: the call graph

kind is one of:
  game     Cavedog code, to be decompiled
  library  statically linked runtime code, matched byte-for-byte already
  gap      code between FPO records (hand-written assembly, thunks, or data)
"""

import csv
import struct
from collections import defaultdict
from pathlib import Path

import capstone
import pefile

from coff import read_archive
from crtmatch import MIN_SIZE, find_masked

ROOT = Path(__file__).resolve().parent.parent
EXE = ROOT / "orig/TotalA.exe"
RUNTIME_LIB = ROOT / "toolchain/msvc5-sp3/LIB/LIBCMT.LIB"
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


def library_names(pe: pefile.PE) -> dict[int, str]:
    """Map exe address -> runtime library symbol for every matched library function."""
    text = next(s for s in pe.sections if s.Name.startswith(b".text"))
    hay = text.get_data()
    text_va = pe.OPTIONAL_HEADER.ImageBase + text.VirtualAddress
    names = {}
    for obj in read_archive(RUNTIME_LIB):
        for sec in obj.sections:
            if not sec.is_code or len(sec.data) < MIN_SIZE:
                continue
            off = find_masked(hay, sec.data, sec.mask())
            if off < 0:
                continue
            syms = obj.symbols_in(sec)
            for s in syms:
                names.setdefault(text_va + off + s.value, s.name)
            if not syms:
                names.setdefault(text_va + off, f"{obj.name.split(chr(92))[-1]}:{sec.name}")
    return names


def main() -> None:
    pe = pefile.PE(str(EXE))
    base = pe.OPTIONAL_HEADER.ImageBase
    text = next(s for s in pe.sections if s.Name.startswith(b".text"))
    text_start = base + text.VirtualAddress
    text_end = text_start + text.Misc_VirtualSize

    funcs = fpo_records(pe)
    lib = library_names(pe)
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
    starts = {f["address"] for f in funcs}

    # Everything in .text not covered by an FPO record (ignoring alignment padding).
    gaps = []
    cursor = text_start
    for f in funcs + [{"address": text_end, "size": 0}]:
        if f["address"] > cursor:
            chunk = pe.get_data(cursor - base, f["address"] - cursor)
            lead = len(chunk) - len(chunk.lstrip(b"\x90\xcc\x00"))
            size = len(chunk.rstrip(b"\x90\xcc\x00")) - lead
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
