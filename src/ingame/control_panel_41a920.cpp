// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Builds the "<unit name>PREV" / "<unit name>NEXT" gadget names for the
// skeleton of the unit's owner and pushes them into the GUI.
#include <stdio.h>

#pragma pack(push, 1)
struct Owner_0041a920 {
    char unknown_0[0x95];
    unsigned char playerIndex;         // +0x95
};

struct Link_0041a920 {
    char unknown_0[0x27];
    Owner_0041a920* owner;             // +0x27
};

struct Info_0041a920 {
    char unknown_0[0x22e];
    unsigned char field_22e;           // +0x22e
};

struct Unit {
    char unknown_0[0x92];
    Info_0041a920* info;               // +0x92
    Link_0041a920* link;               // +0x96
};

struct Player_0041a920 {
    char name[0x232];                  // +0x00
};

struct Object_0041a920 {
    char unknown_0[0x18];
    void* data;                        // +0x18
};

struct Game {
    char unknown_0[0x519];
    Object_0041a920 obj;               // +0x519
    char unknown_519[0x37f5b - 0x519 - sizeof(Object_0041a920)];
    Player_0041a920 players[10];       // +0x37f5b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004a0570(Object_0041a920* obj, char* name, int param_3);

// FUNCTION: 0x41a920
void __stdcall SetPrevNextGadgetNames(Unit* unit)
{
    char buf[256];
    if (unit->info->field_22e < 2) {
        sprintf(buf, "%sPREV", g_game->players[unit->link->owner->playerIndex].name);
        FUN_004a0570(&g_game->obj, buf, 0);
        sprintf(buf, "%sNEXT", g_game->players[unit->link->owner->playerIndex].name);
        FUN_004a0570(&g_game->obj, buf, 0);
    }
}
