#!/usr/bin/env bash
# Download and unpack Visual C++ 5.0 Professional plus Service Pack 3 into toolchain/,
# and copy the original TotalA.exe from the Steam install into orig/.
# Needs: curl, 7z, cabextract, wine.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TC="$ROOT/toolchain"
# Downloads live in a cache shared by all worktrees, so the ~880 MB of deps is
# fetched once. Override with BT_DL_CACHE.
DL="${BT_DL_CACHE:-${XDG_CACHE_HOME:-$HOME/.cache}/byte-tactics}"
STEAM_TA="${STEAM_TA:-$HOME/.local/share/Steam/steamapps/common/Total Annihilation}"
mkdir -p "$DL" "$ROOT/orig"

fetch() {  # url dest md5
    if [ ! -f "$2" ]; then curl -fSL -o "$2.part" "$1" && mv "$2.part" "$2"; fi
    echo "$3  $2" | md5sum -c --quiet
}

# RTM: the VC directories sit uncompressed on the CD.
if [ ! -d "$TC/msvc5-rtm" ]; then
    fetch "https://winworldpc.com/download/3a575cc3-84e2-809c-c383-11c3a6e28094/from/c39ac2af-c381-c2bf-1b25-11c3a4e284a2" \
        "$DL/vc5pro.7z" 106c769210e8b5c4e5be97e86bf4abae
    tmp="$(mktemp -d -p "$TC")"
    7z x -y -bso0 -bsp0 "$DL/vc5pro.7z" -o"$tmp"
    7z x -y -bso0 -bsp0 "$tmp"/*/VCPP-5.00.iso -o"$tmp/iso" \
        'DEVSTUDIO/VC/BIN/*' 'DEVSTUDIO/VC/INCLUDE/*' 'DEVSTUDIO/VC/LIB/*' 'DEVSTUDIO/SHAREDIDE/BIN/*' \
        'DEVSTUDIO/VC/REDIST/MSVCP50.DLL' -r
    mkdir -p "$TC/msvc5-rtm"
    mv "$tmp/iso/DEVSTUDIO/VC/"{BIN,INCLUDE,LIB} "$TC/msvc5-rtm/"
    mv "$tmp/iso/DEVSTUDIO/SHAREDIDE/BIN/"{*.DLL,RC.EXE} "$TC/msvc5-rtm/BIN/"
    # LINK.EXE loads MSDIS100.DLL, which needs MSVCP50.DLL; the compiler does not
    # use it, so it only lives under REDIST on the CD, not in BIN.
    cp "$tmp/iso/DEVSTUDIO/VC/REDIST/MSVCP50.DLL" "$TC/msvc5-rtm/BIN/"
    rm -rf "$tmp"
fi

# The resource compiler (tools/resources.py): RC.EXE is a front end to the
# RCDLL.DLL above, and sits beside it in SHAREDIDE/BIN on the CD, not in
# VC/BIN. The service pack updates neither. Toolchains set up before the
# resources were built lack it, so it is fetched on its own.
for v in msvc5-rtm msvc5-sp3; do
    if [ -d "$TC/$v" ] && [ ! -f "$TC/$v/BIN/RC.EXE" ]; then
        if [ ! -f "$TC/.rc/RC.EXE" ]; then
            # Older setups kept the downloads in toolchain/downloads.
            cd_image="$TC/downloads/vc5pro.7z"
            if [ ! -f "$cd_image" ]; then
                cd_image="$DL/vc5pro.7z"
                fetch "https://winworldpc.com/download/3a575cc3-84e2-809c-c383-11c3a6e28094/from/c39ac2af-c381-c2bf-1b25-11c3a4e284a2" \
                    "$cd_image" 106c769210e8b5c4e5be97e86bf4abae
            fi
            tmp="$(mktemp -d -p "$TC")"
            7z x -y -bso0 -bsp0 "$cd_image" -o"$tmp"
            7z x -y -bso0 -bsp0 "$tmp"/*/VCPP-5.00.iso -o"$tmp/iso" 'DEVSTUDIO/SHAREDIDE/BIN/RC.EXE' -r
            mkdir -p "$TC/.rc"
            mv "$tmp/iso/DEVSTUDIO/SHAREDIDE/BIN/RC.EXE" "$TC/.rc/"
            rm -rf "$tmp"
        fi
        echo "fc8b4f7aba5a067fa8868aee5d82b953  $TC/.rc/RC.EXE" | md5sum -c --quiet
        cp "$TC/.rc/RC.EXE" "$TC/$v/BIN/"
    fi
