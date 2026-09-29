// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Map viewport tile painter: draws the partially visible edge tiles one at a
// time through FUN_004b8150 (a 32x32 bitmap built on the stack) and the fully
// visible interior rows through FUN_004c6e70.
//
// STATUS: gave up, best 13.3%. The algorithm below is transcribed from the
// disassembly and the field map is complete, but the generated code differs
// in register allocation and frame size almost everywhere, so the diff score
// is low despite matching operations.
//
// Facts a next attempt should start from:
//  - frame is `sub esp, 0x48` (18 dwords); locals live at [esp+0x10]..[esp+0x57]
//    and [esp+0x00..0x0f] are the four saved registers. Slot [esp+0x54] is
//    reserved but never touched (the compiler's 18th dword).
//  - g_game fields: +0x1431f scrollX, +0x14323 scrollY, +0x37e27 view origin Y
//    (used as the y base), +0x37e2b (read once, stored to [esp+0x18] and then
//    overwritten with the row count, i.e. a dead store), +0x37e37 viewW,
//    +0x37e3b viewH, +0x14233 map pixel width (halved to get the map stride),
//    +0x1428b unsigned short* tile map, +0x14283 icon set whose +0x4 is the
//    8bpp tile base (tile index * 0x400 selects a 32x32 tile).
//  - the argument at [esp+0x5c] (a surface) is copied to ebx early and its
//    stack home is then reused for a tile-map pointer local, so the parameter
//    slot does double duty.
//  - rx = scrollX - (scrollX/32)*32 (the compiler emits the sub, not a `%`),
//    same for ry; w1 = ceil((viewW+rx)/32) but computed as
//    (viewW+rx)/32 and rem1 = viewW - w1*32 + rx, w1++ when rem1 != 0
//    (same for w2/rem2). rem1/rem2 select the partially visible last
//    column/row.
//  - the x accumulator ([esp+0x20]) is READ at 0x484100 and 0x4841ef before it
//    is first WRITTEN at 0x4842c9 (`ax += 32 - rx`): the original leaves it
//    uninitialised (real bug, see below). Writing `int ax = 0;` or
//    `int ax = g->field_37e2b;` adds a store the original does not have.
//  - the third block's row offset is `py * (stride & 0xffff) + px`, the
//    0xffff mask is real (unsigned short truncation).
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
    int viewY;                         // +0x37e27
    int field_37e2b;                   // +0x37e2b
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
};

extern Game_00483fa0* g_game;

void __stdcall FUN_004b8150(void* dst, Bitmap_00483fa0* bmp, int x, int y);
void __stdcall FUN_004c6e70(void* dst, int x, int y, unsigned char* pix);

// FUNCTION: 0x483fa0
void __stdcall FUN_00483fa0(void* surface)
{
    Game_00483fa0* g = g_game;
    int rowBase = g->viewY;
    int scrollX = g->scrollX;
    int scrollY = g->scrollY;
    int viewW = g->viewW;
    int viewH = g->viewH;

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
    int stride = g->mapWidth / 2;

    Bitmap_00483fa0 bmp;
    bmp.width = 32;
    bmp.height = 32;
    bmp.dx = 0;
    bmp.dy = 0;
    bmp.flag9 = 0;
    bmp.count = 0;

    int ax;

    if (rx != 0 || rem1 != 0) {
        unsigned short* p1 = g->mapValues + py * stride + px;
        unsigned short* p2 = p1 + w1 - 1;
        int y = rowBase;
        int n = w2;
        do {
            if (rx != 0) {
                bmp.data = g->iconSet->data + *p1 * 0x400;
                FUN_004b8150(surface, &bmp, ax - rx, y - ry);
            }
            if (rem1 != 0) {
                bmp.data = g->iconSet->data + *p2 * 0x400;
                FUN_004b8150(surface, &bmp, w1 * 32 + ax - rx - 32, y - ry);
            }
            p1 += stride;
            p2 += stride;
            y += 32;
        } while (--n != 0);
    }

    if (ry != 0 || rem2 != 0) {
        unsigned short* p1 = g->mapValues + py * stride + px;
        unsigned short* p2 = g->mapValues + (py + w2 - 1) * stride + px;
        int n = w1;
        do {
            if (ry != 0) {
                bmp.data = g->iconSet->data + *p1 * 0x400;
                FUN_004b8150(surface, &bmp, ax - rx, rowBase - ry);
            }
            if (rem2 != 0) {
                bmp.data = g->iconSet->data + *p2 * 0x400;
                FUN_004b8150(surface, &bmp, ax - rx, w2 * 32 + rowBase - ry - 32);
            }
            p1++;
            p2++;
            ax += 32;
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
        unsigned short* p = g->mapValues + py * (stride & 0xffff) + px;
        int y = rowBase;
        int n = w2;
        do {
            unsigned short* q = p;
            int x = ax;
            for (int m = w1; m > 0; m--) {
                FUN_004c6e70(surface, x, y, g->iconSet->data + *q * 0x400);
                x += 32;
                q++;
            }
            p += stride;
            y += 32;
        } while (--n != 0);
    }
}
