// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// Space Bunny Free continued from 68.6% to 72.9% (1041 bytes against 1046).
// WHAT WORKED: an exhaustive greedy search over the ORDER of declarations and
// statements inside each block, scored with check.py. Declaration order does not
// change MSVC's frame slots by itself, but it changes how the scheduler and
// register allocator lay the code out, and three orders were worth +4.3%:
//   (a) block 3 now declares `int y = ay; unsigned short s = stride; int
//       stride2 = s * 2; int offset = (py * s + px) * 2; int n = w2;`, with n
//       LAST (it was first). That is the original's order: the original stores
//       n = w2 (mov [esp+0x34], esi) after y = ay, and only then computes s.
//       69.9% -> 71.7%.
//   (b) the `if (ry != 0)` adjust block is `w2--; ay += 32 - ry; py++;` (w2--
//       FIRST). The original emits `mov edx,[ay]; mov esi,[w2]; mov eax,0x20;
//       dec esi; sub eax,edi`, i.e. it loads w2 and decrements it between the
//       two halves of the 32 - ry arithmetic. Earlier passes tried this and it
//       lost; on top of (a) it wins. 71.7% -> 72.9%.
//   (c) block 1 declares `int n = w2; int base = ...; p1; p2; int y = ay;`
//       (n first, y last). 69.3% -> 69.9%.
// Block 2 wants `p1; p2; int x = ax; int n = w1;` (n last); the permuter's
// `p1; n; p2; x` scored the same on its own fine score but 0.1% lower here.
// Still tried and all NEUTRAL or worse: `static inline` helpers for the icon
// lookup (`g_game->iconSet->cell[v][0][0]` with the IconSet typed as
// `unsigned char (*cell)[32][32]` the way 0x466780 has it, or the flat
// `+ v * 0x400` form), a helper that assigns bmp.data and calls FUN_004b8150,
// `const` on stride/base/s/rx/ry, `void*` for bmp.data and iconSet->data, the
// callee parameter types from 0x4b8150's own file, `for`/nested-if/`while`
// rewrites of every loop, and swapping the operands of every commutative
// expression (MSVC canonicalises those, so they compile identically).
// tools/headers.py over all 128 header sets: only <windows.h> and <ddreach.h>
// reach the same 72.9%, so <windows.h> is required and no header is the lever.
// A 9 minute permuter run from the 69.3% file found nothing (score 3277 flat).
// claude-sonnet-5-5 continued: best 68.6% (1033 bytes against 1046), from
// 66.2%. Two source-order changes helped: (a) bmp.dx/bmp.dy = 0 now sit
// before the `if (rem1 != 0)` tests (the other bmp stores stay after them),
// which makes the compiler hold the zero in a register for the compares;
// (b) px, rx, py, ry are computed in that order (px, rx, py, ry), which makes
// px be stored to its slot right after the divide like the original.
// NEW FINDINGS on what still differs (all register allocation):
// * The original keeps constant 0 in ebp from the FIRST compare (`xor ebp,ebp;
// cmp ebx,ebp`) and moves rem2 to edx first. Ours keeps rem2 in ebp and the
// zero only starts after the cdq of mapWidth/2, so the first compares are
// `test`. A small test file reproduces the original behaviour (zero held
// in a register for every compare), so it is a register-pressure choice;
// variants that put vw/vh loads earlier all got worse (48 to 61%).
// * Plain `for (i = 0; i < n; i++)` loops are turned into dec/jne count-down
// loops by MSVC too, but gave 63 to 66% here (entry jmp and a spilled
// counter), do/while with `--n` stays best. Latch variants with `m = w1`
// after the inner loop (l1/l2) were size-exact (1046 bytes) but 62.3%
// because m is spilled and y takes ebp.
// * Slots: ax .20, py .24, w1 .10 match; ay/px/w2 are a rotation of
// .14/.18/.1c (original ay .1c, px .14, w2 .18). Moving statements did not
// change that.
// Structural ratio of this version against the original is 0.897; a
// permuter run from this file is the next step.
// mimo-v2.6-pro continued: best 66.2% (1031 bytes against 1046). Prior work
// took the prologue to the original's arithmetic order with int locals vw/vh
// and rebuilt block 3 with `unsigned short s = stride` (restores the and
// 0xffff mask, mov eax,edx copy and lea order), reaching 62.4%; the file now
// sits at 66.2%.
// WHAT STILL DIFFERS (all compiler scheduling/register colour, no plain source
// lever found yet):
// (1) block 3 counter home: the original keeps w1 in ebp and reloads it in the
// outer-loop LATCH (`mov ebp,[esp+0x10]` at 0x484384, after the inner loop,
// inside the if) with NO entry jmp; ours reloads w1 at the loop TOP (0x484336)
// and carries an entry `jmp 0x48433a` to skip that reload on the first pass.
// Tried: `int m=w1` before + `m=w1` inside if (variant A, 59.6%, spills m to
// eax/0x5c); `m=w1` unconditional in latch (variant B, 39.5%); `if(w1>0)` +
// `int m=w1` inside (variant E, 66.2% identical to current); uninit `int m` +
// `m=w1` in latch (variant H, 64.5%: reload DOES move to the latch at 0x484390
// but m gets a stack slot 0x5c and an extra top reload). So the latch reload
// is reachable but keeping the counter in ebp across the latch is the blocker.
// (2) slot rotation of px/w2/ay at 0x14/0x18/0x1c: ours w2 0x14, ay 0x18, px
// 0x1c vs original px 0x14, w2 0x18, ay 0x1c (ax 0x20, py 0x24, w1 0x10,
// stride 0x28, p2 0x2c, rem1 0x30, rem2 0x34, n 0x38, stride2 0x3c all match).
// Both files store ax,ay,px,py,w1,w2 in that order, so it is the frame layout
// (declaration/last-use order), not the store order. Both our order and the
// original are consistent with "ascending last use -> ascending slot" given a
// different last-use ordering, so the lever is the last-use POSITION of px/w2
// (px must end earliest, ay latest). No source rewrite moved it yet.
// (3) block 1 p1/p2: the original computes the shared offset py*stride+px in
// eax, then p2 into edx reusing mapValues (`lea edx,[edx+eax*2-2]` at 0x4840b9)
// and DELAYS the p2 store past the test (`test eax,eax; mov [esp+0x2c],edx`).
// Ours computes p2 into eax (`lea eax,[edx+eax*2-2]`), stores it before the
// test, and loads ay into ebp early (0x4840b3) where the original loads ay
// late (0x4840d1). The lea destination is a register-allocation choice driven
// by that delayed store / ay load timing.
// (4) scattered register colours from the above (w2 eax vs esi at block 3
// entry, imul edx vs esi for py, block 2 px memory fold).
#include <windows.h>