done
rm -rf "$TC/.rc"

# SP3: RTM with the service pack's files laid over it (upper-cased to avoid
# case-only duplicates, which confuse Wine).
if [ ! -d "$TC/msvc5-sp3" ]; then
    fetch "https://archive.org/download/vs97sp3/vs97sp3.zip" "$DL/vs97sp3.zip" e25a5de59a663cd0b5bd3d1089f8adc8
    tmp="$(mktemp -d -p "$TC")"
    7z x -y -bso0 -bsp0 "$DL/vs97sp3.zip" -o"$tmp"
    cabextract -q -d "$tmp/x" "$tmp/vssp3_1.exe"
    cp -r "$TC/msvc5-rtm" "$TC/msvc5-sp3"
    S="$tmp/x/VS97_SP3"
    overlay() {
        [ -d "$1" ] || return 0
        for f in "$1"/*; do
            [ -f "$f" ] || continue
            b="$(basename "$f")"
            find "$2" -maxdepth 1 -iname "$b" -delete
            cp "$f" "$2/${b^^}"
        done
    }
    overlay "$S/all/vc/bin" "$TC/msvc5-sp3/BIN"
    overlay "$S/enu/vc/bin" "$TC/msvc5-sp3/BIN"
    overlay "$S/all/shared/bin" "$TC/msvc5-sp3/BIN"
    overlay "$S/enu/shared/bin" "$TC/msvc5-sp3/BIN"
    overlay "$S/all/vc/include" "$TC/msvc5-sp3/INCLUDE"
    overlay "$S/all/vc/lib" "$TC/msvc5-sp3/LIB"
    rm -rf "$tmp"
fi

# Ghidra, for pseudo-C starting points (tools/ghidra.sh). Needs Java 21 to run.
GHIDRA_ZIP=ghidra_12.1.4_PUBLIC_20260921.zip
if [ ! -d "$TC/ghidra" ]; then
    if [ ! -f "$DL/$GHIDRA_ZIP" ]; then
        curl -fSL -o "$DL/$GHIDRA_ZIP.part" \
            "https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_12.1.4_build/$GHIDRA_ZIP"
        mv "$DL/$GHIDRA_ZIP.part" "$DL/$GHIDRA_ZIP"
    fi
    echo "ddac49f903da9d5bac833e5cc79395098b9c33cfd3279be5f31bd00387d2d4db  $DL/$GHIDRA_ZIP" | sha256sum -c --quiet
    tmp="$(mktemp -d -p "$TC")"
    7z x -y -bso0 -bsp0 "$DL/$GHIDRA_ZIP" -o"$tmp"
    mv "$tmp"/ghidra_* "$TC/ghidra"
    rm -rf "$tmp"
fi

# zlib 1.0.4, statically linked into the game (compiled by Cavedog with /Gz /Zp1).
# Built here so tools/functions.py can recognise its functions.
ZLIB=zlib-1.0.4
if [ ! -f "$TC/thirdparty/$ZLIB/zutil.obj" ]; then
    mkdir -p "$TC/thirdparty"
    fetch "https://github.com/madler/zlib/archive/refs/tags/v1.0.4.tar.gz" "$DL/$ZLIB.tar.gz" 54076bfd66625988bb752b500ced6569
    [ -d "$TC/thirdparty/$ZLIB" ] || tar xzf "$DL/$ZLIB.tar.gz" -C "$TC/thirdparty"
    (cd "$TC/thirdparty/$ZLIB" && for f in adler32 compress crc32 deflate gzio infblock infcodes inffast \
            inflate inftrees infutil trees uncompr zutil; do
        "$ROOT/tools/wcl" /c /O2 /Ob2 /MT /Gy /Gz /Zp1 "/Fo$f.obj" "$f.c" > /dev/null
    done)
fi

if [ ! -f "$ROOT/orig/TotalA.exe" ]; then
    cp "$STEAM_TA/TotalA.exe" "$ROOT/orig/TotalA.exe"
fi
(cd "$ROOT/orig" && sha256sum -c --quiet TotalA.exe.sha256)

echo "Toolchain ready. Try: tools/wcl /c /O2 some.cpp"
