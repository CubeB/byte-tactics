// Decompiled by deepseek-v4.1-flash. Names are provisional.

#include <string.h>

void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
char* __stdcall FUN_004b6af0(char* text, int n);
void __stdcall TruncateTextWithEllipsis(int a1, char* text, int a3, int a4, int a5);

// Builds a block of NUL-terminated item strings from the text blob at a3
// (one name per line) and copies it back over a3. Each line is truncated to
// 100 bytes, expanded by TruncateTextWithEllipsis (marker glyphs) and appended in place.
// FUNCTION: 0x4a31c0
void __stdcall FUN_004a31c0(int a1, int a2, char* a3, int a4)
{
    char* items = (char*)FUN_004d83b0("SCROLLITEMS", *(int*)(a3 - 0x44));
    char* out = items;
    for (int i = 0; i < a4; i++) {
        char buf[100];
        strncpy(buf, FUN_004b6af0(a3, i), 100);
        TruncateTextWithEllipsis(a1, buf, a2, -1, 0);
        strcpy(out, buf);
        out += strlen(buf);
        *out = 0;
        out++;
    }
    memcpy(a3, items, out - items);
}