#pragma pack(push, 1)
struct IconSet_00483fa0 {
    char unknown_0[4];
    unsigned char* data;               // +0x4
};

struct Game_00483fa0 {
    char unknown_0[0x14233];
    int mapWidth;                      // +0x14233
    char unknown_14237[0x14283 - 0x14237];
    IconSet_00483fa0* iconSet;         // +0x14283
    char unknown_14287[0x1428b - 0x14287];
    unsigned short* mapValues;         // +0x1428b
    char unknown_1428f[0x1431f - 0x1428f];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x37e27 - 0x14327];
    int viewX;                         // +0x37e27
    int viewY;                         // +0x37e2b
    char unknown_37e2f[0x37e37 - 0x37e2f];
    int viewW;                         // +0x37e37
    int viewH;                         // +0x37e3b
};

struct Bitmap_00483fa0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short dx;                          // +0x4
    short dy;                          // +0x6
    unsigned char colour;              // +0x8
    unsigned char flag9;               // +0x9
    unsigned char count;               // +0xa
    unsigned char kind;                // +0xb
    int unknown_c;                     // +0xc
    unsigned char* data;               // +0x10
    int unknown_14;                    // +0x14
};
#pragma pack(pop)

extern Game_00483fa0* g_game;

void __stdcall FUN_004b8150(void* dst, Bitmap_00483fa0* bmp, int x, int y);
void __stdcall FUN_004c6e70(void* dst, int x, int y, unsigned char* pix);

