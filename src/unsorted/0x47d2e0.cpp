// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, retried by Sonnet 5.5, retried by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// Can a unit's footprint stand on the map cell `cell`? The guards are the map
// bounds, then the two visibility tests (seen on the shared bit mask, or on the
// player's explored byte map when flag 2 of g_game+0x14281 is set), then a walk
// of the footprint cells that accumulates the build cost into DAT_0051e688 and
// the height envelope into the returned DAT_0051e684.
//
// PARTIAL 88.5% (1339 of 1339 bytes, exact size; 82.0% before this pass).
//
// space-bunny-free pass (#4566, from 82.0%): the file's own note called the
// prologue's esi/edi choice "a colour tie-break inside MSVC 5's LCL, not a
// source-order effect" and made it the headline residual. That was wrong, and
// finding out why is worth the pass: it was the header set. Two levers, both
// re-checkable in one compile each.
//
// WHAT THIS PASS FOUND, in the order it paid:
//  1. THE PROLOGUE'S esi/edi TIE-BREAK WAS FRONT-END STATE, NOT THE SOURCE.
//     The note below called it "a colour tie-break inside MSVC 5's LCL" and
//     measured it as the dominant item. It is not: it is the header set.
//     `#include <string.h>` + `#include <math.h>` in front of the file (nothing
//     from either is used) makes `mov di, word ptr [esp+0x46]` appear where the
//     original has it, and with it origin.x in esi, `los` in edi, g_game in ebp
//     and every later use of those three, which is what the whole 82% plateau
//     was made of. 82.0 -> 86.9 with the header alone. tools/headers.py (all
//     1536 sets) puts four sets at that 86.9 and no set higher; <stdio.h>
//     <stdlib.h>, <string.h> <math.h>, <stdio.h> <string.h> <memory.h> and
//     <string.h> <math.h> <memory.h> <minmax.h> are the four. This is the same
//     front-end-state lever the permuter result recorded as "#include <math.h>
//     with nothing from math.h used, worth 6%", and the same one 0x47d820's
//     notes describe as a symbol-hash coin flip. <windows.h> is NOT it: it makes
//     the frame 1392 bytes. With the header in place the residual is small and
//     specific (below), unlike the register-allocation fog it was before.
//  2. `bit` INITIALISED AFTER THE Contains TEST, not before it. The old notes
//     say "bit declared BEFORE the Contains test, worth several points through
//     register allocation only", and that is true of the 82% baseline; with the
//     header set it inverts. Declaring `unsigned int bit = 1 << g_game->player;`
//     after the `if (!los->explored.size.Contains(x, y)) return 0;` puts the
//     shift after the test, which is where the original has it, and stops bit
//     living in a register across the whole block: 86.9 -> 88.5. Declaring it at
//     the top of the function and assigning it there scores the same 88.5, and
//     so does putting it after the `& bit` test, so the order of the two tests
//     is free.
//  3. The header sweep on THIS file: `<string.h>` alone and `<math.h>` alone
//     both reach 88.5 and are byte-identical to the pair, so the two are the
//     same front-end state; `<memory.h>`, `<crtdbg.h>`, `<cstring>` (87.7),
//     `<exception>` (87.4) and every other real header in the VC5 include
//     directory is equal or worse, including `<windows.h>` (1392 bytes). tools/
//     headers.py on this file: four sets at 88.5, no set higher. Both the
//     declaration-count probe from 0x47d820 (N = 0..64 unused `extern int`,
//     `extern void __cdecl f(void)`, `extern int __cdecl f(int,int)`, `typedef`,
//     `static int` and one `struct` per line) and the symbol-hash probe (the
//     two LOS helpers, the two terrain helpers and Cell/Los/ByteMap/MapSize
//     each renamed, the helpers swapped, the terrain helpers moved above them,
//     the extern globals reordered) are NEGATIVE for this function: 103
//     declaration-count variants and 20 renaming variants are 88.5 or worse, so
//     unlike 0x47d820 and 0x47d0e0 this file's residual is not front-end state.
//  4. Measured flat at 88.5 or worse on top of the file below (each one is
//     byte-identical unless a number is given): the header sets <stdio.h>
//     <string.h> (86.4), <memory.h> (81.5), <string.h> (81.2), <minmax.h>
//     (81.2); the declaration order of x0/y0 (both orders), x0/y0/cols before
//     `Point origin` (81.5-81.7), cols before y0/x0 (80.4), `int ok` after the
//     guard (81.5), `ok = 1` before the guard or initialised at the top
//     (88.2), `unsigned int ok` (64.6), `ok = 1` after the LOS block (45.8);
//     the DAT pair chained, after the guard (87.4), `if (los)` for
//     `if (los != 0)`; pos, hgt, bit, x/y moved into or out of the LOS block
//     (58.7 if bit is initialised in the top block); all 15 permutations of
//     the nine loop locals' declaration order that keep the initialisers with
//     their declarations; min6/max5/max5b declared in the top block (41-44),
//     as `char` (43.3), `int` (63.1), `short` (65.3), comma-separated or in
//     a different order; the cell index as `cell.x + cell.y * width`,
//     `width * cell.y`, `cells + y*w + x`, an `int` index local, `(int)` and
//     `(unsigned)` casts, a `Game*` local or an `int mw` width local (43.6);
//     the vis test through an `unsigned short*` local, `!(v & bit)`, a `v`
//     local, `y * width + x`, a `MapSize*` local, a `w` local; the LOS arms
//     as a ternary (56.4), with the two tests swapped (86.9), with IsSeen
//     first (60.4), with `(flags & 2) != 2` (86.9), with a flag local;
//     the helpers taking the bit by pointer, `hgt` by value, `pos` by value,
//     `los` by reference, a `ByteMap*` (66.8), a `MapSize*` plus data (63.2),
//     or `(data, width, height)` by value (50.7), as static members of a
//     wrapper struct (87.9) or of Game (82.0); `Contains` as a free function
//     (55.1), `Get` returning `int`, `Contains` with a redundant `0 <=`;
//     `Bit()` and `Clamp()` helpers, a third unused inline helper; `static
//     unsigned int bit` (71.0, frame 1360), `unsigned short bit` (69.5),
//     `int bit`, bit used twice; and the braces-around-a-statement lever on the
//     Contains test and the vis test.

