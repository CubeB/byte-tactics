// Decompiled by Opus. Names are provisional.
// Checks a "SQSH" header: 1 if the magic is wrong, otherwise the byte at +5
// clamped to at most 4.
#include <string.h>

// FUNCTION: 0x4d1b00
int __stdcall SquashGetPackType(unsigned char* header)
{
    if (memcmp(header, "SQSH", 4) != 0) {
        return 1;
    }
    if (header[5] >= 4) {
        return 4;
    }
    return header[5];
}
