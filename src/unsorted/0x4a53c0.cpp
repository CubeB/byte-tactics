// Decompiled by space-bunny-free. Names are provisional.
//
// PARTIAL: 61.5%, 703 against 748 bytes. What the function is: it walks the
// entry list of a layout object, picks the language for the n-th entry by
// walking the tab stops (entries whose +0x00 byte is 7, counting only those,
// until the counter reaches the entry's +0x28 tab index), sets the line
// height, and then lays the entry's text out right aligned (+0x1b bit 2),
// centred (+0x1b bit 1) or left as measured (bit 0), writing the new x, the
// new width and the line height back into the entry.
//
// The struct shape (0x15b entry stride, packed, +0x13 x / +0x17 w / +0x19 h)
// is copied from the matched sibling 0x4a4660, which uses the same array. The
// +0xb6 field is a union: a signed short count on entry 0 (the loop bound) and
// the NUL terminated text of every other entry.
//
// Three findings took this from 52.0% to 61.5%, and all three are about the
// inlined Measure helper, not about the main function:
//
//  1. the accumulator must be declared at the TOP of the helper, before the
//     two early returns (52.0% to about 55%). With `int total = 0;` after them
//     MSVC puts the entry's x local in a stack slot ([esp+0x14]) instead of a
//     register, and every use of it then reloads from the stack, which
//     rewrites the whole tail of the function;
//  2. the helper must keep `char* p = text;` as a separate variable and the
//     call must pass `text`, not `p` (55% to 64%). `FUN_004c1480(FUN_004c1440(),
//     p)` is identical C but scores 4% lower;
//  3. the two early `return`s must be spelled `return 0;` and `return
//     FUN_004c1480(...)` while the loop exit is `return width;`. All three
//     exits then stay distinct expressions, and MSVC tail-duplicates the
//     store-and-return block for each of the three inlined copies. Folding
//     them into one join (`if (p != 0) { ... } return width;`, which reads
//     better) shares one block and loses 9%.
//
// Ruled out, all measured: the loop bound spelled `i <= count` (the original
// computes count+1 once and tests `i < count+1`); the exit test spelled
// `i > count` (it is `i == count + 1`, and it re-reads the count from memory);
// `x` as short, unsigned short, long, unsigned int or volatile; `x` and `lh`
// declared at the top of the function in all 24 orders; the ternary spelled
// `type ? x : 0`, `type && x`, `!type ? 0 : x`, if/else, or with a separate
// `xt` temporary (the `!type ? 0 : x` form is the one kept, it is the only one
// that gets the `jne` to the loaded arm right); the whole if/else-if chain
// instead of early returns; the three measure loops written out by hand
// instead of inlined; Measure as a real call; lh or x computed by a static
// inline helper; `register` on x or lh (no effect at all, as expected).
//
// What is left, and the one thing I could not move: the original keeps TWO
// copies of x, one in ebx and one in esi. It loads the ternary into esi
// (`movsx esi, word [ebp+0x13]`, `xor esi,esi`), copies esi to ebx, and then
// the bit 2 and bit 1 arms consume esi (because they first overwrite ebx with
// `movsx ebx, word [ebp+0x17]; add ebx, esi`), while the tail reads ebx. Ours
// has only ebx, so the arms do not clobber it and the copy is not needed. The
// cause is visible one step earlier: the original computes the arms' x partial
// in 32 bits BEFORE the Measure call, so ebx is already busy with `w + x` and
// the call's return value lands in eax; ours narrows the same expression to
// 16 bits (`mov cx, word [ebp+0x17]`, `sub cx, ax`, `add ecx, ebx`) because
// the result is only stored to a 16-bit field, so the partial is computed
// after the call and ebx is free for the accumulator. Every spelling I tried
// to force 32 bits (`int nx = ...; entry->x = (short)nx;`, `int nw = w + x;`
// as its own statement, dropping the `(short)` cast, `int m = Measure(...);`
// first) restores the 32-bit arithmetic but then merges the null-string exit
// block with the loop exit instead of duplicating it, which is worth more
// than the 32-bit arithmetic gains. That trade is the next thing to attack:
// it needs a shape where the partial is 32-bit and the three exits stay
// distinct.
//
// Smaller leftovers: the two stores in each arm come out as h then x where the
// original has x then h, and the epilogue's `pop edi` lands one store earlier
// in the final arm. Both are statement-order and scheduling effects that
// resisted every order I tried (x/h both ways, w/x/h, w/h/x).
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a53c0 {
    unsigned char type;                 // +0x00
    char unknown_01[0x12];
    short x;                            // +0x13
    char unknown_15[2];
    short w;                            // +0x17
    short h;                            // +0x19
    unsigned char align;                // +0x1b
    char unknown_1c[0x0c];
    signed char tab;                    // +0x28
    char unknown_29[0x8d];
    union {
        short count;                    // +0xb6 on entry 0
        char text[0xd6 - 0xb6];
    } b6;
    int language;                       // +0xd6
    char unknown_da[0x15b - 0xda];
};
#pragma pack(pop)

