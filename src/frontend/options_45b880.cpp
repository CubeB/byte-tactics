// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_0045b880 {
    char state;                        // +0x0
    char unknown_1[1];
    char name[0x10];                   // +0x2
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6
    char unknown_b8[0x15b - 0xb8];
};

struct Holder_0045b880 {
    char unknown_0[4];
    Entry_0045b880* entries;           // +0x4
};

struct Menu_0045b880 {
    char unknown_0[0x18];
    Holder_0045b880* holder;           // +0x18
};

struct Game {
    char unknown_0[0x519];
    Menu_0045b880 menu;                // +0x519
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004a1200(Menu_0045b880* menu, int index, int value);
void __stdcall FUN_0049fa90(Menu_0045b880* menu);

// FUNCTION: 0x45b880
void __stdcall FUN_0045b880(char* name, int value)
{
    for (int i = 0; i <= g_game->menu.holder->entries->count; i++) {
        if (strncmp(g_game->menu.holder->entries[i].name, name, strlen(name)) == 0 &&
            g_game->menu.holder->entries[i].state == 1) {
            FUN_004a1200(&g_game->menu, i, value);
        }
    }
    FUN_0049fa90(&g_game->menu);
}
