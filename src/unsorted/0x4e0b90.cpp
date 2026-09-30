// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free. Names are provisional.
// 90.5% (check.py). Everything outside the formatter section now matches; the
// two remaining hunks, both listed here for a region based pass:
//
//  1. 0x4e1134-0x4e11a7 (formatter temporaries). Ten counters are formatted
//     into twenty byte temporaries; nine calls to fmt_004e0b90 are inlined
//     here and the tenth is a real call to an out of line copy of the helper
//     (2058 bytes against 2150, 726 instructions against 756), which also
//     moves two temporaries off the original's slots: 0xf4 and 0x11c where the
//     original has 0xe0 (s0) and 0x114 (req). Nothing is lost, and the
//     macro spelling that inlines all ten (build/scratch/0x4e0b90/w6.cpp,
//     2156 bytes) drops to 63%, so the ten copies need per call site register
//     choices this spelling does not give them.
//     build/scratch/0x4e0b90/w1.cpp is the variant that inlines all ten
//     (2144 bytes, 759 instructions) and gets the x87 hunk wrong instead, at
//     89.1%.
//  2. 0x4e0c31-0x4e0c63 (x87 rate update) matches only when the quotient is
//     computed inside rate_add. What the original does that nothing here
//     reproduces: it stores the quotient to its frame slot (0x4e0c45 `fst
//     qword ptr [esp + 0x10]`, non popping) immediately after the divide,
//     before `time = now`, then adds `total` with a memory operand
//     (0x4e0c51 `fadd qword ptr [eax + 0x50]`) and reloads the quotient for
//     the table store. So the original's local has a frame home and is still
//     read from the x87 register, which needs a use between the two that this
//     compiler keeps in one register. Spelled as a member method, or with the
//     quotient in a caller local, it emits `fdivp st(1)` after one fxch.
//
// Findings kept elsewhere: the 0x111 switch handles only 1 and 2 (0x4e12e3
// `jle`, not `jl`), the rate is clamped with a ternary into a second double
// (`fcom` then `fstp`, 0x4e0e8a) rather than `if (!(x > 0))`, and strcmp
// wants the global as the first argument (0x4e1236 `lea esi` before the
// `mov eax`).
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
        fmt_004e0b90(s6, DAT_00528a08);


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
            FUN_004e07e0(&buf[strlen(buf)]);

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
