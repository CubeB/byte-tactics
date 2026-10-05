// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>

void __cdecl LoadMatch(void* param);
int __stdcall ExceptionFilter(EXCEPTION_POINTERS* exception, const char* thread);

// The loading thread's body: an exception in it is reported, with the
// thread's name, before the process dies.
// FUNCTION: 0x497c70
void __cdecl LoadThreadMain(void* param)
{
    __try {
        LoadMatch(param);
    } __except (ExceptionFilter(GetExceptionInformation(), "Load Thread")) {
    }
}
