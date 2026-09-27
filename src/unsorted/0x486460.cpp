// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Info_00486460 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Owner_00486460 {
    char unknown_0[0x95];
    unsigned char playerIndex;         // +0x95
};

struct Link_00486460 {
    char unknown_0[0x27];
    Owner_00486460* owner;             // +0x27
};

struct Unit_00486460 {
    char unknown_0[0x92];
    Info_00486460* info;               // +0x92
    Link_00486460* link;               // +0x96
};

struct Player_00486460 {
    char name[0x232];                  // +0x00
};

struct Game_00486460 {
    char unknown_0[0x37f5f];
    Player_00486460 players[10];       // +0x37f5f
};
#pragma pack(pop)

extern Game_00486460* g_game;

// FUNCTION: 0x486460
int __stdcall FUN_00486460(Unit_00486460* unit)
{
    return _strcmpi(g_game->players[unit->link->owner->playerIndex].name, unit->info->name) == 0;
}
