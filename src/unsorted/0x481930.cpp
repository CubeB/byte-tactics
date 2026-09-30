// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, re-tried by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)

class Class_00433500 {
public:
    void* FUN_00433500(int n);
};

class Class_00433520 {
public:
    short FUN_00433520();
};

class Class_004335c0 {
public:
    short FUN_004335c0();
};

class Class_4335e0 {
public:
    void* FUN_004335e0(short i);
};

class Class_004339c0 {
public:
    short FUN_004339c0();
};

class Class_004339e0 {
public:
    void FUN_004339e0(short i, int* a, int* b);
};

extern char DAT_0051e6a0[];

struct Grid_00481930 {
    unsigned char* cells;              // +0x00
    unsigned int width;                // +0x04
    unsigned int height;               // +0x08
    int field_c;                       // +0x0c
};

struct Player_00481930 {
    char unknown_0[0x7c];
    Grid_00481930 grid;                // +0x7c
    char unknown_8c[0x146 - 0x8c];
    unsigned char field_146;           // +0x146
};

struct Params_00481930 {
    Player_00481930* field_0;          // +0x00
    short* field_4;                    // +0x04
    short field_8;                     // +0x08
    unsigned char field_a;             // +0x0a
    char unknown_b;                    // +0x0b
    unsigned char* field_c;            // +0x0c
    char unknown_10[0xc];              // +0x10
};

struct LosTable_00481930 {
    unsigned short count;              // +0x00
};

struct Frame_00481930 {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
    char unknown_4[4];                 // +0x04
    unsigned char mask;                // +0x08
    char unknown_9[0x10 - 9];
    unsigned char* data;               // +0x10
};

struct Game_00481930 {
    char unknown_0[0x2a43];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned short bit0 : 1;           // +0x14281
    unsigned short bit1 : 1;
    unsigned short flag2 : 1;
    unsigned short flag3 : 1;
    unsigned short rest : 12;
    char unknown_14283[0x1428f - 0x14283];
    Grid_00481930 grid1;               // +0x1428f
    char unknown_1429f[0x142f1 - 0x1429f];
    unsigned char flags_142f1;         // +0x142f1
    char unknown_142f2[0x1485b - 0x142f2];
    LosTable_00481930* losTable;       // +0x1485b
};

#pragma pack(pop)

extern Game_00481930* g_game;

Frame_00481930* __stdcall FUN_004b7f30(LosTable_00481930* table, int index);

inline int Lod_00481930(Params_00481930* params)
{
    int v = params->field_8 / 32;
    return v < 0 ? 0 : v;
}

