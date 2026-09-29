// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (best 70.9%). Everything below is semantically right and the call
// sequence, string arguments and field offsets match; only register
// allocation differs, and it differs from the first instruction:
//   * original opens with `sub esp,0x10` + `xor esi,esi` + `cmp eax,esi`;
//     ours has `sub esp,0xc` + `test eax,eax`. The original keeps a zero in
//     esi from function entry (it is the first flags-loop offset, hoisted),
//     and reuses esi for the three FUN_004288d0("optvisual4x") args and for
//     `layer->data = 0`; ours emits `push 0` and `test eax,eax`. Declaring an
//     explicit `int zero = 0` was constant-folded and changed nothing.
//   * in the video-mode block the original keeps list/modes in esi, the
//     VIDSLDR entry in edi and g_game->width in ebp; ours picks edi, ebp and
//     ebx. Same statements, different allocation.
//   * the original spills &g_game->menu to the 4th local ([esp+0x1c]) inside
//     the video-mode block; ours has no 4th local.
// A later attempt should try separate scoped loop counters and a `Menu* menu`
// local, and the `if (param_1 == 0)`/`else` inversion, to shift the allocator.
// Builds the visual / video-mode options page (SELVMODE.GUI when param_1 is
// set, otherwise the VISUALS or VISUALRT page next to the normal menu),
// installs FUN_0045e100 as its handler, fills the video mode list and the
// VIDSLDR / GAMMA sliders, then writes the ANTISHADING, BSHADOWS and SHADING
// checkboxes back from the flags word at g_game+0x37f06.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_0045e5e0 {                  // 0x15b bytes
    unsigned char type;                  // +0x00
    char unknown_1;
    char name[0x10];                     // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                         // +0xb6 (entry 0 only)
    char unknown_b8[0x136 - 0xb8];
    short steps;                         // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                             // +0x13c
    short pos;                           // +0x140
    char unknown_142[2];
    void (__stdcall* fn)(void* obj, int value);  // +0x144
    char unknown_148[2];
    void* data;                          // +0x14a
};

struct Mode_0045e5e0 {
    int width;                           // +0x0
    int height;                          // +0x4
    int refreshRate;                     // +0x8
};

struct List_0045e5e0 {                   // the "SELECT VIDEO MODE" object
    int count;                           // +0x0
    Mode_0045e5e0* modes;                // +0x4
    char unknown_8[0xc];
    char* buffer;                        // +0x14
};

struct Layer_0045e5e0 {                  // object returned by FUN_004aa8f0
    char unknown_0[4];
    Entry_0045e5e0* entries;             // +0x4
    void (__stdcall* handler)(void*);    // +0x8
    void* data;                          // +0xc
    char unknown_10[0x15b];
};

struct Holder_0045e5e0 {
    char unknown_0[4];
    Entry_0045e5e0* entries;             // +0x4
};

struct Menu_0045e5e0 {
    char unknown_0[0x18];
    Holder_0045e5e0* holder;             // +0x18
};

struct Game_0045e5e0 {
    char unknown_0[0x519];
    Menu_0045e5e0 menu;                  // +0x519 (holder at +0x531)
    char unknown_535[0x37ebe - 0x535];
    unsigned char flags_37ebe;           // +0x37ebe
    char unknown_37ebf[0x37f06 - 0x37ebf];
    unsigned char flags_37f06;           // +0x37f06
    char unknown_37f07[1];
    int brightness;                      // +0x37f08
    char unknown_37f0c[0x37f1b - 0x37f0c];
    int width;                           // +0x37f1b
    int height;                          // +0x37f1f
};
#pragma pack(pop)

extern Game_0045e5e0* g_game;

Layer_0045e5e0* __cdecl FUN_0045cfc0();
void FUN_0045ce80();
void FUN_00428b60();
void* FUN_004d83b0(char* name, unsigned int size);
Layer_0045e5e0* __stdcall FUN_004aa8f0(Menu_0045e5e0* menu, char* name, int flags);
void __stdcall FUN_004288d0(char* name, int a, int b, int c);
void __stdcall FUN_0049fa50(Menu_0045e5e0* menu);
int __stdcall FUN_0049fdf0(Entry_0045e5e0* entries, char* name, int type);
Entry_0045e5e0* __stdcall FUN_004a0200(Entry_0045e5e0* entries, char* name);
char* __stdcall FUN_004a0180(Entry_0045e5e0* entries, char* name);
void __stdcall FUN_0045e4c0(List_0045e5e0* list);
int __stdcall FUN_004b5370(List_0045e5e0* list);
void __stdcall FUN_0045e100(void* layer);
void __stdcall FUN_0045bbf0(void* obj, int value);
void __stdcall FUN_0045bd20(void* obj, int value);
void __stdcall FUN_004a1110(Menu_0045e5e0* menu, char* name, int value);
void __stdcall FUN_004a1030(Menu_0045e5e0* menu, int index, int value);
void __stdcall FUN_004a0570(Menu_0045e5e0* menu, char* name, int value);
void __stdcall FUN_0049fb10(Menu_0045e5e0* menu, int value);
void __stdcall FUN_004a81e0(Menu_0045e5e0* menu, int value);

