// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <stdio.h>

#pragma pack(push, 1)
struct Entry_0044c370 {
    char unknown_0[0x52];
    int unitType;                       // +0x52
    char unknown_56[0x62 - 0x56];
};

struct Unit_0044c370 {
    char unknown_0[0xba];
    short field_ba;                     // +0xba
    char unknown_bc[0xd2 - 0xbc];
    Entry_0044c370* field_d2;           // +0xd2
};

struct Def_0044c370 {
    char unknown_0[0x186];
    float field_186;                    // +0x186
    float field_18a;                    // +0x18a
    char unknown_18e[0x249 - 0x18e];
};

struct Game {
    char unknown_0[0x1439b];
    Def_0044c370* defs;                 // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004a0bf0(void* obj, char* name, int param_3, int param_4);

// FUNCTION: 0x44c370
void __stdcall FUN_0044c370(void* panel, Unit_0044c370* unit)
{
    char buf[20];
    Def_0044c370* def = &g_game->defs[unit->field_d2[unit->field_ba].unitType];
    sprintf(buf, "%d", (int)def->field_186);
    FUN_004a0bf0(panel, "ENERGYTEXT", (int)buf, 0);
    sprintf(buf, "%d", (int)def->field_18a);
    FUN_004a0bf0(panel, "METALTEXT", (int)buf, 0);
}
