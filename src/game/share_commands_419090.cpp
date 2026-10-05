// Decompiled by Claude Opus 5.5. Names are provisional.
// Chat command: toggles the local player's ShareMetal, ShareEnergy,
// ShareMapping and ShareRadar flags in turn. The four toggles are the chat
// commands 0x418cd0, 0x418d90, 0x418e50 and 0x418fd0, inlined. The last one
// must be the mask form with a pointer local (as in 0x418d90): the `!` on a
// bitfield form gives its block different registers from the other three.
#include <stdio.h>

#pragma pack(push, 1)
struct PlayerData_00419090 {
    char unknown_0[0x97];
    unsigned short flags;              // +0x97
};

struct Player_00419090 {
    char unknown_0[0x27];
    PlayerData_00419090* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00419090 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    unsigned char flags;               // +0x2a44
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall AddMessage(char* param_1, int param_2, int param_3, int param_4);
void BroadcastPlayerInfo();

static inline void ShareMetal(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        PlayerData_00419090* data = g_game->players[g_game->localPlayer].data;
        data->flags = (data->flags & ~2) | (~data->flags & 2);
        sprintf(buf, "Toggled ShareMetal to: %s",
                (g_game->players[g_game->localPlayer].data->flags & 2)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

static inline void ShareEnergy(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        PlayerData_00419090* data = g_game->players[g_game->localPlayer].data;
        data->flags = (data->flags & ~4) | (~data->flags & 4);
        sprintf(buf, "Toggled ShareEnergy to: %s",
                (g_game->players[g_game->localPlayer].data->flags & 4)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

static inline void ShareMapping(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        PlayerData_00419090* data = g_game->players[g_game->localPlayer].data;
        data->flags = (data->flags & ~0x20) | (~data->flags & 0x20);
        sprintf(buf, "Toggled ShareMapping to: %s",
                (g_game->players[g_game->localPlayer].data->flags & 0x20)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

static inline void ShareRadar(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        PlayerData_00419090* data = g_game->players[g_game->localPlayer].data;
        data->flags = (data->flags & ~0x40) | (~data->flags & 0x40);
        sprintf(buf, "Toggled ShareRadar to: %s",
                (g_game->players[g_game->localPlayer].data->flags & 0x40)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

// FUNCTION: 0x419090
void __stdcall FUN_00419090(int unused)
{
    if (g_game->flags & 1) {
        ShareMetal(unused);
        ShareEnergy(unused);
        ShareMapping(unused);
        ShareRadar(unused);
    }
}
