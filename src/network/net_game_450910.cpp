// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Player_450910 {
    int active;                      // +0x00
    char unknown_4[8];
    int id;                          // +0x0c
    char unknown_10[0x73 - 0x10];
    char type;                       // +0x73
    char unknown_74[0x146 - 0x74];
    char f_146;                      // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_450910 players[10];       // +0x1b63
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

// FUNCTION: 0x450910
char FUN_00450910(void)
{
    Game* game = g_game;
    for (int id = 1; id <= 10; id++) {
        int used = 0;
        for (int i = 0; i < 10; i++) {
            Player_450910* p = &game->players[i];
            if (p->active != 0
                && (p->type == 1 || p->type == 2 || p->type == 3)
                && p->f_146 != 10
                && p->id == id) {
                used = 1;
            }
        }
        if (!used) {
            return id;
        }
    }
    return 0;
}
