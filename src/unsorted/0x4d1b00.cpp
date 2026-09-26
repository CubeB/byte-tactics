// Decompiled by Sonnet. Names are provisional.
#include <string.h>

extern char DAT_0050ba94[];  // "SQSH"

// FUNCTION: 0x4d1b00
unsigned char __stdcall FUN_004d1b00(char* param)
{
    if (memcmp(param, DAT_0050ba94, 4) != 0) {
        return 1;
    }

    unsigned char v = (unsigned char)param[5];
    if (v < 4) {
        return v;
    }
    return 4;
}
