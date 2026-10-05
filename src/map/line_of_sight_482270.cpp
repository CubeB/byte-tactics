// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, deepseek-v4.1-flash, GPT-6, GPT-6.1-sol, deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.
// Space Bunny Free: MATCH (was 80.6%).
// The break came from the matched twin 0x481d50 (the same line-of-sight update
// with a decrement instead of an increment): copying its source shape fixed
// five things at once.
// (1) The x/y frame transposition was never a declaration-order question. The
// x/y slots follow from the whole local set, and this source's extra locals (an
// `idx` temporary, the ternary limits, direct `player->grid.cells` /
// `grid.width` pointer arithmetic, and `src` declared before `dst`) had shifted
// the allocator's order. Removing them puts x back at 0x1c and y at 0x20, and
// every other slot on the original's offset.
// (2) The lod clamp is the ternary written inline as the argument of
// GetLosTable, with no `idx` local; the spill to 0x34 is then the same.
// (3) limitX/limitY as `if (c) v = a; else v = b;` statements (not `?:`), and
// the byte map reached through a named `ByteMap_482270* ex` local inside the
// loop: that is what gives `mov edx,[ebx]; add edx,0x7c; imul ecx,[edx+4]`
// and with it the whole `add reg,0x7c` grid-accessor shape in the destination
// address. Declaring `dst` before `src` then no longer reshuffles the frame (it
// did before, which is what every earlier attempt ran into), and the loop body,
// the loop-head reloads and the `limitX - nx` counter recomputed in esi all
// land on the original's code.
// (4) The inner loop is `for (int j = nx; j < limitX; j++)`; MSVC proves
// limitX - nx > 0 from the two guards and rewrites it to `dec esi; jne`.
// (5) The inner lod loop is `int bestIdx = 0; int j1; int bestDiff = -1;
// short j = 0;` with `if ((short)num > 0) { j1 = 1; do {...} while (j++, j1++,
// (short)j < (short)num); }`, and the neighbour coordinates are the int locals
// `dx`/`dy` used through `(short)`. That declaration order puts bestDiff in eax
// for `bestDiff * j1` (the imul operand order no source spelling could move on
// its own) and gives dx/dy slots 0x24/0x14.
// That left four bytes: the original computes `i * frame->width` as
// `mov dx,[ebp]; imul edx, eax`, with the 16-bit width as the imul destination
// and the loop counter as the source, while this source copied the width into
// esi first (`mov esi,edx; mov edx,eax; imul edx,esi`), i.e. MSVC's canonical
// operand order for the commutative multiply was the other way round. Source
// operand order (`i * frame->width` vs `frame->width * i`), casts on either
// side, a named offset temporary, a hoisted width local, `src += nx` as its own
// statement, `i` declared outside the loop and a 16-bit cast on `i` were all
// tried and none flips it. `<math.h>` (used for nothing here) does: it is
// pure compiler state, it moves the value numbering enough to canonicalise the
// multiply the other way, and with it the function matches byte for byte.
// Not used by the code; MSVC 5's value numbering with it in scope
// canonicalises `i * frame->width` the way the original does (see above).
#include <math.h>
#include <windows.h>

#pragma pack(push, 1)

class Class_00433500 {
public:
    void* GetLosTable(int n);
};

class Class_00433520 {
public:
    short GetLosTableCount();
};

class Class_004335c0 {
public:
    short GetLosLineCount();
};

class Class_4335e0 {
public:
    void* GetLosLine(short i);
};

class Class_004339c0 {
public:
    short GetLosLineStepCount();
};

class Class_004339e0 {
public:
    void GetLosLineStep(short i, int* a, int* b);
};

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
        void* table = ((Class_00433500*)g_losTables)
                          ->GetLosTable(
                              (params->field_8 / 32 < 0 ? 0 : params->field_8 / 32)
                                      < ((Class_00433520*)g_losTables)
                                            ->GetLosTableCount() - 1
                                  ? (params->field_8 / 32 < 0 ? 0 : params->field_8 / 32)
                                  : ((Class_00433520*)g_losTables)
                                        ->GetLosTableCount() - 1);
        short count = ((Class_004335c0*)table)->GetLosLineCount();
        short i = 0;
        ((Player_482270*)params->field_0)->grid.at(x, y)++;
        int ref = *params->field_c;
        for (i = 0; i < count; i++) {
            void* line = ((Class_4335e0*)table)->GetLosLine(i);
            short num = ((Class_004339c0*)line)->GetLosLineStepCount();
            int bestIdx = 0;
            int j1;
            int bestDiff = -1;
            short j = 0;
            if ((short)num > 0) {
                j1 = 1;
                do {
                    int dx;
                    int dy;
                    ((Class_004339e0*)line)->GetLosLineStep(j, &dx, &dy);
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
            ByteMap_482270* ex = &((Player_482270*)params->field_0)->grid;
            unsigned char* dst = ex->data + (y + i) * ex->size.width + nx + x;
            unsigned char* src = frame->data + i * frame->width + nx;
            for (int j = nx; j < limitX; j++) {
                if (*src != frame->mask)
                    (*dst)++;
                dst++;
                src++;
            }
        }
    }
}
