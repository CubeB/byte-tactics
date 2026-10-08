// Decompiled by Opus, Haiku, Sonnet, DeepSeek V4.1 Flash, deepseek-v4.1-flash, GPT-6 Astra, Claude Opus 5.5, GPT-5.6 Astra, GPT-6.1-sol and deepseek-v4.1. Names are provisional.
// The console commands: what the player can type after Enter, each with the
// function that runs it (0x416240 to 0x418ca0), then the three command tables
// at 0x501d38, 0x501f48 and 0x501fd0. 0x419560 hands the tables to 0x4b7760,
// which files every name in a sorted table with its handler and its mask; a
// cheat (mask 2) works only once cheats are on, a debug command (mask 4) only
// in a debug build. Each table ends with a record whose name is null.
//
// The lean <windows.h> and the two symbol-only headers below are what the
// symbol-id windows of the drawing functions (0x417bb0 to 0x418310) need
// (docs/c2-regalloc.md); the full <windows.h> moves them out.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
// Only for its symbol ids: it puts 0x4181d0 in the window it matches in.
#include <wctype.h>

#pragma pack(push, 1)

struct Point {
    short x;
    short y;
};

struct Vec3 {
    int x;
    int y;
    int z;
};

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct PlayerData {
    char unknown_0[0x95];
    unsigned char field_95;            // +0x95
    unsigned char field_96;            // +0x96
};

struct Player {
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerData* data;                  // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x7c - 0x74];
    unsigned char* fog;                // +0x7c
    int fogWidth;                      // +0x80
    char unknown_84[0x8c - 0x84];
    float field_8c;                    // +0x8c
    char unknown_90[0x98 - 0x90];
    float field_98;                    // +0x98
    char unknown_9c[0x146 - 0x9c];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];

    void SetType(int type);
};

struct Movement {
    char unknown_0[0x10];
    int width;                         // +0x10
    char unknown_14[4];
    unsigned int* states;              // +0x18, 2 bits per cell, 16 rows per word
};

struct UnitDef {
    char unknown_0[0x20];
    char name[0x15e - 0x20];
    Vec3 min, max;
    char unknown_176[0x1b6 - 0x176];
    Movement* movement;                // +0x1b6
    char unknown_1ba[0x249 - 0x1ba];
};

struct Unit {
    char unknown_0[0x92];
    UnitDef* def;                      // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Tile {
    unsigned short unit;               // +0x0
    unsigned short feature;            // +0x2
    unsigned char height;              // +0x4
    char unknown_5[2];
    unsigned char metal;               // +0x7
    unsigned short object;             // +0x8
    char unknown_a[2];
    unsigned char flags;               // +0xc
};

struct PathCell {
    unsigned char flags;
    signed char direction;
    char unknown_2[2];
};

struct PathGrid {
    PathCell* cells;                   // +0x0
    int width;                         // +0x4
    // Inlined cell lookup on the path grid: puts the whole frame in place.
    PathCell* At(int x, int y) { return &cells[width * y + x]; }
};

struct PathMap {
    char unknown_0[0x1c];
    PathGrid grid;                     // +0x1c
    char unknown_24[0x48 - 0x24];
    int value;                         // +0x48
    char unknown_4c[0x54 - 0x4c];
    int fixed;                         // +0x54
};

class CMemoryCache {
public:
    void FlushCache();
};

class MissionConditions {
public:
    void Deactivate();
};

// The option flags word at +0x37f06.
struct GameFlags {
    unsigned short damagebars : 1;     // bit 0
    unsigned short antiAlias : 1;      // bit 1
    unsigned short shadows : 1;        // bit 2
    unsigned short vehicleShadows : 1; // bit 3
    unsigned short featureShadows : 1; // bit 4
    unsigned short shading : 1;        // bit 5
    unsigned short ditheredFog : 1;    // bit 6
    unsigned short unused7 : 1;        // bit 7
    unsigned short switchAlt : 1;      // bit 8
    unsigned short rest : 7;
};

// The view flags word at +0x14281.
struct ViewFlags {
    unsigned short bit0 : 1;
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short rest : 13;
};

struct Game {
    char unknown_0[0x10];
    void* sound;                       // +0x10
    char unknown_14[0xdcb - 0x14];
    unsigned char colors[16];          // +0xdcb
    char unknown_ddb[0x1b63 - 0xddb];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x2c8e - 0x2a44];
    Point point;                       // +0x2c8e
    char unknown_2c92[0x2caa - 0x2c92];
    Vec3 pos;                          // +0x2caa
    char unknown_2cb6[0x2cbc - 0x2cb6];
    unsigned short cellFeature;        // +0x2cbc
    char unknown_2cbe[0x14207 - 0x2cbe];
    PathMap* paths;                    // +0x14207
    char unknown_1420b[0x14223 - 0x1420b];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    int mapWidth;                      // +0x1422b
    int mapHeight;                     // +0x1422f
    int width;                         // +0x14233
    int height;                        // +0x14237
    int screenTilesX;                  // +0x1423b
    int screenTilesY;                  // +0x1423f
    char unknown_14243[0x1427f - 0x14243];
    unsigned char seaLevel;            // +0x1427f
    unsigned char mode;                // +0x14280
    ViewFlags viewFlags;               // +0x14281
    char unknown_14283[0x14287 - 0x14283];
    Tile* tiles;                       // +0x14287
    char unknown_1428b[0x1431f - 0x1428b];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x1434d - 0x14327];
    unsigned char scrollSpeed;         // +0x1434d
    char unknown_1434e[0x14357 - 0x1434e];
    Unit* units;                       // +0x14357
    Unit* units_end;                   // +0x1435b
    char unknown_1435f[0x14371 - 0x1435f];
    unsigned short autoFollowTicks;    // +0x14371
    unsigned int bit0 : 1;             // +0x14373
    unsigned int paused : 1;
    unsigned int rest_14373 : 30;
    char unknown_14377[0x1437b - 0x14377];
    CMemoryCache* obj;                 // +0x1437b
    char unknown_1437f[0x1438f - 0x1437f];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitDef* defs;                     // +0x1439b
    char unknown_1439f[0x148db - 0x1439f];
    void* logos32;                     // +0x148db
    char unknown_148df[0x37e23 - 0x148df];
    int bottom;                        // +0x37e23
    char unknown_37e27[0x37efa - 0x37e27];
    int field_37efa;                   // +0x37efa
    char unknown_37efe[0x37f02 - 0x37efe];
    int field_37f02;                   // +0x37f02
    GameFlags flags;                   // +0x37f06
    int brightness;                    // +0x37f08
    char unknown_37f0c[0x37f2f - 0x37f0c];
    union {
        unsigned short flags_37f2f;    // +0x37f2f
        struct {
            unsigned short b0 : 1;
            unsigned short b1 : 1;
            unsigned short b2 : 1;
            unsigned short b3 : 1;
            unsigned short b4 : 1;
            unsigned short b5 : 1;
            unsigned short b6 : 1;
            unsigned short b7 : 1;
            unsigned short b8 : 1;
            unsigned short b9 : 1;
            unsigned short flag10 : 1;
            unsigned short rest_37f2f : 5;
        };
    };
    char unknown_37f31[0x37f35 - 0x37f31];
    int field_37f35;                   // +0x37f35
    char unknown_37f39[0x38a37 - 0x37f39];
    int lastShotTime;                  // +0x38a37
    char unknown_38a3b[0x38a47 - 0x38a3b];
    int field_38a47;                   // +0x38a47
    char unknown_38a4b[0x38a53 - 0x38a4b];
    // The install path the Film command replaces and the film speed value
    // overlap.
    union {
        char path[0x20c];              // +0x38a53
        struct {
            char unknown_38a53[0x38c57 - 0x38a53];
            int field_38c57;           // +0x38c57
        };
    };
    int field_38c5f;                   // +0x38c5f
    int field_38c63;                   // +0x38c63
    char unknown_38c67[0x38dd5 - 0x38c67];
    int field_38dd5;                   // +0x38dd5
    char unknown_38dd9[0x391ed - 0x38dd9];
    MissionConditions* field_391ed;    // +0x391ed
    char unknown_391f1[0x391fd - 0x391f1];
    int font;                          // +0x391fd
    char unknown_39201[0x3923b - 0x39201];
    union {
        unsigned short flags_3923b;    // +0x3923b
        struct {
            unsigned short bits_3923b : 2;
            unsigned short flag2 : 1;
            unsigned short bit3 : 1;
            unsigned short flag4 : 1;
            unsigned short flag5 : 1;
            unsigned short flag6 : 1;
            unsigned short rest_3923b : 9;
        };
    };
};

#pragma pack(pop)

// Command arguments.
class CommandArgs {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0

