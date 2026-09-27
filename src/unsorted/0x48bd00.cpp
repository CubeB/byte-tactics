// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Unit_0048bd00 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game_0048bd00 {
    char unknown_0[0x14357];
    Unit_0048bd00* units_begin;        // +0x14357
    Unit_0048bd00* units_end;          // +0x1435b
};
#pragma pack(pop)

extern Game_0048bd00* g_game;

void __stdcall FUN_00491d70(int a);

// FUNCTION: 0x48bd00
void FUN_0048bd00(void)
{
    for (Unit_0048bd00* u = g_game->units_begin; u <= g_game->units_end; u++)
        u->flags &= 0xffffff2f;
    FUN_00491d70(0);
}
