#!/usr/bin/env bash
# Import TotalA.exe into a Ghidra project seeded with data/functions.csv, then
# export pseudo-C for every game function to build/ghidra/decomp/<address>.c.
# Re-running reuses the analysed project and only re-exports.
# Needs Java 21 (Ghidra finds it under /usr/lib/jvm automatically).
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
HEADLESS="$ROOT/toolchain/ghidra/support/analyzeHeadless"
PROJ="$ROOT/build/ghidra"
CSV="$ROOT/data/functions.csv"
OUT="$PROJ/decomp"
[ -x "$HEADLESS" ] || { echo "Ghidra missing, run tools/setup_toolchain.sh" >&2; exit 1; }
mkdir -p "$PROJ"

# Prefer a Java 21 install even when the system default java is older.
for jdk in /usr/lib/jvm/java-21-openjdk-* /usr/lib/jvm/java-2[2-9]-openjdk-*; do
    if [ -x "$jdk/bin/java" ]; then
        export JAVA_HOME="$jdk" PATH="$jdk/bin:$PATH"
        break
    fi
done

if [ ! -d "$PROJ/TotalA.rep" ]; then
    "$HEADLESS" "$PROJ" TotalA -import "$ROOT/orig/TotalA.exe" \
        -scriptPath "$ROOT/tools/ghidra" \
        -preScript BtImportFunctions.java "$CSV" \
        -postScript BtExportDecomp.java "$CSV" "$OUT"
else
    "$HEADLESS" "$PROJ" TotalA -process TotalA.exe -noanalysis \
        -scriptPath "$ROOT/tools/ghidra" \
        -postScript BtExportDecomp.java "$CSV" "$OUT"
fi
