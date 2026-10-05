// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Toggles bit 4 (0x10) of the currently selected unit at +0x110 when the
// command byte at param+8 has bit 2 set, or otherwise clears bits 4 and 7
// from every unit, restores bit 6 (0x40) on the local player's units in the
// selection list and sets bit 4 on the selected unit, then refreshes the
// orders menu through g_game+0x37ebe.

#pragma pack(push, 1)
union Flags_0048c7f0 {
    union {
        unsigned int raw;
        struct {
            unsigned int a : 4;
            unsigned int b : 1;
            unsigned int c : 27;
        } bits;
    };
};

struct Link_0048c7f0 {
    char unknown_0[0x110];
    unsigned int flags;                 // +0x110
};

struct Unit {
    char unknown_0[0x86];
    Link_0048c7f0* link;                // +0x86
    char unknown_8a[0xfb - 0x8a];
    int field_fb;                       // +0xfb
    unsigned char owner;                // +0xff
    char unknown_100[0x104 - 0x100];
    float field_104;                    // +0x104
    char unknown_108[0x110 - 0x108];
    Flags_0048c7f0 flags;               // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game {
    char unknown_0[0x2a42];
    unsigned char player;               // +0x2a42
    char unknown_2a43[0x2cba - 0x2a43];
    unsigned short sel2;                // +0x2cba
    char unknown_2cbc[0x14357 - 0x2cbc];
    Unit* units;                        // +0x14357
    Unit* units_end;                    // +0x1435b
    unsigned short* list;               // +0x1435f
    char unknown_14363[0x14367 - 0x14363];
    int count;                          // +0x14367
    char unknown_1436b[0x37e9c - 0x1436b];
    unsigned short field_37e9c;         // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short flags_37ebe;         // +0x37ebe
};

struct Param_0048c7f0 {
    char unknown_0[8];
    unsigned char flags;                // +0x8
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text);
void __stdcall FUN_00491d70(int force);

// FUNCTION: 0x48c7f0
void __stdcall FUN_0048c7f0(Param_0048c7f0* param)
{
    Unit* unit = !g_game->sel2 ? 0 : &g_game->units[g_game->sel2];
    if (unit == 0)
        return;
    if (unit->owner != g_game->player)
        return;
    if (!(unit->flags.raw & 0x20))
        return;
    if (unit->field_104 != 0.0f)
        return;
    if (unit->field_fb != 0)
        return;
    if (unit->link != 0 && !(unit->link->flags & 0x40000000))
        return;

    if (param->flags & 4) {
        unit->flags.bits.b = !unit->flags.bits.b;
        if (unit->flags.bits.b)
            QueueUnitSpeech(unit, 1, 0);
        g_game->field_37e9c = 0;
        g_game->flags_37ebe |= 0x10;
        return;
    }

    for (Unit* u = g_game->units; u <= g_game->units_end; u++)
        u->flags.raw &= 0xffffff2f;
    FUN_00491d70(0);
    unsigned short* list = g_game->list;
    for (int i = 0; i < g_game->count; i++) {
        Unit* u = &g_game->units[list[i]];
        if (u->owner == g_game->player)
            u->flags.raw = u->flags.raw & 0xffffff7f | 0x40;
    }
    unit->flags.raw |= 0x10;
    QueueUnitSpeech(unit, 1, 0);
    g_game->flags_37ebe |= 0x10;
}
