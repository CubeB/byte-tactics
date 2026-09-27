// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Normalizes the line endings in `text` in place: counts the '\n's, copies the
// string to the tail of the buffer, then rewrites it from the front converting
// lone '\r' and lone '\n' into "\r\n" and collapsing existing "\r\n" pairs.
//
// GAVE UP: 95.6%. The bytes match except for two instruction-scheduling swaps
// the C++ frontend insists on (the original loads bl = '\n' before `not ecx`
// and initialises the counting pointer edx = text after the loop guard; MSVC 5
// emits the reverse order here). <stdio.h> is needed or the C++ frontend
// reassociates the destination to `(text + size) - len - 1`; `#pragma
// function(memcpy)` is needed or /O2 inlines memcpy, but the original calls it.
#include <stdio.h>
#include <string.h>

#pragma function(memcpy)

// FUNCTION: 0x4da3f0
void __cdecl FUN_004da3f0(char *text, int size)
{
    int len = strlen(text);
    char *dst = text + (size - len) - 1;
    int room = dst - text;
    char *p = text;

    while (*p != '\0') {
        if (*p == '\n')
            room--;
        p++;
    }
    if (room <= 0)
        return;

    memcpy(dst, text, len + 1);

    while (*dst != '\0') {
        char c = *dst;
        if (c == '\r') {
            *text++ = c;
            *text++ = '\n';
            if (dst[1] == '\n')
                dst++;
        } else if (c == '\n') {
            *text++ = '\r';
            *text++ = '\n';
        } else {
            *text++ = c;
        }
        dst++;
    }
    *text = '\0';
}
