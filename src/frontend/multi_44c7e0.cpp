// Decompiled by deepseek-v4.1-flash, finished by claude-opus-5-5. Names are provisional.
// Opens the unit restrictions dialog (RESTRICT2.GUI): allocates the unit
// list, picture and text arrays, fills one 0x62-byte record per buildable
// unit type (name, description, costs, its type index and its current
// limit), sorts the records by name, sets up the twelve SLIDERn gadgets and
// the SCROLLSLIDER, and fills the energy and metal costs of the selected
// unit. Only the host may load, save or reset the restrictions.
//
// MATCH. What it took (from 85.2%):
// - The SCROLLSLIDER block is the slider set-up 0x445e50, which has no
//   callers, defined above unannotated and inlined (0x449bb0 calls it too).
//   It keeps the menu in esi and the index in edi for the UpdateUnitSliders and
//   FUN_0049fa90 calls, which the older file could not reproduce.
// - SetGadgetRows takes the menu's table (g_game+0x531), not its entries.
// - The scan loop skips with `continue` on the unit type's bit 15 (an int
//   bitfield, tested positively: shr/test) and on its name.
// - The limit is one ternary (`count = info.field_c == -1 ? 0x65 :
//   info.field_c;`); the if-statement form put the record offset in ebx and
//   the DESCLIST gadget in ebp, the other way round from the original.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct PlayerInfo_0044c7e0 {
    char unknown_0[0x97];
    unsigned char f97_0 : 1;            // +0x97
    unsigned char f97_rest : 7;
};

struct Player_0044c7e0 {                // 0x14b bytes
    char unknown_0[0x27];
    PlayerInfo_0044c7e0* info;          // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Record_0044c7e0 {                // 0x62 bytes
    char name[0x52];                    // +0x00
    int type;                           // +0x52
    int field_56;                       // +0x56
    int count;                          // +0x5a
    int field_5e;                       // +0x5e
};

struct UnitType_0044c7e0 {              // 0x249 bytes
    char unitName[0x20];                // +0x00
    char name[0x80];                    // +0x20
    char description[0xe6];             // +0xa0
    float energyCost;                   // +0x186
    float metalCost;                    // +0x18a
    char unknown_18e[0x245 - 0x18e];
    unsigned int f245_low : 15;         // +0x245
    unsigned int f245_15 : 1;
    unsigned int f245_high : 16;
};

struct Info_0044c7e0 {                  // filled by GetUnitEntry
    char unknown_0[0xa];
    short field_a;                      // +0xa
    int field_c;                        // +0xc
};

struct Gui_0044c7e0;
typedef void (__stdcall* Callback_0044c7e0)(Gui_0044c7e0* gui, int index);

struct Gadget_0044c7e0 {                // 0x15b bytes
    char unknown_0[0x1b];
    unsigned int field_1b;              // +0x1b
    char unknown_1f[0xba - 0x1f];
    short field_ba;                     // +0xba
    char unknown_bc[0xce - 0xbc];
    void* field_ce;                     // +0xce
    Record_0044c7e0* records;           // +0xd2
    char* flags;                        // +0xd6
    short field_da;                     // +0xda
    char unknown_dc[0x13c - 0xdc];
    int max;                            // +0x13c
    short value;                        // +0x140
    short unknown_142;
    void* callback;                     // +0x144
    char unknown_148[2];
    void* game;                         // +0x14a
    char unknown_14e[0x15b - 0x14e];
};

struct Layer_0044c7e0 {
    int unknown_0;
    Gadget_0044c7e0* entries;           // +0x04
    void* handler;                      // +0x08
    int field_c;                        // +0x0c
    char unknown_10[0x1c - 0x10];
    void* field_1c;                     // +0x1c
};

struct Gui_0044c7e0 {
    char unknown_0[0x18];
    Layer_0044c7e0* table;              // +0x18
};

class UnitSync {
public:
    int GetUnitEntry(UnitType_0044c7e0* type, Info_0044c7e0* out);
};

