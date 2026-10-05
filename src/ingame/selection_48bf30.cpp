// Decompiled by space-bunny-free. Names are provisional.
// Sets or clears flag 0x10 on every finished unit of the local player that
// stands in the line-of-sight bitmask named by the first argument, then clears
// the selected unit and re-sends the stop order.
// The head of the unit range is read through its own pointer to the player
// element while the end of the range is read through p: that is what makes
// MSVC materialise the element address (lea with the 0x1b63 array offset in it)
// for the end field and use the plain array-indexed form for the head load,
// which is what the original does (see also 0x486f10.cpp).

#pragma pack(push, 1)

struct Unit {
    char unknown_0[0x86];
    Unit* owner;                       // +0x86
    char unknown_8a[0xa6 - 0x8a];
    unsigned short field_a6;           // +0xa6
    char unknown_a8[0xfb - 0xa8];
    int field_fb;                      // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player_0048bf30 {
    char unknown_0[0x67];
    Unit* units_begin;                 // +0x67
    Unit* units_end;                   // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

// The bit set here is bit 4 of the 12-bit group at 0x37ebe (see 0x41a120.cpp).
struct Orders_0048bf30 {
    unsigned short unknown_0 : 4;      // +0x37ebe
    unsigned short flag_4 : 1;
    unsigned short unknown_5 : 11;
};

struct Game {
    char unknown_0[0x1b63];
    Player_0048bf30 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37e9c - 0x2a43];
    unsigned short unitIndex;          // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    Orders_0048bf30 orders;            // +0x37ebe
};

#pragma pack(pop)

extern Game* g_game;

void* __stdcall GetCategoryMask(char* name);
void __stdcall FUN_00495860(void);

// FUNCTION: 0x48bf30
void __stdcall FUN_0048bf30(char* name, int param_2)
{
    int* mask = (int*)GetCategoryMask(name);
    int player = g_game->localPlayer;
    Player_0048bf30* p = &g_game->players[player];
    Player_0048bf30* q = &g_game->players[player];
    Unit* u = q->units_begin;

    for (; u <= p->units_end; u++) {
        unsigned int flags = u->flags;
        if ((flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0
            && (u->owner == 0 || (u->owner->flags & 0x40000000))) {
            unsigned short bits = u->field_a6;
            unsigned int bit = 1 << (bits & 0x1f);
            if (mask[bits >> 5] & bit) {
                u->flags = flags | 0x10;
            } else if (param_2 == 0) {
                u->flags = flags & ~0x10;
            }
        }
    }
    g_game->unitIndex = 0;
    FUN_00495860();
    g_game->orders.flag_4 = 1;
}
