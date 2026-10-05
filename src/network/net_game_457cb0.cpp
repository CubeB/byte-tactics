// Decompiled by Opus. Names are provisional.

struct Struct_00457cb0 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
    char unknown_95[0x9b - 0x95];
    unsigned char flags;               // +0x9b
};

#pragma pack(push, 1)
struct Player_00457cb0 {
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    Struct_00457cb0* data;             // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x140 - 0x74];
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_00457cb0 {
    char unknown_0[0x1b63];
    Player_00457cb0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game_00457cb0* g_game;

// FUNCTION: 0x457cb0
int FUN_00457cb0()
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2 || g_game->players[i].type == 3)
            && g_game->players[i].field_146 != 10
            && (g_game->players[i].field_144 != 0 || g_game->players[i].field_140 == 0)
            && (g_game->players[i].type == 1
                || (g_game->players[i].type == 3 && g_game->players[i].data->field_94 == 1))
            && !(g_game->players[i].data->flags & 0x40))
            count++;
    }
    return count;
}
