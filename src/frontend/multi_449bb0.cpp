// Decompiled by deepseek-v4.1-flash, finished by claude-opus-5-5, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Opens the multiplayer battle room (LOUNGE2.GUI): resets the room state,
// copies the lobby's command-line options (g_lobbyLock..g_lobbyWatching) or the
// host's game options into the local player's flags, sets up the chat list,
// MEM, START, the METAL/MAXUNITS/ENERGY sliders and the map, then refreshes
// the whole room.
//
// The flag word at +0x9b is the bitfield layout of 0x447b10; the game's
// mapping/los/losType/commander and the options' fixedloc are ints read for
// their low bit. f97 (host) and the word at +0x9d are unsigned short
// bitfields, as in 0x44a680.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// First: gives g_game a symbol id below 65536.
#include "ta_types.h"

#pragma pack(push, 1)
struct PlayerInfo_00449bb0 {
    char map[0x8b];                     // +0x00
    unsigned short width;               // +0x8b
    unsigned short height;              // +0x8d
    char unknown_8f[0x97 - 0x8f];
    unsigned short f97_0 : 1;           // +0x97, host
    unsigned short f97_rest : 15;
    unsigned short memory;              // +0x99
    struct {
        unsigned short low : 4;         // +0x9b
        unsigned short started : 1;
        unsigned short ready : 1;
        unsigned short bit6 : 1;
        unsigned short watching : 1;
        unsigned short mapping : 1;
        unsigned short los : 1;
        unsigned short losType : 1;
        unsigned short commander : 2;
        // Unsigned short, not signed: the writes compute the value before masking.
        unsigned short cheating : 1;
        unsigned short fixedloc : 1;
        unsigned short closed : 1;
    } b;
    unsigned short f9d_0 : 2;           // +0x9d
    unsigned short f9d_2 : 1;
    unsigned short f9d_rest : 13;
    char unknown_9f[0xa1 - 0x9f];
    unsigned short energy;              // +0xa1
    unsigned short metal;               // +0xa3
    unsigned short maxUnits;            // +0xa5
    unsigned char versionMajor;         // +0xa7
    unsigned char versionMinor;         // +0xa8
    int mapCrc;                         // +0xa9
};

struct Player_00449bb0 {                // 0x14b bytes
    int active;                         // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00449bb0* info;          // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Gadget_00449bb0;
struct Gui_00449bb0;
typedef void (__stdcall* Callback_00449bb0)(Gui_00449bb0* gui, int index);

struct Gadget_00449bb0 {                // 0x15b bytes
    char unknown_0[0x1b];
    int field_1b;                       // +0x1b
    char unknown_1f[0x23 - 0x1f];
    int colour;                         // +0x23
    char unknown_27[0x29 - 0x27];
    unsigned char visible;              // +0x29
    char unknown_2a[0xb6 - 0x2a];
    union {
        char text[0x10];                // +0xb6
        struct {                        // the layer's first entry
            short count;                // +0xb6
            char unknown_b8[8];
            void* gaf;                  // +0xc0
        } head;
        struct {
            char unknown_b6[8];
            void* frames;               // +0xbe
        } anim;
    };
    short frame;                        // +0xc6
    unsigned int c8_0 : 1;              // +0xc8
    unsigned int c8_rest : 31;
    char unknown_cc[0x138 - 0xcc];
    unsigned short field_138;           // +0x138
    char unknown_13a[0x13c - 0x13a];
    int max;                            // +0x13c
    short value;                        // +0x140
    short knobSize;
    Callback_00449bb0 callback;         // +0x144
    char unknown_148[2];
    void* game;                         // +0x14a
    char unknown_14e[0x15b - 0x14e];
};

struct Layer_00449bb0 {
    int unknown_0;
    Gadget_00449bb0* entries;           // +0x4
    void* handler;                      // +0x8
    void* game;                         // +0xc
};

struct Gui_00449bb0 {
    char unknown_0[0x18];
    Layer_00449bb0* table;              // +0x18
};

struct Options_00449bb0 {
    char unknown_0[0x118];
    int fixedloc;                       // +0x118
};

struct Game_00449bb0 {
    char unknown_0[0x519];
    Gui_00449bb0 gui;                   // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_00449bb0 players[10];        // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Options_00449bb0* options;          // +0x29a0
    char unknown_29a4[0x2a30 - 0x29a4];
    void* net;                          // +0x2a30
    char unknown_2a34[0x2a42 - 0x2a34];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x2a9b - 0x2a43];
    char* chatter;                      // +0x2a9b
    char unknown_2a9f[0x2bee - 0x2a9f];
    unsigned short dirty : 1;           // +0x2bee
    unsigned short dirty_rest : 15;
    char unknown_2bf0[0x2c28 - 0x2bf0];
    int playerIds[11];                  // +0x2c28
    char unknown_2c54[0x2c74 - 0x2c54];
    unsigned short locked : 1;          // +0x2c74
    unsigned short locked_rest : 15;
    char unknown_2c76[0x37ee8 - 0x2c76];
    unsigned short field_37ee8;         // +0x37ee8
    unsigned short maxUnits;            // +0x37eea
    unsigned short field_37eec;         // +0x37eec
    char unknown_37eee[0x37ef6 - 0x37eee];
    int field_37ef6;                    // +0x37ef6
    char unknown_37efa[0x37f1b - 0x37efa];
    unsigned short width;               // +0x37f1b
    char unknown_37f1d[2];
    unsigned short height;              // +0x37f1f
    char unknown_37f21[0x391e9 - 0x37f21];
    void* map;                          // +0x391e9
    char unknown_391ed[0x39229 - 0x391ed];
    int commander;                      // +0x39229
    int mapping;                        // +0x3922d
    int los;                            // +0x39231
    int losType;                        // +0x39235
};
#pragma pack(pop)