// WHAT IS LEFT, four independent items, in rough order of diff lines:
//  * The home slot of the `cols` copy of origin.x. The original stores esi
//    (origin.x) to [esp+0x18], frame offset 8, and reloads it from there after
//    the LOS block; we store to [esp+0x14], frame offset 4. Offset 4 belongs to
//    min6 and offset 8 to max5 in BOTH files, so this is not a different set of
//    slots, it is which byte local MSVC lets the 4-byte cols copy share: theirs
//    shares with max5 (declared after it, at 0x08), ours with min6 (0x04). A
//    declaration-order sweep that moves min6's slot did not move this, because
//    every attempt also moved the `min6 = 0xff` store out of the block the
//    original has it in.
//  * The LOS block keeps the map width in a register: ours hoists
//    `los->explored.size.width` into ebp before the vis index and spills it to
//    the dead `hgt` slot [esp+0x4c], so the slot holds the width and `bit`
//    stays in esi; the original spills `bit` to that slot and reloads it five
//    instructions later (`mov ebx, [esp+0x4c]; test ebp, ebx`), which forces
//    esi to be free, which is why its second index multiply reuses esi
//    (`imul esi, eax`) where ours uses eax off the spilled width.
//  * The cell pointer: the original folds the width into the multiply
//    (`movsx eax, [esp+0x46]; imul eax, [ebp+0x14233]`), we load the width
//    into eax first and register-multiply (`mov eax, [ebp+0x14233]; imul
//    eax, ecx`). 0x47d0e0 has this exact eight-instruction block and settles
//    the cause with an isolation harness: what decides the fold is WHICH
//    REGISTER the allocator gives the sign-extended short. Here it is ecx and
//    there it is eax, so this item, the SIB operand order of the two guard
//    `lea`s (`lea ecx, [esi+edx]` vs `[edx+esi]`, both operands are already in
//    registers there, and both expression orders compile identically) and the
//    ecx/edx choice for the byte temp and the mask pointer in the loop are one
//    allocator decision, not four source bugs. 0x47d0e0 records that ~90 index
//    spellings and two permuter runs do not move it, and this pass is the same
//    result for the same expression (15 spellings here, all byte-identical or
//    worse).
//  * The inner loop's byte temp and mask pointer get ecx in the original and
//    edx here, so `unit` gets edx there and ecx here, and the mask load's SIB
//    byte comes out `[edi+ecx]` (index in the base slot) here as
//    `[edx+edi]`. That is the exact residual 0x47d820 is stuck on, in the same
//    family and with the same `unit->mask[i++]` source, so it is likely the
//    same one-instruction SIB coin flip rather than a spelling.
//
// CARRIED OVER FROM THE EARLIER PASSES, still load bearing:
//  * The ground height is NOT pos.y. The original stores
//    `FUN_00485010(&cell) << 16` into the dead `los` home slot [esp+0x4c] and
//    reads it back with `movsx word [esp+0x4e]`, so it is a separate `Fix`
//    local declared beside the Pos, and both inline helpers take it as a third
//    `Fix*` argument.
//  * Both inline helpers take the player bit as a fourth argument, which stops
//    MSVC re-deriving `1 << g_game->player` inside IsSeen (69.3 -> 80.2), and
//    read the position through a six-short `Position` cast.
//  * `los` is the neighbours' Map: `explored` = ByteMap {data,
//    MapSize{width, height}} with MapSize::Contains and ByteMap::Get.
//  * The loop's rr/terrain tests are early-return inline helpers (Blocked_,
//    Terrain_), which reproduces the `xor ecx,ecx; jmp join` ladders exactly.
//  * The second visibility test is NOT folded to ok = 1 when it goes through an
//    inline helper that re-derives tx/ty from &pos (IsSeen_/IsExplored_) while
//    the first test is written by hand (Contains, then
//    `(vis[w*y+x] & bit) == 0`); a helper taking (los, x, y) is folded again.
//  * The bounds guard is one combined `if` with `short y0` and `short x0` read
//    off the by-value Point, which is what puts `movsx edx, cx` before the
//    width load.
//  * The footprint loop is the rotated form with an explicit outer guard and a
//    POSITIVE bottom test. A plain `for` gives 80.9 (`cmp ecx, edx / jg`),
//    `while (1) { ... if (origin.y <= row) break; }` 81.5, a bare `do/while`
//    57%.
//
// The permuter reached 91.0% on the 82% base; every one of its wins was a
// one-instruction scheduling nudge that did not survive on its own, plus six
// one-line helpers that just return a member, `unsigned int ok`, a
// `goto skip0/skip1/skip2` ladder and `if (1) do {} while (1)`. Its useful
// content was the header, and that is now in the file below, which took its
// score. Re-run on the 88.5% file below (22 min, 9768 candidates, 81 compile
// failures): NO improvement, its best is the same 88.5% (and its best.cpp is
// only cosmetically different plus one more equivalent header), so 88.5% is a
// local optimum of the permuter's mutation space and not just of the spellings
// in the list above.
#include <string.h>
#include <math.h>

