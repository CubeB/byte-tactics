// Decompiled by Opus. Names are provisional.
#include <stdio.h>

struct Class_004a1080;

struct PlayerInfo_448bf0 {
    char unknown_0[0x95];
    unsigned char f_95;              // +0x95
    char unknown_96[0x9b - 0x96];
    unsigned char flags_9b;          // +0x9b
};

#pragma pack(push, 1)
struct Player_448bf0 {
    int active;                      // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_448bf0* info;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x519];
    char unknown_519[0x1b63 - 0x519]; // +0x519
    Player_448bf0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall SetButtonStageByName(Class_004a1080* obj, char* name, char value);

// FUNCTION: 0x448bf0
void __stdcall FUN_00448bf0(int side)
{
    char name[20];
    Player_448bf0* p = &g_game->players[side];

    sprintf(name, "SIDE%d", side);
    SetButtonStageByName((Class_004a1080*)g_game->unknown_519, name,
                 (p->active != 0 && (p->info->flags_9b & 0x40)) ? 2 : p->info->f_95);
}
