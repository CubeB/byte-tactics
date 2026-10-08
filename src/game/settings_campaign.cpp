// Decompiled by Opus, space-bunny-free, DeepSeek V4.1 Flash and Sonnet. Names are provisional.
// The game file a saved game is written to and read from (a HapiBank named
// "Total Annihilation 3.0": a game summary plus each subsystem's state), and
// the campaign name and a few settings saved in the registry.
#include <stdio.h>
#include <string.h>

#include "../util/hapi_bank.h"

class MissionConditions {
public:
    void LoadConditions(HapiBank* file);
    int SaveConditions(void* file);
};

class Mission;

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x95];
    unsigned char side;                // +0x95
};

struct PlayerEntry_004326b0 {
    Unit* unit;                        // +0x00
    char unknown_4[0x14b - 4];
};

struct Options_004326b0 {
    char unknown_0[0x108];
    int commanderDeath;                // +0x108
    int mapping;                       // +0x10c
    int lineOfSight;                   // +0x110
    int lineOfSightType;               // +0x114
    int location;                      // +0x118
};

struct Game {
    char unknown_0[4];
    char* buildDate;                   // +0x04
    char* buildTime;                   // +0x08
    char unknown_c[0x1b8a - 0xc];
    PlayerEntry_004326b0 players[10];  // +0x1b8a
    char unknown_2878[0x29a0 - 0x2878];
    Options_004326b0* options;         // +0x29a0
    char unknown_29a4[0x2a3c - 0x29a4];
    unsigned short numPlayers;         // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x142db - 0x2a43];
    void* finalSurface;                // +0x142db
    char unknown_142df[0x37ebe - 0x142df];
    unsigned short flags_0 : 4;        // +0x37ebe
    unsigned short loaded : 1;         // +0x37ebe, bit 4
    unsigned short flags_5 : 11;
    char unknown_37ec0[0x37eec - 0x37ec0];
    unsigned short maxUnits;           // +0x37eec
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x38a47 - 0x37ef2];
    int ticks;                         // +0x38a47
    char unknown_38a4b[0x38d7f - 0x38a4b];
    unsigned short flags_38d7f;        // +0x38d7f
    int numSkirmishPlayers;            // +0x38d81
    char unknown_38d85[0x391ab - 0x38d85];
    int mission;                       // +0x391ab
    char unknown_391af[0x391cf - 0x391af];
    char thumbs[0x19];                 // +0x391cf
    char unknown_391e8[0x391e9 - 0x391e8];
    Mission* campaign;                 // +0x391e9
    MissionConditions* victoryConditions;  // +0x391ed
    int state;                         // +0x391f1
};
#pragma pack(pop)

#include "../map/mission.h"

extern Game* g_game;
extern char* g_saveBankName;          // "Total Annihilation 3.0"
extern char* g_summaryAccountName;    // "Summary"
extern char g_buildDateFormat[];      // "BUILD DATE: %s"
extern char g_buildTimeFormat[];      // "BUILD TIME: %s"
extern char g_maxUnitsKey[];          // "maxunits"
extern char g_campaignKey[];          // "Campaign"
extern char g_missionKey[];           // "Mission"
extern char g_mapKey[];               // "Map"
extern char g_difficultyKey[];        // "Difficulty"
extern char g_sideKey[];              // "Side"
extern char g_playersKey[];           // "Players"
extern char g_gameTypeKey[];          // "Gametype"
extern char g_thumbsKey[];            // "Thumbs"
extern char g_commanderDeathKey[];    // "CommanderDeath"
extern char g_locationKey[];          // "Location"
extern char g_mappingKey[];           // "Mapping"
extern char g_lineOfSightKey[];       // "LineOfSight"
extern char g_lineOfSightTypeKey[];   // "LineOfSightType"
extern char g_betweenMissionsKey[];   // "BetweenMissions"
extern char g_descriptionKey[];       // "Description"
extern char g_gameIdKey[];            // "Game ID"
extern char g_gameTimeKey[];          // "Game Time"
extern char g_radarImageBoxName[];    // "Radar Image"

