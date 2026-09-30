// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Builds a block of 258-byte scroll-list item strings from a packed list of
// NUL-terminated names. The first min(a, b) names are copied verbatim; any
// remaining slots up to b get a "&G" marker prefixed to the next name. Frees
// the packed name list and returns the block.

#include <string.h>

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x41eb60
char* __stdcall FUN_0041eb60(char* names, int a, int b)
{
    char* items = (char*)FUN_004d83b0("ScrollItems2", b * 0x102);
    if (a > b) {
        a = b;
    }
    char* src = names;
    char* out = items;
    int i = 0;
    for (; i < a; i++) {
        strcpy(out, src);
        int len = strlen(src) + 1;
        src += len;
        out += len;
    }
    for (; i < b; i++) {
        strcpy(out, "&G");
        out += 2;
        strcpy(out, src);
        int len = strlen(src) + 1;
        src += len;
        out += len;
    }
    FUN_004d85a0(names);
    return items;
}
