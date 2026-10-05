// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Unit_0041b2a0 {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

struct Game_0041b2a0 {
    char unknown_0[0x14357];
    Unit_0041b2a0* units;              // +0x14357
    char unknown_1435b[0x37e9c - 0x1435b];
    unsigned short unitIndex;          // +0x37e9c
};
#pragma pack(pop)

extern Game_0041b2a0* g_game;

// FUNCTION: 0x41b2a0
Unit_0041b2a0* FUN_0041b2a0()
{
    unsigned short index = g_game->unitIndex;
    if (index) {
        Unit_0041b2a0* unit = &g_game->units[index];
        if (unit->field_a6)
            return unit;
    }
    return 0;
}
