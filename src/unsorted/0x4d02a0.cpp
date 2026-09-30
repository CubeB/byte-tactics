// Decompiled by deepseek-v4.1. Names are provisional.
// Earlier attempts by space-bunny-free, deepseek-v4.1-flash and GPT-6, kept below.
// Finished by deepseek-v4.1-flash: rewrote the FindChunk loop as `for (;;)` with
// an early `return size`, which removed the two extra `jmp` instructions at the
// end of the second chunk search (906 -> 897 bytes).
// Best so far: 56.9 percent, 897 bytes against the original 895, NOT a match.
// What still differs (all inside the kind == 2 RIFF body):
//   - the chunk search is scheduled differently: the original emits
//     `mov edx,[esp+0x38]; add edx,edi; push edx` for the seek argument while
//     ours emits `lea eax,[edi+edx]; push eax`, and the lea temps for &id and
//     &size rotate eax,ecx,edx in the original but ecx,edx,eax here.
//   - at the FUN_004cf940 / FUN_004cf8a0 call sites the original loads `this`
//     after the argument pushes (`mov ecx,[esp+0x2c]`), ours loads it before
//     them, and the p3/p4 loads pick ecx/edx instead of the original edx/ecx.
// Tried: shared inline FindChunk helper (current, best), plain __stdcall
// FUN_004d01b0 (56.3 percent), __thiscall free function (MSVC5 rejects with
// C4234), result/chunk locals declared both ways (slots already agree).
// NAMING NOTE: the original calls 0x4d01b0 as a __thiscall method
// (`push esi; mov ecx,edi; call 0x4d01b0`, and 0x4d01b0 never reads ecx), but
// data/symbols.csv names 0x4d01b0 as the bare `FUN_004d01b0`, so check.py
// reports our member reference as a name mismatch once the bytes match. The
// orchestrator should add the class-qualified name for 0x4d01b0 to
// data/symbols.csv.
#include <string.h>

struct File_004bb5d0;

class Class_004cf370 {
  public:
    void* FUN_004cf370(File_004bb5d0* file, int bytes, int sampleRate, int bits, int channels);
};
class Class_004cf8a0 {
  public:
    int FUN_004cf8a0(File_004bb5d0* file, int bytes, int sampleRate, int bits, int channels, int f,
                     int g);
};
class Class_004cfb40 {
  public:
    void FUN_004cf940(File_004bb5d0* file, int sampleRate, int bits, int channels, int volume);
};
class Class_004d02a0 {
  public:
    int FUN_004d02a0(char* path, int mode, int p3, int p4);
};

struct Class_004d01b0 { int FUN_004d01b0(File_004bb5d0* file); };
File_004bb5d0* __stdcall FUN_004bb5b0(char* path);
int __stdcall FUN_004bb5d0(File_004bb5d0* file);
int __stdcall FUN_004bb710(File_004bb5d0* file, int pos);
int __stdcall FUN_004bb7c0(File_004bb5d0* file, void* buf, int size);
int __stdcall FUN_004bbd00(File_004bb5d0* file);

struct WaveFormat {
    unsigned short wFormatTag;
    unsigned short nChannels;
    int nSamplesPerSec;
    int nAvgBytesPerSec;
    unsigned short nBlockAlign;
    unsigned short wBitsPerSample;
};

static inline unsigned int FindChunk(File_004bb5d0* file, const char* tag) {
    unsigned int limit, id, size, offset;
    FUN_004bb710(file, 4);
    FUN_004bb7c0(file, &limit, 4);
    limit += 8;
    FUN_004bb710(file, 12);
    FUN_004bb7c0(file, &id, 4);
    FUN_004bb7c0(file, &size, 4);
    offset = 20;
    for (;;) {
        if (strncmp((char*)&id, tag, 4) == 0)
            return size;
        FUN_004bb710(file, size + offset);
        offset += size;
        if (offset >= limit)
            return 0;
        FUN_004bb7c0(file, &id, 4);
        FUN_004bb7c0(file, &size, 4);
        offset += 8;
    }
}
// FUNCTION: 0x4d02a0
int Class_004d02a0::FUN_004d02a0(char* path, int mode, int p3, int p4) {
    int result = 0;
    File_004bb5d0* file = FUN_004bb5b0(path);
    if (file == 0)
        return result;
    int kind = ((Class_004d01b0*)this)->FUN_004d01b0(file);
    int size = FUN_004bbd00(file);
    switch (kind) {
    case 0:
        FUN_004bb710(file, 0);
        switch (mode) {
        case 0:
            result = (int)((Class_004cf370*)this)->FUN_004cf370(file, size, 0x2b11, 8, 1);
            break;
        case 1:
            result = ((Class_004cf8a0*)this)->FUN_004cf8a0(file, size, 0x2b11, 8, 1, p3, p4);
            break;
        case 2:
            ((Class_004cfb40*)this)->FUN_004cf940(file, 0x2b11, 8, 1, p3);
            return 1;
        }
        break;
    case 1: {
        unsigned int x;
        FUN_004bb710(file, 0x16);
        FUN_004bb7c0(file, &x, 4);
        if (x == 0x2af8)
            x = 0x2b11;
        FUN_004bb710(file, 0x28);
        switch (mode) {
        case 0:
            result = (int)((Class_004cf370*)this)->FUN_004cf370(file, size - 0x28, x, 8, 1);
            break;
        case 1:
            result = ((Class_004cf8a0*)this)->FUN_004cf8a0(file, size - 0x28, x, 8, 1, p3, p4);
            break;
        case 2:
            ((Class_004cfb40*)this)->FUN_004cf940(file, x, 8, 1, p3);
            return 1;
        }
        break;
    }
    case 2: {
        unsigned int len = FindChunk(file, "fmt ");
        if (len < 0x10)
            goto end;
        WaveFormat wfx;
        FUN_004bb7c0(file, &wfx, 0x10);
        int bits = wfx.wBitsPerSample;
        int chans = wfx.nChannels;
        len = FindChunk(file, "data");
        if ((int)len <= 0)
            goto end;
        switch (mode) {
        case 0:
            result = (int)((Class_004cf370*)this)
                         ->FUN_004cf370(file, len, wfx.nSamplesPerSec, bits, chans);
            break;
        case 1:
            result = ((Class_004cf8a0*)this)
                         ->FUN_004cf8a0(file, len, wfx.nSamplesPerSec, bits, chans, p3, p4);
            break;
        case 2:
            ((Class_004cfb40*)this)->FUN_004cf940(file, wfx.nSamplesPerSec, bits, chans, p3);
            return 1;
        }
        break;
    }
    }
end:
    FUN_004bb5d0(file);
    return result;
}
