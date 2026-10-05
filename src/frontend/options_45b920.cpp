// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_0045b920 {                // 0x15b bytes
    short unknown_0;
    char name[16];                     // +0x2
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6 (used in entry 0)
    char unknown_b8[0x15b - 0xb8];
};

struct Data_0045b920 {
    int unknown_0;
    Entry_0045b920* entries;           // +0x4
};

struct Menu_0045b920 {
    char unknown_0[0x18];
    Data_0045b920* data;               // +0x18
};

struct Game {
    char unknown_0[0x519];
    Menu_0045b920 menu;                // +0x519
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004a0570(Menu_0045b920* menu, char* name, int value);

// FUNCTION: 0x45b920
void __stdcall FUN_0045b920(char* prefix)
{
    for (int i = 0; i <= g_game->menu.data->entries[0].count; i++) {
        if (strncmp(g_game->menu.data->entries[i].name, prefix, strlen(prefix)) == 0) {
            FUN_004a0570(&g_game->menu, g_game->menu.data->entries[i].name, 0);
        }
    }
}
