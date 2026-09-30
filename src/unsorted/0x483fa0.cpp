// Decompiled by deepseek-v4.1-flash, finished by GPT-6, retried by deepseek-v4.1-flash. Names are provisional.
// Best 53.1% (1037 bytes against 1046). Frame now matches at 0x48 once the
// Bitmap sprite struct gets its 4-byte tail field (unknown_14), which was the
// missing dword. What still differs: the original spills px to [esp+0x14] and
// RELOADS it after computing py, while MSVC keeps px in eax and consumes it for
// rx immediately; the slot layout is therefore shifted (ours px at 0x18, w2 at
// 0x2c). Forcing the px,py,rx,ry source order makes the schedule worse (51.5),
// so the residual is register pressure from a live value we do not model, not
// the statement order. Call census matches: 4x FUN_004b8150 + 1x FUN_004c6e70.
// The fourth saved-register push occurs after the viewport-origin stores.
// Both origins are initialized; preserve X across the second edge loop.
// Edge loops guard nonpositive counts and game state reloads after drawing.
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
#pragma pack(pop)

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
    int unknown_14;
};

extern Game_00483fa0* g_game;

void __stdcall FUN_004b8150(void* dst, Bitmap_00483fa0* bmp, int x, int y);
void __stdcall FUN_004c6e70(void* dst, int x, int y, unsigned char* pix);

// FUNCTION: 0x483fa0
void __stdcall FUN_00483fa0(void* surface)
{
    int ax = g_game->viewX;
    int scrollX = g_game->scrollX;
    int rowBase = g_game->viewY;
    int scrollY = g_game->scrollY;
    int viewW = g_game->viewW;
    int viewH = g_game->viewH;

    int px = scrollX / 32;
    int rx = scrollX - px * 32;
    int py = scrollY / 32;
    int ry = scrollY - py * 32;
    int w1 = (viewW + rx) / 32;
    int w2 = (viewH + ry) / 32;
    int rem1 = viewW - w1 * 32 + rx;
    int rem2 = viewH - w2 * 32 + ry;
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
        int y = rowBase;
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
        } while (--n != 0);
    }

    if (ry != 0 || rem2 != 0) {
        unsigned short* p1 = g_game->mapValues + py * stride + px;
        unsigned short* p2 = g_game->mapValues + (py + w2 - 1) * stride + px;
        int n = w1;
        int x = ax;
        if (n > 0) do {
            if (ry != 0) {
                bmp.data = g_game->iconSet->data + *p1 * 0x400;
                FUN_004b8150(surface, &bmp, x - rx, rowBase - ry);
            }
            if (rem2 != 0) {
                bmp.data = g_game->iconSet->data + *p2 * 0x400;
                FUN_004b8150(surface, &bmp, x - rx, w2 * 32 + rowBase - ry - 32);
            }
            p1++;
            p2++;
            x += 32;
        } while (--n != 0);
    }

    if (rx != 0) {
        w1--;
        ax += 32 - rx;
        px++;
    }
    if (ry != 0) {
        w2--;
        rowBase += 32 - ry;
        py++;
    }
    if (rem1 != 0)
        w1--;
    if (rem2 != 0)
        w2--;

    if (w2 > 0) {
        unsigned short* p = g_game->mapValues + py * (stride & 0xffff) + px;
        int y = rowBase;
        int n = w2;
        do {
            unsigned short* q = p;
            int x = ax;
            for (int m = w1; m > 0; m--) {
                FUN_004c6e70(surface, x, y, g_game->iconSet->data + *q * 0x400);
                x += 32;
                q++;
            }
            p += stride;
            y += 32;
        } while (--n != 0);
    }
}
