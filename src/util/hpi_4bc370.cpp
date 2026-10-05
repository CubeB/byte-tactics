// Decompiled by GPT-5.6-Terra, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Case-insensitive wildcard match of a string against a pattern that may
// contain '?' (any one character) and '*' (zero or more characters). The
// pattern positions still alive after each input character are kept in a
// stack of at most 100 entries, so a '*' backtracks in linear time.
//
// MATCHES. Seven earlier passes stalled at 86.4% on ONE instruction, the SIB
// byte of the load at 0x4bc3cb, and every one of them concluded the slot order
// was a backend choice the source cannot steer. It can. Two levers, both
// needed, both in the file below:
//
//  1. The subscript must read the array element in memory: `pat[stack[i]]`,
//     with the local `idx = stack[i]` still declared for the stores. The load
//     then indexes off a Lod of stack[i] and X86RM puts the POINTER in the
//     base slot (`0f be 0c 28`, base eax, index ebp). With the local as the
//     subscript index the RM is built as [int + ptr + 0] and the integer takes
//     the base slot instead (`0f be 4c 05 00`, 5 bytes, and every following
//     branch target one byte out, 315 against 314).
//  2. `#include <windows.h>`, which is what makes lever 1 fire in a
//     single-function TU. The flip is also a per-TU function-order effect:
//     with the function first in its file, `<ctype.h>` alone gives 4c 05 00
//     and windows.h gives 0c 28, but if two other functions are compiled
//     first, the function is the right slot order even with only <ctype.h>.
//     So the state that produced the original was the one of a real TU in
//     which this was the third or a later function, and windows.h reproduces
//     it here. (Defining the real neighbours 0x4bc2e0, 0x4bc300, 0x4bc320 and
//     0x4bc360 above this function also gives a MATCH, which is the same
//     effect, but duplicating them was not worth it.)
//
// Seven passes of negative results that still stand, all of them measured
// with the LOCAL as the subscript index, so they rule out the address
// spelling, not the shape above:
//  * every spelling of the address: pat[idx], *(pat+idx), *(idx+pat), with
//    index casts, a `const char*` local copy, an `int&` reference, an inlined
//    helper, struct and array casts, a second pointer to the parameter, and
//    a `char`/`short` staging local (that last one reaches the pointer-in-base
//    form but adds the extension instruction and a live SCE, so it can never
//    be byte-identical).
//  * index type (int, unsigned, long, unsigned long, short, unsigned short,
//    char, signed char, unsigned char, size_t), parameter constness, and the
//    declaration order of the four locals: no effect.
//  * an `extern int` / `struct` / `static` declaration sweep, N = 0..6000, and
//    768 header sets from tools/headers.py: flat, for the local-index shape.
//    A header set that DOES flip it would have been missed by all of those,
//    which is why this took eight passes.
//  * tools/headers.py is worth re-running on a function whose registers will
//    not budge, but only after the source shape is right: the header lever
//    here is invisible until the subscript is a memory read.
// Census worth keeping: this scale-1 two-register SIB load occurs only EIGHT
// times in the executable, and seven of the eight put the INTEGER in the
// base slot. 0x4bc3cb was the only exception, which is why seven passes read
// it as a compiler bug rather than as a source shape.
#include <windows.h>
#include <ctype.h>

// FUNCTION: 0x4bc370
int __stdcall MatchWildcard(const char* str, const char* pat)
{
    int stack[100];
    int n;
    int i;
    char c;

    stack[0] = 0;
    n = 1;
    while ((c = toupper(*str++)) != 0) {
        for (i = 0; i < n; i++) {
            int idx = stack[i];
            char pc = toupper(pat[stack[i]]);
            if (pc == '?' || pc == c) {
                stack[i] = idx + 1;
            } else if (pc == '*') {
                if (n < 100)
                    stack[n++] = idx + 1;
            } else {
                if (--n == 0)
                    return 0;
                stack[i] = stack[n];
                i--;
            }
        }
    }
    for (i = 0; i < n; i++) {
        char pc = pat[stack[i]];
        if (pc == 0)
            return 1;
        if (pc == '*' && pat[stack[i] + 1] == 0)
            return 1;
    }
    return 0;
}
