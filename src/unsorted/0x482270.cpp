// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <vector>

struct Elem_00434020 {
    unsigned short a;
    unsigned short b;
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;
};

typedef std::vector<Elem_00434360> Middle_482270;
typedef std::vector<Middle_482270> Outer_482270;

class Class_00433520 {
public:
    int FUN_00433520();
};

class Class_004335c0 {
public:
    char unknown_0[4];
    char* first;
    char* last;
    int FUN_004335c0();
};

class Class_004339c0 {
public:
    char unknown_0[4];
    char* first;
    char* last;
    int FUN_004339c0();
};

class Class_4335e0 {
public:
    char unknown_0[4];
    char* first;
    Class_004339c0* FUN_004335e0(short index);
};

class Class_004339e0 {
public:
    void FUN_004339e0(short index, unsigned short* out1, unsigned short* out2);
};

class Class_00433500 {
public:
    Class_004335c0* FUN_00433500(int n);
};

extern Class_00433520 DAT_0051e6a0;

#pragma pack(push, 1)
struct Grid_482270 {
    unsigned char* cells;   // +0x0
    int width;              // +0x4
    int height;             // +0x8
    int field_c;            // +0xc
};

class Player_482270 {
public:
    char unknown_0[0x7c];
    Grid_482270 grid;              // +0x7c
    char unknown_8c[0x146 - 0x8c];
    unsigned char field_146;       // +0x146
    char unknown_147[0x14b - 0x147];
};

class Game_482270 {
public:
    char unknown_0[0x2a43];
    unsigned char playerIndex;     // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int field_14233;               // +0x14233
    int field_14237;               // +0x14237
    char unknown_1423b[0x14281 - 0x1423b];
    unsigned short flags;          // +0x14281
    char unknown_14283[0x1428f - 0x14283];
    Grid_482270 grid1;             // +0x1428f
    Grid_482270 grid2;             // +0x1429f
    char unknown_142af[0x142f1 - 0x142af];
    unsigned short blinkOn : 1;    // +0x142f1
    unsigned short unknown_bit1 : 1;
    unsigned short mapChanged : 1;
    unsigned short unknown_rest : 13;
    char unknown_142f3[0x1485b - 0x142f3];
    void* field_1485b;             // +0x1485b
};
#pragma pack(pop)

struct CellEnt_482270 {
    unsigned short u0;             // +0x0
    unsigned short u1;             // +0x2
    char unknown_4[4];
    unsigned char b8;              // +0x8
    char unknown_9[7];
    unsigned char* data;           // +0x10
};

#pragma pack(push, 1)
struct Params_482270 {
    Player_482270* field_0;        // +0x0
    short* field_4;                // +0x4
    short field_8;                 // +0x8
    unsigned char field_a;         // +0xa
    char unknown_b;
    unsigned char* field_c;        // +0xc
    int pos_x;                     // +0x10
    int pos_y;
    int pos_z;
};
#pragma pack(pop)

extern Game_482270* g_game;
CellEnt_482270* __stdcall FUN_004b7f30(unsigned short* table, int index);

// FUNCTION: 0x482270
void __stdcall FUN_00482270(Params_482270* params)
{
    if (params->field_0->field_146 == g_game->playerIndex) {
        g_game->flags &= 0xfff7;
        g_game->mapChanged = 1;
    }
    int halfW = g_game->field_14233 / 2;
    int halfH = g_game->field_14237 / 2;
    int sx = params->field_4[0];
    short sy = params->field_4[1];
    Grid_482270* grid = &g_game->grid1;

    if ((g_game->flags & 4) == 4) {
        if ((unsigned)sx >= (unsigned)grid->width) {
            return;
        }
        if ((unsigned)sy >= (unsigned)grid->height) {
            return;
        }
        int lod = params->field_8 / 32;
        lod = lod < 0 ? 0 : lod;
        short cnt = (short)((Class_00433520*)&DAT_0051e6a0)->FUN_00433520();
        if (lod >= cnt - 1) {
            lod = cnt - 1;
        }
        Class_004335c0* mid = ((Class_00433500*)&DAT_0051e6a0)->FUN_00433500(lod);
        short n = (short)mid->FUN_004335c0();
        Grid_482270* pg0 = &params->field_0->grid;
        pg0->cells[sy * pg0->width + sx]++;
        int c0 = *params->field_c;
        for (short j = 0; j < n; j++) {
            Class_004339c0* ep = ((Class_4335e0*)mid)->FUN_004335e0(j);
            short m = (short)ep->FUN_004339c0();
            int best = -1;
            int w = 0;
            int kp1 = 1;
            short k = 0;
            if (m > 0) {
                for (;;) {
                    int a;
                    int b;
                    ((Class_004339e0*)ep)->FUN_004339e0(k, (unsigned short*)&a, (unsigned short*)&b);
                    a += sx;
                    sx = (short)a;
                    b += sy;
                    short nys = (short)b;
                    if ((unsigned)sx < (unsigned)grid->width
                            && (unsigned)nys < (unsigned)grid->height) {
                        unsigned char* cell = grid->cells + (nys * grid->width + sx) * 2;
                        int lo = cell[0];
                        int hi = cell[1];
                        int dhi = hi - c0;
                        int rhs = best * kp1;
                        if ((lo - c0) * w > rhs) {
                            Grid_482270* pg = &params->field_0->grid;
                            pg->cells[nys * pg->width + sx]++;
                            if (dhi * w > rhs) {
                                w = kp1;
                                best = dhi;
                            }
                        }
                    }
                    k++;
                    kp1++;
                    if (k >= m) {
                        break;
                    }
                }
            }
        }
        return;
    }

    CellEnt_482270* e = FUN_004b7f30((unsigned short*)g_game->field_1485b, *params->field_c);
    int w0 = e->u0;
    int h0 = e->u1;
    int xext = sx + w0 < halfW ? w0 : halfW - sx;
    int yext = sy + h0 < halfH ? h0 : halfH - sy;
    int clipx = sx < 0 ? -sx : 0;
    int clipy = sy < 0 ? -sy : 0;
    if (clipy >= yext) {
        return;
    }
    if (clipx >= xext) {
        return;
    }
    for (int r = clipy; r < yext; r++) {
        unsigned char* dst = params->field_0->grid.cells
            + (sy + r) * params->field_0->grid.width + clipx + sx;
        unsigned char* src = e->data + w0 * r + clipx;
        int c = xext - clipx;
        do {
            if (*src != e->b8) {
                (*dst)++;
            }
            dst++;
            src++;
        } while (--c);
    }
}
