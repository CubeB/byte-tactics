// Decompiled by Opus. Names are provisional.
// Reads each player's "Controller" value from its "Player%i" section.
#include <stdio.h>

#pragma pack(push, 1)
struct Player_00465fb0 {
    char unknown_0[0x73];
    char controller;                   // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Slot_00465fb0 {
    int controller;                    // +0x0
    char unknown_4[0x18 - 0x4];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00465fb0 players[10];       // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Slot_00465fb0* slots;              // +0x29a0
};
#pragma pack(pop)

extern Game* g_game;

class HapiBank {
public:
    int OpenAccount(char* name);
    int GetIntegerItem(char* name, int def);
};

// FUNCTION: 0x465fb0
void __stdcall LoadPlayerControllers(HapiBank* file)
{
    char name[16];
    for (int i = 0; i < 10; i++) {
        Player_00465fb0* player = &g_game->players[i];
        int* slot = &g_game->slots[i].controller;
        sprintf(name, "Player%i", i);
        if (file->OpenAccount(name)) {
            *slot = ((HapiBank*)file)->GetIntegerItem("Controller", 0);
            player->controller = *slot;
        } else {
            *slot = 0;
            player->controller = 0;
        }
    }
}
