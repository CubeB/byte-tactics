// Decompiled by Opus, Haiku, Sonnet, DeepSeek V4.1 Flash, GPT-6 Astra, Claude Opus 5.5, GPT-5.6 Astra, GPT-6.1-sol and deepseek-v4.1. Names are provisional.
// Console commands: the save and reload, AI profile, big brother, profile,
// now-I-see, net stats, music mode, poster, meteor, drop, include and crash
// commands, the command dispatcher, the world-to-screen conversions, the
// contour map, the debug overlay, the player dump, senderror and shootall.
// The files of the module's third part (0x417430 to 0x418ca0) gathered in
// address order.
#include <stdio.h>
// 0x417a60's two Win32 calls, as <windows.h> declares them.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

#pragma pack(push, 1)

// Command arguments.
class CommandArgs {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0

    char* GetArg(int index, char* fallback);
    int GetIntArg(int index, int fallback);
    int Tokenize(char* param_1, char* param_2);
    CommandArgs* InitArgs();
};

struct Vec3 {
    int x;
    int y;
    int z;
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
};

struct Player {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x7c - 0x74];
    unsigned char* fog;                // +0x7c
    int fogWidth;                      // +0x80
    char unknown_84[0x146 - 0x84];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
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
};

struct Game {
    char unknown_0[0xdcb];
    unsigned char colors[16];          // +0xdcb
    char unknown_ddb[0x1b63 - 0xddb];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a43 - (0x1b63 + 10 * 0x14b)];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x2caa - 0x2a44];
    Vec3 pos;                          // +0x2caa
    char unknown_2cb6[0x14207 - 0x2cb6];
    PathMap* paths;                    // +0x14207
    char unknown_1420b[0x1422b - 0x1420b];
    int mapWidth;                      // +0x1422b
    int mapHeight;                     // +0x1422f
    int width;                         // +0x14233
    int height;                        // +0x14237
    int screenTilesX;                  // +0x1423b
    int screenTilesY;                  // +0x1423f
    char unknown_14243[0x1427f - 0x14243];
    unsigned char seaLevel;            // +0x1427f
    unsigned char mode;                // +0x14280
    char unknown_14281[0x14287 - 0x14281];
    Tile* tiles;                       // +0x14287
    char unknown_1428b[0x1431f - 0x1428b];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x14371 - 0x14327];
    unsigned short field_14371;        // +0x14371
    unsigned int bit0 : 1;             // +0x14373
    unsigned int paused : 1;
    unsigned int rest_14373 : 30;
    char unknown_14377[0x1438f - 0x14377];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitDef* defs;                     // +0x1439b
    char unknown_1439f[0x37e23 - 0x1439f];
    int bottom;                        // +0x37e23
    char unknown_37e27[0x37f2f - 0x37e27];
    union {
        unsigned char flags_37f2f;     // +0x37f2f
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
    char installPath[1];               // +0x38a53
    char unknown_38a54[0x38dd5 - 0x38a54];
    int field_38dd5;                   // +0x38dd5
    char unknown_38dd9[0x391fd - 0x38dd9];
    int font;                          // +0x391fd
    char unknown_39201[0x3923b - 0x39201];
    unsigned char flags_3923b;         // +0x3923b
};

struct Pair_00419560 {
    int a;
    int b;
};

