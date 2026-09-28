// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
#include <dsound.h>

class Class_004cef90 {
public:
    char unknown_0[0x24];
    IDirectSound* field_24;             // +0x24
    IDirectSoundBuffer* field_28;       // +0x28
    char unknown_2c[0x1b8 - 0x2c];
    int field_1b8;                      // +0x1b8
    int field_1bc;                      // +0x1bc
    int field_1c0;                      // +0x1c0
    char unknown_1c4[0x290 - 0x1c4];
    int field_290;                      // +0x290

    int FUN_004cef90(int rate, int bits, int channels, HWND handle);
    void FUN_004ceee0();
};

// FUNCTION: 0x4cef90
int Class_004cef90::FUN_004cef90(int rate, int bits, int channels, HWND handle)
{
    WAVEFORMATEX wfx;
    DSBUFFERDESC desc;
    HRESULT hr;

    if (field_24 == 0) {
        hr = DirectSoundCreate(0, &field_24, 0);
        if (hr != 0)
            goto error;
        hr = field_24->SetCooperativeLevel(handle, 2);
        if (hr != 0)
            goto error;
        desc.dwSize = sizeof(desc);
        desc.dwFlags = 1;
        desc.dwBufferBytes = 0;
        desc.dwReserved = 0;
        desc.lpwfxFormat = 0;
        hr = field_24->CreateSoundBuffer(&desc, &field_28, 0);
        if (hr != 0)
            goto error;
    }
    wfx.wFormatTag = 1;
    wfx.nChannels = (WORD)channels;
    wfx.nSamplesPerSec = rate;
    wfx.nBlockAlign = (WORD)(bits / 8 * channels);
    wfx.nAvgBytesPerSec = rate * wfx.nBlockAlign;
    wfx.wBitsPerSample = (WORD)bits;
    wfx.cbSize = 0x12;
    hr = field_28->SetFormat(&wfx);
    if (hr != 0)
        goto error;
    field_1c0 = channels;
    field_1b8 = rate;
    field_1bc = bits;
    return 1;
error:
    if (hr == (HRESULT)0x88780078)
        field_290 = 1;
    FUN_004ceee0();
    return 0;
}
