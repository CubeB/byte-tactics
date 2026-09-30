// Decompiled by deepseek-v4.1-flash, edited by deepseek-v4.1, re-tried by
// space-bunny-free. Names are provisional.
// Partial, 79.4 percent (824 bytes vs 821; the file was already at 79.1 when
// this attempt started, not the 53.5 the packet says). This attempt's one fix,
// worth 0.3, is the inner loop's shape:
//   the original materialises `j1 = 1` in the loop PREHEADER, below the
//   `jle` that guards the loop (`test ax,ax / jle / mov [esp+0x10],1 / jmp`),
//   while every other initialiser (bestIdx, bestDiff, j) is stored above it.
//   That only happens if the source puts the `j1 = 1` inside the guarded
//   region, so the inner loop must be `if ((short)num > 0) { int j1 = 1;
//   do { ... } while (j++, j1++, (short)j < (short)num); }`: the `if` gives
//   the guard, the comma expression in the controlling expression keeps the
//   two increments in the bottom block AND keeps `continue` jumping to them
//   (a do-while with the increments in the body would skip them). A nested
//   -if spelling with the increments in the body scores the same 79.4.
// Still different, in order of size:
//   1. the x/y frame slots are swapped: original x 0x1c, y 0x20, ours y 0x1c,
//      x 0x20. This is the lod/ebp story from the previous notes and it is
//      NOT the declaration order: declaring y before x scores 79.1, i.e. the
//      pair moves together or not at all.
//   2. `bestDiff * j1` is built `mov eax,[0x2c] / imul eax,[0x10]` in the
//      original and `mov eax,[0x10] / imul eax,[0x2c]` here, and the source
//      operand order (`j1 * bestDiff`, either or both compares) does not move
//      it, so MSVC5 canonicalises the multiply and this is allocator state.
//   3. the whole else branch: limitX/limitY land in 0x34/0x30 here and
//      0x18/0x34 in the original, `n = limitX - nx` is hoisted here (its own
//      slot 0x44) and computed inside the row loop there, nx/ny are swapped
//      (original: edi = nx, eax = ny = the row index; ours: eax = nx, edi =
//      the row index), the two row guards are in the other order, and the map
//      base is reached as `mov edx,[ebx] / add edx,0x7c / imul ecx,[edx+4]`
//      in the original but as two separate loads `mov ebx,[esi+0x7c] /
//      imul ecx,[esi+0x80]` here.
// Earlier attempts, all below the current best:
// The LOD-table block, from the previous session, worth 59.3 to 65.6:
//   1. the table index must be the *ternary* form of the sibling function
//      (`Lod(params) < FUN_00433520() - 1 ? Lod(params) : FUN_00433520() - 1`)
//      with the clamp inlined, not a named `lod` local. That alone made the
//      prologue, the flag block, the x/y loads and the map update match.
//   2. limitX/limitY are `(x + frame->width < halfW) ? halfW - x : frame->width`
//      and `(y + frame->height < halfH) ? halfH - y : frame->height`; with the
//      arms the other way round the branch polarity and both stores moved.
// Frame slots now agree with the target for j1 0x10, dx 0x24, ref 0x28,
// bestDiff 0x2c, grid 0x30, j/limitY 0x34, table 0x38, line 0x3c, num 0x40 and
// halfH/count 0x44. Still different (original -> ours): dy 0x14 -> 0x18,
// i/limitX 0x18 -> 0x1c, x 0x1c -> 0x20, y 0x20 -> 0x14 (y is the only variable
// out of order in the allocator's list; swapping it back would fix all four),
// and the Lod clamp: the original evaluates max(field_8/32, 0) *before* the
// FUN_00433520 call and keeps it in a slot (`mov [esp+0x34], ecx` at 0x481e05,
// `mov eax, [esp+0x34]` at 0x481e16) so y stays in ebp (`imul edx, ebp` at
// 0x481e75); ours hoists only the raw division into ebp across the call and
// clamps after it (`test ebp,ebp; setl dl`), so y is memory resident and the
// x/y/dy/i slots shift. The else branch also differs: ours hoists
// `n = limitX - nx` out of the outer loop (with its own slot 0x44), keeps
// limitX at 0x34 and limitY at 0x30, has nx/ny in eax/edi where the original
// has edi/eax, and loads the explored base with [reg+0x7c] displacements where
// the original materialises `add edx, 0x7c`.
// Tried and rejected this session: `int i` for the grid-loop cursor (60.4),
// `int j` (33.0), `#include <math.h>` (62.5), `#include <string.h>` (no change),
// reversed condition `Count() - 1 > Lod(params)` (identical bytes), subscript
// form `&explored.data[...]` for dst (identical bytes), reversed compare
// `bestDiff * j1 < d0 * bestIdx` (64.9), named `lod` reused in the then-arm
// (63.8, 798 bytes), dst declared before src in the else branch (32.8: it
// reshuffles the whole prologue, g_game moves from ecx to edi). Earlier
// attempts, all below the current best (see the previous note in git history):
// statement-form clamp, no `= 1` on j1, ascending-slot declaration order.
// <windows.h> is required.
#include <windows.h>
#include <stdio.h>

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
    unsigned char& at(int x, int y) { return data[y * size.width + x]; }
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
        void* table = ((Class_00433500*)DAT_0051e6a0)
                          ->FUN_00433500(
                              (params->field_8 / 32 < 0 ? 0 : params->field_8 / 32) <
                                      ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1
                                  ? (params->field_8 / 32 < 0 ? 0 : params->field_8 / 32)
                                  : ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1);
        short count = ((Class_004335c0*)table)->FUN_004335c0();
        short i = 0;
        ((Map_00481d50*)params->field_0)->explored.at(x, y)--;
        int ref = *params->field_c;
        for (i = 0; i < count; i++) {
            void* line = ((Class_4335e0*)table)->FUN_004335e0(i);
            short num = ((Class_004339c0*)line)->FUN_004339c0();
            int bestDiff = -1;
            int bestIdx = 0;
            short j = 0;
            if ((short)num > 0) {
                int j1 = 1;
                do {
                    int dx;
                    int dy;
                    ((Class_004339e0*)line)->FUN_004339e0(j, &dx, &dy);
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
                        ((Map_00481d50*)params->field_0)
                            ->explored.at((short)dx, (short)dy)--;
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
        Frame_00481d50* frame =
            FUN_004b7f30((unsigned short*)g_game->losTable, ref);
        int limitX = (x + frame->width < halfW) ? halfW - x : frame->width;
        int limitY = (y + frame->height < halfH) ? halfH - y : frame->height;
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
                dst++;
                src++;
            } while (--n);
        }
    }
}
