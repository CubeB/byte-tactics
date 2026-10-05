// Decompiled by Opus. Names are provisional.
// The same body as 0x44b330 (copies the selected "GAMES" line into the
// "GAMENAME" gadget and marks the menu for redraw), as a __stdcall callback
// whose two arguments are unused.
#include <string.h>

#pragma pack(push, 1)
struct Gadget_0044b600 {
    char unknown_0[0xba];
    short selected;                    // +0xba
};
#pragma pack(pop)

struct Inner_0044b600 {
    int unknown_0;
    void* gadgets;                     // +0x4
};

struct Menu_0044b600 {
    char unknown_0[0x18];
    Inner_0044b600* inner;             // +0x18
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Menu_0044b600 menu;                // +0x519
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_005129b0;
extern char DAT_005119b8[];

Gadget_0044b600* __stdcall FindGadgetChecked(void* gadgets, char* name);
int __stdcall FindGadgetIndex(void* gadgets, const char* name, int flag);
char* __stdcall FUN_004b6af0(char* text, int n);
void __stdcall SetGadgetText(Menu_0044b600* menu, int index, char* text);
void __stdcall FUN_0049fa90(Menu_0044b600* menu);

// FUNCTION: 0x44b600
void __stdcall FUN_0044b600(int unused1, int unused2)
{
    Menu_0044b600* menu = &g_game->menu;
    void* gadgets = g_game->menu.inner->gadgets;
    Gadget_0044b600* games = FindGadgetChecked(gadgets, "GAMES");
    int index = FindGadgetIndex(gadgets, "GAMENAME", 3);
    char* name;
    if (games->selected > -1 && (name = FUN_004b6af0(DAT_005129b0, games->selected)) != 0 && strlen(name) != 0)
        SetGadgetText(menu, index, name);
    else
        SetGadgetText(menu, index, DAT_005119b8);
    FUN_0049fa90(&g_game->menu);
}
