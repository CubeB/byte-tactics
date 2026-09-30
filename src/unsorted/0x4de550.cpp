// Decompiled by deepseek-v4.1. Names are provisional.
// Best version: 77.5%. Remaining diff, all one root cause: MSVC picks a
// cross-block zero constant in ESI (xor esi,esi at entry, cmp eax,esi instead
// of test eax,eax) and reuses it for the line fields and both *err stores,
// where the original materialises a zero in EAX inside the main path
// (xor eax,eax) and uses immediate zero stores for the tail *err and disp.
// The original also keeps the dead `line.Address = 0` store and schedules the
// SizeOfStruct = 0x14 store after the first two pushes. Tried: `int` return
// (fixed the mov al,1/xor al,al bytes but the entry test stayed), flat
// `if (!ptr) { *err = 0; return 0; }` (fail block was laid out first), nested
// if/else with the main path in the then arm (fixed the layout, still 77.5%).

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
char __cdecl FUN_004de550(DWORD addr, Line_004de550* out, DWORD* err)
{
    DWORD disp;
    Line_004de550 line;

    if (DAT_00528ab4) {
        if (!(DAT_00528aac & 1)) {
            DAT_00528aac |= 1;
            DAT_00528aa4 = GetCurrentProcess();
        }
        line.SizeOfStruct = 0x14;
        line.Key = 0;
        line.LineNumber = 0;
        line.FileName = 0;
        line.Address = 0;
        disp = 0;
        line.Address = addr;
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


