// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <windows.h>
#include <stdlib.h>

CRITICAL_SECTION* FUN_004da780();
char FUN_004d80d0();
char FUN_004d8200();
int FUN_004d8260();
void* __cdecl FUN_004dacf0(unsigned int size, int flags);
void __cdecl FUN_004da7d0(unsigned int size);
void __cdecl FUN_004d82c0(void* p, unsigned int pattern, unsigned int n);

extern void (*DAT_005289bc)();

// FUNCTION: 0x4d83c0
void* __cdecl FUN_004d83c0(unsigned int size)
{
    CRITICAL_SECTION* cs = FUN_004da780();
    EnterCriticalSection(cs);
    void* p;
    do {
        if (FUN_004d80d0()) {
            p = FUN_004dacf0(size, 0);
        } else {
            p = malloc(size);
            if (p != 0)
                FUN_004da7d0(size);
        }
        if (p == 0 && DAT_005289bc != 0)
            DAT_005289bc();
    } while (p == 0 && DAT_005289bc != 0);
    if (p != 0) {
        if (FUN_004d8200())
            FUN_004d82c0(p, FUN_004d8260(), size);
    }
    LeaveCriticalSection(cs);
    return p;
}
