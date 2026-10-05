"""The game's resources: src/res/TotalA.rc, compiled the way the original's
were, with the icon and cursor it names.

    uv run tools/resources.py                  # compile them, and compare with the original's .rsrc
    uv run tools/resources.py --art DIR        # the icon and cursor from DIR (or BT_ART_DIR=DIR)
    uv run tools/resources.py --extract        # rewrite src/res/TotalA.rc from orig/TotalA.exe

The exe holds an icon (32x32 and 16x16, 16 colours), a cursor and a version
resource. The script is in the repository; the icon and cursor are Cavedog's
art, which it does not hold. They come from a directory given with --art or
the BT_ART_DIR environment variable (CI supplies them that way), or else are
extracted from the player's own orig/TotalA.exe into build/res/art/. With
neither, the build stops and says so. --extract writes the script out of the
exe again, which is how it was made.

The build compiles the script with the toolchain's RC.EXE (5.00.1472) into
build/res/TotalA.res, finding TotalA.ico and TotalA.cur through RC's include
path, and converts that with CVTRES.EXE (5.00.1668, SP3's, the version the
exe's Rich header names) into build/res/TotalA.obj, as LINK does when it is
given a .res file: /MACHINE:IX86 /READONLY. CVTRES lays out the whole
resource directory itself, as .rsrc$01, with the resources' data after it as
.rsrc$02, so tools/place.py places the two sections at the start of .rsrc and
resolves their relocations like any other object's.
"""

import argparse
import os
import struct
import subprocess
import sys
from pathlib import Path

from check import ROOT, winpath

SRC = ROOT / "src/res"
RC = SRC / "TotalA.rc"
OUT = ROOT / "build/res"
EXTRACTED = OUT / "art"
RES = OUT / "TotalA.res"
OBJ = OUT / "TotalA.obj"
EXE = ROOT / "orig/TotalA.exe"
ART = ("TotalA.ico", "TotalA.cur")       # the files the script names

RT_CURSOR, RT_ICON, RT_GROUP_CURSOR, RT_GROUP_ICON, RT_VERSION = 1, 3, 12, 14, 16


# --- the art --------------------------------------------------------------------------

def art_dir(given: Path | None = None) -> Path:
    """The directory holding the icon and cursor: --art, BT_ART_DIR, or the
    files extracted from orig/TotalA.exe."""
    given = given or (Path(os.environ["BT_ART_DIR"]) if os.environ.get("BT_ART_DIR") else None)
    if given is not None:
        given = given if given.is_absolute() else Path.cwd() / given
        missing = [name for name in ART if not (given / name).is_file()]
        if missing:
            raise SystemExit(f"{given}: {', '.join(missing)} missing (the art for src/res/TotalA.rc)")
        return given
    if EXE.exists():
        if not all((EXTRACTED / name).is_file() for name in ART) or any(
                (EXTRACTED / name).stat().st_mtime < EXE.stat().st_mtime for name in ART):
            extract_art(EXE, EXTRACTED)
        return EXTRACTED
    raise SystemExit("the icon and cursor (Cavedog's art) are not in the repository: put your copy of the "
                     "game's TotalA.exe at orig/TotalA.exe, or pass --art DIR (or set BT_ART_DIR) with "
                     f"{' and '.join(ART)} in DIR")


def extract_art(exe: Path, out: Path) -> None:
    """TotalA.ico and TotalA.cur, as files RC reads back to the same resources."""
    import pefile
    items = resources(pefile.PE(str(exe)))
    images = {(t, i): d for t, i, _, d in items if t in (RT_ICON, RT_CURSOR)}
    out.mkdir(parents=True, exist_ok=True)
    for kind, _, _, data in items:
        if kind == RT_GROUP_ICON:
            (out / "TotalA.ico").write_bytes(group_file(data, {i: d for (t, i), d in images.items()
                                                               if t == RT_ICON}, False))
        elif kind == RT_GROUP_CURSOR:
            (out / "TotalA.cur").write_bytes(group_file(data, {i: d for (t, i), d in images.items()
                                                               if t == RT_CURSOR}, True))


# --- compiling ----------------------------------------------------------------------

def build(art: Path | None = None) -> Path:
    """Compile src/res/TotalA.rc and the art into build/res/TotalA.obj (only
    when one of them changed)."""
    art = art_dir(art)
    sources = [RC] + [art / name for name in ART]
    stamp = OUT / "art.path"
    if (OBJ.exists() and stamp.exists() and stamp.read_text() == str(art)
            and all(s.stat().st_mtime <= OBJ.stat().st_mtime for s in sources)):
        return OBJ
    OUT.mkdir(parents=True, exist_ok=True)
    for path in (RES, OBJ, stamp):
        path.unlink(missing_ok=True)
    # /x leaves out the INCLUDE path, /i adds the art's directory.
    run(["tools/wrc", "/r", "/x", f"/i{winpath(art)}", f"/fo{winpath(RES)}", winpath(RC)], RES)
    run(["tools/wcvtres", "/MACHINE:IX86", "/READONLY", f"/OUT:{winpath(OBJ)}", winpath(RES)], OBJ)
    stamp.write_text(str(art))
    return OBJ


