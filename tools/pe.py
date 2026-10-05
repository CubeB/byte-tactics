"""The PE headers and debug data of TotalA.exe, generated the way LINK 5.10
wrote them, from the placed sections and the link's settings (link/link.toml).

tools/place.py lays the sections out; this module writes what LINK adds
around them:

  * the MS-DOS stub: LINK's default, which LINK.EXE holds as a template;
  * the Rich header after it: one entry per @comp.id among the linked
    objects, with how many objects carry it, XORed with a key that is the
    stub's size plus each entry rotated left by its count (LINK 5.10's sum
    leaves out the stub's own bytes, which later linkers add in);
  * the PE file header, the optional header with its data directories, and
    the section table;
  * the debug directory (in .rdata) and the three records it lists, which
    follow the last section in the file: MISC (the exe's name as /OUT gave
    it), FPO (every linked object's frame pointer omission records from its
    .debug$F, sorted by address) and CodeView (NB10: the program database's
    name, signature and age).

Nothing here reads the original exe.
"""

import datetime
import struct
import tomllib
from dataclasses import dataclass
from pathlib import Path

from check import ROOT

SETTINGS = ROOT / "link/link.toml"
LINK = ROOT / "toolchain/msvc5-sp3/BIN/LINK.EXE"
LINKER_VERSION = (5, 10)             # LINK.EXE 5.10.7303, the toolchain's (and the exe's)

IMAGE_FILE_RELOCS_STRIPPED = 0x0001
IMAGE_FILE_EXECUTABLE_IMAGE = 0x0002
IMAGE_FILE_LINE_NUMS_STRIPPED = 0x0004
IMAGE_FILE_LOCAL_SYMS_STRIPPED = 0x0008
IMAGE_FILE_32BIT_MACHINE = 0x0100
SUBSYSTEMS = {"windows": 2, "console": 3}
SCN_CODE, SCN_INIT, SCN_UNINIT = 0x20, 0x40, 0x80
DEBUG_TYPE_CODEVIEW, DEBUG_TYPE_FPO, DEBUG_TYPE_MISC = 2, 3, 4
DIR_IMPORT, DIR_RESOURCE, DIR_DEBUG, DIR_TLS, DIR_IAT = 1, 2, 6, 9, 12
DEBUG_ENTRY = 28                     # sizeof(IMAGE_DEBUG_DIRECTORY)
MISC_SIZE = 12 + 260                 # IMAGE_DEBUG_MISC with a MAX_PATH name


def settings() -> dict:
    with SETTINGS.open("rb") as fh:
        s = tomllib.load(fh)
    stamp = s["timestamp"]
    s["time"] = int(stamp.astimezone(datetime.timezone.utc).timestamp())
    return s


def align(n: int, a: int) -> int:
    return (n + a - 1) & ~(a - 1)


def rol(v: int, n: int) -> int:
    n &= 31
    return ((v << n) | (v >> (32 - n))) & 0xFFFFFFFF


@dataclass
class Section:
    name: str
    rva: int
    vsize: int
    raw: int             # SizeOfRawData: the initialised part, aligned to the file alignment
    chars: int
    pointer: int = 0     # PointerToRawData, set by layout_file


# --- the stub -------------------------------------------------------------------------

def dos_stub() -> bytes:
    """LINK's default MS-DOS header and stub program, the template LINK.EXE
    copies into every image it makes (its e_lfanew is left 0)."""
    data = LINK.read_bytes()
    at = 0
    while True:
        at = data.find(b"MZ\x90\x00\x03\x00", at + 1)
        if at < 0:
            raise SystemExit(f"{LINK.name}: no MS-DOS stub template")
        stub = data[at:at + 0x80]
        if b"This program cannot be run in DOS mode" in stub and stub[0x3C:0x40] == b"\0\0\0\0":
            return stub


def rich_header(entries: list[tuple[int, int]]) -> bytes:
    """The Rich header for (@comp.id, count) entries, in the order LINK met
    them, starting at offset 0x80 (after the stub)."""
    key = 0x80
    for compid, count in entries:
        key = (key + rol(compid, count)) & 0xFFFFFFFF
    words = [0x536E6144, 0, 0, 0] + [w for e in entries for w in e]     # "DanS"
    return b"".join(struct.pack("<I", w ^ key) for w in words) + b"Rich" + struct.pack("<I", key)


# --- the image ------------------------------------------------------------------------

@dataclass
class Image:
    sections: list[Section]
    entry: int                        # RVA
    directories: dict[int, tuple[int, int]]
    fpo: list[bytes]                  # 16-byte FPO_DATA records, sorted by address
    rich: list[tuple[int, int]]


