// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>

void __cdecl ReportException(EXCEPTION_POINTERS* exception, const char* message);

// Raises a divide-by-zero on purpose and reports it, so that `message`
// arrives with a register dump and stack trace of the place it came from.
// FUNCTION: 0x49e680
void __stdcall ReportViaException(const char* message)
{
    int zero = 0;
    __try {
        __asm {
            push eax
            push edx
            div zero
            pop edx
            pop eax
        }
    } __except (ReportException(GetExceptionInformation(), message), EXCEPTION_EXECUTE_HANDLER) {
    }
}
