// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Partial: every block matches the original's layout except that the
// allocation-failure `return 0` block sits out of line at the end in the
// original (reached by `je`) while MSVC 5 puts it inline for every plain
// if/else spelling (75.4%). Writing the buffer setup as a `while` loop keeps
// the original's block order (DAT return inline, failure return last) but adds
// the loop's back-edge test and hoists 0x42a into ebx, which is 78%.
#include <windows.h>

void* operator new(unsigned int size);

extern int DAT_00506dbc;

class Class_00462470 {
public:
    int field_0;                       // +0
    char unknown_4[0x18];              // +4
    int field_1c;                      // +0x1c
    char unknown_20[4];                // +0x20
    int field_24;                      // +0x24
    char unknown_28[0x1044 - 0x28];    // pad to 0x1044

    void FUN_00461db0(int a1, int a2, int a3, int a4);
};

class Class_00461750 {
public:
    char unknown_0[4];
    int field_4;                       // +4
    Class_00462470 entries[11];        // +8
    char unknown_b2f4[0x20];           // +0xb2f4
    int field_b314;                    // +0xb314
    int field_b318;                    // +0xb318
    int field_b31c;                    // +0xb31c
    char unknown_b320[0xb528 - 0xb320];
    int field_b528;                    // +0xb528
    int field_b52c;                    // +0xb52c

    int FUN_00461750(int arg1, int arg2);
};

// FUNCTION: 0x461750
int Class_00461750::FUN_00461750(int arg1, int arg2)
{
    if (DAT_00506dbc == 0) {
        return 0;
    }
    while (field_b318 == 0) {
        if (field_b31c != 0) {
            field_b318 = field_b31c;
            field_b31c = 0;
        } else {
            field_b318 = (int)operator new(0x42a);
            if (field_b318 == 0) {
                return 0;
            }
            field_b528 = 0x42a;
        }
    }
    field_b52c = 0;
    if (field_b314 != 0) {
        *(int*)(field_b314 + 0xc) = 0;
        field_b314 = 0;
    }
    entries[0].FUN_00461db0(0, field_4, arg1, arg2);
    Class_00462470* e = entries + 1;
    int n = 10;
    do {
        e->FUN_00461db0(-1, field_4, 2, 100);
        e++;
    } while (--n);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
    return 1;
}
