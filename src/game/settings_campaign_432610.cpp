// Decompiled by Opus. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(char* section);
};

class Class_004b48f0 {
public:
    int FUN_004b48f0(char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

class Class_0048fe60 {
public:
    void LoadConditions(Class_004b4560* file);
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
    Class_0048fe60* unknown_391ed;   // +0x391ed
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_00503320;           // "Summary"

void __stdcall LoadPlayers(Class_004b4560* file);
void __stdcall ReadCameraPosition(Class_004b4560* file);
void __stdcall LoadFeatures(Class_004b4560* file);
void __stdcall LoadMetalPlotmap(Class_004b4560* file);
void __stdcall LoadPlayerFeaturesPlotmap(Class_004b4560* file);
void __stdcall LoadMappingData(Class_004b4560* file);
void __stdcall LoadUnits(Class_004b4560* file);
void __stdcall LoadMeteors(Class_004b4560* file);

// Reads the game summary section and every subsystem's saved state.
// FUNCTION: 0x432610
int __stdcall LoadSavedGameState(Class_004b4560* file)
{
    file->FUN_004b4560(DAT_00503320);
    if (((Class_004b48f0*)file)->FUN_004b48f0("maxunits"))
        g_game->maxUnits = ((Class_004b4800*)file)->FUN_004b4800("maxunits", 0);
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
