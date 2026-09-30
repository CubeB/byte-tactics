// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 73.2% (1525 vs 1519 bytes). Layout follows the matched 0x41d920:
// grid1 at +0x1428f and grid2 at +0x1429f (the +0x10/+0x14 words are the
// separate g_game fields 0x142af/0x142b3). The function allocates both
// passability grids, marks their borders, propagates row and column maxima
// twice, then fills the smoothed values.
// 64.1% -> 73.2%: the passability pass re-applies q to the freshly assigned
// p1/p2 cells inside the valid-range branch, not just to the cells carried
// over from the previous row. Adding `p1[0] = max(p1[0], q); p1[1] =
// min(p1[1], q);` (and the p2 twin) after each pointer assignment restored
// the original's second max/min pair. That also released the register
// pressure that had spilled `cellval` to the stack, so the frame is now the
// original 0x18 and `idiv` uses ebp.
// What still differs:
//  - loop counter slots are permuted: ours has outer=[esp+0x1c],
//    accum=[esp+0x14], cells2/grid1=[esp+0x10]; the original has
//    outer=[esp+0x10], cells2/grid1=[esp+0x14], accum=[esp+0x1c]. Hoisting
//    `outer` to function scope and reordering the accum/p1/p2 declarations
//    did not move them (all scored a flat 73.2), so this is not a plain
//    declaration-order lever.
//  - the outer loop carries an extra `jmp`/`xor esi,esi` at its head where
//    the original zeroes p1 at the loop bottom.
//  - the two max/min fill loops keep the same instruction sequence but with
//    `prev`/`res`/`m` in different registers (ours prev in dl, original
//    prev in cl with a bl copy of it).
//  - the grid2 setup swaps which of field_14227/field_14223 lands in edi
//    versus eax, and the fourth border loop uses a different induction
//    register.
// Earlier steps: 61.4% removed a redundant `row` local and stored
// grid2->field_10 before field_14; 64.1% wrote
// `grid2->cells = cond ? new Rec[n] : 0;` then read it back.
#include <new.h>
#include <windows.h>

#pragma pack(push, 1)

struct Grid_482c20 {
    unsigned char* cells;               // +0x0
    int width;                          // +0x4
    int height;                         // +0x8
    int field_c;                        // +0xc
};

struct Grid2_482c20 {
    unsigned char* cells;               // +0x0
    int width;                          // +0x4
    int height;                         // +0x8
    int field_c;                        // +0xc
    int field_10;                       // +0x10
    int field_14;                       // +0x14
};

struct Rec_482c20 {
    Rec_482c20() { a = 0; b = 0; flags = 0; field_6 = 0; }
    unsigned char a;                    // +0x0
    unsigned char b;                    // +0x1
    int flags;                          // +0x2
    int field_6;                        // +0x6
};

struct Game_482c20 {
    char unknown_0[0x14223];
    int field_14223;                    // +0x14223
    int field_14227;                    // +0x14227
    char unknown_1422b[0x14233 - 0x1422b];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char field_1427f;          // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    unsigned char* cells_14287;         // +0x14287
    char unknown_1428b[0x1428f - 0x1428b];
    Grid_482c20 grid1;                  // +0x1428f
    Grid2_482c20 grid2;                 // +0x1429f
    Rec_482c20* field_142b7;            // +0x142b7
};

#pragma pack(pop)

extern Game_482c20* g_game;

