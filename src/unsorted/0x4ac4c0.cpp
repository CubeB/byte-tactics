// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// 72.4% match. Builds a word-wrapped copy of `text` in a buffer allocated
// from the pool: the number of characters per line is width / (width of one
// digit), and a line is broken at a space or '-' when a word would exceed
// `width` pixels. `index` selects the current font entry (FUN_004a1810) when
// it is not -1.
//
// The head (through the memset) matches the original exactly once `text` is
// forced to stay live across the whole loop: the original keeps `text` in
// ebp (its only reads are the initial `*text` and `s = text`), which demotes
// `len` to esi and `index` to edi. A trailing use of `text` after the loop
// reproduces that allocation. What still differs:
//   * the loop counter and the input pointer have swapped callee-saved
//     registers (i in edi and s in esi here; the original has i in esi and
//     s in edi). Declaration order, int/unsigned/long, and adding live
//     locals all failed to swap them.
//   * the trailing `if (text == buf) buf[0] = 1;` that keeps `text` live is a
//     real instruction pair the original does not have (8 extra bytes).
// All other bytes are exact; the wrap-back loop and the tail match.
//
// space-bunny-free, 72.4%, same two diffs, no improvement:
//   * Removing the trailing use entirely (so `text` is only read by `*text`
//     and `s = text`) collapses `text` out of ebp entirely and the score falls
//     to 60.8%: with only those two adjacent uses MSVC 5 copy-propagates
//     `text` into `s` and `text` dies at the last call, so the head rewrites
//     (`mov ebp,[esp+0x10]` becomes `mov esi,[esp+0x14]`). The original really
//     does keep the parameter in ebp, so some use of it past the calls is real
//     and the cheapest one available is that 8-byte pair.
//   * The esi/edi swap of `i` and `s` is completely insensitive to the source
//     shape. All of these emit the identical 333 bytes, same swap: the six
//     declaration orders of i/c/s/p, `unsigned`/`long` for i, `unsigned char*`
//     for s, `const char*` for s, and splitting the declarations from the
//     assignments. It is not a tie-break on definition order. What the
//     allocator seems to use is the number of IR references: `i` is
//     referenced more often (3 incs, 2 decs, 6 indexed stores, 2 leas) than
//     `s` (1 def, 4 incs/decs, 5 loads) yet `s` wins esi, so a reference has
//     to be added to `s` or removed from `i` without emitting an instruction,
//     and no zero-cost spelling of that was found.
//   * Rewriting the loop as `while (1) { if (c == 0) break; ... }` (technique
//     9) does not swap them either, it only adds 2 bytes.
//   * The def of `i` is emitted in the slot between the two halves of the
//     inlined memset and the def of `s` just after the loop-entry `test al,al`;
//     in this version those two slots are the other way round. Whatever
//     decides which variable wins that slot is upstream of everything the
//     source spelling can reach from here.
//   * Scoring note: rewriting the 0xff test as `if (c == (char)0xff) break;`
//     scores 73.2% / 332 bytes, one byte better, but it keeps `c` in al across
//     the latch and loses the original's `mov al,[edi]` reload at the top of
//     the loop body, so the instruction sequence stops being the original's
//     with two registers renamed. This file keeps the exact-shape version,
//     which is one register fix away from a match, not two.
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
        if (*s == (char)0xff)
            break;
        buf[i] = c;
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
    if (text == buf)
        buf[0] = 1;
    buf[i] = 0;
    return buf;
}
