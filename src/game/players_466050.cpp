// Decompiled by Claude Opus 5.5. Names are provisional.
#include <stdio.h>

// Two functions with aligned frames (`and esp, -8`) under the default flags,
// no /Op: the doubles they pass and receive are enough for MSVC to align.

#pragma pack(push, 1)
struct PlayerData_00466050 {
    char unknown_0[0x95];
    unsigned char side;                // +0x95
    unsigned char logo;                // +0x96
};

struct Player_00466050 {               // 0x14b bytes
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerData_00466050* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char controller;          // +0x73
    char unknown_74[0x8c - 0x74];
    float energy;                      // +0x8c
    char unknown_90[0x98 - 0x90];
    float metal;                       // +0x98
    char unknown_9c[0xac - 0x9c];
    double totalEnergyProduced;        // +0xac
    double totalMetalProduced;         // +0xb4
    double totalEnergyConsumed;        // +0xbc
    double totalMetalConsumed;         // +0xc4
    double energyWasted;               // +0xcc
    double metalWasted;                // +0xd4
    float energyStorage;               // +0xdc
    float metalStorage;                // +0xe0
    char unknown_e4[0xf0 - 0xe4];
    int updateTime;                    // +0xf0
    int winLoseTime;                   // +0xf4
    int displayTimer;                  // +0xf8
    short kills;                       // +0xfc
    short losses;                      // +0xfe
    char unknown_100[0x108 - 0x100];
    char allied[11];                   // +0x108
    char unknown_113[0x149 - 0x113];
    unsigned short addStorage : 1;     // +0x149, bit 0
    unsigned short bits_149_1 : 15;
};

struct Game_00466050 {
    char unknown_0[0x1b63];
    Player_00466050 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    char unknown_2a44[0x38a37 - 0x2a44];
    char gameTime[0x1c];               // +0x38a37
};
#pragma pack(pop)

extern Game_00466050* g_game;

class HapiBank {
public:
    int OpenAccount(char* name);
    int SetIntegerItem(const char* name, int value);
    int SetDoubleItem(const char* name, double value);
    int GetIntegerItem(char* name, int def);
    double GetDoubleItem(char* name, double def);
    int OpenNamedBox(char* name);
    int GetBoxSize();
    int ReadBox(void* dst, int len);
    int WriteBox(void* src, int len);
};

// Loads the "Players" section of a saved game: the human player, the game
// clock, and each player's resources, statistics, logo, side and alliances
// from its "Player%i" section (a player without one gets no controller).
// FUNCTION: 0x466050
void __stdcall LoadPlayers(HapiBank* file)
{
    char name[16];
    file->OpenAccount("Players");
    g_game->localPlayer = ((HapiBank*)file)->GetIntegerItem("Human Player", 10);
    g_game->field_2a43 = g_game->localPlayer;
    ((HapiBank*)file)->OpenNamedBox("GameTime");
    if (((HapiBank*)file)->ReadBox(g_game->gameTime, 0x1c) != 0x1c)
        return;
    for (int i = 0; i < 10; i++) {
        Player_00466050* p = &g_game->players[i];
        sprintf(name, "Player%i", i);
        if (file->OpenAccount(name)) {
            p->energy = ((HapiBank*)file)->GetDoubleItem("Energy", 0);
            p->metal = ((HapiBank*)file)->GetDoubleItem("Metal", 0);
            p->totalEnergyProduced = ((HapiBank*)file)->GetDoubleItem("TotalEnergyProduced", 0);
            p->totalMetalProduced = ((HapiBank*)file)->GetDoubleItem("TotalMetalProduced", 0);
            p->totalEnergyConsumed = ((HapiBank*)file)->GetDoubleItem("TotalEnergyConsumed", 0);
            p->totalMetalConsumed = ((HapiBank*)file)->GetDoubleItem("TotalMetalConsumed", 0);
            p->energyWasted = ((HapiBank*)file)->GetDoubleItem("EnergyWasted", 0);
            p->metalWasted = ((HapiBank*)file)->GetDoubleItem("MetalWasted", 0);
            p->energyStorage = ((HapiBank*)file)->GetDoubleItem("PlayerEnergyStorage", 0);
            p->metalStorage = ((HapiBank*)file)->GetDoubleItem("PlayerMetalStorage", 0);
            p->addStorage = ((HapiBank*)file)->GetIntegerItem("AddPlayerStorage", 0);
            p->kills = ((HapiBank*)file)->GetIntegerItem("Kills", 0);
            p->losses = ((HapiBank*)file)->GetIntegerItem("Losses", 0);
            p->updateTime = ((HapiBank*)file)->GetIntegerItem("UpdateTime", 0);
            p->winLoseTime = ((HapiBank*)file)->GetIntegerItem("WinLoseTime", 0);
            p->displayTimer = ((HapiBank*)file)->GetIntegerItem("DisplayTimer", 0);
            p->data->logo = ((HapiBank*)file)->GetIntegerItem("Logo", 0);
            p->data->side = ((HapiBank*)file)->GetIntegerItem("Side", 0);
            ((HapiBank*)file)->OpenNamedBox("Alliances");
            if (((HapiBank*)file)->GetBoxSize() == 11)
                ((HapiBank*)file)->ReadBox(p->allied, 11);
            p->allied[i] = 1;
        } else {
            p->controller = 0;
        }
    }
}

