// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, re-tried by
// deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, edited
// by deepseek-v4.1, edited by space-bunny-free, finished by
// deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by
// Space Bunny Free. Names are provisional.
// MATCH, from 83.3%. Four changes did it, and the last one is the interesting
// one: the whole fix is about WHERE the locals sit in the function's single
// allocation table, not about their order relative to the code that uses them.
//  1. THE INNER LOOP IS THE MATCHED SIBLING 0x481d50'S LOOP, VERBATIM.  That
//     function MATCHes and its loop is the guarded do-while with `int j1;`
//     DECLARED BEFORE `int bestDiff = -1;` and `j1 = 1;` ASSIGNED inside the
//     guard.  The earlier notes here tried the do-while with `int j1 = 1;`
//     after bestDiff (79.1%) and concluded the `bestDiff * j1` imul operand
//     order was not worth chasing.  It is: 0x481d50's declaration order
//     (bestIdx, j1, bestDiff, j) fixes that operand order too, 79.1 -> 79.7.
//     The rule its header records holds here: MSVC takes the imul REGISTER
//     operand from declaration order, so the variable declared first is the one
//     loaded into eax.  Neither half helps alone.
//  2. THE ELSE BRANCH'S LOCALS MUST BE DECLARED AT THE TOP OF THE FUNCTION AND
//     ASSIGNED IN THE BRANCH, not initialised in place.  That took the size
//     from 1055 back to 1052.  So the 3 bytes the do-while appeared to cost are
//     not a cost of the loop: they are paid only while the else branch's
//     locals are initialised in place.
//  3. x AND y MUST BE IN THAT TOP BLOCK TOO.  This is the big one and it had
//     never been tried: adding `int x, y;` to the same declaration block and
//     assigning them after bit/halfW/halfH took the score from 82.1 to 92.3 and
//     126 diff lines to 53.  Putting x and y in the function's single local
//     table is what finally gives the allocator the register pressure shape the
//     original has, and with it BOTH long-standing problems disappear at once:
//     the first visibility cell's in-place `imul edi, [esp+0x10]` (the product
//     lands in halfW's own register) with the 5-byte `mov eax, [g_game]`, and
//     every jump target after it.  This is the "add or remove one local from
//     the allocator's table" lead the earlier notes kept circling; the real form
//     of it is "put x and y in the table too".
//  4. THE LAST 7.7 POINTS WERE `changed` MERGED INTO `int x, y, changed = 0;`
//     AND `frame` MOVED DOWN PAST `bit`.  Both are pure declaration moves and
//     both are needed: the exact position of `frame` in the table is what puts
//     it in grid's slot (esp+0x24) with the LOS frame pointer in ecx instead of
//     its own slot with the pointer in edx.  Before this the two branches'
//     locals shared slots in the wrong pairing (`grid+limitX`, `frame+?`) and
//     every reload in the else branch moved with it; with it they share the
//     original's six pairs.  Found by tools/permute.py as two `move_decl`
//     mutations off the 92.3% base, then written up here by hand.
// The order inside the top block is otherwise free, but `x, y` must come
// before it, and `frame` must come after `bit`.  Dropping x or y from the block
// drops the score to 82.1; `changed` may sit before or after `int x, y;`.
// One more thing is load bearing and looks wrong: in the row loop the
// `dst` declaration must come BEFORE the `src` one, the reverse of the order
// the values are used in.  Swapping the two back costs 7.7 points, because the
// visibility-mask pointer is loaded into the register the frame pointer wants
// and MSVC has to keep both live one instruction longer.  This is the same
// `dst++; src++;` order the earlier notes recorded for the two pointer bumps,
// and it is the same reason: the statement order is what decides which
// register is free when.
// The general lesson, worth having in the guide: when a function is close and
// the residue is register allocation, look at WHERE the locals sit in the
// function's single allocation table, not only at the order of the statements
// that use them.  Moving two ordinary locals into a top-of-function block took
// this one from 82.1% to 92.3% and the last declaration move took it to MATCH.
// <windows.h> is required.
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
    void GetLosLineStep(short i, unsigned short* a, unsigned short* b);
};

extern char g_losTables[];

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

struct Game {
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

extern Game* g_game;

Frame_00481930* __stdcall GetGafFrame(LosTable_00481930* table, int index);

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
    int x, y;
    int limitX, limitY, nx, ny;
    int i, stride, off;
    unsigned int bit = 1 << params->field_0->field_146;
    Frame_00481930* frame;
    int halfW = g_game->width / 2;
    int halfH = g_game->height / 2;
    x = params->field_4[0];
    y = params->field_4[1];
    if (g_game->flag2 == 1) {
        Grid_00481930* grid = &g_game->grid1;
        if ((unsigned)x < grid->width && (unsigned)y < grid->height) {
            void* table =
                ((Class_00433500*)g_losTables)
                    ->GetLosTable(
                        (params->field_8 / 32 < 0 ? 0 : params->field_8 / 32) <
                                ((Class_00433520*)g_losTables)->GetLosTableCount() - 1
                            ? (params->field_8 / 32 < 0 ? 0 : params->field_8 / 32)
                            : ((Class_00433520*)g_losTables)->GetLosTableCount() - 1);
            short count = ((Class_004335c0*)table)->GetLosLineCount();
            unsigned short* cell = &g_game->visibilityMask[halfW * y + x];
            if ((unsigned short)(bit & *cell) == 0) {
                *cell ^= bit;
                changed = 1;
            }
            int ref = *params->field_c;
            for (short i = 0; (short)i < count; i++) {
                void* line = ((Class_4335e0*)table)->GetLosLine(i);
                short num = ((Class_004339c0*)line)->GetLosLineStepCount();
                int bestIdx = 0;
                int j1;
                int bestDiff = -1;
                short j = 0;
                if ((short)num > 0) {
                    j1 = 1;
                    do {
                        int y2, x2;
                        ((Class_004339e0*)line)->GetLosLineStep((short)j, (unsigned short*)&x2, (unsigned short*)&y2);
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
                        j++;
                        j1++;
                    } while ((short)j < (short)num);
                }
            }
        }
    } else {
        int lod = LodRaw_00481930(params) - 5;
        if (lod < 0)
            lod = 0;
        else if (lod >= g_game->losTable->count)
            lod = g_game->losTable->count - 1;
        frame = GetGafFrame(g_game->losTable, lod);
        limitX = (x + frame->width < halfW) ? frame->width : halfW - x;
        limitY = (y + frame->height >= halfH) ? halfH - y : frame->height;
        nx = x < 0 ? -x : 0;
        ny = y < 0 ? -y : 0;
        changed = 0;
        i = ny;
        if (i < limitY) {
            stride = halfW * 2;
            off = ((y + ny) * halfW + nx + x) * 2;
            do {
                unsigned short* dst =
                    (unsigned short*)((unsigned char*)g_game->visibilityMask + off);
                unsigned char* src = frame->data + i * frame->width + nx;
                if (nx < limitX) {
                    int n = limitX - nx;
                    do {
                        if (*src != frame->mask && (unsigned short)(bit & *dst) == 0) {
                            changed = 1;
                            *dst ^= bit;
                        }
                        dst++;
                        src++;
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
