// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

extern const char g_errorCaption[];  // "Error"

// FUNCTION: 0x4b6b80
void __stdcall ShowErrorBox(const char* param_1, int unused)
{
    (void)unused;
    MessageBoxA(0, param_1, g_errorCaption, 0x40000);
}
