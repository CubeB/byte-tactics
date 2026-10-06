// Decompiled by Haiku, Sonnet and Opus. Names are provisional.

#include <windows.h>

extern char* g_loopingWav;
extern int* g_diskWav;

void* __stdcall HAPI_LoadFile(char* path, int flags);
void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x49f610
int IsWindowsSoundAvailable(void)
{
    return 1;
}

// FUNCTION: 0x49f620
void ResumeLoopingWav(void)
{
    if (g_loopingWav != 0) {
        PlaySoundA(g_loopingWav, 0, 0x15);
    }
}

// FUNCTION: 0x49f640
void StopWindowsSound()
{
    PlaySoundA(0, 0, 0x2000);
    if (g_diskWav != 0) {
        FUN_004d85a0(g_diskWav);
        g_diskWav = 0;
    }
    g_loopingWav = 0;
}

// Plays a WAV image held in memory, asynchronously (SND_ASYNC | SND_MEMORY).
// FUNCTION: 0x49f680
void __stdcall PlayWavMemory(const char* sound)
{
    PlaySoundA(sound, 0, SND_ASYNC | SND_MEMORY);
}

// FUNCTION: 0x49f6a0
void __stdcall PlayLoopingWavMemory(char* param_1)
{
    g_loopingWav = param_1;
    PlaySoundA(param_1, 0, 0xd);
}

// Loads a sound file into memory, plays it asynchronously from memory and
// frees the previously playing buffer.
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
