// Decompiled by Opus. Names are provisional.
// For each of the 10 slots at +0x38dd9 that is in use, sets the menu entry
// named "<prefix><slot>" to 1.
#include <windows.h>

struct Menu_0041f5c0 {
    char unknown_0[0x1c];
};

#pragma pack(push, 1)
struct Slot_0041f5c0 {                 // 0x3a bytes
    char used;                         // +0x0
    char unknown_1[0x3a - 1];
};

struct Game_0041f5c0 {
    char unknown_0[0x519];
    Menu_0041f5c0 menu;                // +0x519
    char unknown_535[0x38dd9 - 0x519 - sizeof(Menu_0041f5c0)];
    Slot_0041f5c0 slots[10];           // +0x38dd9
};
#pragma pack(pop)

extern Game_0041f5c0* g_game;

void __stdcall FUN_004a0570(Menu_0041f5c0* menu, char* name, int value);

// FUNCTION: 0x41f5c0
void __stdcall FUN_0041f5c0(char* prefix)
{
    char name[64];
    for (int i = 0; i < 10; i++) {
        if (g_game->slots[i].used) {
            wsprintfA(name, "%s%d", prefix, i);
            FUN_004a0570(&g_game->menu, name, 1);
        }
    }
}