    int GetIntArg(int index, int fallback);
    float GetFloatArg(int index, float def);
    char* GetArg(int index, char* fallback);
    int Tokenize(char* param_1, char* param_2);
    CommandArgs* InitArgs();
};

class Sound {
public:
    void PlayCdTrack(int index, int flag);
    int Is3DEnabled();
    void Disable3D();
    void Enable3D();
    void SetTrackCategory(int);
};

class Class_004ced40 {
public:
    void StopCdAudio();
};

struct Display_00417a60 {
    char unknown_0[0xf0];
    unsigned short bit0 : 1;           // +0xf0
    unsigned short windowed : 1;
    unsigned short bit2_15 : 14;
};

struct Pos_00417bb0 {
    unsigned short x_frac;             // +0x0
    short x;                           // +0x2
    unsigned short y_frac;             // +0x4
    short y;                           // +0x6
    unsigned short z_frac;             // +0x8
    short z;                           // +0xa
};

struct Point16 {
    short x;                           // +0x0
    short z;                           // +0x2
};

class Class_0044f010 {
public:
    char unknown_4[4];
    char* field_8;                     // +0x8
    Point16 points[20];                // +0xc
    int count;                         // +0x5c
    char unknown_60[4];
    unsigned char field_64;            // +0x64
    virtual void DrawOnSurface(void* surface);  // slot 10
};

class Iface_00417f30 {
public:
    virtual void Method_00();
    virtual void Method_04();
    virtual void Method_08();
    virtual void Method_0c();
    virtual void Method_10();
    virtual void Method_14();
    virtual void Method_18();
    virtual void Method_1c();
    virtual void Method_20();
    virtual void Method_24();
    virtual void Method_28(int param_1);
};

struct Point_00417f60 {
    int x;
    int y;
};

struct Rect {
    int left, top, right, bottom;
};

// The command records the three tables hold.
struct ConsoleCommand {
    const char* name;                             // +0x0
    void (__stdcall* run)(CommandArgs* args);     // +0x4
    int mask;                                     // +0x8
};

struct Pair_00419560 {
    int a;
    int b;
};

void __stdcall CmdMemDump(CommandArgs* args);
void __stdcall CmdMem(CommandArgs* args);
void __stdcall CmdAI(CommandArgs* args);
void __stdcall CmdAssign(CommandArgs* args);
void __stdcall CmdAssert(CommandArgs* args);
void __stdcall CmdDPrint(CommandArgs* args);
void __stdcall CmdBurnAll(CommandArgs* args);
void __stdcall CmdBurnOne(CommandArgs* args);
void __stdcall CmdFeature(CommandArgs* args);
void __stdcall CmdShading(CommandArgs* args);
void __stdcall CmdSelectable(CommandArgs* args);
void __stdcall CmdKill(CommandArgs* args);
void __stdcall CmdZBuffer(CommandArgs* args);
void __stdcall CmdAntiAlias(CommandArgs* args);
void __stdcall CmdShadow(CommandArgs* args);
void __stdcall CmdDither(CommandArgs* args);
void __stdcall CmdSwitchAlt(CommandArgs* args);
void __stdcall CmdTShadow(CommandArgs* args);
void __stdcall CmdFShadow(CommandArgs* args);
void __stdcall CmdLOSType(CommandArgs* args);
void __stdcall CmdLight(CommandArgs* args);
void __stdcall CmdRCache(CommandArgs* args);
void __stdcall CmdEdge(CommandArgs* args);
void __stdcall CmdSearch(CommandArgs* args);
void __stdcall CmdCDPlay(CommandArgs* args);
void __stdcall CmdCDStop(CommandArgs* args);
void __stdcall CmdSound3D(CommandArgs* args);
void __stdcall CmdMove(CommandArgs* args);
void __stdcall CmdLogo(CommandArgs* args);
void __stdcall CmdIWin(CommandArgs* args);
void __stdcall CmdILose(CommandArgs* args);
void __stdcall CmdSeaLevel(CommandArgs* args);
void __stdcall CmdControl(CommandArgs* args);
void __stdcall CmdView(CommandArgs* args);
void __stdcall CmdGive(CommandArgs* args);
void __stdcall CmdScrollSpeed(CommandArgs* args);
void __stdcall CmdIFace(CommandArgs* args);
void __stdcall CmdLOS(CommandArgs* args);
void __stdcall CmdMapping(CommandArgs* args);
void __stdcall CmdContour(CommandArgs* args);
void __stdcall CmdSelBoxes(CommandArgs* args);
void __stdcall CmdTreeDeath(CommandArgs* args);
void __stdcall CmdNoShake(CommandArgs* args);
void __stdcall CmdNow(CommandArgs* args);
void __stdcall CmdDoubleShot(CommandArgs* args);
void __stdcall CmdHalfShot(CommandArgs* args);
void __stdcall CmdRadar(CommandArgs* args);
void __stdcall CmdATM(CommandArgs* args);
void __stdcall CmdScreenChat(CommandArgs* args);
void __stdcall CmdNoMetal(CommandArgs* args);
void __stdcall CmdNoEnergy(CommandArgs* args);
void __stdcall CmdGamma(CommandArgs* args);
void __stdcall CmdSing(CommandArgs* args);
void __stdcall CmdClock(CommandArgs* args);
void __stdcall CmdFilm(CommandArgs* args);
void __stdcall CmdFilmSpeed(CommandArgs* args);
void __stdcall CmdSave(CommandArgs* args);
void __stdcall CmdReload(CommandArgs* args);
void __stdcall CmdReloadAIProfiles(CommandArgs* args);
void __stdcall CmdBigBrother(CommandArgs* args);
void __stdcall CmdProfile(CommandArgs* args);
void __stdcall CmdNowISee(CommandArgs* args);
void __stdcall CmdNetStats(CommandArgs* args);
void __stdcall CmdMusicMode(CommandArgs* args);
void __stdcall CmdMakePoster(CommandArgs* args);
void __stdcall CmdMeteor(CommandArgs* args);
void __stdcall CmdDrop(CommandArgs* args);
void __stdcall CmdInclude(CommandArgs* args);
void __stdcall CmdDebugBreak(CommandArgs* args);
void __stdcall CmdPrintWeights(CommandArgs* args);
void __stdcall CmdSenderror(CommandArgs* args);
void __stdcall CmdShootAll(CommandArgs* args);
void __stdcall CmdShareMetal(CommandArgs* args);
void __stdcall CmdShareEnergy(CommandArgs* args);
void __stdcall CmdShareMapping(CommandArgs* args);
void __stdcall CmdShareRadar(CommandArgs* args);
void __stdcall CmdShareAll(CommandArgs* args);
void __stdcall CmdSetShareMetal(CommandArgs* args);
void __stdcall CmdSetShareEnergy(CommandArgs* args);
void __stdcall CmdShowRanges(CommandArgs* args);
void __stdcall CmdCompression(CommandArgs* args);
void __stdcall CmdBPS(CommandArgs* args);
void __stdcall CmdSFX(CommandArgs* args);

// GLOBAL: 0x511de8
extern Game* g_game;
extern char DAT_005119b8[];
extern char g_commandLineBuf[];
extern int DAT_00511bc0;
extern int DAT_00511bc4;
extern int DAT_00511bc8;
extern int DAT_00511c20;
extern int DAT_00511c34;
extern int DAT_00511c48;
extern int DAT_00511c50;
extern int g_contourSpacing;           // contour spacing
extern int g_contourOffset;            // contour offset
extern int DAT_0051e698;
extern Pair_00419560 DAT_00511a60[44];
extern Pair_00419560 DAT_00511c60[44];
extern unsigned char g_contourColors[];  // colour by height band
extern unsigned char g_movementClassColors[];
extern signed char DAT_004fd670[], DAT_004fd678[];

void __stdcall AddMessage(char* text, int param_2, int param_3, int param_4);
extern void RemoveAllFeatures();
void* __stdcall GetMapCell(int x, int y);
void __stdcall RemoveFeature(void* target, int flag);
short __stdcall FindFeatureType(char* name);
void* __stdcall PlaceFeature(void* target, unsigned short id, void* pos, void* field_64,
                             unsigned char owner);
