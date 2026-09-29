// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Streamed sound init (see 0x4cfb40 / 0x4cfbc0 / 0x4cfca0): tears down any
// previous handle and streaming buffer, then creates a new looping
// DirectSound buffer twice the size of one block, sets its volume and starts
// it. On any failure the buffer is stopped, released and its file closed.
//
// Gave up at 91.7% (501 vs 499 bytes). Everything matches except one region,
// the nAvgBytesPerSec computation, and it is ONE cause: register pressure at
// the `imul`. The original is
//     mov eax, [esp+0x44]      ; sampleRate
//     and edx, 0xffff          ; zero-extend the WORD field
//     mov ebx, [esp+0x48]      ; bits  (live across the following call)
//     imul edx, eax            ; nBlockAlign * sampleRate, dest = the dead edx
// i.e. all six registers are live there (esi this, edi &stream, ecx n, edx the
// reloaded nBlockAlign, eax sampleRate, ebx the bits argument for the later
// wBitsPerSample and this->bits), so the multiply lands on the dying edx. Here
// `mov ebx, eax / imul ebx, edx` is emitted instead: the allocator copies
// sampleRate (which is live for nSamplesPerSec) into ebx and multiplies there,
// which frees ebx and lets the `bits` load be scheduled after the imul. That
// leaves the `lea` for &wfx in eax instead of ecx and schedules
// desc.dwBufferBytes last. The bits load cannot be steered from the source:
// ~800 statement orderings were scored by an earlier model (every single- and
// two-statement move, 140 random topological permutations), and every ordering
// that yields `imul edx, eax` wrecks the earlier block (78-82%).
//
// New lead (this run): wrapping the product in a 16-bit cast DOES flip the
// multiply to `imul edx, eax`, but it flips it the wrong way. Every form of
//     wfx.nAvgBytesPerSec = (WORD)(wfx.nBlockAlign * sampleRate);
// (also (unsigned short), static_cast, and explicit `& 0xffff` or `| 0` or
// `<< 0` on the operand) reaches 499 bytes / 96.3% and emits
//     imul edx, eax / mov ebx, [esp+0x48] / ... / and edx, 0xffff
// i.e. the mask lands on the PRODUCT, after the multiply. MSVC knows the field
// is 16-bit, so it sinks the truncation past the imul. But that is
// semantically wrong for real values (44100 * 4 > 0xffff) and, crucially, the
// original zero-extends the WORD OPERAND before the imul, so a truncating
// source cannot reproduce these bytes: mask-before and mask-after are
// different instruction sequences. The truncation is a tree-order lever only,
// not the answer. The real remaining blocker is that MSVC 5 picks the
// sampleRate register as the multiply destination (forcing a copy) rather than
// the dead nBlockAlign register in edx.
//
// `wfx.nSamplesPerSec` first (as in the matched neighbours 0x4cf230 and
// 0x4cf370), a cached nBlockAlign local, an `int ba = wfx.nBlockAlign` local,
// a `WORD ba` local, pointer locals for &wfx and &desc, casts to int/long/
// DWORD/unsigned on either side, and `wfx.nAvgBytesPerSec *= ...` all leave it
// at 91.7 or worse. Inlining `(WORD)(bits / 8 * channels)` instead of reading
// the field gets 498 bytes but loses the reload (81.3%).
// `n` must be a signed int (unsigned gives `shr` instead of `sar`).
#include <windows.h>
#include <dsound.h>

struct File_004bb5d0;

int __stdcall FUN_004b64d0(int i);
int __stdcall FUN_004bb5d0(File_004bb5d0* file);

class Class_004cfb40 {
public:
    char unknown_0[0x24];
    IDirectSound* field_24;                 // +0x24
    char unknown_28[0x1e4 - 0x28];
    IDirectSoundBuffer* stream;             // +0x1e4
    File_004bb5d0* file;                    // +0x1e8
    int bits;                               // +0x1ec
    int size;                               // +0x1f0
    int pos;                                // +0x1f4
    int off;                                // +0x1f8
    char unknown_1fc[0x288 - 0x1fc];
    int handle;                             // +0x288

    void FUN_004cfca0();
    void FUN_004cf940(File_004bb5d0* file, int sampleRate, int bits,
                      int channels, LONG volume);
};

// FUNCTION: 0x4cf940
void Class_004cfb40::FUN_004cf940(File_004bb5d0* file, int sampleRate, int bits,
                                  int channels, LONG volume)
{
    WAVEFORMATEX wfx;
    DSBUFFERDESC desc;
    int n;

    if (handle != -1) {
        FUN_004b64d0(handle);
        handle = -1;
    }
    if (stream != 0) {
        stream->Stop();
        stream->Release();
        stream = 0;
        FUN_004bb5d0(this->file);
    }
    if (handle != -1) {
        FUN_004b64d0(handle);
        handle = -1;
    }

    desc.dwSize = 0x14;
    desc.dwFlags = 0x82;
    n = channels * sampleRate * (bits / 8) * 2;
    size = n / 2;
    desc.dwReserved = 0;
    wfx.wFormatTag = 1;
    wfx.nChannels = (WORD)channels;
    wfx.nBlockAlign = (WORD)(bits / 8 * channels);
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * sampleRate;
    wfx.wBitsPerSample = (WORD)bits;
    wfx.cbSize = 0x12;
    wfx.nSamplesPerSec = sampleRate;
    desc.lpwfxFormat = &wfx;
    desc.dwBufferBytes = n;

    if (field_24->CreateSoundBuffer(&desc, &stream, 0) != 0) {
        stream = 0;
        return;
    }

    this->bits = bits;
    pos = 0;
    off = -1;
    this->file = file;
    FUN_004cfca0();
    if (stream->SetVolume(volume) != 0) {
        if (handle != -1) {
            FUN_004b64d0(handle);
            handle = -1;
        }
        if (stream != 0) {
            stream->Stop();
            stream->Release();
            stream = 0;
            FUN_004bb5d0(this->file);
        }
        return;
    }
    if (stream->Play(0, 0, 1) != 0) {
        if (handle != -1) {
            FUN_004b64d0(handle);
            handle = -1;
        }
        if (stream != 0) {
            stream->Stop();
            stream->Release();
            stream = 0;
            FUN_004bb5d0(this->file);
        }
    }
}