// FUNCTION: 0x483fa0
void __stdcall FUN_00483fa0(void* surface)
{
    Bitmap_00483fa0 bmp;
    int ax = g_game->viewX;
    int sx = g_game->scrollX;
    int ay = g_game->viewY;
    int sy = g_game->scrollY;
    int px = sx / 32;
    int rx = sx - px * 32;
    int py = sy / 32;
    int ry = sy - py * 32;
    int vw = g_game->viewW;
    int w1 = (vw + rx) / 32;
    int vh = g_game->viewH;
    int w2 = (vh + ry) / 32;
    int rem1 = vw - w1 * 32 + rx;
    int rem2 = vh - w2 * 32 + ry;
    bmp.dx = 0;
    bmp.dy = 0;
    if (rem2 != 0)
        w2++;
    if (rem1 != 0)
        w1++;
    int stride = g_game->mapWidth / 2;
    bmp.width = 32;
    bmp.height = 32;
    bmp.flag9 = 0;
    bmp.count = 0;
    if (rx != 0 || rem1 != 0) {
        int n = w2;
        int base = py * stride + px;
        unsigned short* p1 = g_game->mapValues + base;
        unsigned short* p2 = g_game->mapValues + base + w1 - 1;
        int y = ay;
        if (n > 0) do {
            if (rx != 0) {
                bmp.data = g_game->iconSet->data + *p1 * 0x400;
                FUN_004b8150(surface, &bmp, ax - rx, y - ry);
            }
            if (rem1 != 0) {
                bmp.data = g_game->iconSet->data + *p2 * 0x400;
                FUN_004b8150(surface, &bmp, w1 * 32 + ax - rx - 32, y - ry);
            }
            p1 += stride;
            p2 += stride;
            y += 32;
        } while (--n);
    }

    if (ry != 0 || rem2 != 0) {
        unsigned short* p1 = g_game->mapValues + py * stride + px;
        unsigned short* p2 = g_game->mapValues + (py + w2 - 1) * stride + px;
        int x = ax;
        int n = w1;
        if (n > 0) do {
            if (ry != 0) {
                bmp.data = g_game->iconSet->data + *p1 * 0x400;
                FUN_004b8150(surface, &bmp, x - rx, ay - ry);
            }
            if (rem2 != 0) {
                bmp.data = g_game->iconSet->data + *p2 * 0x400;
                FUN_004b8150(surface, &bmp, x - rx, w2 * 32 + ay - ry - 32);
            }
            p1++;
            p2++;
            x += 32;
        } while (--n);
    }

    if (rx != 0) {
        w1--;
        ax += 32 - rx;
        px++;
    }
    if (ry != 0) {
        w2--;
        ay += 32 - ry;
        py++;
    }
    if (rem1 != 0)
        w1--;
    if (rem2 != 0)
        w2--;

    if (w2 > 0) {
        int y = ay;
        unsigned short s = stride;
        int stride2 = s * 2;
        int offset = (py * s + px) * 2;
        int n = w2;
        do {
            unsigned short* p = (unsigned short*)((char*)g_game->mapValues + offset);
            int m = w1;
            if (m > 0) {
                int x = ax;
                do {
                    FUN_004c6e70(surface, x, y, g_game->iconSet->data + *p * 0x400);
                    x += 32;
                    p++;
                } while (--m);
            }
            offset += stride2;
            y += 32;
        } while (--n);
    }
}