void __stdcall IssueOrderToSelection(void* a, int b, Class_00438760 kind, int d, int e, int f);
void SaveSettings();
void KillAllUnits(void);
void __stdcall KillPlayerUnits(unsigned char player);
void __stdcall RecalculateLineOfSight(int flag);
void __stdcall SetLightVector(int param_1, int param_2, int param_3);
void __stdcall TransferEnergy(unsigned char a, int b, float c, int d);
void __stdcall TransferMetal(unsigned char a, int b, float c, int d);
void __stdcall SetBrightness(float value);
void __stdcall MakeDirectoryPath(char* dir);
void __stdcall SaveGameFile(char* path, char* description, int param_3);
short __stdcall FindUnitTypeId(char* name);
void __stdcall KillUnitsOfType(short id);
void __stdcall ReloadUnitType(unsigned short id);
extern void ResetAIPlayers();
extern void ClearCameraFollowState(void);
void FUN_004161f0();
void __stdcall WriteScreenshot(char* name, char* description, int x, int y, int w, int h);
unsigned int GetTicks();
void EnableMeteors();
void DisableMeteors();
void StartMeteorShower();
void* __stdcall HAPI_OpenFileRead(char* path);
void* __stdcall HAPI_LoadOpenFile(char* path, void* file, int* out);
unsigned int __stdcall ExecuteCommandText(char* text, int size, CommandArgs* vars, unsigned int param_4);
void __cdecl FUN_004d85a0(void* data);
int __stdcall HAPI_CloseFile(void* file);
int __stdcall MatchWildcard(const char* name, const char* pattern);
void __stdcall SnapWorldPosToFootprint(UnitDef* def, Vec3* pos);
void* __stdcall CreateUnit(unsigned char owner, short id, Vec3 pos, int param_4, int param_5, int param_6);
extern Display_00417a60* GetDisplay();
void ToggleFullScreen();
void __cdecl FUN_004d83b0(char* name, unsigned int size);
void __stdcall ExecuteCommand(void* param_1, int param_2);
int __stdcall GetGroundHeight(Pos_00417bb0* pos);
// The real callee takes unsigned char; int here reproduces the original's
// loop-invariant widening of the colour byte.
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, int color);
void __stdcall DrawMapTileSelectionOutline(void* a, short* b, int c, int d);
void __stdcall SetFont(int font);
int __stdcall GetTextKeyColor();
void __stdcall SetTextColors(int color, int background);
void __stdcall DrawString(void* surface, const char* text, int x, int y, int maxWidth);
void __stdcall FillPolygon(void* surface, Point_00417f60* points, int count, int color);
void __stdcall FillRectangle(void* surface, Rect* rect, int color);
void __stdcall DrawCellContours(void* surface, Point_00417f60* corners, unsigned char* heights);
Unit* __stdcall FindNextSelectedUnit(int, int);
void __stdcall DumpPlayerAI(int player, FILE* file);

// Creates (truncates) memdump.txt and closes it again; the argument is unused.
// FUNCTION: 0x416240
void __stdcall CmdMemDump(char* args)
{
    HANDLE file = CreateFileA("memdump.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, 0);
    if (file != 0) {
        CloseHandle(file);
    }
}

// FUNCTION: 0x416270
void __stdcall CmdMem(int)
{
}

// FUNCTION: 0x416280
void __stdcall CmdAI(CommandArgs* args)
{
    unsigned char i = args->GetIntArg(1, g_game->localPlayer);
    if (i < 10) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            // Re-derived pointer: MSVC then re-reads the fields instead of
            // reusing the values tested above.
            Player* q = &g_game->players[i];
            if (q->active != 0 && q->type == 1)
                q->SetType(2);
            else
                q->SetType(1);
        }
    }
}

// Console command: looks up the order type named by argument 1 and, when it
// exists, hands it to IssueOrderToSelection with arguments 1 and 2 as numbers.
// FUNCTION: 0x416310
void __stdcall CmdAssign(CommandArgs* args)
{
    Class_00438760 kind(((CommandArgs*)args)->GetArg(1, DAT_005119b8));
    if (kind.index) {
        int a = args->GetIntArg(1, 0);
        int b = args->GetIntArg(2, 0);
        IssueOrderToSelection((char*)g_game + 0x2c76, 0, kind, 0, a, b);
    }
}

// FUNCTION: 0x416370
void __stdcall CmdAssert(int)
{
}

// FUNCTION: 0x416380
void __stdcall CmdDPrint(int)
{
}

// FUNCTION: 0x416390
void __stdcall CmdBurnAll(int arg1)
{
    RemoveAllFeatures();
}

// Console command handler (same family as 0x4163d0); the argument is unused.
// FUNCTION: 0x4163a0
void __stdcall CmdBurnOne(void* args)
{
    if (g_game->cellFeature < 0xfffb) {
        void* target = GetMapCell(g_game->point.x, g_game->point.y);
        RemoveFeature(target, 1);
    }
}

// FUNCTION: 0x4163d0
void __stdcall CmdFeature(CommandArgs* args)
{
    unsigned short id = FindFeatureType(args->GetArg(1, DAT_005119b8));
    if (id != 0xffff) {
        void* target = GetMapCell(g_game->point.x, g_game->point.y);
        PlaceFeature(target, id, 0, 0, 10);
    }
}

// FUNCTION: 0x416420
void __stdcall CmdShading(int unused)
{
    g_game->flags.shading = !g_game->flags.shading;
    g_game->obj->FlushCache();
    SaveSettings();
}

// FUNCTION: 0x416460
void __stdcall CmdSelectable(int unused)
{
    for (Unit* u = &g_game->units[1]; u <= g_game->units_end; u++) {
        if (u->flags & 0x10000000) {
            u->flags |= 0x20;
        }
    }
}

// Command callback: with no arguments (token count 1) resets everything
// (KillAllUnits); otherwise sets the local player (KillPlayerUnits) from
// argument 1. Both paths then pump the object at g_game+0x391ed.
// Compare 0x4169d0 / 0x416a30.
// FUNCTION: 0x4164b0
void __stdcall CmdKill(CommandArgs* args)
{
    if (args->count == 1) {
        KillAllUnits();
    } else {
        KillPlayerUnits(args->GetIntArg(1, 0));
    }
    g_game->field_391ed->Deactivate();
}

// FUNCTION: 0x416500
void __stdcall CmdZBuffer(int)
{
}

// Same as 0x416420 with bit 1 of the flags word at +0x37f06.
// FUNCTION: 0x416510
void __stdcall CmdAntiAlias(int unused)
{
    g_game->flags.antiAlias = !g_game->flags.antiAlias;
    g_game->obj->FlushCache();
    SaveSettings();
}

// Same as 0x416510 with bit 2 of the flags word at +0x37f06.
// FUNCTION: 0x416550
void __stdcall CmdShadow(int unused)
{
    g_game->flags.shadows = !g_game->flags.shadows;
    g_game->obj->FlushCache();
    SaveSettings();
}

// FUNCTION: 0x416590
void __stdcall CmdDither(int unused)
{
    g_game->flags.ditheredFog = !g_game->flags.ditheredFog;
    SaveSettings();
}

// FUNCTION: 0x4165c0
void __stdcall CmdSwitchAlt(CommandArgs* args)
{
    if (args->count < 2) {
        g_game->flags.switchAlt = !g_game->flags.switchAlt;
        SaveSettings();
        return;
    }
    g_game->flags.switchAlt = args->GetIntArg(1, 0);
}

// Toggles one flag bit in a 16-bit bitfield inside the game state.
// FUNCTION: 0x416630
void __stdcall CmdTShadow(int unused)
{
    g_game->flags.vehicleShadows = !g_game->flags.vehicleShadows;
}

// Toggles one flag bit in a 16-bit bitfield inside the game state (the same
// flags word as 0x416630 and 0x417060).
// FUNCTION: 0x416660
void __stdcall CmdFShadow(int unused)
{
    g_game->flags.featureShadows = !g_game->flags.featureShadows;
}

// FUNCTION: 0x416690
void __stdcall CmdLOSType(int unused)
{
    g_game->viewFlags.bit2 = !g_game->viewFlags.bit2;
    RecalculateLineOfSight(0);
}

// FUNCTION: 0x4166c0
void __stdcall CmdLight(CommandArgs* args)
{
    SetLightVector(args->GetIntArg(1, 0), args->GetIntArg(2, 0), args->GetIntArg(3, 0));
    g_game->obj->FlushCache();
}

// FUNCTION: 0x416710
void __stdcall CmdRCache(int unused)
{
    g_game->obj->FlushCache();
}

// FUNCTION: 0x416730
void __stdcall CmdEdge(CommandArgs* args)
{
    g_game->mapWidth = g_game->baseX - args->GetIntArg(1, 0x20);
    g_game->mapHeight = g_game->baseY - args->GetIntArg(2, 0x80);
}

// Console command: sets two fields of a game sub-object, the second from a
// 16.16 fixed-point number.
// FUNCTION: 0x416780
void __stdcall CmdSearch(CommandArgs* args)
{
    if (args->GetIntArg(1, 0)) {
        g_game->paths->value = args->GetIntArg(1, 0);
    }
    if (args->count == 3) {
        g_game->paths->fixed = (int)(atof(((CommandArgs*)args)->GetArg(2, DAT_005119b8)) * 65536.0);
    }
}

