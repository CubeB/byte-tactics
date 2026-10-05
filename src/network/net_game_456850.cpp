// Decompiled by Opus. Names are provisional.

struct Struct_00456850 {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97
};

#pragma pack(push, 1)
struct Player_00456850 {
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    Struct_00456850* data;             // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00456850 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x456850
unsigned char FUN_00456850()
{
    unsigned char i;
    for (i = 0; i < 10; i++) {
        if (g_game->players[i].type != 0 && (g_game->players[i].data->flags & 1))
            return i;
    }
    return 10;
}
