// Decompiled by deepseek-v4.1-flash. Names are provisional.
// STATUS: partial, 45.3% (1422 bytes vs the original 1519). Two things moved it
// from 27.4%: (1) the main loop's odd order, inside `if (block > -1)` the q
// value is applied to the PREVIOUS iteration's p1/p2 first, then p1/p2 are
// recomputed, then cellval is applied to the new p1/p2; on a block <= -1
// iteration the q step is skipped and cellval hits the stale pointers, which is
// why the original carries them in esi/edi; (2) cellval declared `int` rather
// than `unsigned char`, so it stays in bl (`xor ebx,ebx; mov bl,[..]`) instead
// of being spilled. max()/min() are the windows.h macros; their double
// evaluation gives the original's reload in the min case.
// Remaining diff: frame is 0x1c against the original 0x18. v3 still reserves a
// stack slot for cellval (spilled at the `v+31` temporary because ebp holds
// g_game there, where the original clobbers ebp) and the four border-flag loops
// at +0x11a..+0x245 use a different register rotation than the original.
// Next step: free ebp before the division (or get cellval's slot dropped) to
// reach the 0x18 frame, then align the border loops.
#include <new.h>
#include <windows.h>

#pragma pack(push, 1)

struct Grid_482c20 {
    unsigned char* cells;               // +0x0
    int width;                          // +0x4
    int height;                         // +0x8
    int field_c;                        // +0xc
};

struct Rec_482c20 {
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
    char unknown_14280;
    unsigned char field_14281;          // +0x14281
    char unknown_14282[0x14287 - 0x14282];
    unsigned char* cells_14287;         // +0x14287
    char unknown_1428b[0x1428f - 0x1428b];
    Grid_482c20 grid1;                  // +0x1428f
    Grid_482c20 grid2;                  // +0x1429f
    int field_142af;                    // +0x142af
    int field_142b3;                    // +0x142b3
    Rec_482c20* field_142b7;            // +0x142b7
};

#pragma pack(pop)

extern Game_482c20* g_game;

// FUNCTION: 0x482c20
void FUN_00482c20(void)
{
    Rec_482c20* rec = (Rec_482c20*)operator new(10);
    if (rec != 0) {
        rec->a = 0;
        rec->b = 0;
        rec->flags = 0;
        rec->field_6 = 0;
    } else {
        rec = 0;
    }
    g_game->field_142b7 = rec;
    g_game->field_142b7->flags = 0x1f;

    Grid_482c20* grid2 = &g_game->grid2;
    int b = g_game->field_14227 * 0x10000;
    int a = g_game->field_14223 * 0x10000;
    g_game->field_142b3 = b;
    int h2 = (b + 0x7fffff) >> 0x17;
    int w2 = (a + 0x7fffff) >> 0x17;
    g_game->field_142af = a;
    grid2->width = w2;
    grid2->height = h2;
    operator delete(grid2->cells);
    int count2 = (h2 * w2 + 7) & 0xfffffff8;
    grid2->field_c = count2;

    unsigned char* cells2;
    if (count2 == 0 || (cells2 = (unsigned char*)operator new(count2 * 10)) == 0) {
        cells2 = 0;
    } else {
        Rec_482c20* q = (Rec_482c20*)cells2;
        for (int z = count2; z > 0; z--) {
            q->a = 0;
            q->b = 0;
            q->flags = 0;
            q->field_6 = 0;
            q++;
        }
    }
    grid2->cells = cells2;

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
        unsigned char* cellp = g_game->cells_14287;
        unsigned char* recp = cells2;
        for (int y = 0; y < g_game->height; y++) {
            for (int x = 0; x < g_game->width; x++) {
                if (cellp[5] > recp[0])
                    recp[0] = cellp[5];
                if ((x & 7) == 7)
                    recp += 10;
                cellp += 0xd;
            }
            if ((y & 7) == 7)
                recp += grid2->width * 10;
        }
    }

    for (unsigned int r = 0; r < (unsigned int)grid2->height; r++) {
        unsigned char* p = (unsigned char*)grid2->cells + r * grid2->width * 10;
        unsigned char prev = 0;
        for (unsigned int fr = 1; fr < (unsigned int)grid2->width; fr++) {
            unsigned char m = p[0];
            unsigned char t = p[10];
            if (t > m)
                m = t;
            unsigned char res = prev;
            if (m > prev)
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
            unsigned char m = p[1];
            unsigned char t = p[grid2->width * 10 + 1];
            if (t > m)
                m = t;
            unsigned char res = prev;
            if (m > prev)
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
    for (int hf = 0; hf < count1; hf++) {
        cells1[hf * 2] = 0;
        cells1[hf * 2 + 1] = 0xff;
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
                        && (unsigned int)block < (unsigned int)grid1->height)
                    p1 = grid1->cells + (block * grid1->width + t20) * 2;
                else
                    p1 = 0;
                if (t20 != t24
                        && (unsigned int)t24 < (unsigned int)grid1->width
                        && (unsigned int)block < (unsigned int)grid1->height)
                    p2 = grid1->cells + (block * grid1->width + t24) * 2;
                else
                    p2 = 0;
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
