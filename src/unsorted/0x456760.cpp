// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct PlayerInfo_00456760 {
    char unknown_0[0x9b];
    unsigned char field_9b;            // +0x9b
};

struct Player_00456760 {
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00456760* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00456760 {
    char unknown_0[0x1b63];
    Player_00456760 players[10];       // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    short field_2a3c;                  // +0x2a3c
};
#pragma pack(pop)

extern Game_00456760* g_game;

// FUNCTION: 0x456760
bool FUN_00456760(void)
{
    int count = 0;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0 && g_game->players[i].type == 3)
            count++;
    }
    if (count == 0)
        return false;

    bool all = true;
    for (unsigned char j = 0; j < 10; j++) {
        Player_00456760* p = &g_game->players[j];
        if (p->active != 0
            && (p->type == 1 || p->type == 2
                || (p->active != 0 && p->type == 3))) {
            if ((p->data->field_9b & 0x20) == 0)
                return false;
        } else {
            all = false;
            continue;
        }
        if (p->active == 0 || (p->data->field_9b & 0x40) == 0)
            all = false;
    }
    if (g_game->field_2a3c == 1)
        return false;
    return !all;
}