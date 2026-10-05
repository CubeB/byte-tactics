// Decompiled by Space Bunny Free. Names are provisional.
// Writes the local player's stored metal (gadget "METAL") rounded down to
// hundreds into the "METALTEXT" label, mirrors it into the unit at +0xa3 and,
// if the unit's flag byte at +0x97 has bit 0, re-sends the player block and the
// name call. Near-copy of 0x445d60 (the "ENERGY" one) and 0x445b70.
#include <stdlib.h>

struct Holder_00445c70 {
    int unknown_0;
    void* entries;                    // +0x4
};

struct Sub_00445c70 {
    char unknown_0[0x18];
    Holder_00445c70* holder;          // +0x18
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x97];
    unsigned short flag_97 : 1;      // +0x97, stored in the two bytes at 0x97
    char unknown_99[0xa3 - 0x99];
    unsigned short field_a3;          // +0xa3
};

struct PlayerEntry_00445c70 {
    Unit* unit;                       // +0x00
    char unknown_4[0x14b - 4];
};

struct Game_00445c70 {
    char unknown_0[0x1b8a];
    PlayerEntry_00445c70 players[10]; // +0x1b8a
    char unknown_2878[0x2a42 - 0x2878];
    unsigned char localPlayer;        // +0x2a42
};
#pragma pack(pop)

extern Game_00445c70* g_game;

void* __stdcall FUN_004a0200(void* entries, char* name);
int __stdcall FUN_0045ba20(void* text);
void __stdcall FUN_004a0bf0(Sub_00445c70* obj, char* name, char* text, int param_4);
void FUN_00450f90(void);
void FUN_00451180(void);

// FUNCTION: 0x445c70
void __stdcall FUN_00445c70(Sub_00445c70* sub, int unused)
{
    char text[20];
    void* value = FUN_004a0200(sub->holder->entries, "METAL");

    if (value != 0) {
        int shown = FUN_0045ba20(value) / 100 * 100;
        int hundreds;
        Unit* unit;

        _itoa(shown, text, 10);
        FUN_004a0bf0(sub, "METALTEXT", text, 0);
        hundreds = shown / 100;
        g_game->players[g_game->localPlayer].unit->field_a3 = (unsigned short)hundreds;
        // The original writes the same value to the same field a second time,
        // through a freshly looked up unit pointer, before testing its flag.
        unit = g_game->players[g_game->localPlayer].unit;
        unit->field_a3 = (unsigned short)hundreds;
        if (unit->flag_97 & 1) {
            FUN_00450f90();
            FUN_00451180();
        }
    }
}
