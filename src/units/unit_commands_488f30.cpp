// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14267];
    float field_14267;                 // +0x14267
    char unknown_1426b[0x37ede - 0x1426b];
    float field_37ede;                 // +0x37ede
};

struct Unit_00488f30 {
    char unknown_0[0x1c6];
    float field_1c6;                   // +0x1c6
    char unknown_1ca[0x1d2 - 0x1ca];
    float field_1d2;                   // +0x1d2
    float field_1d6;                   // +0x1d6
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x488f30
float __stdcall FUN_00488f30(Unit_00488f30* unit)
{
    if (unit->field_1c6 != 0.0) {
        return unit->field_1c6;
    }
    if (unit->field_1d2 > 0.0f) {
        return -(g_game->field_37ede * unit->field_1d2);
    }
    if (unit->field_1d6 > 0.0f) {
        return -(g_game->field_14267 * unit->field_1d6);
    }
    return 0.0f;
}
