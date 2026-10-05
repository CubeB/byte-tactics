// Decompiled by Opus. Names are provisional.
// Loads a sound file into memory, plays it asynchronously from memory and
// frees the previously playing buffer.
#include <windows.h>

void* __stdcall HAPI_LoadFile(char* path, int flags);
void __cdecl FUN_004d85a0(int* param_1);

extern int* g_diskWav;

// FUNCTION: 0x49f6c0
BOOL __stdcall PlayWavFromDisk(char* path)
{
    int* sound = (int*)HAPI_LoadFile(path, 0);
    if (sound == 0)
        return 0;
    BOOL result = PlaySoundA((LPCSTR)sound, 0, SND_ASYNC | SND_MEMORY);
    if (g_diskWav != 0)
        FUN_004d85a0(g_diskWav);
    g_diskWav = sound;
    return result;
}