// FUNCTION: 0x4167f0
void __stdcall CmdCDPlay(CommandArgs* param_1)
{
    ((Sound*)g_game->sound)->PlayCdTrack(param_1->GetIntArg(1, 0), 1);
}

// FUNCTION: 0x416810
void __stdcall CmdCDStop(int unused)
{
    ((Class_004ced40*)g_game->sound)->StopCdAudio();
}

// FUNCTION: 0x416820
void __stdcall CmdSound3D(int unused)
{
    if (((Sound*)g_game->sound)->Is3DEnabled()) {
        ((Sound*)g_game->sound)->Disable3D();
        SaveSettings();
    } else {
        ((Sound*)g_game->sound)->Enable3D();
        SaveSettings();
    }
}

// Console command callback: "move <dx> <dz>" shifts a game position at
// +0x2caa/+0x2cb2 by whole units (<< 20). Each call result goes through a
// local; `g_game->pos.x += f() << 20` loads g_game before the call.
// FUNCTION: 0x416860
void __stdcall CmdMove(CommandArgs* args)
{
    if (_strcmpi(((CommandArgs*)args)->GetArg(0, DAT_005119b8), "move") == 0) {
        int dx = args->GetIntArg(1, 0);
        g_game->pos.x += dx << 20;
        int dz = args->GetIntArg(2, 0);
        g_game->pos.z += dz << 20;
    }
}

// Console command callback: applies a logo selection to a player.
// FUNCTION: 0x4168d0
void __stdcall CmdLogo(CommandArgs* args)
{
    int n = args->GetIntArg(1, 0);
    if (n >= 0) {
        int m = args->GetIntArg(1, 0);
        unsigned int limit = 0;
        limit = *(unsigned short*)g_game->logos32;
        if (m < (int)limit) {
            unsigned char i = args->GetIntArg(2, 0);
            if (i < 10) {
                Player* p = &g_game->players[i];
                if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10) {
                    int v = args->GetIntArg(1, 0);
                    int j = args->GetIntArg(2, 0);
                    g_game->players[j].data->field_96 = v;
                    g_game->obj->FlushCache();
                    return;
                }
            }
        }
    }
    AddMessage("Invalid logo setting", 2, 0, 10);
}

// Command callback: calls KillPlayerUnits with 0 when the local player's byte
// at +0x95 of its data block is 1, otherwise with 1, and sets game flags at
// +0x3923b; compare 0x416a30.
// FUNCTION: 0x4169d0
void __stdcall CmdIWin(CommandArgs* args)
{
    if (g_game->players[g_game->localPlayer].data->field_95 == 1)
        KillPlayerUnits(0);
    else
        KillPlayerUnits(1);
    g_game->flag4 = 1;
    g_game->flag5 = 1;
    g_game->flag2 = 1;
}

// Command callback: passes the local player's byte at +0x95 of its data
// block to KillPlayerUnits and updates the game flags at +0x3923b; compare
// 0x4169d0.
// FUNCTION: 0x416a30
void __stdcall CmdILose(CommandArgs* args)
{
    KillPlayerUnits(g_game->players[g_game->localPlayer].data->field_95);
    g_game->flag4 = 0;
    g_game->flag6 = 1;
    g_game->flag2 = 1;
}

// FUNCTION: 0x416a90
void __stdcall CmdSeaLevel(CommandArgs* args)
{
    g_game->seaLevel = args->GetIntArg(1, 0);
}

// FUNCTION: 0x416ab0
void __stdcall CmdControl(CommandArgs* args)
{
    unsigned char i = args->GetIntArg(1, 0);
    if (i < 10) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            g_game->localPlayer = args->GetIntArg(1, 0);
            g_game->playerIndex = args->GetIntArg(1, 0);
        }
    }
}

// FUNCTION: 0x416b50
void __stdcall CmdView(CommandArgs* args)
{
    unsigned char i = args->GetIntArg(1, 0);
    if (i < 10) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            g_game->playerIndex = args->GetIntArg(1, 0);
        }
    }
}

// FUNCTION: 0x416bd0
void __stdcall CmdGive(CommandArgs* args)
{
    unsigned char i = args->GetIntArg(1, 0);
    if (i < 10) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            if (_strcmpi(((CommandArgs*)args)->GetArg(3, DAT_005119b8), "metal") == 0) {
                TransferEnergy(g_game->localPlayer, args->GetIntArg(1, 0),
                             (float)args->GetIntArg(2, 0), 1);
            }
            if (_strcmpi(((CommandArgs*)args)->GetArg(3, DAT_005119b8), "energy") == 0) {
                TransferMetal(g_game->localPlayer, args->GetIntArg(1, 0),
                             (float)args->GetIntArg(2, 0), 1);
            }
        }
    }
}

// FUNCTION: 0x416cf0
void __stdcall CmdScrollSpeed(void* param_1)
{
    unsigned char result = ((CommandArgs*)param_1)->GetIntArg(1, 0);
    g_game->scrollSpeed = result;
    SaveSettings();
}

// FUNCTION: 0x416d20
void __stdcall CmdIFace(void* param_1)
{
    int result = ((CommandArgs*)param_1)->GetIntArg(1, 0);
    g_game->field_37efa = result;
    SaveSettings();
}

// FUNCTION: 0x416d50
void __stdcall CmdLOS(int unused)
{
    g_game->viewFlags.bit1 = !g_game->viewFlags.bit1;
    SaveSettings();
    RecalculateLineOfSight(0);
}

// Toggles bit 0 of the game flags at +0x14281 (0x416d50 toggles bit 1).
// FUNCTION: 0x416d80
void __stdcall CmdMapping(int unused)
{
    g_game->viewFlags.bit0 = !g_game->viewFlags.bit0;
    SaveSettings();
    RecalculateLineOfSight(1);
}

// FUNCTION: 0x416db0
void __stdcall CmdContour(CommandArgs* args)
{
    g_contourSpacing = (int)(args->GetFloatArg(1, 0.0f) * 256.0f);
    g_contourOffset = (int)(args->GetFloatArg(2, 0.75f) * 256.0f);
}

// Toggles one flag bit in a 16-bit bitfield inside the game state (the same
// flags word as 0x417060, mask 4 = bit 2).
// FUNCTION: 0x416e00
void __stdcall CmdSelBoxes(int unused)
{
    g_game->b2 = !g_game->b2;
}

// FUNCTION: 0x416e30
void __stdcall CmdTreeDeath(int unused)
{
    g_game->b3 = !g_game->b3;
}

// Toggles bit 4 of the flags word at g_game+0x37f2f (like 0x416e30, bit 3).
// FUNCTION: 0x416e60
void __stdcall CmdNoShake(int unused)
{
    g_game->b4 = !g_game->b4;
}

// FUNCTION: 0x416e90
void __stdcall CmdNow(CommandArgs* args)
{
    if (args->count==6 && !strcmp(args->GetArg(1,DAT_005119b8),"Film") &&
        !strcmp(args->GetArg(2,DAT_005119b8),"Chris") &&
        !strcmp(args->GetArg(3,DAT_005119b8),"Include") &&
        !strcmp(args->GetArg(4,DAT_005119b8),"Reload") &&
        !strcmp(args->GetArg(5,DAT_005119b8),"Assert"))
        g_game->flags_37f2f|=2;
    else g_game->flags_37f2f&=~2;
}

// FUNCTION: 0x417030
void __stdcall CmdDoubleShot(int unused)
{
    unsigned short* ptr = &g_game->flags_37f2f;
    unsigned short ax = *ptr;
    unsigned short edx = ax;
    edx = (unsigned short)(~edx);
    edx ^= ax;
    edx &= 0x80;
    edx ^= ax;
    *ptr = edx;
}

// Toggles one flag bit in a 16-bit bitfield inside the game state (the same
// flags word as 0x416e30 and 0x418ca0).
// FUNCTION: 0x417060
void __stdcall CmdHalfShot(int unused)
{
    g_game->b8 = !g_game->b8;
}

// FUNCTION: 0x417090
void __stdcall CmdRadar(int unused)
{
    g_game->b9 = !g_game->b9;
}

// Command callback (cheat): adds 1000 metal and 1000 energy to the local
// player.
// FUNCTION: 0x4170c0
void __stdcall CmdATM(CommandArgs* args)
{
    g_game->players[g_game->playerIndex].field_8c += 1000.0f;
    g_game->players[g_game->playerIndex].field_98 += 1000.0f;
}

// FUNCTION: 0x417130
void __stdcall CmdScreenChat(int unused)
{
    g_game->field_37f02 ^= 1;
    SaveSettings();
}

