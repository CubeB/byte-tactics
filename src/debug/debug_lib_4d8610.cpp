// Decompiled by Opus. Names are provisional.
// strdup() through the game's allocator.
#include <string.h>

void* __cdecl GameAlloc(unsigned int size);

// FUNCTION: 0x4d8610
char* __cdecl GameStrdup(char* s)
{
    if (!s)
        return 0;
    char* p = (char*)GameAlloc(strlen(s) + 1);
    if (p)
        strcpy(p, s);
    return p;
}
