// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, deepseek-v4.1-flash, space-bunny-free, deepseek-v4.1-flash, GPT-6.1-sol and mimo-v2.6-pro. Names are provisional.
//
// Function: builds a word-wrapped copy of `text` in a buffer allocated from
// the pool: the number of characters per line is width / (width of one
// digit), and a line is broken at a space, newline or '-' when the measured
// line would exceed `width` pixels. `index` selects the current font entry
// (SelectFontForEntry) when it is not -1; measurements go through the same
// GetTextPixelWidth / GetFont+GetTextWidth pair the sibling text fitter
// 0x4ac610 uses.
// The preheader size estimate `len + 3 * (len / (width / w)) + 2` allocates
// room for the CRLF pairs; the wrap-back loop overwrites the break character
// with the CR, so `s` is only advanced past it once.
#include <string.h>

struct Gadget_004ac4c0;
struct Dialog_004ac4c0 {
    int unknown_0;
    Gadget_004ac4c0* gadgets;
};
struct Menu_004ac4c0 {
    char unknown_0[0x18];
    Dialog_004ac4c0* dialog;
};

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
int __stdcall SelectFontForEntry(Gadget_004ac4c0* gadgets, int index);
int __stdcall GetTextPixelWidth(unsigned char* text);
int GetFont();
int __stdcall GetTextWidth(int font, unsigned char* text);

// FUNCTION: 0x4ac4c0
char* __stdcall WordWrapText(Menu_004ac4c0* menu, char* text, int width, int index)
{
    Gadget_004ac4c0* gadgets = menu->dialog->gadgets;
    // Net-zero update: makes the allocator count an early use of text.
    text += 1; text -= 1;
    int len = strlen(text);
    if (index != -1)
        SelectFontForEntry(gadgets, index);
    int w;
    if (index == -1)
        w = GetTextPixelWidth((unsigned char*)"d");
    else
        w = GetTextWidth(GetFont(), (unsigned char*)"d");
    int size = len + 3 * (len / (width / w)) + 2;
    char* buf = (char*)FUN_004d83b0("WordWrap", size);
    memset(buf, 0, size);
    int i = 0;
    char c = *text;
    char* s = text;
    char* p = buf;
    while (c != 0) {
        if (*s == (char)0xff)
            break;
        buf[i] = *s;
        char next = s[1];
        // i is incremented before s: the source order sets the instruction order.
        i++;
        s++;
        if (next == ' ' || next == '\n' || next == '-') {
            int wrapped = 0;
            int m;
            if (index == -1)
                m = GetTextPixelWidth((unsigned char*)p);
            else
                m = GetTextWidth(GetFont(), (unsigned char*)p);
            if (width <= m) {
                buf[i] = 0;
                wrapped = 1;
                char ch = s[-1];
                i--;
                s--;
                while (ch != ' ' && ch != '-') {
                    // Net-zero update: makes the allocator count an extra use of i.
                    i += 1; i -= 1;
                    buf[i] = 0;
                    ch = s[-1];
                    i--;
                    s--;
                }
                buf[i] = '\r';
                i++;
                buf[i] = '\n';
                i++;
                s++;
            }
            if (wrapped)
                p = buf + i;
        }
        c = *s;
        if (c == '\n')
            p = buf + i + 1;
    }
    buf[i] = 0;
    return buf;
}
