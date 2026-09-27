// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Unit_004813b0 {
    char unknown_0[0x76];
    int position;                      // +0x76
    char unknown_7a[0x86 - 0x7a];
    int owner;                         // +0x86
    char unknown_8a[0x92 - 0x8a];
    void* type;                        // +0x92
    char unknown_96[0xa8 - 0x96];
    short field_a8;                    // +0xa8
    char unknown_aa[0x110 - 0xaa];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game_004813b0 {
    char unknown_0[0x14357];
    Unit_004813b0* units;              // +0x14357
};
#pragma pack(pop)

extern Game_004813b0* g_game;

struct Player_004813b0 {
    char unknown_0[0xc];
    int id;                            // +0x0c
};

int __stdcall FUN_0047db70(void* type, short a, int position, int b);
void __stdcall FUN_0048aac0(Unit_004813b0* unit, int player, int a, int b);

static inline Unit_004813b0* GetUnit(unsigned short id)
{
    if (id == 0)
        return 0;
    return &g_game->units[id];
}

class Class_004813b0 {
public:
    char unknown_0[0x540];
    Player_004813b0* player;           // +0x540
    void FUN_004813b0(unsigned short id);
};

// FUNCTION: 0x4813b0
void Class_004813b0::FUN_004813b0(unsigned short id)
{
    Unit_004813b0* u = GetUnit(id);
    if (u != 0 && (u->flags & 0x10000000) && u->owner == player->id) {
        if (FUN_0047db70(u->type, u->field_a8, u->position, 1)) {
            FUN_0048aac0(u, 0, -1, 1);
        }
    }
}
