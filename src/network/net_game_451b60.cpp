// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Player_00451b60 {
    int active;                        // +0x00
    int field_4;                       // +0x04
    char unknown_8[0x73 - 0x8];
    char type;                         // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00451b60 players[10];       // +0x1b63
    char unknown_2851[0x2a44 - 0x2851];
    unsigned short flags_2a44;         // +0x2a44
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall RemovePlayer(int index);
void ShutdownScoreTables();

// FUNCTION: 0x451b60
void RemoveLocalPlayers()
{
    if (!(g_game->flags_2a44 & 1))
        return;
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0
            && (g_game->players[i].type == 1 || g_game->players[i].type == 2)) {
            RemovePlayer(g_game->players[i].field_4);
        }
    }
    ShutdownScoreTables();
}
