// Decompiled by Space Bunny Free. Names are provisional.
// Shows the local player's stored energy (gadget "ENERGY") rounded down to
// hundreds in the "ENERGYTEXT" label, mirrors it into the unit at +0xa1 and,
// if the unit's flag byte at +0x97 has bit 0, re-sends the player block and
// the name call. Near-copy of 0x445c70 (the "METAL" one).
#include <stdlib.h>

struct Holder_00445d60 {
    int unknown_0;
    void* entries;                     // +0x4
};

struct Sub_00445d60 {
    char unknown_0[0x18];
    Holder_00445d60* holder;           // +0x18
};

#pragma pack(push, 1)
struct Unit_00445d60 {
    char unknown_0[0x97];
    unsigned char flag_97;             // +0x97
    char unknown_98[0xa1 - 0x98];
    unsigned short field_a1;           // +0xa1
};

struct PlayerEntry_00445d60 {
    Unit_00445d60* unit;               // +0x00
    char unknown_4[0x14b - 4];
};

struct Game_00445d60 {
    char unknown_0[0x1b8a];
    PlayerEntry_00445d60 players[10];  // +0x1b8a
    char unknown_2878[0x2a42 - 0x2878];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game_00445d60* g_game;

void* __stdcall FUN_004a0200(void* entries, char* name);
int __stdcall FUN_0045ba20(void* gadget);
void __stdcall FUN_004a0bf0(Sub_00445d60* obj, char* name, char* text, int param_4);
void FUN_00450f90(void);
void FUN_00451180(void);

// FUNCTION: 0x445d60
void __stdcall FUN_00445d60(Sub_00445d60* sub, int unused)
{
    char text[20];
    void* value = FUN_004a0200(sub->holder->entries, "ENERGY");

    if (value != 0) {
        int shown = FUN_0045ba20(value) / 100 * 100;
        Unit_00445d60* unit;

        _itoa(shown, text, 10);
        FUN_004a0bf0(sub, "ENERGYTEXT", text, 0);
        unit = g_game->players[g_game->localPlayer].unit;
        unit->field_a1 = (unsigned short)(shown / 100);
        if (unit->flag_97 & 1) {
            FUN_00450f90();
            FUN_00451180();
        }
    }
}
