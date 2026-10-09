// Decompiled by Claude Opus 5.5. Names are provisional.
#include <stdio.h>

// Two functions with aligned frames (`and esp, -8`) under the default flags,
// no /Op: the doubles they pass and receive are enough for MSVC to align.

#pragma pack(push, 1)
#include "../network/player_info.h"

// The header's type, kept local: the +0x149 storage flag must stay a 1-bit
// bitfield (a plain word moves LoadPlayers).
struct Player {                        // 0x14b bytes
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo* info;                  // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
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
    float energyStorageBonus;          // +0xdc
    float metalStorageBonus;           // +0xe0
    char unknown_e4[0xf0 - 0xe4];
    int updateTime;                    // +0xf0
    int winLoseTime;                   // +0xf4
    int displayTimer;                  // +0xf8
    short kills;                       // +0xfc
    short losses;                      // +0xfe
    char unknown_100[0x108 - 0x100];
    char allied[11];                   // +0x108
    char unknown_113[0x149 - 0x113];
    unsigned short flags : 1;          // +0x149, bit 0
    unsigned short bits_149_1 : 15;
};

struct Game_00466050 {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x38a37 - 0x2a44];
    char gameTime[0x1c];               // +0x38a37
};
#pragma pack(pop)

extern Game_00466050* g_game;

#include "../util/hapi_bank.h"

// Loads the "Players" section of a saved game: the human player, the game
// clock, and each player's resources, statistics, logo, side and alliances
// from its "Player%i" section (a player without one gets no controller).
// FUNCTION: 0x466050
void __stdcall LoadPlayers(HapiBank* file)
{
    char name[16];
    file->OpenAccount("Players");
    g_game->localPlayer = file->GetIntegerItem("Human Player", 10);
    g_game->playerIndex = g_game->localPlayer;
    file->OpenNamedBox("GameTime");
    if (file->ReadBox(g_game->gameTime, 0x1c) != 0x1c)
        return;
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        sprintf(name, "Player%i", i);
        if (file->OpenAccount(name)) {
            p->energy = file->GetDoubleItem("Energy", 0);
            p->metal = file->GetDoubleItem("Metal", 0);
            p->totalEnergyProduced = file->GetDoubleItem("TotalEnergyProduced", 0);
            p->totalMetalProduced = file->GetDoubleItem("TotalMetalProduced", 0);
            p->totalEnergyConsumed = file->GetDoubleItem("TotalEnergyConsumed", 0);
            p->totalMetalConsumed = file->GetDoubleItem("TotalMetalConsumed", 0);
            p->energyWasted = file->GetDoubleItem("EnergyWasted", 0);
            p->metalWasted = file->GetDoubleItem("MetalWasted", 0);
            p->energyStorageBonus = file->GetDoubleItem("PlayerEnergyStorage", 0);
            p->metalStorageBonus = file->GetDoubleItem("PlayerMetalStorage", 0);
            p->flags = file->GetIntegerItem("AddPlayerStorage", 0);
            p->kills = file->GetIntegerItem("Kills", 0);
            p->losses = file->GetIntegerItem("Losses", 0);
            p->updateTime = file->GetIntegerItem("UpdateTime", 0);
            p->winLoseTime = file->GetIntegerItem("WinLoseTime", 0);
            p->displayTimer = file->GetIntegerItem("DisplayTimer", 0);
            p->info->color = file->GetIntegerItem("Logo", 0);
            p->info->side = file->GetIntegerItem("Side", 0);
            file->OpenNamedBox("Alliances");
            if (file->GetBoxSize() == 11)
                file->ReadBox(p->allied, 11);
            p->allied[i] = 1;
        } else {
            p->type = 0;
        }
    }
}

// Saves what LoadPlayers loads, for every player that has a controller.
// FUNCTION: 0x4662f0
void __stdcall SavePlayers(HapiBank* file)
{
    char name[16];
    file->OpenAccount("Players");
    file->SetIntegerItem("Human Player", g_game->localPlayer);
    file->OpenNamedBox("GameTime");
    file->WriteBox(g_game->gameTime, 0x1c);
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->type) {
            sprintf(name, "Player%i", i);
            file->OpenAccount(name);
            file->SetDoubleItem("Energy", p->energy);
            file->SetDoubleItem("Metal", p->metal);
            file->SetDoubleItem("TotalEnergyProduced", p->totalEnergyProduced);
            file->SetDoubleItem("TotalMetalProduced", p->totalMetalProduced);
            file->SetDoubleItem("TotalEnergyConsumed", p->totalEnergyConsumed);
            file->SetDoubleItem("TotalMetalConsumed", p->totalMetalConsumed);
            file->SetDoubleItem("EnergyWasted", p->energyWasted);
            file->SetDoubleItem("MetalWasted", p->metalWasted);
            file->SetDoubleItem("PlayerEnergyStorage", p->energyStorageBonus);
            file->SetDoubleItem("PlayerMetalStorage", p->metalStorageBonus);
            file->SetIntegerItem("AddPlayerStorage", p->flags);
            file->SetIntegerItem("Kills", p->kills);
            file->SetIntegerItem("Losses", p->losses);
            file->SetIntegerItem("UpdateTime", p->updateTime);
            file->SetIntegerItem("WinLoseTime", p->winLoseTime);
            file->SetIntegerItem("DisplayTimer", p->displayTimer);
            file->SetIntegerItem("Controller", p->type);
            file->SetIntegerItem("Logo", p->info->color);
            file->SetIntegerItem("Side", p->info->side);
            file->OpenNamedBox("Alliances");
            file->WriteBox(p->allied, 11);
        }
    }
}
