// Decompiled by DeepSeek V4.1 Flash and Claude Opus 5.5, finished by
// deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
// MATCH (208 of 208 bytes).
//
// The last remaining hunk was the allocation-failure epilogue: the original
// ends with `pop edi; pop esi; xor eax, eax; pop ebx; ret 8` while every
// helper based shape emitted `xor eax, eax; pop edi; pop esi; pop ebx; ret 8`.
// The fix is the shape the matched sibling 0x461020 uses: the whole body after
// the network check is a flat `do { ... } while (0)` region whose exit is
// BEFORE the SetThreadPriority call, and the failure `return 0` lives inside
// the region. C2 then moves that `return 0` out to the end of the function and
// emits it as the function's last block, and the last block is the one that
// puts the register restores before the return value. Without the `while (0)`
// region (or with the failure return written as an out-of-line helper's
// `return 0`, which was the previous 98.6% version) the network check's
// `return 0` is either merged into the failure block or duplicated with the
// xor first. The region must end before SetThreadPriority: putting the call
// inside the region adds a third edge into the `return 1` epilogue and C2
// duplicates that epilogue.
#include <windows.h>

void* __cdecl operator new(unsigned int size);

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
    do {
        if (field_b318 == 0) {
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
    } while (0);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
    return 1;
}
