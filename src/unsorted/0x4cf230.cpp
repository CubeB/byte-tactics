// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Still differs (best 65.5%). MSVC keeps the shared zero in ebp and the byte
// count in a caller-saved register (it reloads it for Lock); the original puts
// the initial zero in eax (which the &wfx lea then clobbers, so the later 0
// arguments are immediates) and keeps the byte count in ebp. The original also
// does not zero the whole DSBUFFERDESC first (that memset is a scheduling
// probe, it over-zeroes and adds stores); removing it costs ~2%. Everything
// else, the CreateSoundBuffer/Lock/memcpy/Unlock sequence and the wrap-up
// allocation, is in place. 0x4cf370 in the same module has the identical
// prologue and is unmatched for the same reason.
#include <windows.h>
#include <dsound.h>
#include <string.h>
void* FUN_004d83b0(char* name, unsigned int size);
class Class_004cf230 {
public:
    char unknown_0[0x24];
    IDirectSound* field_24;
    IDirectSoundBuffer** FUN_004cf230(void* src, DWORD bytes, int sampleRate, int bits, int channels);
};
// FUNCTION: 0x4cf230
IDirectSoundBuffer** Class_004cf230::FUN_004cf230(void* src, DWORD bytes, int sampleRate, int bits, int channels)
{
    IDirectSoundBuffer* buf = 0;
    WAVEFORMATEX wfx;
    DSBUFFERDESC desc;
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = 0x14;
    desc.dwFlags = 0x92;
    desc.dwBufferBytes = bytes;
    desc.lpwfxFormat = &wfx;
    wfx.wFormatTag = 1;
    wfx.nChannels = (WORD)channels;
    wfx.nSamplesPerSec = sampleRate;
    wfx.nBlockAlign = (WORD)(bits / 8 * channels);
    wfx.nAvgBytesPerSec = sampleRate * wfx.nBlockAlign;
    wfx.wBitsPerSample = (WORD)bits;
    wfx.cbSize = 0x12;
    if (field_24->CreateSoundBuffer(&desc, &buf, 0) != 0) goto error;
    if (buf->Lock(0, bytes, (LPVOID*)&channels, &bytes, 0, 0, 0) != 0) goto error;
    memcpy((void*)channels, src, bytes);
    if (buf->Unlock((void*)channels, bytes, 0, 0) == 0) {
        IDirectSoundBuffer** set = (IDirectSoundBuffer**)FUN_004d83b0("Digital Audio Sample", 0x10);
        set[0] = 0;
        set[1] = 0;
        set[2] = 0;
        set[3] = 0;
        set[0] = buf;
        return set;
    }
error:
    if (buf != 0) buf->Release();
    return 0;
}