// FUNCTION: 0x417150
void __stdcall CmdNoMetal(CommandArgs* args)
{
    unsigned char i;
    if (args->count == 1)
        i = g_game->localPlayer;
    else
        i = args->GetIntArg(1, 0);
    if (i < 10) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            g_game->players[i].field_98 = args->GetIntArg(2, 0);
        }
    }
}

// Console command, twin of 0x417150 (which sets the float at +0x98): sets a
// player's float at +0x8c from the second argument. The player is the first
// argument, or the local player when there is only the command word.
// FUNCTION: 0x4171f0
void __stdcall CmdNoEnergy(CommandArgs* args)
{
    unsigned char i;
    if (args->count == 1)
        i = g_game->localPlayer;
    else
        i = args->GetIntArg(1, 0);
    if (i < 10) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            g_game->players[i].field_8c = args->GetIntArg(2, 0);
        }
    }
}

// Console command: sets the brightness from the first argument (tenths).
// FUNCTION: 0x417290
void __stdcall CmdGamma(CommandArgs* args)
{
    SetBrightness(args->GetIntArg(1, 0) * 0.1f);
    g_game->brightness = args->GetIntArg(1, 0);
    SaveSettings();
}

// A console command handler (like its neighbours 0x417130 and 0x417300):
// toggles a global flag.
// FUNCTION: 0x4172e0
void __stdcall CmdSing(int unused)
{
    DAT_0051e698 = !DAT_0051e698;
}

// Toggles one flag bit in the same 16-bit flags word as 0x416e30, 0x417060
// and 0x418ca0 (mask 0x40 = bit 6), then calls SaveSettings.
// FUNCTION: 0x417300
void __stdcall CmdClock(int unused)
{
    g_game->b6 = !g_game->b6;
    SaveSettings();
}

// FUNCTION: 0x417330
void __stdcall CmdFilm(CommandArgs* args)
{
    if (args->count > 1) {
        strcpy(g_game->path, args->GetArg(1, DAT_005119b8));
        if (g_game->path[strlen(g_game->path) - 1] == '\\' ||
            g_game->path[strlen(g_game->path) - 1] == '/')
            g_game->path[strlen(g_game->path) - 1] = 0;
        g_game->field_38c5f = 1;
        SaveSettings();
    }
}

// Console command: takes a positive number from the first argument.
// FUNCTION: 0x4173e0
void __stdcall CmdFilmSpeed(CommandArgs* args)
{
    if (args->count > 1 && args->GetIntArg(1, 0) > 0) {
        g_game->field_38c57 = args->GetIntArg(1, 0);
        g_game->field_38c63 = 1;
        SaveSettings();
    }
}

// FUNCTION: 0x417430
void __stdcall CmdSave(CommandArgs* args)
{
    char path[256];
    if (args->count > 1) {
        sprintf(path, "savegame\\%s.sav", args->GetArg(1, DAT_005119b8));
        MakeDirectoryPath("savegame");
        SaveGameFile(path, "Generic Game Description", 0x29a);
    }
}

// Command handler (sibling of 0x417430): looks up the unit type named by the
// first argument and, if it exists, applies KillUnitsOfType and ReloadUnitType
// to its id.
// FUNCTION: 0x417490
void __stdcall CmdReload(CommandArgs* args)
{
    if (args->count > 1) {
        short id = FindUnitTypeId(args->GetArg(1, DAT_005119b8));
        if (id) {
            KillUnitsOfType(id);
            ReloadUnitType(id);
        }
    }
}

// FUNCTION: 0x4174d0
void __stdcall CmdReloadAIProfiles(int arg1)
{
    ResetAIPlayers();
}

// FUNCTION: 0x4174e0
void __stdcall CmdBigBrother(int unused)
{
    if (g_game->paused) {
        g_game->paused = 0;
        ClearCameraFollowState();
    } else {
        g_game->paused = 1;
        g_game->autoFollowTicks = 1;
    }
}

// Chat command handler (table at 0x5020b8): toggles a game flag.
// FUNCTION: 0x417520
void __stdcall CmdProfile(void* args)
{
    g_game->field_38dd5 = g_game->field_38dd5 == 0 ? 1 : 0;
}

// FUNCTION: 0x417540
void __stdcall CmdNowISee(void*)
{
    void* p_game = g_game;
    unsigned short* p = (unsigned short*)((char*)p_game + 0x14281);
    *p = *p & 0xfffe;

    p_game = g_game;
    p = (unsigned short*)((char*)p_game + 0x14281);
    *p = *p & 0xfffd;

    RecalculateLineOfSight(1);
}

// Same reset sequence as InitCommands, run as a callback after FUN_004161f0.
// FUNCTION: 0x417570
void __stdcall CmdNetStats(int)
{
    FUN_004161f0();
    DAT_00511c20 = g_game->field_38a47;
    DAT_00511bc0 = 0;
    DAT_00511bc4 = 0;
    Pair_00419560* p = DAT_00511c60;
    for (int i = 0; i < 44; i++) {
        DAT_00511a60[i].a = 0;
        DAT_00511a60[i].b = 0;
        DAT_00511c60[i].a = 0;
        p->b = 0;
        p++;
    }
    DAT_00511c34 = 0;
    DAT_00511bc8 = 0;
    DAT_00511c48 = 0;
    DAT_00511c50 = 0;
}

// FUNCTION: 0x4175e0
void __stdcall CmdMusicMode(void* param_1)
{
    CommandArgs* obj = (CommandArgs*)param_1;
    int result = obj->GetIntArg(1, 0);
    void* p = *(void**)((char*)g_game + 0x10);
    ((Sound*)p)->SetTrackCategory(result);
}

// FUNCTION: 0x417600
void __stdcall CmdMakePoster(CommandArgs* args)
{
    int w = 0xc80;
    int h = 0x960;
    if (args->count > 1)
        w = ((CommandArgs*)args)->GetIntArg(1, 0);
    if (args->count > 2)
        h = ((CommandArgs*)args)->GetIntArg(2, 0);
    if (_strcmpi(args->GetArg(1, DAT_005119b8), "all") == 0) {
        w = g_game->mapWidth;
        h = g_game->mapHeight;
    }
    int sx = g_game->screenTilesX;
    w = max(w, sx * 16);
    w = min(w, g_game->mapWidth);
    int sy = g_game->screenTilesY;
    h = max(h, sy * 16);
    h = min(h, g_game->mapHeight);
    int x = g_game->scrollX - w / 2 + sx * 8;
    int y = g_game->scrollY - h / 2 + sy * 8;
    x = max(x, 0);
    x = min(x, g_game->mapWidth - w);
    y = max(y, 0);
    y = min(y, g_game->mapHeight - h);
    char buf[256];
    sprintf(buf, "%s\\screenshots", g_game->path);
    MakeDirectoryPath(buf);
    WriteScreenshot(buf, "BIGSHOT", x, y, w, h);
    g_game->lastShotTime = GetTicks();
}

// Console command: with an argument, calls EnableMeteors or DisableMeteors
// depending on it; without one, calls StartMeteorShower.
// FUNCTION: 0x417760
void __stdcall CmdMeteor(CommandArgs* args)
{
    if (args->count > 1) {
        if (args->GetIntArg(1, 0))
            EnableMeteors();
        else
            DisableMeteors();
    } else {
        StartMeteorShower();
    }
}

// FUNCTION: 0x4177a0
void __stdcall CmdDrop(CommandArgs* args)
{
    int value = args->GetIntArg(1, 0);
    g_game->b0 = value == 0;
}

// Debug-data dump helper (sibling of 0x417430/0x417490): builds
// "debugdat\\<name>.txt", saves the game position at +0x2caa as a Vec3 (the
// x/y/z values survive the calls in registers, only z is spilled), reads the
// file's contents and writes them back out, then restores the position.
// FUNCTION: 0x4177e0
void __stdcall CmdInclude(CommandArgs* args)
{
    Vec3 pos;
    int info;
    char path[60];

    sprintf(path, "debugdat\\%s.txt", args->GetArg(0, DAT_005119b8));
    void* file = HAPI_OpenFileRead(path);
    if (file != 0) {
        pos = g_game->pos;
        void* data = HAPI_LoadOpenFile(path, file, &info);
        if (data != 0) {
            ExecuteCommandText((char*)data, info, args, 0xffffffff);
            FUN_004d85a0(data);
        }
        HAPI_CloseFile(file);
        g_game->pos = pos;
    }
}

