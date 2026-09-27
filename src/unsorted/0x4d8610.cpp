// Decompiled by Opus. Names are provisional.
// strdup() through the game's allocator.
#include <string.h>

void* FUN_004d83c0(unsigned int size);

// FUNCTION: 0x4d8610
char* FUN_004d8610(char* s)
{
    if (!s)
        return 0;
    char* p = (char*)FUN_004d83c0(strlen(s) + 1);
    if (p)
        strcpy(p, s);
    return p;
}
