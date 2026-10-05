// Decompiled by Opus. Names are provisional.
// Builds a block of 128-byte scroll-list item strings from a packed list of
// NUL-terminated names, prefixing each with marker glyphs chosen by its flag
// character ('L', 'W', 'U'). Frees the packed name list and returns the block.

#include <string.h>

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x41eaa0
char* __stdcall BuildScrollItems1(char* names, char* flags, int count)
{
    char* items = (char*)FUN_004d83b0("ScrollItems1", count << 7);
    memset(items, 0, count << 7);
    char* src = names;
    char* out = items;
    for (int i = 0; i < count; i++) {
        if (flags[i] == 'L') {
            *out++ = (char)0xff;
            *out++ = ' ';
        } else if (flags[i] == 'W') {
            *out++ = (char)0xfe;
            *out++ = ' ';
        }
        if (flags[i] == 'U') {
            *out++ = (char)0xfd;
            *out++ = ' ';
        }
        strcpy(out, src);
        int len = strlen(src) + 1;
        src += len;
        out += len;
    }
    FUN_004d85a0(names);
    return items;
}