#pragma pack(push, 1)

union Fix_0047d2e0 {
    int v;
    struct { short lo; short hi; } p;
};

struct Point {

    short x;
    short y;
};

struct Cell_0047d2e0 {
    short field_0;
    char unknown_2[0x5 - 0x2];
    unsigned char field_5;
    unsigned char field_6;
    unsigned char field_7;
    short field_8;
    unsigned char field_a;
    unsigned char field_b;
    unsigned char field_c;
};

struct Unit_0047d2e0 {
    char unknown_0[0x14a];
    Point origin;
    unsigned char* mask;
    char unknown_152[0x1be - 0x152];
    short field_1be;
    short field_1c0;
    char unknown_1c2[0x228 - 0x1c2];
    unsigned char field_228;
    char unknown_229[0x22c - 0x229];
    unsigned char field_22c;
};

struct MapSize_0047d2e0 {
    unsigned int width;
    unsigned int height;
    int Contains(unsigned int tx, unsigned int ty) { return tx < width && ty < height; }
};

struct ByteMap_0047d2e0 {
    unsigned char* data;
    MapSize_0047d2e0 size;
    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct Los_0047d2e0 {
    char unknown_0[0x7c];
    ByteMap_0047d2e0 explored;
};

struct Game_0047d2e0 {
    char unknown_0[0x2a43];
    unsigned char player;
    char unknown_2a44[0x14233 - 0x2a44];
    int width;
    int height;
    char unknown_1423b[0x14253 - 0x1423b];
    int field_14253;
    char unknown_14257[0x1426f - 0x14257];
    unsigned char* field_1426f;
    unsigned short* field_14273;
    char unknown_14277[0x1427f - 0x14277];
    unsigned char seaLevel;
    char unknown_14280[0x14281 - 0x14280];
    unsigned char losFlags;
    char unknown_14282[0x14287 - 0x14282];
    Cell_0047d2e0* cells;
};
#pragma pack(pop)

extern Game_0047d2e0* g_game;
extern int DAT_0051e684;
extern int DAT_0051e688;

int __stdcall FUN_00485010(Point* p);

struct Pos_0047d2e0 {
    Fix_0047d2e0 x, y, z;
};

struct Position_0047d2e0 {              // 16.16 fixed point, only high words read
    short xFrac;
    short x;
    short yFrac;
    short y;
    short zFrac;
    short z;
};

static inline int IsExplored_0047d2e0(Los_0047d2e0* los, Position_0047d2e0* pos,
    Fix_0047d2e0* hgt, unsigned int bit)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    if (los->explored.size.Contains(tx, ty) && los->explored.Get(tx, ty) != 0)
        return 1;
    return 0;
}

