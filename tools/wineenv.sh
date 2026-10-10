# Sourced by tools/wcl and tools/wlib: the Wine environment the compiler runs in.
#
# The compiler's front end numbers its symbols from a counter that the paths it
# is given move (docs/c2-regalloc.md, "The compile's paths move the ids"), so
# the same tree built to different bytes under different checkout paths. Every
# path the compiler sees is therefore written under drive Y:, which is this
# checkout's root, so the strings are the same wherever the checkout is.
#
# A drive mapping belongs to a Wine prefix, and one prefix is shared by every
# process using it, so each checkout gets its own prefix, $ROOT/build/wineprefix,
# whose Y: points at that checkout. It is made from toolchain/wineprefix (the
# registry files are copied, drive_c is shared), or by Wine itself if that does
# not exist yet. Worktrees compile at once without touching each other's Y:.
#
# Needs ROOT set; sets TC, WINEPREFIX, WINEARCH, WINEDEBUG, INCLUDE and LIB, and
# defines winpath (a path under $ROOT as the compiler sees it, Y:\...).

TC="$ROOT/toolchain/${BT_TOOLCHAIN:-msvc5-sp3}"

winpath() {
    case "$1" in
        "$ROOT") echo 'Y:\' ;;
        "$ROOT"/*) local rel="${1#"$ROOT"/}"; echo "Y:\\${rel//\//\\}" ;;
        *) echo "Z:${1//\//\\}" ;;
    esac
}

TEMPLATE="${BT_WINEPREFIX_TEMPLATE:-$ROOT/toolchain/wineprefix}"
WINEPREFIX="$ROOT/build/wineprefix"
export WINEDEBUG=-all
# Keep an existing prefix's architecture: setups made before this change have a
# win32 prefix, which Wine refuses to run as win64. New prefixes are win64,
# which wow64-only Wine (Wine 11 on Fedora, for one) requires and which runs the
# 32-bit compiler on other Wine builds too.
if [ -z "${WINEARCH:-}" ]; then
    WINEARCH="$(sed -n 's/^#arch=//p' "$TEMPLATE/system.reg" 2>/dev/null | head -n 1 || true)"
fi
export WINEARCH="${WINEARCH:-win64}"

# The prefix is made in a scratch folder and renamed into place, so compiles
# starting together never see it half made. The drive links hold absolute
# paths, so a checkout that moved is repaired here too.
make_prefix() {
    local tmp
    mkdir -p "$ROOT/build"
    tmp="$(mktemp -d "$ROOT/build/wineprefix.XXXXXX")"
    if [ -f "$TEMPLATE/system.reg" ]; then
        cp "$TEMPLATE"/*.reg "$TEMPLATE"/.update-timestamp "$tmp/" 2>/dev/null || true
        mkdir -p "$tmp/dosdevices"
        ln -s "$TEMPLATE/drive_c" "$tmp/drive_c"
    else
        WINEPREFIX="$tmp" wineboot -i >/dev/null 2>&1 || true
        WINEPREFIX="$tmp" wineserver -w 2>/dev/null || true
    fi
    # The compiler's temporary files are named after its process id, and each
    # prefix has its own Wine server numbering processes from the same start,
    # so prefixes sharing a temp folder overwrite one another's files (C1083,
    # C1900, C1001). Each prefix gets a temp folder of its own, in the checkout.
    sed -i -E 's/^"(TEMP|TMP)"=.*/"\1"="Y:\\\\build\\\\wintemp"/' "$tmp/user.reg"
    mv -T "$tmp" "$WINEPREFIX" 2>/dev/null || rm -rf "$tmp"
}
[ -f "$WINEPREFIX/system.reg" ] || make_prefix
mkdir -p "$ROOT/build/wintemp"
mkdir -p "$WINEPREFIX/dosdevices"
[ "$(readlink "$WINEPREFIX/dosdevices/y:" 2>/dev/null)" = "$ROOT" ] || ln -sfn "$ROOT" "$WINEPREFIX/dosdevices/y:"
[ -e "$WINEPREFIX/dosdevices/z:" ] || ln -sfn / "$WINEPREFIX/dosdevices/z:"
[ -e "$WINEPREFIX/dosdevices/c:" ] || ln -sfn "$WINEPREFIX/drive_c" "$WINEPREFIX/dosdevices/c:"
export WINEPREFIX
export INCLUDE="$(winpath "$TC/INCLUDE")" LIB="$(winpath "$TC/LIB")"