def run(cmd: list[str], out: Path) -> None:
    cmd[0] = str(ROOT / cmd[0])
    proc = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT)
    if proc.returncode != 0 or not out.exists():
        raise SystemExit(f"{' '.join(cmd)} failed:\n{(proc.stdout + proc.stderr).replace(chr(13), '')}")


# --- extracting ---------------------------------------------------------------------

def resources(pe) -> list[tuple[int, int, int, bytes]]:
    """(type, id, language, data) for every resource, in the order of their data."""
    image = pe.get_memory_mapped_image()
    out = []
    for t in pe.DIRECTORY_ENTRY_RESOURCE.entries:
        for n in t.directory.entries:
            for lang in n.directory.entries:
                d = lang.data.struct
                if t.name or n.name:
                    raise SystemExit("a named resource: extend tools/resources.py")
                out.append((d.OffsetToData, t.id, n.id, lang.id, image[d.OffsetToData:d.OffsetToData + d.Size]))
    return [r[1:] for r in sorted(out)]


def group_file(group: bytes, images: dict[int, bytes], cursor: bool) -> bytes:
    """An .ico or .cur file from a group resource and its images: the group's
    entries with each image's offset in the file in place of its id. A cursor
    image starts with its hot spot, which the file keeps in its entry."""
    _, kind, count = struct.unpack_from("<HHH", group, 0)
    head = struct.pack("<HHH", 0, kind, count)
    body = bytearray()
    entries = bytearray()
    offset = 6 + 16 * count
    for i in range(count):
        e = group[6 + 14 * i: 20 + 14 * i]
        (ident,) = struct.unpack_from("<H", e, 12)
        data = images[ident]
        if cursor:
            # The group gives a cursor's width and height as words, the height
            # doubled (the image and its mask); the file, as bytes.
            width, height, _, _ = struct.unpack_from("<HHHH", e, 0)
            hx, hy = struct.unpack_from("<HH", data, 0)
            data = data[4:]
            entries += struct.pack("<BBBBHHII", width, height // 2, 0, 0, hx, hy, len(data), offset + len(body))
        else:
            entries += e[:8] + struct.pack("<II", len(data), offset + len(body))
        body += data
    return head + bytes(entries) + bytes(body)


def rc_string(text: str) -> str:
    """A string literal of the script, which is plain ASCII: any other
    character is an octal escape of its byte in code page 1252 (the script's
    #pragma code_page), as RC reads it."""
    text = text.replace("\\", "\\\\").replace('"', '""')
    return '"' + "".join(c if ord(c) < 0x80 else f"\\{c.encode('cp1252')[0]:03o}" for c in text) + '"'


def version_block(data: bytes) -> list[str]:
    """A VERSIONINFO statement from a VS_VERSIONINFO resource."""
    def node(at: int) -> tuple[int, int, int, str, int]:
        length, value_len, kind = struct.unpack_from("<HHH", data, at)
        end = at + 6
        while data[end:end + 2] != b"\0\0":
            end += 2
        key = data[at + 6:end].decode("utf-16-le")
        value_at = (end + 2 + 3) & ~3
        return length, value_len, kind, key, value_at

    length, value_len, _, key, at = node(0)
    assert key == "VS_VERSION_INFO" and value_len == 52
    fixed = struct.unpack_from("<13I", data, at)
    assert fixed[0] == 0xFEEF04BD
    ver = lambda ms, ls: f"{ms >> 16},{ms & 0xFFFF},{ls >> 16},{ls & 0xFFFF}"
    lines = [
        "VS_VERSION_INFO VERSIONINFO",
        f" FILEVERSION {ver(fixed[2], fixed[3])}",
        f" PRODUCTVERSION {ver(fixed[4], fixed[5])}",
        f" FILEFLAGSMASK {fixed[6]:#x}L",
        f" FILEFLAGS {fixed[7]:#x}L",
        f" FILEOS {fixed[8]:#x}L",
        f" FILETYPE {fixed[9]:#x}L",
        f" FILESUBTYPE {fixed[10]:#x}L",
        "BEGIN",
    ]

    def children(at: int, end: int, depth: int) -> None:
        pad = "    " * depth
        while at < end:
            length, value_len, kind, key, value_at = node(at)
            if key in ("StringFileInfo", "VarFileInfo") or (depth == 2 and value_len == 0 and length > value_at - at):
                lines.append(f"{pad}BLOCK {rc_string(key)}")
                lines.append(f"{pad}BEGIN")
                children(value_at, at + length, depth + 1)
                lines.append(f"{pad}END")
            elif kind == 1:
                # A string, its length in characters with the terminator, which
                # the script spells out as Developer Studio's scripts do.
                text = data[value_at:value_at + 2 * value_len].decode("utf-16-le")
                assert text.endswith("\0")
                lines.append(f"{pad}VALUE {rc_string(key)}, {rc_string(text[:-1])[:-1]}\\0\"")
            else:
                words = struct.unpack_from(f"<{value_len // 2}H", data, value_at)
                lines.append(f"{pad}VALUE {rc_string(key)}, " + ", ".join(f"{w:#x}" for w in words))
            at = (at + length + 3) & ~3

    children((at + value_len + 3) & ~3, length, 1)
    lines.append("END")
    return lines


def extract(exe: Path) -> None:
    """Write src/res/TotalA.rc from the exe's resources."""
    import pefile
    items = resources(pefile.PE(str(exe)))
    langs = {lang for *_, lang, _ in items}
    if langs != {0x409}:
        raise SystemExit(f"languages {sorted(langs)}: extend tools/resources.py")
    SRC.mkdir(parents=True, exist_ok=True)
    lines = [
        "// The resources of TotalA.exe: its icon, its cursor and its version.",
        "// Written by tools/resources.py --extract; compiled with RC.EXE and",
        "// CVTRES.EXE by the same tool (docs/linking.md). The icon and cursor are",
        "// Cavedog's art, which the repository does not hold: RC finds them on its",
        "// include path, in the directory tools/resources.py gives it.",
        "",
        "LANGUAGE 0x09, 0x01    // LANG_ENGLISH, SUBLANG_ENGLISH_US",
        "#pragma code_page(1252)",
        "",
    ]
    for kind, ident, _, data in items:
        if kind == RT_GROUP_ICON:
            lines.append(f"{ident} ICON DISCARDABLE {rc_string(ART[0])}")
        elif kind == RT_GROUP_CURSOR:
            lines.append(f"{ident} CURSOR DISCARDABLE {rc_string(ART[1])}")
        elif kind == RT_VERSION:
            assert ident == 1
            lines += ["", "#define VS_VERSION_INFO 1", ""] + version_block(data)
        elif kind not in (RT_ICON, RT_CURSOR):
            raise SystemExit(f"resource type {kind}: extend tools/resources.py")
    RC.write_text("\n".join(lines) + "\n", encoding="ascii")
    print(f"wrote {RC.relative_to(ROOT)}")


# --- comparing ----------------------------------------------------------------------

def compare(exe: Path) -> int:
    """Lay the compiled sections out as LINK would at the original's .rsrc and
    count the bytes that differ."""
    import pefile
    from place import parse
    pe = pefile.PE(str(exe))
    sec = next(s for s in pe.sections if s.Name.rstrip(b"\0") == b".rsrc")
    want = sec.get_data()[:sec.Misc_VirtualSize]
    obj = parse(OBJ)
    out = bytearray()
    starts = {}
    for s in sorted((s for s in obj.secs if s.name.startswith(".rsrc$")), key=lambda s: s.name):
        n = (s.chars >> 20) & 0xF
        align = 1 << (n - 1) if n else 16
        out += bytes(-len(out) % align)
        starts[s.index] = len(out)
        out += s.data
    for s in obj.secs:
        for off, symidx, rtype in s.relocs:
            sym = obj.syms[symidx]
            at = starts[s.index] + off
            (addend,) = struct.unpack_from("<I", out, at)
            struct.pack_into("<I", out, at, sec.VirtualAddress + starts[sym.section] + sym.value + addend)
    diff = sum(a != b for a, b in zip(out, want)) + abs(len(out) - len(want))
    print(f"{OBJ.relative_to(ROOT)}: {len(out):,} bytes of .rsrc, the original's {len(want):,}; {diff:,} differ")
    return diff


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--art", type=Path, help=f"the directory with {' and '.join(ART)} (default: BT_ART_DIR, "
                                             "or extracted from orig/TotalA.exe)")
    ap.add_argument("--extract", action="store_true", help="rewrite src/res/TotalA.rc from orig/TotalA.exe")
    args = ap.parse_args()
    if args.extract:
        extract(EXE)
    obj = build(args.art)
    if EXE.exists():
        if compare(EXE):
            sys.exit(1)
    else:
        print(f"{obj.relative_to(ROOT)} built; no orig/TotalA.exe to compare it with")


if __name__ == "__main__":
    main()
