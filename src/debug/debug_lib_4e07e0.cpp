// Decompiled by space-bunny-free, muse-spark-1.3-free and deepseek-v4.1-flash,
// finished by deepseek-v4.1-flash. Names are provisional.
//
// Reports the process working set into a caller-supplied buffer, with a
// psapi.dll QueryWorkingSet refresh at most once every ten calls.
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
char __cdecl FormatWorkingSet(char *dest)
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
        // Chained, not separate statements: decouples register order from store order.
        DAT_00529528 = DAT_005295b8 = DAT_005295c0 = 0;
        // Counters stay globals, incremented in the loop: the formatters reload them.
        if (n > 0) {
            p = (DWORD *)&ws.WorkingSetInfo[0];
            // Separate copy of n, assigned after p: fixes the instruction order.
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
