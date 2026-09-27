// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Builds "<name><n>.GUI" for entry `index` of the table at g_game+0x1439b
// (0x249-byte entries) into `dest`. <windows.h> fixes the operand order of
// the address lea (tools/headers.py).
#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_0041b230 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x249 - 0x40];
};

struct Game_0041b230 {
    char unknown_0[0x1439b];
    Entry_0041b230* entries;           // +0x1439b
};
#pragma pack(pop)

extern Game_0041b230* g_game;

// FUNCTION: 0x41b230
void __stdcall FUN_0041b230(char* dest, unsigned short index, int n)
{
    char name[256];
    strncpy(name, g_game->entries[index].name, 0x20);
    name[0x1f] = 0;
    sprintf(dest, "%s%d.GUI", name, n);
}
