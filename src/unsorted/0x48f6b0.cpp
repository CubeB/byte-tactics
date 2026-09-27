// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Info_0048f6b0 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Owner_0048f6b0 {
    char unknown_0[0x95];
    unsigned char playerIndex;         // +0x95
};

struct Link_0048f6b0 {
    char unknown_0[0x27];
    Owner_0048f6b0* owner;             // +0x27
};

struct Unit_0048f6b0 {
    char unknown_0[0x92];
    Info_0048f6b0* info;               // +0x92
    Link_0048f6b0* link;               // +0x96
    char unknown_9a[0xff - 0x9a];
    unsigned char kind;                // +0xff
};

struct Player_0048f6b0 {
    char name[0x232];                  // +0x00
};

struct Game_0048f6b0 {
    char unknown_0[0x37f5f];
    Player_0048f6b0 players[10];       // +0x37f5f
};
#pragma pack(pop)

extern Game_0048f6b0* g_game;

class Class_0048f6b0 {
public:
    int unknown_0;                     // +0x00
    int done;                          // +0x04
    void FUN_0048f6b0(Unit_0048f6b0* unit);
};

// FUNCTION: 0x48f6b0
void Class_0048f6b0::FUN_0048f6b0(Unit_0048f6b0* unit)
{
    if (unit->kind == 0) {
        if (_strcmpi(unit->info->name, g_game->players[unit->link->owner->playerIndex].name) == 0) {
            done = 1;
        }
    }
}
