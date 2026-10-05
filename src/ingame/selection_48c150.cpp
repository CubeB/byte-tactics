// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game_0048c150 {
    char unknown_0[0x14357];
    Unit* units_begin;                 // +0x14357
    Unit* units_end;                   // +0x1435b
};
#pragma pack(pop)

extern Game_0048c150* g_game;

// FUNCTION: 0x48c150
void FUN_0048c150(void)
{
    for (Unit* u = g_game->units_begin; u <= g_game->units_end; u++)
        u->flags &= 0xffffff3f;
}
