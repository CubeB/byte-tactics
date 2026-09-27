// Decompiled by Opus. Names are provisional.
// Plays a sound by name: looks the name up (case-insensitively) in the
// game's 32-byte sound name table and passes its index (0xffff when not
// found) to FUN_0047f300.

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

#pragma pack(push, 1)
struct Game_0047f610 {
    char unknown_0[0x33a0f];
    int soundCount;                    // +0x33a0f
    char unknown_33a13[0x33e13 - 0x33a13];
    char soundNames[1][0x20];          // +0x33e13
};
#pragma pack(pop)

extern Game_0047f610* g_game;

void __stdcall FUN_0047f300(int index, int param_2, int param_3);

static inline int FindSound(char* name)
{
    for (int i = 0; i < g_game->soundCount; i++) {
        if (g_game->soundNames[i][0] && _strcmpi(g_game->soundNames[i], name) == 0)
            return i;
    }
    return 0xffff;
}

// FUNCTION: 0x47f610
void __stdcall FUN_0047f610(char* name, int param_2, int param_3)
{
    FUN_0047f300(FindSound(name), param_2, param_3);
}
