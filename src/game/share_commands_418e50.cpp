// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Chat command: toggles the local player's ShareMapping flag and prints the
// new state.
#include <stdio.h>

#pragma pack(push, 1)
struct PlayerData_00418e50 {
    char unknown_0[0x97];
    unsigned short flags;              // +0x97
};

struct Player_00418e50 {
    char unknown_0[0x27];
    PlayerData_00418e50* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00418e50 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char local_player;        // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    unsigned char flags_2a44;          // +0x2a44
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall AddMessage(char* param_1, int param_2, int param_3, int param_4);
void BroadcastPlayerInfo(void);

// FUNCTION: 0x418e50
void __stdcall CmdShareMapping(int unused)
{
    char buf[256];
    if (g_game->flags_2a44 & 1) {
        PlayerData_00418e50* data = g_game->players[g_game->local_player].data;
        data->flags = (data->flags & ~0x20) | (~data->flags & 0x20);
        sprintf(buf, "Toggled ShareMapping to: %s",
                (g_game->players[g_game->local_player].data->flags & 0x20)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}