def debug_records(s: dict, fpo: list[bytes]) -> list[tuple[int, bytes]]:
    """(type, data) of the debug records LINK writes after the sections."""
    name = s["out"].encode("latin-1")
    misc = struct.pack("<IIB3x", 1, MISC_SIZE, 0) + name + bytes(260 - len(name))
    cv = b"NB10" + struct.pack("<III", 0, s["time"], s["pdb_age"]) + s["pdb"].encode("latin-1") + b"\0"
    return [(DEBUG_TYPE_MISC, misc), (DEBUG_TYPE_FPO, b"".join(fpo)), (DEBUG_TYPE_CODEVIEW, cv)]


def debug_directory(s: dict, records: list[tuple[int, bytes]], pointer: int) -> bytes:
    """The debug directory: one entry per record, the records from `pointer`
    in the file on, none of them mapped into memory."""
    out = bytearray()
    for kind, data in records:
        out += struct.pack("<IIHHIIII", 0, s["time"], 0, 0, kind, len(data), 0, pointer)
        pointer += len(data)
    return bytes(out)


def headers(s: dict, img: Image) -> bytes:
    """The stub, the Rich header and the PE headers, padded to SizeOfHeaders."""
    file_align, sect_align = s["file_alignment"], s["section_alignment"]
    rich = rich_header(img.rich)
    lfanew = align(0x80 + len(rich) + 8, 16)
    stub = bytearray(dos_stub())
    struct.pack_into("<I", stub, 0x3C, lfanew)
    size_of_headers = align(lfanew + 4 + 20 + 0xE0 + 40 * len(img.sections), file_align)

    chars = (IMAGE_FILE_RELOCS_STRIPPED | IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_LOCAL_SYMS_STRIPPED
             | IMAGE_FILE_32BIT_MACHINE)
    # With /DEBUG LINK keeps the line numbers' flag clear.
    if not s.get("pdb"):
        chars |= IMAGE_FILE_LINE_NUMS_STRIPPED
    file_header = struct.pack("<HHIIIHH", 0x14C, len(img.sections), s["time"], 0, 0, 0xE0, chars)

    code = [x for x in img.sections if x.chars & SCN_CODE]
    data = [x for x in img.sections if not x.chars & SCN_CODE]
    size = lambda xs, flag: sum(align(x.vsize, file_align) for x in xs if x.chars & flag)
    last = img.sections[-1]
    os_major, os_minor = map(int, s["os_version"].split("."))
    sub_major, sub_minor = map(int, s["subsystem_version"].split("."))
    optional = struct.pack(
        "<HBBIIIIIIIIIHHHHHHIIIIHHIIIIII",
        0x10B, *LINKER_VERSION,
        size(code, SCN_CODE), size(img.sections, SCN_INIT), size(img.sections, SCN_UNINIT),
        img.entry, code[0].rva, data[0].rva, s["image_base"], sect_align, file_align,
        os_major, os_minor, 0, 0, sub_major, sub_minor, 0,
        align(last.rva + last.vsize, sect_align), size_of_headers, 0, SUBSYSTEMS[s["subsystem"]], 0,
        s["stack_reserve"], s["stack_commit"], s["heap_reserve"], s["heap_commit"], 0, 16)
    for i in range(16):
        optional += struct.pack("<II", *img.directories.get(i, (0, 0)))

    table = bytearray()
    for x in img.sections:
        table += x.name.encode().ljust(8, b"\0") + struct.pack(
            "<IIIIIIHHI", x.vsize, x.rva, x.raw, x.pointer if x.raw else 0, 0, 0, 0, 0, x.chars)
    out = bytes(stub) + rich + bytes(lfanew - 0x80 - len(rich)) + b"PE\0\0" + file_header + optional + table
    return out + bytes(size_of_headers - len(out))


def layout_file(s: dict, img: Image) -> int:
    """Give each section its place in the file; returns where the debug
    records begin (after the last section's raw data)."""
    file_align = s["file_alignment"]
    rich = rich_header(img.rich)
    lfanew = align(0x80 + len(rich) + 8, 16)
    pointer = align(lfanew + 4 + 20 + 0xE0 + 40 * len(img.sections), file_align)
    for x in img.sections:
        x.pointer = pointer
        pointer += x.raw
    return pointer


def write(path: Path, s: dict, img: Image, contents: dict[str, bytes], records: list[tuple[int, bytes]]) -> bytes:
    """The whole file: headers, each section's raw data, the debug records,
    and zeros up to the file alignment."""
    out = bytearray(headers(s, img))
    for x in img.sections:
        assert len(out) == x.pointer or not x.raw, (x.name, hex(len(out)), hex(x.pointer))
        out += contents[x.name][:x.raw].ljust(x.raw, b"\0")
    for _, data in records:
        out += data
    out += bytes(align(len(out), s["file_alignment"]) - len(out))
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(bytes(out))
    return bytes(out)
