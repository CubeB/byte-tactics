// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_0045ba60 {
    char unknown_0[0xb6];
    char text[0x80];                 // +0xb6
};

struct Layer_0045ba60 {
    char unknown_0[4];
    Entry_0045ba60* entries;         // +0x4
};

struct Menu_0045ba60 {
    char unknown_0[0x18];
    Layer_0045ba60* layer;           // +0x18
};

struct Game {
    char unknown_0[0x519];
    Menu_0045ba60 menu;              // +0x519
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004a11c0(Menu_0045ba60* menu, int index, short value);
void __stdcall FUN_0049fed0(Entry_0045ba60* entries, char* name, int index);
Entry_0045ba60* __stdcall FUN_0049ff10(Entry_0045ba60* entries, char* name);

// FUNCTION: 0x45ba60
void __stdcall FUN_0045ba60(int index, int state, char* offText, char* onText)
{
    char name[128];
    Entry_0045ba60* entries = g_game->menu.layer->entries;
    FUN_004a11c0(&g_game->menu, index, state);
    FUN_0049fed0(entries, name, index);
    Entry_0045ba60* e = FUN_0049ff10(entries, name);
    // An if/else of two strcpy calls; a ternary argument places the
    // destination lea after the branch instead of before it.
    if (!state) {
        strcpy(e->text, offText);
    } else {
        strcpy(e->text, onText);
    }
}
