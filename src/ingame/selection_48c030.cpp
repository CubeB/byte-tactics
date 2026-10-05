// Decompiled by space-bunny-free. Names are provisional.
// Clears flag bit 5 (0x20) on every unit, then walks the selection list at
// +0x1435f and puts flag bit 4 (0x10) back on the local player's units that
// are still idle: bit 5 set, no work left at +0x104, no target at +0xfb, and
// either no link object at +0x86 or bit 0x40 set in its byte at +0x113. If any
// unit qualified, the single selected unit at +0x37e9c is cleared and bit
// 0x10 is set in the order byte at +0x37ebe to refresh the orders menu.

#pragma pack(push, 1)
struct Link_0048c030 {
    char unknown_0[0x113];
    unsigned char flags;                // +0x113
};

struct Unit {
    char unknown_0[0x86];
    Link_0048c030* link;                // +0x86
    char unknown_8a[0xfb - 0x8a];
    int field_fb;                       // +0xfb
    unsigned char owner;                // +0xff
    char unknown_100[0x104 - 0x100];
    float field_104;                    // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                 // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game {
    char unknown_0[0x2a42];
    unsigned char player;               // +0x2a42
    char unknown_2a43[0x14357 - 0x2a43];
    Unit* units_begin;                  // +0x14357
    Unit* units_end;                    // +0x1435b
    unsigned short* list;               // +0x1435f
    char unknown_14363[0x14367 - 0x14363];
    int count;                          // +0x14367
    char unknown_1436b[0x37e9c - 0x1436b];
    unsigned short field_37e9c;          // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned char flags_37ebe;          // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_00491d70(int force);
void FUN_00495860(void);

// FUNCTION: 0x48c030
void FUN_0048c030(void)
{
    int found = 0;
    for (Unit* u = g_game->units_begin; u <= g_game->units_end; u++)
        u->flags &= 0xffffff2f;
    FUN_00491d70(0);
    unsigned short* list = g_game->list;
    for (int i = 0; i < g_game->count; i++) {
        Unit* u = &g_game->units_begin[list[i]];
        unsigned int flags = u->flags;
        if ((flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0
            && (u->link == 0 || (u->link->flags & 0x40))
            && u->owner == g_game->player) {
            found = 1;
            u->flags = flags | 0x10;
        }
    }
    if (found) {
        FUN_00495860();
        g_game->field_37e9c = 0;
        g_game->flags_37ebe |= 0x10;
    }
}