// FUNCTION: 0x45e5e0
void __stdcall FUN_0045e5e0(int param_1)
{
    int i;
    Layer_0045e5e0* layer;

    if (param_1 != 0) {
        layer = FUN_004aa8f0(&g_game->menu, "SELVMODE.GUI", 0x800);
    } else {
        layer = FUN_0045cfc0();
        FUN_004a81e0(&g_game->menu, 2);
        FUN_0045ce80();
        if (g_game->flags_37ebe & 1) {
            FUN_004aa8f0(&g_game->menu, "VISUALRT.GUI", 0x200);
        } else {
            FUN_004aa8f0(&g_game->menu, "VISUALS.GUI", 0x200);
            FUN_004288d0("optvisual4x", 0, 0, 0);
        }
    }
    FUN_0049fa50(&g_game->menu);
    layer->handler = FUN_0045e100;

    if (!(g_game->flags_37ebe & 1)) {
        List_0045e5e0* list = (List_0045e5e0*)FUN_004d83b0("SELECT VIDEO MODE", 0x20);
        layer->data = list;
        list->modes = (Mode_0045e5e0*)FUN_004d83b0("DISPLAY MODES", 0x4b0);
        if (FUN_004b5370(list) != 0) {
            FUN_0045e4c0(list);
            list->buffer = (char*)FUN_004d83b0("AVAILABLE MODES", list->count << 8);
            list->buffer[0] = 0;
            if (FUN_0049fdf0(layer->entries, "VIDSLDR", 0xe) != -1) {
                Entry_0045e5e0* e = FUN_004a0200(layer->entries, "VIDSLDR");
                e->max = list->count - 1;
                e->fn = FUN_0045bbf0;
                e->data = list;
                int w = g_game->width;
                for (i = 0; i < list->count; i++) {
                    if (w == list->modes[i].width
                        && g_game->height == list->modes[i].height) {
                        int max = e->max;
                        int value = i;
                        if (value > max)
                            value = max;
                        float f = (float)value / (float)max * (float)(e->steps - 1);
                        if (f - (int)f != 0.0f)
                            f += 1.0;
                        e->pos = (short)f;
                        char* p = FUN_004a0180(g_game->menu.holder->entries, "VIDVAL");
                        if (p != 0) {
                            sprintf(p + 0xb6, "%d X %d", list->modes[i].width, list->modes[i].height);
                        }
                        break;
                    }
                }
            }
        }
    }

    if (g_game->flags_37ebe & 1) {
        layer->data = 0;
        for (i = 0; i <= g_game->menu.holder->entries->count; i++) {
            if (strncmp(g_game->menu.holder->entries[i].name, "MAP", strlen("MAP")) == 0) {
                FUN_004a0570(&g_game->menu, g_game->menu.holder->entries[i].name, 0);
            }
        }
        for (i = 0; i <= g_game->menu.holder->entries->count; i++) {
            if (strncmp(g_game->menu.holder->entries[i].name, "VID", strlen("VID")) == 0) {
                FUN_004a0570(&g_game->menu, g_game->menu.holder->entries[i].name, 0);
            }
        }
    }

    if (param_1 == 0) {
        FUN_004a1110(&g_game->menu, "VISUALS", 1);
        int found = FUN_0049fdf0(layer->entries, "ANTI", 1);
        if (found != -1) {
            FUN_004a1030(&g_game->menu, found, (g_game->flags_37f06 >> 1) & 1);
        }
        found = FUN_0049fdf0(layer->entries, "BSHADOWS", 1);
        if (found != -1) {
            FUN_004a1030(&g_game->menu, found, (g_game->flags_37f06 >> 4) & 1);
        }
        found = FUN_0049fdf0(layer->entries, "SHADING", 1);
        if (found != -1) {
            FUN_004a1030(&g_game->menu, found, (g_game->flags_37f06 >> 5) & 1);
        }
        found = FUN_0049fdf0(layer->entries, "GAMMA", 0xe);
        if (found != -1) {
            Entry_0045e5e0* e = FUN_004a0200(layer->entries, "GAMMA");
            e->max = 0x14;
            e->fn = FUN_0045bd20;
            int value = g_game->brightness;
            if (value > 0x14)
                value = 0x14;
            float f = (float)value * (float)(e->steps - 1) * 0.05f;
            if (f - (int)f != 0.0f)
                f += 1.0;
            e->pos = (short)f;
        }
    }

    FUN_0049fb10(&g_game->menu, 1);
    FUN_00428b60();
    FUN_004a81e0(&g_game->menu, 0x40);
}
