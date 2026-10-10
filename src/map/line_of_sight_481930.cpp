// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, re-tried by
// Kept out of line_of_sight.cpp: the merged file's symbol ids move its registers.
// deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, edited
// by deepseek-v4.1, edited by space-bunny-free, finished by
// deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by
// Space Bunny Free. Names are provisional.
// Required include.
#include <windows.h>

#include "los_tables.h"

#pragma pack(push, 1)

class LosLine {
public:
    short GetLosLineStepCount();
    void GetLosLineStep(short i, unsigned short* a, unsigned short* b);
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
void StepAllGafSequences(void);
void ResetNetStats(void);

extern char g_losTables[];

#include "grid.h"

struct Player_00481930 {
    char unknown_0[0x7c];
    Grid grid;                         // +0x7c
    char unknown_8c[0x146 - 0x8c];
    unsigned char index;               // +0x146
};

#include "sight_query.h"

struct LosTable_00481930 {
    unsigned short count;              // +0x00
};

struct GafFrame {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
    char unknown_4[4];                 // +0x04
    unsigned char mask;                // +0x08
    char unknown_9[0x10 - 9];
    unsigned char* data;               // +0x10
};

struct Game {
    char unknown_0[0x2a43];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int mapWidthTiles;                 // +0x14233
    int mapHeightTiles;                // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned short bit0 : 1;           // +0x14281
    unsigned short bit1 : 1;
    unsigned short flag2 : 1;
    unsigned short flag3 : 1;
    unsigned short rest : 12;
    char unknown_14283[0x1428f - 0x14283];
    Grid losHalfResHeightBand;         // +0x1428f
    char unknown_1429f[0x142f1 - 0x1429f];
    unsigned char flags_142f1;         // +0x142f1
    char unknown_142f2[0x1485b - 0x142f2];
    LosTable_00481930* losTable;       // +0x1485b
};

#pragma pack(pop)

extern Game* g_game;

GafFrame* __stdcall GetGafFrame(LosTable_00481930* table, int index);

inline int LodRaw_00481930(SightQuery* params)
{
    return params->sightDistance / 32;
}

inline int Lod_00481930(SightQuery* params)
{
    int v = LodRaw_00481930(params);
    return v < 0 ? 0 : v;
}

// FUNCTION: 0x481930
void __stdcall RevealAroundUnit(SightQuery* params)
{
    // Both branches' locals, x and y included, are declared here and assigned
    // later; frame stays declared after bit.
    int changed = 0;
    int x, y;
    int limitX, limitY, nx, ny;
    int i, stride, off;
    unsigned int bit = 1 << ((Player_00481930*)params->player)->index;
    GafFrame* frame;
    int halfW = g_game->mapWidthTiles / 2;
    int halfH = g_game->mapHeightTiles / 2;
    x = params->cacheCell[0];
    y = params->cacheCell[1];
    if (g_game->flag2 == 1) {
        Grid* grid = &g_game->losHalfResHeightBand;
        if ((unsigned)x < grid->width && (unsigned)y < grid->height) {
            LosTable* table =
                (LosTable*)((LosTables*)g_losTables)
                    ->GetLosTable(
                        (params->sightDistance / 32 < 0 ? 0 : params->sightDistance / 32) <
                                (short)((LosTables*)g_losTables)->GetLosTableCount() - 1
                            ? (params->sightDistance / 32 < 0 ? 0 : params->sightDistance / 32)
                            : (short)((LosTables*)g_losTables)->GetLosTableCount() - 1);
            short count = table->GetLosLineCount();
            unsigned short* cell = &g_game->visibilityMask[halfW * y + x];
            if ((unsigned short)(bit & *cell) == 0) {
                *cell ^= bit;
                changed = 1;
            }
            int ref = *params->frameIdx;
            for (short i = 0; (short)i < count; i++) {
                LosLine* line = table->GetLosLine(i);
                short num = line->GetLosLineStepCount();
                // Declared in this order: bestIdx, j1, bestDiff; j1 is set in the guard.
                int bestIdx = 0;
                int j1;
                int bestDiff = -1;
                short j = 0;
                if ((short)num > 0) {
                    j1 = 1;
                    do {
                        int y2, x2;
                        line->GetLosLineStep((short)j, (unsigned short*)&x2, (unsigned short*)&y2);
                        x2 += x;
                        y2 += y;
                        if ((unsigned)(short)x2 < grid->width &&
                            (unsigned)(short)y2 < grid->height) {
                            unsigned char* c =
                                grid->cells + ((short)y2 * grid->width + (short)x2) * 2;
                            int d1 = c[1] - ref;
                            int d0 = c[0] - ref;
                            if (d0 * bestIdx > bestDiff * j1) {
                                unsigned short* q = &g_game->visibilityMask[
                                    halfW * (short)y2 + (short)x2];
                                if ((unsigned short)(bit & *q) == 0) {
                                    *q ^= bit;
                                    changed = 1;
                                }
                                if (d1 * bestIdx > bestDiff * j1) {
                                    bestIdx = j1;
                                    bestDiff = d1;
                                }
                            }
                        }
                        j++;
                        j1++;
                    } while ((short)j < (short)num);
                }
            }
        }
    } else {
        int lod = LodRaw_00481930(params) - 5;
        if (lod < 0)
            lod = 0;
        else if (lod >= g_game->losTable->count)
            lod = g_game->losTable->count - 1;
        frame = GetGafFrame(g_game->losTable, lod);
        limitX = (x + frame->width < halfW) ? frame->width : halfW - x;
        limitY = (y + frame->height >= halfH) ? halfH - y : frame->height;
        nx = x < 0 ? -x : 0;
        ny = y < 0 ? -y : 0;
        changed = 0;
        i = ny;
        if (i < limitY) {
            stride = halfW * 2;
            off = ((y + ny) * halfW + nx + x) * 2;
            do {
                // dst is declared before src, the reverse of the order of use.
                unsigned short* dst =
                    (unsigned short*)((unsigned char*)g_game->visibilityMask + off);
                unsigned char* src = frame->data + i * frame->width + nx;
                if (nx < limitX) {
                    int n = limitX - nx;
                    do {
                        if (*src != frame->mask && (unsigned short)(bit & *dst) == 0) {
                            changed = 1;
                            *dst ^= bit;
                        }
                        dst++;
                        src++;
                    } while (--n);
                }
                i++;
                off += stride;
            } while (i < limitY);
        }
    }
    if (changed && ((Player_00481930*)params->player)->index == g_game->playerIndex) {
        g_game->flag3 = 0;
        g_game->flags_142f1 |= 4;
    }
}
