// Decompiled by deepseek-v4.1-flash, edited by deepseek-v4.1, re-tried by
// space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash.
// Names are provisional.
// Not gathered into line_of_sight.cpp: merged, the player grid address folds in
// the other operand order.
// Required include.
#include <windows.h>
#include <stdio.h>

#include "los_tables.h"

#pragma pack(push, 1)

class LosLine {
public:
    short GetLosLineStepCount();
    void GetLosLineStep(short i, int* a, int* b);
};

class LosTable {
public:
    short GetLosLineCount();
    LosLine* GetLosLine(short i);
};

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
void RegisterUnitOrders(void);
void RegisterGroundOrders(void);
void EnableAICommands(void);
void RegisterAICommands(void);
void FUN_00406f40(void);
void ResetAIPlayers(void);
void RegisterVtolOrders(void);

extern LosTables g_losTables;

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

#include "sight_query.h"

#include "grid.h"

struct Game {
    char unknown_0[0x2a43];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int mapWidthTiles;                 // +0x14233
    int mapHeightTiles;                // +0x14237
    char unknown_1423b[0x14281 - 0x1423b];
    unsigned short bit0 : 1;           // +0x14281
    unsigned short bit1 : 1;
    unsigned short flag2 : 1;
    unsigned short flag3 : 1;
    unsigned short rest : 12;
    char unknown_14283[0x1428f - 0x14283];
    Grid losHalfResHeightBand;         // +0x1428f
    char unknown_1429f[0x142f1 - 0x1429f];
    unsigned short flags_142f1_bit0 : 1;  // +0x142f1
    unsigned short flags_142f1_bit1 : 1;
    unsigned short flags_142f1_mapChanged : 1;
    unsigned short flags_142f1_rest : 13;
    char unknown_142f3[0x1485b - 0x142f3];
    void* losTable;                    // +0x1485b
};

#include "../graphics/gaf_frame.h"

#pragma pack(pop)

extern Game* g_game;

GafFrame* __stdcall GetGafFrame(unsigned short* table, int index);



// FUNCTION: 0x481d50
void __stdcall RemoveLineOfSight(SightQuery* params)
{
    if (((Map_00481d50*)params->player)->playerIndex == g_game->playerIndex) {
        g_game->flag3 = 0;
        g_game->flags_142f1_mapChanged = 1;
    }
    int halfW = g_game->mapWidthTiles / 2;
    int halfH = g_game->mapHeightTiles / 2;
    int x = params->cacheCell[0];
    int y = params->cacheCell[1];
    if (g_game->flag2 == 1) {
        Grid* grid = &g_game->losHalfResHeightBand;
        if ((unsigned)x >= grid->width)
            return;
        if ((unsigned)y >= grid->height)
            return;
        LosTable* table = (LosTable*)g_losTables.GetLosTable(
            (params->sightDistance / 32 < 0 ? 0 : params->sightDistance / 32) <
                    (short)g_losTables.GetLosTableCount() - 1
                ? (params->sightDistance / 32 < 0 ? 0 : params->sightDistance / 32)
                : (short)g_losTables.GetLosTableCount() - 1);
        short count = table->GetLosLineCount();
        short i = 0;
        ((Map_00481d50*)params->player)->explored.at(x, y)--;
        int ref = *params->frameIdx;
        for (i = 0; i < count; i++) {
            LosLine* line = table->GetLosLine(i);
            short num = line->GetLosLineStepCount();
            // Declared in this order: bestIdx, j1, bestDiff, j; j1 is set in the guard.
            int bestIdx = 0;
            int j1;
            int bestDiff = -1;
            short j = 0;
            if ((short)num > 0) {
                j1 = 1;
                do {
                    int dx;
                    int dy;
                    line->GetLosLineStep(j, &dx, &dy);
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
                        ((Map_00481d50*)params->player)
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
        int ref = *params->frameIdx;
        GafFrame* frame =
            GetGafFrame((unsigned short*)g_game->losTable, ref);
        int limitX;
        if (x + frame->width >= halfW)
            limitX = halfW - x;
        else
            limitX = frame->width;
        int limitY;
        if (y + frame->height >= halfH)
            limitY = halfH - y;
        else
            limitY = frame->height;
        int nx = x < 0 ? -x : 0;
        int ny = y < 0 ? -y : 0;
        if (ny >= limitY)
            return;
        if (nx >= limitX)
            return;
        for (int i = ny; i < limitY; i++) {
            ByteMap_00481d50* ex = &((Map_00481d50*)params->player)->explored;
            unsigned char* dst = ex->data + (y + i) * ex->size.width + nx + x;
            unsigned char* src = frame->pixelsOrLayers + i * frame->width + nx;
            for (int j = nx; j < limitX; j++) {
                if (*src != frame->transparency)
                    (*dst)--;
                dst++;
                src++;
            }
        }
    }
}
