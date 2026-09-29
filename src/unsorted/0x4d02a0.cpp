// Decompiled by space-bunny-free. Names are provisional.
// Opens a sound file and builds a DirectSound buffer set from it. FUN_004d01b0
// sniffs the container: 0 is a raw Cavedog file, 1 is DIGI/HSHD/SDAT (rate at
// 0x16, 11000 fixed up to 11025, data at 0x28) and 2 is RIFF/WAVE, whose fmt
// and data chunks are found by walking 8 byte chunk headers. mode 0 creates
// the buffer, mode 1 creates it and starts it, mode 2 starts an existing one
// and returns 1. Returns the buffer set, or 0.
// Still differs: the frame slot of the result variable is 4 bytes high
// (result/a/c are rotated one slot against the original), and "x + off" comes
// out as "lea eax, [edi+edx]" instead of "mov edx, [x]; add edx, edi".
#include <string.h>

class Class_004cf8a0 {
public:
    void* FUN_004cf370(void* file, int bytes, int rate, int bits, int channels);
    int FUN_004cf8a0(void* file, int bytes, int rate, int bits, int channels, int f, int g);
    int FUN_004cf940(void* file, int rate, int bits, int channels, int f);
};

class Class_004d02a0 {
public:
    int FUN_004d01b0(void* file);
    int FUN_004d02a0(char* path, int mode, int p3, int p4);
};

void* __stdcall FUN_004bb5b0(char* path);
int __stdcall FUN_004bb5d0(void* file);
int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);
int __stdcall FUN_004bbd00(void* file);

struct WaveFormat {
    unsigned short wFormatTag;
    unsigned short nChannels;
    int nSamplesPerSec;
    int nAvgBytesPerSec;
    unsigned short nBlockAlign;
    unsigned short wBitsPerSample;
};

// FUNCTION: 0x4d02a0
int Class_004d02a0::FUN_004d02a0(char* path, int mode, int p3, int p4)
{
    int result = 0;
    void* file = FUN_004bb5b0(path);
    if (file == 0)
        return result;
    int kind = FUN_004d01b0(file);
    int size = FUN_004bbd00(file);
    unsigned int x;          // sample rate, or the size of the chunk found below
    unsigned int a;
    unsigned int c;
    unsigned int off;
    switch (kind) {
    case 0:
        FUN_004bb710(file, 0);
        switch (mode) {
        case 0:
            result = (int)((Class_004cf8a0*)this)->FUN_004cf370(file, size - 0x28, x, 8, 1);
            break;
        case 1:
            result = ((Class_004cf8a0*)this)->FUN_004cf8a0(file, size, 0x2b11, 8, 1, p3, p4);
            break;
        case 2:
            ((Class_004cf8a0*)this)->FUN_004cf940(file, 0x2b11, 8, 1, p3);
            return 1;
        }
        break;
    case 1:
        FUN_004bb710(file, 0x16);
        FUN_004bb7c0(file, &x, 4);
        if (x == 0x2af8)
            x = 0x2b11;
        FUN_004bb710(file, 0x28);
        switch (mode) {
        case 0:
            result = (int)((Class_004cf8a0*)this)->FUN_004cf370(file, size - 0x28, x, 8, 1);
            break;
        case 1:
            result = ((Class_004cf8a0*)this)->FUN_004cf8a0(file, size - 0x28, x, 8, 1, p3, p4);
            break;
        case 2:
            ((Class_004cf8a0*)this)->FUN_004cf940(file, x, 8, 1, p3);
            return 1;
        }
        break;
    case 2: {
        FUN_004bb710(file, 4);
        FUN_004bb7c0(file, &a, 4);
        a = a + 8;
        FUN_004bb710(file, 0xc);
        FUN_004bb7c0(file, &c, 4);
        FUN_004bb7c0(file, &x, 4);
        off = 0x14;
        int r = 0;
        for (; strncmp((char*)&c, "fmt ", 4) != 0; ) {
            FUN_004bb710(file, x + off);
            off += x;
            if (off >= c)
                break;
            FUN_004bb7c0(file, &c, 4);
            FUN_004bb7c0(file, &x, 4);
            off += 8;
        }
        r = x;
        if ((unsigned)r < 0x10)
            goto end;
        WaveFormat wfx;
        FUN_004bb7c0(file, &wfx, 0x10);
        int bits = wfx.wBitsPerSample;
        int chans = wfx.nChannels;
        FUN_004bb710(file, 4);
        FUN_004bb7c0(file, &c, 4);
        c = c + 8;
        FUN_004bb710(file, 0xc);
        FUN_004bb7c0(file, &a, 4);
        FUN_004bb7c0(file, &x, 4);
        off = 0x14;
        r = 0;
        for (; strncmp((char*)&a, "data", 4) != 0; ) {
            FUN_004bb710(file, x + off);
            off += x;
            if (off >= c)
                break;
            FUN_004bb7c0(file, &a, 4);
            FUN_004bb7c0(file, &x, 4);
            off += 8;
        }
        r = x;
        if (r <= 0)
            goto end;
        switch (mode) {
        case 0:
            result = (int)((Class_004cf8a0*)this)->FUN_004cf370(file, r, wfx.nSamplesPerSec, bits, chans);
            break;
        case 1:
            result = ((Class_004cf8a0*)this)->FUN_004cf8a0(file, r, wfx.nSamplesPerSec, bits, chans, p3, p4);
            break;
        case 2:
            ((Class_004cf8a0*)this)->FUN_004cf940(file, wfx.nSamplesPerSec, bits, chans, p3);
            return 1;
        }
        break;
    }
    }
end:
    FUN_004bb5d0(file);
    return result;
}
