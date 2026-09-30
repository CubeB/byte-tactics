// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <string.h>

void __cdecl FUN_004d83a0(int);

// FUNCTION: 0x41d8c0
void* __stdcall FUN_0041d8c0(unsigned int size)
{
    unsigned int pad = GetTickCount() % 1000 * 7;
    unsigned int total = pad + size;
    char* p = (char*)operator new(total);
    memset(p, 0, total);
    FUN_004d83a0((int)p);
    return p + pad;
}
