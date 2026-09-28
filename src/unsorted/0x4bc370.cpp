// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Case-insensitive wildcard match of a string against a pattern that may
// contain '?' (any one character) and '*' (zero or more characters). The
// pattern positions still alive after each input character are kept in a
// stack of at most 100 entries, so a '*' backtracks in linear time.
//
// Best result: 86.4%. Everything matches byte for byte except one instruction:
// for `pat[idx]` MSVC 5 emits `movsx ecx, [ebp+eax]` (idx as base, so ebp needs
// a zero displacement, one byte longer) where the original has
// `movsx ecx, [eax+ebp]` (pat as base, index ebp). Registers are identical
// (eax=pat, ebp=idx); only the base/index choice in the addressing mode differs.
// Tried without effect: swapping declaration order, int/unsigned/long index,
// local pointer copies, inlined accessor helpers, writing the full expression
// each time, and every common header set.
#include <ctype.h>

// FUNCTION: 0x4bc370
int __stdcall FUN_004bc370(const char* str, const char* pat)
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
            char pc = toupper(pat[idx]);
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
