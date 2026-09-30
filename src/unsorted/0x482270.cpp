// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, deepseek-v4.1-flash, GPT-6, and GPT-6.1-sol. Names are provisional.
// Partial: 61.0%, 832 bytes versus 817. The distance counter advances even
// for out-of-bounds points. Grid accessors recover the receiver-relative
// addressing. Remaining differences include the LOD clamp timing, local
// frame slots and register allocation in both branches. Two 768-set header
// sweeps did not improve the result.
// GPT-6.1-sol retry in #1932: four checks kept 61.0%; the reversed coordinate
// declaration tied, while do-while conversions scored 59.1%. Neighbor coordinates
// need a signed 16-bit cast before an unsigned bounds check (movsx AX then cmp).
#include <windows.h>

#pragma pack(push, 1)

class Class_00433500 {
public:
    void* FUN_00433500(int n);
};

class Class_00433520 {
public:
    int FUN_00433520();
};

class Class_004335c0 {
public:
    int FUN_004335c0();
};

class Class_4335e0 {
public:
    void* FUN_004335e0(short i);
};

class Class_004339c0 {
public:
    int FUN_004339c0();
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
    unsigned char& at(int x, int y) { return cells[y * width + x]; }
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
    unsigned short f0 : 1;             // +0x142f1 bit 0
    unsigned short f1 : 1;
    unsigned short flagA : 1;          // bit 2, mask 4
    unsigned short f3 : 5;
    unsigned short fhi : 8;
    char unknown_142f3[0x1485b - 0x142f3];
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
        g_game->flagA = 1;
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
        int idx = Lod_482270(params);
        if (idx < (short)((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1)
            idx = Lod_482270(params);
        else
            idx = (short)((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1;
        void* table = ((Class_00433500*)DAT_0051e6a0)->FUN_00433500(idx);
        short count = ((Class_004335c0*)table)->FUN_004335c0();
        short i = 0;
        ((Player_482270*)params->field_0)->grid.at(x, y)++;
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
            for (j = 0; j < num; j++, j1++) {
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
                    ((Player_482270*)params->field_0)->grid.at((short)x2, (short)y2)++;
                    if (d1 * bestIdx > bestDiff * j1) {
                        bestIdx = j1;
                        bestDiff = d1;
                    }
                }
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
                dst++;
                src++;
            } while (--n);
        }
    }
}
