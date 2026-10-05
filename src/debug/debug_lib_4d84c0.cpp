// Decompiled by deepseek-v4.1. Names are provisional.
#include <windows.h>
#include <stdlib.h>

CRITICAL_SECTION* FUN_004da780();
char FUN_004d80d0();
void* __cdecl FUN_004dba40(void* p, unsigned int size, int flags);
size_t __cdecl FUN_004d8360(void* p);
void __cdecl FUN_004da840(size_t oldSize);
void __cdecl FUN_004da7d0(unsigned int size);

extern void (*DAT_005289bc)();

// FUNCTION: 0x4d84c0
void* __cdecl FUN_004d84c0(void* param_1, unsigned int param_2)
{
    CRITICAL_SECTION* cs = FUN_004da780();
    EnterCriticalSection(cs);
    void* p;
    do {
        if (FUN_004d80d0()) {
            p = FUN_004dba40(param_1, param_2, 0);
        } else {
            size_t old = FUN_004d8360(param_1);
            if (param_1 == 0 && param_2 == 0)
                p = 0;
            else
                p = realloc(param_1, param_2);
            if (p != 0 || param_2 == 0) {
                if (param_1 != 0)
                    FUN_004da840(old);
                if (param_2 != 0)
                    FUN_004da7d0(param_2);
            }
        }
        if (p == 0 && param_2 != 0 && DAT_005289bc != 0)
            DAT_005289bc();
    } while (p == 0 && param_2 != 0 && DAT_005289bc != 0);
    LeaveCriticalSection(cs);
    return p;
}
