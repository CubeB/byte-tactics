// Decompiled by GPT-5.6 Astra, finished by deepseek-v4.1-flash; verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// Debug overlay for the map: walks the visible tiles, works out the screen
// quad of each one from its four corner heights and draws what the current
// debug mode asks for (1: movement classes and path arrows, 2: tile
// contents, 3: metal values, 4: fog of war), plus the contour lines of
// 0x4181d0 when DAT_00511dd0 is set.
//
// What took it from 65.2% (with /Gi) to MATCH, without /Gi like every other
// function around it:
// * The path cell lookup is an inlined method of the path grid at
//   paths+0x1c (`add eax, 0x1c` then [eax+4] and [eax]); the same grid
//   (cells, width, ...) is Grid_0040d900 in 0x40d900.cpp. That one change put
//   the whole frame in place (y below the quad, the tile/cell slot shared).
// * `cell->flags & ~4` is tested through an `unsigned char` (byte compares
//   `and cl, 0xfb` / `cmp cl, 3`), and the tile's unit, feature and object
//   words are masked with `& 0xff` rather than cast, so the word loaded for
//   the test is reused for the argument (`mov ax, [ebx]` / `and eax, 0xff`).
// * lastX is `__min(...)` from <stdlib.h> (one store of lastX after the
//   select; an if-clamp stores it in both arms).
// * The headers: the two multiplies (`y * width` for the tile and
//   `fogWidth * (y / 2)` for the fog) take their operand order from the
//   declaration count. <stdio.h>, <math.h>, <malloc.h> and <stdlib.h> give
//   both; <stdio.h> with <ctype.h> and <malloc.h>, or with <math.h> and
//   <direct.h>, also match.
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>

struct Point_00417f60 { int x, y; };
struct Rect { int left, top, right, bottom; };

#pragma pack(push, 1)
struct Movement {
    char unknown_0[0x10];
    int width;                         // +0x10
    char unknown_14[4];
    unsigned int* states;              // +0x18, 2 bits per cell, 16 rows per word
};
struct UnitDef { char unknown_0[0x1b6]; Movement* movement; };
struct Unit { char unknown_0[0x92]; UnitDef* def; };
struct Tile {
    unsigned short unit;               // +0x0
    unsigned short feature;            // +0x2
    unsigned char height;              // +0x4
    char unknown_5[2];
    unsigned char metal;               // +0x7
    unsigned short object;             // +0x8
    char unknown_a[2];
    unsigned char flags;               // +0xc
};
#pragma pack(pop)

struct PathCell {
    unsigned char flags;
    signed char direction;
    char unknown_2[2];
};
struct PathGrid {
    PathCell* cells;                   // +0x0
    int width;                         // +0x4
    PathCell* At(int x, int y) { return &cells[width * y + x]; }
};
struct PathMap {
    char unknown_0[0x1c];
    PathGrid grid;                     // +0x1c
};

#pragma pack(push, 1)
struct Player {
    char unknown_0[0x7c];
    unsigned char* fog;                // +0x7c
    int fogWidth;                      // +0x80
    char unknown_84[0x14b - 0x84];
};
struct Game {
    char unknown_0[0xdcb];
    unsigned char colors[16];          // +0xdcb
    char unknown_ddb[0x1b63 - 0xddb];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a43 - (0x1b63 + 10 * 0x14b)];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14207 - 0x2a44];
    PathMap* paths;                    // +0x14207
    char unknown_1420b[0x14233 - 0x1420b];
    int width;                         // +0x14233
    int height;                        // +0x14237
    int viewWidth;                     // +0x1423b
    char unknown_1423f[0x1427f - 0x1423f];
    unsigned char seaLevel;            // +0x1427f
    unsigned char mode;                // +0x14280
    char unknown_14281[6];
    Tile* tiles;                       // +0x14287
    char unknown_1428b[0x1431f - 0x1428b];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x37e23 - 0x14327];
    int bottom;                        // +0x37e23
    char unknown_37e27[0x391fd - 0x37e27];
    int font;                          // +0x391fd
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_00511dd0;
extern unsigned char DAT_004fcc68[];
extern signed char DAT_004fd670[], DAT_004fd678[];

