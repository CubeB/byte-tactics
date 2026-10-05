// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <windows.h>
#include <stdlib.h>

CRITICAL_SECTION* FUN_004da780();
char IsMemFussy();
char FUN_004d8200();
int GetMemSetValue();
void* __cdecl AllocDebugBlock(unsigned int size, int flags);
void __cdecl CountAlloc(unsigned int size);
void __cdecl FillPattern(void* p, unsigned int pattern, unsigned int n);

extern void (*DAT_005289bc)();

// FUNCTION: 0x4d83c0
void* __cdecl GameAlloc(unsigned int size)
{
    CRITICAL_SECTION* cs = FUN_004da780();
    EnterCriticalSection(cs);
    void* p;
    do {
        if (IsMemFussy()) {
            p = AllocDebugBlock(size, 0);
        } else {
            p = malloc(size);
            if (p != 0)
                CountAlloc(size);
        }
        if (p == 0 && DAT_005289bc != 0)
            DAT_005289bc();
    } while (p == 0 && DAT_005289bc != 0);
    if (p != 0) {
        if (FUN_004d8200())
            FillPattern(p, GetMemSetValue(), size);
    }
    LeaveCriticalSection(cs);
    return p;
}
