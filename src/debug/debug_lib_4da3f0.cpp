// Decompiled by Claude Opus 5.5. Names are provisional.
// Normalizes the line endings in `text` in place: counts the '\n's, moves the
// string to the tail of the buffer, then rewrites it from the front converting
// lone '\r' and lone '\n' into "\r\n" and collapsing existing "\r\n" pairs.
// Needed: keeps the destination as `text + (size - len) - 1`.
#include <stdio.h>
#include <string.h>

// FUNCTION: 0x4da3f0
void __cdecl NormalizeLineEndings(char *text, int size)
{
    int len = strlen(text);
    char *dst = text + (size - len) - 1;
    int room = dst - text;

    // Index text[i]; a pointer copy is set up after the loop guard.
    for (int i = 0; text[i] != 0; i++)
        if (text[i] == '\n')
            room--;
    if (room <= 0)
        return;

    // memmove, not memcpy: the callee is 0x4e84e0.
    memmove(dst, text, len + 1);

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
