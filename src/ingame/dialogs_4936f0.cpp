// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Opens the resource sharing screen (SHARE.GUI): it walks the ten player
// records, finds the local player's entry (flagged 0x40), points the METAL and
// ENERGY sliders at the local counts, and finishes with the menu setup calls.
// A resource's owner is flagged 0x40 in the player record; the two sliders are
// the "METAL#" and "ENERGY#" texts.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_004936f0 {                  // 0x15b bytes
    char unknown_0[0x17];
    short field_17;                      // +0x17
    short field_19;                      // +0x19
    char unknown_1b[0x136 - 0x1b];
    short field_136;                     // +0x136
    char unknown_138[0x13c - 0x138];
    int field_13c;                       // +0x13c
    short field_140;                     // +0x140
    short field_142;                     // +0x142
    void (__stdcall* handler)(void*, int); // +0x144
    char unknown_148[0x14a - 0x148];
    void* field_14a;                     // +0x14a
    char unknown_14e[0x15b - 0x14e];
};

struct Layer_004936f0 {
    int unknown_0;
    char* entries;                       // +0x04
    void (__stdcall* handler)(void*);    // +0x08
    void* data;                          // +0x0c
    char unknown_10[0x24 - 0x10];
};

struct Menu_004936f0 {
    char unknown_0[0x18];
    Layer_004936f0* layer;               // +0x18
};

struct Owner_004936f0 {
    char unknown_0[0x9b];
    unsigned short pad : 6;              // +0x9b
    unsigned short bit6 : 1;
    unsigned short rest : 9;
};

struct Player_004936f0 {                 // 0x14b bytes
    int active;                          // +0x00
    int field_4;                         // +0x04
    char unknown_8[0x27 - 0x8];
    Owner_004936f0* owner;               // +0x27
    char name[0x73 - 0x2b];              // +0x2b
    unsigned char state;                 // +0x73
    char unknown_74[0x8c - 0x74];
    float metal;                         // +0x8c
    char unknown_90[0x98 - 0x90];
    float energy;                        // +0x98
    char unknown_9c[0x140 - 0x9c];
    int field_140;                       // +0x140
    unsigned short field_144;            // +0x144
    unsigned char field_146;             // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x519];
    Menu_004936f0 menu;                  // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_004936f0 players[10];         // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short field_2a3c;           // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;           // +0x2a42
    char unknown_2a43[0x37ebe - 0x2a43];
    unsigned short pad_37ebe : 6;        // +0x37ebe
    unsigned short bit6_37ebe : 1;
    unsigned short rest_37ebe : 9;
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_0051e6d0[10];

Layer_004936f0* __stdcall LoadGuiLayer(Menu_004936f0* menu, const char* name, int flags);
void __stdcall HandleShareDialogEvent(void* gadget);
int __stdcall FindGadgetIndex(char* entries, char* name, int type);
Entry_004936f0* __stdcall FUN_004a0200(char* entries, char* name);
void __stdcall UpdateMetalReadout(void* entry, int param_2);
void __stdcall UpdateEnergyReadout(void* entry, int param_2);
void __stdcall SetSliderFromValue(void* entry, int param_2);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __stdcall CloseTopScreen(Menu_004936f0* menu);
void __stdcall FUN_004a32a0(Menu_004936f0* menu, char* name, char* text, int count, int flag);
int __stdcall ReadSliderValue(Entry_004936f0* entry);
void __stdcall FUN_004a0bf0(Menu_004936f0* menu, char* name, char* text, int param_4);
void __stdcall FUN_0049fa90(Menu_004936f0* menu);
void __stdcall FUN_0049fb10(Menu_004936f0* menu, int value);
void __stdcall RenderLayer(Menu_004936f0* menu, int value);

// FUNCTION: 0x4936f0
void OpenShareDialog()
{
    if (g_game->players[g_game->localPlayer].owner->bit6)
        return;
    Layer_004936f0* layer = LoadGuiLayer(&g_game->menu, "SHARE.GUI", 0x800);
    g_game->bit6_37ebe = 1;
    char* entries = layer->entries;
    layer->handler = HandleShareDialogEvent;
    layer->data = g_game;
    int idx = FindGadgetIndex(entries, "METAL", 0xe);
    if (idx != -1) {
        Entry_004936f0* e = (Entry_004936f0*)(entries + idx * 0x15b);
        e->field_142 = ((Entry_004936f0*)(layer->entries + idx * 0x15b))->field_19;
        e->field_136 = ((Entry_004936f0*)(layer->entries + idx * 0x15b))->field_17 - e->field_142;
        e->field_13c = (int)g_game->players[g_game->localPlayer].energy;
        e->handler = UpdateMetalReadout;
        e->field_140 = 0;
        SetSliderFromValue(e, 0);
        e->field_14a = g_game;
    }
    idx = FindGadgetIndex(layer->entries, "ENERGY", 0xe);
    if (idx != -1) {
        Entry_004936f0* e = FUN_004a0200(layer->entries, "ENERGY");
        e->field_142 = ((Entry_004936f0*)(layer->entries + idx * 0x15b))->field_19;
        e->field_136 = ((Entry_004936f0*)(layer->entries + idx * 0x15b))->field_17 - e->field_142;
        e->field_13c = (int)g_game->players[g_game->localPlayer].metal;
        e->handler = UpdateEnergyReadout;
        e->field_140 = 0;
        SetSliderFromValue(e, 0);
        e->field_14a = g_game;
    }

    char* names = (char*)FUN_004d83b0("PLAYERS", g_game->field_2a3c * 30);
    char* np = names;
    *np = 0;
    memset(DAT_0051e6d0, -1, sizeof(DAT_0051e6d0));
    int* ids = DAT_0051e6d0;
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player_004936f0* p = &g_game->players[i];
        if (p->active && (p->state == 1 || p->state == 2 || p->state == 3) && p->field_146 != 10 &&
            (p->field_144 != 0 || p->field_140 == 0) && p->state != 1 && !p->owner->bit6) {
            strcpy(np, p->name);
            np += strlen(p->name) + 1;
            *ids = p->field_4;
            count++;
            ids++;
        }
    }
    if (count == 0) {
        CloseTopScreen(&g_game->menu);
        return;
    }
    FUN_004a32a0(&g_game->menu, "PLYRLIST", names, count, 0);
    char text[0x34];
    // Both tail blocks go through the local menu pointer, menu first, with no
    // self-comparison: this fixes their register choice.
    Menu_004936f0* menu = &g_game->menu;
    Layer_004936f0* lyr = menu->layer;
    char* ents = lyr->entries;
    Entry_004936f0* e = FUN_004a0200(ents, "METAL");
    if (e) {
        sprintf(text, "%d", ReadSliderValue(e));
        FUN_004a0bf0(menu, "METAL#", text, 0);
    }
    menu = &g_game->menu;
    lyr = menu->layer;
    ents = lyr->entries;
    e = FUN_004a0200(ents, "ENERGY");
    if (e) {
        sprintf(text, "%d", ReadSliderValue(e));
        FUN_004a0bf0(menu, "ENERGY#", text, 0);
    }
    FUN_0049fa90(&g_game->menu);
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}
