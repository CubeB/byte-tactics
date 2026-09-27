// Decompiled by Opus. Names are provisional.
// Releases every loaded sound set, then empties the list.
#include <windows.h>
#include <dsound.h>

#pragma pack(push, 1)
struct Game_0042f8c0 {
    char unknown_0[0x33a0f];
    int soundCount;                    // +0x33a0f
    IDirectSoundBuffer** sounds[1];    // +0x33a13
};
#pragma pack(pop)

extern Game_0042f8c0* g_game;

void FUN_0042f740();
void __stdcall FUN_0047f060(IDirectSoundBuffer** set);

// FUNCTION: 0x42f8c0
void FUN_0042f8c0()
{
    FUN_0042f740();
    for (int i = 0; i < g_game->soundCount; i++)
        FUN_0047f060(g_game->sounds[i]);
    g_game->soundCount = 0;
}
