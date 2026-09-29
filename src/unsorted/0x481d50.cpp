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

struct MapSize_00481d50 {
    unsigned int width;                // +0x0
    unsigned int height;               // +0x4
};

struct ByteMap_00481d50 {
    unsigned char* data;               // +0x0
    MapSize_00481d50 size;             // +0x4
};

struct Map_00481d50 {
    char unknown_0[0x7c];
    ByteMap_00481d50 explored;         // +0x7c
    char unknown_88[0x146 - 0x88];
    unsigned char playerIndex;         // +0x146
};

struct Params_00481d50 {
    void* field_0;                     // +0x00
    short* field_4;                    // +0x04
    short field_8;                     // +0x08
    unsigned char field_a;             // +0x0a
    char unknown_b;                    // +0x0b
    unsigned char* field_c;            // +0x0c
    char unknown_10[0xc];              // +0x10
};

struct Grid_00481d50 {
    unsigned char* cells;              // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int field_c;                       // +0xc
};

struct Vec3_00481d50 {
    int x;
    int y;
    int z;
};

struct Game_00481d50 {
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
    Grid_00481d50 grid1;               // +0x1428f
    char unknown_1429f[0x142f1 - 0x1429f];
    unsigned short flags_142f1_bit0 : 1;  // +0x142f1
    unsigned short flags_142f1_bit1 : 1;
    unsigned short flags_142f1_mapChanged : 1;
    unsigned short flags_142f1_rest : 13;
    char unknown_142f3[0x1485b - 0x142f3];
    void* losTable;                    // +0x1485b
};

struct Frame_00481d50 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    char unknown_4[4];
    unsigned char mask;                // +0x8
    char unknown_9[0x10 - 9];
    unsigned char* data;               // +0x10
};

#pragma pack(pop)

extern Game_00481d50* g_game;

Frame_00481d50* __stdcall FUN_004b7f30(unsigned short* table, int index);

inline int Lod_00481d50(Params_00481d50* params)
{
    int v = params->field_8 / 32;
    return v < 0 ? 0 : v;
}

// Status: partial, 53.5 percent. The body shape is right; what still differs is
// the local frame slot assignment (and everything downstream of it). Original
// slots, low to high: j1 0x10, e2 0x14, i 0x18, x 0x1c, y 0x20, e1 0x24,
// ref 0x28, bestDiff 0x2c, grid 0x30, lod/j 0x34, table 0x38, line 0x3c,
// num 0x40, halfH/count 0x44. Ours: j1 0x10, y 0x14, table 0x18, i 0x1c,
// x 0x20, ref 0x24, bestDiff 0x28, j 0x2c, line 0x30, e1 0x34, e2 0x38,
// grid 0x3c, num 0x40, halfH 0x44. Only j1 (0x10), num (0x40) and halfH (0x44)
// agree. This is MSVC spilling in a different order, not declaration order: a
// probe with six address-taken locals confirms first-declared gets the lowest
// offset, but here only spilled variables get slots, assigned in spill order.
// The inner loop must be the comma-for, `for (j = 0, j1 = 1; (short)j <
// (short)num; j++, j1++)`: j1 increments on the `continue` paths too (the
// original back edge at 0x481f7a bumps both counters). That alone took 52.7 to
// 53.5. Tried and rejected (free scratch scoring): all locals at function scope
// in the exact ascending-slot order (42.2, frame shrank to 779 bytes); removing
// the named `lod` local (identical 52.7); swapping the top declaration order to
// x, y, halfW, halfH (50.9). <windows.h> is required (family sibling 0x4825b0
// matches only with it). The 0x482270 twin is byte-identical apart from the sign
// of the two explored-map updates and is still unmatched at 50.3.
// FUNCTION: 0x481d50
void __stdcall FUN_00481d50(Params_00481d50* params)
{
    if (((Map_00481d50*)params->field_0)->playerIndex == g_game->playerIndex) {
        g_game->flag3 = 0;
        g_game->flags_142f1_mapChanged = 1;
    }
    int halfW = g_game->width / 2;
    int halfH = g_game->height / 2;
    int x = params->field_4[0];
    int y = params->field_4[1];
    if (g_game->flag2 == 1) {
        Grid_00481d50* grid = &g_game->grid1;
        if ((unsigned)x >= grid->width)
            return;
        if ((unsigned)y >= grid->height)
            return;
        int lod = Lod_00481d50(params);
        void* table;
        if (lod < ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1)
            table = ((Class_00433500*)DAT_0051e6a0)->FUN_00433500(Lod_00481d50(params));
        else
            table = ((Class_00433500*)DAT_0051e6a0)->FUN_00433500(
                ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1);
        short count = ((Class_004335c0*)table)->FUN_004335c0();
        short i = 0;
        ((Map_00481d50*)params->field_0)->explored.data[y * ((Map_00481d50*)params->field_0)->explored.size.width + x]--;
        int ref = *params->field_c;
        for (i = 0; i < count; i++) {
            void* line = ((Class_4335e0*)table)->FUN_004335e0(i);
            short num = ((Class_004339c0*)line)->FUN_004339c0();
            int bestDiff = -1;
            int bestIdx = 0;
            short j = 0;
            int j1 = 1;
            for (j = 0, j1 = 1; (short)j < (short)num; j++, j1++) {
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
                    ((Map_00481d50*)params->field_0)->explored.data[(short)y2 * ((Map_00481d50*)params->field_0)->explored.size.width + (short)x2]--;
                    if (d1 * bestIdx > bestDiff * j1) {
                        bestIdx = j1;
                        bestDiff = d1;
                    }
                }
            }
        }
    } else {
        int ref = *params->field_c;
        Frame_00481d50* frame =
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
                ((Map_00481d50*)params->field_0)->explored.data + (y + i) * ((Map_00481d50*)params->field_0)->explored.size.width + nx + x;
            int n = limitX - nx;
            do {
                if (*src != frame->mask)
                    (*dst)--;
                src++;
                dst++;
            } while (--n);
        }
    }
}
