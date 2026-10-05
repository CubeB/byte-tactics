// Decompiled by Opus. Names are provisional.
// Copies the 32-character name of entry `index` of the table at
// g_game+0x1439b (0x249-byte entries) into `dest`. <windows.h> fixes the
// operand order of the address lea (tools/headers.py).
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_004195f0 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x249 - 0x40];
};

struct Game {
    char unknown_0[0x1439b];
    Entry_004195f0* entries;           // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x4195f0
void __stdcall FUN_004195f0(char* dest, unsigned short index)
{
    strncpy(dest, g_game->entries[index].name, 0x20);
    dest[0x1f] = 0;
}
