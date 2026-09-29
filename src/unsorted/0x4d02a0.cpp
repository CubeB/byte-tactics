// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Best attempt, 55.0%. Remaining differences (first diff hunks):
//   - our `result` local lands at [esp+0x1c], the original keeps it at
//     [esp+0x14] (original: result=E+0, a=E+4, c=E+8; ours: c=E+0, a=E+4,
//     result=E+8). Declaration order was tried two ways and did not move it.
//   - the chunk-search seek argument compiles as `lea eax,[edi+edx]` here but
//     the original emits `add edx,edi; push edx`.
//   - the second "data" search emits a slightly longer block, so every label
//     after 0x4d03a9 is shifted by a few bytes.
#include <string.h>

struct File_004bb5d0;

class Class_004cf370 {
public:
    void* FUN_004cf370(File_004bb5d0* file, int bytes, int sampleRate, int bits, int channels);
};
class Class_004cf8a0 {
public:
    int FUN_004cf8a0(File_004bb5d0* file, int bytes, int sampleRate, int bits, int channels,
                     int f, int g);
};
class Class_004cfb40 {
public:
    void FUN_004cf940(File_004bb5d0* file, int sampleRate, int bits, int channels, int volume);
};
class Class_004d02a0 {
public:
    int FUN_004d01b0(File_004bb5d0* file);
    int FUN_004d02a0(char* path, int mode, int p3, int p4);
};

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

// FUNCTION: 0x4d02a0
int Class_004d02a0::FUN_004d02a0(char* path, int mode, int p3, int p4)
{
    int result = 0;
    File_004bb5d0* file = FUN_004bb5b0(path);
    if (file == 0)
        return result;
    int kind = FUN_004d01b0(file);
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
        unsigned int a, c, x, off, len;
        FUN_004bb710(file, 4);
        FUN_004bb7c0(file, &a, 4);
        a += 8;
        FUN_004bb710(file, 0xc);
        FUN_004bb7c0(file, &c, 4);
        FUN_004bb7c0(file, &x, 4);
        off = 0x14;
        while (strncmp((char*)&c, "fmt ", 4) != 0) {
            FUN_004bb710(file, x + off);
            off += x;
            if (off >= a)
                goto fail1;
            FUN_004bb7c0(file, &c, 4);
            FUN_004bb7c0(file, &x, 4);
            off += 8;
        }
        len = x;
        goto after1;
    fail1:
        len = 0;
    after1:
        if (len < 0x10)
            goto end;
        WaveFormat wfx;
        FUN_004bb7c0(file, &wfx, 0x10);
        int bits = wfx.wBitsPerSample;
        int chans = wfx.nChannels;
        FUN_004bb710(file, 4);
        FUN_004bb7c0(file, &c, 4);
        c += 8;
        FUN_004bb710(file, 0xc);
        FUN_004bb7c0(file, &a, 4);
        FUN_004bb7c0(file, &x, 4);
        off = 0x14;
        while (strncmp((char*)&a, "data", 4) != 0) {
            FUN_004bb710(file, x + off);
            off += x;
            if (off >= c)
                goto fail2;
            FUN_004bb7c0(file, &a, 4);
            FUN_004bb7c0(file, &x, 4);
            off += 8;
        }
        len = x;
        goto after2;
    fail2:
        len = 0;
    after2:
        if ((int)len <= 0)
            goto end;
        switch (mode) {
        case 0:
            result = (int)((Class_004cf370*)this)->FUN_004cf370(file, len, wfx.nSamplesPerSec,
                                                                bits, chans);
            break;
        case 1:
            result = ((Class_004cf8a0*)this)->FUN_004cf8a0(file, len, wfx.nSamplesPerSec,
                                                           bits, chans, p3, p4);
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
