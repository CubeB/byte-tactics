// Decompiled by deepseek-v4.1-flash. Names are provisional.
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

// Status: partial, 37.4 percent (1063 bytes vs 1052). Fog-of-war visibility
// bitmask reveal; the sibling of 0x4825b0/0x481d50/0x482270 (same Params and
// gate at flags bit 2) but it toggles the 16-bit per-player bit at
// g_game->visibilityMask (0x14273) instead of decrementing a byte explored map.
// What still differs, all downstream of the local frame slot assignment and of
// which callee-saved register holds the mask bit:
//   1. original slots: y 0x10, x 0x14, changed 0x18, j1 0x1c, e2/y2 0x20,
//      grid/frame 0x24, e1/x2 0x28, ref 0x2c, bestDiff/i 0x30, bestIdx/nx 0x34,
//      i/limitX 0x38, table 0x3c, line 0x40, halfW 0x44, bit 0x48, num 0x4c,
//      count/stride 0x50. Ours puts changed at 0x10 and shifts the rest.
//   2. original holds the mask bit in ebx and only borrows ebp as a scratch for
//      the params/first field load; ours swaps them (params in ebx, bit in ebp).
//      Best declaration order found (x, y, changed, bit, halfW, halfH) scores
//      37.4; bit, halfW, halfH, x, y, changed scores 35.0; halfW, halfH, x, y,
//      changed, bit scores 29.0. The register/frame choice is not explained by
//      declaration order alone.
//   3. the if-branch lod clamp: the original computes `field_8/32` and clamps
//      with `sets cl; dec ecx; and ecx,eax` straight off the sar flags BEFORE
//      calling FUN_00433520 and keeps it in ebp across the call; ours moves the
//      raw quotient into esi and applies `test/setl/dec/and` after the call.
//   4. the else branch dst walk: the original pre-scales a byte offset
//      (shl ebp,1) and advances it by [esp+0x50] = halfW*2; ours recomputes the
//      row offset. The nx<limitX guard is inside the row loop as an if, not a
//      continue (j1++ must still run), which the sibling files got wrong.
// Tried and rejected: <windows.h> vs no header, int vs unsigned int bit,
// four declaration orders. This family (0x481d50, 0x482270) is also stuck in
// the high 40s-low 50s on the same frame-allocation cause.
// FUNCTION: 0x481930
void __stdcall FUN_00481930(Params_00481930* params)
{
    int x = params->field_4[0];
    int y = params->field_4[1];
    int changed = 0;
    unsigned int bit = 1 << params->field_0->field_146;
    int halfW = g_game->width / 2;
    int halfH = g_game->height / 2;
    if (g_game->flag2 == 1) {
        Grid_00481930* grid = &g_game->grid1;
        if ((unsigned)x < grid->width && (unsigned)y < grid->height) {
            int lod = Lod_00481930(params);
            void* table;
            if (lod < ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1)
                table = ((Class_00433500*)DAT_0051e6a0)->FUN_00433500(Lod_00481930(params));
            else
                table = ((Class_00433500*)DAT_0051e6a0)->FUN_00433500(
                    ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1);
            short count = ((Class_004335c0*)table)->FUN_004335c0();
            unsigned short* cell = &g_game->visibilityMask[halfW * y + x];
            if ((bit & *cell) == 0) {
                *cell ^= bit;
                changed = 1;
            }
            int ref = *params->field_c;
            if (count > 0) {
                for (short i = 0; i < count; i++) {
                    void* line = ((Class_4335e0*)table)->FUN_004335e0(i);
                    short num = ((Class_004339c0*)line)->FUN_004339c0();
                    int bestDiff = -1;
                    int bestIdx = 0;
                    if (num > 0) {
                        int j1 = 1;
                        for (short j = 0; j < num; j++) {
                            int e1;
                            int e2;
                            ((Class_004339e0*)line)->FUN_004339e0(j, &e1, &e2);
                            int x2 = x + e1;
                            int y2 = y + e2;
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
                            j1++;
                        }
                    }
                }
            }
        }
    } else {
        int lod = params->field_8 / 32 - 5;
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
                off += halfW * 2;
            } while (i < limitY);
        }
    }
    if (changed && params->field_0->field_146 == g_game->playerIndex) {
        g_game->flag3 = 0;
        g_game->flags_142f1 |= 4;
    }
}
