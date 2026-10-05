// Decompiled by Opus. Names are provisional.

class HapiBank {
public:
    void OpenAccount(char* section);
    int HasItem(char* name);
    int GetIntegerItem(char* name, int def);
};

class MissionConditions {
public:
    void LoadConditions(HapiBank* file);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37ebe];
    unsigned short flags_0 : 4;      // +0x37ebe
    unsigned short loaded : 1;       // +0x37ebe, bit 4
    unsigned short flags_5 : 11;
    char unknown_37ec0[0x37eec - 0x37ec0];
    short maxUnits;                  // +0x37eec
    char unknown_37eee[0x391ed - 0x37eee];
    MissionConditions* unknown_391ed;  // +0x391ed
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_00503320;           // "Summary"

void __stdcall LoadPlayers(HapiBank* file);
void __stdcall ReadCameraPosition(HapiBank* file);
void __stdcall LoadFeatures(HapiBank* file);
void __stdcall LoadMetalPlotmap(HapiBank* file);
void __stdcall LoadPlayerFeaturesPlotmap(HapiBank* file);
void __stdcall LoadMappingData(HapiBank* file);
void __stdcall LoadUnits(HapiBank* file);
void __stdcall LoadMeteors(HapiBank* file);

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
    g_game->unknown_391ed->LoadConditions(file);
    g_game->loaded = 1;
    return 1;
}
