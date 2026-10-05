// Decompiled by Opus. Names are provisional.
// Plays a WAV image held in memory, asynchronously (SND_ASYNC | SND_MEMORY).
#include <windows.h>
#include <mmsystem.h>

// FUNCTION: 0x49f680
void __stdcall PlayWavMemory(const char* sound)
{
    PlaySoundA(sound, 0, SND_ASYNC | SND_MEMORY);
}
