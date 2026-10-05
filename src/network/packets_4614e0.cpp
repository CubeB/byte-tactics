// Decompiled by space-bunny-free. Names are provisional.
// Appends len bytes to the net send buffer, growing it (by at least 0x200
// bytes) when needed, and accounts the message in the byte counters. The very
// first allocation reserves 4 extra bytes and starts the buffer with the
// one dword type word -2, which FUN_004615f0 confirms by setting size to 4
// when the buffer is non null.
#include <string.h>

void __stdcall FUN_00415ef0(unsigned char type, int len, int which);

class Class_004614e0 {
public:
    char unknown_0[0xb2f4];
    unsigned char* buffer;           // +0xb2f4
    unsigned int size;               // +0xb2f8
    unsigned int capacity;           // +0xb2fc

    int FUN_004614e0(unsigned char* data, unsigned int len);
};

// FUNCTION: 0x4614e0
int Class_004614e0::FUN_004614e0(unsigned char* data, unsigned int len)
{
    if (size + len > capacity) {
        unsigned int newcap = capacity + (len > 0x200 ? len : 0x200);
        if (buffer == 0) {
            newcap += 4;
        }
        unsigned char* newbuf = new unsigned char[newcap];
        if (newbuf != 0) {
            if (buffer != 0) {
                memcpy(newbuf, buffer, size);
            } else {
                size = 4;
                *(int*)newbuf = -2;
            }
            delete[] buffer;
            buffer = newbuf;
            capacity = newcap;
        } else {
            return 0;
        }
    }
    memcpy(buffer + size, data, len);
    size += len;
    FUN_00415ef0(*data, len, 1);
    return 1;
}
