// Decompiled by deepseek-v4.1. Names are provisional.
#include <windows.h>
#include <stdlib.h>

CRITICAL_SECTION* FUN_004da780();
char IsMemFussy();
void* __cdecl ReallocDebugBlock(void* p, unsigned int size, int flags);
size_t __cdecl GetBlockSize(void* p);
void __cdecl CountFree(size_t oldSize);
void __cdecl CountAlloc(unsigned int size);

extern void (*DAT_005289bc)();

// FUNCTION: 0x4d84c0
void* __cdecl GameRealloc(void* param_1, unsigned int param_2)
{
    CRITICAL_SECTION* cs = FUN_004da780();
    EnterCriticalSection(cs);
    void* p;
    do {
        if (IsMemFussy()) {
            p = ReallocDebugBlock(param_1, param_2, 0);
        } else {
            size_t old = GetBlockSize(param_1);
            if (param_1 == 0 && param_2 == 0)
                p = 0;
            else
                p = realloc(param_1, param_2);
            if (p != 0 || param_2 == 0) {
                if (param_1 != 0)
                    CountFree(old);
                if (param_2 != 0)
                    CountAlloc(param_2);
            }
        }
        if (p == 0 && param_2 != 0 && DAT_005289bc != 0)
            DAT_005289bc();
    } while (p == 0 && param_2 != 0 && DAT_005289bc != 0);
    LeaveCriticalSection(cs);
    return p;
}