class Class_00435a20 { public: int LoadMissionByName(char* map); };
class Class_00435c30 { public: char* GetMissionName(); };

extern Game_00449bb0* g_game;
extern int g_battleRoomSlotsBuilt;
extern unsigned int g_lastPlayerCount;
extern int g_battleRoomBaseGadgetCount;
extern int g_lobbyLock;
extern int g_lobbyMaxUnits;
extern int g_lobbyEnergy;
extern int g_lobbyMetal;
extern int g_lobbyCommander;
extern int g_lobbyLineOfSight;
extern int g_lobbyCheating;
extern int g_lobbyFixedLoc;
extern int g_lobbyMapping;
extern int g_lobbyWatching;
extern char g_lobbyMapName[];
extern char* g_hostOnlyGadgets[];
extern char* g_battleRoomGadgetNames[];
// After the externs: keeps the locals' symbol ids past 65536 (g_game stays below).
#include "ta_protos.h"

char __stdcall FindGameCdDrive(int side);
int __stdcall LoadPictureCached(const char* name, int param_2, int param_3, int param_4);
void OrLabelAttribs();
void __stdcall UpdateMaxUnitsText(Gui_00449bb0* gui, int index);
void __stdcall UpdateMetalText(Gui_00449bb0* gui, int index);
void UpdateBattleRoomFlags();
void RefreshTeamIcons();
void RefreshBattleRoomRows();
void BroadcastPlayerInfo();
void UpdateNetGameInfo();
int AreAllPlayersReady();
int IsHostLocal();
int IsOnlineConfigLoaded();
void __stdcall SetSliderFromValue(Gadget_00449bb0* gadget, int value);
int __stdcall ReadSliderValue(Gadget_00449bb0* gadget);
void __stdcall CreateUnitSync(int param_1);
void __stdcall MarkChanged(Gui_00449bb0* gui);
void __stdcall SetKeyboardInput(Gui_00449bb0* gui, int value);
int __stdcall FindGadgetIndex(Gadget_00449bb0* entries, char* name, int type);
Gadget_00449bb0* __stdcall FindGadgetChecked(Gadget_00449bb0* entries, char* name);
Gadget_00449bb0* __stdcall FUN_004a0180(Gadget_00449bb0* entries, char* name);
Gadget_00449bb0* __stdcall FUN_004a0200(Gadget_00449bb0* entries, char* name);
Gadget_00449bb0* __stdcall FUN_004a0280(Gadget_00449bb0* entries, char* name);
void __stdcall SetGadgetActiveByName(Gui_00449bb0* gui, char* name, int value);
void __stdcall SetTranslatedTextByName(Gui_00449bb0* gui, char* name, char* text, int size);
void __stdcall SetGrayedOutByName(Gui_00449bb0* gui, char* name, int value);
void __stdcall SetGadgetGrayedOutByName(Gui_00449bb0* gui, char* name, int value);
void __stdcall ConfigureListBoxByName(Gui_00449bb0* gui, char* name, char* text, int count, int flag);
void __stdcall BeginTextEdit(Gui_00449bb0* gui, int index);
void __stdcall RenderLayer(Gui_00449bb0* gui, int value);
Layer_00449bb0* __stdcall LoadGuiLayer(Gui_00449bb0* gui, const char* name, int size);
int __stdcall IsScreenNamed(Gui_00449bb0* gui, char* name);
void __stdcall FatalError(char* message);
void* __stdcall FindGafEntry(void* gaf, const char* name);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);

