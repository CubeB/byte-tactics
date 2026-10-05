// Decompiled by space-bunny-free. Names are provisional.
// Scans the local player's unit list for the same "build finished and the
// builder is dead or gone" state the victory condition check in 0x48f250
// tests, marks each such unit with flag 0x10 and drops the current selection
// (+0x37e9c) to none, then issues the STOP order and sets order flag 0x10.

#pragma pack(push, 1)
struct Unit_0048bd50 {                   // 0x118 bytes
    char unknown_0[0x86];
    Unit_0048bd50* owner;               // +0x86
    char unknown_8a[0xfb - 0x8a];
    int field_fb;                       // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                    // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                 // +0x110
};

struct Player_0048bd50 {                 // 0x14b bytes
    char unknown_0[0x67];
    Unit_0048bd50* unitsBegin;          // +0x67
    Unit_0048bd50* unitsEnd;            // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048bd50 {
    char unknown_0[0x1b63];
    Player_0048bd50 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x37e9c - 0x2a43];
    unsigned short unitIndex;           // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned char field_37ebe;          // +0x37ebe
};
#pragma pack(pop)

extern Game_0048bd50* g_game;

void FUN_00495860(void);

// FUNCTION: 0x48bd50
void FUN_0048bd50(void)
{
    Player_0048bd50* pl = &g_game->players[g_game->localPlayer];
    Unit_0048bd50* u = pl->unitsBegin;
    if (u <= pl->unitsEnd) {
        do {
            if ((u->flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0
                && (u->owner == 0 || (u->owner->flags & 0x40000000))) {
                u->flags |= 0x10;
                g_game->unitIndex = 0;
            }
            u = (Unit_0048bd50*)((char*)u + 0x118);
        } while (u <= pl->unitsEnd);
    }
    FUN_00495860();
    g_game->field_37ebe |= 0x10;
}
