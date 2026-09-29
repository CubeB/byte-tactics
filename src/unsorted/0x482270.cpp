// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
//
// Gave up at 50.3% (839 bytes vs 817). What still differs, all of it downstream of
// ONE cause, the local frame slot assignment: the original lays its 14 frame dwords
// out as j1, e2, i, x, y, e1, ref, bestDiff, grid, lod/j, table, line, num, count,
// ours comes out in a different order, and every block that reads a spilled local
// then differs. Symptoms of that single cause, in the order they appear:
//   1. the lod clamp: the original keeps the masked value in ecx and SPILLS it to
//      [esp+0x34] before the call, then reloads it for the compare, giving a real
//      branch (`cmp eax,edx / jge`). Ours if-converts the whole if/else into
//      `test ebp,ebp / setl dl / dec edx / and edx,ebp` plus `push`, and hoists the
//      callee's `mov ecx, DAT_0051e6a0` above the `mov ecx,0` the mask needs.
//   2. `g_game->flags_142f1 |= 4` compiles to a cl read-modify-write here instead of
//      the original's `or byte ptr [eax+0x142f1], 4`.
//   3. x2/y2 are folded into registers here; the original stores them back over the
//      e1/e2 slots (`mov [esp+0x24],eax` / `mov [esp+0x14],edx`).
// Tried and rejected: the pre-loop `cells[y*w + x]++` written through a named
// `unsigned char*` (46.3%, worse: it costs `lea eax,[edx+esi]` and drops the
// `add eax,edx` form the original has).
// The register/loop structure of the inner k loop and of the whole else branch is
// otherwise an exact match for the disassembly.
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

struct Grid_482270 {
    unsigned char* cells;              // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int field_c;                       // +0xc
};

struct Player_482270 {
    char unknown_0[0x7c];
    Grid_482270 grid;                  // +0x7c
    char unknown_8c[0x146 - 0x8c];
    unsigned char playerIndex;         // +0x146
};

struct Params_482270 {
    void* field_0;                     // +0x00
    short* field_4;                    // +0x04
    short field_8;                     // +0x08
    unsigned char field_a;             // +0x0a
    char unknown_b;                    // +0x0b
    unsigned char* field_c;            // +0x0c
    char unknown_10[0xc];              // +0x10
};

struct Game_482270 {
    char unknown_0[0x2a43];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14281 - 0x1423b];
    unsigned short bit0 : 1;           // +0x14281
    unsigned short bit1 : 1;
    unsigned short flag2 : 1;
    unsigned short flag3 : 1;
    unsigned short rest : 12;
    char unknown_14283[0x1428f - 0x14283];
    Grid_482270 grid1;                 // +0x1428f
    char unknown_1429f[0x142f1 - 0x1429f];
    unsigned char flags_142f1;         // +0x142f1
    char unknown_142f2[0x1485b - 0x142f2];
    void* losTable;                    // +0x1485b
};

struct Frame_482270 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    char unknown_4[4];
    unsigned char mask;                // +0x8
    char unknown_9[0x10 - 9];
    unsigned char* data;               // +0x10
};

#pragma pack(pop)

extern Game_482270* g_game;

Frame_482270* __stdcall FUN_004b7f30(unsigned short* table, int index);

inline int Lod_482270(Params_482270* params)
{
    int v = params->field_8 / 32;
    return v < 0 ? 0 : v;
}

// FUNCTION: 0x482270
void __stdcall FUN_00482270(Params_482270* params)
{
    if (((Player_482270*)params->field_0)->playerIndex == g_game->playerIndex) {
        g_game->flag3 = 0;
        g_game->flags_142f1 |= 4;
    }
    int halfW = g_game->width / 2;
    int halfH = g_game->height / 2;
    int x = params->field_4[0];
    int y = params->field_4[1];
    if (g_game->flag2 == 1) {
        Grid_482270* grid = &g_game->grid1;
        if ((unsigned)x >= grid->width)
            return;
        if ((unsigned)y >= grid->height)
            return;
        int lod = Lod_482270(params);
        void* table;
        if (lod < ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1)
            table = ((Class_00433500*)DAT_0051e6a0)->FUN_00433500(Lod_482270(params));
        else
            table = ((Class_00433500*)DAT_0051e6a0)->FUN_00433500(
                ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1);
        short count = ((Class_004335c0*)table)->FUN_004335c0();
        short i = 0;
        ((Player_482270*)params->field_0)->grid.cells[y * ((Player_482270*)params->field_0)->grid.width + x]++;
        int ref = *params->field_c;
        if (count <= 0)
            return;
        for (i = 0; i < count; i++) {
            void* line = ((Class_4335e0*)table)->FUN_004335e0(i);
            short num = ((Class_004339c0*)line)->FUN_004339c0();
            int bestDiff = -1;
            int bestIdx = 0;
            short j = 0;
            int j1 = 1;
            if (num <= 0)
                continue;
            for (j = 0; j < num; j++) {
                int e1;
                int e2;
                ((Class_004339e0*)line)->FUN_004339e0(j, &e1, &e2);
                int x2 = x + e1;
                int y2 = y + e2;
                if ((unsigned)(short)x2 >= grid->width)
                    continue;
                if ((unsigned)(short)y2 >= grid->height)
                    continue;
                unsigned char* cell =
                    grid->cells + ((short)y2 * grid->width + (short)x2) * 2;
                int d1 = cell[1] - ref;
                int d0 = cell[0] - ref;
                if (d0 * bestIdx > bestDiff * j1) {
                    ((Player_482270*)params->field_0)->grid.cells[(short)y2 * ((Player_482270*)params->field_0)->grid.width + (short)x2]++;
                    if (d1 * bestIdx > bestDiff * j1) {
                        bestIdx = j1;
                        bestDiff = d1;
                    }
                }
                j1++;
            }
        }
    } else {
        int ref = *params->field_c;
        Frame_482270* frame =
            FUN_004b7f30((unsigned short*)g_game->losTable, ref);
        int limitX = (x + frame->width < halfW) ? frame->width : halfW - x;
        int limitY = (y + frame->height < halfH) ? frame->height : halfH - y;
        int nx = x < 0 ? -x : 0;
        int ny = y < 0 ? -y : 0;
        if (ny >= limitY)
            return;
        if (nx >= limitX)
            return;
        for (int i = ny; i < limitY; i++) {
            unsigned char* src = frame->data + i * frame->width + nx;
            unsigned char* dst =
                ((Player_482270*)params->field_0)->grid.cells + (y + i) * ((Player_482270*)params->field_0)->grid.width + nx + x;
            int n = limitX - nx;
            do {
                if (*src != frame->mask)
                    (*dst)++;
                src++;
                dst++;
            } while (--n);
        }
    }
}
