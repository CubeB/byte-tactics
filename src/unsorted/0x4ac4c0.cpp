// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free. Names are provisional.
// GAVE UP: 67.7% match. Register allocation and stack layout differences.
// Builds a word-wrapped copy of `text` in a buffer allocated from the pool:
// the number of characters per line is width / (width of one digit), and a
// line is broken at a space or '-' when a word would exceed `width` pixels.
// `index` selects the current font entry (FUN_004a1810) when it is not -1.
//
// Still differs: the original keeps the `text` parameter as a dead home in
// `ebp` for the whole loop and uses `edi` as the active input pointer with the
// output index `i` in `esi` and `wrapped` reusing `ebp`. Every source form
// tried here (a plain `text` loop, a separate `char* s = text;` loop) either
// lets MSVC coalesce the copy into one register (text ends in `esi`, i in
// `edi`) or keeps `text` live through the loop (then `wrapped` spills). The
// head of the function through the memset matches exactly; the loop body and
// the `wrapped` allocation are what remain.
#include <string.h>

struct Gadget_004ac4c0;
struct Dialog_004ac4c0 {
    int unknown_0;
    Gadget_004ac4c0* gadgets;      // +0x4
};
struct Menu_004ac4c0 {
    char unknown_0[0x18];
    Dialog_004ac4c0* dialog;       // +0x18
};

void* FUN_004d83b0(char* name, unsigned int size);
int __stdcall FUN_004a1810(Gadget_004ac4c0* gadgets, int index);
int __stdcall FUN_004a5030(unsigned char* text);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, unsigned char* text);

// FUNCTION: 0x4ac4c0
char* __stdcall FUN_004ac4c0(Menu_004ac4c0* menu, char* text, int width, int index)
{
    Gadget_004ac4c0* gadgets = menu->dialog->gadgets;
    int len = strlen(text);
    if (index != -1)
        FUN_004a1810(gadgets, index);
    int w;
    if (index == -1)
        w = FUN_004a5030((unsigned char*)"d");
    else
        w = FUN_004c1480(FUN_004c1440(), (unsigned char*)"d");
    int size = len + 3 * (len / (width / w)) + 2;
    char* buf = (char*)FUN_004d83b0("WordWrap", size);
    memset(buf, 0, size);
    int i = 0;
    char c = *text;
    char* s = text;
    char* p = buf;
    while (c != 0) {
        if (*text == (char)0xff)
            break;
        buf[i] = c;
        char next = s[1];
        i++;
        s++;
        text++;
        if (next == ' ' || next == '\n' || next == '-') {
            int wrapped = 0;
            int m;
            if (index == -1)
                m = FUN_004a5030((unsigned char*)p);
            else
                m = FUN_004c1480(FUN_004c1440(), (unsigned char*)p);
            if (width <= m) {
                buf[i] = 0;
                wrapped = 1;
                char ch = s[-1];
                i--;
                s--;
                text--;
                while (ch != ' ' && ch != '-') {
                    buf[i] = 0;
                    ch = s[-1];
                    i--;
                    s--;
                    text--;
                }
                buf[i] = '\r';
                i++;
                buf[i] = '\n';
                i++;
                s++;
                text++;
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
