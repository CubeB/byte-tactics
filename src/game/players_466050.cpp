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

class Class_004b4560 {
public:
    int FUN_004b4560(char* name);
};

class Class_004b4630 {
public:
    int FUN_004b4630(const char* name, int value);
};

class Class_004b46c0 {
public:
    int FUN_004b46c0(const char* name, double value);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

class Class_004b4850 {
public:
    double FUN_004b4850(char* name, double def);
};

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4bf0 {
public:
    int FUN_004b4bf0();
};

class Class_004b4c80 {
public:
    int FUN_004b4c80(void* dst, int len);
};

class Class_004b4cf0 {
public:
    int FUN_004b4cf0(void* src, int len);
};

// Loads the "Players" section of a saved game: the human player, the game
// clock, and each player's resources, statistics, logo, side and alliances
// from its "Player%i" section (a player without one gets no controller).
// FUNCTION: 0x466050
void __stdcall LoadPlayers(Class_004b4560* file)
{
    char name[16];
    file->FUN_004b4560("Players");
    g_game->localPlayer = ((Class_004b4800*)file)->FUN_004b4800("Human Player", 10);
    g_game->field_2a43 = g_game->localPlayer;
    ((Class_004b4ba0*)file)->FUN_004b4ba0("GameTime");
    if (((Class_004b4c80*)file)->FUN_004b4c80(g_game->gameTime, 0x1c) != 0x1c)
        return;
    for (int i = 0; i < 10; i++) {
        Player_00466050* p = &g_game->players[i];
        sprintf(name, "Player%i", i);
        if (file->FUN_004b4560(name)) {
            p->energy = ((Class_004b4850*)file)->FUN_004b4850("Energy", 0);
            p->metal = ((Class_004b4850*)file)->FUN_004b4850("Metal", 0);
            p->totalEnergyProduced = ((Class_004b4850*)file)->FUN_004b4850("TotalEnergyProduced", 0);
            p->totalMetalProduced = ((Class_004b4850*)file)->FUN_004b4850("TotalMetalProduced", 0);
            p->totalEnergyConsumed = ((Class_004b4850*)file)->FUN_004b4850("TotalEnergyConsumed", 0);
            p->totalMetalConsumed = ((Class_004b4850*)file)->FUN_004b4850("TotalMetalConsumed", 0);
            p->energyWasted = ((Class_004b4850*)file)->FUN_004b4850("EnergyWasted", 0);
            p->metalWasted = ((Class_004b4850*)file)->FUN_004b4850("MetalWasted", 0);
            p->energyStorage = ((Class_004b4850*)file)->FUN_004b4850("PlayerEnergyStorage", 0);
            p->metalStorage = ((Class_004b4850*)file)->FUN_004b4850("PlayerMetalStorage", 0);
            p->addStorage = ((Class_004b4800*)file)->FUN_004b4800("AddPlayerStorage", 0);
            p->kills = ((Class_004b4800*)file)->FUN_004b4800("Kills", 0);
            p->losses = ((Class_004b4800*)file)->FUN_004b4800("Losses", 0);
            p->updateTime = ((Class_004b4800*)file)->FUN_004b4800("UpdateTime", 0);
            p->winLoseTime = ((Class_004b4800*)file)->FUN_004b4800("WinLoseTime", 0);
            p->displayTimer = ((Class_004b4800*)file)->FUN_004b4800("DisplayTimer", 0);
            p->data->logo = ((Class_004b4800*)file)->FUN_004b4800("Logo", 0);
            p->data->side = ((Class_004b4800*)file)->FUN_004b4800("Side", 0);
            ((Class_004b4ba0*)file)->FUN_004b4ba0("Alliances");
            if (((Class_004b4bf0*)file)->FUN_004b4bf0() == 11)
                ((Class_004b4c80*)file)->FUN_004b4c80(p->allied, 11);
            p->allied[i] = 1;
        } else {
            p->controller = 0;
        }
    }
}

// Saves what LoadPlayers loads, for every player that has a controller.
// FUNCTION: 0x4662f0
void __stdcall SavePlayers(Class_004b4560* file)
{
    char name[16];
    file->FUN_004b4560("Players");
    ((Class_004b4630*)file)->FUN_004b4630("Human Player", g_game->localPlayer);
    ((Class_004b4ba0*)file)->FUN_004b4ba0("GameTime");
    ((Class_004b4cf0*)file)->FUN_004b4cf0(g_game->gameTime, 0x1c);
    for (int i = 0; i < 10; i++) {
        Player_00466050* p = &g_game->players[i];
        if (p->controller) {
            sprintf(name, "Player%i", i);
            file->FUN_004b4560(name);
            ((Class_004b46c0*)file)->FUN_004b46c0("Energy", p->energy);
            ((Class_004b46c0*)file)->FUN_004b46c0("Metal", p->metal);
            ((Class_004b46c0*)file)->FUN_004b46c0("TotalEnergyProduced", p->totalEnergyProduced);
            ((Class_004b46c0*)file)->FUN_004b46c0("TotalMetalProduced", p->totalMetalProduced);
            ((Class_004b46c0*)file)->FUN_004b46c0("TotalEnergyConsumed", p->totalEnergyConsumed);
            ((Class_004b46c0*)file)->FUN_004b46c0("TotalMetalConsumed", p->totalMetalConsumed);
            ((Class_004b46c0*)file)->FUN_004b46c0("EnergyWasted", p->energyWasted);
            ((Class_004b46c0*)file)->FUN_004b46c0("MetalWasted", p->metalWasted);
            ((Class_004b46c0*)file)->FUN_004b46c0("PlayerEnergyStorage", p->energyStorage);
            ((Class_004b46c0*)file)->FUN_004b46c0("PlayerMetalStorage", p->metalStorage);
            ((Class_004b4630*)file)->FUN_004b4630("AddPlayerStorage", p->addStorage);
            ((Class_004b4630*)file)->FUN_004b4630("Kills", p->kills);
            ((Class_004b4630*)file)->FUN_004b4630("Losses", p->losses);
            ((Class_004b4630*)file)->FUN_004b4630("UpdateTime", p->updateTime);
            ((Class_004b4630*)file)->FUN_004b4630("WinLoseTime", p->winLoseTime);
            ((Class_004b4630*)file)->FUN_004b4630("DisplayTimer", p->displayTimer);
            ((Class_004b4630*)file)->FUN_004b4630("Controller", p->controller);
            ((Class_004b4630*)file)->FUN_004b4630("Logo", p->data->logo);
            ((Class_004b4630*)file)->FUN_004b4630("Side", p->data->side);
            ((Class_004b4ba0*)file)->FUN_004b4ba0("Alliances");
            ((Class_004b4cf0*)file)->FUN_004b4cf0(p->allied, 11);
        }
    }
}
