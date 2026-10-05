// Decompiled by Opus. Names are provisional.
// Returns field +4 of a player slot (index 10 means none), or -1 when the
// slot is unused; the pointer version is 0x450010.

#pragma pack(push, 1)
struct Player_0044ffd0 {
    char unknown_0[4];
    int field_4;                       // +0x04
    char unknown_8[0x73 - 0x8];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_0044ffd0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x44ffd0
int __stdcall GetSlotDpid(unsigned char index)
{
    if (index != 10 && g_game->players[index].type != 0)
        return g_game->players[index].field_4;
    return -1;
}
