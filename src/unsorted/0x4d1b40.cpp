// Decompiled by Opus. Names are provisional.
// Returns the dword at +0xb of a "SQSH" header (see FUN_004d1b00), or 0 when
// the magic is wrong.
#include <string.h>

// FUNCTION: 0x4d1b40
int __stdcall FUN_004d1b40(unsigned char* header)
{
    if (memcmp(header, "SQSH", 4) != 0) {
        return 0;
    }
    return *(int*)(header + 0xb);
}
