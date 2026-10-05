// Decompiled by Haiku. Names are provisional.
#include <string.h>

extern char DAT_005120b8[];

// FUNCTION: 0x4287b0
void ClearPictureCache()
{
    memset(DAT_005120b8, 0, 0x64 * 4);
}
