// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2a42];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37ebe - 0x2a43];
    unsigned short flags;              // +0x37ebe
};

struct Unit {
    char unknown_0[0xff];
    unsigned char player;              // +0xff
    char unknown_100[0x110 - 0x100];
    unsigned char flags;               // +0x110
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x41c110
void __stdcall FUN_0041c110(Unit* unit)
{
    if (unit->player == g_game->localPlayer && (unit->flags & 0x10)) {
        g_game->flags |= 0x10;
    }
}
