// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Best 57.9% (1048 bytes against 1046). The frame map is confirmed, not guessed: the two
// stores before `push edi` at 0x483fcc are relative to the pre-push esp, so they land at
// final-esp 0x20 (viewX/ax) and 0x1c (viewY/ay). Original final frame: w1 0x10, px 0x14,
// w2 0x18, ay 0x1c, ax 0x20, py 0x24, stride 0x28, p2 0x2c, rem1 0x30, rem2 0x34,
// n 0x38, stride2 0x3c. Ours now matches p2 0x2c, rem1 0x30, rem2 0x34, n 0x38,
// stride2 0x3c and all of the final blit loop except one reload; ours allocates
// w2 0x14, py 0x18, px 0x1c, ay 0x20, ax 0x24, stride 0x28, so only the
// px/py/w2/ay/ax block is permuted. Removing the `& 0xffff` masks on stride in the tail
// block (the original imuls the full 32-bit stride, `imul edx,[esp+0x28]`) lifted
// 57.4 -> 57.9. Tried this round: original px/py/rx/ry evaluation order (57.0%),
// function-scope p1/p2 (54.7%). MSVC 5 assigns these slots by code, not by declaration
// order (nine declaration orders left every offset unchanged, see SHARED.md), so the
// remaining gap is the allocator permutation of px/py/w2/ay/ax plus one missing
// `mov ebp, [esp+0x10]` reload after the block-3 inner loop (the original reloads w1
// there, ours keeps it in ebp).
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
    int ax = g_game->viewX;
    int sx = g_game->scrollX;
    int ay = g_game->viewY;
    int sy = g_game->scrollY;
    int px = sx / 32;
    int rx = sx - px * 32;
    int py = sy / 32;
    int ry = sy - py * 32;
    int w1 = (g_game->viewW + rx) / 32;
    int rem1 = g_game->viewW - w1 * 32 + rx;
    int w2 = (g_game->viewH + ry) / 32;
    int rem2 = g_game->viewH - w2 * 32 + ry;
    if (rem1 != 0)
        w1++;
    if (rem2 != 0)
        w2++;
    int stride = g_game->mapWidth / 2;

    Bitmap_00483fa0 bmp;
    bmp.width = 32;
    bmp.height = 32;
    bmp.dx = 0;
    bmp.dy = 0;
    bmp.flag9 = 0;
    bmp.count = 0;

    if (rx != 0 || rem1 != 0) {
        unsigned short* p1 = g_game->mapValues + py * stride + px;
        unsigned short* p2 = p1 + w1 - 1;
        int y = ay;
        int n = w2;
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
        int n = w1;
        int x = ax;
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
        int offset = (py * stride + px) * 2;
        int stride2 = stride * 2;
        int y = ay;
        int n = w2;
        do {
            unsigned short* p = (unsigned short*)((char*)g_game->mapValues + offset);
            int x = ax;
            int m = w1;
            if (m > 0) do {
                FUN_004c6e70(surface, x, y, g_game->iconSet->data + *p * 0x400);
                x += 32;
                p++;
            } while (--m);
            offset += stride2;
            y += 32;
        } while (--n);
    }
}
