// Decompiled by Opus. Names are provisional.
// Looks up a named entry (0x20-byte names in the game object) and passes its
// index, or 0xffff when not found, to FUN_0047f0c0 with DAT_0051e69c set for
// the duration of the call. 0x47f1a0 is the same without the flag.
#include <string.h>

struct Sound_0047f210 {
    char name[0x20];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x33a0f];
    int soundCount;                    // +0x33a0f
    char unknown_33a13[0x33e13 - 0x33a13];
    Sound_0047f210 sounds[1];          // +0x33e13
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_0051e69c;

void __stdcall FUN_0047f0c0(int index, int param_2);

static inline int FindSound(char* name)
{
    for (int i = 0; i < g_game->soundCount; i++) {
        if (g_game->sounds[i].name[0] && _strcmpi(g_game->sounds[i].name, name) == 0)
            return i;
    }
    return 0xffff;
}

// FUNCTION: 0x47f210
void __stdcall FUN_0047f210(char* name, int param_2)
{
    DAT_0051e69c = 1;
    FUN_0047f0c0(FindSound(name), param_2);
    DAT_0051e69c = 0;
}
