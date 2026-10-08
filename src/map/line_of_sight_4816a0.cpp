// Decompiled by deepseek-v4.1-flash and space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Kept out of line_of_sight.cpp: the merged file's symbol ids move its registers.
#include <string.h>
// Needed: it changes the schedule of the pos.y and pos.z divisions.
#include <math.h>

#pragma pack(push, 1)
struct UnitDef_004816a0 {
    char unknown_0[0x170];
    unsigned char field_170;           // +0x170
    char unknown_171[0x202 - 0x171];
    unsigned short range;              // +0x202
};

struct Player_004816a0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x7c - 0x74];
    unsigned char* seenMap;            // +0x7c
    char unknown_80[0x88 - 0x80];
    int seenCount;                     // +0x88
    char unknown_8c[0x146 - 0x8c];
    unsigned char index;               // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Vec3_004816a0 {
    int x;                             // +0x00
    int y;                             // +0x04
    int z;                             // +0x08
};

struct Unit {
    char unknown_0[0x6a];
    Vec3_004816a0 position;            // +0x6a
    char unknown_76[0x7a - 0x76];
    short losCacheCellX;                    // +0x7a
    char unknown_7c[0x92 - 0x7c];
    UnitDef_004816a0* def;             // +0x92
    Player_004816a0* owner;            // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short id;                 // +0xa6
    char unknown_a8[0xf8 - 0xa8];
    unsigned char losSightFrameIdx;            // +0xf8
    char unknown_f9[0x118 - 0xf9];
};

struct Entry_004816a0 {
    int field_0;                       // +0x00
    short field_4;                     // +0x04
    short field_6;                     // +0x06
};

struct Cell_004816a0 {
    unsigned short count;              // +0x00
    char unknown_2[0x28 - 0x2];
    Entry_004816a0* entries;           // +0x28
};

struct Params_004816a0 {
    void* field_0;                     // +0x00
    short* field_4;                    // +0x04
    short field_8;                     // +0x08
    unsigned char field_a;             // +0x0a
    char unknown_b;                    // +0x0b
    unsigned char* field_c;            // +0x0c
    Vec3_004816a0 pos;                 // +0x10
    int unknown_1c;                    // +0x1c
    int unknown_20;                    // +0x20
};

struct Flags_142f1_004816a0 {
    unsigned short bit0 : 1;
    unsigned short bit1 : 1;
    unsigned short mapChanged : 1;
    unsigned short rest : 13;
};

struct Game {
    char unknown_0[0x1b63];
    Player_004816a0 players[10];       // +0x1b63
    char unknown_2851[0x14233 - 0x2851];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x1427f - 0x14277];
    unsigned char seaLevel;            // +0x1427f
    unsigned char debugMode;
    unsigned short flags;              // +0x14281
    char unknown_14283[0x142f1 - 0x14283];
    Flags_142f1_004816a0 viewDirtyFlags;  // +0x142f1
    char unknown_142f2[0x14356 - 0x142f2];
    Unit* units;                       // +0x14357
    Unit* units_end;                   // +0x1435b
    char unknown_1435f[0x1485b - 0x1435f];
    Cell_004816a0* losTable;           // +0x1485b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall UpdateLineOfSight(Params_004816a0* params);
void __stdcall AddLineOfSight(Params_004816a0* params);
void __stdcall RevealAroundUnit(Params_004816a0* params);
Entry_004816a0* __stdcall GetGafFrame(Cell_004816a0* table, int index);
void UpdateRadarMapped();
void DrawRadarUnits();

// FUNCTION: 0x4816a0
void __stdcall RecalculateLineOfSight(int arg)
{
    if (arg != 0) {
        memset(g_game->visibilityMask, (g_game->flags & 1) ? 0 : 0xFFFF,
               g_game->width * g_game->height * sizeof(short) / 4);
    }
    for (unsigned char i = 0; i < 10; i++) {
        if (i >= 10) continue;
        Player_004816a0* p = &g_game->players[i];
        if (p->active == 0) continue;
        if (p->type != 1 && p->type != 2 && p->type != 3) continue;
        if (p->index == 10) continue;
        memset(p->seenMap, (unsigned char)~((unsigned char)g_game->flags >> 1) & 1, p->seenCount);
    }
    for (Unit* u = g_game->units + 1; u <= g_game->units_end; u++) {
        if (u->id == 0)
            continue;
        Params_004816a0 params;
        params.field_0 = u->owner;
        params.field_4 = &u->losCacheCellX;
        params.field_8 = u->def->range;
        params.field_c = &u->losSightFrameIdx;
        params.pos = u->position;
        params.field_a = u->def->field_170;
        if (params.pos.y < (int)((g_game->seaLevel + 1) << 16))
            params.pos.y = (g_game->seaLevel + 1) << 16;
        if ((g_game->flags & 2) == 2) {
            *params.field_c = 0;
            if ((g_game->flags & 4) == 4) {
                UpdateLineOfSight(&params);
            } else {
                int i = params.field_8 / 32 - 5;
                if (i < 0)
                    i = 0;
                else if (i >= g_game->losTable->count)
                    i = g_game->losTable->count - 1;
                // The original reads this as a 16 bit load of the high word of
                // pos.y, so it is spelled as one here; a plain shift of pos.y
                // would be a 32 bit load plus `sar`.
                int y = ((short*)&params.pos.y)[1] / 64;
                int cx = params.pos.x / 0x200000;
                int cy = params.pos.z / 0x200000 - y;
                Entry_004816a0* e = GetGafFrame(g_game->losTable, i);
                // Full 32 bit subtractions, truncated only at the stores.
                int vx = cx - e->field_4;
                int vz = cy - e->field_6;
                params.field_4[0] = (short)vx;
                params.field_4[1] = (short)vz;
                *params.field_c = (unsigned char)i;
                AddLineOfSight(&params);
                RevealAroundUnit(&params);
            }
        }
    }
    g_game->viewDirtyFlags.mapChanged = 1;
    g_game->flags &= 0xfff7;
    UpdateRadarMapped();
    DrawRadarUnits();
}

// Kept from the earlier partial: none of these source orders or spellings for
// the coordinate block moved the schedule on their own (all 640 bytes, 88.5
// percent, y sunk past the pos.z division): y/cx/cy in any order, cx/cy/y,
// cy -= y, int locals for the three divisions, a short or an int for the high
// word, an inline accessor for it, a function scope int y, and dropping the
// (short) cast on the two stores. The schedule only changed once <math.h> was
// in the file; the store shape only changed once the subtraction results were
// held in int locals.
