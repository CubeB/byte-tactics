// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Creates a DirectSound buffer for a block of a sound file. FP exceptions are
// masked while the buffer is created, the buffer is locked, its contents are
// read from the file with HAPI_readfromfile, and it is unlocked again. The buffer
// is then wrapped in a 4-slot set (only slot 0 is used) allocated with the
// tagged allocator. Releases the buffer and returns 0 on any failure.
#include <windows.h>
#include <dsound.h>
#include <float.h>

struct FileHandle;

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
int __stdcall HAPI_readfromfile(FileHandle* file, void* buf, int size);

class Sound {
public:
    char unknown_0[0x24];
    IDirectSound* field_24;                 // +0x24

    IDirectSoundBuffer** CreateSampleFromFile(FileHandle* file, DWORD bytes,
                                      int sampleRate, int bits, int channels);
};

// FUNCTION: 0x4cf370
IDirectSoundBuffer** Sound::CreateSampleFromFile(FileHandle* file, DWORD bytes,
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

    unsigned int oldcw = _controlfp(0, 0);
    _controlfp(0x18, 0x18);
    HRESULT hr = field_24->CreateSoundBuffer(&desc, &buf, 0);
    _controlfp(oldcw, 0x18);
    if (hr != 0)
        goto error;

    {
        void* ptr;
        DWORD size;
        hr = buf->Lock(0, bytes, &ptr, &size, 0, 0, 0);
        if (hr != 0)
            goto error;
        unsigned copied = (unsigned)HAPI_readfromfile(file, ptr, size);
        hr = buf->Unlock(ptr, size, 0, 0);
        if (hr != 0)
            goto error;
        if (copied < size)
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
