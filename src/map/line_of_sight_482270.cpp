// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, deepseek-v4.1-flash, GPT-6, GPT-6.1-sol, deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.
// Kept out of line_of_sight.cpp: the merged file's symbol ids move its registers.
// Unused by the code but needed: it flips the operand order of `i * frame->width`.
#include <math.h>
#include <windows.h>

#pragma pack(push, 1)

class LosTables {
public:
    void* GetLosTable(int n);
    short GetLosTableCount();
};

class LosTable {
public:
    short GetLosLineCount();
};

class Class_4335e0 {
public:
    void* GetLosLine(short i);
};

class LosLine {
public:
    short GetLosLineStepCount();
    void GetLosLineStep(short i, int* a, int* b);
};

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int RIReport(int, int, int, int, int, int, int, int, int, int);
void CopyDwordIfNonNull(int*, int*);

extern char g_losTables[];

struct MapSize_482270 {
    unsigned int width;                // +0x0
    unsigned int height;               // +0x4
};

struct ByteMap_482270 {
    unsigned char* data;               // +0x0
    MapSize_482270 size;               // +0x4
    unsigned char& at(int x, int y) { return data[y * size.width + x]; }
};

struct Grid_482270 {
    unsigned char* cells;              // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int field_c;                       // +0xc
};

struct Player_482270 {
    char unknown_0[0x7c];
    ByteMap_482270 grid;              // +0x7c
    char unknown_88[0x146 - 0x88];
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

struct Game {
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

extern Game* g_game;

Frame_482270* __stdcall GetGafFrame(unsigned short* table, int index);

// FUNCTION: 0x482270
void __stdcall AddLineOfSight(Params_482270* params)
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
        // Clamp written inline as the GetLosTable argument, with no temporary.
        void* table = ((LosTables*)g_losTables)
                          ->GetLosTable(
                              (params->field_8 / 32 < 0 ? 0 : params->field_8 / 32)
                                      < ((LosTables*)g_losTables)
                                            ->GetLosTableCount() - 1
                                  ? (params->field_8 / 32 < 0 ? 0 : params->field_8 / 32)
                                  : ((LosTables*)g_losTables)
                                        ->GetLosTableCount() - 1);
        short count = ((LosTable*)table)->GetLosLineCount();
        short i = 0;
        ((Player_482270*)params->field_0)->grid.at(x, y)++;
        int ref = *params->field_c;
        for (i = 0; i < count; i++) {
            void* line = ((Class_4335e0*)table)->GetLosLine(i);
            short num = ((LosLine*)line)->GetLosLineStepCount();
            // Declared in this order: bestIdx, j1, bestDiff, j; dx and dy stay int locals.
            int bestIdx = 0;
            int j1;
            int bestDiff = -1;
            short j = 0;
            if ((short)num > 0) {
                j1 = 1;
                do {
                    int dx;
                    int dy;
                    ((LosLine*)line)->GetLosLineStep(j, &dx, &dy);
                    dx += x;
                    dy += y;
                    if ((unsigned)(short)dx >= grid->width)
                        continue;
                    if ((unsigned)(short)dy >= grid->height)
                        continue;
                    unsigned char* cell =
                        grid->cells + ((short)dy * grid->width + (short)dx) * 2;
                    int d1 = cell[1] - ref;
                    int d0 = cell[0] - ref;
                    if (d0 * bestIdx > bestDiff * j1) {
                        ((Player_482270*)params->field_0)
                            ->grid.at((short)dx, (short)dy)++;
                        if (d1 * bestIdx > bestDiff * j1) {
                            bestIdx = j1;
                            bestDiff = d1;
                        }
                    }
                } while (j++, j1++, (short)j < (short)num);
            }
        }
    } else {
        int ref = *params->field_c;
        Frame_482270* frame =
            GetGafFrame((unsigned short*)g_game->losTable, ref);
        // limitX and limitY are if/else statements, not ?:.
        int limitX;
        if (x + frame->width >= halfW)
            limitX = halfW - x;
        else
            limitX = frame->width;
        int limitY;
        if (y + frame->height >= halfH)
            limitY = halfH - y;
        else
            limitY = frame->height;
        int nx = x < 0 ? -x : 0;
        int ny = y < 0 ? -y : 0;
        if (ny >= limitY)
            return;
        if (nx >= limitX)
            return;
        for (int i = ny; i < limitY; i++) {
            // Named ex local: shapes the destination address.
            ByteMap_482270* ex = &((Player_482270*)params->field_0)->grid;
            unsigned char* dst = ex->data + (y + i) * ex->size.width + nx + x;
            unsigned char* src = frame->data + i * frame->width + nx;
            // Counted j loop: lets the compiler turn it into dec/jne.
            for (int j = nx; j < limitX; j++) {
                if (*src != frame->mask)
                    (*dst)++;
                dst++;
                src++;
            }
        }
    }
}
