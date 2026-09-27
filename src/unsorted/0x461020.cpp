// Decompiled by space-bunny-free. Names are provisional.
// MATCH. The function guards a body on DAT_00506dbc and returns 1 from a single
// shared epilogue, with the allocation-failure `return 0` as a second, out of
// line epilogue reached by `je` from the new() null test.
//
// The shape that reproduces it: the `do { } while (0)` region must end BEFORE
// the SetThreadPriority call, not around the whole body. The region is what
// makes C2 move the failure `return 0` out to the end of the function and give
// it the `pop edi; xor eax, eax; pop esi; ret 8` form (the last block C2 emits
// puts the register restores before the return value; an earlier one keeps the
// xor first). But with the call inside the region the region exit is a third
// entry edge into the shared `return 1`, and C2 tail duplicates that epilogue
// (10 extra bytes) with the flag test's `je` aiming at the copy. Leaving
// SetThreadPriority after the region, still inside the `if`, gives the success
// epilogue exactly its original two entry edges (the `je` and the call's fall
// through) and the layout is identical. The same problem is unsolved in the
// near-identical sibling 0x461750.
//
// Two things the disassembly forces:
//
//  - The object at DAT_00513000 holds m_defaultSendPacingMs at +4 and the
//    0x1044 byte entries at +8: the first Class_00462470::FUN_00461db0 call
//    sets ecx to 0x513008 and the pacing store lands on 0x51300c, so the
//    per-entry pacing field is entry+4. 0x4619e0.cpp models the same array as
//    channels at +0xc with pacing at +0, which is indistinguishable there.
//  - The pacing block is 0x4619e0 (Class_004619e0::FUN_004619e0) inlined, and
//    it has to be inlined from source with `__inline`: written out inline in
//    the body, MSVC 5 folds its second `rate == 0` test away, and called out of
//    line, as in 0x4578d0, it would not be inlined at all. Inlined it keeps the
//    redundant test, which is what the original has.
#include <windows.h>

void* operator new(unsigned int size);
void FUN_00461170(const char* fmt, ...);

class Class_00462470 {
public:
    int field_0;                       // +0x00
    unsigned long m_pacingMs;          // +0x04
    char unknown_8[0x14 - 0x8];
    int field_14;                      // +0x14
    char unknown_18[0x1c - 0x18];
    int field_1c;                      // +0x1c
    char unknown_20[4];
    int field_24;                      // +0x24
    char unknown_28[0x1044 - 0x28];

    void FUN_00461db0(int a1, int a2, int a3, int a4);
};

class Class_004619e0 {
public:
    char unknown_0[4];
    unsigned long m_defaultSendPacingMs;   // +0x04
    Class_00462470 entries[11];            // +0x08

    void FUN_004619e0(int rate);
};

struct Buffer_461020 {
    char unknown_0[0xc];
    int field_c;
};

extern Class_004619e0 DAT_00513000;
extern int DAT_00512c94;
extern int DAT_00506dbc;
extern Buffer_461020* DAT_0051e314;
extern void* DAT_0051e318;
extern void* DAT_0051e31c;
extern int DAT_0051e528;
extern int DAT_0051e52c;

__inline void Class_004619e0::FUN_004619e0(int rate)
{
    unsigned long pace;
    if (rate < 0) {
        DAT_00506dbc = 0;
        return;
    }
    if (rate == 0) {
        pace = 200;
    } else {
        if (rate < 2) {
            rate = 2;
        } else if (rate > 30) {
            rate = 30;
        }
        pace = 1000 / rate;
    }
    m_defaultSendPacingMs = pace;
    FUN_00461170("setting m_defaultSendPacingMs to: %lums\n", pace);
    Class_00462470* p = &entries[0];
    for (int i = 0; i < 11; i++, p++) {
        p->m_pacingMs = (m_defaultSendPacingMs * 30 + 999) / 1000;
    }
}

// FUNCTION: 0x461020
int __stdcall FUN_00461020(int param_1, int param_2)
{
    if (DAT_00512c94 != 0) {
        DAT_00513000.FUN_004619e0(DAT_00512c94);
    }
    if (DAT_00506dbc != 0) {
        do {
            if (DAT_0051e318 == 0) {
                if (DAT_0051e31c != 0) {
                    DAT_0051e318 = DAT_0051e31c;
                    DAT_0051e31c = 0;
                } else {
                    DAT_0051e318 = operator new(0x42a);
                    if (DAT_0051e318 == 0) {
                        return 0;
                    }
                    DAT_0051e528 = 0x42a;
                }
            }
            DAT_0051e52c = 0;
            if (DAT_0051e314 != 0) {
                DAT_0051e314->field_c = 0;
                DAT_0051e314 = 0;
            }
            DAT_00513000.entries[0].FUN_00461db0(0, DAT_00513000.m_defaultSendPacingMs, param_1, param_2);
            Class_00462470* p = &DAT_00513000.entries[1];
            for (int i = 1; i < 11; i++, p++) {
                p->FUN_00461db0(-1, DAT_00513000.m_defaultSendPacingMs, 2, 100);
            }
        } while (0);
        SetThreadPriority(GetCurrentThread(), -2);
    }
    return 1;
}