// The ENERGY slider handler at 0x445d60, which has no callers: /Ob2 inlined it.
void __stdcall UpdateEnergyText(Gui_00449bb0* gui, int unused)
{
    char text[20];
    Gadget_00449bb0* value = FUN_004a0200(gui->table->entries, "ENERGY");

    if (value != 0) {
        int shown = ReadSliderValue(value) / 100 * 100;
        PlayerInfo_00449bb0* info;

        _itoa(shown, text, 10);
        SetTranslatedTextByName(gui, "ENERGYTEXT", text, 0);
        info = g_game->players[g_game->localPlayer].info;
        info->energy = (unsigned short)(shown / 100);
        if (info->f97_0 & 1) {
            BroadcastPlayerInfo();
            UpdateNetGameInfo();
        }
    }
}

// The slider set-up at 0x445e50, which has no callers: /Ob2 inlined it.
void __stdcall BindNamedSliderWithCallback(char* name, int max, int value, Callback_00449bb0 callback)
{
    Gui_00449bb0* gui = &g_game->gui;
    Gadget_00449bb0* gadgets = gui->table->entries;
    int index = FindGadgetIndex(gadgets, name, 0xe);
    if (index != -1) {
        Gadget_00449bb0* gadget = FUN_004a0200(gadgets, name);
        gadget->max = max;
        gadget->callback = callback;
        gadget->value = value;
        SetSliderFromValue(gadget, gadget->value);
        gadget->game = g_game;
    }
    callback(gui, index);
    MarkChanged(gui);
}

