// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Chat command: toggles the local player's ShareMetal bit and prints the new
// state ("ON"/"OFF"); compare the sibling toggles 0x4194d0 and 0x419400.
#include <stdio.h>

#pragma pack(push, 1)
struct PlayerData_00418cd0 {
    char unknown_0[0x97];
    unsigned short unknownBit0 : 1;    // +0x97 bit 0
    unsigned short shareMetal : 1;     // +0x97 bit 1
    unsigned short unknownRest : 14;
};

struct Player_00418cd0 {
    char unknown_0[0x27];
    PlayerData_00418cd0* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00418cd0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2a44 - 0x2a43];
    unsigned short flags;              // +0x2a44
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall AddMessage(char* param_1, int param_2, int param_3, int param_4);
void BroadcastPlayerInfo();

// FUNCTION: 0x418cd0
void __stdcall FUN_00418cd0(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        g_game->players[g_game->localPlayer].data->shareMetal =
            !g_game->players[g_game->localPlayer].data->shareMetal;
        sprintf(buf, "Toggled ShareMetal to: %s",
                (g_game->players[g_game->localPlayer].data->shareMetal != 0)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}
