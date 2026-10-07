// Decompiled by Opus, space-bunny-free, DeepSeek V4.1 Flash and Sonnet. Names are provisional.
// The game file a saved game is written to and read from (the HapiBank "3P"
// file: a game summary plus each subsystem's state), and the campaign name
// and a few settings saved in the registry.
#include <stdio.h>
#include <string.h>

class HapiBank {
public:
    int field_0;                       // +0x0

    HapiBank* InitBank();
    void CloseBank();
    int OpenBank(char* name, char* a, char* b);
    void OpenAccount(char* section);
    int HasItem(char* name);
    int GetIntegerItem(char* name, int def);
    void NewBank();
    int SaveBank(char* name, char* ext, int a, int b);
    void SetIntegerItem(const char* name, int value);
    void SetStringItem(const char* name, char* value);
    int OpenNamedBox(const char* name);
};

class MissionConditions {
public:
    void LoadConditions(HapiBank* file);
    int SaveConditions(void* file);
};

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
    int field_38d81;                   // +0x38d81
    char unknown_38d85[0x391ab - 0x38d85];
    int mission;                       // +0x391ab
    char unknown_391af[0x391cf - 0x391af];
    char thumbs[0x19];                 // +0x391cf
    char unknown_391e8[0x391e9 - 0x391e8];
    void* campaign;                    // +0x391e9
    MissionConditions* field_391ed;    // +0x391ed
    int state;                         // +0x391f1
};
#pragma pack(pop)

class Mission {
public:
    int FUN_00435100();
    char* FUN_004352b0();
    char* FUN_00435c30();
    void AdvanceMission();
    int FUN_00435c00(int param_1);
};

extern Game* g_game;
extern char* DAT_0050331c;            // ",3P"
extern char* DAT_00503320;            // "Summary"
extern char DAT_005049ac[];           // "BUILD DATE: %s"
extern char DAT_0050499c[];           // "BUILD TIME: %s"
extern char DAT_005048f8[];           // "maxunits"
extern char DAT_005028f8[];           // "Campaign"
extern char DAT_00504994[];           // "Mission"
extern char DAT_00504990[];           // "Map"
extern char DAT_00502a78[];           // "Difficulty"
extern char DAT_00504988[];           // "Side"
extern char DAT_00504980[];           // "Players"
extern char DAT_00504974[];           // "Gametype"
extern char DAT_0050496c[];           // "Thumbs"
extern char DAT_0050495c[];           // "CommanderDeath"
extern char DAT_00504950[];           // "Location"
extern char DAT_00502288[];           // "Mapping"
extern char DAT_00504944[];           // "LineOfSight"
extern char DAT_00504934[];           // "LineOfSightType"
extern char DAT_00504924[];           // "BetweenMissions"
extern char DAT_00502e78[];           // "Description"
extern char DAT_0050491c[];           // "Game ID"
extern char DAT_00504910[];           // "Game Time"
extern char DAT_00504904[];           // "Radar Image"

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
HapiBank* __stdcall FUN_00432520(char* name)
{
    HapiBank* mem = new HapiBank;
    HapiBank* obj;
    if (mem != 0) {
        obj = mem->InitBank();
    } else {
        obj = 0;
    }
    if (((HapiBank*)obj)->OpenBank(name, DAT_0050331c, DAT_00503320) == 0) {
        if (obj != 0) {
            ((HapiBank*)obj)->CloseBank();
            delete obj;
        }
        return 0;
    }
    ((HapiBank*)obj)->OpenAccount(DAT_00503320);
    return obj;
}

