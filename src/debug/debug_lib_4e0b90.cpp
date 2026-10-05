// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
// MATCH (check.py, 2150 bytes).
//
// The last diff was the /Ob2 inline budget in the formatter section: the ten
// fmt_004e0b90 call sites need eleven inline expansions, which is one more
// than this function gets (the rate_add helper takes the other slot), so the
// tenth formatter was always emitted as a call to an out of line copy, which
// also shifted the temporary slots and every internal jump target (2058 vs
// 2150 bytes, 90.5%). Writing the tenth site (s6, DAT_00528a08) out by hand
// as a plain block costs no inline budget and the whole function snaps into
// place. The nine helper sites and the rate_add helper are untouched, so the
// x87 rate update keeps its original `fdiv st(2)` with the quotient spilled
// to [esp+0x10] (0x4e0c45 `fst`, reloaded at 0x4e0c54), which only the
// inlined helper shape produces.
//
// Findings kept elsewhere: the 0x111 switch handles only 1 and 2 (0x4e12e3
// `jle`, not `jl`), and strcmp wants the global as the first argument
// (0x4e1236 `lea esi` before the `mov eax`).
#include <windows.h>
#include <stdio.h>
#include <string.h>

extern unsigned int DAT_005289d0;
extern unsigned int DAT_005289d4;
extern unsigned int DAT_005289d8;
extern unsigned int DAT_005289dc;
extern unsigned int DAT_005289f0;
extern unsigned int DAT_005289f8;
extern unsigned int DAT_005289fc;
extern unsigned int DAT_00528a00;
extern unsigned int DAT_00528a04;
extern unsigned int DAT_00528a08;
extern unsigned int DAT_00528a1c;
extern char DAT_005119b8;
extern char DAT_005295d8[];
extern char* DAT_0050d72c;

void __cdecl FUN_004e33d0(HWND hwnd, char* name, double a, double b);
void __cdecl SaveWindowPosition(HWND hwnd, char* name);
void __cdecl OpenUrl(HWND hwnd, char* url, char* ext);
void ResetAllocStats(void);
double GetTimeSeconds(void);
char __cdecl FormatWorkingSet(char* dest);

class Class_004e0520 {
public:
    char unknown_0[0x79];
    unsigned char workingSet;          // +0x79
    void FUN_004e0520(int readOnly);
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

class Class_004e0b90 {
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
};

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

// FUNCTION: 0x4e0b90
int Class_004e0b90::HandleMemoryStatusMessage(unsigned int msg, int wParam, int lParam)
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
            ((Class_004e0520*)this)->FUN_004e0520(0);
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
            SaveWindowPosition(hwnd, DAT_0050d72c);
        }

        int delta = DAT_00528a04 - field_4;
        double now = GetTimeSeconds();
        double speed = now - time;
        time = now;
        rate_add(&rates, delta, speed);
        field_4 = DAT_00528a04;

        buf[0] = DAT_005119b8;
        memset(buf + 1, 0, sizeof(buf) - 1);

        if (DAT_005289fc != 0) {
            fmt_004e0b90(t, DAT_005289dc + 1000000000);
            strcpy(copy, t);
            fmt_004e0b90(b, DAT_005289fc);
            sprintf(req, "%s,%s", b, copy + 2);
        } else {
            fmt_004e0b90(t2, DAT_005289dc);
            sprintf(req, "%s", t2);
        }

        double rate = rates.total * 0.1;
        double rateText = (rate > 0.0) ? rate : 0.0;

        fmt_004e0b90(s0, DAT_005289d0);
        fmt_004e0b90(s1, DAT_005289f0);
        fmt_004e0b90(s2, DAT_005289d8);
        fmt_004e0b90(s3, DAT_005289f8);
        fmt_004e0b90(s4, (unsigned int)field_4);
        fmt_004e0b90(s5, DAT_00528a1c);
        {
            char* buf = s6;
            unsigned int n = DAT_00528a08;
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
                s6, s5, s4, rateText, s3, s2, req, DAT_00528a00, DAT_005289d4, s1, s0);

        if (workingSet != 0)
            FormatWorkingSet(&buf[strlen(buf)]);

        if (strcmp(DAT_005295d8, buf) == 0)
            return 0;

        SetDlgItemTextA(hwnd, 1000, buf);
        UpdateWindow(hwnd);
        strcpy(DAT_005295d8, buf);
        return 0;
    }

    default:
        return 0;
    }
}
