// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Builds the speed options page: opens the SPEEDS / SPEEDSRT.GUI layout, adds
// the SPEEDS group, positions the GAME, SCREEN, MAXLINES and TXTSCROL sliders
// from the saved settings, runs every type 4 entry, then refreshes the menu.
#include <stdio.h>

#pragma pack(push, 1)
struct Entry_0045ed50 {                  // 0x15b bytes
    unsigned char type;                  // +0x00
    char unknown_1[0xb6 - 1];
    short count;                         // +0xb6 (entry 0 only)
    char unknown_b8[0x136 - 0xb8];
    short steps;                         // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                             // +0x13c
    short pos;                           // +0x140
    char unknown_142[2];
    void (__stdcall* fn)(void* obj, int arg);   // +0x144
    char unknown_148[0x15b - 0x148];
};

struct Object_0045ed50 {
    char unknown_0[4];
    Entry_0045ed50* entries;             // +0x04
    void (__stdcall* fn)(void* obj, int arg);   // +0x08
};

struct Game {
    char unknown_0[0x519];
    char menu[1];                        // +0x519
    char unknown_51a[0x1434d - 0x51a];
    unsigned char field_1434d;           // +0x1434d
    char unknown_1434e[0x37ebe - 0x1434e];
    unsigned char flag_37ebe;            // +0x37ebe
    char unknown_37ebf[0x37efa - 0x37ebf];
    int field_37efa;                     // +0x37efa
    char unknown_37efe[0x37f18 - 0x37efe];
    unsigned char field_37f18;           // +0x37f18
    char unknown_37f19[0x37f23 - 0x37f19];
    int field_37f23;                     // +0x37f23
    int field_37f27;                     // +0x37f27
    char unknown_37f2b[0x38a4b - 0x37f2b];
    unsigned short field_38a4b;          // +0x38a4b
};
#pragma pack(pop)

extern Game* g_game;

Object_0045ed50* __cdecl OpenOptionsLayout();
void FUN_0045ce80();
void __stdcall RenderLayer(void* obj, int value);
int __stdcall LoadGuiLayer(void* obj, char* name, int size);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void __stdcall FUN_0049fa50(void* obj);
int __stdcall FindGadgetIndex(Entry_0045ed50* entries, char* name, int type);
Entry_0045ed50* __stdcall FUN_004a0200(Entry_0045ed50* entries, char* name);
void __stdcall SetGadgetStatusByName(void* obj, char* name, int value);
void __stdcall SetButtonStageByName(void* obj, char* name, int value);
void __stdcall FUN_004a0bf0(void* obj, char* name, char* text, int value);
void __stdcall FUN_0049fb10(void* obj, int value);
void FUN_00428b60();

void __stdcall FUN_0045ead0(void* obj, int arg);
void __stdcall HandleGameSpeedSlider(void* obj, int arg);
void __stdcall HandleScreenSlider(void* obj, int arg);
void __stdcall HandleMaxLinesSlider(void* obj, int arg);
void __stdcall HandleTextScrollSlider(void* obj, int arg);

// FUNCTION: 0x45ed50
void OpenSpeedOptions()
{
    Object_0045ed50* obj = OpenOptionsLayout();
    RenderLayer(&g_game->menu, 2);
    FUN_0045ce80();
    if (g_game->flag_37ebe & 1) {
        LoadGuiLayer(&g_game->menu, "SPEEDSRT.GUI", 0x200);
    } else {
        LoadGuiLayer(&g_game->menu, "SPEEDS.GUI", 0x200);
        LoadPictureCached("optinterface4x", 0, 0, 0);
    }
    obj->fn = FUN_0045ead0;
    FUN_0049fa50(&g_game->menu);
    int found = FindGadgetIndex(obj->entries, "GAME", 0xe);
    SetGadgetStatusByName(&g_game->menu, "SPEEDS", 1);
    if (found != -1) {
        Entry_0045ed50* e = FUN_004a0200(obj->entries, "GAME");
        e->max = 0x15;
        e->fn = HandleGameSpeedSlider;
        int value = g_game->field_38a4b;
        if (value > 0x15) {
            value = 0x15;
        }
        float f = (float)value * (float)(e->steps - 1) * 0.0476190485060215f;
        if (f - (int)f != 0.0f) {
            f += 1.0;
        }
        e->pos = (short)f;
    }
    if (FindGadgetIndex(obj->entries, "SCREEN", 0xe) != -1) {
        Entry_0045ed50* e = FUN_004a0200(obj->entries, "SCREEN");
        e->max = 0x41;
        int value = g_game->field_1434d;
        if (value > 0x41) {
            value = 0x41;
        }
        float f = (float)value * (float)(e->steps - 1) * 0.015384615398943424f;
        if (f - (int)f != 0.0f) {
            f += 1.0;
        }
        e->pos = (short)f;
        e->fn = HandleScreenSlider;
    }
    SetButtonStageByName(&g_game->menu, "UNITCHAT", g_game->field_37f18 / 5);
    SetButtonStageByName(&g_game->menu, "LEFTCLICK", g_game->field_37efa);
    char text[20];
    sprintf(text, g_game->field_37f27 ? "%d" : "None", g_game->field_37f27);
    FUN_004a0bf0(&g_game->menu, "MAXLINESTEXT", text, 0);
    if (FindGadgetIndex(obj->entries, "MAXLINES", 4) != -1) {
        Entry_0045ed50* e = FUN_004a0200(obj->entries, "MAXLINES");
        e->max = 0x1e;
        int value = g_game->field_37f27;
        if (value > 0x1e) {
            value = 0x1e;
        }
        float f = (float)value * (float)(e->steps - 1) * 0.03333333507180214f;
        if (f - (int)f != 0.0f) {
            f += 1.0;
        }
        e->pos = (short)f;
        e->fn = HandleMaxLinesSlider;
    }
    if (FindGadgetIndex(obj->entries, "TXTSCROL", 0xe) != -1) {
        Entry_0045ed50* e = FUN_004a0200(obj->entries, "TXTSCROL");
        e->max = 0x14;
        int value = g_game->field_37f23;
        if (value > 0x14) {
            value = 0x14;
        }
        float f = (float)value * (float)(e->steps - 1) * 0.05000000074505806f;
        if (f - (int)f != 0.0f) {
            f += 1.0;
        }
        e->pos = (short)f;
        e->fn = HandleTextScrollSlider;
    }
    for (int i = 1; i <= obj->entries->count; i++) {
        if (obj->entries[i].type == 4)
            obj->entries[i].fn(&g_game->menu, 0);
    }
    FUN_0049fb10(&g_game->menu, 1);
    FUN_00428b60();
    RenderLayer(&g_game->menu, 0x40);
}
