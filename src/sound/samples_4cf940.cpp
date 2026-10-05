// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Streamed sound init (see 0x4cfb40 / 0x4cfbc0 / 0x4cfca0): tears down any
// previous handle and streaming buffer, then creates a new looping
// DirectSound buffer twice the size of one block, sets its volume and starts
// it. On any failure the buffer is stopped, released and its file closed.
//
// MATCH (499 bytes, all 8 linker references check out).
//
// The last 2 percent was the SOURCE ORDER of the ten desc/wfx field
// assignments, and only their order: every spelling, cast and statement
// grouping is the obvious one, but the store scheduler sinks
// `desc.dwBufferBytes = n;` to the end of the block unless that statement
// sits in the first half of the group. Then ecx still holds n when the
// scheduler needs a register for `lea <&wfx>`, so the lea takes eax, ds gets
// reloaded after it, and the buffer-bytes store lands after the two word
// stores. Writing dwBufferBytes second (after dwReserved, before
// lpwfxFormat) puts the store back where the original has it and frees ecx
// in time. Five different orders of these ten statements all match, so the
// original's own order is not recoverable, only a member of the class.
//
// What the earlier model established, still true: adding `#include <stdio.h>`
// (time.h, string.h, stdlib.h, math.h, mmsystem.h do the same) flips the
// multiply from `mov ebx,eax / imul ebx,edx` to the original's
// `and edx,0xffff / mov ebx,[esp+0x48] / imul edx,eax`, so the multiply's
// destination register was decided by compiler/header state, not by the
// expression. malloc.h and io.h give the 501-byte form instead.
// `(WORD)(sampleRate * wfx.nBlockAlign)` reaches 96.3% by moving the 0xffff
// mask after the imul, but the original zero-extends the operand first.
// n must be a signed int (unsigned gives shr instead of sar).
#include <windows.h>
#include <stdio.h>
#include <dsound.h>

struct FileHandle;

int __stdcall RemoveTimer(int i);
int __stdcall HAPI_CloseFile(FileHandle* file);

class Sound {
public:
    char unknown_0[0x24];
    IDirectSound* field_24;                 // +0x24
    char unknown_28[0x1e4 - 0x28];
    IDirectSoundBuffer* stream;             // +0x1e4
    FileHandle* file;                       // +0x1e8
    int bits;                               // +0x1ec
    int size;                               // +0x1f0
    int pos;                                // +0x1f4
    int off;                                // +0x1f8
    char unknown_1fc[0x288 - 0x1fc];
    int handle;                             // +0x288

    void FillStreamHalf();
    void StartStream(FileHandle* file, int sampleRate, int bits,
                      int channels, LONG volume);
};

// FUNCTION: 0x4cf940
void Sound::StartStream(FileHandle* file, int sampleRate, int bits,
                                  int channels, LONG volume)
{
    WAVEFORMATEX wfx;
    DSBUFFERDESC desc;
    int n;

    if (handle != -1) {
        RemoveTimer(handle);
        handle = -1;
    }
    if (stream != 0) {
        stream->Stop();
        stream->Release();
        stream = 0;
        HAPI_CloseFile(this->file);
    }
    if (handle != -1) {
        RemoveTimer(handle);
        handle = -1;
    }

    desc.dwSize = 0x14;
    desc.dwFlags = 0x82;
    n = channels * sampleRate * (bits / 8) * 2;
    size = n / 2;
    desc.dwReserved = 0;
    desc.dwBufferBytes = n;
    desc.lpwfxFormat = &wfx;
    wfx.wFormatTag = 1;
    wfx.nChannels = (WORD)channels;
    wfx.nSamplesPerSec = sampleRate;
    wfx.nBlockAlign = (WORD)(bits / 8 * channels);
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * sampleRate;
    wfx.wBitsPerSample = (WORD)bits;
    wfx.cbSize = 0x12;

    if (field_24->CreateSoundBuffer(&desc, &stream, 0) != 0) {
        stream = 0;
        return;
    }

    this->bits = bits;
    pos = 0;
    off = -1;
    this->file = file;
    FillStreamHalf();
    if (stream->SetVolume(volume) != 0) {
        if (handle != -1) {
            RemoveTimer(handle);
            handle = -1;
        }
        if (stream != 0) {
            stream->Stop();
            stream->Release();
            stream = 0;
            HAPI_CloseFile(this->file);
        }
        return;
    }
    if (stream->Play(0, 0, 1) != 0) {
        if (handle != -1) {
            RemoveTimer(handle);
            handle = -1;
        }
        if (stream != 0) {
            stream->Stop();
            stream->Release();
            stream = 0;
            HAPI_CloseFile(this->file);
        }
    }
}
