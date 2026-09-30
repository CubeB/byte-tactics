// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, re-tried by deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free. Names are provisional.
// PARTIAL: 71.0% (1047 of 1052 bytes). Fixed this session, all in the branch
// that selects the LOD frame index:
//  1. The else branch needs the UNCLAMPED lod/32 - 5, then the clamp. Clamping
//     inside the helper first (as before) emitted sets/dec/and before the -5.
//     A separate unclamped helper for that site was worth 0.8.
//  2. The inner j loop needs NO `if (num > 0)` wrapper: a single `for` test
//     gives the original's one `test ax,ax / jle` instead of two. Worth 2.9.
//  3. With (2) in place the coordinate outputs no longer want `= 0`, and
//     dropping the zero stores was worth 2.5. This is the trap: on its own,
//     removing those initialisers COST 5.6 points. It only pays once the
//     duplicated loop test is gone, because both perturb the same allocation.
// Still different: the inner loop's two induction variables are allocated the
// wrong way round (ours puts j1 in ebx and bestIdx in a frame slot, the
// original puts bestIdx in ebx and j1 at [esp+0x1c], with j in ecx), and
// because ours folds j into j1-1 the frame is one dword short (0x40 not
// 0x44). The lod clamp is also sunk past the FUN_00433520 call here, so the
// first copy in the frame is the raw value. See NOTES at the bottom.
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
    void FUN_004339e0(short i, unsigned short* a, unsigned short* b);
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

inline int LodRaw_00481930(Params_00481930* params)
{
    return params->field_8 / 32;
}

inline int Lod_00481930(Params_00481930* params)
{
    int v = LodRaw_00481930(params);
    return v < 0 ? 0 : v;
}

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
            if ((unsigned short)(bit & *cell) == 0) {
                *cell ^= bit;
                changed = 1;
            }
            int ref = *params->field_c;
            for (short i = 0; (short)i < count; i++) {
                void* line = ((Class_4335e0*)table)->FUN_004335e0(i);
                short num = ((Class_004339c0*)line)->FUN_004339c0();
                int j1 = 1;
                int bestIdx = 0;
                int bestDiff = -1;
                {
                    for (short j = 0; (short)j < (short)num; j++) {
                        j1++;
                        int y2, x2;
                        ((Class_004339e0*)line)->FUN_004339e0((short)j, (unsigned short*)&x2, (unsigned short*)&y2);
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
                                if ((unsigned short)(bit & *q) == 0) {
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
        int lod = LodRaw_00481930(params) - 5;
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
                        if (*src != frame->mask && (unsigned short)(bit & *dst) == 0) {
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
// NOTES for the next attempt (all of these were tried and did NOT help, so do
// not repeat them):
//  * Naming the clamped lod in a local (`int lod = Lod(params); if (lod < ...)`)
//    does not force the clamp before the call. MSVC 5 still sinks the mask
//    sequence past the call and keeps only the RAW value live in ebp.
//  * The clamp helper spelled `if (v < 0) v = 0; return v;` instead of
//    `v < 0 ? 0 : v` is worse by 3.1: the original really is the ternary, which
//    if-converts to sets/dec/and.
//  * Nesting the two lod helpers (a clamped one calling an unclamped one) makes
//    no difference at all, as the guide's "an inlined boundary is not a CSE
//    boundary" note predicts. Only the caller's spelling matters.
//  * `int j, j1` instead of `short j` for the inner counters: worse by 10.
//  * `int count` / `int num` instead of `short`: worse by 0.3.
//  * Reordering the `bestIdx` / `bestDiff` initialisers: catastrophic, 28.9%.
//  * Hoisting a shared `int rhs = bestDiff * j1;` for the two comparisons
//    changes nothing: the original already reuses the product in eax.
//  * Moving `j1++` to the end of the loop body instead of the top: no change.
//  * Wrapping the loop in a bare brace block instead of `if (num > 0)`: needed
//    for fix (2) above; the block itself is otherwise free.
