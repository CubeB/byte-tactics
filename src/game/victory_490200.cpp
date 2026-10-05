// Decompiled by Opus. Names are provisional.
// Whether the word at +0x144 of the local player's record is zero.

#pragma pack(push, 1)
struct Player_00490200 {
    char unknown_0[0x144];
    short field_144;                   // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game_00490200 {
    char unknown_0[0x1b63];
    Player_00490200 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char field_2a42;          // +0x2a42
};
#pragma pack(pop)

extern Game_00490200* g_game;

// FUNCTION: 0x490200
int FUN_00490200()
{
    return g_game->players[g_game->field_2a42].field_144 == 0 ? 1 : 0;
}
