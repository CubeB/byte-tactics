// Decompiled by Opus. Names are provisional.
// Command callback (cheat): adds 1000 metal and 1000 energy to the local
// player.

#pragma pack(push, 1)
struct Player_004170c0 {
    char unknown_0[0x8c];
    float metal;                       // +0x8c
    char unknown_90[0x98 - 0x90];
    float energy;                      // +0x98
    char unknown_9c[0x14b - 0x9c];
};

struct Game {
    char unknown_0[0x1b63];
    Player_004170c0 players[10];       // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char localPlayer;         // +0x2a43
};
#pragma pack(pop)

extern Game* g_game;

// Command arguments.
class CommandArgs;

// FUNCTION: 0x4170c0
void __stdcall CmdATM(CommandArgs* args)
{
    g_game->players[g_game->localPlayer].metal += 1000.0f;
    g_game->players[g_game->localPlayer].energy += 1000.0f;
}
