// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Unit_00481340 {
    char unknown_0[0x86];
    int owner;                         // +0x86
    char unknown_8a[0x110 - 0x8a];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game_00481340 {
    char unknown_0[0x14357];
    Unit_00481340* units;              // +0x14357
};
#pragma pack(pop)

extern Game_00481340* g_game;

struct Player_00481340 {
    char unknown_0[0xc];
    int id;                            // +0x0c
};

void __stdcall FUN_0048aac0(Unit_00481340* unit, int player, int a, int b);

static inline Unit_00481340* GetUnit(unsigned short id)
{
    if (id == 0)
        return 0;
    return &g_game->units[id];
}

class Class_00481340 {
public:
    char unknown_0[0x540];
    Player_00481340* player;           // +0x540
    void FUN_00481340(unsigned short id, int a, int b);
};

// FUNCTION: 0x481340
void Class_00481340::FUN_00481340(unsigned short id, int a, int b)
{
    Unit_00481340* u = GetUnit(id);
    if (u != 0 && (u->flags & 0x10000000)
        && (u->owner == 0 || u->owner == player->id)) {
        FUN_0048aac0(u, player->id, a, b);
    }
}
