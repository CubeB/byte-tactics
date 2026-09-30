// Decompiled by space-bunny-free, muse-spark-1.3-free and deepseek-v4.1-flash,
// finished by space-bunny-free. Names are provisional.
//
// Reports the process working set into a caller-supplied buffer, with a
// psapi.dll QueryWorkingSet refresh at most once every ten calls.
//
// deepseek-v4.1-flash: 99.4% (934 of 934 bytes, only instruction order in the
// scan prologue differs). The breakthrough was realising the three counters
// are not locals: the five number-formatting blocks all reload them from
// DAT_005295c0/b8/28, so the loop increments the globals directly and MSVC
// promotes them to esi/ebx/edi, storing the initial zeros before the loop
// (the n==0 path needs them) and the final values after it. Writing explicit
// locals with a pre/post store pair made MSVC rematerialise one zero and spill
// the counter (951 bytes, 76.7%). A separate `cnt = n` assigned just after the
// pointer lea puts `mov ebp,eax` after `lea edx` like the original.
//
// Remaining difference (4 instructions out of 934, 99.4%): in the pre-loop
// zero block the original orders the registers esi, ebx, edi and the globals
// pt, shared, priv, while this version emits esi, edi, ebx and pt, priv,
// shared. Both have the same register mapping (pt=esi, shared=ebx,
// priv=edi); only the emission order differs.
//
// space-bunny-free: the pool order for these three promoted globals is fixed
// at (esi, edi, ebx) and no source shape moves it. Verified unchanged at
// 99.4% by: writing the stores in the original pt, shared, priv order (which
// does fix the store order but flips the loop-body inc order to esi, edi,
// ebx, 98.5%); reordering the extern declarations, both c0/b8/28 first and
// b8/c0/28 first; and declaring the three counters unsigned instead of int.
// The xors follow the register-assignment order and the stores follow source
// order, and the allocator derives both from the first source mention, so
// store order and register order cannot be steered independently. This is
// compiler state, not source shape.
#include <windows.h>
#include <stdio.h>

LPCRITICAL_SECTION FUN_004e06f0(void);

extern int DAT_00529508;
extern HMODULE DAT_005295bc;
extern char DAT_005295cc;
extern HANDLE DAT_00529530;
extern int DAT_00529528;
extern int DAT_005295b8;
extern int DAT_005295c0;
extern int DAT_005295c8;
extern int DAT_005295d0;
extern int DAT_005295d4;

typedef struct {
    DWORD NumberOfPages;
    unsigned int WorkingSetInfo[99999];
} WSInfo_004e07e0;

typedef BOOL (WINAPI *QueryWorkingSet_004e07e0)(HANDLE, PVOID, DWORD);

// Writes n as a decimal with thousands separators, then reverses it.
static void __inline fmt_004e07e0(char *buf, unsigned int n)
{
    char *f;
    char *w;
    int digits;

    *buf = 0;
    f = buf;
    w = buf;
    digits = 0;
    do {
        *w = (char)('0' + n % 10);
        w++;
        n /= 10;
        digits++;
        if (digits % 3 == 0) {
            if (n != 0) {
                *w = ',';
                w++;
            }
        }
    } while (n);
    *w = 0;
    w--;
    for (; f < w;) {
        char a = *w;
        char b = *f;
        *f++ = a;
        *w-- = b;
    }
}

// FUNCTION: 0x4e07e0
char __cdecl FUN_004e07e0(char *dest)
{
    LPCRITICAL_SECTION cs = FUN_004e06f0();
    WSInfo_004e07e0 ws;
    char priv[20];
    char max[20];
    char shared[20];
    char total[20];
    char pt[20];
    DWORD *p;
    DWORD w;
    DWORD lo;
    DWORD hi;
    DWORD n;

    EnterCriticalSection(cs);
    if (DAT_005295d0 < 0 || !(--DAT_005295d0 > 0)) {
        if (DAT_005295cc == 0) {
            DAT_005295bc = LoadLibraryA("psapi.dll");
            if (DAT_005295bc != 0) {
                DAT_00529508 = (int)GetProcAddress(DAT_005295bc, "QueryWorkingSet");
            }
            DAT_00529530 = GetCurrentProcess();
            DAT_005295cc = 1;
        }
        if (DAT_00529508 == 0) {
            LeaveCriticalSection(cs);
            return 0;
        }
        if (!((QueryWorkingSet_004e07e0)DAT_00529508)(DAT_00529530, &ws, 0x61a80)) {
            LeaveCriticalSection(cs);
            return 0;
        }
        n = ws.NumberOfPages;
        DAT_005295c8 = n;
        if (n > DAT_005295d4) {
            DAT_005295d4 = n;
        }
        DAT_005295c0 = 0;
        DAT_00529528 = 0;
        DAT_005295b8 = 0;
        if (n > 0) {
            p = (DWORD *)&ws.WorkingSetInfo[0];
            DWORD cnt = n;
            do {
                w = *p;
                lo = w & 0xfff;
                hi = w & 0xfffff000;
                if (hi >= 0xc0000000 && hi <= 0xe0000000) {
                    DAT_005295c0++;
                } else if (lo & 0x100) {
                    DAT_005295b8++;
                } else {
                    DAT_00529528++;
                }
                p++;
            } while (--cnt);
        }
        DAT_005295d0 = 10;
    }
    fmt_004e07e0(pt, DAT_005295c0 << 12);
    fmt_004e07e0(shared, DAT_005295b8 << 12);
    fmt_004e07e0(priv, DAT_00529528 << 12);
    fmt_004e07e0(max, DAT_005295d4 << 12);
    fmt_004e07e0(total, DAT_005295c8 << 12);
    sprintf(dest, "\r\nTotal Working set:   %13s\r\nMaximum Working set: %13s\r\nPrivate:             %13s\r\nShared:              %13s\r\nPage Tables:         %13s\r\n", total, max, priv, shared, pt);
    LeaveCriticalSection(cs);
    return 1;
}
