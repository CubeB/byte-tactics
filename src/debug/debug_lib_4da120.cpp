// Decompiled by Claude Opus 5.5. Names are provisional.
// FLAGS: /Od /Gy
//
// Cavedog's debug helpers, compiled without optimisation: every value goes
// through its local, and the linker pads after each function with int3.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

char* __cdecl FindCommandLineSwitch(const char* option);
void AbortProgram(void);
void __stdcall UnhandledExceptionHandler(EXCEPTION_POINTERS* exception);
void FUN_004df160(void);
DWORD __stdcall DebugThreadProc(void* param);

extern unsigned char DAT_005289c8;      // the debug thread is running
extern unsigned char DAT_005289cc;      // InitDebugSupport has run

// FUNCTION: 0x4da120
void __cdecl ShowDebugMessage(const char* message)
{
    OutputDebugStringA(message);
    MessageBoxA(0, message, "Cavedog", 0);
}

// Hands `value` to DebugFunc1 of DebugHelper.dll when that is installed.
// FUNCTION: 0x4da150
int __cdecl CallDebugHelperDll(int value)
{
    HMODULE library = LoadLibraryA("DebugHelper.dll");
    if (library) {
        int (__cdecl* func)(int) = (int (__cdecl*)(int))GetProcAddress(library, "DebugFunc1");
        if (func)
            value = func(value);
        else
            ShowDebugMessage("Couldn't find function 'DebugFunc1'\n");
        FreeLibrary(library);
    } else
        ShowDebugMessage("Couldn't find library 'debughelper.dll'\n");
    AbortProgram();
    return value;
}

// Sets up the debug support once: DebugHelper.dll when the command line asks
// for it (-debughelper=<n>), and, unless `flags` says otherwise, the
// unhandled exception filter (2), FUN_004df160 (4) and the debug thread (8).
// FUNCTION: 0x4da1d0
void __cdecl InitDebugSupport(unsigned int flags)
{
    if (DAT_005289cc)
        return;
    DAT_005289cc = 1;
    char* option = FindCommandLineSwitch("-debughelper");
    if (option) {
        int value = 0;
        sscanf(option, "=%d", &value);
        CallDebugHelperDll(value);
    }
    getenv("windir");
    if (!(flags & 2))
        SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)UnhandledExceptionHandler);
    if (!(flags & 4))
        FUN_004df160();
    if (!(flags & 8)) {
        DWORD id;
        HANDLE thread = CreateThread(0, 0x1f40, DebugThreadProc, 0, 0, &id);
        if (thread)
            while (!DAT_005289c8)
                ;
    }
}