struct Holder_004a53c0 {
    char unknown_0[4];
    Entry_004a53c0* entries;
};

struct Class_004a53c0 {
    char unknown_0[0x18];
    Holder_004a53c0* holder;
};

struct Glyph_004a53c0 { unsigned short width, height; };
struct Language_004a53c0 { char unknown_0[0xc]; unsigned short* glyphs; };
struct LanguageRoot_004a53c0 {
    int language0;                      // +0x0
    char unknown_4[0x10];
    Language_004a53c0* language;        // +0x14
};

extern LanguageRoot_004a53c0* DAT_0051fba4;

void __stdcall FUN_004c1420(int param);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, char* text);
int FUN_004c1450();
int __stdcall FUN_004b7f30(unsigned short* glyphs, int c);

static inline int Measure_004a53c0(char* text)
{
    int width = 0;
    char* p = text;
    if (p == 0)
        return 0;
    if (DAT_0051fba4->language == 0)
        return FUN_004c1480(FUN_004c1440(), text);
    while (*p != 0) {
        char ch = *p;
        Glyph_004a53c0* glyph = (Glyph_004a53c0*)FUN_004b7f30(DAT_0051fba4->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++p;
    }
    return width;
}

// FUNCTION: 0x4a53c0
void __stdcall FUN_004a53c0(Class_004a53c0* obj, int index)
{
    Entry_004a53c0* entries = obj->holder->entries;
    Entry_004a53c0* entry = &entries[index];
    int i;
    int t = 0;
    for (i = 1; i < entries[0].b6.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entry->tab) {
                FUN_004c1420(entries[i].language);
                break;
            }
            t++;
        }
    }
    if (i == entries[0].b6.count + 1)
        FUN_004c1420(DAT_0051fba4->language0);
    int x = !entry->type ? 0 : entry->x;
    int lh;
    if (DAT_0051fba4->language == 0)
        lh = FUN_004c1450();
    else
        lh = ((Glyph_004a53c0*)FUN_004b7f30(DAT_0051fba4->language->glyphs, 0x49))->height + 2;
    if (entry->align & 4) {
        entry->x = (short)(entry->w + x - Measure_004a53c0(entry->b6.text));
        entry->h = (short)lh;
        return;
    }
    if (entry->align & 2) {
        int newx = entry->w / 2 / 2 + x;
        int half = Measure_004a53c0(entry->b6.text) / 2;
        entry->w = (short)(half * 2);
        entry->x = (short)(newx - half);
        entry->h = (short)lh;
        return;
    }
    if (entry->align & 1)
        entry->w = (short)Measure_004a53c0(entry->b6.text);
    entry->x = (short)x;
    entry->h = (short)lh;
}