// FUNCTION: 0x417890
void __stdcall DefaultCommandHandler(CommandArgs* args)
{
    Vec3 pos=g_game->pos;
    int count=0;
    for (unsigned short i=1;i<g_game->count;++i) {
        UnitDef* def=&g_game->defs[i];
        if (MatchWildcard(def->name,args->GetArg(0,DAT_005119b8))) {
            if (count) pos.x-=def->min.x;
            SnapWorldPosToFootprint(def,&pos);
            CreateUnit(((CommandArgs*)args)->GetIntArg(1,0),i,pos,1,1,0);
            pos.x+=def->max.x+0x200000;
            if (pos.x >= (g_game->mapWidth<<16)) { pos.x=0xa00000; pos.z+=0xa00000; }
            ++count;
        }
    }
    if (!count) {
        Vec3 saved;
        int size;
        char path[60];
        // The original unbounded formatting can overflow path for a long argument.
        sprintf(path,"debugdat\\%s.txt",args->GetArg(0,DAT_005119b8));
        void* file=HAPI_OpenFileRead(path);
        if (file) {
            saved=g_game->pos;
            void* data=HAPI_LoadOpenFile(path,file,&size);
            if (data) {
                ExecuteCommandText((char*)data,size,args,0xffffffff);
                FUN_004d85a0(data);
            }
            HAPI_CloseFile(file);
            g_game->pos=saved;
        }
    }
}

// Debug "crash" chat command (cheats enabled only): argument 1 exhausts
// memory with operator new, 2 through FUN_004d83b0, 3 exits with a division
// by zero; no argument or 0 breaks into the debugger.
// FUNCTION: 0x417a60
void __stdcall CmdDebugBreak(CommandArgs* args)
{
    if ((g_game->flags_37f2f & 2) && (g_game->flags_3923b & 2)) {
        if (args->count > 0) {
            if (args->GetIntArg(1, 0) == 1) {
                for (;;)
                    operator new(0x2000000);
            }
            if (args->GetIntArg(1, 0) == 2) {
                for (;;)
                    FUN_004d83b0("FORCE OUT-OF-MEMORY", 0x2000000);
            }
            if (args->GetIntArg(1, 0) == 3) {
                volatile int one = 1;   // keeps the division by zero out of the constant folder
                exit(one / (one >> 1));
                return;
            }
        }
        if (args->count == 0 || args->GetIntArg(1, 0) == 0) {
            if (GetDisplay()->windowed) {
                ToggleFullScreen();
                Sleep(500);
            }
            DebugBreak();
        }
    }
}

// Splits the command line param_1 into tokens in a local tokenizer object and
// dispatches it with the flags param_2. A null command line copies the global
// command buffer g_commandLineBuf instead. Compare 0x416780.
// FUNCTION: 0x417b50
void __stdcall ExecuteCommandLine(char* param_1, int param_2)
{
    char buf[0xd4];

    if (param_1 == 0)
        param_1 = g_commandLineBuf;
    else
        strncpy(g_commandLineBuf, param_1, 0x4f);

    ((CommandArgs*)buf)->InitArgs();
    ((CommandArgs*)buf)->Tokenize(param_1, 0);
    ExecuteCommand(buf, param_2);
}

// Converts a 16.16 fixed-point world position into screen coordinates.
// FUNCTION: 0x417bb0
void __stdcall WorldToScreenWithHeight(Pos_00417bb0* pos, int* screen_x, int* screen_y)
{
    int height = GetGroundHeight(pos);
    *screen_x = pos->x - g_game->scrollX + 0x80;
    *screen_y = pos->z - g_game->scrollY - (height >> 1) + 0x20;
}

static inline void WorldToScreen(Pos_00417bb0* pos, int* screen_x, int* screen_y)
{
    int height = GetGroundHeight(pos);
    *screen_x = pos->x - g_game->scrollX + 0x80;
    *screen_y = pos->z - g_game->scrollY - (height >> 1) + 0x20;
}

// Converts a map position in whole units into screen coordinates, through the
// 16.16 fixed-point conversion of 0x417bb0 (inlined).
// FUNCTION: 0x417c00
void __stdcall WorldToScreenTileOffset(int x, int z, int* screen_x, int* screen_y)
{
    Pos_00417bb0 pos;
    *(int*)&pos.x_frac = x << 16;
    *(int*)&pos.z_frac = z << 16;
    WorldToScreen(&pos, screen_x, screen_y);
}

// Draws a line from a world position to the same position shifted by
// (dx, dz) whole map units, converting both ends to screen coordinates.
// FUNCTION: 0x417c70
void __stdcall DrawWorldDeltaLine(void* surface, Pos_00417bb0* p, short dx, short dz, int color)
{
    Pos_00417bb0 pos;
    *(int*)&pos.x_frac = (dx << 16) + *(int*)&p->x_frac;
    *(int*)&pos.z_frac = (dz << 16) + *(int*)&p->z_frac;

    int h1 = GetGroundHeight(p);
    int x1 = p->x - g_game->scrollX + 0x80;
    int y1 = p->z - g_game->scrollY - (h1 >> 1) + 0x20;
    int h2 = GetGroundHeight(&pos);
    int x2 = pos.x - g_game->scrollX + 0x80;
    int y2 = pos.z - g_game->scrollY - (h2 >> 1) + 0x20;

    DrawLine(surface, x1, y1, x2, y2, color & 0xff);
}

// Draws a line on the given surface between two map positions converted to
// screen coordinates: a start point (a 4-byte pair of shorts) and an end point
// formed by adding the dx/dz deltas to it.
// FUNCTION: 0x417d30
void __stdcall DrawWorldSegmentLine(void* surface, Point16 from, short dx, short dz, int color)
{
    Pos_00417bb0 pos1, pos2;
    int x1 = from.x << 16;
    int z1 = from.z << 16;
    *(int*)&pos1.x_frac = x1;
    *(int*)&pos1.z_frac = z1;
    *(int*)&pos2.x_frac = x1 + (dx << 16);
    *(int*)&pos2.z_frac = z1 + (dz << 16);
    int sx1, sy1, sx2, sy2;
    WorldToScreen(&pos1, &sx1, &sy1);
    WorldToScreen(&pos2, &sx2, &sy2);
    DrawLine(surface, sx1, sy1, sx2, sy2, color & 0xff);
}

// Class_0044f010's override of slot 10 (vtable 0x4fd458). The rest of the
// class is in order_targets_44f010.cpp; this one sits far from it in the exe,
// compiled with the console code.
// Draws an open polyline stored on the object as 16-bit map points: each
// consecutive pair is converted to screen space (through the 16.16 fixed-point
// helper of 0x417bb0) and one line is drawn. The colour byte comes from a
// two-entry table in game state, selected by bit 0 of the object's field 0x64.
// FUNCTION: 0x417e00
void Class_0044f010::DrawOnSurface(void* surface)
{
    DrawMapTileSelectionOutline(surface, (short*)(this->field_8 + 0x76), *(int*)(this->field_8 + 0x7e), 0xf);
    unsigned char color = *(unsigned char*)((char*)g_game + 0xdcb + ((this->field_64 & 1) ? 9 : 12));
    for (int i = 0; i < this->count - 1; i++) {
        Pos_00417bb0 p1;
        *(int*)&p1.x_frac = this->points[i].x << 16;
        *(int*)&p1.z_frac = this->points[i].z << 16;
        int h = GetGroundHeight(&p1);
        int x1 = p1.x - g_game->scrollX + 0x80;
        int y1 = p1.z - g_game->scrollY - (h >> 1) + 0x20;

        Pos_00417bb0 p2;
        *(int*)&p2.x_frac = this->points[i + 1].x << 16;
        *(int*)&p2.z_frac = this->points[i + 1].z << 16;
        h = GetGroundHeight(&p2);
        int x2 = p2.x - g_game->scrollX + 0x80;
        int y2 = p2.z - g_game->scrollY - (h >> 1) + 0x20;

        DrawLine(surface, x1, y1, x2, y2, color);
    }
}

// FUNCTION: 0x417f30
void __stdcall DrawSelectedGoal(int param_1, Iface_00417f30*** param_2)
{
    if (param_2 && *param_2 && **param_2) {
        (**param_2)->Method_28(param_1);
    }
}