// Status: partial, 53.0 percent (1072 bytes vs 1052). Fog-of-war visibility bitmask
// reveal; sibling of 0x4825b0 / 0x481d50 / 0x482270 (same Params, same gate on the
// bitfield at g_game+0x14281 bit 2) but it toggles the 16-bit per-player bit at
// g_game->visibilityMask (0x14273) instead of decrementing an explored map.
// What moved the number (from 37.4):
//   1. The lod table lookup must be ONE call with a ternary ARGUMENT, not the call
//      duplicated in both arms (guide item 27 is backwards here). The original has a
//      single `push eax` at the diamond join (0x481a1c); duplicating the call gives
//      two pushes and a tail merge. The ternary's two arms must each be a full
//      Lod(params) expression, not a shared named local: the original recomputes the
//      whole `/32` + clamp in the true arm (0x4819f0..0x481a0a) even though the
//      first copy is still live in ebp across the FUN_00433520 call.
//   2. Outer declaration order `changed, bit, halfW, halfH, x, y` (NOT x, y, changed).
//      With x/y first MSVC gives bit and halfW stack homes and we end up with 16 frame
//      slots and a different prologue store order; with this order they stay in
//      registers, the frame shrinks, and x lands in ebp and y in eax exactly as the
//      original does. Worth 8 points on its own.
//   3. The two out-params of Class_004339e0 are declared `int y2; int x2;` and the call
//      is `(j, &x2, &y2)`; the second pushed pointer (lower frame slot) is the one that
//      gets y added. Swapping them costs 5 points.
//   4. The inner scan is a `for (j = 0, j1 = 1; (short)j < (short)num; j++, j1++)`, not
//      a do/while: MSVC otherwise folds the inner counter into the outer one and emits
//      `lea eax, [ebx-1]` where the original has its own memory counter. Worth 1.3.
// What still differs, all one frame-allocation cause plus register choice:
//   1. Frame slots. Original (17 dwords): y 0x10, x 0x14, changed 0x18, j1 0x1c,
//      y2 0x20, grid/frame 0x24, x2 0x28, ref 0x2c, bestDiff/row 0x30, bestIdx/nx 0x34,
//      i/limitX 0x38, table 0x3c, line 0x40, halfW 0x44, bit 0x48, num 0x4c,
//      count/stride 0x50. Ours matches y, x, grid, x2, ref, bestDiff and is shifted
//      from bestIdx up. I could NOT derive MSVC 5's slot assignment order: it is not
//      declaration order, not reverse declaration order and not first or last use
//      (checked against the offset lists the /Fa listing prints). Everything from
//      bestIdx upwards is a single permutation.
//   2. In the if branch the original keeps bestIdx in ebx (zeroed by `xor ebx,ebx` at
//      0x481a9f and never spilled) and spills BOTH inner counters: j at [esp+0x34] and
//      j1 at [esp+0x1c]. We keep bestIdx in memory and j1 in ebx and derive j=j1-1
//      (`lea eax,[ebx-1]`). That is also why our frame is one dword short: the original
//      needs a slot for j, we fold j into ebx. Ruled out this session: `register` on
//      bestIdx, swapping the bestIdx/bestDiff declarations, and hoisting
//      `int j = 0, j1 = 1;` out of the for header (all identical, 53.0). The board
//      note "original holds the bit in ebx" is wrong: the bit has a home at [esp+0x48]
//      and is reloaded into edi for each `(bit & *cell)` test; ebx is bestIdx.
//   3. `table` is memory-resident in the original (`mov [esp+0x3c], eax`, reloaded each
//      outer iteration); ours parks it in ebp.
//   4. `(bit & *cell) == 0`: the original ANDs the two registers and tests only DI/BP
//      (it knows the result fits in 16 bits); we materialise `and edx, 0xffff` first.
// Tried and rejected (all scored equal or worse, free scratch scoring):
//   `if (v < 0) v = 0;` instead of `v < 0 ? 0 : v` in Lod (43.0, no change);
//   outer orders (x,y,changed,bit,halfW,halfH) 46.8, (changed,x,y,bit,halfW,halfH)
//   47.1, (changed,bit,halfH,halfW,x,y) 49.9, (changed,bit,halfW,halfH,y,x) 46.6;
//   swapping the bestDiff/bestIdx declarations (no change); hoisting j out of the
//   `if (num > 0)`; `int i` instead of `short i` for the outer loop; a `while` outer
//   loop; else-branch declaration order (limitY before limitX, ny before nx) 43.8 and
//   (stride, off, i) 49.8; explicit `(unsigned short)` casts on the mask tests.
// FUNCTION: 0x481930
void __stdcall FUN_00481930(Params_00481930* params)
{
    int changed = 0;
    unsigned int bit = 1 << params->field_0->field_146;
    int halfW = g_game->width / 2;
    int halfH = g_game->height / 2;
    int x = params->field_4[0];
    int y = params->field_4[1];
    if (g_game->flag2 == 1) {
        Grid_00481930* grid = &g_game->grid1;
        if ((unsigned)x < grid->width && (unsigned)y < grid->height) {
            void* table = ((Class_00433500*)DAT_0051e6a0)
                              ->FUN_00433500(
                                  (Lod_00481930(params) <
                                   ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1)
                                      ? Lod_00481930(params)
                                      : ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1);
            short count = ((Class_004335c0*)table)->FUN_004335c0();
            unsigned short* cell = &g_game->visibilityMask[halfW * y + x];
            if ((bit & *cell) == 0) {
                *cell ^= bit;
                changed = 1;
            }
            int ref = *params->field_c;
            for (short i = 0; (short)i < count; i++) {
                void* line = ((Class_4335e0*)table)->FUN_004335e0(i);
                short num = ((Class_004339c0*)line)->FUN_004339c0();
                int bestDiff = -1;
                int bestIdx = 0;
                if (num > 0) {
                    for (int j = 0, j1 = 1; (short)j < (short)num; j++, j1++) {
                        int y2;
                        int x2;
                        ((Class_004339e0*)line)->FUN_004339e0((short)j, &x2, &y2);
                        x2 += x;
                        y2 += y;
                        if ((unsigned)(short)x2 < grid->width &&
                            (unsigned)(short)y2 < grid->height) {
                            unsigned char* c =
                                grid->cells + ((short)y2 * grid->width + (short)x2) * 2;
                            int d1 = c[1] - ref;
                            int d0 = c[0] - ref;
                            if (d0 * bestIdx > bestDiff * j1) {
                                unsigned short* q = &g_game->visibilityMask[
                                    halfW * (short)y2 + (short)x2];
                                if ((bit & *q) == 0) {
                                    *q ^= bit;
                                    changed = 1;
                                }
                                if (d1 * bestIdx > bestDiff * j1) {
                                    bestIdx = j1;
                                    bestDiff = d1;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        int lod = Lod_00481930(params) - 5;
        if (lod < 0)
            lod = 0;
        else if (lod >= g_game->losTable->count)
            lod = g_game->losTable->count - 1;
        Frame_00481930* frame = FUN_004b7f30(g_game->losTable, lod);
        int limitX = (x + frame->width < halfW) ? frame->width : halfW - x;
        int limitY = (y + frame->height < halfH) ? frame->height : halfH - y;
        int nx = x < 0 ? -x : 0;
        int ny = y < 0 ? -y : 0;
        changed = 0;
        if (ny < limitY) {
            int i = ny;
            int stride = halfW * 2;
            int off = ((y + ny) * halfW + nx + x) * 2;
            do {
                unsigned char* src = frame->data + i * frame->width + nx;
                unsigned short* dst =
                    (unsigned short*)((unsigned char*)g_game->visibilityMask + off);
                if (nx < limitX) {
                    int n = limitX - nx;
                    do {
                        if (*src != frame->mask && (bit & *dst) == 0) {
                            changed = 1;
                            *dst ^= bit;
                        }
                        src++;
                        dst++;
                    } while (--n);
                }
                i++;
                off += stride;
            } while (i < limitY);
        }
    }
    if (changed && params->field_0->field_146 == g_game->playerIndex) {
        g_game->flag3 = 0;
        g_game->flags_142f1 |= 4;
    }
}