// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Tears down the sound object: releases every buffer set, disables the sound
// handle, stops and releases the streamed buffer and closes its file, shuts
// down the CD audio device, then releases the two remaining DirectSound
// objects.
#include <windows.h>
#include <dsound.h>

struct FileHandle;

class Class_004ce410 {
public:
    int open;                          // +0x0

    void CloseCdAudio();
};

int __stdcall RemoveTimer(int i);
int __stdcall HAPI_CloseFile(FileHandle* file);

class Sound {
public:
    char unknown_0[0x24];
    IDirectSoundBuffer* field_24;      // +0x24
    IDirectSoundBuffer* field_28;      // +0x28
    char unknown_2c[0x1c4 - 0x2c];
    IDirectSoundBuffer** sets[8];      // +0x1c4
    IDirectSoundBuffer* stream;        // +0x1e4
    FileHandle* file;                  // +0x1e8
    char unknown_1ec[0x288 - 0x1ec];
    int handle;                        // +0x288

    void ReleaseDirectSound();
    void ReleaseSampleSet(IDirectSoundBuffer** set);
};

// FUNCTION: 0x4ceee0
void Sound::ReleaseDirectSound()
{
    int i;
    for (i = 0; i < 8; i++) {
        if (sets[i] != 0) {
            ((Sound*)this)->ReleaseSampleSet(sets[i]);
            sets[i] = 0;
        }
    }
    if (handle != -1) {
        RemoveTimer(handle);
        handle = -1;
    }
    if (stream != 0) {
        stream->Stop();
        stream->Release();
        stream = 0;
        HAPI_CloseFile(file);
    }
    ((Class_004ce410*)this)->CloseCdAudio();
    if (field_28 != 0)
        field_28->Release();
    if (field_24 != 0)
        field_24->Release();
    field_28 = 0;
    field_24 = 0;
}
