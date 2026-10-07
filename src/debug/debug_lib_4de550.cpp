// Decompiled by deepseek-v4.1. Names are provisional.
// Wrapper over the imagehelp SymGetLineFromAddr pointer (DAT_00528ab4, resolved
// by the loader next door): looks the line for `addr` up through the cached
// process handle (DAT_00528aa4, initialised once behind the DAT_00528aac bit 0
// latch) and copies the resulting IMAGEHLP_LINE out. On failure it stores
// GetLastError() through `err`, or 0 when the imagehelp DLL was never loaded.
#include <windows.h>

extern char DAT_00528aac;
extern HANDLE DAT_00528aa4;

struct Line_004de550 {
    DWORD SizeOfStruct;
    DWORD Key;
    DWORD LineNumber;
    DWORD FileName;
    DWORD Address;
};

typedef BOOL (__stdcall *SymGetLineFromAddr_004de550)(HANDLE, DWORD, DWORD*, Line_004de550*);
extern SymGetLineFromAddr_004de550 DAT_00528ab4;

// FUNCTION: 0x4de550
char __cdecl GetLineFromAddress(DWORD addr, Line_004de550* out, DWORD* err)
{
    if (DAT_00528ab4) {
        if (!(DAT_00528aac & 1)) {
            DAT_00528aac |= 1;
            DAT_00528aa4 = GetCurrentProcess();
        }
        // Aggregate-initialised inside this block, not hoisted: keeps the zero stores here.
        Line_004de550 line = { 0x14 };
        // No initialiser, zeroed after the Address store: fixes where the store lands.
        DWORD disp;
        line.Address = addr;
        disp = 0;
        if (DAT_00528ab4(DAT_00528aa4, addr, &disp, &line)) {
            *out = line;
            return 1;
        }
        *err = GetLastError();
        return 0;
    }
    *err = 0;
    return 0;
}