// FUNCTION: 0x482c20
void FUN_00482c20(void)
{
    Rec_482c20* rec = new Rec_482c20;
    g_game->field_142b7 = rec;
    g_game->field_142b7->flags = 0x1f;

    Grid2_482c20* grid2 = &g_game->grid2;
    int b = g_game->field_14227 * 0x10000;
    int a = g_game->field_14223 * 0x10000;
    grid2->field_14 = b;
    grid2->field_10 = a;
    int h2 = (b + 0x7fffff) >> 0x17;
    int w2 = (a + 0x7fffff) >> 0x17;
    grid2->width = w2;
    grid2->height = h2;
    operator delete(grid2->cells);
    int count2 = (h2 * w2 + 7) & 0xfffffff8;
    grid2->field_c = count2;

    grid2->cells = count2 != 0 ? (unsigned char*)new Rec_482c20[count2] : 0;
    unsigned char* cells2 = grid2->cells;
    unsigned char* cellp = g_game->cells_14287;

    for (unsigned int lb1 = 0; lb1 < (unsigned int)grid2->width; lb1++)
        ((Rec_482c20*)grid2->cells)[lb1].flags |= 1;
    for (unsigned int lb2 = 0; lb2 < (unsigned int)grid2->width; lb2++)
        ((Rec_482c20*)grid2->cells)[(grid2->height - 1) * grid2->width + lb2].flags |= 2;
    for (unsigned int lb3 = 0; lb3 < (unsigned int)grid2->height; lb3++)
        ((Rec_482c20*)grid2->cells)[lb3 * grid2->width].flags |= 4;
    for (unsigned int lb4 = 0; lb4 < (unsigned int)grid2->height; lb4++)
        ((Rec_482c20*)grid2->cells)[(lb4 + 1) * grid2->width - 1].flags |= 8;

    for (int d = 0; d < grid2->field_c; d++)
        grid2->cells[d * 10] = g_game->field_1427f;

    {
        for (int y = 0; y < g_game->height; y++) {
            unsigned char* recp = cells2;
            for (int x = 0; x < g_game->width; x++) {
                if (cellp[5] > recp[0])
                    recp[0] = cellp[5];
                if ((x & 7) == 7)
                    recp += 10;
                cellp += 0xd;
            }
            if ((y & 7) == 7)
                cells2 += grid2->width * 10;
        }
    }

    for (unsigned int r = 0; r < (unsigned int)grid2->height; r++) {
        unsigned char* p = (unsigned char*)grid2->cells + r * grid2->width * 10;
        unsigned char prev = 0;
        for (unsigned int fr = 1; fr < (unsigned int)grid2->width; fr++) {
            unsigned char t = p[10];
            unsigned char m = p[0];
            if (m <= t)
                m = t;
            unsigned char res = prev;
            if (prev <= m)
                res = m;
            p[1] = res;
            prev = m;
            p += 10;
        }
        p[1] = prev;
    }

    for (unsigned int i = 0; i < (unsigned int)grid2->width; i++) {
        unsigned char* p = (unsigned char*)grid2->cells + i * 10;
        unsigned char prev = 0;
        for (unsigned int gc = 1; gc < (unsigned int)grid2->height; gc++) {
            unsigned char t = p[grid2->width * 10 + 1];
            unsigned char m = p[1];
            if (m <= t)
                m = t;
            unsigned char res = prev;
            if (prev <= m)
                res = m;
            p[1] = res;
            prev = m;
            p += grid2->width * 10;
        }
        p[1] = prev;
    }

    Grid_482c20* grid1 = &g_game->grid1;
    int h1 = g_game->height / 2;
    int w1 = g_game->width / 2;
    grid1->width = w1;
    grid1->height = h1;
    operator delete(grid1->cells);
    int count1 = (h1 * w1 + 7) & 0xfffffff8;
    grid1->field_c = count1;
    unsigned char* cells1;
    if (count1 == 0)
        cells1 = 0;
    else
        cells1 = (unsigned char*)operator new(count1 * 2);
    grid1->cells = cells1;
    for (int hf = 0; hf < grid1->field_c; hf++) {
        grid1->cells[hf * 2] = 0;
        grid1->cells[hf * 2 + 1] = 0xff;
    }

    for (int outer = 0; outer < g_game->width; outer++) {
        int t20 = (outer - 1) >> 1;
        int t24 = outer >> 1;
        unsigned char* p1 = 0;
        unsigned char* p2 = 0;
        int accum = 0;
        for (int inner = 0; inner < g_game->height; inner++) {
            int idx = g_game->width * inner + outer;
            int cellval = g_game->cells_14287[idx * 0xd + 4];
            int v = accum - (cellval >> 1);
            int block = v >> 5;
            if (block > -1) {
                int q = ((block * 32 + 31) * cellval) / (v + 31);
                if (p1) {
                    p1[0] = max(p1[0], q);
                    p1[1] = min(p1[1], q);
                }
                if (p2) {
                    p2[0] = max(p2[0], q);
                    p2[1] = min(p2[1], q);
                }
                if ((unsigned int)t20 < (unsigned int)grid1->width
                        && (unsigned int)block < (unsigned int)grid1->height) {
                    p1 = grid1->cells + (block * grid1->width + t20) * 2;
                    p1[0] = max(p1[0], q);
                    p1[1] = min(p1[1], q);
                } else {
                    p1 = 0;
                }
                if (t20 != t24
                        && (unsigned int)t24 < (unsigned int)grid1->width
                        && (unsigned int)block < (unsigned int)grid1->height) {
                    p2 = grid1->cells + (block * grid1->width + t24) * 2;
                    p2[0] = max(p2[0], q);
                    p2[1] = min(p2[1], q);
                } else {
                    p2 = 0;
                }
            }
            if (p1) {
                p1[0] = max(p1[0], cellval);
                p1[1] = min(p1[1], cellval);
            }
            if (p2) {
                p2[0] = max(p2[0], cellval);
                p2[1] = min(p2[1], cellval);
            }
            accum += 0x10;
        }
    }

    int def = g_game->field_1427f;
    for (int jf = 0; jf < grid1->field_c; jf++) {
        unsigned char* p = grid1->cells + jf * 2;
        int aa = p[0];
        int bb = p[1];
        int q1 = (aa * 2 + bb) / 3;
        int q2 = (aa + bb * 2) / 3;
        if (q1 <= def)
            q1 = def;
        p[0] = q1;
        if (q2 <= def)
            q2 = def;
        p[1] = q2;
    }
}