void __stdcall LoadPlayers(HapiBank* file);
void __stdcall ReadCameraPosition(HapiBank* file);
void __stdcall LoadFeatures(HapiBank* file);
void __stdcall LoadMetalPlotmap(HapiBank* file);
void __stdcall LoadPlayerFeaturesPlotmap(HapiBank* file);
void __stdcall LoadMappingData(HapiBank* file);
void __stdcall LoadUnits(HapiBank* file);
void __stdcall LoadMeteors(HapiBank* file);

void __stdcall SaveSurface(void* surface, void* file);
void __stdcall WriteCameraPosition(void* file);
void __stdcall SavePlayers(void* file);
void __stdcall SaveUnits(void* file);
void __stdcall SaveMappingData(void* file);
void __stdcall SaveFeatures(void* file);
void __stdcall SavePlayerFeaturesPlotmap(void* file);
void __stdcall SaveMetalPlotmap(void* file);
void __stdcall SaveMeteors(void* file);

extern int __stdcall ReadRegistryData(const char* app, const char* key, char* buf, int* size);
extern void __stdcall WriteRegistryString(const char* app, const char* key, const char* value);
extern int __stdcall WriteRegistryDword(const char* param1, const char* param2, int param3);

// Allocates a 4-byte object, initialises it, and opens it with the given
// name and two global strings; frees it and returns 0 on failure.
// FUNCTION: 0x432520
HapiBank* __stdcall OpenSummaryBank(char* name)
{
    HapiBank* mem = new HapiBank;
    HapiBank* obj;
    if (mem != 0) {
        obj = mem->InitBank();
    } else {
        obj = 0;
    }
    if (obj->OpenBank(name, g_saveBankName, g_summaryAccountName) == 0) {
        if (obj != 0) {
            obj->CloseBank();
            delete obj;
        }
        return 0;
    }
    obj->OpenAccount(g_summaryAccountName);
    return obj;
}

// Deletes an object whose (out-of-line) destructor is CloseBank.
// FUNCTION: 0x432590
void __stdcall FreeSummaryBank(HapiBank* obj)
{
    if (obj) {
        obj->CloseBank();
        operator delete(obj);
    }
}

// Allocates a 4-byte object, initialises it, and opens it with the given
// name and a global string; frees it and returns 0 on failure.
// Sibling of 0x432520.
// FUNCTION: 0x4325b0
HapiBank* __stdcall OpenHapiBank(char* name)
{
    HapiBank* mem = new HapiBank;
    HapiBank* obj;
    if (mem != 0) {
        obj = mem->InitBank();
    } else {
        obj = 0;
    }
    if (obj->OpenBank(name, g_saveBankName, 0) == 0) {
        if (obj != 0) {
            obj->CloseBank();
            delete obj;
        }
        return 0;
    }
    return obj;
}

// Reads the game summary section and every subsystem's saved state.
// FUNCTION: 0x432610
int __stdcall LoadSavedGameState(HapiBank* file)
{
    file->OpenAccount(g_summaryAccountName);
    if (file->HasItem("maxunits"))
        g_game->maxUnits = file->GetIntegerItem("maxunits", 0);
    LoadPlayers(file);
    ReadCameraPosition(file);
    LoadFeatures(file);
    LoadMetalPlotmap(file);
    LoadPlayerFeaturesPlotmap(file);
    LoadMappingData(file);
    LoadUnits(file);
    LoadMeteors(file);
    g_game->victoryConditions->LoadConditions(file);
    g_game->loaded = 1;
    return 1;
}

