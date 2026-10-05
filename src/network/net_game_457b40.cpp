// Decompiled by Opus. Names are provisional.

struct Struct_00457b40 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
};

#pragma pack(push, 1)
struct Player_00457b40 {
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    Struct_00457b40* data;             // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00457b40 {
    char unknown_0[0x1b63];
    Player_00457b40 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game_00457b40* g_game;

// FUNCTION: 0x457b40
int FUN_00457b40()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if ((g_game->players[i].active != 0 && g_game->players[i].type == 2)
            || (g_game->players[i].active != 0 && g_game->players[i].type == 3
                && g_game->players[i].data->field_94 == 2))
            count++;
    }
    return count;
}
