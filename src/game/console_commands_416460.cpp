// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game_00416460 {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
    Unit* units_end;                   // +0x1435b
};
#pragma pack(pop)

extern Game_00416460* g_game;

// FUNCTION: 0x416460
void __stdcall FUN_00416460(int unused)
{
    for (Unit* u = &g_game->units[1]; u <= g_game->units_end; u++) {
        if (u->flags & 0x10000000) {
            u->flags |= 0x20;
        }
    }
}