// Deletes an object whose (out-of-line) destructor is CloseBank.
// FUNCTION: 0x432590
void __stdcall FUN_00432590(HapiBank* obj)
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
HapiBank* __stdcall FUN_004325b0(char* name)
{
    HapiBank* mem = new HapiBank;
    HapiBank* obj;
    if (mem != 0) {
        obj = mem->InitBank();
    } else {
        obj = 0;
    }
    if (((HapiBank*)obj)->OpenBank(name, DAT_0050331c, 0) == 0) {
        if (obj != 0) {
            ((HapiBank*)obj)->CloseBank();
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
    file->OpenAccount(DAT_00503320);
    if (((HapiBank*)file)->HasItem("maxunits"))
        g_game->maxUnits = ((HapiBank*)file)->GetIntegerItem("maxunits", 0);
    LoadPlayers(file);
    ReadCameraPosition(file);
    LoadFeatures(file);
    LoadMetalPlotmap(file);
    LoadPlayerFeaturesPlotmap(file);
    LoadMappingData(file);
    LoadUnits(file);
    LoadMeteors(file);
    g_game->field_391ed->LoadConditions(file);
    g_game->loaded = 1;
    return 1;
}

// Writes the game summary file named by argument 1 (the "Summary" section of
// a freshly built parse tree, plus the radar image and every subsystem's save
// state when the game is in state 6) and hands it to SaveBank, which
// writes the ",3P" file and returns its result. Argument 2 is an optional
// description string, argument 3 the game id.
// FUNCTION: 0x4326b0
int __stdcall SaveGameFile(char* param_1, char* param_2, int param_3)
{
    HapiBank file;
    char buf[256];

    ((HapiBank*)&file)->InitBank();
    file.NewBank();
    ((HapiBank*)&file)->OpenAccount(DAT_00503320);

    sprintf(buf, DAT_005049ac, g_game->buildDate);
    ((HapiBank*)&file)->SetIntegerItem(buf, 0);
    sprintf(buf, DAT_0050499c, g_game->buildTime);
    ((HapiBank*)&file)->SetIntegerItem(buf, 0);
    ((HapiBank*)&file)->SetIntegerItem(DAT_005048f8, g_game->maxUnits);
    ((HapiBank*)&file)->SetStringItem(DAT_005028f8, ((Mission*)g_game->campaign)->FUN_004352b0());
    if (g_game->state != 6) {
        ((Mission*)g_game->campaign)->AdvanceMission();
    }
    ((HapiBank*)&file)->SetStringItem(DAT_00504994, ((Mission*)g_game->campaign)->FUN_00435c30());
    ((HapiBank*)&file)->SetStringItem(DAT_00504990, ((Mission*)g_game->campaign)->FUN_00435c30());
    ((HapiBank*)&file)->SetIntegerItem(DAT_00502a78, g_game->difficulty);
    ((HapiBank*)&file)->SetIntegerItem(DAT_00504988, g_game->players[g_game->localPlayer].unit->side);
    ((HapiBank*)&file)->SetIntegerItem(DAT_00504980, g_game->numPlayers);
    ((HapiBank*)&file)->SetIntegerItem(DAT_00504974, ((Mission*)g_game->campaign)->FUN_00435100());
    ((HapiBank*)&file)->SetStringItem(DAT_0050496c, g_game->thumbs);
    if (((Mission*)g_game->campaign)->FUN_00435100() == 2) {
        ((HapiBank*)&file)->SetIntegerItem(DAT_0050495c, g_game->options->commanderDeath);
        ((HapiBank*)&file)->SetIntegerItem(DAT_00504950, g_game->options->location);
        ((HapiBank*)&file)->SetIntegerItem(DAT_00502288, g_game->options->mapping);
        ((HapiBank*)&file)->SetIntegerItem(DAT_00504944, g_game->options->lineOfSight);
        ((HapiBank*)&file)->SetIntegerItem(DAT_00504934, g_game->options->lineOfSightType);
    }
    if (g_game->state != 6) {
        ((HapiBank*)&file)->SetIntegerItem(DAT_00504924, 1);
        ((Mission*)g_game->campaign)->FUN_00435c00(g_game->mission);
    }
    if (param_2 != 0) {
        ((HapiBank*)&file)->SetStringItem(DAT_00502e78, param_2);
    }
    ((HapiBank*)&file)->SetIntegerItem(DAT_0050491c, param_3);
    ((HapiBank*)&file)->SetIntegerItem(DAT_00504910, g_game->ticks);
    if (g_game->state == 6) {
        ((HapiBank*)&file)->OpenNamedBox(DAT_00504904);
        SaveSurface(g_game->finalSurface, &file);
        WriteCameraPosition(&file);
        SavePlayers(&file);
        SaveUnits(&file);
        SaveMappingData(&file);
        SaveFeatures(&file);
        SavePlayerFeaturesPlotmap(&file);
        SaveMetalPlotmap(&file);
        SaveMeteors(&file);
        g_game->field_391ed->SaveConditions(&file);
    }
    int result = file.SaveBank(param_1, DAT_0050331c, 1, 0);
    ((HapiBank*)&file)->CloseBank();
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
    WriteRegistryDword("Total Annihilation", "NumSkirmishPlayers", g_game->field_38d81);
}
