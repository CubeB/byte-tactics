// Decompiled by Opus. Names are provisional.
// Passes a value through DebugFunc1 of DebugHelper.dll when that DLL exists.
#include <windows.h>

typedef int (__stdcall* DebugFunc_004cbb70)(int);

// FUNCTION: 0x4cbb70
int __stdcall PassThroughDebugHelper(int value)
{
    HMODULE lib = LoadLibraryA("DebugHelper.dll");
    if (lib != 0) {
        DebugFunc_004cbb70 func = (DebugFunc_004cbb70)GetProcAddress(lib, "DebugFunc1");
        if (func != 0) {
            value = func(value);
        } else {
            OutputDebugStringA("Couldn't find function.\n");
        }
        FreeLibrary(lib);
    } else {
        OutputDebugStringA("Couldn't find library.\n");
    }
    return value;
}