struct Game {
    char unknown_0[0x519];
    Gui_0044c7e0 gui;                   // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_0044c7e0 players[10];        // +0x1b63
    char unknown_2851[0x2a30 - 0x2851];
    UnitSync* queue;                    // +0x2a30
    char unknown_2a34[0x2a42 - 0x2a34];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x1438f - 0x2a43];
    int numUnitTypes;                   // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_0044c7e0* unitTypes;       // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;
extern Record_0044c7e0* DAT_005129b4;
extern int* DAT_005129b8;
extern int* DAT_005129c4;
extern int DAT_00512768;
extern int DAT_005129c0;

Layer_0044c7e0* __stdcall LoadGuiLayer(Gui_0044c7e0* gui, const char* name, int size);
int __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
Gadget_0044c7e0* __stdcall FindGadgetChecked(Gadget_0044c7e0* entries, char* name);
int __stdcall FindGadgetIndex(Gadget_0044c7e0* entries, char* name, int type);
Gadget_0044c7e0* __stdcall FUN_004a0200(Gadget_0044c7e0* entries, char* name);
void __stdcall FUN_004a0bf0(Gui_0044c7e0* gui, char* name, char* text, int flag);
void __stdcall FUN_004a1250(Gui_0044c7e0* gui, char* name, int enabled);
void __stdcall FUN_004a32a0(Gui_0044c7e0* gui, char* name, char* text, int count, int flag);
void __stdcall SetGadgetRows(Layer_0044c7e0* table, char* name, int* pics, int count);
void __stdcall RenderLayer(Gui_0044c7e0* gui, int value);
void __stdcall FUN_0049fa90(Gui_0044c7e0* gui);
void __stdcall FUN_0049fb10(Gui_0044c7e0* gui, int value);
void __stdcall SetSliderFromValue(Gadget_0044c7e0* gadget, int value);
void __stdcall UpdateUnitSliders(Gui_0044c7e0* gui, int index);
char* __stdcall Translate(char* text);
void __stdcall FUN_0044c370(void* panel, UnitType_0044c7e0* type);
void FUN_0044c220();
void __stdcall HandleRestrictionsClick(Gui_0044c7e0* gui);
void __stdcall HandleUnitCountSlider(void* obj, char* gadget);
int __cdecl FUN_0044c7a0(const char* a, const char* b);

// The slider set-up at 0x445e50, which has no callers: /Ob2 inlined it.
void __stdcall FUN_00445e50(char* name, int max, int value, Callback_0044c7e0 callback)
{
    Gui_0044c7e0* gui = &g_game->gui;
    Gadget_0044c7e0* gadgets = gui->table->entries;
    int index = FindGadgetIndex(gadgets, name, 0xe);
    if (index != -1) {
        Gadget_0044c7e0* gadget = FUN_004a0200(gadgets, name);
        gadget->max = max;
        gadget->callback = callback;
        gadget->value = value;
        SetSliderFromValue(gadget, gadget->value);
        gadget->game = g_game;
    }
    callback(gui, index);
    FUN_0049fa90(gui);
}