// Draws the contour lines that cross one triangle of the height map. Each
// corner is a screen point and a height (8 fraction bits). The corners are
// sorted by height, then every contour level between the lowest and highest
// corner (the multiples of g_contourSpacing, offset by g_contourOffset) is drawn as
// one line across the triangle: first where it crosses the long edge 1-3 and
// the upper edge 2-3, then the long edge and the lower edge 1-2. The colour
// comes from the level's height above sea level through g_contourColors.
// FUNCTION: 0x417f60
void __stdcall DrawTriangleContours(void* surface, Point_00417f60 p1, int z1,
                            Point_00417f60 p2, int z2,
                            Point_00417f60 p3, int z3)
{
    int tz;
    Point_00417f60 tp;
    if (z2 > z3) {
        tz = z2; z2 = z3; z3 = tz;
        tp = p2; p2 = p3; p3 = tp;
    }
    if (z1 > z2) {
        tz = z1; z1 = z2; z2 = tz;
        tp = p1; p1 = p2; p2 = tp;
    }
    if (z2 > z3) {
        tz = z2; z2 = z3; z3 = tz;
        tp = p2; p2 = p3; p3 = tp;
    }
    int level = z3 / g_contourSpacing * g_contourSpacing + g_contourOffset;
    while (level > z3) {
        level -= g_contourSpacing;
    }
    if (level > z2) {
        int dz13 = z3 - z1;
        int dz23 = z3 - z2;
        do {
            int h13 = level - z1;
            int h23 = level - z2;
            DrawLine(surface,
                         (p1.x * (dz13 - h13) + h13 * p3.x) / dz13,
                         (p1.y * (dz13 - h13) + h13 * p3.y) / dz13,
                         (p2.x * (dz23 - h23) + h23 * p3.x) / dz23,
                         (p2.y * (dz23 - h23) + h23 * p3.y) / dz23,
                         g_contourColors[((level >> 8) - g_game->seaLevel + 0x100) >> 4]);
            level -= g_contourSpacing;
        } while (level > z2);
    }
    if (level > z1) {
        int dz13 = z3 - z1;
        int dz12 = z2 - z1;
        do {
            int h13 = level - z1;
            // Separate local from h13: a shared one computes both weights through one neg.
            int h12 = level - z1;
            DrawLine(surface,
                         (p1.x * (dz13 - h13) + h13 * p3.x) / dz13,
                         (p1.y * (dz13 - h13) + h13 * p3.y) / dz13,
                         (p1.x * (dz12 - h12) + h12 * p2.x) / dz12,
                         (p1.y * (dz12 - h12) + h12 * p2.y) / dz12,
                         g_contourColors[((level >> 8) - g_game->seaLevel + 0x100) >> 4]);
            level -= g_contourSpacing;
        } while (level > z1);
    }
}

// Draws one map cell: four triangles fanning from the cell's centre, whose
// position and height are the rounded averages of the four corners. The
// corner heights are bytes, scaled to 8 fraction bits.
// FUNCTION: 0x4181d0
void __stdcall DrawCellContours(void* surface, Point_00417f60* corners, unsigned char* heights)
{
    Point_00417f60 mid;
    mid.x = (corners[0].x + corners[1].x + corners[2].x + corners[3].x + 2) / 4;
    mid.y = (corners[0].y + corners[1].y + corners[2].y + corners[3].y + 2) / 4;
    int midHeight = (heights[0] + heights[1] + heights[2] + heights[3]) * 64;
    // "* 256", not "<< 8": gives mov cl,[mem]; shl ecx,8 instead of mov ch.
    DrawTriangleContours(surface, corners[0], heights[0] * 256, corners[1], heights[1] * 256, mid, midHeight);
    DrawTriangleContours(surface, corners[1], heights[1] * 256, corners[2], heights[2] * 256, mid, midHeight);
    DrawTriangleContours(surface, corners[2], heights[2] * 256, corners[3], heights[3] * 256, mid, midHeight);
    DrawTriangleContours(surface, corners[3], heights[3] * 256, corners[0], heights[0] * 256, mid, midHeight);
}

// Debug overlay for the map: walks the visible tiles, works out the screen
// quad of each one from its four corner heights and draws what the current
// debug mode asks for (1: movement classes and path arrows, 2: tile
// contents, 3: metal values, 4: fog of war), plus the contour lines of
// 0x4181d0 when g_contourSpacing is set.
//
// The header set fixes the operand order of the tile and fog multiplies.
// <direct.h> is only for its symbol ids: it puts 0x418310 in the window it
// matches in (docs/c2-regalloc.md), and must not sit at the top, where it
// moves 0x4181d0 out of its own window.
#include <direct.h>
// FUNCTION: 0x418310
void __stdcall DrawMapDebugOverlay(void* surface)
{
    if (!g_game->mode && !g_contourSpacing) return;
    Movement* movement = 0;
    Player* player = &g_game->players[g_game->playerIndex];
    if (g_game->mode == 1) {
        Unit* unit = FindNextSelectedUnit(0, 0);
        if (unit) movement = unit->def->movement;
    }
    int firstY = g_game->scrollY / 16;
    int firstX = g_game->scrollX / 16;
    // __min, not an if-clamp: one store of lastX after the select.
    int lastX = __min(g_game->screenTilesX + firstX + 1, g_game->width - 1);
    int lastY = g_game->height - 1;
    unsigned char* colors = g_game->colors;
    unsigned char arrowColor;
    for (int y = firstY; y < lastY; ++y) {
        int offscreen = 1;
        Point_00417f60 p[4];
        unsigned char heights[4];
        for (int x = firstX; x < lastX; ++x) {
            // The four corners, walking round the cell from its own tile.
            Tile* tile = &g_game->tiles[y * g_game->width + x];
            heights[0] = tile->height;
            p[0].x = (x + 8) * 16 - g_game->scrollX;
            p[0].y = (y + 2) * 16 - (heights[0] >> 1) - g_game->scrollY;
            ++tile; ++x;
            heights[1] = tile->height;
            p[1].x = (x + 8) * 16 - g_game->scrollX;
            p[1].y = (y + 2) * 16 - (heights[1] >> 1) - g_game->scrollY;
            tile += g_game->width; ++y;
            heights[2] = tile->height;
            p[2].x = (x + 8) * 16 - g_game->scrollX;
            p[2].y = (y + 2) * 16 - (heights[2] >> 1) - g_game->scrollY;
            --tile; --x;
            heights[3] = tile->height;
            p[3].x = (x + 8) * 16 - g_game->scrollX;
            p[3].y = (y + 2) * 16 - (heights[3] >> 1) - g_game->scrollY;
            tile -= g_game->width; --y;
            if (p[0].y < g_game->bottom) offscreen = 0;
            if (g_game->mode == 1) {
                if (movement) {
                    unsigned int state = (movement->states[movement->width * (y >> 4) + x] >> ((y & 15) * 2)) & 3;
                    if (state < 3) {
                        unsigned char color = colors[g_movementClassColors[state]];
                        DrawLine(surface, p[0].x, p[0].y, p[2].x, p[2].y, color);
                        DrawLine(surface, p[1].x, p[1].y, p[3].x, p[3].y, color);
                    }
                }
                PathCell* cell = g_game->paths->grid.At(x, y);
                if (cell->flags & 4) {
                    SetFont(g_game->font);
                    SetTextColors(rand() & 255, GetTextKeyColor());
                    DrawString(surface, "G", p[0].x, p[0].y, -1);
                }
                // unsigned char: gives the byte compares.
                unsigned char kind = cell->flags & ~4;
                if (kind != 0 && kind != 3) {
                    int cx = p[0].x + 8, cy = p[0].y + 8;
                    // Flags 5 and 6 pass the test above but set no colour, so
                    // the arrow keeps the colour of the last arrow drawn.
                    switch (cell->flags) {
                    case 1: arrowColor = colors[15]; break;
                    case 2: arrowColor = colors[4]; break;
                    }
                    DrawLine(surface, cx - DAT_004fd670[cell->direction] * 14,
                                 cy - DAT_004fd678[cell->direction] * 14, cx, cy, arrowColor);
                    int direction = (cell->direction + 1) & 7;
                    DrawLine(surface, cx - DAT_004fd670[direction] * 4,
                                 cy - DAT_004fd678[direction] * 4, cx, cy, arrowColor);
                    direction = (cell->direction - 1) & 7;
                    DrawLine(surface, cx - DAT_004fd670[direction] * 4,
                                 cy - DAT_004fd678[direction] * 4, cx, cy, arrowColor);
                }
            } else if (g_game->mode == 2) {
                if (tile->height > g_game->seaLevel) {
                    DrawLine(surface, p[0].x, p[0].y, p[1].x, p[1].y, colors[15]);
                    DrawLine(surface, p[0].x, p[0].y, p[3].x, p[3].y, colors[15]);
                } else {
                    DrawLine(surface, p[0].x, p[0].y, p[1].x, p[1].y, colors[13]);
                    DrawLine(surface, p[0].x, p[0].y, p[3].x, p[3].y, colors[13]);
                }
                // Masked with & 0xff, not cast: the loaded word is reused for the argument.
                if (tile->unit) FillPolygon(surface, p, 4, tile->unit & 0xff);
                else if (tile->object != 0xffff) FillPolygon(surface, p, 4, (tile->object - 56) & 0xff);
                if (tile->feature) {
                    DrawLine(surface, p[0].x, p[0].y, p[2].x, p[2].y, tile->feature & 0xff);
                    DrawLine(surface, p[1].x, p[1].y, p[3].x, p[3].y, tile->feature & 0xff);
                }
                if (tile->flags & 2) {
                    DrawLine(surface, (p[0].x + p[1].x) / 2, (p[0].y + p[1].y) / 2 + 2,
                                 (p[1].x + p[2].x) / 2 - 2, (p[1].y + p[2].y) / 2, colors[15]);
                    DrawLine(surface, (p[1].x + p[2].x) / 2 - 2, (p[1].y + p[2].y) / 2,
                                 (p[2].x + p[3].x) / 2, (p[2].y + p[3].y) / 2 - 2, colors[15]);
                    DrawLine(surface, (p[2].x + p[3].x) / 2, (p[2].y + p[3].y) / 2 - 2,
                                 (p[3].x + p[0].x) / 2 + 2, (p[3].y + p[0].y) / 2, colors[15]);
                    DrawLine(surface, (p[3].x + p[0].x) / 2 + 2, (p[3].y + p[0].y) / 2,
                                 (p[0].x + p[1].x) / 2, (p[0].y + p[1].y) / 2 + 2, colors[15]);
                }
            } else if (g_game->mode == 3) {
                if (tile->height > g_game->seaLevel) {
                    DrawLine(surface, p[0].x, p[0].y, p[1].x, p[1].y, colors[15]);
                    DrawLine(surface, p[0].x, p[0].y, p[3].x, p[3].y, colors[15]);
                } else {
                    DrawLine(surface, p[0].x, p[0].y, p[1].x, p[1].y, colors[13]);
                    DrawLine(surface, p[0].x, p[0].y, p[3].x, p[3].y, colors[13]);
                }
                SetFont(g_game->font);
                SetTextColors(colors[15], GetTextKeyColor());
                char buffer[20];
                DrawString(surface, _itoa(tile->metal, buffer, 10), p[0].x + 2, p[0].y + 2, -1);
            } else if (g_game->mode == 4) {
                DrawLine(surface, p[0].x, p[0].y, p[1].x, p[1].y, colors[0]);
                DrawLine(surface, p[0].x, p[0].y, p[3].x, p[3].y, colors[0]);
                if (player->fog[(y / 2) * player->fogWidth + x / 2]) {
                    Rect r;
                    r.left = p[0].x - 5;
                    r.right = p[0].x + 5;
                    r.top = p[0].y - 5;
                    r.bottom = p[0].y + 5;
                    FillRectangle(surface, &r, colors[15]);
                }
            }
            if (g_contourSpacing) DrawCellContours(surface, p, heights);
        }
        if (offscreen) break;
    }
}

