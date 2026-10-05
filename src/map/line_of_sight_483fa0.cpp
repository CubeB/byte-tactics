// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by opus. Names are provisional.
// MATCH. Draws the visible map tiles into `surface`: the partial tiles along
// the left/right edges and then the top/bottom edges through DrawFrameOpaque
// (a 32x32 Bitmap whose data points at the tile's icon), then the whole
// interior tiles through DrawTile.
// opus rewrote the earlier 77.2% file from scratch. What it took:
// * The edge and interior loops are index loops: the screen coordinate is
//   written from the index (`screenY + j * 32 - offY`, `screenX + i * 32`),
//   so MSVC strength-reduces it into a register and builds the down-counter
//   and the `stride * 2` step itself, in the preheader after the guard. The
//   earlier explicit `y += 32` / `n--` counters put them before the guard
//   and moved the zero register (`mov edx, ebp; xor ebp, ebp`).
// * All the prologue locals are declared together at the top and the view
//   size is loaded before the divisions; that keeps viewW/viewH in ebx/ebp
//   across them, so tileX is reloaded for offX as in the original and the
//   frame slots come out in the original's order. Declaration order still
//   matters: tilesX/tilesY must be declared before tileX/tileY (74.2%
//   otherwise) and offX/offY before viewW/viewH (about 53% otherwise).
// * The interior inner loop advances the map pointer in the `for` increment,
//   `i++, p++`, which orders the two `add`s as the original does.
#include <windows.h>

#pragma pack(push, 1)
struct IconSet_00483fa0 {
    char unknown_0[4];
    unsigned char* data;               // +0x4, 32x32 icons, 0x400 bytes each
};

struct Game {
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

extern Game* g_game;

void __stdcall DrawFrameOpaque(void* dst, Bitmap_00483fa0* bmp, int x, int y);
void __stdcall DrawTile(void* dst, int x, int y, unsigned char* pix);

// FUNCTION: 0x483fa0
void __stdcall DrawMapTiles(void* surface)
{
    int tilesX, tilesY;
    int tileX, tileY;
    int screenX, screenY;
    int offX, offY;
    int edgeX, edgeY;
    int scrollX, scrollY, viewW, viewH;
    Bitmap_00483fa0 bmp;
    screenX = g_game->viewX;
    screenY = g_game->viewY;
    scrollX = g_game->scrollX;
    scrollY = g_game->scrollY;
    viewW = g_game->viewW;
    viewH = g_game->viewH;
    tileX = scrollX / 32;
    tileY = scrollY / 32;
    offX = scrollX - tileX * 32;
    offY = scrollY - tileY * 32;
    tilesX = (viewW + offX) / 32;
    tilesY = (viewH + offY) / 32;
    edgeX = (viewW - tilesX * 32) + offX;
    edgeY = (viewH - tilesY * 32) + offY;
    if (edgeX != 0)
        tilesX++;
    if (edgeY != 0)
        tilesY++;
    int stride = g_game->mapWidth / 2;
    bmp.width = 32;
    bmp.height = 32;
    bmp.dx = 0;
    bmp.dy = 0;
    bmp.flag9 = 0;
    bmp.count = 0;

    // Left and right columns of partial tiles.
    if (offX != 0 || edgeX != 0) {
        unsigned short* left = g_game->mapValues + tileY * stride + tileX;
        unsigned short* right = g_game->mapValues + tileY * stride + tileX + tilesX - 1;
        for (int j = 0; j < tilesY; j++) {
            if (offX != 0) {
                bmp.data = g_game->iconSet->data + *left * 0x400;
                DrawFrameOpaque(surface, &bmp, screenX - offX, (screenY + j * 32) - offY);
            }
            if (edgeX != 0) {
                bmp.data = g_game->iconSet->data + *right * 0x400;
                DrawFrameOpaque(surface, &bmp, tilesX * 32 + screenX - offX - 32, (screenY + j * 32) - offY);
            }
            left += stride;
            right += stride;
        }
    }

    // Top and bottom rows of partial tiles.
    if (offY != 0 || edgeY != 0) {
        unsigned short* top = g_game->mapValues + tileY * stride + tileX;
        unsigned short* bottom = g_game->mapValues + (tileY + tilesY - 1) * stride + tileX;
        for (int i = 0; i < tilesX; i++) {
            if (offY != 0) {
                bmp.data = g_game->iconSet->data + *top * 0x400;
                DrawFrameOpaque(surface, &bmp, (screenX + i * 32) - offX, screenY - offY);
            }
            if (edgeY != 0) {
                bmp.data = g_game->iconSet->data + *bottom * 0x400;
                DrawFrameOpaque(surface, &bmp, (screenX + i * 32) - offX, tilesY * 32 + screenY - offY - 32);
            }
            top++;
            bottom++;
        }
    }

    // Leave only the whole tiles for the interior.
    if (offX != 0) {
        tilesX--;
        screenX += 32 - offX;
        tileX++;
    }
    if (offY != 0) {
        tilesY--;
        screenY += 32 - offY;
        tileY++;
    }
    if (edgeX != 0)
        tilesX--;
    if (edgeY != 0)
        tilesY--;

    for (int j = 0; j < tilesY; j++) {
        unsigned short* p = g_game->mapValues + (tileY + j) * (unsigned short)stride + tileX;
        for (int i = 0; i < tilesX; i++, p++) {
            DrawTile(surface, screenX + i * 32, screenY + j * 32, g_game->iconSet->data + *p * 0x400);
        }
    }
}
