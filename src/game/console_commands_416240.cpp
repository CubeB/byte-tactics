// Decompiled by Opus, Haiku, Sonnet, DeepSeek V4.1 Flash. Names are provisional.
// Console commands: the memory commands, the AI and assign commands, the burn
// commands, the rendering option toggles, the light and cache commands, the CD
// player commands and the move command. The files of the module's first part
// (0x416240 to 0x416860) gathered in address order.
#include <windows.h>
#include <string.h>
#include <stdlib.h>

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

struct Player {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];

    void SetType(int type);
};

struct Unit {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

class CMemoryCache {
public:
    void FlushCache();
};

class MissionConditions {
public:
    void FUN_004904b0();
};

struct Obj_00416780 {
    char unknown_0[0x48];
    int value;                         // +0x48
    char unknown_4c[0x54 - 0x4c];
    int fixed;                         // +0x54
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
    char unknown_14[0x1b63 - 0x14];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char field_2a42;          // +0x2a42
    char unknown_2a43[0x2c8e - 0x2a43];
    Point point;                       // +0x2c8e
    char unknown_2c92[0x2caa - 0x2c92];
    Vec3 pos;                          // +0x2caa
    char unknown_2cb6[0x2cbc - 0x2cb6];
    unsigned short field_2cbc;         // +0x2cbc
    char unknown_2cbe[0x14207 - 0x2cbe];
    Obj_00416780* field_14207;         // +0x14207
    char unknown_1420b[0x14223 - 0x1420b];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    int offsetX;                       // +0x1422b
    int offsetY;                       // +0x1422f
    char unknown_14233[0x14281 - 0x14233];
    ViewFlags viewFlags;               // +0x14281
    char unknown_14283[0x14357 - 0x14283];
    Unit* units;                       // +0x14357
    Unit* units_end;                   // +0x1435b
    char unknown_1435f[0x1437b - 0x1435f];
    CMemoryCache* obj;                 // +0x1437b
    char unknown_1437f[0x37f06 - 0x1437f];
    GameFlags flags;                   // +0x37f06
    char unknown_37f08[0x391ed - 0x37f08];
    MissionConditions* field_391ed;    // +0x391ed
};

#pragma pack(pop)

// Command arguments.
class CommandArgs {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0

    int GetIntArg(int index, int fallback);
    char* GetArg(int index, char* fallback);
};

class Sound {
public:
    void PlayCdTrack(int index, int flag);
    int Is3DEnabled();
    void Disable3D();
    void Enable3D();
};

class Class_004ced40 {
public:
    void StopCdAudio();
};

// GLOBAL: 0x511de8
extern Game* g_game;
extern char DAT_005119b8[];

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
extern void __stdcall RecalculateLineOfSight(int flag);
void __stdcall SetLightVector(int param_1, int param_2, int param_3);

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
    unsigned char i = args->GetIntArg(1, g_game->field_2a42);
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
    if (g_game->field_2cbc < 0xfffb) {
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
    g_game->field_391ed->FUN_004904b0();
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
    g_game->offsetX = g_game->baseX - args->GetIntArg(1, 0x20);
    g_game->offsetY = g_game->baseY - args->GetIntArg(2, 0x80);
}

// Console command: sets two fields of a game sub-object, the second from a
// 16.16 fixed-point number.
// FUNCTION: 0x416780
void __stdcall CmdSearch(CommandArgs* args)
{
    if (args->GetIntArg(1, 0)) {
        g_game->field_14207->value = args->GetIntArg(1, 0);
    }
    if (args->count == 3) {
        g_game->field_14207->fixed = (int)(atof(((CommandArgs*)args)->GetArg(2, DAT_005119b8)) * 65536.0);
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