static inline int IsSeen_0047d2e0(Los_0047d2e0* los, Position_0047d2e0* pos,
    Fix_0047d2e0* hgt, unsigned int bit)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (hgt->p.hi >> 1)) >> 5;
    if (!los->explored.size.Contains(tx, ty))
        return 0;
    return (g_game->field_14273[los->explored.size.width * ty + tx] & bit) != 0;
}

static int Blocked_0047d2e0(Cell_0047d2e0* c)
{
    unsigned short v = c->field_8;
    if (v == 0xffff)
        return 0;
    if (v < 0xfffb) {
        if ((int)v >= g_game->field_14253)
            return 1;
        return (g_game->field_1426f[v * 0x100 + 0xfe] >> 6) & 1;
    }
    if (v != 0xfffe)
        return 1;
    Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
    unsigned short v2 = ref->field_8;
    if (v2 >= 0xfffb)
        return 0;
    return (g_game->field_1426f[v2 * 0x100 + 0xfe] >> 6) & 1;
}

static unsigned char* Terrain_0047d2e0(
Cell_0047d2e0* c)
{
    if (c == 0)
        return 0;
    unsigned short v = c->field_8;
    if (v < 0xfffb) {
        if ((int)v >= g_game->field_14253)
            return 0;
        return g_game->field_1426f + v * 0x100;
    }
    if (v != 0xfffe)
        return 0;
    Cell_0047d2e0* ref = c - (c->field_a * g_game->width + c->field_b);
    unsigned short v2 = ref->field_8;
    if (v2 >= 0xfffb)
        return 0;
    return g_game->field_1426f + v2 * 0x100;
}

