// Decompiled by Space Bunny Free. Names are provisional.
//
// PARTIAL: 80.1%, 497 against 485 bytes. The signature, the word-wrap
// algorithm, the inlined line-height helper, the strlen, the character scan,
// the glyph-width loop, the call sequence and the epilogue all match. Three
// changes took this from the previous attempt's 62.4%, in this order:
//
//  1. extracting the measure into a `static inline int Measure(char* word)`
//     (62.4% to 68.8%). As an inline block inside the loop it scored 62.4% to
//     65.3% depending on how the guard was spelled; as a helper it is much
//     better, because the helper's own accumulator changes what the caller has
//     to keep in registers;
//  2. the local declaration order, with `len` declared last (68.8% to 70.5%).
//     Seventy-two permutations of `i`, `w`, `last`, `k` and the position of
//     `len` were swept; eight tie at 70.5% and the rest land between 62.4% and
//     69.9%. Note this only pays off *after* the helper: the previous attempt
//     swept the same permutations without the helper and found that every order
//     which fixed the slots moved `text` out of ebp, scoring 54.5%. Changing
//     the helper changed the pressure that was holding `text` in place, so an
//     earlier negative result was no longer negative;
//  3. the outer loop written as `while (text[0] != 0)` with a `break` when the
//     text runs out (70.5% to 80.1%). The original enters the loop body by an
//     explicit `jmp` to the loop head, which only happens if the head is a
//     separate block; a `do { } while (text[k])`, a `for(;;)` with the test at
//     the bottom, a `for(;;)` with the test at the top and a `while (text[k])`
//     all fall straight through and score 69.9% to 70.5%.
//
// What is left, 12 bytes. The original stores the zero of the width local
// (`mov dword ptr [esp+0x14], eax`) before the `je` that skips the measure, in
// the same basic block as the `text[i] = 0` store and the `word` compare; ours
// emits the same store after the font-is-null path, in the fall-through block.
// Repositioning the source does not move it: an explicit `else w = 0`, the
// ternary `w = word ? Measure(word) : 0`, assigning through a third local,
// `w = 0` written before the `word` and `text[i]` statements, and the positive
// `if (word)` guard all give the identical 80.1% bytes, while moving the
// zeroing earlier drops it to 65.9%. MSVC sinks the store into the block that
// uses it regardless of where the source puts it.
//
// Ruled out for the rest: about thirty orderings of the cursor and line-height
// statements, and every header set headers.py tries. The cursor `y` is still
// register-based here (edi) where the original keeps it in its incoming argument
// slot and reloads it, which is the largest single remaining item and is where
// the second return block comes from.
#include <string.h>

struct Glyph_004a50b0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Font_004a50b0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Class_0051fba4 {
    char unknown_0[0x14];
    Font_004a50b0* font;               // +0x14
};

extern Class_0051fba4* DAT_0051fba4;

void* __stdcall FUN_004b7f30(void* glyphs, int c);
int FUN_004c1440();
int __stdcall FUN_004c1480(int a, char* text);
int FUN_004c1450();
void __stdcall FUN_004a50e0(char* dest, char* text, int p3, int x, int maxw, int style);


static inline int LineHeight_004a50b0()
{
    if (DAT_0051fba4->font == 0)
        return FUN_004c1450();
    return (int)((Glyph_004a50b0*)FUN_004b7f30(DAT_0051fba4->font->glyphs, 'I'))->height + 2;
}

static inline int Measure(char* word)
{
    if (DAT_0051fba4->font == 0)
        return FUN_004c1480(FUN_004c1440(), word);
    int t = 0;
    for (char* n = word; *n; n++) {
        unsigned char ch = *n;
        unsigned short* g = (unsigned short*)FUN_004b7f30(DAT_0051fba4->font->glyphs, ch);
        if (g)
            t += *g;
    }
    return t;
}

// FUNCTION: 0x4a51d0
int __stdcall FUN_004a51d0(char* p2, char* text, int p4, int y, int maxw, int rem, int p7)
{
    int last = 0;
    int w;
    int k = 0;
    int i = 0;
    int len = strlen(text);
    while (text[0] != 0) {
        while (i != len && text[i] != ' ' && text[i] != '\r')
            i++;
        char saved = text[i];
        char* word = text + k;
        text[i] = 0;
        w = 0;
        if (word != 0)
            w = Measure(word);
        if (w > maxw) {
            text[i] = saved;
            i = last;
            saved = text[i];
            text[i] = 0;
        } else if (saved != '\r' && i != len) {
            last = i;
            text[i] = saved;
            i++;
            continue;
        }
        FUN_004a50e0(p2, word, p4, y, maxw, p7);
        text[i] = saved;
        y += LineHeight_004a50b0() + 2;
        rem -= LineHeight_004a50b0() + 2;
        if (text[i] == 0)
            return y;
        k = i + 1;
        if (rem <= 0)
            return y;
        last = i = k;
        if (!text[k])
            break;
    }
    return y;
}
