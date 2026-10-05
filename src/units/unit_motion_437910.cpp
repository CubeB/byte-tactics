// Decompiled by Opus. Names are provisional.
// For a unit whose type has a positive value at +0x1d2 (presumably the wind
// generator rating), passes the current wind direction and speed to its
// script's SetDirection and SetSpeed functions when wind is enabled.

class Class_004b0a70 {
public:
    int FUN_004b0a70(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

#pragma pack(push, 1)
struct UnitType_00437910 {
    char unknown_0[0x1d2];
    float field_1d2;                   // +0x1d2
};

struct Unit {
    char unknown_0[0x92];
    UnitType_00437910* type;           // +0x92
    char unknown_96[0x9a - 0x96];
    Class_004b0a70* script;            // +0x9a
};

struct Game_00437910 {
    char unknown_0[0x37ed8];
    unsigned short windDirection;      // +0x37ed8
    int windSpeed;                     // +0x37eda
    char unknown_37ede[0x37ee2 - 0x37ede];
    int windEnabled;                   // +0x37ee2
};
#pragma pack(pop)

extern Game_00437910* g_game;

// FUNCTION: 0x437910
void __stdcall FUN_00437910(Unit* unit)
{
    if (unit->type->field_1d2 > 0.0f && g_game->windEnabled) {
        unit->script->FUN_004b0a70("SetDirection", 0, 0, 1, g_game->windDirection, 0, 0, 0);
        unit->script->FUN_004b0a70("SetSpeed", 0, 0, 1, g_game->windSpeed << 4, 0, 0, 0);
    }
}