Unit* __stdcall FUN_0048c190(int, int);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, int color);
void __stdcall FUN_004c1420(int font);
int __stdcall FUN_004c13f0();
void __stdcall FUN_004c13a0(int color, int background);
void __stdcall FUN_004c14f0(void* surface, const char* text, int x, int y, int maxWidth);
void __stdcall FUN_004c0310(void* surface, Point_00417f60* points, int count, int color);
void __stdcall FUN_004bf6f0(void* surface, Rect* rect, int color);
void __stdcall FUN_004181d0(void* surface, Point_00417f60* corners, unsigned char* heights);

// FUNCTION: 0x418310
void __stdcall FUN_00418310(void* surface)
{
    if (!g_game->mode && !DAT_00511dd0) return;
    Movement* movement = 0;
    Player* player = &g_game->players[g_game->playerIndex];
    if (g_game->mode == 1) {
        Unit* unit = FUN_0048c190(0, 0);
        if (unit) movement = unit->def->movement;
    }
    int firstY = g_game->scrollY / 16;
    int firstX = g_game->scrollX / 16;
    int lastX = __min(g_game->viewWidth + firstX + 1, g_game->width - 1);
    int lastY = g_game->height - 1;
    unsigned char* colors = g_game->colors;
    unsigned char arrowColor;
    for (int y = firstY; y < lastY; ++y) {
        int offscreen = 1;
        Point_00417f60 p[4];
        unsigned char heights[4];
        for (int x = firstX; x < lastX; ++x) {
            // The four corners, walking round the cell from its own tile.
            Tile* tile = &g_game->tiles[y * g_game->width + x];
            heights[0] = tile->height;
            p[0].x = (x + 8) * 16 - g_game->scrollX;
            p[0].y = (y + 2) * 16 - (heights[0] >> 1) - g_game->scrollY;
            ++tile; ++x;
            heights[1] = tile->height;
            p[1].x = (x + 8) * 16 - g_game->scrollX;
            p[1].y = (y + 2) * 16 - (heights[1] >> 1) - g_game->scrollY;
            tile += g_game->width; ++y;
            heights[2] = tile->height;
            p[2].x = (x + 8) * 16 - g_game->scrollX;
            p[2].y = (y + 2) * 16 - (heights[2] >> 1) - g_game->scrollY;
            --tile; --x;
            heights[3] = tile->height;
            p[3].x = (x + 8) * 16 - g_game->scrollX;
            p[3].y = (y + 2) * 16 - (heights[3] >> 1) - g_game->scrollY;
            tile -= g_game->width; --y;
            if (p[0].y < g_game->bottom) offscreen = 0;
            if (g_game->mode == 1) {
                if (movement) {
                    unsigned int state = (movement->states[movement->width * (y >> 4) + x] >> ((y & 15) * 2)) & 3;
                    if (state < 3) {
                        unsigned char color = colors[DAT_004fcc68[state]];
                        FUN_004be950(surface, p[0].x, p[0].y, p[2].x, p[2].y, color);
                        FUN_004be950(surface, p[1].x, p[1].y, p[3].x, p[3].y, color);
                    }
                }
                PathCell* cell = g_game->paths->grid.At(x, y);
                if (cell->flags & 4) {
                    FUN_004c1420(g_game->font);
                    FUN_004c13a0(rand() & 255, FUN_004c13f0());
                    FUN_004c14f0(surface, "G", p[0].x, p[0].y, -1);
                }
                unsigned char kind = cell->flags & ~4;
                if (kind != 0 && kind != 3) {
                    int cx = p[0].x + 8, cy = p[0].y + 8;
                    // Flags 5 and 6 pass the test above but set no colour, so
                    // the arrow keeps the colour of the last arrow drawn.
                    switch (cell->flags) {
                    case 1: arrowColor = colors[15]; break;
                    case 2: arrowColor = colors[4]; break;
                    }
                    FUN_004be950(surface, cx - DAT_004fd670[cell->direction] * 14,
                                 cy - DAT_004fd678[cell->direction] * 14, cx, cy, arrowColor);
                    int direction = (cell->direction + 1) & 7;
                    FUN_004be950(surface, cx - DAT_004fd670[direction] * 4,
                                 cy - DAT_004fd678[direction] * 4, cx, cy, arrowColor);
                    direction = (cell->direction - 1) & 7;
                    FUN_004be950(surface, cx - DAT_004fd670[direction] * 4,
                                 cy - DAT_004fd678[direction] * 4, cx, cy, arrowColor);
                }
            } else if (g_game->mode == 2) {
                if (tile->height > g_game->seaLevel) {
                    FUN_004be950(surface, p[0].x, p[0].y, p[1].x, p[1].y, colors[15]);
                    FUN_004be950(surface, p[0].x, p[0].y, p[3].x, p[3].y, colors[15]);
                } else {
                    FUN_004be950(surface, p[0].x, p[0].y, p[1].x, p[1].y, colors[13]);
                    FUN_004be950(surface, p[0].x, p[0].y, p[3].x, p[3].y, colors[13]);
                }
                if (tile->unit) FUN_004c0310(surface, p, 4, tile->unit & 0xff);
                else if (tile->object != 0xffff) FUN_004c0310(surface, p, 4, (tile->object - 56) & 0xff);
                if (tile->feature) {
                    FUN_004be950(surface, p[0].x, p[0].y, p[2].x, p[2].y, tile->feature & 0xff);
                    FUN_004be950(surface, p[1].x, p[1].y, p[3].x, p[3].y, tile->feature & 0xff);
                }
                if (tile->flags & 2) {
                    FUN_004be950(surface, (p[0].x + p[1].x) / 2, (p[0].y + p[1].y) / 2 + 2,
                                 (p[1].x + p[2].x) / 2 - 2, (p[1].y + p[2].y) / 2, colors[15]);
                    FUN_004be950(surface, (p[1].x + p[2].x) / 2 - 2, (p[1].y + p[2].y) / 2,
                                 (p[2].x + p[3].x) / 2, (p[2].y + p[3].y) / 2 - 2, colors[15]);
                    FUN_004be950(surface, (p[2].x + p[3].x) / 2, (p[2].y + p[3].y) / 2 - 2,
                                 (p[3].x + p[0].x) / 2 + 2, (p[3].y + p[0].y) / 2, colors[15]);
                    FUN_004be950(surface, (p[3].x + p[0].x) / 2 + 2, (p[3].y + p[0].y) / 2,
                                 (p[0].x + p[1].x) / 2, (p[0].y + p[1].y) / 2 + 2, colors[15]);
                }
            } else if (g_game->mode == 3) {
                if (tile->height > g_game->seaLevel) {
                    FUN_004be950(surface, p[0].x, p[0].y, p[1].x, p[1].y, colors[15]);
                    FUN_004be950(surface, p[0].x, p[0].y, p[3].x, p[3].y, colors[15]);
                } else {
                    FUN_004be950(surface, p[0].x, p[0].y, p[1].x, p[1].y, colors[13]);
                    FUN_004be950(surface, p[0].x, p[0].y, p[3].x, p[3].y, colors[13]);
                }
                FUN_004c1420(g_game->font);
                FUN_004c13a0(colors[15], FUN_004c13f0());
                char buffer[20];
                FUN_004c14f0(surface, _itoa(tile->metal, buffer, 10), p[0].x + 2, p[0].y + 2, -1);
            } else if (g_game->mode == 4) {
                FUN_004be950(surface, p[0].x, p[0].y, p[1].x, p[1].y, colors[0]);
                FUN_004be950(surface, p[0].x, p[0].y, p[3].x, p[3].y, colors[0]);
                if (player->fog[(y / 2) * player->fogWidth + x / 2]) {
                    Rect r;
                    r.left = p[0].x - 5;
                    r.right = p[0].x + 5;
                    r.top = p[0].y - 5;
                    r.bottom = p[0].y + 5;
                    FUN_004bf6f0(surface, &r, colors[15]);
                }
            }
            if (DAT_00511dd0) FUN_004181d0(surface, p, heights);
        }
        if (offscreen) break;
    }
}
