// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Looks up a sound by name in the game's 32-byte sound name table and passes
// its index (0xffff when not found) to PlaySoundByIndex.

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x33a0f];
    int soundCount;                    // +0x33a0f
    char unknown_33a13[0x33e13 - 0x33a13];
    char soundNames[1][0x20];          // +0x33e13
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall PlaySoundByIndex(int index, int param_2);

static inline int FindSound(char* name)
{
    for (int i = 0; i < g_game->soundCount; i++) {
        if (g_game->soundNames[i][0] && _strcmpi(g_game->soundNames[i], name) == 0)
            return i;
    }
    return 0xffff;
}

// FUNCTION: 0x47f1a0
void __stdcall PlaySoundByName(char* name, int param_2)
{
    PlaySoundByIndex(FindSound(name), param_2);
}
