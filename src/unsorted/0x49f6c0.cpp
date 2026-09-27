// Decompiled by Opus. Names are provisional.
// Loads a sound file into memory, plays it asynchronously from memory and
// frees the previously playing buffer.
#include <windows.h>

void* __stdcall FUN_004bbe50(char* path, int flags);
void FUN_004d85a0(int* param_1);

extern int* DAT_0051fba0;

// FUNCTION: 0x49f6c0
BOOL __stdcall FUN_0049f6c0(char* path)
{
    int* sound = (int*)FUN_004bbe50(path, 0);
    if (sound == 0)
        return 0;
    BOOL result = PlaySoundA((LPCSTR)sound, 0, SND_ASYNC | SND_MEMORY);
    if (DAT_0051fba0 != 0)
        FUN_004d85a0(DAT_0051fba0);
    DAT_0051fba0 = sound;
    return result;
}
