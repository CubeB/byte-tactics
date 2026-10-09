// Decompiled by DeepSeek V4.1 Flash, Claude Opus 5.5, Opus and Haiku. Names are provisional.
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
#include "../network/player.h"
#include "../network/player_info.h"

struct Game {
    char unknown_0[0x4ed];
    int compressionOff;                // +0x4ed
    char unknown_4f1[0x1b63 - 0x4f1];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    unsigned char playerIndex;         // +0x2a43
    unsigned char flags;               // +0x2a44
    char unknown_2a45[0x391bf - 0x2a45];
    int showRanges;                    // +0x391bf
    int showBps;                       // +0x391c3
};
#pragma pack(pop)

extern Game* g_game;

// Command arguments.
class CommandArgs {
public:
    int GetIntArg(int index, int fallback);
};

void __stdcall AddMessage(char* param_1, int param_2, int param_3, int param_4);
void BroadcastPlayerInfo();

extern char g_fxEventPoolBlocked;

// Chat command: toggles the local player's ShareMetal bit and prints the new
// state ("ON"/"OFF"); compare the sibling toggles 0x4194d0 and 0x419400.
// FUNCTION: 0x418cd0
void __stdcall CmdShareMetal(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        g_game->players[g_game->localPlayer].info->shareMetal =
            !g_game->players[g_game->localPlayer].info->shareMetal;
        sprintf(buf, "Toggled ShareMetal to: %s",
                (g_game->players[g_game->localPlayer].info->shareMetal != 0)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

// Chat command: toggles the local player's energy-sharing flag and prints the
// new state.
// FUNCTION: 0x418d90
void __stdcall CmdShareEnergy(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        PlayerInfo* data = g_game->players[g_game->localPlayer].info;
        data->flags_97_wide = (data->flags_97_wide & ~4) | (~data->flags_97_wide & 4);
        sprintf(buf, "Toggled ShareEnergy to: %s",
                (g_game->players[g_game->localPlayer].info->flags_97_wide & 4)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

// Chat command: toggles the local player's ShareMapping flag and prints the
// new state.
// FUNCTION: 0x418e50
void __stdcall CmdShareMapping(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        PlayerInfo* data = g_game->players[g_game->localPlayer].info;
        data->flags_97_wide = (data->flags_97_wide & ~0x20) | (~data->flags_97_wide & 0x20);
        sprintf(buf, "Toggled ShareMapping to: %s",
                (g_game->players[g_game->localPlayer].info->flags_97_wide & 0x20)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

// Chat command: toggles the local player's ShareLOS bit and prints the state.
// FUNCTION: 0x418f10
void __stdcall ToggleShareLos(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        g_game->players[g_game->localPlayer].info->shareLOS =
            !g_game->players[g_game->localPlayer].info->shareLOS;
        sprintf(buf, "Toggled ShareLOS to: %s",
                (g_game->players[g_game->localPlayer].info->shareLOS != 0)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

// Chat command: toggles the local player's ShareRadar flag and prints the
// new state.
// FUNCTION: 0x418fd0
void __stdcall CmdShareRadar(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        g_game->players[g_game->localPlayer].info->shareRadar =
            !g_game->players[g_game->localPlayer].info->shareRadar;
        sprintf(buf, "Toggled ShareRadar to: %s",
                (g_game->players[g_game->localPlayer].info->flags_97_wide & 0x40)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

// Chat command: toggles the local player's ShareMetal, ShareEnergy,
// ShareMapping and ShareRadar flags in turn. The four toggles are the chat
// commands 0x418cd0, 0x418d90, 0x418e50 and 0x418fd0, inlined. The last one
// must be the mask form with a pointer local (as in 0x418d90): the `!` on a
// bitfield form gives its block different registers from the other three.
static inline void ShareMetal(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        PlayerInfo* data = g_game->players[g_game->localPlayer].info;
        data->flags_97_wide = (data->flags_97_wide & ~2) | (~data->flags_97_wide & 2);
        sprintf(buf, "Toggled ShareMetal to: %s",
                (g_game->players[g_game->localPlayer].info->flags_97_wide & 2)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

static inline void ShareEnergy(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        PlayerInfo* data = g_game->players[g_game->localPlayer].info;
        data->flags_97_wide = (data->flags_97_wide & ~4) | (~data->flags_97_wide & 4);
        sprintf(buf, "Toggled ShareEnergy to: %s",
                (g_game->players[g_game->localPlayer].info->flags_97_wide & 4)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

static inline void ShareMapping(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        PlayerInfo* data = g_game->players[g_game->localPlayer].info;
        data->flags_97_wide = (data->flags_97_wide & ~0x20) | (~data->flags_97_wide & 0x20);
        sprintf(buf, "Toggled ShareMapping to: %s",
                (g_game->players[g_game->localPlayer].info->flags_97_wide & 0x20)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

static inline void ShareRadar(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        PlayerInfo* data = g_game->players[g_game->localPlayer].info;
        data->flags_97_wide = (data->flags_97_wide & ~0x40) | (~data->flags_97_wide & 0x40);
        sprintf(buf, "Toggled ShareRadar to: %s",
                (g_game->players[g_game->localPlayer].info->flags_97_wide & 0x40)
                    ? "ON" : "OFF");
        AddMessage(buf, 2, 0, 10);
        BroadcastPlayerInfo();
    }
}

// FUNCTION: 0x419090
void __stdcall CmdShareAll(int unused)
{
    if (g_game->flags & 1) {
        ShareMetal(unused);
        ShareEnergy(unused);
        ShareMapping(unused);
        ShareRadar(unused);
    }
}

// Chat command: sets the local player's metal-sharing threshold to the
// argument, capped at metalCapacity, and prints a confirmation.
// FUNCTION: 0x419340
void __stdcall CmdSetShareMetal(CommandArgs* args)
{
    char buf[256];
    if (g_game->flags & 1) {
        Player* p = &g_game->players[g_game->localPlayer];
        // __min macro with an explicit (float) cast: the cast places the store
        // after the next call's pushes.
        p->shareMetal = __min(p->metalCapacity, (float)args->GetIntArg(1, 0));
        sprintf(buf, "OK.  Will share metal if above %d", args->GetIntArg(1, 0));
        AddMessage(buf, 2, 0, 10);
    }
}

// Chat command: sets the local player's energy-sharing threshold to the
// argument, capped at energyCapacity (a min() macro, so the argument is read twice),
// and prints a confirmation.
// FUNCTION: 0x419400
void __stdcall CmdSetShareEnergy(CommandArgs* args)
{
    char buf[256];
    if (g_game->flags & 1) {
        Player* p = &g_game->players[g_game->localPlayer];
        // __min macro with an explicit (float) cast: the cast places the store
        // after the next call's pushes.
        p->shareEnergy = __min(p->energyCapacity, (float)args->GetIntArg(1, 0));
        sprintf(buf, "OK.  Will share energy if above %d", args->GetIntArg(1, 0));
        AddMessage(buf, 2, 0, 10);
    }
}

// FUNCTION: 0x4194c0
void __stdcall CmdShowRanges(int unused)
{
    int* ptr = &g_game->showRanges;
    *ptr ^= 1;
}

// Chat command: toggles outgoing packet compression and prints the new state.
// FUNCTION: 0x4194d0
void __stdcall CmdCompression(int unused)
{
    char buf[256];
    if (g_game->flags & 1) {
        g_game->compressionOff = (g_game->compressionOff == 0);
        sprintf(buf, "Ok.  Outgoing packet compression turned %s",
                g_game->compressionOff ? "OFF" : "ON");
        AddMessage(buf, 2, 0, 10);
    }
}

// FUNCTION: 0x419540
void __stdcall CmdBPS(int unused)
{
    g_game->showBps ^= 1;
}

// FUNCTION: 0x419550
void __stdcall CmdSFX(int arg1)
{
    g_fxEventPoolBlocked ^= 1;
}
