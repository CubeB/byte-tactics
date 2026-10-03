// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by claude-opus-5-5. Names are provisional.
// Formats the saved call stack and stack dump into buf. MATCH after a rebuild
// from the disassembly; what decided it (claude-opus-5-5, #5034):
//  - Both loops index the arrays (`ret[i]`, `stack[i]`, `&q0[i]`). MSVC
//    strength-reduces them into the pointer walks at [esp+0x18]/[esp+0x1c] and
//    sets those up after the loop guard, as the original does; walking pointers
//    in the source set them up before the guard.
//  - One `len` for both phases. The earlier files' separate `len2` copy kept
//    len in memory inside the loops but put it in ebp between them (91.1%).
//  - The call-stack separator is a named local chosen by an if/else that
//    stores " " only once `i != n - 1` is known. That one local is what gives i
//    ebp and keeps len in memory: with the ternary inside strcat (as in the
//    stack-dump loop) MSVC gives len ebp instead (36.3%), and
//    `sep = " "; if (i == n - 1 || i % 8 == 7) sep = "\n";` is one instruction
//    out of place (99.6%).
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

class Class_004d9ca0 {
public:
    unsigned long ret[0x1e];
    int count;
    int stack[0x800];
    int copied;
    unsigned long* pc;
    char buf[0xa44c];

    void FUN_004d9ca0();
};

// FUNCTION: 0x4d9ca0
void Class_004d9ca0::FUN_004d9ca0()
{
    int m = copied;
    unsigned long* q0 = pc;
    int n = count;
    unsigned int len = 0xa44c;
    char* p = buf;
    int i;

    if (n > 0) {
        sprintf(p, "Call stack:\n");
        len -= strlen(p);
        p += strlen(p);
        for (i = 0; i < n; i++) {
            if (len <= 0x1e) break;
            sprintf(p, "%08lX", ret[i]);
            const char* sep;
            if (i == n - 1) {
                sep = "\n";
            } else {
                sep = " ";
                if (i % 8 == 7) sep = "\n";
            }
            strcat(p, sep);
            len -= strlen(p);
            p += strlen(p);
        }
    } else {
        *p = 0;
    }
    if (m > 0 && len > 0x1e) {
        sprintf(p, "Stack dump:\n");
        len -= strlen(p);
        p += strlen(p);
        for (i = 0; i < m; i++) {
            if (len <= 0x1e) break;
            if (i % 8 == 0) {
                sprintf(p, "%08lX: ", (unsigned long)&q0[i]);
                len -= strlen(p);
                p += strlen(p);
            }
            sprintf(p, "%08lX", stack[i]);
            strcat(p, (i == m - 1 || i % 8 == 7) ? "\n" : " ");
            len -= strlen(p);
            p += strlen(p);
        }
    }
}