// Writes the game summary file named by argument 1 (the "Summary" section of
// a freshly built parse tree, plus the radar image and every subsystem's save
// state when the game is in state 6) and hands it to SaveBank, which
// writes the save file and returns its result. Argument 2 is an optional
// description string, argument 3 the game id.
// FUNCTION: 0x4326b0
int __stdcall SaveGameFile(char* param_1, char* param_2, int param_3)
{
    HapiBank file;
    char buf[256];

    (&file)->InitBank();
    file.NewBank();
    (&file)->OpenAccount(g_summaryAccountName);

    sprintf(buf, g_buildDateFormat, g_game->buildDate);
    (&file)->SetIntegerItem(buf, 0);
    sprintf(buf, g_buildTimeFormat, g_game->buildTime);
    (&file)->SetIntegerItem(buf, 0);
    (&file)->SetIntegerItem(g_maxUnitsKey, g_game->maxUnits);
    (&file)->SetStringItem(g_campaignKey, g_game->campaign->GetCampaignName());
    if (g_game->state != 6) {
        g_game->campaign->AdvanceMission();
    }
    (&file)->SetStringItem(g_missionKey, g_game->campaign->GetMissionName());
    (&file)->SetStringItem(g_mapKey, g_game->campaign->GetMissionName());
    (&file)->SetIntegerItem(g_difficultyKey, g_game->difficulty);
    (&file)->SetIntegerItem(g_sideKey, g_game->players[g_game->localPlayer].unit->side);
    (&file)->SetIntegerItem(g_playersKey, g_game->numPlayers);
    (&file)->SetIntegerItem(g_gameTypeKey, g_game->campaign->GetGameType());
    (&file)->SetStringItem(g_thumbsKey, g_game->thumbs);
    if (g_game->campaign->GetGameType() == 2) {
        (&file)->SetIntegerItem(g_commanderDeathKey, g_game->options->commanderDeath);
        (&file)->SetIntegerItem(g_locationKey, g_game->options->location);
        (&file)->SetIntegerItem(g_mappingKey, g_game->options->mapping);
        (&file)->SetIntegerItem(g_lineOfSightKey, g_game->options->lineOfSight);
        (&file)->SetIntegerItem(g_lineOfSightTypeKey, g_game->options->lineOfSightType);
    }
    if (g_game->state != 6) {
        (&file)->SetIntegerItem(g_betweenMissionsKey, 1);
        g_game->campaign->SelectMission(g_game->mission);
    }
    if (param_2 != 0) {
        (&file)->SetStringItem(g_descriptionKey, param_2);
    }
    (&file)->SetIntegerItem(g_gameIdKey, param_3);
    (&file)->SetIntegerItem(g_gameTimeKey, g_game->ticks);
    if (g_game->state == 6) {
        (&file)->OpenNamedBox(g_radarImageBoxName);
        SaveSurface(g_game->finalSurface, &file);
        WriteCameraPosition(&file);
        SavePlayers(&file);
        SaveUnits(&file);
        SaveMappingData(&file);
        SaveFeatures(&file);
        SavePlayerFeaturesPlotmap(&file);
        SaveMetalPlotmap(&file);
        SaveMeteors(&file);
        g_game->victoryConditions->SaveConditions(&file);
    }
    int result = file.SaveBank(param_1, g_saveBankName, 1, 0);
    (&file)->CloseBank();
    return result;
}

// FUNCTION: 0x432a80
void __stdcall LoadCampaignRegistryName(int core, char* name)
{
    char key[64];
    if (core)
        strcpy(key, "CoreCamp");
    else
        strcpy(key, "ArmCamp");
    core = 26; // the original reuses the dead first argument slot for the size
    if (ReadRegistryData("Total Annihilation", key, name, &core) == 0) {
        memset(name, 0x55, 0x19);
        name[0x19] = 0;
    }
}

// FUNCTION: 0x432b00
void __stdcall SaveCampaignRegistryName(int core, char* name)
{
    char key[64];
    if (core)
        strcpy(key, "CoreCamp");
    else
        strcpy(key, "ArmCamp");
    name[0x19] = 0;
    WriteRegistryString("Total Annihilation", key, name);
}

// Saves the "AllMissions" flag (bit 0 of g_game+0x38d7f) to the registry.
// FUNCTION: 0x432b60
void SaveAllMissionsSetting()
{
    WriteRegistryDword("Total Annihilation", "AllMissions", g_game->flags_38d7f & 1);
}

// FUNCTION: 0x432b80
void SaveNumSkirmishPlayers()
{
    WriteRegistryDword("Total Annihilation", "NumSkirmishPlayers", g_game->numSkirmishPlayers);
}
