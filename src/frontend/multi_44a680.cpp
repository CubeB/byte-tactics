// Decompiled by deepseek-v4.1-flash, finished by claude-opus-5-5. Names are provisional.
// Per-frame update of the multiplayer battle room (LOUNGE2.GUI): checks the
// map, compacts the player slots when the room is dirty, copies the host's
// MAXUNITS/METAL/ENERGY sliders (or map) to a client, runs the host's start
// countdown, draws each player's version under their logo, and refreshes the
// local player's bit at +0x9d once a minute.
// RECT comes from windows.h: the rect sums follow the symbol count.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
struct PlayerInfo_0044a680 {
    char map[0x97];                     // +0x00
    unsigned char flags;                // +0x97, bit 0: host
    char unknown_98[0x9d - 0x98];
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

struct Player_0044a680 {                // 0x14b bytes
    int active;                         // +0x00
    char unknown_4[0x22 - 0x4];
    unsigned char field_22;             // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerInfo_0044a680* info;          // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                 // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;            // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Gadget_0044a680 {                // 0x15b bytes
    char unknown_0[0x1f];
    unsigned int colour;                // +0x1f
    char unknown_23[0xbe - 0x23];
    unsigned short* frames;             // +0xbe, the frame count first
    char unknown_c2[0xc6 - 0xc2];
    short frame;                        // +0xc6
    unsigned int c8_0 : 1;              // +0xc8
    unsigned int c8_rest : 31;
    char unknown_cc[0x15b - 0xcc];
};

struct Layer_0044a680 {
    int unknown_0;
    Gadget_0044a680* entries;           // +0x4
};

struct Gui_0044a680 {
    char unknown_0[0x18];
    Layer_0044a680* table;              // +0x18
};

struct Game {
    char unknown_0[0x519];
    Gui_0044a680 gui;                   // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_0044a680 players[10];        // +0x1b63
    char unknown_2851[0x2a30 - 0x2851];
    void* net;                          // +0x2a30
    char unknown_2a34[0x2a3c - 0x2a34];
    unsigned short field_2a3c;          // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x2bc0 - 0x2a43];
    unsigned char field_2bc0;           // +0x2bc0
    char unknown_2bc1[0x2bee - 0x2bc1];
    unsigned short dirty : 1;           // +0x2bee
    unsigned short dirty_rest : 15;
    char unknown_2bf0[0x38a47 - 0x2bf0];
    int frame;                          // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    void* map;                          // +0x391e9
};
#pragma pack(pop)

struct Mission { int GetTerrainLength(); int ComputeMapChecksum(); void LoadMissionByName(PlayerInfo_0044a680* info); char* GetMissionName(); };
struct UnitSync {
    int AllPlayersSynced();
    void ProcessSync();
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    char* GetSyncStatusText();
    void CheckUnitAvailable(unsigned int, int);
};
struct Player { void SetType(int param); };

extern Game* g_game;
extern int DAT_00512994;
extern int DAT_005129a4;
extern unsigned int DAT_005129a8;
extern unsigned int DAT_0050550c;

int __stdcall HandleNetPackets();
unsigned char __stdcall FindHostSlot();
void __stdcall FUN_004455b0();
void __stdcall RefreshBattleRoomRows();
void ShowSelectedMapInfo();
void __stdcall UpdateMaxUnitsText(Gui_0044a680* gui, int index);
void __stdcall UpdateMetalText(Gui_0044a680* gui, int index);
void __stdcall PlaySoundByName(char* name, int param);
void __stdcall FUN_0049fa90(Gui_0044a680* gui);
void __stdcall FUN_0049fad0(Gui_0044a680* gui);
int __stdcall FindGadgetIndex(Gadget_0044a680* entries, char* name, int type);
void __stdcall FUN_004a0570(Gui_0044a680* gui, char* name, int param);
void __stdcall FUN_004a0bf0(Gui_0044a680* gui, char* name, char* text, int param);
void __stdcall FUN_004a1250(Gui_0044a680* gui, char* name, int param);
Gadget_0044a680* __stdcall FUN_004a0200(Gadget_0044a680* entries, char* name);
Gadget_0044a680* __stdcall FUN_004a0280(Gadget_0044a680* entries, char* name);
void __stdcall FUN_004a15c0(Gadget_0044a680* entries, int widget, RECT* rect);
int __stdcall GetTextPixelWidth(char* text);
int __stdcall GetFontLineHeight();
void __stdcall FUN_004a50e0(int a, char* text, int x, int y, int w, int h);
void __stdcall FUN_004a5d30(Gui_0044a680* gui, int flag);
void __stdcall CloseTopScreen(void* gui);
int __stdcall IsScreenNamed(Gui_0044a680* gui, char* name);
void __stdcall SetSliderFromValue(Gadget_0044a680* gadget, int value);
int __stdcall ReadSliderValue(Gadget_0044a680* gadget);
int __stdcall GetTicks();
unsigned char __stdcall FindGameCdDrive(int param);
void __stdcall SendNetHeartbeat();
void __stdcall BroadcastPlayerInfo();
void __stdcall UpdateNetGameInfo();
int __stdcall AreAllPlayersReady();

// The map check at 0x440cd0, which has no callers: /Ob2 inlined it.
int CheckMapCrc()
{
    if (!((Mission*)g_game->map)->GetTerrainLength()) {
        return 0;
    }
    unsigned char me = FindHostSlot();
    PlayerInfo_0044a680* data = 0;
    int check = 0;
    if (me != 10) {
        data = g_game->players[me].info;
        if (data->versionMajor >= 2)
            check = 1;
        else if (data->versionMajor == 1 && data->versionMinor >= 2)
            check = 1;
    }
    if (!check) {
        return 1;
    }
    if (((Mission*)g_game->map)->ComputeMapChecksum() != data->mapCrc)
        return 0;
    return 1;
}

// The slot swap at 0x4453a0, which has no callers. MSVC inlines it only when
// it is declared inline (it has a loop).
inline void __stdcall SwapPlayerSlots(Player_0044a680* param_1, Player_0044a680* param_2)
{
    Player_0044a680 tmp = *param_2;
    *param_2 = *param_1;
    *param_1 = tmp;
    ((Player*)param_1)->SetType(0);
    param_1->active = 0;
    for (int i = 0; i <= 10; i++) {
        Player_0044a680* p = &g_game->players[i];
        if (p->active != 0
            && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            p->field_146 = i;
        } else {
            g_game->players[i].field_146 = 10;
        }
    }
}

// The slot compaction at 0x445450, which has no callers. Declared inline for
// its loops, like 0x4453a0.
inline void FUN_00445450()
{
    Player_0044a680* p = g_game->players;
    Player_0044a680* q = g_game->players + 1;
    Player_0044a680* end = g_game->players + 10;
    while (1) {
        if (q >= end && p >= end)
            break;
        while ((p->active != 0
                    && (p->type == 1 || p->type == 2 || p->type == 3)
                    && p->field_146 != 10)
               || p->type == 4) {
            if (p >= end)
                break;
            p++;
        }
        q = p + 1;
        for (; q->active == 0
               || (q->type != 1 && q->type != 2 && q->type != 3)
               || q->field_146 == 10;
             q++) {
            if (q >= end)
                break;
        }
        if (q >= end)
            break;
        if (p >= end)
            break;
        SwapPlayerSlots(q, p);
    }
}

// The ENERGY slider handler at 0x445d60, which has no callers: /Ob2 inlined it.
void __stdcall UpdateEnergyText(Gui_0044a680* gui, int unused)
{
    char text[20];
    Gadget_0044a680* value = FUN_004a0200(gui->table->entries, "ENERGY");

    if (value != 0) {
        int shown = ReadSliderValue(value) / 100 * 100;
        PlayerInfo_0044a680* info;

        _itoa(shown, text, 10);
        FUN_004a0bf0(gui, "ENERGYTEXT", text, 0);
        info = g_game->players[g_game->localPlayer].info;
        info->energy = (unsigned short)(shown / 100);
        if (info->flags & 1) {
            BroadcastPlayerInfo();
            UpdateNetGameInfo();
        }
    }
}

// The slider setter at 0x445e20, which has no callers: /Ob2 inlined it.
void __stdcall FUN_00445e20(Gui_0044a680* gui, char* name, int value)
{
    // Gadget kept in a local: holds the value in edi across FUN_004a0200.
    Gadget_0044a680* gadget = FUN_004a0200(gui->table->entries, name);
    SetSliderFromValue(gadget, value);
}

// FUNCTION: 0x44a680
void UpdateBattleRoom()
{
    Player_0044a680* pl;
    Gadget_0044a680* entries;

    g_game->frame++;

    if (HandleNetPackets())
        g_game->dirty = 1;
    else if (!CheckMapCrc())
        g_game->dirty = 1;

    pl = &g_game->players[g_game->localPlayer];
    if (pl->field_22 != 0) {
        g_game->field_2bc0 = 3;
        CloseTopScreen(&g_game->gui);
        return;
    }

    entries = g_game->gui.table->entries;
    if (IsScreenNamed(&g_game->gui, "LOUNGE2.GUI") != 0) {
        int idx = FindGadgetIndex(entries, "PLAYER0", 0xe);
        entries[idx].colour = 0x18;
    }

    if (g_game->dirty) {
        if (DAT_00512994 == 0) {
            FUN_004455b0();
        } else {
            FUN_00445450();
        }

        if ((unsigned int)g_game->field_2a3c != DAT_0050550c) {
            DAT_0050550c = g_game->field_2a3c;
            UpdateNetGameInfo();
        }

        if ((pl->info->flags & 1) == 0) {
            unsigned char host = FindHostSlot();
            if (host != 10) {
                if (IsScreenNamed(&g_game->gui, "LOUNGE2.GUI") != 0) {
                    PlayerInfo_0044a680* info = g_game->players[host].info;
                    ((Mission*)g_game->map)->LoadMissionByName(info);
                    FUN_00445e20(&g_game->gui, "MAXUNITS", g_game->players[host].info->maxUnits - 0x14);
                    FUN_00445e20(&g_game->gui, "METAL", g_game->players[host].info->metal * 100);
                    FUN_00445e20(&g_game->gui, "ENERGY", g_game->players[host].info->energy * 100);
                    UpdateMaxUnitsText(&g_game->gui, 0);
                    UpdateEnergyText(&g_game->gui, 0);
                    UpdateMetalText(&g_game->gui, 0);
                } else if (IsScreenNamed(&g_game->gui, "viewmap.gui") != 0) {
                    PlayerInfo_0044a680* info = g_game->players[host].info;
                    if (strcmp(((Mission*)g_game->map)->GetMissionName(), info->map) != 0) {
                        ((Mission*)g_game->map)->LoadMissionByName(g_game->players[host].info);
                        ShowSelectedMapInfo();
                        FUN_0049fad0(&g_game->gui);
                    }
                }
            }
        }
        g_game->dirty = 0;
        if (IsScreenNamed(&g_game->gui, "LOUNGE2.GUI") != 0) {
            if (pl->info->flags & 1) {
                int synched = ((UnitSync*)g_game->net)->AllPlayersSynced();
                int ready = AreAllPlayersReady();
                Gadget_0044a680* start;

                FUN_004a1250(&g_game->gui, "SYNCHING", 1);
                start = FUN_004a0280(g_game->gui.table->entries, "battlestart");
                if (start->frame > 0 && DAT_005129a4 < GetTicks()) {
                    if (start->frame < 8) {
                        start->frame++;
                        FUN_0049fa90(&g_game->gui);
                        g_game->dirty = 1;
                    }
                    DAT_005129a4 += 4;
                    if (start->frame == 4)
                        PlaySoundByName("Panel", 0);
                }
                if (start->frame != 0 && start->frame < *start->frames - 1)
                    g_game->dirty = 1;
                if (ready != 0) {
                    g_game->dirty = 1;
                    if (start->frame == 0) {
                        start->frame = 1;
                        DAT_005129a4 = GetTicks();
                        PlaySoundByName("Options", 0);
                    }
                    start->c8_0 = 0;
                    {
                        int idx = FindGadgetIndex(entries, "START", 1);
                        unsigned int colour = GetTicks() & 0x1f;
                        if (colour != entries[idx].colour) {
                            entries[idx].colour = colour;
                            FUN_0049fa90(&g_game->gui);
                        }
                    }
                }
                FUN_004a1250(&g_game->gui, "START", ready == 0);
                FUN_004a0570(&g_game->gui, "START", synched);
                FUN_004a0570(&g_game->gui, "SYNCHING", synched == 0);
            }
            RefreshBattleRoomRows();
            FUN_0049fa90(&g_game->gui);
        }
    }

    if (IsScreenNamed(&g_game->gui, "LOUNGE2.GUI") != 0) {
        FUN_004a5d30(&g_game->gui, 1);
        {
            // unsigned char counter (a short works too), not int.
            for (unsigned char i = 0; i < 10; i++) {
                Player_0044a680* p = &g_game->players[i];
                if (p->type != 0 && p->type != 4) {
                    RECT rect;
                    char buf[20];
                    int widget;
                    PlayerInfo_0044a680* info;
                    int w;
                    int h;
                    sprintf(buf, "LOGO%i", i);
                    widget = FindGadgetIndex(entries, buf, 0xe);
                    FUN_004a15c0(entries, widget, &rect);
                    info = p->info;
                    sprintf(buf, "%i.%i", info->versionMajor, info->versionMinor);
                    w = GetTextPixelWidth(buf);
                    h = GetFontLineHeight();
                    FUN_004a50e0(0, buf,
                                 (rect.left + rect.right - w) / 2,
                                 (rect.top + rect.bottom - h) / 2,
                                 w, 0);
                }
            }
        }
        FUN_004a5d30(&g_game->gui, 0);
    }

    ((UnitSync*)g_game->net)->ProcessSync();
    if (DAT_005129a8 < (unsigned int)GetTicks()) {
        unsigned char r;
        PlayerInfo_0044a680* info;
        DAT_005129a8 = GetTicks() + 0x3c;
        r = FindGameCdDrive(1);
        info = pl->info;
        info->f9d_2 = (r != 0);
        SendNetHeartbeat();
    }
}
