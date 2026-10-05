// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Chat command: toggles the local player's ShareRadar flag and prints the
// new state.
#include <stdio.h>

#pragma pack(push, 1)
struct Flags_418fd0 {
    unsigned short unknown_97_0 : 6;     // +0x97
    unsigned short share_radar : 1;      // bit 6
    unsigned short unknown_97_7 : 9;
};

struct PlayerInfo_418fd0 {
    char unknown_0[0x97];
    union {
        unsigned short flags;            // +0x97
        Flags_418fd0 bits;
    };
};

struct Player_418fd0 {
    char unknown_0[0x27];
    PlayerInfo_418fd0* info;             // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x1b63];
    Player_418fd0 players[10];           // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;           // +0x2a42
    char unknown_2a43[0x2a44 - 0x2a43];
    unsigned short flags;                // +0x2a44
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall AddMessage(char* text, int param_2, int param_3, int param_4);
void BroadcastPlayerInfo();

// FUNCTION: 0x418fd0
void __stdcall FUN_00418fd0(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        g_game->players[g_game->localPlayer].info->bits.share_radar =
            !g_game->players[g_game->localPlayer].info->bits.share_radar;
        sprintf(buf, "Toggled ShareRadar to: %s",
                (g_game->players[g_game->localPlayer].info->flags & 0x40)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}
