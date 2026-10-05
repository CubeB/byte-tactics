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
    void FUN_0048fe60(Class_004b4560* file);
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

void __stdcall FUN_00466050(Class_004b4560* file);
void __stdcall FUN_0041d2b0(Class_004b4560* file);
void __stdcall FUN_00424c00(Class_004b4560* file);
void __stdcall FUN_00484d60(Class_004b4560* file);
void __stdcall FUN_00484e80(Class_004b4560* file);
void __stdcall FUN_00484fa0(Class_004b4560* file);
void __stdcall LoadUnits(Class_004b4560* file);
void __stdcall FUN_00438250(Class_004b4560* file);

// Reads the game summary section and every subsystem's saved state.
// FUNCTION: 0x432610
int __stdcall FUN_00432610(Class_004b4560* file)
{
    file->FUN_004b4560(DAT_00503320);
    if (((Class_004b48f0*)file)->FUN_004b48f0("maxunits"))
        g_game->maxUnits = ((Class_004b4800*)file)->FUN_004b4800("maxunits", 0);
    FUN_00466050(file);
    FUN_0041d2b0(file);
    FUN_00424c00(file);
    FUN_00484d60(file);
    FUN_00484e80(file);
    FUN_00484fa0(file);
    LoadUnits(file);
    FUN_00438250(file);
    g_game->unknown_391ed->FUN_0048fe60(file);
    g_game->loaded = 1;
    return 1;
}
