// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Creates a DirectSound buffer from a block of raw sample data, locks it,
// memcpy's the data in and unlocks it, then wraps the buffer in a 4-slot set
// (only slot 0 is used) allocated with the tagged allocator. Releases the
// buffer and returns 0 on any failure. The Lock outputs must be separate
// `void* ptr; DWORD size;` locals (MSVC coalesces them onto the dead `src`
// and `channels` parameter slots); passing &channels/&bytes makes the
// parameters address-taken and stops `bytes` from staying in ebp.
#include <windows.h>
#include <dsound.h>
#include <string.h>

void* FUN_004d83b0(char* name, unsigned int size);

class Class_004cf230 {
public:
    char unknown_0[0x24];
    IDirectSound* field_24;                 // +0x24

    IDirectSoundBuffer** FUN_004cf230(void* src, DWORD bytes,
                                      int sampleRate, int bits, int channels);
};

// FUNCTION: 0x4cf230
IDirectSoundBuffer** Class_004cf230::FUN_004cf230(void* src, DWORD bytes,
                                                  int sampleRate, int bits, int channels)
{
    WAVEFORMATEX wfx;
    DSBUFFERDESC desc;
    IDirectSoundBuffer* buf = 0;

    desc.dwReserved = 0;
    desc.lpwfxFormat = &wfx;
    desc.dwSize = 0x14;
    desc.dwFlags = 0x92;
    desc.dwBufferBytes = bytes;
    wfx.wFormatTag = 1;
    wfx.nChannels = (WORD)channels;
    wfx.nSamplesPerSec = sampleRate;
    wfx.nBlockAlign = (WORD)(bits / 8 * channels);
    wfx.nAvgBytesPerSec = sampleRate * wfx.nBlockAlign;
    wfx.wBitsPerSample = (WORD)bits;
    wfx.cbSize = 0x12;

    if (field_24->CreateSoundBuffer(&desc, &buf, 0) != 0)
        goto error;

    {
        void* ptr;
        DWORD size;
        if (buf->Lock(0, bytes, &ptr, &size, 0, 0, 0) != 0)
            goto error;
        memcpy(ptr, src, size);
        if (buf->Unlock(ptr, size, 0, 0) != 0)
            goto error;
    }

    {
        IDirectSoundBuffer** set = (IDirectSoundBuffer**)FUN_004d83b0("Digital Audio Sample", 0x10);
        for (int i = 0; i < 4; i++)
            set[i] = 0;
        set[0] = buf;
        return set;
    }

error:
    if (buf != 0)
        buf->Release();
    return 0;
}
