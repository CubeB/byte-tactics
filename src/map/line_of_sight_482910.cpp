// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Kept out of line_of_sight.cpp: the merged file's symbol ids move its registers.
// Appends a new "eyeball" record to the array at g_game + 0x1427b (the same
// record type as 0x482130), then runs the same inlined processing tail as
// 0x482ac0 / 0x482830. The tail is the body of 0x482830 inlined on the new
// record, so the map-flags & 2 test appears twice.

struct Vec3_482910 {
    int x;
    int y;
    int z;
};

struct Pos_482910 {
    short x;
    short y;
};

#include "../graphics/gaf_frame.h"

struct Table_482910 {
    unsigned short count;              // +0
    char unknown_2[0x28 - 0x2];
    void* entries;                     // +0x28
};

#pragma pack(push, 1)
struct Eye_482910 {
    void* player;                      // +0x00
    Pos_482910* screen;                // +0x04, &screenPos
    short x;                           // +0x08
    unsigned char flagA;               // +0x0a
    char flagB;                        // +0x0b
    char* flagPtr;                     // +0x0c, &flagB
    Vec3_482910 pos;                   // +0x10
    unsigned int expires;              // +0x1c
    Pos_482910 screenPos;              // +0x20
};

struct Game {
    char unknown_0[0x1b63];
    char players[10][0x14b];           // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14277 - 0x2a44];
    int count;                         // +0x14277
    Eye_482910* eyes;                  // +0x1427b
    unsigned char seaLevel;            // +0x1427f
    char debugMode;
    unsigned short mapFlags;           // +0x14281
    char unknown_14283[0x1485b - 0x14283];
    Table_482910* losTable;            // +0x1485b
    char unknown_1485f[0x38a47 - 0x1485f];
    unsigned int ticks;                // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall UpdateLineOfSight(Eye_482910* e);
void __stdcall AddLineOfSight(Eye_482910* e);
void __stdcall RevealAroundUnit(Eye_482910* e);
GafFrame* __stdcall GetGafFrame(unsigned short* table, int index);

// FUNCTION: 0x482910
// Plain int parameters: a char or unsigned char one changes the stack frame.
void __stdcall AddEyeball(Vec3_482910* src, int a, int b, int c)
{
    if ((g_game->mapFlags & 2) == 2 && g_game->count < 0x14) {
        Eye_482910* e = &g_game->eyes[g_game->count];
        e->player = g_game->players[g_game->playerIndex];
        // Assigned before screen: &e->screenPos would clobber ecx, which holds g_game.
        e->screen = &e->screenPos;
        e->x = a;
        e->flagPtr = &e->flagB;
        e->pos = *src;
        e->flagA = b;
        int minY = (g_game->seaLevel + 1) << 16;
        if (e->pos.y < minY) {
            e->pos.y = minY;
        }
        e->expires = g_game->ticks + c;
        if ((g_game->mapFlags & 2) == 2) {
            *e->flagPtr = 0;
            if ((g_game->mapFlags & 4) == 4) {
                UpdateLineOfSight(e);
            } else {
                int lod = e->x / 32 - 5;
                if (lod < 0) {
                    lod = 0;
                } else if (lod >= g_game->losTable->count) {
                    lod = g_game->losTable->count - 1;
                }
                int cell_x = e->pos.x / 0x200000;
                int cell_y = e->pos.z / 0x200000 - ((short*)&e->pos.y)[1] / 64;
                GafFrame* ce = GetGafFrame((unsigned short*)g_game->losTable, lod);
                cell_x -= ce->xOffset;
                cell_y -= ce->yOffset;
                e->screen->x = (short)cell_x;
                e->screen->y = (short)cell_y;
                *e->flagPtr = (char)lod;
                AddLineOfSight(e);
                RevealAroundUnit(e);
            }
        }
        g_game->count++;
    }
}
