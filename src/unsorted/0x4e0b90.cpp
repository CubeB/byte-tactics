// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL 79.1% (2102 of 2150 bytes). Memory Status dialog window proc,
// __thiscall(msg,wParam,lParam), ret 0xc, frame 0x92c (frame size now exact).
// Same class as the ctor at 0x4e0570 (hwnd +0x00, field_4 +0x04, left +0x08,
// top +0x0c, time double +0x10, table double[10] +0x18, total double +0x68,
// index int +0x70, flag_78 +0x78, workingSet unsigned char +0x79).
// Dispatch is a switch on the unsigned message, cases laid out in reverse
// source order 0x312, 0x110, 0x111, 0x113. The 0x113 arm builds the whole
// "Current Allocations..." report from thirteen inlined copies of the
// decimal-with-commas formatter (same code as fmt_004e07e0 in 0x4e07e0.cpp)
// plus one big sprintf. The formatter helper and all stack slots line up.
//
// Remaining differences:
//  - `this` lives in esi; the original keeps it in ebx (`mov ebx,ecx` right
//    after `push ebp`, before `push esi`), so every `[ebx+N]` here is `[esi+N]`
//    and the prologue/epilogue ordering differs. This is the single biggest
//    remaining block of diff.
//  - The "%s,%s" call has its two string arguments in the opposite order:
//    original is sprintf(req, "%s,%s", b, copy+2) (it leas 0xf6 first,
//    then 0xb8); this file has (copy+2, b). Swap it.
//  - The `total*0.1 / >0` fp block must sit right after that branch's sprintf
//    tail (original 0x4e0e81), not after the seven common formatter calls.
//  - `req` is char[100] (0x108..0x16b) so buf[2000] lands at 0x16c and the
//    frame totals 0x92c; keep that if refactoring.
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
void __cdecl FUN_004e3400(HWND hwnd, char* name);
void __cdecl FUN_004da5b0(HWND hwnd, char* url, char* ext);
void FUN_004da860(void);
double FUN_004e1730(void);
char __cdecl FUN_004e07e0(char* dest);

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
    void FUN_004e05f0(char on);
};

class Class_004e0b90 {
public:
    HWND hwnd;                         // +0x00
    int field_4;                       // +0x04
    int left;                          // +0x08
    int top;                           // +0x0c
    double time;                       // +0x10
    double table[10];                  // +0x18
    double total;                      // +0x68
    int index;                         // +0x70
    int unknown_74;                    // +0x74
    char flag_78;                      // +0x78
    unsigned char workingSet;          // +0x79

    int FUN_004e0b90(unsigned int msg, int wParam, int lParam);
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
int Class_004e0b90::FUN_004e0b90(unsigned int msg, int wParam, int lParam)
{
    switch (msg) {
    case 0x312:
        if (wParam != 10)
            return 0;
        ((Class_004e05f0*)this)->FUN_004e05f0(!IsWindowVisible(hwnd));
        return 0;

    case 0x110: {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        left = rect.left;
        top = rect.top;
        RegisterHotKey(hwnd, 10, 1, 0x23);
        CheckDlgButton(hwnd, 0x3f9, workingSet);
        if (flag_78)
            ((Class_004e05f0*)this)->FUN_004e05f0(1);
        return 1;
    }

    case 0x111:
        switch (wParam & 0xffff) {
        case 0x3f9:
            workingSet = (workingSet == 0);
            ((Class_004e0520*)this)->FUN_004e0520(0);
            return 0;
        case 0x3fa:
            FUN_004da5b0(hwnd, "http://10.0.150.18/programming/library/extras/memorystatusdialog.html", ".htm");
            return 0;
        case 0x3eb:
            FUN_004da860();
            return 0;
        case 0:
        case 1:
        case 2:
            ((Class_004e05f0*)this)->FUN_004e05f0(0);
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
            FUN_004e3400(hwnd, DAT_0050d72c);
        }

        int delta = DAT_00528a04 - field_4;
        double now = FUN_004e1730();
        double speed = (double)delta / (now - time);
        time = now;
        total = speed + total - table[index];
        table[index] = speed;
        if (++index == 10)
            index = 0;
        field_4 = DAT_00528a04;

        buf[0] = DAT_005119b8;

        if (DAT_005289fc != 0) {
            fmt_004e0b90(t, DAT_005289dc + 1000000000);
            strcpy(copy, t);
            fmt_004e0b90(b, DAT_005289fc);
            sprintf(req, "%s,%s", copy + 2, b);
        } else {
            fmt_004e0b90(t2, DAT_005289dc);
            sprintf(req, "%s", t2);
        }

        fmt_004e0b90(s0, DAT_005289d0);
        fmt_004e0b90(s1, DAT_005289f0);
        fmt_004e0b90(s2, DAT_005289d8);
        fmt_004e0b90(s3, DAT_005289f8);
        fmt_004e0b90(s4, (unsigned int)field_4);
        fmt_004e0b90(s5, DAT_00528a1c);
        fmt_004e0b90(s6, DAT_00528a08);

        double rate = total * 0.1;
        if (!(rate > 0.0))
            rate = 0.0;

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
                s6, s5, s4, rate, s3, s2, req, DAT_00528a00, DAT_005289d4, s1, s0);

        if (workingSet != 0)
            FUN_004e07e0(&buf[strlen(buf)]);

        if (strcmp(buf, DAT_005295d8) == 0)
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
