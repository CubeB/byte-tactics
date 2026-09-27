// Decompiled by Opus. Names are provisional.
// Command callback: calls FUN_00486f10 with 0 when the local player's byte
// at +0x95 of its data block is 1, otherwise with 1, and sets game flags at
// +0x3923b; compare 0x416a30.

#pragma pack(push, 1)
struct PlayerData_004169d0 {
    char unknown_0[0x95];
    unsigned char field_95;            // +0x95
};

struct Player_004169d0 {
    char unknown_0[0x27];
    PlayerData_004169d0* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game_004169d0 {
    char unknown_0[0x1b63];
    Player_004169d0 players[10];       // +0x1b63
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

extern Game_004169d0* g_game;

// Command arguments.
class Class_004b73e0;

void __stdcall FUN_00486f10(unsigned char player);

// FUNCTION: 0x4169d0
void __stdcall FUN_004169d0(Class_004b73e0* args)
{
    if (g_game->players[g_game->localPlayer].data->field_95 == 1)
        FUN_00486f10(0);
    else
        FUN_00486f10(1);
    g_game->flag4 = 1;
    g_game->flag5 = 1;
    g_game->flag2 = 1;
}