// FUNCTION: 0x44c7e0
void OpenUnitRestrictions()
{
    int host = g_game->players[g_game->localPlayer].info->f97_0 & 1;
    Layer_0044c7e0* layer;
    Gadget_0044c7e0* entries;
    char* flags;
    Gadget_0044c7e0* desc;
    Gadget_0044c7e0* pic;
    int* pics;
    char* text;
    char* dst;
    int n;
    int i;

    layer = LoadGuiLayer(&g_game->gui, "RESTRICT2.GUI", 0x880);
    layer->handler = HandleRestrictionsClick;
    layer->field_c = 0;
    layer->field_1c = FUN_0044c220;
    DAT_00512768 = 0;
    LoadPictureCached("UnitRestrict5x", 0, 0, 0);

    entries = layer->entries;
    flags = (char*)FUN_004d83b0("FLAGS", g_game->numUnitTypes);
    desc = FindGadgetChecked(entries, "DESCLIST");
    desc->field_ce = FUN_0044c370;
    desc->flags = flags;
    desc->field_da = 0x20;
    desc->field_1b |= 0x100;

    pic = FindGadgetChecked(layer->entries, "PICLIST");
    pic->flags = flags;
    pic->field_1b |= 0x180;
    pic->field_da = desc->field_da;

    pics = (int*)FUN_004d83b0("UNITPICARRAY", g_game->numUnitTypes * 0x18);
    memset(pics, 0, g_game->numUnitTypes * 0x18);
    text = (char*)FUN_004d83b0("UNITTEXTARRAY", g_game->numUnitTypes << 5);
    *(int*)text = 0;

    DAT_005129b4 = (Record_0044c7e0*)FUN_004d83b0("UNITSRESTRICTINFO", g_game->numUnitTypes * 0x62);
    desc->records = DAT_005129b4;
    for (i = 0; i < g_game->numUnitTypes; i++)
        DAT_005129b4[i].type = 0;

    DAT_005129b8 = (int*)FUN_004d83b0("UNITSPICS", g_game->numUnitTypes << 2);
    memset(DAT_005129b8, 0, g_game->numUnitTypes << 2);
    memset(DAT_005129b4, 0, g_game->numUnitTypes * 0x62);
    DAT_005129c4 = (int*)FUN_004d83b0("OLDCOUNTS", g_game->numUnitTypes << 2);

    n = 0;
    for (i = 1; i < g_game->numUnitTypes; i++) {
        if (g_game->unitTypes[i].f245_15)
            continue;
        if (!g_game->unitTypes[i].name)
            continue;
        {
            UnitType_0044c7e0* type = &g_game->unitTypes[i];
            Info_0044c7e0 info;
            int count;
            sprintf(DAT_005129b4[n].name, "%s\r%s %dM  %dE",
                    g_game->unitTypes[i].unitName, Translate(type->description),
                    (int)type->metalCost, (int)type->energyCost);
            DAT_005129b4[n].type = i;
            g_game->queue->GetUnitEntry(&g_game->unitTypes[i], &info);
            count = info.field_c == -1 ? 0x65 : info.field_c;
            DAT_005129b4[n].count = count;
            DAT_005129c4[n] = count;
            DAT_005129b4[n].field_5e = info.field_a;
            n++;
        }
    }

    qsort(DAT_005129b4, n, 0x62, (int (__cdecl*)(const void*, const void*))FUN_0044c7a0);

    dst = text;
    for (i = 0; i < g_game->numUnitTypes; i++) {
        strcpy(dst, DAT_005129b4[i].name);
        dst += strlen(DAT_005129b4[i].name) + 1;
    }

    for (i = 0; i < 0xc; i++) {
        char name[0x14];
        Gadget_0044c7e0* slider;
        sprintf(name, "SLIDER%d", i);
        slider = FUN_004a0200(entries, name);
        slider->game = slider;
        slider->max = 0x65;
        slider->callback = HandleUnitCountSlider;
    }

    FUN_00445e50("SCROLLSLIDER", 0xd2, 0, UpdateUnitSliders);

    FUN_004a32a0(&g_game->gui, "DESCLIST", text, n, 0);
    SetGadgetRows(g_game->gui.table, "PICLIST", pics, n);
    UpdateUnitSliders(&g_game->gui, 0);

    {
        Gui_0044c7e0* gui = &g_game->gui;
        UnitType_0044c7e0* type = &g_game->unitTypes[desc->records[desc->field_ba].type];
        char buf[0x14];
        sprintf(buf, "%d", (int)type->energyCost);
        FUN_004a0bf0(gui, "ENERGYTEXT", buf, 0);
        sprintf(buf, "%d", (int)type->metalCost);
        FUN_004a0bf0(gui, "METALTEXT", buf, 0);
    }

    {
        int enabled = host == 0;
        FUN_004a1250(&g_game->gui, "Load", enabled);
        FUN_004a1250(&g_game->gui, "Save", enabled);
        FUN_004a1250(&g_game->gui, "Reset", enabled);
    }
    RenderLayer(&g_game->gui, 0x40);
    FUN_0049fb10(&g_game->gui, 1);
    DAT_005129c0 = 0;
}
