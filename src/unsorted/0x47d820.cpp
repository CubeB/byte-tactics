// Decompiled by Space Bunny Free, finished by space-bunny-free. Names are provisional.
// Ground height under a unit's footprint: walks the rectangle of map cells the
// unit covers and keeps the lowest cell floor over the cells whose footprint
// mask has bit 3 set, plus the highest floor over the cells with bit 3 and the
// highest floor over the cells with bit 4 set. The bit 4 maximum is never read
// (see the bug note), so the result is the low value, or the water surface
// minus the type's draft when no mask bit was set at all.
// The declaration order matters: y before the footprint copy before x is what
// gives g_game ebx and cell.y si, as in the original.
//
// PARTIAL: 99.1%, one instruction out. The footprint mask load is
// `mov al, [edi+esi]` here (mask pointer in the base slot, the mask index in
// the index slot) where the original has `mov al, [esi+edi]`, so MSVC picked
// the other operand as the base of that memory reference. Nothing in the
// source moved it: the index type (int, unsigned, a separate unsigned),
// subscript versus explicit pointer arithmetic, a local copy of the mask
// pointer, the order of the index declaration, an explicit cast of the result
// and incrementing the index in a separate statement were all tried, and
// tools/headers.py (128 sets) plus 20 unused extern declarations in front
// (the compiler-state probe from the guide) all score the same or worse. The
// same swap only appears (with the mask in esi and the index in edi, which the
// rest of the function then gets wrong) when the index is declared before the
// cell pointer. Treated as the guide's "operand order that nothing changes"
// case: one SIB byte, compiler state, move on.
//
// Claude Sonnet 5.5 pass (#571): it IS compiler state, and the source below is
// the right shape. Scoring this file with N unused `extern int dummyK;` lines in
// front of the first `#pragma pack` (check.py --sym, not committed) gives:
//   N = 0 to 8      99.1 percent (this file)
//   N = 16 to 80    MATCH (every step of 8: 16, 24, ... 80)
//   N = 88 to 528   67.9 percent (a different, worse state; 0x47d820 scores the
//                   same as with <windows.h> or <stdio.h> in front)
// So the original file had roughly 16 to 80 declarations in scope before this
// function, fewer than any C header set gives. Plain headers.py: no set matches
// (best <memory.h> 85.0, the empty set 99.1, windows.h/stdio.h/stdlib.h 67.9).
// `headers.py --cpp` printed nothing within 15 minutes (not conclusive). The
// preceding function, 0x47d2e0 (1339 bytes), is too big to define above this
// one as the guide suggests, so the remaining lever is a legitimate header or
// declaration set of about 16 to 80 entries (for example a game header's
// prototypes) that the original included. Untried: sweeping unused function
// prototypes instead of extern ints, and pairs like <string.h> plus <math.h>
// that headers.py covers only as part of its 128 sets.
// Other spellings scored this pass, none helped: `*(i++ + unit->mask)`,
// `(i++)[unit->mask]`, `i[unit->mask]; i++`, an `unsigned char` mask value
// (88.3), a GetMask() getter (84.1), and helpers taking (unit, i).
//
// space-bunny-free pass (#1112), 1 check run. Confirmed the residual is exactly
// one SIB byte, at exactly the right total size (333 = 333). Every source shape
// tried scores 99.1% and none moves it, so the source is right and the choice
// is not in the source text. Measured this pass, all free-scored with score.py:
//
// 1. Real headers, one at a time, all 99.1%: <xutility> <climits> <dsound.h>
//    <list> <algorithm> <utility> <map> <xmemory> <cstring> <cstdio> <cmath>
//    <vector> <string> <iomanip> <new> <typeinfo> <exception> <assert> <set>
//    <deque> <limits> <strstream> <iostream> <fstream> <sys/types.h>. So no
//    header puts this file in the MATCH state, and the answer to #571's "sweep
//    the declaration set" is negative: a real header is not what moves it.
// 2. The mask access, re-spelled 12 ways, all 99.1%: `mask[i]; i++` (separate
//    statement), `*(mask + i++)`, `*(i++ + mask)`, `i++[mask]`, `(i++)[mask]`,
//    `(unsigned char)mask[i++]`, `unsigned` index, `unsigned int f`,
//    `const unsigned char* mask`, `char* mask` (82.6, worse), `mask[++i]; i--`
//    (86.5), a `signed char` temp for the value (79.4), an
//    `*(unsigned char*)(int i + int mask)` integer-add form, and a
//    `MaskRef{p}[i]` struct aggregate.
// 3. The mask pointer as a local, which is where the original's per-row reload
//    would come from, all worse: hoisted before both loops (46.8), assigned
//    inside the outer loop (79.1, both declaration positions), walked with
//    `*m++` (37.1), plus a GetMask(unit, i) helper (29.6).
// 4. Declaration order, the lever that works elsewhere: `i` first, `i` last,
//    `i` between the cell pointer and the loops, `i` with `low`/`high`/`high2`
//    split out one per line, `i` declared after the early return. 99.1, 99.1,
//    93.5, 99.1, 99.1. A second index variable with `i` on the for-increment
//    and a row-base offset (`mask[base + i++]`, 50.9) is much worse.
// 5. Line numbers are not it: 16, 24 and 40 blank lines, and 16, 24 and 40
//    comment lines, are all 99.1%.
//
// The #571 measurement reproduces exactly, and the mechanism is now clearer
// than "declaration count": it is a hash-state coin flip, not a threshold.
// `extern int padv_N;` scores 99.1 at N = 0, 2, 4, 8, 12, 85.0 at N = 6, 10,
// 14, and MATCH at N = 16, 20, 28, 36. `extern void __cdecl padfn_N(void);`
// crosses over at N = 8 and holds MATCH to 36. `extern int __cdecl
// padfn_N(int,int);` is MATCH at N = 6, 8, 12, 16, 20, 28 and 67.9 at 36. A
// typedef or a `static int` flips it at N = 16 and never reverts (MATCH through
// 64); a struct definition per line does the opposite and is 67.9 at every
// count. So the same count can flip or not depending on the names, which is
// what a symbol-hash collision in the front end looks like. One declaration
// alone (84.1) moves the code as much as a wrong source shape does.
//
// That settles it: the MATCH is not reachable from the source, it is a
// front-end state artifact, and the file below is the correct source. Not
// taking the extern-padding MATCH, since padding the file with declarations to
// steer a hash collision is exactly the kind of thing review would undo.
// Instruction: do not spend another pass on this file's operand order.
//
// Guide advice: a base/index SIB-byte swap that no source change moves, and
// that a count of unused declarations can flip on and off non-monotonically,
// is front-end symbol-hash state, not a header and not a spelling. Once real
// headers, real spellings and declaration order are all flat, stop.
#pragma pack(push, 1)

