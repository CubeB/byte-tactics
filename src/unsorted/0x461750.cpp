// Decompiled by DeepSeek V4.1 Flash and Claude Opus 5.5, finished by
// deepseek-v4.1-flash and space-bunny-free. Names are provisional.
// Partial (98.6%), 2 bytes. Only the last block differs: the original's
// out-of-line allocation-failure `return 0` is the real epilogue, scheduled as
// `pop edi; pop esi; xor eax, eax; pop ebx`, while this version emits
// `xor eax, eax; pop edi; pop esi; pop ebx` there.
// Third pass (space-bunny-free): everything below is still 98.6% with the
// same one-hunk diff unless a percentage is given.
//   - a `goto fail` inside the helper, with `e` and `n` hoisted uninitialised
//     to the top of the helper so no jump skips an initialiser: byte-identical
//     to this file, so the source order of the return statement is not the
//     lever either;
//   - `if (field_b318 != 0) {} else if (...) {} else {}` instead of the nested
//     `if`, and a `goto ready` for the "already allocated" arm (both 98.6%);
//   - an explicit `if (Setup(...)) return 1; else return 0;` in the outer
//     (85.7%, 204 bytes): the fail block moves up next to the network check;
//   - the failure arm assigning to a local the outer returns at the end
//     (`int r = 0; if (Setup(...)) r = 1; return r;`, 76.1%): the return
//     becomes `mov eax, edi` and the network check moves;
//   - the network check inside the helper, the outer written only
//     `return Setup(arg1, arg2);` (84.1%, 208 bytes): the fail block is then
//     emitted early, and the interleaved schedule lands on the success block
//     (`pop edi; pop esi; mov eax, 1; pop ebx; ret 8`), which is the
//     function's last block entered by fall-through after the call. So the
//     interleaving is not tied to the fail return, it goes to the last block;
//   - `if (DAT_00506dbc == 0) return 0; return Setup(arg1, arg2);` (73.9%):
//     MSVC drops ebx from the prologue and reorders the first blocks.
// Untried: making the fail block a fall-through rather than a `je` target
// while keeping the alloc path after it, which needs the body reordered so
// that a block ending in a call falls into the fail return.
//
// What got it from 86.5% to 98.6%: the body after the network check is an
// inline helper whose failure `return 0` the caller tests with
// `if (!Setup(...)) return 0;`. That keeps the two `return 0` blocks apart and
// puts the failure one last. Writing the failure `return 0` as the function's
// last statement instead (a `goto fail`, `if (Setup()) return 1; return 0;`,
// an outer `if (DAT_00506dbc)`) gives the interleaved epilogue but makes MSVC
// merge the network check's `return 0` into it (84 to 86%). An N-declarations
// sweep (0 to 400) leaves both shapes unchanged, so the rest is source shape.
//
// Retried with more shapes, all still 98.6% with the same one-hunk diff:
//   - the same helper but with the outer written `if (Setup(...) == 0) goto
//     fail; return 1; fail: return 0;` (identical bytes to this version);
//   - a free `static int Setup(Class*, int, int)` helper.
// A `goto fail` in the outer with no helper (v1) puts the fail return inline
// after the branch and makes the success `return 1` the interleaved final
// epilogue (84.1%). Sending the network check to the same label (v5) merges
// both exits into one early block (82.7%). So the inlined-helper shape is
// needed for the control flow, and the interleaved schedule seems to need the
// fail block to be the function's canonical exit (as in 0x461db0, where a
// labelled `return 0` is shared by three gotos); with a single jump
// predecessor MSVC emits an ordinary duplicated return block. The original
// reaches 0x461818 from exactly one `je` (0x46179d) and still interleaves, so
// the missing lever is elsewhere.
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
    int Setup(int arg1, int arg2)
    {
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
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
        return 1;
    }
};

// FUNCTION: 0x461750
int Class_00461750::FUN_00461750(int arg1, int arg2)
{
    if (DAT_00506dbc == 0) {
        return 0;
    }
    if (!Setup(arg1, arg2)) {
        return 0;
    }
    return 1;
}
