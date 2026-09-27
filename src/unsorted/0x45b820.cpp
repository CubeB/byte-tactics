// Decompiled by Opus. Names are provisional.
#include <string.h>

extern int DAT_00512c84;
extern char DAT_00512ca8[];

// FUNCTION: 0x45b820
void __stdcall FUN_0045b820(int flag, char* text)
{
    DAT_00512c84 = flag != 0;
    if (text) {
        DAT_00512ca8[0] = 0;
        strncat(DAT_00512ca8, text, 0x3f);
    }
}