// FUNCTION: 0x47d2e0
int __stdcall FUN_0047d2e0(Unit_0047d2e0* unit, Point cell, short type, Los_0047d2e0* los)
{
    int ok;
    DAT_0051e684 = 0;
    DAT_0051e688 = 0;
    Point origin = unit->origin;
    short y0 = cell.y;
    short x0 = cell.x;
    if (x0 < 1 || y0 < 1 || x0 + origin.x >= g_game->width ||
        y0 + origin.y >= g_game->height)
        return 0;
    int cols = origin.x;
    int x;
    int y;
    ok = 1;
    if (los != 0) {
        Pos_0047d2e0 pos;
        Fix_0047d2e0 hgt;
        pos.x.v = (origin.x + cell.x * 2) << 19;
        pos.z.v = (origin.y + cell.y * 2) << 19;
        hgt.v = FUN_00485010(&cell) << 16;
        x = pos.x.p.hi >> 5;
        y = (pos.z.p.hi - (hgt.p.hi >> 1)) >> 5;
        if (!los->explored.size.Contains(x, y))
            return 0;
        unsigned int bit = 1 << g_game->player;
        if ((g_game->field_14273[los->explored.size.width * y + x] & bit) == 0)
            return 0;
        if ((g_game->losFlags & 2) == 2)
            ok = IsExplored_0047d2e0(los, (Position_0047d2e0*)&pos, &hgt, bit);
        else
            ok = IsSeen_0047d2e0(los, (Position_0047d2e0*)&pos, &hgt, bit);
    }
    unsigned char min6 = 0xff;
    unsigned char max5 = 0;
    unsigned char max5b = 0;
    int found80 = 0;
    int foundFE20 = 0;
    int index = 0;
    Cell_0047d2e0* c = &g_game->cells[cell.y * g_game->width + cell.x];
    int row;
    int col;
    row = 0;
    if (origin.y > row) {
        do {
            for (col = 0; col < cols; col++) {
                DAT_0051e688 += c->field_7;
                int m = unit->mask[index++];
                if (m & 8) {
                    if (c->field_6 < min6)
                        min6 = c->field_6;
                    if (c->field_5 > max5)
                        max5 = c->field_5;
                }
                if ((m & 0x10) && c->field_5 > max5b)
                    max5b = c->field_5;
                if ((m & 1) && (c->field_c & 2) && ok)
                    return 0;
                if ((m & 6) && c->field_0 != 0 && c->field_0 != type && ok)
                    return 0;
                if (m & 0x20) {
                    if (Blocked_0047d2e0(c) != 0)
                        return 0;
                }
                if (m & 0x40) {
                    unsigned char* e = Terrain_0047d2e0(c);
                    if (e != 0 && (e[0xff] & 2))
                        return 0;
                }
                if (m & 0x80) {
                    found80 = 1;
                    unsigned char* e = Terrain_0047d2e0(c);
                    if (e != 0 && (e[0xfe] & 0x20))
                        foundFE20 = 1;
                }
                ++c;
            }
            row++;
            c = (g_game->width - ((int)cols)) + c;
        } while (row < origin.y);
    }
    if (found80 && !foundFE20)
        return 0;
    unsigned char r;
    if (max5 < min6) {
        r = g_game->seaLevel - unit->field_22c;
    } else {
        if (max5 - min6 > unit->field_228)
            return 0;
        r = min6;
    }
    if (max5b > r)
        return 0;
    if (min6 < g_game->seaLevel - unit->field_1be)
        return 0;
    if ((max5 > max5b ? max5 : max5b) > g_game->seaLevel - unit->field_1c0)
        return 0;
    DAT_0051e684 = r;
    return 1;
}