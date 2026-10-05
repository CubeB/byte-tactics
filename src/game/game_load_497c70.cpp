// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>

void __cdecl FUN_00497180(void* param);
int __stdcall ExceptionFilter(EXCEPTION_POINTERS* exception, const char* thread);

// The loading thread's body: an exception in it is reported, with the
// thread's name, before the process dies.
// FUNCTION: 0x497c70
void __cdecl FUN_00497c70(void* param)
{
    __try {
        FUN_00497180(param);
    } __except (ExceptionFilter(GetExceptionInformation(), "Load Thread")) {
    }
}