// FUNCTION: 0x449bb0
void OpenBattleRoom()
{
    int energy = 1000;
    int metal = 1000;
    Layer_00449bb0* layer;
    Player_00449bb0* player;
    PlayerInfo_00449bb0* info;
    Gadget_00449bb0* entries;
    short host;
    short isHost;
    int i;

    g_battleRoomSlotsBuilt = 0;
    g_lastPlayerCount = -1;
    g_game->dirty = 1;
    memset(g_game->playerIds, 0, sizeof(g_game->playerIds));
    // info before player: stores the player pointer without its 0x1b63 bias.
    info = g_game->players[g_game->localPlayer].info;
    player = &g_game->players[g_game->localPlayer];
    host = info->f97_0;
    g_game->field_37ee8 = 0;
    if (host)
        g_game->maxUnits = g_game->field_37eec;
    info->width = g_game->width;
    info->height = g_game->height;
    info->f9d_2 = FindGameCdDrive(1) != 0;

    layer = LoadGuiLayer(&g_game->gui, "LOUNGE2.GUI", 0);
    layer->handler = HandleBattleRoomClick;
    layer->game = g_game;
    LoadPictureCached("battleroom", 0, 1, 0);
    entries = g_game->gui.table->entries;
    g_battleRoomBaseGadgetCount = layer->entries->head.count;

    i = FindGadgetIndex(entries, "MESSAGE", 3);
    if (i != -1)
        entries[i].field_138 = 0x7f;
    if (!IsHostLocal()) {
        i = FindGadgetIndex(entries, "MAP", 1);
        if (i != -1) {
            entries[i].field_1b = 2;
            SetTranslatedTextByName(&g_game->gui, "MAP", "View Map", 0);
        }
    }

    if (IsOnlineConfigLoaded()) {
        if (g_lobbyMaxUnits) {
            info->maxUnits = g_lobbyMaxUnits;
            g_game->maxUnits = g_lobbyMaxUnits;
        }
        g_game->locked = g_lobbyLock != 0;
        if (g_lobbyCommander) {
            int commander = g_lobbyCommander - 1;
            if (commander >= 0 && commander <= 2) {
                g_game->commander = commander;
                g_game->field_37ef6 = commander;
                // Block-local pointer: the original forms 0x1b63 first, then reads +0x27.
                Player_00449bb0* p = &g_game->players[g_game->localPlayer];
                p->info->b.commander = commander;
            }
        }
        if (g_lobbyEnergy)
            energy = g_lobbyEnergy;
        if (g_lobbyMetal)
            metal = g_lobbyMetal;
        if (g_lobbyLineOfSight) {
            switch (g_lobbyLineOfSight) {
            case 1:
                info->b.los = 1;
                info->b.losType = 1;
                break;
            case 2:
                info->b.los = 1;
                info->b.losType = 0;
                break;
            case 3:
                info->b.los = 0;
                break;
            }
        }
        if (g_lobbyCheating)
            info->b.cheating = g_lobbyCheating == 2;
        if (g_lobbyFixedLoc)
            info->b.fixedloc = g_lobbyFixedLoc == 1;
        if (g_lobbyMapping)
            info->b.mapping = g_lobbyMapping == 1;
        if (g_lobbyWatching)
            info->b.watching = g_lobbyWatching == 2;
    } else if (host) {
        info->b.mapping = g_game->mapping;
        info->b.los = g_game->los;
        info->b.losType = g_game->losType;
        info->b.fixedloc = g_game->options->fixedloc;
        info->b.commander = g_game->commander;
    }

    if (!host || g_game->locked) {
        for (char** p = g_hostOnlyGadgets; *p; p++)
            SetGrayedOutByName(&g_game->gui, *p, 1);
    }

    UpdateBattleRoomFlags();
    g_game->chatter = (char*)FUN_004d83b0("LOUNGE CHATTER", 0xa00);
    *g_game->chatter = 0;
    {
        Gadget_00449bb0* mem = FUN_004a0180(g_game->gui.table->entries, "MEMx");
        mem->colour = player->info->memory < ((Class_00435920*)g_game->map)->GetTerrainSizeTier() ? 0xc : 0;
        sprintf(mem->text, "%d", g_game->players[g_game->localPlayer].info->memory);
    }
    isHost = g_game->players[g_game->localPlayer].info->f97_0;
    CreateUnitSync(isHost);
    SetGadgetActiveByName(&g_game->gui, "START", ((UnitSync*)g_game->net)->AllPlayersSynced());
    SetGrayedOutByName(&g_game->gui, "START",
                 host && AreAllPlayersReady() && ((UnitSync*)g_game->net)->AllPlayersSynced() ? 0 : 1);
    SetGrayedOutByName(&g_game->gui, "RESTRICTIONS", 0);
    ConfigureListBoxByName(&g_game->gui, "OUTPUT", g_game->chatter, 0, 0);
    {
        Gadget_00449bb0* output = FindGadgetChecked(entries, "OUTPUT");
        output->field_1b |= 0x100;
    }
    SetTranslatedTextByName(&g_game->gui, "METALTEXT", "0", 0);
    SetTranslatedTextByName(&g_game->gui, "ENERGYTEXT", "0", 0);

    BindNamedSliderWithCallback("METAL", 0x2711, metal, UpdateMetalText);
    {
        // maxUnits - 20 twice, not a named local: one CSE in the slot `player` used.
        BindNamedSliderWithCallback("MAXUNITS", g_game->maxUnits - 20, g_game->maxUnits - 20, UpdateMaxUnitsText);
    }
    if (!isHost || g_game->locked) {
        SetGadgetGrayedOutByName(&g_game->gui, "MAXUNITS", 1);
        SetGadgetGrayedOutByName(&g_game->gui, "ENERGY", 1);
        SetGadgetGrayedOutByName(&g_game->gui, "METAL", 1);
    }
    BindNamedSliderWithCallback("ENERGY", 0x2711, energy, UpdateEnergyText);

    ((Class_00435d30*)g_game->map)->RefreshMapList(1);
    if (isHost && IsOnlineConfigLoaded() && g_lobbyMapName[0])
        ((Class_00435a20*)g_game->map)->LoadMissionByName(g_lobbyMapName);
    if (!((Class_00435c40*)g_game->map)->HasMissionName())
        FatalError("Could not find the multiplayer map!!");
    strcpy(info->map, ((Class_00435c30*)g_game->map)->GetMissionName());
    info->mapCrc = ((Class_004373a0*)g_game->map)->ComputeMapChecksum();
    BroadcastPlayerInfo();
    BeginTextEdit(&g_game->gui, FindGadgetIndex(g_game->gui.table->entries, "MESSAGE", 0xe));

    for (char** p = g_battleRoomGadgetNames; *p; p++) {
        int k = FindGadgetIndex(layer->entries, *p, 0xe);
        if (k != -1)
            layer->entries[k].visible = 0;
    }

    BuildPlayerSlotGadgets();
    RefreshTeamIcons();
    RefreshBattleRoomRows();
    OrLabelAttribs();

    if (IsScreenNamed(&g_game->gui, "LOUNGE2.GUI")) {
        Gadget_00449bb0* start = FUN_004a0280(layer->entries, "battlestart");
        start->anim.frames = FindGafEntry(layer->entries->head.gaf, "battlestart");
        start->frame = 0;
        start->c8_0 = 1;
        SetGadgetActiveByName(&g_game->gui, "battlestart", 1);
    }

    SetKeyboardInput(&g_game->gui, 1);
    RenderLayer(&g_game->gui, 0x40);
}