struct Point {
    short x;
    short y;
};

struct Cell {
    char unknown_0[0x5];
    unsigned char field_5;              // +0x5, highest floor
    unsigned char field_6;              // +0x6, lowest floor
    char unknown_7[0xd - 0x7];          // 13 bytes per cell
};

struct Unit_0047d820 {
    char unknown_0[0x14a];
    Point origin;                       // +0x14a, footprint in map cells
    unsigned char* mask;                // +0x14e, one byte per footprint cell
    char unknown_152[0x22c - 0x152];
    unsigned char draft;                // +0x22c
};

struct Game_0047d820 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char seaLevel;             // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    Cell* cells;                        // +0x14287
};
#pragma pack(pop)

extern Game_0047d820* g_game;

// FUNCTION: 0x47d820
int __stdcall FUN_0047d820(Unit_0047d820* unit, Point cell)
{
    short y = cell.y;
    Point fp = unit->origin;
    short x = cell.x;
    if (x < 1 || y < 1 || x + fp.x >= g_game->width || y + fp.y >= g_game->height)
        return 0;
    int width = g_game->width;
    Cell* c = &g_game->cells[y * width + x];
    unsigned char low = 0xff, high = 0, high2 = 0;
    int i = 0;
    for (int row = fp.y; row > 0; row--) {
        for (int col = fp.x; col > 0; col--) {
            int f = unit->mask[i++];
            if (f & 8) {
                if (c->field_6 < low)
                    low = c->field_6;
                if (c->field_5 > high)
                    high = c->field_5;
            }
            if (f & 0x10) {
                if (c->field_5 > high2)
                    high2 = c->field_5;
            }
            c++;
        }
        c += width - fp.x;
    }
    unsigned char r;
    if (high < low)
        r = g_game->seaLevel - unit->draft;
    else
        r = low;
    return r;
}
