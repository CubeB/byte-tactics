// Decompiled by deepseek-v4.1. Names are provisional.
// check.py prints MATCH.
// Wrapper over the imagehelp SymGetLineFromAddr pointer (DAT_00528ab4, resolved
// by the loader next door): looks the line for `addr` up through the cached
// process handle (DAT_00528aa4, initialised once behind the DAT_00528aac bit 0
// latch) and copies the resulting IMAGEHLP_LINE out. On failure it stores
// GetLastError() through `err`, or 0 when the imagehelp DLL was never loaded.
//
// Three source-level facts carry the match, all about the line local:
//
// 1. It must be aggregate-initialised INSIDE the main block
//    (`Line_004de550 line = { 0x14 };`). An initialiser's zero stores do not
//    common up with the tail's `*err = 0`, so MSVC materialises the zero
//    (`xor eax,eax` inside the main block, at the original's 0x4de57d) instead
//    of hoisting it to the entry, and the entry keeps a plain `test eax,eax`
//    instead of `xor esi,esi / cmp eax,esi`.
// 2. The initialiser also emits a store of 0 to Address which the later
//    `line.Address = addr;` overwrites, and MSVC keeps that dead store (a plain
//    assignment list is DSE'd instead, 4 bytes short).
// 3. `disp` must be declared WITHOUT an initialiser (`DWORD disp;`) and zeroed
//    by a statement AFTER the Address assignment. `DWORD disp = 0;` reuses the
//    live zero register in eax and sinks the store before the addr load, while
//    the statement form is an immediate `mov dword ptr [esp+0x18], 0` the
//    scheduler places after the last push, exactly as the original does.
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
    if (DAT_00528ab4) {
        if (!(DAT_00528aac & 1)) {
            DAT_00528aac |= 1;
            DAT_00528aa4 = GetCurrentProcess();
        }
        Line_004de550 line = { 0x14 };
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
