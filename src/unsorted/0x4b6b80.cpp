// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

extern const char DAT_00509edc[];   // "Error"

// FUNCTION: 0x4b6b80
void __stdcall FUN_004b6b80(const char* param_1, int unused)
{
    (void)unused;
    MessageBoxA(0, param_1, DAT_00509edc, 0x40000);
}
