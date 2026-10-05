// Decompiled by Opus. Names are provisional.
// Releases a set of sound buffers: frees the set directly when
// DAT_0051e694 is set, otherwise through the sound object's FUN_004cf4d0.
#include <windows.h>
#include <dsound.h>

void __cdecl FUN_004d85a0(int* param_1);

class Class_004cf4d0 {
public:
    void FUN_004cf4d0(IDirectSoundBuffer** set);
};

struct Game {
    char unknown_0[0x10];
    Class_004cf4d0* sound;             // +0x10
};

extern Game* g_game;
extern int DAT_0051e694;

// FUNCTION: 0x47f060
void __stdcall FUN_0047f060(IDirectSoundBuffer** set)
{
    if (DAT_0051e694 != 0) {
        FUN_004d85a0((int*)set);
        return;
    }
    g_game->sound->FUN_004cf4d0(set);
}
