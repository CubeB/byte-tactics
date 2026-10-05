// Decompiled by Opus. Names are provisional.
// Copies the text of the selected line of the "GAMES" list into the
// "GAMENAME" gadget of the menu embedded in the game object, then marks the
// menu for redraw.
#include <string.h>

#pragma pack(push, 1)
struct Gadget_0044b330 {
    char unknown_0[0xba];
    short selected;                    // +0xba
};
#pragma pack(pop)

struct Inner_0044b330 {
    int unknown_0;
    void* gadgets;                     // +0x4
};

struct Menu_0044b330 {
    char unknown_0[0x18];
    Inner_0044b330* inner;             // +0x18
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Menu_0044b330 menu;                // +0x519
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_005129b0;
extern char DAT_005119b8[];

Gadget_0044b330* __stdcall FUN_0049ff90(void* gadgets, char* name);
int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int flag);
char* __stdcall FUN_004b6af0(char* text, int n);
void __stdcall FUN_004a0880(Menu_0044b330* menu, int index, char* text);
void __stdcall FUN_0049fa90(Menu_0044b330* menu);

// FUNCTION: 0x44b330
void FUN_0044b330()
{
    Menu_0044b330* menu = &g_game->menu;
    void* gadgets = g_game->menu.inner->gadgets;
    Gadget_0044b330* games = FUN_0049ff90(gadgets, "GAMES");
    int index = FUN_0049fdf0(gadgets, "GAMENAME", 3);
    char* name;
    if (games->selected > -1 && (name = FUN_004b6af0(DAT_005129b0, games->selected)) != 0 && strlen(name) != 0)
        FUN_004a0880(menu, index, name);
    else
        FUN_004a0880(menu, index, DAT_005119b8);
    FUN_0049fa90(&g_game->menu);
}
