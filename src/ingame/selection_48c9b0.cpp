// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37e9c];
    short field_37e9c;                 // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned char flags_37ebe;         // +0x37ebe
};

struct Unit_0048c9b0 {
    char unknown_0[0x86];
    Unit_0048c9b0* field_86;           // +0x86
    char unknown_8a[0xfb - 0x8a];
    int field_fb;                      // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                // +0x110
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x48c9b0
void __stdcall FUN_0048c9b0(Unit_0048c9b0* unit)
{
    unsigned int flags = unit->flags;
    if (flags & 0x10) {
        if (!(flags & 0x20) || unit->field_104 != 0.0f || unit->field_fb != 0
            || (unit->field_86 != 0 && !(unit->field_86->flags & 0x40000000))) {
            unit->flags = flags & ~0x10;
            g_game->field_37e9c = 0;
            g_game->flags_37ebe |= 0x10;
        }
    }
}