// Saves what LoadPlayers loads, for every player that has a controller.
// FUNCTION: 0x4662f0
void __stdcall SavePlayers(HapiBank* file)
{
    char name[16];
    file->OpenAccount("Players");
    ((HapiBank*)file)->SetIntegerItem("Human Player", g_game->localPlayer);
    ((HapiBank*)file)->OpenNamedBox("GameTime");
    ((HapiBank*)file)->WriteBox(g_game->gameTime, 0x1c);
    for (int i = 0; i < 10; i++) {
        Player_00466050* p = &g_game->players[i];
        if (p->controller) {
            sprintf(name, "Player%i", i);
            file->OpenAccount(name);
            ((HapiBank*)file)->SetDoubleItem("Energy", p->energy);
            ((HapiBank*)file)->SetDoubleItem("Metal", p->metal);
            ((HapiBank*)file)->SetDoubleItem("TotalEnergyProduced", p->totalEnergyProduced);
            ((HapiBank*)file)->SetDoubleItem("TotalMetalProduced", p->totalMetalProduced);
            ((HapiBank*)file)->SetDoubleItem("TotalEnergyConsumed", p->totalEnergyConsumed);
            ((HapiBank*)file)->SetDoubleItem("TotalMetalConsumed", p->totalMetalConsumed);
            ((HapiBank*)file)->SetDoubleItem("EnergyWasted", p->energyWasted);
            ((HapiBank*)file)->SetDoubleItem("MetalWasted", p->metalWasted);
            ((HapiBank*)file)->SetDoubleItem("PlayerEnergyStorage", p->energyStorage);
            ((HapiBank*)file)->SetDoubleItem("PlayerMetalStorage", p->metalStorage);
            ((HapiBank*)file)->SetIntegerItem("AddPlayerStorage", p->addStorage);
            ((HapiBank*)file)->SetIntegerItem("Kills", p->kills);
            ((HapiBank*)file)->SetIntegerItem("Losses", p->losses);
            ((HapiBank*)file)->SetIntegerItem("UpdateTime", p->updateTime);
            ((HapiBank*)file)->SetIntegerItem("WinLoseTime", p->winLoseTime);
            ((HapiBank*)file)->SetIntegerItem("DisplayTimer", p->displayTimer);
            ((HapiBank*)file)->SetIntegerItem("Controller", p->controller);
            ((HapiBank*)file)->SetIntegerItem("Logo", p->data->logo);
            ((HapiBank*)file)->SetIntegerItem("Side", p->data->side);
            ((HapiBank*)file)->OpenNamedBox("Alliances");
            ((HapiBank*)file)->WriteBox(p->allied, 11);
        }
    }
}
