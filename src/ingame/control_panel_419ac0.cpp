// Decompiled by Opus. Names are provisional.
// Sets the "ONOFF" menu entry from bit 0 of the unit's flags at +0x10e
// (compare 0x495860).

struct Struct_00419ac0 {
    int unknown_0;
    int value;                         // +0x4
};

struct Menu_00419ac0 {
    char unknown_0[0x18];
    Struct_00419ac0* layer;            // +0x18
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Menu_00419ac0 menu;                // +0x519
};

struct Unit_00419ac0 {
    char unknown_0[0x10e];
    unsigned short on : 1;             // +0x10e bit 0
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_0049fe60(int value, char* name);
void __stdcall FUN_004a11c0(Menu_00419ac0* menu, int index, short value);

// FUNCTION: 0x419ac0
void __stdcall FUN_00419ac0(Unit_00419ac0* unit)
{
    int index = FUN_0049fe60(g_game->menu.layer->value, "ONOFF");
    if (index != -1) {
        FUN_004a11c0(&g_game->menu, index, unit->on);
    }
}
