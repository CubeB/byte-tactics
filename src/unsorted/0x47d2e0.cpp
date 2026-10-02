// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, retried by Sonnet 5.5, retried by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// Can a unit's footprint stand on the map cell `cell`? The guards are the map
// bounds, then the two visibility tests (seen on the shared bit mask, or on the
// player's explored byte map when flag 2 of g_game+0x14281 is set), then a walk
// of the footprint cells that accumulates the build cost into DAT_0051e688 and
// the height envelope into the returned DAT_0051e684.
//
// PARTIAL 82.0% (1339 of 1339 bytes, exact size; was 70.6% at 1337 bytes).
//
// What moved it, from the 70.6% starting point:
//  * The ground height is NOT pos.y. The original stores `FUN_00485010(&cell) << 16`
//    into the dead `los` home slot [esp+0x4c] and reads it back with
//    `movsx word [esp+0x4e]`, so it is a separate `Fix` local declared beside the
//    Pos, and both inline helpers take it as a third `Fix*` argument. With the
//    height inside Pos the helpers keep it there and neither the frame layout nor
//    the store sequence can match (70.6% -> 69.3% on its own, but it is what makes
//    the rest reachable).
//  * Both inline helpers take the player bit as a fourth argument. That stops MSVC
//    from re-deriving `1 << g_game->player` inside IsSeen, which is what put the
//    g_game reload and the vis multiply in the wrong basic block (69.3% -> 80.2%).
//  * The helpers read the position through a six-short `Position` cast
//    (`(Position_0047d2e0*)&pos`), the shape matched in 0x408090 and 0x465ac0,
//    instead of `pos->x.p.hi` (80.2% -> 80.7%).
//  * The bounds guard is the one combined `if` of the 99.1% sibling 0x47d820, with
//    `short y0` and `short x0` read off the by-value Point first. That is what puts
//    `movsx edx, cx` (cell.x) before the width load as the original has it
//    (80.7% -> 80.9%).
//  * The footprint loop is written in the rotated form the original emits: an
//    explicit outer guard with a POSITIVE bottom test
//    (`row = 0; if (origin.y > row) { do { for (col = 0; ...) { ...; ++c; }
//    row++; c = (width - cols) + c; } while (row < origin.y); }`). The original
//    has both the guard and the bottom test, and writing the rotation out with the
//    test positive rather than as a negated `break` is what puts
//    `cmp edx, ecx / jl` where the original has it (80.9% -> 82.0%). A plain `for`
//    gives 80.9% (`cmp ecx, edx / jg`), `while (1) { ... if (origin.y <= row)
//    break; }` gives 81.5%, and a `do/while` with no guard 57%.
//
// Earlier passes, still true:
//  * `los` is the neighbours' Map: `explored` = ByteMap {data, MapSize{width, height}}
//    with MapSize::Contains and ByteMap::Get (see 0x4658e0, 0x465ac0, 0x408090).
//  * The loop's `rr`/Terrain tests are early-return inline helpers (Blocked_,
//    Terrain_), which reproduces the `xor ecx,ecx; jmp join` ladders exactly.
//  * The second visibility test is NOT folded to ok = 1 when it goes through an
//    inline helper that re-derives tx/ty from `&pos` (IsSeen_/IsExplored_) while the
//    first test is written by hand (Contains, then `(vis[w*y+x] & bit) == 0`). A
//    helper taking (los, x, y) is folded again, and so is any hand-written copy.
//  * `int ok` declared at the very top (assigned before `if (los)`), and `bit`
//    declared BEFORE the Contains test, each worth several points through register
//    allocation only.
//
// WHAT IS LEFT. The dominant item is one cause: the esi/edi choice in the prologue.
// The original takes `esi` for origin.x (spilled to [esp+0x18]) and `edi` for `los`;
// we take `edi` for origin.x ([esp+0x14]) and `esi` for `los`, so every later use of
// the two is mirrored, `los` has to be reloaded from [esp+0x4c] in the explored arm,
// and the loop's mask index lands in esi instead of edi. The register the 16-bit
// cell.y temp takes decides it: the original emits `mov di, word [esp+0x46]`, we emit
// `mov si, word [esp+0x46]`. It is a colour tie-break inside MSVC 5's LCL, not a
// source-order effect: the same two values come out in the other order in the
// sibling 0x47d820, whose notes call the same thing for `esi`/`edi`.
//
// Tried and flat at 82.0% or worse (all on top of the best version here, most of
// them measured on the 80.9% and 81.5% intermediate versions):
//   - guard: `origin.x + cell.x` / `cols + cell.x` / `cell.x + cols`, `x0 + origin.x`
//     and `origin.x + x0`; `int width0` cached before or after `cols`; `int rows`,
//     `int oy`, `int cy` locals; the guard as two `if`s, as one `||`, as an inline
//     OutOfBounds helper; the two bounds tests swapped; `cols` materialised before
//     the cell guard; origin read after the guard; `int x0`/`y0` as `int` not
//     `short`; `cols` declared then assigned; `x0 < 1` as `1 > x0`.
//   - LOS block: the map width cached in a local `w` (with and without passing it to
//     the helpers), a `row = width * y` temporary, `g_game` through a `Game*` local,
//     the first visibility test folded into its own inline helper (54-61%), `los` and
//     `unit` cached in locals, the helpers re-deriving the bit (69.3%) or taking it
//     as a pointer, the helpers taking the Fix height by value, `2 == (flags & 2)`,
//     `0 == Contains`, the explored arm's Contains as a nested `if` or an early
//     `return 0`, braces on the visibility arms.
//   - loop: the mask-index spellings (`*(mask + i)`, `mask[i]; i++`, the load before
//     or after the cost accumulation), `DAT_0051e688 = DAT_0051e688 + c->field_7`,
//     the 0x40/0x80/owner/type tests as nested `if`s, `8 & m` and `2 & e[0xff]`
//     operand swaps, `unsigned int ok` (69.3%), merging the latch declarations,
//     hoisting `g_game->seaLevel - unit->field_1be`, `(unsigned char)min6` in the
//     spread test, `min6 > max5` for `max5 < min6`, the found80 latch as a nested
//     `if`, `col = 1 + col` / `++c` alone, `c = (width - cols) + c`, a
//     `unsigned char hi5 = max5;` local for the loop-exit load, and swapping
//     min6/max5 (75.5%).
//   - prologue: adding a second `Game*` local for the cell index (64.6%), a
//     `CellAt()` helper for it (75.2%), a `GetMask()` helper (80.4%), and
//     `#include <math.h>` / `<string.h>` / `<memory.h>` / `<stdlib.h>` (66-80%).
//
// THE PERMUTER REACHED 91.0% AND ITS RESULT IS NOT USABLE SOURCE. tools/permute.py
// (1339 candidates, 17 min) got this function from 80.2% to 91.0% at the exact size,
// and build/permute/0x47d2e0/best.cpp is that result. Every one of its wins is a
// one-instruction scheduling nudge and none of them survives on its own: reverting
// any single change costs about 0.6%, and a clean rewrite of the whole function in
// its shape scores 63.8%. What it adds, none of which I can write as plausible
// source: `#include <math.h>` with nothing from math.h used (worth 6% on its own,
// and the same front-end-state lever 0x47d820's notes describe); six one-line
// helpers that just return a member or a mask (`inl0..inl5`, e.g.
// `static inline int inl0(int m) { return m & 0x10; }`); `unsigned int ok`; a
// `goto skip0/skip1/skip2` ladder where the original has none; and
// `if (1) do { ... } while (1);` around the outer loop, which duplicates the guard
// the guide warns against. Per the guide's rule, those are recorded here as leads
// rather than committed. Its guard change is neutral here: best.cpp with this
// file's guard still scores 91.0%, so the 9 points it holds over this file are
// front-end state and one-instruction scheduling, not source shape.
//
// Also still open, independent of the register flip: the explored arm's own
// `mov [esp+0x10], 0` block (we share one store with the seen arm), and
// `movsx eax,[esi+0x46]; imul eax,[ebp+0x14233]` against our width-into-eax form,
// which is the same sign-extended-short multiply 0x47d0e0 and 0x47d820 both record
// as unreachable from the source.
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
        unsigned int bit = 1 << g_game->player;
        if (!los->explored.size.Contains(x, y))
            return 0;
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