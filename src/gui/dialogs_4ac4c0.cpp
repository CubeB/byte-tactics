// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, deepseek-v4.1-flash, space-bunny-free, deepseek-v4.1-flash, GPT-6.1-sol and mimo-v2.6-pro. Names are provisional.
// mimo-v2.6-pro: MATCH (325 bytes). What fixed the two long-standing defects:
//
//   1. The esi/edi swap of i and s, and the preheader def schedule that
//      follows it, is a register-priority tie. One extra *weighted* reference
//      pair for i flips it. The pair that works is `i += 1; i -= 1;` written
//      in the loop: it compiles to zero bytes (the two updates fold away
//      completely, no inc/dec is emitted) but the allocator still counts the
//      reads/writes, so i outranks s and takes esi. All of these did NOT flip
//      it: `i += 0;`, a doubled `buf[i] = 0;` store (CSE'd away before
//      weighting), a dead `int j = i;`, a comma `(i, buf + i)`, an inline
//      `LineEnd(buf, i)` helper (beta reduced before weighting), and a real
//      `if (i < 0) break;` compare (a read alone is not enough; the trigger
//      needs the write pair). Placement matters: a pair in the inner
//      wrap-back loop or in the outer loop body flips it, the same pair in
//      the `if (width <= m)` block does not.
//   2. The 9-byte `if (text == buf) buf[0] = 1;` tail hack (which kept the
//      `text` parameter live so MSVC emits `mov ebp,[esp+0x10]` instead of
//      copy-propagating `s = text`) is replaced by the same zero-cost trick
//      on the parameter itself: `text += 1; text -= 1;` at the top of the
//      body. It folds to nothing but records reads/writes of text early, the
//      parameter stays in ebp across the calls, and the copy `mov edi, ebp`
//      survives. The tail `cmp [esp+0x18],ebx / jne / mov [ebx],1` is gone
//      and the size is exactly 325.
//   3. With the registers flipped the increment source order has to be
//      i first, s second (`i++; s++;` at the loop top and
//      `buf[i]='\n'; i++; s++;` after the CRLF stores); the old order only
//      matched by accident while the registers were swapped.
//
// WARNING: the two `+= 1; -= 1;` pairs are diagnostic-shaped. They are the
// only spelling found that is byte-free and moves the allocator, but the
// natural construct Cavedog actually wrote is unknown; it folds exactly like
// a net-zero read/modify/write pair of the same variable. If a cleaner
// spelling is found (an inlined helper whose body touches text/i, or a
// duplicated update the back end folds), swap it in and re-check.
//
// Function: builds a word-wrapped copy of `text` in a buffer allocated from
// the pool: the number of characters per line is width / (width of one
// digit), and a line is broken at a space, newline or '-' when the measured
// line would exceed `width` pixels. `index` selects the current font entry
// (FUN_004a1810) when it is not -1; measurements go through the same
// FUN_004a5030 / FUN_004c1440+FUN_004c1480 pair the sibling text fitter
// 0x4ac610 uses (likely via the same kind of inlined Measure helper).
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
int __stdcall FUN_004a1810(Gadget_004ac4c0* gadgets, int index);
int __stdcall FUN_004a5030(unsigned char* text);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, unsigned char* text);

// FUNCTION: 0x4ac4c0
char* __stdcall FUN_004ac4c0(Menu_004ac4c0* menu, char* text, int width, int index)
{
    Gadget_004ac4c0* gadgets = menu->dialog->gadgets;
    text += 1; text -= 1;
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
        if (*s == (char)0xff)
            break;
        buf[i] = *s;
        char next = s[1];
        i++;
        s++;
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
                while (ch != ' ' && ch != '-') {
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
