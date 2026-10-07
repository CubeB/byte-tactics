// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by claude-opus-5-5. Names are provisional.
// Formats the saved call stack and stack dump into buf.
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

    void FormatStackReport();
};

// FUNCTION: 0x4d9ca0
void Class_004d9ca0::FormatStackReport()
{
    int m = copied;
    unsigned long* q0 = pc;
    int n = count;
    // One len for both phases, not a second copy.
    unsigned int len = 0xa44c;
    char* p = buf;
    int i;

    if (n > 0) {
        sprintf(p, "Call stack:\n");
        len -= strlen(p);
        p += strlen(p);
        // Both loops index the arrays; walking pointers changes the loop setup.
        for (i = 0; i < n; i++) {
            if (len <= 0x1e) break;
            sprintf(p, "%08lX", ret[i]);
            // A named local set by this if/else, not a ternary: fixes the register split.
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
