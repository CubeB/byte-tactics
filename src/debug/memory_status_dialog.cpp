// Decompiled by Sonnet, deepseek-v4.1-flash, GPT-6, space-bunny-free and edited by deepseek-v4.1. Names are provisional.

#include <windows.h>
#include <stdio.h>
#include <string.h>

HWND __cdecl CreateDialogFromTemplate(int id, HWND parent, DLGPROC proc, LPARAM param);
BOOL CALLBACK MemoryStatusDlgProc(HWND, UINT, WPARAM, LPARAM);

extern char g_performanceDialogError[]; // "Performance dialog failed to open"
extern char g_cavedogTitle[]; // "Cavedog"

extern unsigned int g_committedBytesPeak;
extern unsigned int g_lastAllocOffset;
extern unsigned int g_currentBytesPeak;
extern unsigned int g_bytesRequestedLo;
extern unsigned int g_committedBytes;
extern unsigned int g_currentBytes;
extern unsigned int g_bytesRequestedHi;
extern unsigned int g_freeBlockWraps;
extern unsigned int g_allocSerial;
extern unsigned int g_liveAllocCount;
extern unsigned int g_liveAllocPeak;
extern char DAT_005119b8;
extern char g_memStatusLastText[];
extern char* g_memoryStatusWindowName;

void __cdecl RestoreWindow(HWND hwnd, char* name, double a, double b);
void __cdecl SaveWindowPosition(HWND hwnd, char* name);
void __cdecl OpenUrl(HWND hwnd, char* url, char* ext);
void ResetAllocStats(void);
double GetTimeSeconds(void);
char __cdecl FormatWorkingSet(char* dest);

class Class_004e0520 {
public:
    char unknown_0[0x79];
    unsigned char workingSet;          // +0x79
    void LoadWorkingSetPref(int readOnly);
};

class Class_004e05f0 {
public:
    HWND hwnd;                         // +0x00
    char unknown_4[0x74];
    char flag_78;                      // +0x78
    void SetMemoryStatusWindowVisible(char on);
};

struct Rate_004e0b90 {
    double table[10]; double total; int index; int unknown_5c;
};

// Free function shape: the quotient is computed inside the helper, so the
// divisor stays on the x87 stack and the divide becomes three fxch plus
// fdiv st(2), as in the original.
static void __inline rate_add(Rate_004e0b90* r, int delta, double dt)
{
    double speed = (double)delta / dt;
    r->total = speed + r->total - r->table[r->index];
    r->table[r->index] = speed;
    if (++r->index == 10)
        r->index = 0;
}

