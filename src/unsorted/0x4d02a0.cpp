// Decompiled by GPT-6.1-sol. Names are provisional.
// Retry note: best verified source scores 31.7%. The main remaining difference is
// the parsed header/chunk state and its argument selection; branch dispatch and
// MSVC local/register allocation still differ substantially from the original.
#include <stdio.h>
#include <string.h>

struct File_004d02a0 { FILE* fp; void* shared; void* info; unsigned int pos; int* buffer; int* buffer2; };
int __stdcall FUN_004bb710(File_004d02a0*, long);
int __stdcall FUN_004bb7c0(File_004d02a0*, void*, int);
int __stdcall FUN_004bb5d0(File_004d02a0*);
File_004d02a0* __stdcall FUN_004bb5b0(char*);
int __stdcall FUN_004d01b0(File_004d02a0*);
long __stdcall FUN_004bbd00(File_004d02a0*);

class Class_004cfb40 {
public:
    int FUN_004cf940(File_004d02a0*, int, int, int, int);
    int FUN_004cf8a0(File_004d02a0*, int, int, int, int, int, int);
    int FUN_004cf370(File_004d02a0*, int, int, int, int);
};
class Class_004d02a0 { public: int FUN_004d02a0(int, int, int, char*); };

// FUNCTION: 0x4d02a0
int Class_004d02a0::FUN_004d02a0(int a, int b, int c, char* path)
{
    int result = 0;
    File_004d02a0* file = FUN_004bb5b0(path);
    if (!file) return 0;
    int type = FUN_004d01b0(file);
    int length = FUN_004bbd00(file);
    if (type == 0) {
        FUN_004bb710(file, 0);
        if (b == 0)
            result = ((Class_004cfb40*)this)->FUN_004cf370(file, 0x2b11, length, 8, 1);
        else if (b == 1)
            result = ((Class_004cfb40*)this)->FUN_004cf8a0(file, length, 0x2b11, 8, 1, c, a);
        else if (b == 2)
            result = ((Class_004cfb40*)this)->FUN_004cf940(file, length, 0x2b11, 8, 1);
    } else if (type == 1) {
        int rate;
        FUN_004bb710(file, 0x16);
        FUN_004bb7c0(file, &rate, 4);
        if (rate == 0x2af8) rate = 0x2b11;
        FUN_004bb710(file, 0x28);
        if (b == 0)
            result = ((Class_004cfb40*)this)->FUN_004cf370(file, rate, 0x2b11, 8, 1);
        else if (b == 1)
            result = ((Class_004cfb40*)this)->FUN_004cf940(file, rate, 8, 1, c);
        else if (b == 2)
            result = ((Class_004cfb40*)this)->FUN_004cf8a0(file, rate, 8, 1, c, a, b);
    } else if (type == 2) {
        int riffSize;
        FUN_004bb710(file, 4);
        FUN_004bb7c0(file, &riffSize, 4);
        int offset = 12;
        FUN_004bb710(file, offset);
        int chunkSize;
        FUN_004bb7c0(file, &chunkSize, 4);
        char id[4];
        FUN_004bb7c0(file, id, 4);
        while (strncmp(id, "fmt ", 4) != 0) {
            FUN_004bb710(file, offset + chunkSize);
            FUN_004bb7c0(file, &chunkSize, 4);
            FUN_004bb7c0(file, id, 4);
            if ((unsigned)(offset + chunkSize) >= (unsigned)riffSize) { chunkSize = 0; break; }
            offset += chunkSize + 8;
        }
        if (chunkSize >= 0x10) {
            unsigned char fmt[16];
            FUN_004bb7c0(file, fmt, 0x10);
            int format = *(unsigned short*)fmt;
            int channels = *(unsigned short*)(fmt + 2);
            int rate = *(int*)(fmt + 4);
            int bits = *(unsigned short*)(fmt + 14);
            FUN_004bb710(file, 4);
            FUN_004bb7c0(file, &riffSize, 4);
            offset = 12;
            FUN_004bb710(file, offset);
            FUN_004bb7c0(file, &chunkSize, 4);
            FUN_004bb7c0(file, id, 4);
            while (strncmp(id, "data", 4) != 0) {
                FUN_004bb710(file, offset + chunkSize);
                FUN_004bb7c0(file, &chunkSize, 4);
                FUN_004bb7c0(file, id, 4);
                if ((unsigned)(offset + chunkSize) >= (unsigned)riffSize) { chunkSize = 0; break; }
                offset += chunkSize + 8;
            }
            if (chunkSize > 0) {
                if (format == 0)
                    result = ((Class_004cfb40*)this)->FUN_004cf940(file, chunkSize, channels, rate, bits);
                else if (format == 1)
                    result = ((Class_004cfb40*)this)->FUN_004cf8a0(file, chunkSize, channels, rate, bits, c, a);
                else if (format == 2)
                    result = ((Class_004cfb40*)this)->FUN_004cf940(file, chunkSize, channels, rate, bits);
            }
        }
    }
    FUN_004bb5d0(file);
    return result;
}
