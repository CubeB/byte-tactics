// Decompiled by deepseek-v4.1. Names are provisional.
// Builds a "name(line) : <call stack>" debug string.
// MATCH: the argument block pins the two length expressions in locals, in the
// source order q first then space. Passing the expressions inline made MSVC5
// evaluate stack.count before `space`, keep it in eax and re-materialise al=0
// with a second `xor eax,eax` (295 bytes). With the locals the count load sinks
// past the `sub eax,ecx` and lands in ecx, as in the original.
#include <stdio.h>
#include <string.h>

struct Stack_004d89b0 {
    unsigned long addrs[14];    // +0x00
    int count;                  // +0x38
};

class CallSite {
public:
    char name[0x40];            // +0x00
    int field_40;               // +0x40
    Stack_004d89b0 stack;       // +0x44
    void FormatCallSite(char* out, int size);
};

void __cdecl GetSourceFilePath(char* out, char* name);
void __cdecl FormatCallStack(char* dest, int space, int per, int n, unsigned long* addrs);

// FUNCTION: 0x4d89b0
void CallSite::FormatCallSite(char* out, int size)
{
    char path[1000];

    out[0] = 0;
    strcpy(out, "\n");
    if (name[0]) {
        GetSourceFilePath(path, name);
        char* p = out + strlen(out);
        sprintf(p, "%s(%d)", path, field_40);
        strcat(out, " : ");
        strcat(out, "\n");
    }
    Stack_004d89b0& s = stack;
    char* q = out + strlen(out);
    int space = size - strlen(out);
    FormatCallStack(q, space, 14, s.count, s.addrs);
    int len = strlen(out);
    if (len != 0 && out[len - 1] == '\n')
        out[len - 1] = 0;
}

