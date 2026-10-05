// Decompiled by Opus. Names are provisional.
// Releases a set of sound buffers: frees the set directly when
// g_useWindowsSound is set, otherwise through the sound object's ReleaseSampleSet.
#include <windows.h>
#include <dsound.h>

void __cdecl FUN_004d85a0(int* param_1);

class Sound {
public:
    void ReleaseSampleSet(IDirectSoundBuffer** set);
};

struct Game {
    char unknown_0[0x10];
    Sound* sound;                      // +0x10
};

extern Game* g_game;
extern int g_useWindowsSound;

// FUNCTION: 0x47f060
void __stdcall FreeSoundSet(IDirectSoundBuffer** set)
{
    if (g_useWindowsSound != 0) {
        FUN_004d85a0((int*)set);
        return;
    }
    g_game->sound->ReleaseSampleSet(set);
}
