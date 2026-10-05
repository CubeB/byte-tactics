// Decompiled by space-bunny-free. Names are provisional.
// Writes the game summary file named by argument 1 (the "Summary" section of
// a freshly built parse tree, plus the radar image and every subsystem's save
// state when the game is in state 6) and hands it to FUN_004b39c0, which
// writes the ",3P" file and returns its result. Argument 2 is an optional
// description string, argument 3 the game id.
// check.py: MATCH (976 bytes, both).
#include <stdio.h>

class Class_0048ff40 {
public:
    int FUN_0048fdf0(void* file);
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

struct Game_004326b0 {
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
    Class_0048ff40* field_391ed;      // +0x391ed
    int state;                         // +0x391f1
};
#pragma pack(pop)

class Class_004b3620 {
public:
    int field_0;

    Class_004b3620* FUN_004b3620();
};

class Class_004b3630 {
public:
    void FUN_004b3630();
};

class Class_004b3750 {
public:
    void* field_0;                     // +0x0
    char buf[256];                     // +0x4

    void FUN_004b3750();
    int FUN_004b39c0(char* name, char* ext, int a, int b);
};

class Class_004b4560 {
public:
    void FUN_004b4560(const char* section);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

class Class_004b4750 {
public:
    void FUN_004b4750(const char* name, char* value);
};

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(const char* name);
};

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_004352b0 {
public:
    char* FUN_004352b0();
};

class Class_00435c30 {
public:
    char* FUN_00435c30();
};

class Class_00435c00 {
public:
    void FUN_00435c60();
    int FUN_00435c00(int param_1);
};

extern Game_004326b0* g_game;
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

void __stdcall FUN_004c6f10(void* surface, void* file);
void __stdcall FUN_0041d360(void* file);
void __stdcall FUN_004662f0(void* file);
void __stdcall FUN_004876c0(void* file);
void __stdcall FUN_00484f50(void* file);
void __stdcall FUN_00424890(void* file);
void __stdcall FUN_00484df0(void* file);
void __stdcall FUN_00484ce0(void* file);
void __stdcall FUN_00438180(void* file);

// FUNCTION: 0x4326b0
int __stdcall FUN_004326b0(char* param_1, char* param_2, int param_3)
{
    Class_004b3750 file;

    ((Class_004b3620*)&file)->FUN_004b3620();
    file.FUN_004b3750();
    ((Class_004b4560*)&file)->FUN_004b4560(DAT_00503320);

    sprintf(file.buf, DAT_005049ac, g_game->buildDate);
    ((Class_004b4630*)&file)->FUN_004b4630(file.buf, 0);
    sprintf(file.buf, DAT_0050499c, g_game->buildTime);
    ((Class_004b4630*)&file)->FUN_004b4630(file.buf, 0);
    ((Class_004b4630*)&file)->FUN_004b4630(DAT_005048f8, g_game->maxUnits);
    ((Class_004b4750*)&file)->FUN_004b4750(DAT_005028f8, ((Class_004352b0*)g_game->campaign)->FUN_004352b0());
    if (g_game->state != 6) {
        ((Class_00435c00*)g_game->campaign)->FUN_00435c60();
    }
    ((Class_004b4750*)&file)->FUN_004b4750(DAT_00504994, ((Class_00435c30*)g_game->campaign)->FUN_00435c30());
    ((Class_004b4750*)&file)->FUN_004b4750(DAT_00504990, ((Class_00435c30*)g_game->campaign)->FUN_00435c30());
    ((Class_004b4630*)&file)->FUN_004b4630(DAT_00502a78, g_game->difficulty);
    ((Class_004b4630*)&file)->FUN_004b4630(DAT_00504988, g_game->players[g_game->localPlayer].unit->side);
    ((Class_004b4630*)&file)->FUN_004b4630(DAT_00504980, g_game->numPlayers);
    ((Class_004b4630*)&file)->FUN_004b4630(DAT_00504974, ((Class_00435100*)g_game->campaign)->FUN_00435100());
    ((Class_004b4750*)&file)->FUN_004b4750(DAT_0050496c, g_game->thumbs);
    if (((Class_00435100*)g_game->campaign)->FUN_00435100() == 2) {
        ((Class_004b4630*)&file)->FUN_004b4630(DAT_0050495c, g_game->options->commanderDeath);
        ((Class_004b4630*)&file)->FUN_004b4630(DAT_00504950, g_game->options->location);
        ((Class_004b4630*)&file)->FUN_004b4630(DAT_00502288, g_game->options->mapping);
        ((Class_004b4630*)&file)->FUN_004b4630(DAT_00504944, g_game->options->lineOfSight);
        ((Class_004b4630*)&file)->FUN_004b4630(DAT_00504934, g_game->options->lineOfSightType);
    }
    if (g_game->state != 6) {
        ((Class_004b4630*)&file)->FUN_004b4630(DAT_00504924, 1);
        ((Class_00435c00*)g_game->campaign)->FUN_00435c00(g_game->mission);
    }
    if (param_2 != 0) {
        ((Class_004b4750*)&file)->FUN_004b4750(DAT_00502e78, param_2);
    }
    ((Class_004b4630*)&file)->FUN_004b4630(DAT_0050491c, param_3);
    ((Class_004b4630*)&file)->FUN_004b4630(DAT_00504910, g_game->ticks);
    if (g_game->state == 6) {
        ((Class_004b4ba0*)&file)->FUN_004b4ba0(DAT_00504904);
        FUN_004c6f10(g_game->finalSurface, &file);
        FUN_0041d360(&file);
        FUN_004662f0(&file);
        FUN_004876c0(&file);
        FUN_00484f50(&file);
        FUN_00424890(&file);
        FUN_00484df0(&file);
        FUN_00484ce0(&file);
        FUN_00438180(&file);
        g_game->field_391ed->FUN_0048fdf0(&file);
    }
    int result = file.FUN_004b39c0(param_1, DAT_0050331c, 1, 0);
    ((Class_004b3630*)&file)->FUN_004b3630();
    return result;
}