class Sound {
public:
    void SetTrackCategory(int);
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
    virtual void FUN_0044ef50(void* surface);  // slot 10
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

#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;
extern char DAT_005119b8[];
extern char DAT_00511bd0[];
extern int DAT_00511bc0;
extern int DAT_00511bc4;
extern int DAT_00511bc8;
extern int DAT_00511c20;
extern int DAT_00511c34;
extern int DAT_00511c48;
extern int DAT_00511c50;
extern Pair_00419560 DAT_00511a60[44];
extern Pair_00419560 DAT_00511c60[44];
extern int DAT_00511dd0;               // contour spacing
extern int DAT_00511dd4;               // contour offset
extern unsigned char DAT_00501d18[];   // colour by height band
extern unsigned char DAT_004fcc68[];
extern signed char DAT_004fd670[], DAT_004fd678[];

void __stdcall MakeDirectoryPath(char* dir);
void __stdcall SaveGameFile(char* path, char* description, int param_3);
short __stdcall FindUnitTypeId(char* name);
void __stdcall KillUnitsOfType(short id);
void __stdcall ReloadUnitType(unsigned short id);
extern void ResetAIPlayers();
extern void FUN_0041c390(void);
void __stdcall RecalculateLineOfSight(int flag);
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
void __stdcall FUN_0047ddc0(UnitDef* def, Vec3* pos);
void* __stdcall CreateUnit(unsigned char owner, short id, Vec3 pos, int param_4, int param_5, int param_6);
extern Display_00417a60* GetDisplay();
void ToggleFullScreen();
void __cdecl FUN_004d83b0(char* name, unsigned int size);
void __stdcall ExecuteCommand(void* param_1, int param_2);
int __stdcall GetGroundHeight(Pos_00417bb0* pos);
// The real callee takes unsigned char; int here reproduces the original's
// loop-invariant widening of the colour byte.
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, int color);
void __stdcall FUN_0046b9d0(void* a, short* b, int c, int d);
void __stdcall SetFont(int font);
int __stdcall GetTextKeyColor();
void __stdcall SetTextColors(int color, int background);
void __stdcall DrawString(void* surface, const char* text, int x, int y, int maxWidth);
void __stdcall FillPolygon(void* surface, Point_00417f60* points, int count, int color);
void __stdcall FillRectangle(void* surface, Rect* rect, int color);
void __stdcall FUN_004181d0(void* surface, Point_00417f60* corners, unsigned char* heights);
Unit* __stdcall FindNextSelectedUnit(int, int);
void __stdcall DumpPlayerAI(int player, FILE* file);

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
        FUN_0041c390();
    } else {
        g_game->paused = 1;
        g_game->field_14371 = 1;
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

// Same reset sequence as FUN_00419560, run as a callback after FUN_004161f0.
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
    sprintf(buf, "%s\\screenshots", g_game->installPath);
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
void __stdcall FUN_00417890(CommandArgs* args)
{
    Vec3 pos=g_game->pos;
    int count=0;
    for (unsigned short i=1;i<g_game->count;++i) {
        UnitDef* def=&g_game->defs[i];
        if (MatchWildcard(def->name,args->GetArg(0,DAT_005119b8))) {
            if (count) pos.x-=def->min.x;
            FUN_0047ddc0(def,&pos);
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
// command buffer DAT_00511bd0 instead. Compare 0x416780.
// FUNCTION: 0x417b50
void __stdcall FUN_00417b50(char* param_1, int param_2)
{
    char buf[0xd4];

    if (param_1 == 0)
        param_1 = DAT_00511bd0;
    else
        strncpy(DAT_00511bd0, param_1, 0x4f);

    ((CommandArgs*)buf)->InitArgs();
    ((CommandArgs*)buf)->Tokenize(param_1, 0);
    ExecuteCommand(buf, param_2);
}

// Converts a 16.16 fixed-point world position into screen coordinates.
// FUNCTION: 0x417bb0
void __stdcall FUN_00417bb0(Pos_00417bb0* pos, int* screen_x, int* screen_y)
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
void __stdcall FUN_00417c00(int x, int z, int* screen_x, int* screen_y)
{
    Pos_00417bb0 pos;
    *(int*)&pos.x_frac = x << 16;
    *(int*)&pos.z_frac = z << 16;
    WorldToScreen(&pos, screen_x, screen_y);
}

// Draws a line from a world position to the same position shifted by
// (dx, dz) whole map units, converting both ends to screen coordinates.
// FUNCTION: 0x417c70
void __stdcall FUN_00417c70(void* surface, Pos_00417bb0* p, short dx, short dz, int color)
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
void __stdcall FUN_00417d30(void* surface, Point16 from, short dx, short dz, int color)
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
void Class_0044f010::FUN_0044ef50(void* surface)
{
    FUN_0046b9d0(surface, (short*)(this->field_8 + 0x76), *(int*)(this->field_8 + 0x7e), 0xf);
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
void __stdcall FUN_00417f30(int param_1, Iface_00417f30*** param_2)
{
    if (param_2 && *param_2 && **param_2) {
        (**param_2)->Method_28(param_1);
    }
}

// Draws the contour lines that cross one triangle of the height map. Each
// corner is a screen point and a height (8 fraction bits). The corners are
// sorted by height, then every contour level between the lowest and highest
// corner (the multiples of DAT_00511dd0, offset by DAT_00511dd4) is drawn as
// one line across the triangle: first where it crosses the long edge 1-3 and
// the upper edge 2-3, then the long edge and the lower edge 1-2. The colour
// comes from the level's height above sea level through DAT_00501d18.
// FUNCTION: 0x417f60
void __stdcall FUN_00417f60(void* surface, Point_00417f60 p1, int z1,
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
    int level = z3 / DAT_00511dd0 * DAT_00511dd0 + DAT_00511dd4;
    while (level > z3) {
        level -= DAT_00511dd0;
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
                         DAT_00501d18[((level >> 8) - g_game->seaLevel + 0x100) >> 4]);
            level -= DAT_00511dd0;
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
                         DAT_00501d18[((level >> 8) - g_game->seaLevel + 0x100) >> 4]);
            level -= DAT_00511dd0;
        } while (level > z1);
    }
}

// Draws one map cell: four triangles fanning from the cell's centre, whose
// position and height are the rounded averages of the four corners. The
// corner heights are bytes, scaled to 8 fraction bits.
// FUNCTION: 0x4181d0
void __stdcall FUN_004181d0(void* surface, Point_00417f60* corners, unsigned char* heights)
{
    Point_00417f60 mid;
    mid.x = (corners[0].x + corners[1].x + corners[2].x + corners[3].x + 2) / 4;
    mid.y = (corners[0].y + corners[1].y + corners[2].y + corners[3].y + 2) / 4;
    int midHeight = (heights[0] + heights[1] + heights[2] + heights[3]) * 64;
    // "* 256", not "<< 8": gives mov cl,[mem]; shl ecx,8 instead of mov ch.
    FUN_00417f60(surface, corners[0], heights[0] * 256, corners[1], heights[1] * 256, mid, midHeight);
    FUN_00417f60(surface, corners[1], heights[1] * 256, corners[2], heights[2] * 256, mid, midHeight);
    FUN_00417f60(surface, corners[2], heights[2] * 256, corners[3], heights[3] * 256, mid, midHeight);
    FUN_00417f60(surface, corners[3], heights[3] * 256, corners[0], heights[0] * 256, mid, midHeight);
}

// Nothing here uses <ctype.h>: its symbols put 0x418310 in the symbol-id
// window it matches in (docs/c2-regalloc.md). Not at the top: there its
// symbols move 0x4181d0 out of its own window.
#include <ctype.h>

// Debug overlay for the map: walks the visible tiles, works out the screen
// quad of each one from its four corner heights and draws what the current
// debug mode asks for (1: movement classes and path arrows, 2: tile
// contents, 3: metal values, 4: fog of war), plus the contour lines of
// 0x4181d0 when DAT_00511dd0 is set.
//
// The header set fixes the operand order of the tile and fog multiplies.
// FUNCTION: 0x418310
void __stdcall FUN_00418310(void* surface)
{
    if (!g_game->mode && !DAT_00511dd0) return;
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
                        unsigned char color = colors[DAT_004fcc68[state]];
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
            if (DAT_00511dd0) FUN_004181d0(surface, p, heights);
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
