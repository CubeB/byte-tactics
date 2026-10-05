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
};

class Class_004b4630 {
public:
    int SetIntegerItem(const char* name, int value);
};

class Class_004b46c0 {
public:
    int SetDoubleItem(const char* name, double value);
};

class Class_004b4800 {
public:
    int GetIntegerItem(char* name, int def);
};

class Class_004b4850 {
public:
    double GetDoubleItem(char* name, double def);
};

class Class_004b4ba0 {
public:
    int OpenNamedBox(char* name);
};

class Class_004b4bf0 {
public:
    int GetBoxSize();
};

class Class_004b4c80 {
public:
    int ReadBox(void* dst, int len);
};

class Class_004b4cf0 {
public:
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
    g_game->localPlayer = ((Class_004b4800*)file)->GetIntegerItem("Human Player", 10);
    g_game->field_2a43 = g_game->localPlayer;
    ((Class_004b4ba0*)file)->OpenNamedBox("GameTime");
    if (((Class_004b4c80*)file)->ReadBox(g_game->gameTime, 0x1c) != 0x1c)
        return;
    for (int i = 0; i < 10; i++) {
        Player_00466050* p = &g_game->players[i];
        sprintf(name, "Player%i", i);
        if (file->OpenAccount(name)) {
            p->energy = ((Class_004b4850*)file)->GetDoubleItem("Energy", 0);
            p->metal = ((Class_004b4850*)file)->GetDoubleItem("Metal", 0);
            p->totalEnergyProduced = ((Class_004b4850*)file)->GetDoubleItem("TotalEnergyProduced", 0);
            p->totalMetalProduced = ((Class_004b4850*)file)->GetDoubleItem("TotalMetalProduced", 0);
            p->totalEnergyConsumed = ((Class_004b4850*)file)->GetDoubleItem("TotalEnergyConsumed", 0);
            p->totalMetalConsumed = ((Class_004b4850*)file)->GetDoubleItem("TotalMetalConsumed", 0);
            p->energyWasted = ((Class_004b4850*)file)->GetDoubleItem("EnergyWasted", 0);
            p->metalWasted = ((Class_004b4850*)file)->GetDoubleItem("MetalWasted", 0);
            p->energyStorage = ((Class_004b4850*)file)->GetDoubleItem("PlayerEnergyStorage", 0);
            p->metalStorage = ((Class_004b4850*)file)->GetDoubleItem("PlayerMetalStorage", 0);
            p->addStorage = ((Class_004b4800*)file)->GetIntegerItem("AddPlayerStorage", 0);
            p->kills = ((Class_004b4800*)file)->GetIntegerItem("Kills", 0);
            p->losses = ((Class_004b4800*)file)->GetIntegerItem("Losses", 0);
            p->updateTime = ((Class_004b4800*)file)->GetIntegerItem("UpdateTime", 0);
            p->winLoseTime = ((Class_004b4800*)file)->GetIntegerItem("WinLoseTime", 0);
            p->displayTimer = ((Class_004b4800*)file)->GetIntegerItem("DisplayTimer", 0);
            p->data->logo = ((Class_004b4800*)file)->GetIntegerItem("Logo", 0);
            p->data->side = ((Class_004b4800*)file)->GetIntegerItem("Side", 0);
            ((Class_004b4ba0*)file)->OpenNamedBox("Alliances");
            if (((Class_004b4bf0*)file)->GetBoxSize() == 11)
                ((Class_004b4c80*)file)->ReadBox(p->allied, 11);
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
    ((Class_004b4630*)file)->SetIntegerItem("Human Player", g_game->localPlayer);
    ((Class_004b4ba0*)file)->OpenNamedBox("GameTime");
    ((Class_004b4cf0*)file)->WriteBox(g_game->gameTime, 0x1c);
    for (int i = 0; i < 10; i++) {
        Player_00466050* p = &g_game->players[i];
        if (p->controller) {
            sprintf(name, "Player%i", i);
            file->OpenAccount(name);
            ((Class_004b46c0*)file)->SetDoubleItem("Energy", p->energy);
            ((Class_004b46c0*)file)->SetDoubleItem("Metal", p->metal);
            ((Class_004b46c0*)file)->SetDoubleItem("TotalEnergyProduced", p->totalEnergyProduced);
            ((Class_004b46c0*)file)->SetDoubleItem("TotalMetalProduced", p->totalMetalProduced);
            ((Class_004b46c0*)file)->SetDoubleItem("TotalEnergyConsumed", p->totalEnergyConsumed);
            ((Class_004b46c0*)file)->SetDoubleItem("TotalMetalConsumed", p->totalMetalConsumed);
            ((Class_004b46c0*)file)->SetDoubleItem("EnergyWasted", p->energyWasted);
            ((Class_004b46c0*)file)->SetDoubleItem("MetalWasted", p->metalWasted);
            ((Class_004b46c0*)file)->SetDoubleItem("PlayerEnergyStorage", p->energyStorage);
            ((Class_004b46c0*)file)->SetDoubleItem("PlayerMetalStorage", p->metalStorage);
            ((Class_004b4630*)file)->SetIntegerItem("AddPlayerStorage", p->addStorage);
            ((Class_004b4630*)file)->SetIntegerItem("Kills", p->kills);
            ((Class_004b4630*)file)->SetIntegerItem("Losses", p->losses);
            ((Class_004b4630*)file)->SetIntegerItem("UpdateTime", p->updateTime);
            ((Class_004b4630*)file)->SetIntegerItem("WinLoseTime", p->winLoseTime);
            ((Class_004b4630*)file)->SetIntegerItem("DisplayTimer", p->displayTimer);
            ((Class_004b4630*)file)->SetIntegerItem("Controller", p->controller);
            ((Class_004b4630*)file)->SetIntegerItem("Logo", p->data->logo);
            ((Class_004b4630*)file)->SetIntegerItem("Side", p->data->side);
            ((Class_004b4ba0*)file)->OpenNamedBox("Alliances");
            ((Class_004b4cf0*)file)->WriteBox(p->allied, 11);
        }
    }
}
