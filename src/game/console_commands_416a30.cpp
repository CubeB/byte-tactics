// Decompiled by Opus. Names are provisional.
// Command callback: passes the local player's byte at +0x95 of its data
// block to KillPlayerUnits and updates the game flags at +0x3923b; compare
// 0x4169d0.

#pragma pack(push, 1)
struct PlayerData_00416a30 {
    char unknown_0[0x95];
    unsigned char field_95;            // +0x95
};

struct Player_00416a30 {
    char unknown_0[0x27];
    PlayerData_00416a30* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00416a30 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x3923b - 0x2a43];
    unsigned short bits_3923b : 2;     // +0x3923b
    unsigned short flag2 : 1;
    unsigned short bit3 : 1;
    unsigned short flag4 : 1;
    unsigned short flag5 : 1;
    unsigned short flag6 : 1;
    unsigned short rest : 9;
};
#pragma pack(pop)

extern Game* g_game;

// Command arguments.
class Class_004b73e0;

void __stdcall KillPlayerUnits(unsigned char player);

// FUNCTION: 0x416a30
void __stdcall FUN_00416a30(Class_004b73e0* args)
{
    KillPlayerUnits(g_game->players[g_game->localPlayer].data->field_95);
    g_game->flag4 = 0;
    g_game->flag6 = 1;
    g_game->flag2 = 1;
}
