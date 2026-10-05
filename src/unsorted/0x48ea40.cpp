// Decompiled by Opus. Names are provisional.
// Slot 1 of the "kill enemy commander" victory condition (vtable 0x4fd960,
// compare 0x48f6b0 and 0x48ec20): when a unit of kind 1 is named after its
// owner, marks the condition met and announces it once.
#include <string.h>

#pragma pack(push, 1)
struct Info_0048ea40 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Owner_0048ea40 {
    char unknown_0[0x95];
    unsigned char playerIndex;         // +0x95
};

struct Link_0048ea40 {
    char unknown_0[0x27];
    Owner_0048ea40* owner;             // +0x27
};

struct Unit {
    char unknown_0[0x92];
    Info_0048ea40* info;               // +0x92
    Link_0048ea40* link;               // +0x96
    char unknown_9a[0xff - 0x9a];
    unsigned char kind;                // +0xff
};

struct Player_0048ea40 {
    char name[0x232];                  // +0x00
};

struct Game_0048ea40 {
    char unknown_0[0x37f5f];
    Player_0048ea40 players[10];       // +0x37f5f
};
#pragma pack(pop)

extern Game_0048ea40* g_game;

void __stdcall FUN_0047f1a0(char* str, int flag);

class Class_0048ea00 {
public:
    int done;                          // +0x04
    int announced;                     // +0x08
    virtual void FUN_0048ea10(Unit* unit);
};

// FUNCTION: 0x48ea40
void Class_0048ea00::FUN_0048ea10(Unit* unit)
{
    if (unit->kind == 1) {
        if (_strcmpi(unit->info->name, g_game->players[unit->link->owner->playerIndex].name) == 0) {
            done = 1;
            if (announced == 0) {
                FUN_0047f1a0("Victory Condition", 0);
                announced = 1;
            }
        }
    }
}
