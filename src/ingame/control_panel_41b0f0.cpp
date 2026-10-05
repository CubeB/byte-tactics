// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Opens the side-specific "GEN.GUI" dialog (ARMGEN.GUI / COREGEN.GUI) with
// FUN_0041aa00 as its handler and remembers the selected unit's two ids.
#include <stdio.h>

#pragma pack(push, 1)

struct Unit {
    char unknown_0[0x95];
    unsigned char side;                // +0x95
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;           // +0xa6
    unsigned short field_a8;           // +0xa8
};

struct PlayerEntry_0041b0f0 {
    Unit* unit;                        // +0x00
    char unknown_4[0x14b - 4];
};

struct Sub_0041b0f0 {
    char unknown_0[0x10];
};

struct Game {
    char unknown_0[0x519];
    Sub_0041b0f0 sub;                  // +0x519
    char unknown_529[0x1b8a - 0x529];
    PlayerEntry_0041b0f0 players[10];  // +0x1b8a
    char unknown_2878[0x2a42 - 0x2878];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37e9c - 0x2a43];
    unsigned short field_37e9c;        // +0x37e9c
    unsigned short field_37e9e;        // +0x37e9e
    char unknown_37ea0[0x37f5b - 0x37ea0];
    char sideNames[2][0x232];          // +0x37f5b
};
#pragma pack(pop)

struct Gadget_0041b0f0 {
    char unknown_0[0x8];
    void (__stdcall* handler)(void*);  // +0x8
    int field_c;                       // +0xc
};

extern Game* g_game;

Gadget_0041b0f0* __stdcall FUN_004aa8f0(Sub_0041b0f0* sub, const char* name, int flags);
void __stdcall FUN_0041a120(Unit* unit);
void __stdcall FUN_004a81e0(Sub_0041b0f0* sub, int value);
void __stdcall FUN_0041aa00(void* unit);

// FUNCTION: 0x41b0f0
void __stdcall FUN_0041b0f0(Unit* unit)
{
    char name[256];
    sprintf(name, "%sGEN.GUI",
            g_game->sideNames[g_game->players[g_game->localPlayer].unit->side]);
    Gadget_0041b0f0* gadget = FUN_004aa8f0(&g_game->sub, name, 0);
    if (gadget != 0) {
        gadget->handler = FUN_0041aa00;
        gadget->field_c = 0;
        FUN_0041a120(unit);
        FUN_004a81e0(&g_game->sub, 0x40);
        if (unit != 0) {
            g_game->field_37e9c = unit->field_a8;
            g_game->field_37e9e = unit->field_a6;
        } else {
            g_game->field_37e9c = 0;
            g_game->field_37e9e = 0;
        }
    }
}
