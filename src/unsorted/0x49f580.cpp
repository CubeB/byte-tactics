// Decompiled by Haiku. Names are provisional.
#include <string.h>

extern const char DAT_0051fb50[];

// FUNCTION: 0x49f580
int FUN_0049f580(void)
{
    int len = strlen(DAT_0051fb50);
    int result = (0 == len) ? 0 : -1;
    return result & (int)(const void*)DAT_0051fb50;
}