// Same routine as fmt_004e07e0 (0x4e07e0.cpp): decimal with thousands
// separators, then reversed in place.
static void __inline fmt_004e0b90(char* buf, unsigned int n)
{
    char* f;
    char* w;
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

class MemoryStatusDialog {
public:
    HWND hwnd;                         // +0x00
    int field_4;                       // +0x04
    int left;                          // +0x08
    int top;                           // +0x0c
    double time;                       // +0x10
    Rate_004e0b90 rates;              // +0x18
    char flag_78;                      // +0x78
    unsigned char workingSet;          // +0x79

    int HandleMemoryStatusMessage(unsigned int msg, int wParam, int lParam);
    void CreateMemoryStatusDialog();
};

// 0x4e05c0 CreateMemoryStatusDialog is defined in src/debug/debug_lib.cpp;
// this file keeps only HandleMemoryStatusMessage, which matches in its own
// symbol context.

// FUNCTION: 0x4e0b90
int MemoryStatusDialog::HandleMemoryStatusMessage(unsigned int msg, int wParam, int lParam)
{
    switch (msg) {
    case 0x312:
        if (wParam != 10)
            return 0;
        ((Class_004e05f0*)this)->SetMemoryStatusWindowVisible(!IsWindowVisible(hwnd));
        return 0;

    case 0x110: {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        left = rect.left;
        top = rect.top;
        RegisterHotKey(hwnd, 10, 1, 0x23);
        CheckDlgButton(hwnd, 0x3f9, workingSet);
        if (flag_78)
            ((Class_004e05f0*)this)->SetMemoryStatusWindowVisible(1);
        return 1;
    }

    case 0x111:
        switch (wParam & 0xffff) {
        case 0x3f9:
            workingSet = (workingSet == 0);
            ((Class_004e0520*)this)->LoadWorkingSetPref(0);
            return 0;
        case 0x3fa:
            OpenUrl(hwnd, "http://10.0.150.18/programming/library/extras/memorystatusdialog.html", ".htm");
            return 0;
        case 0x3eb:
            ResetAllocStats();
            return 0;
        case 1:
        case 2:
            ((Class_004e05f0*)this)->SetMemoryStatusWindowVisible(0);
            return 0;
        default:
            return 0;
        }

    case 0x113: {
        char buf[2000];
        char req[100];
        char copy[20];
        char s0[20];
        char s2[20];
        char b[20];
        char s6[20];
        char t[20];
        char s5[20];
        char t2[20];
        char s4[20];
        char s3[20];
        char s1[20];

        if (!IsWindowVisible(hwnd))
            return 0;

        RECT rect;
        GetWindowRect(hwnd, &rect);
        if (rect.left != left || rect.top != top) {
            left = rect.left;
            top = rect.top;
            SaveWindowPosition(hwnd, g_memoryStatusWindowName);
        }

        int delta = g_allocSerial - field_4;
        double now = GetTimeSeconds();
        double speed = now - time;
        time = now;
        rate_add(&rates, delta, speed);
        field_4 = g_allocSerial;

        buf[0] = DAT_005119b8;
        memset(buf + 1, 0, sizeof(buf) - 1);

        if (g_bytesRequestedHi != 0) {
            fmt_004e0b90(t, g_bytesRequestedLo + 1000000000);
            strcpy(copy, t);
            fmt_004e0b90(b, g_bytesRequestedHi);
            sprintf(req, "%s,%s", b, copy + 2);
        } else {
            fmt_004e0b90(t2, g_bytesRequestedLo);
            sprintf(req, "%s", t2);
        }

        double rate = rates.total * 0.1;
        double rateText = (rate > 0.0) ? rate : 0.0;

        fmt_004e0b90(s0, g_committedBytesPeak);
        fmt_004e0b90(s1, g_committedBytes);
        fmt_004e0b90(s2, g_currentBytesPeak);
        fmt_004e0b90(s3, g_currentBytes);
        fmt_004e0b90(s4, (unsigned int)field_4);
        fmt_004e0b90(s5, g_liveAllocPeak);
        // Written out by hand: a tenth helper call would exceed the inline budget.
        {
            char* buf = s6;
            unsigned int n = g_liveAllocCount;
            char* f;
            char* w;
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


        sprintf(buf,
                "Current Allocations:  %13s\r\n"
                "Max Allocations:      %13s\r\n"
                "Allocation Requests:  %13s\r\n"
                "Allocations per second:  %10.1f\r\n"
                "\r\n"
                "Current Bytes:        %13s\r\n"
                "Max Bytes:            %13s\r\n"
                "Bytes Requested:  %17s\r\n"
                "\r\n"
                "Next Alloc Location:    %2X:%08lX\r\n"
                "Current with padding: %13s\r\n"
                "Max with padding:     %13s\r\n",
                s6, s5, s4, rateText, s3, s2, req, g_freeBlockWraps, g_lastAllocOffset, s1, s0);

        if (workingSet != 0)
            FormatWorkingSet(&buf[strlen(buf)]);

        if (strcmp(g_memStatusLastText, buf) == 0)
            return 0;

        SetDlgItemTextA(hwnd, 1000, buf);
        UpdateWindow(hwnd);
        strcpy(g_memStatusLastText, buf);
        return 0;
    }

    default:
        return 0;
    }
}
