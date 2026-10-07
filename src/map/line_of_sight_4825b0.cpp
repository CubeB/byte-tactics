// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// Needed: it changes how the operands of the cell-y subtraction are scheduled.
#include <windows.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct Vec3_4825b0 {
    int x;                      // +0x00
    int y;                      // +0x04
    int z;                      // +0x08
};

struct Entry_4825b0 {
    int field_0;                // +0x00
    short field_4;              // +0x04
    short field_6;              // +0x06
    short field_8;              // +0x08
};

struct Cell_4825b0 {
    unsigned short count;       // +0x00
    char unknown_2[0x28 - 0x2];
    Entry_4825b0* entries;      // +0x28
};

struct Params_4825b0 {
    void* field_0;              // +0x00
    short* field_4;             // +0x04
    short field_8;              // +0x08
    unsigned char field_a;      // +0x0a
    char unknown_b;             // +0x0b
    unsigned char* field_c;     // +0x0c
    Vec3_4825b0 pos;            // +0x10
};

struct Game {
    char unknown_0[0x14281];
    unsigned short flags;       // +0x14281
    char unknown_14283[0x14293 - 0x14283];
    unsigned int field_14293;   // +0x14293
    unsigned int field_14297;   // +0x14297
    char unknown_1429b[0x1485b - 0x1429b];
    Cell_4825b0* field_1485b;   // +0x1485b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall RemoveLineOfSight(Params_4825b0* params);
void __stdcall AddLineOfSight(Params_4825b0* params);
void __stdcall FUN_00481930(Params_4825b0* params);
Entry_4825b0* __stdcall GetGafFrame(Cell_4825b0* table, int index);

// FUNCTION: 0x4825b0
void __stdcall UpdateLineOfSight(Params_4825b0* params)
{
    if ((g_game->flags & 4) == 4) {
        int x = ((short*)&params->pos.x)[1] >> 5;
        int v = params->field_a + ((short*)&params->pos.y)[1];
        if (v < 0) {
            v = 0;
        }
        if (v > 0xff) {
            v = 0xff;
        }
        int y = (((short*)&params->pos.z)[1] - (v >> 1)) >> 5;
        int diff = abs((int)*params->field_c - v);
        unsigned char c = *params->field_c;
        if (params->field_4[0] != x || params->field_4[1] != y || diff > 5) {
            if (c != 0 && (g_game->flags & 2)) {
                RemoveLineOfSight(params);
            }
            params->field_4[0] = (short)x;
            params->field_4[1] = (short)y;
            if ((unsigned)x >= g_game->field_14293 || (unsigned)y >= g_game->field_14297) {
                *params->field_c = 0;
                return;
            }
            *params->field_c = (unsigned char)v;
            if ((unsigned char)(g_game->flags >> 1) & 1) {
                AddLineOfSight(params);
            }
            if (g_game->flags & 1) {
                FUN_00481930(params);
            }
        }
    }
    else {
        int i = (short)params->field_8 / 32 - 5;
        if (i < 0) {
            i = 0;
        } else if (i >= g_game->field_1485b->count) {
            i = g_game->field_1485b->count - 1;
        }
        // cx before cy: puts its magic multiply ahead of the subtraction.
        int cx = params->pos.x / 0x200000;
        int cy = params->pos.z / 0x200000 - ((short*)&params->pos.y)[1] / 64;
        Entry_4825b0* e = GetGafFrame(g_game->field_1485b, i);
        cx -= e->field_4;
        cy -= e->field_6;
        if (params->field_4[0] != cx || params->field_4[1] != cy || *params->field_c != i) {
            if ((g_game->flags & 2) == 2) {
                RemoveLineOfSight(params);
                params->field_4[0] = (short)cx;
                params->field_4[1] = (short)cy;
                *params->field_c = (unsigned char)i;
                AddLineOfSight(params);
            } else {
                params->field_4[0] = (short)cx;
                params->field_4[1] = (short)cy;
                *params->field_c = (unsigned char)i;
            }
            if (g_game->flags & 1) {
                FUN_00481930(params);
            }
        }
    }
}
