// Decompiled by Opus. Names are provisional.
// Looks up a 0x115-byte named entry (256 of them from g_game+0x2cf3) by
// case-insensitive name; returns the entry or 0.
#include <string.h>

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

struct Entry_0049e5b0 {
    char name[0x115];
};

#pragma pack(push, 1)
struct Game_0049e5b0 {
    char unknown_0[0x2cf3];
    Entry_0049e5b0 entries[0x100];     // +0x2cf3
};
#pragma pack(pop)

extern Game_0049e5b0* g_game;

// FUNCTION: 0x49e5b0
char* __stdcall FUN_0049e5b0(char* name)
{
    if (name == 0 || strlen(name) == 0)
        return 0;
    for (int i = 0; i < 0x100; i++) {
        char* entry = g_game->entries[i].name;
        if (_strcmpi(entry, name) == 0)
            return entry;
    }
    return 0;
}
