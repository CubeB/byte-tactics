// Decompiled by Space Bunny Free, finished by space-bunny-free. Names are provisional.
// Needed: it moves the operand order of the y subtraction.
#include <string.h>

struct Out_482830 {
    short x;
    short y;
};

struct Entry_482830 {
    char unknown_0[4];
    short field_4;
    short field_6;
};

struct Table_482830 {
    unsigned short count;         // +0
    char unknown_2[0x28 - 0x2];
    Entry_482830* entries;        // +0x28
};

#pragma pack(push, 1)
struct Pos_482830 {
    int x;                        // +0x10
    short y_lo;                   // +0x14
    short y_hi;                   // +0x16, that is y / 0x10000
    int z;                        // +0x18
};

struct Params_482830 {
    void* field_0;                // +0
    Out_482830* field_4;          // +4
    short field_8;                // +8
    unsigned char field_a;        // +0xa
    char unknown_b;               // +0xb
    char* field_c;                // +0xc
    Pos_482830 pos;               // +0x10
};

struct Game {
    char unknown_0[0x14281];
    unsigned char flags;          // +0x14281
    char unknown_14282[0x1485b - 0x14282];
    Table_482830* field_1485b;    // +0x1485b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall UpdateLineOfSight(Params_482830* params);
void __stdcall AddLineOfSight(Params_482830* params);
void __stdcall FUN_00481930(Params_482830* params);
Entry_482830* __stdcall GetGafFrame(Table_482830* table, int index);

// FUNCTION: 0x482830
void __stdcall FUN_00482830(Params_482830* params)
{
    if ((g_game->flags & 2) != 2) {
        return;
    }
    *params->field_c = 0;
    if ((g_game->flags & 4) == 4) {
        UpdateLineOfSight(params);
        return;
    }
    // Clamp reads g_game->field_1485b->count twice, not through a local table pointer.
    int lod = params->field_8 / 32 - 5;
    if (lod < 0) {
        lod = 0;
    } else {
        if (lod >= g_game->field_1485b->count) {
            lod = g_game->field_1485b->count - 1;
        }
    }
    int x = params->pos.x / 0x200000;
    int y = params->pos.z / 0x200000 - params->pos.y_hi / 64;
    Entry_482830* entry = GetGafFrame(g_game->field_1485b, lod);
    x -= entry->field_4;
    y -= entry->field_6;
    params->field_4->x = (short)x;
    params->field_4->y = (short)y;
    *params->field_c = (char)lod;
    AddLineOfSight(params);
    FUN_00481930(params);
}
