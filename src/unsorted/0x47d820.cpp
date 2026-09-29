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
