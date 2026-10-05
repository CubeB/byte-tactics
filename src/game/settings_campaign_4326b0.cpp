// Decompiled by space-bunny-free. Names are provisional.
// Writes the game summary file named by argument 1 (the "Summary" section of
// a freshly built parse tree, plus the radar image and every subsystem's save
// state when the game is in state 6) and hands it to SaveBank, which
// writes the ",3P" file and returns its result. Argument 2 is an optional
// description string, argument 3 the game id.
// check.py: MATCH (976 bytes, both).
#include <stdio.h>

class MissionConditions {
public:
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
    char unknown_142df[0x37eec - 0x142df];
    unsigned short maxUnits;           // +0x37eec
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x38a47 - 0x37ef2];
    int ticks;                         // +0x38a47
    char unknown_38a4b[0x391ab - 0x38a4b];
    int mission;                       // +0x391ab
    char unknown_391af[0x391cf - 0x391af];
    char thumbs[0x19];                 // +0x391cf
    char unknown_391e8[0x391e9 - 0x391e8];
    void* campaign;                    // +0x391e9
    MissionConditions* field_391ed;   // +0x391ed
    int state;                         // +0x391f1
};
#pragma pack(pop)

class HapiBank {
public:
    void* field_0;                     // +0x0
    char buf[256];                     // +0x4

    void NewBank();
    int SaveBank(char* name, char* ext, int a, int b);
    HapiBank* InitBank();
    void CloseBank();
    void OpenAccount(const char* section);
    void SetIntegerItem(const char* name, int value);
    void SetStringItem(const char* name, char* value);
    int OpenNamedBox(const char* name);
};

class Mission {
public:
    int FUN_00435100();
    char* FUN_004352b0();
    char* FUN_00435c30();
    void AdvanceMission();
    int FUN_00435c00(int param_1);
};

extern Game* g_game;
extern char* DAT_0050331c;
extern char* DAT_00503320;
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

void __stdcall SaveSurface(void* surface, void* file);
void __stdcall WriteCameraPosition(void* file);
void __stdcall SavePlayers(void* file);
void __stdcall SaveUnits(void* file);
void __stdcall SaveMappingData(void* file);
void __stdcall SaveFeatures(void* file);
void __stdcall SavePlayerFeaturesPlotmap(void* file);
void __stdcall SaveMetalPlotmap(void* file);
void __stdcall SaveMeteors(void* file);

// FUNCTION: 0x4326b0
int __stdcall SaveGameFile(char* param_1, char* param_2, int param_3)
{
    HapiBank file;

    ((HapiBank*)&file)->InitBank();
    file.NewBank();
    ((HapiBank*)&file)->OpenAccount(DAT_00503320);

    sprintf(file.buf, DAT_005049ac, g_game->buildDate);
    ((HapiBank*)&file)->SetIntegerItem(file.buf, 0);
    sprintf(file.buf, DAT_0050499c, g_game->buildTime);
    ((HapiBank*)&file)->SetIntegerItem(file.buf, 0);
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
