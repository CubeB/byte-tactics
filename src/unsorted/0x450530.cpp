// Decompiled by mimo-v2.6-pro. Names are provisional.

#pragma pack(push, 1)
struct PlayerData_00450530 {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97
};

struct Player_00450530 {
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0xc - 0x8];
    int field_c;                       // +0x0c
    char unknown_10[0x27 - 0x10];
    PlayerData_00450530* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;               // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char f_146;               // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_00450530 {
    char unknown_0[0x1b63];
    Player_00450530 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char local_player;        // +0x2a42
    char unknown_2a43[0x391f1 - 0x2a43];
    int mode;                          // +0x391f1
};

struct Msg_00450530 {
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int id;                            // +0x2
    int arg;                           // +0x6
};
#pragma pack(pop)

extern Game_00450530* g_game;

int __stdcall FUN_0044ffd0(unsigned char index);
int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);
unsigned char FUN_00456850();

// FUNCTION: 0x450530
void FUN_00450530()
{
    Msg_00450530 msg;
    if (g_game->mode != 6) {
        for (int i = 0; i < 10; i++) {
            Player_00450530* p = &g_game->players[i];
            if (p->active != 0
                && (p->state == 1 || p->state == 2 || p->state == 3)
                && p->f_146 != 10
                && p->field_c == 0) {
                if (p->state == 1) {
                    int w = FUN_00456850();
                    if (g_game->players[w].active != 0
                        && (g_game->players[w].state == 1 || g_game->players[w].state == 2)) {
                        p->field_c = 1;
                        continue;
                    }
                    msg.type = 0x21;
                    msg.flag = 1;
                    msg.id = p->id;
                    msg.arg = -1;
                    if (FUN_00456850() == 10)
                        continue;
                    int to = -1;
                    for (int j = 0; j < 10; j++) {
                        if (g_game->players[j].data->flags & 1) {
                            to = FUN_0044ffd0(j);
                            break;
                        }
                    }
                    int from = -1;
                    for (int k = 0; k < 10; k++) {
                        if (g_game->players[k].active != 0
                            && (g_game->players[k].state == 1 || g_game->players[k].state == 2)) {
                            from = g_game->players[k].id;
                            break;
                        }
                    }
                    FUN_00451bc0(from, to, &msg, 10);
                } else if (p->state == 2) {
                    int w = FUN_00456850();
                    if (g_game->players[w].active != 0
                        && (g_game->players[w].state == 1 || g_game->players[w].state == 2)) {
                        p->field_c = 1;
                        continue;
                    }
                    msg.type = 0x21;
                    msg.flag = 1;
                    msg.id = p->id;
                    msg.arg = g_game->players[g_game->local_player].id;
                    if (FUN_00456850() == 10)
                        continue;
                    int to = -1;
                    for (int j = 0; j < 10; j++) {
                        if (g_game->players[j].data->flags & 1) {
                            to = FUN_0044ffd0(j);
                            break;
                        }
                    }
                    int from = -1;
                    for (int k = 0; k < 10; k++) {
                        if (g_game->players[k].active != 0
                            && (g_game->players[k].state == 1 || g_game->players[k].state == 2)) {
                            from = g_game->players[k].id;
                            break;
                        }
                    }
                    FUN_00451bc0(from, to, &msg, 10);
                } else if (p->state == 3) {
                    msg.type = 0x21;
                    msg.flag = 0;
                    msg.id = p->id;
                    msg.arg = -1;
                    if (FUN_00456850() == 10)
                        continue;
                    int to = -1;
                    for (int j = 0; j < 10; j++) {
                        if (g_game->players[j].data->flags & 1) {
                            to = FUN_0044ffd0(j);
                            break;
                        }
                    }
                    int from = -1;
                    for (int k = 0; k < 10; k++) {
                        if (g_game->players[k].active != 0
                            && (g_game->players[k].state == 1 || g_game->players[k].state == 2)) {
                            from = g_game->players[k].id;
                            break;
                        }
                    }
                    FUN_00451bc0(from, to, &msg, 10);
                }
            }
        }
    }
}
