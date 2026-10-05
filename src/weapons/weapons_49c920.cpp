// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_0049c920 {
    char unknown_0[0x38a47];
    unsigned int now;                  // +0x38a47
};

struct Unit {
    char unknown_0[0x68];
    unsigned int f_68;                 // +0x68
    char unknown_6c[0xdc - 0x6c];
    int f_dc;                          // +0xdc
    char unknown_e0[0xe6 - 0xe0];
    unsigned short f_e6;               // +0xe6
    char unknown_e8[0x111 - 0xe8];
    unsigned int flags;                // +0x111
};

struct Object_0049c920 {
    Unit* unit;                        // +0x0
    char unknown_4[0x46 - 0x4];
    unsigned int time;                 // +0x46
};
#pragma pack(pop)

extern Game_0049c920* g_game;

// FUNCTION: 0x49c920
void __stdcall FUN_0049c920(Object_0049c920* obj)
{
    Unit* unit = obj->unit;
    if (unit->f_68 != 0 && !(unit->flags & 0x8000000)) {
        obj->time = (unit->f_dc << 16) / unit->f_68 + g_game->now;
        return;
    }
    obj->time = g_game->now + unit->f_e6;
}
