// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Chat command: toggles the local player's ShareLOS bit and prints the state.
#include <stdio.h>

#pragma pack(push, 1)
struct ShareFlags_00418f10 {
    unsigned short unknown_0 : 3;
    unsigned short shareLOS : 1;       // +0x97, bit 3
    unsigned short unknown_1 : 12;
};

struct PlayerData_00418f10 {
    char unknown_0[0x97];
    ShareFlags_00418f10 flags;         // +0x97
};

struct Player_00418f10 {
    int active;                        // +0x0
    int id;                            // +0x4
    char unknown_8[0x27 - 0x8];
    PlayerData_00418f10* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game_00418f10 {
    char unknown_0[0x1b63];
    Player_00418f10 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char local_player;        // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    unsigned char flags;               // +0x2a44
};
#pragma pack(pop)

extern Game_00418f10* g_game;

void __stdcall FUN_00463ca0(char* param_1, int param_2, int param_3, int param_4);
void FUN_00450f90();

// FUNCTION: 0x418f10
void __stdcall FUN_00418f10(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        g_game->players[g_game->local_player].data->flags.shareLOS =
            !g_game->players[g_game->local_player].data->flags.shareLOS;
        sprintf(buf, "Toggled ShareLOS to: %s",
                (g_game->players[g_game->local_player].data->flags.shareLOS != 0)
                    ? "ON" : "OFF");
        FUN_00463ca0(buf, 2, 0, 10);
        FUN_00450f90();
    }
}
