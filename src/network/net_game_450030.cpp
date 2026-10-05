// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

#pragma pack(push, 1)
struct Info_00450030 {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97
};

struct Player_00450030 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    char unknown_8[0x27 - 0x8];
    Info_00450030* info;               // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00450030 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

static inline int PlayerField(unsigned char index)
{
    if (index != 10 && g_game->players[index].type != 0)
        return g_game->players[index].field_4;
    return -1;
}

// FUNCTION: 0x450030
int GetHostDpid()
{
    int i;
    for (i = 0; i < 10; i++) {
        if (g_game->players[i].info->flags & 1)
            return PlayerField(i);
    }
    return -1;
}
