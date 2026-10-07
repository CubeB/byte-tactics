// Decompiled by deepseek-v4.1-flash, Opus, Haiku, Sonnet and GPT-6 Astra. Names are provisional.
// Console commands: the logo, win/lose, sea level, control, view and give
// commands, the scroll speed, interface, line-of-sight, mapping and contour
// commands, the rendering option toggles and cheats, the ATM, screen chat,
// no-metal, no-energy, gamma, sing and clock commands, and the film commands.
// The files of the module's second part (0x4168d0 to 0x4173e0) gathered in
// address order.
#include <string.h>

#pragma pack(push, 1)

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
    char unknown_74[0x8c - 0x74];
    float field_8c;                    // +0x8c
    char unknown_90[0x98 - 0x90];
    float field_98;                    // +0x98
    char unknown_9c[0x146 - 0x9c];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

class CMemoryCache {
public:
    void FlushCache();
};

// The view flags word at +0x14281.
struct ViewFlags {
    unsigned short bit0 : 1;
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short rest : 13;
};

struct Game {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x1427f - 0x2a44];
    unsigned char field_1427f;         // +0x1427f
    char unknown_14280;
    ViewFlags viewFlags;               // +0x14281
    char unknown_14283[0x1434d - 0x14283];
    unsigned char field_1434d;         // +0x1434d
    char unknown_1434e[0x1437b - 0x1434e];
    CMemoryCache* obj;                 // +0x1437b
    char unknown_1437f[0x148db - 0x1437f];
    void* logos32;                     // +0x148db
    char unknown_148df[0x37efa - 0x148df];
    int field_37efa;                   // +0x37efa
    char unknown_37efe[0x37f02 - 0x37efe];
    int field_37f02;                   // +0x37f02
    char unknown_37f06[0x37f08 - 0x37f06];
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
            unsigned short rest_37f2f : 6;
        };
    };
    char unknown_37f31[0x38a53 - 0x37f31];
    // The film path buffer and the film speed value overlap.
    union {
        char path[0x20c];              // +0x38a53
        struct {
            char unknown_38a53[0x38c57 - 0x38a53];
            int field_38c57;           // +0x38c57
        };
    };
    int field_38c5f;                   // +0x38c5f
    int field_38c63;                   // +0x38c63
    char unknown_38c67[0x3923b - 0x38c67];
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
};

// GLOBAL: 0x511de8
extern Game* g_game;
extern char DAT_005119b8[];
extern int DAT_00511dd0;
extern int DAT_00511dd4;
extern int DAT_0051e698;

void __stdcall AddMessage(char* text, int param_2, int param_3, int param_4);
void __stdcall KillPlayerUnits(unsigned char player);
void __stdcall TransferEnergy(unsigned char a, int b, float c, int d);
void __stdcall TransferMetal(unsigned char a, int b, float c, int d);
void SaveSettings();
void __stdcall RecalculateLineOfSight(int flag);
void __stdcall SetBrightness(float value);

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
    g_game->field_1427f = args->GetIntArg(1, 0);
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
    g_game->field_1434d = result;
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
    DAT_00511dd0 = (int)(args->GetFloatArg(1, 0.0f) * 256.0f);
    DAT_00511dd4 = (int)(args->GetFloatArg(2, 0.75f) * 256.0f);
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