// Console command: writes a player's save file. Only runs with three
// arguments, needs a valid active player slot, and dumps it through
// DumpPlayerAI into the file named by argument 2.
// FUNCTION: 0x418bb0
void __stdcall CmdPrintWeights(CommandArgs* args)
{
    if (args->count == 3) {
        unsigned char i = args->GetIntArg(1, 0);
        if (i < 10) {
            Player* p = &g_game->players[i];
            if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
                && p->field_146 != 10) {
                FILE* f = fopen(((CommandArgs*)args)->GetArg(2, DAT_005119b8), "w+b");
                if (f != 0) {
                    DumpPlayerAI(args->GetIntArg(1, 0), f);
                    fclose(f);
                }
            }
        }
    }
}

// Console command: with one argument, stores it (0..100, else 0) in g_game.
// FUNCTION: 0x418c70
void __stdcall CmdSenderror(CommandArgs* args)
{
    if (args->count == 2) {
        int value = args->GetIntArg(1, 0);
        if (value < 0 || value > 100)
            value = 0;
        g_game->field_37f35 = value;
    }
}

// Toggles bit 10 of the game flags word at +0x37f2f (0x4177a0 toggles bit 0).
// FUNCTION: 0x418ca0
void __stdcall CmdShootAll(int unused)
{
    g_game->flag10 = !g_game->flag10;
}

// The commands anyone may type (mask 1).
// GLOBAL: 0x501d38
ConsoleCommand g_consoleCommands[44] = {
    {"NoShake", CmdNoShake, 1},
    {"Contour", CmdContour, 1},
    {"ScrollSpeed", CmdScrollSpeed, 1},
    {"IFace", CmdIFace, 1},
    {"Give", CmdGive, 1},
    {"CDPlay", CmdCDPlay, 1},
    {"CDStop", CmdCDStop, 1},
    {"Sound3D", CmdSound3D, 1},
    {"Shading", CmdShading, 1},
    {"AntiAlias", CmdAntiAlias, 1},
    {"Shadow", CmdShadow, 1},
    {"Dither", CmdDither, 1},
    {"SwitchAlt", CmdSwitchAlt, 1},
    {"TShadow", CmdTShadow, 1},
    {"FShadow", CmdFShadow, 1},
    {"LOSType", CmdLOSType, 1},
    {"Light", CmdLight, 1},
    {"RCache", CmdRCache, 1},
    {"Selectable", CmdSelectable, 1},
    {"MusicMode", CmdMusicMode, 1},
    {"Logo", CmdLogo, 1},
    {"ScreenChat", CmdScreenChat, 1},
    {"Gamma", CmdGamma, 1},
    {"Clock", CmdClock, 1},
    {"NetStats", CmdNetStats, 1},
    {"Sing", CmdSing, 1},
    {"NoMetal", CmdNoMetal, 1},
    {"NoEnergy", CmdNoEnergy, 1},
    {"BigBrother", CmdBigBrother, 1},
    {"Now", CmdNow, 1},
    {"Drop", CmdDrop, 1},
    {"ShootAll", CmdShootAll, 1},
    {"ShareMetal", CmdShareMetal, 1},
    {"ShareEnergy", CmdShareEnergy, 1},
    {"ShareMapping", CmdShareMapping, 1},
    {"ShareRadar", CmdShareRadar, 1},
    {"ShareAll", CmdShareAll, 1},
    {"ShowRanges", CmdShowRanges, 1},
    {"SetShareMetal", CmdSetShareMetal, 1},
    {"SetShareEnergy", CmdSetShareEnergy, 1},
    {"Compression", CmdCompression, 1},
    {"BPS", CmdBPS, 1},
    {"SFX", CmdSFX, 1},
    {0, 0, 0},
};

// The cheats (mask 2).
// GLOBAL: 0x501f48
ConsoleCommand g_cheatCommands[11] = {
    {"Radar", CmdRadar, 2},
    {"ATM", CmdATM, 2},
    {"View", CmdView, 2},
    {"LOS", CmdLOS, 2},
    {"Mapping", CmdMapping, 2},
    {"DoubleShot", CmdDoubleShot, 2},
    {"HalfShot", CmdHalfShot, 2},
    {"NowISee", CmdNowISee, 2},
    {"Meteor", CmdMeteor, 2},
    {"MakePoster", CmdMakePoster, 2},
    {0, 0, 0},
};

// GLOBAL: 0x501fcc
int DAT_00501fcc = 0;

// The debug commands (mask 4).
// GLOBAL: 0x501fd0
ConsoleCommand g_debugCommands[31] = {
    {"AI", CmdAI, 4},
    {"Control", CmdControl, 4},
    {"Kill", CmdKill, 4},
    {"IWin", CmdIWin, 4},
    {"ILose", CmdILose, 4},
    {"Film", CmdFilm, 4},
    {"FilmSpeed", CmdFilmSpeed, 4},
    {"Assert", CmdAssert, 4},
    {"Assign", CmdAssign, 4},
    {"BurnAll", CmdBurnAll, 4},
    {"BurnOne", CmdBurnOne, 4},
    {"DebugBreak", CmdDebugBreak, 4},
    {"DPrint", CmdDPrint, 4},
    {"Edge", CmdEdge, 4},
    {"Include", CmdInclude, 4},
    {"Mem", CmdMem, 4},
    {"MemDump", CmdMemDump, 4},
    {"Move", CmdMove, 4},
    {"PrintWeights", CmdPrintWeights, 4},
    {"Profile", CmdProfile, 4},
    {"Reload", CmdReload, 4},
    {"ReloadAIProfiles", CmdReloadAIProfiles, 4},
    {"Save", CmdSave, 4},
    {"SeaLevel", CmdSeaLevel, 4},
    {"Search", CmdSearch, 4},
    {"SelBoxes", CmdSelBoxes, 4},
    {"Senderror", CmdSenderror, 4},
    {"TreeDeath", CmdTreeDeath, 4},
    {"Feature", CmdFeature, 4},
    {"ZBuffer", CmdZBuffer, 4},
    {0, 0, 0},
};
