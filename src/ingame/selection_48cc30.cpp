// Decompiled by space-bunny-free. Names are provisional.

// The flags dword at +0x110 mixes plain masks (0x10000000, 0x4000) with a
// 1-bit bitfield at bit 4: the bitfield test is the only one the original
// emits as a shift (shr ecx,4 / test cl,1) instead of a masked test, so the
// union keeps both spellings available.
// The team array is indexed by an unsigned char, so its stride shows up as
// g_game + player*0x14b + 0x1b63 (0x14b = 330 + 1).

#pragma pack(push, 1)
struct UnitInfo_0048cc30 {
    char unknown_0[0x156];
    int field_156;                            // +0x156
};

struct Flags_0048cc30 {
    union {
        unsigned int raw;
        struct {
            unsigned int a : 4;
            unsigned int b : 1;
            unsigned int c : 27;
        } bits;
    };
};

struct Unit {
    char unknown_0[0x92];
    UnitInfo_0048cc30* info;                  // +0x92
    char unknown_96[0xa8 - 0x96];
    unsigned short team;                      // +0xa8
    char unknown_aa[0x110 - 0xaa];
    Flags_0048cc30 flags;                     // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Team_0048cc30 {
    char unknown_0[0x67];
    Unit* first;                              // +0x67
    Unit* last;                               // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048cc30 {
    char unknown_0[0x1b63];
    Team_0048cc30 teams[10];                  // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;                     // +0x2a42
    char unknown_2a43[0x2cba - 0x2a43];
    unsigned short sel2;                      // +0x2cba
    char unknown_2cbc[0x14357 - 0x2cbc];
    Unit* units;                              // +0x14357
    char unknown_1435b[0x37e9c - 0x1435b];
    unsigned short sel1;                      // +0x37e9c
};
#pragma pack(pop)

extern Game_0048cc30* g_game;

void __stdcall FUN_00439b30(Unit* unit, int mask, void* obj,
                           Unit** sel, int flag);

// FUNCTION: 0x48cc30
void __stdcall FUN_0048cc30(void* obj, Unit** sel)
{
    Team_0048cc30* team = &g_game->teams[g_game->player];
    Unit* sel1unit = !g_game->sel1 ? 0 : &g_game->units[g_game->sel1];
    Unit* sel2unit = !g_game->sel2 ? 0 : &g_game->units[g_game->sel2];
    Unit* selunit = *sel;
    bool flag = (selunit && selunit->info->field_156)
        || (sel1unit && sel1unit->info->field_156)
        || (sel2unit && sel2unit->info->field_156);
    for (Unit* u = team->first; u <= team->last; u++) {
        if ((u->flags.raw & 0x10000000) && !(u->flags.raw & 0x4000)) {
            if (u == *sel || u->team == g_game->sel1 || u->team == g_game->sel2)
                FUN_00439b30(u, 0x1f, obj, sel, 1);
            else if (u->flags.bits.b)
                FUN_00439b30(u, 0x1f, obj, sel, 0);
            else if (flag)
                FUN_00439b30(u, 1, obj, sel, 1);
        }
    }
}
