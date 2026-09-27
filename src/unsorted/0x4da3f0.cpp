// Decompiled by Claude Opus 5.5. Names are provisional.
// Normalizes the line endings in `text` in place: counts the '\n's, moves the
// string to the tail of the buffer, then rewrites it from the front converting
// lone '\r' and lone '\n' into "\r\n" and collapsing existing "\r\n" pairs.
//
// The bytes match. The call is memmove, not memcpy: /O2 always inlines memcpy,
// and 0x4e84e0 is also what std::string::erase (0x4c4b10) calls for
// char_traits::move, while std::string::assign (0x4e3c00) inlines its memcpy as
// rep movsd. data/symbols.csv names 0x4e84e0 `memcpy`, so check.py reports the
// reference as wrong until that entry is renamed to `memmove`.
//
// The counting loop must index `text[i]` (the pointer copy is set up after the
// loop guard); <stdio.h> keeps the destination as `text + (size - len) - 1`.
#include <stdio.h>
#include <string.h>

// FUNCTION: 0x4da3f0
void __cdecl FUN_004da3f0(char *text, int size)
{
    int len = strlen(text);
    char *dst = text + (size - len) - 1;
    int room = dst - text;

    for (int i = 0; text[i] != 0; i++)
        if (text[i] == '\n')
            room--;
    if (room <= 0)
        return;

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
