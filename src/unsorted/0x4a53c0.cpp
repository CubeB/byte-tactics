// Decompiled by space-bunny-free, verified by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol refinement: four checks retained the 76.7% best; no MATCH. A
// sequential right-base local (x; += w; -= Measure) fell to 62.7%. Explicit
// casts, if/else x assignment and an explicit null check did not help. The
// register-copy mismatch described below remains the dominant difference.
//
// PARTIAL: 76.7%, 697 against 748 bytes. What the function does: it walks the
// entry list of a layout object looking for the n-th tab stop (entries whose
// +0x00 byte is 7), sets the language from that entry, sets the line height,
// and then lays the entry's text out right aligned (+0x1b bit 2), centred
// (+0x1b bit 1) or left at its measured width (bit 0), writing the new x, the
// new width and the line height back into the entry.
//
// The struct shape (0x15b entry stride, packed, +0x13 x / +0x17 w / +0x19 h /
// +0x1b a 4-byte align field) is copied from the matched sibling 0x4a4660,
// which walks the same array. The +0xb6 field is a union: a signed short count
// on entry 0 (the loop bound) and the NUL terminated text of every other entry.
//
// Two things took this from 52% to 62%:
//  1. +0x1b is a 4-byte field, not a byte: the original loads it with
//     `mov eax, dword ptr [ebp+0x1b]` and then tests al with 4, 2 and 1.
//  2. the centred arm divides the box width by 2, not by 4. `entry->w / 2 / 2`
//     compiles to two `cdq/sub/sar` triples and the original has one;
//  3. the new width has to go through its own named local (`int nw = half * 2;`,
//     62% to 77%). Assigned straight into the field, MSVC narrows the whole
//     tail and puts the stores out of order; through a local it keeps the
//     product in a register and the tail lands in the original's order.
//
// The inlined Measure helper has to be spelled exactly as it is here: the
// accumulator first, `char* p = text` kept as a separate variable with the
// call passing `text`, and three distinct return expressions (`return 0`,
// `return FUN_004c1480(...)`, `return width`) so that MSVC tail-duplicates
// the store-and-return block once per exit instead of merging them.
//
// What still differs, and the one thing I could not move: the original keeps
// TWO copies of x, one in ebx and one in esi. It loads the ternary into esi
// (`xor esi,esi` / `movsx esi, word [ebp+0x13]`), then `mov ebx, esi`. The two
// alignment arms consume esi and overwrite ebx with their own value
// (`movsx ebx, word [ebp+0x17]; add ebx, esi`), while the final tail reads ebx.
// Ours has only one copy, in ebx, so the arms clobber it and no copy is made.
// That one register difference cascades everywhere in the tail half:
//   * right arm: with no free callee-saved register, MSVC folds `x` into the
//     post-call arithmetic and narrows it to 16 bits
//     (`mov cx, word [ebp+0x17]; sub cx, ax; add ecx, ebx`) instead of the
//     original's 32-bit `movsx ebx, [ebp+0x17]; add ebx, esi` before the call
//     and `sub ebx, eax` after it. Three copies of this one hunk differ.
//   * centred arm: the new x and the width accumulator swap registers
//     (ours `mov edi, eax / sar edi,1 / add edi, ebx` with the width in ebx,
//     the original `mov ebx, eax / sar ebx,1 / add ebx, esi` with the width
//     in edi), again because edi is the only register the original had free.
//
// Spelling the arms with a named temporary reproduces the original's 32-bit
// arithmetic but costs far more than it wins: `int nx = x + entry->w;` scores
// 62.7% and `int nx = x + entry->w; int mw = Measure(...);` scores 61.6%, both
// because MSVC then merges the three Measure exits' tails into one block and
// rewrites the width accumulator and the lh store. So the copy of x is the
// lever, not the arithmetic.
//
// Also tried, all at 76.7% or worse and none of them move the copy: an extra
// named copy of x (`int xb = x`) used by the tail, the if chain written as
// else-if, `int e4 = entry->align`, the ternary spelled `entry->type ? ... : 0`,
// `x + entry->w` instead of `entry->w + x` (MSVC 5 canonicalises it, identical
// bytes), a copy of x taken inside the right arm, a named local for the width
// in the third arm, and `char* text = entry->b6.text` hoisted (49.5%, it kills
// the per-arm `lea esi, [ebp + 0xb6]`). Declaring x `short` scores 65.5%.
//
// A second session went after the copy directly and ruled out the whole
// "named local" family, measured with `check.py --sym` (so none of it cost a
// real run). Every one of these compiles to BYTE-IDENTICAL code to what is in
// the file, 697 bytes, 76.7%: `int nx = x;` with the tail storing nx; the same
// with nx declared before the loop; `short sx = (short)x;` and
// `unsigned short ux = (unsigned short)x;` as the tail's operand; `int nx = x
// + 0;`; `int x;` and `int x; int lh;` declared before the loop and only
// assigned after it (so the register allocator cannot be ordering by
// declaration); `unsigned int x`; and `int nx = x;` with the copy taken after
// the line-height computation instead of before. MSVC 5 copy-propagates all of
// them away, so the copy is NOT a source-level assignment of x.
//
// Writing the ternary as an if/else that assigns x in both arms
// (`if (!entry->type) { x = 0; nx = x; } else { x = entry->x; nx = x; }`) is
// the one shape that does produce two live values, but MSVC 5 lowers that phi
// through the STACK: it emits `mov dword ptr [esp + 0x1c], ebx` in both arms,
// grows the frame by a slot (every argument reference moves from `esp + 0x14`
// to `esp + 0x18`) and scores 65.4%. So the original's single register copy at
// the merge point is a phi that MSVC keeps in registers, and no plain C++ local
// spelling of it survives copy propagation.
//
// Swapping the source order of the x ternary and the line-height computation
// costs a byte and a percent (75.8%), so the order in the file is the right
// one.
//
// Smaller leftovers: the centred arm's text==0 exit is merged with the loop
// exit in ours and duplicated in the original; the final arm's stores are
// `mov dx, lh / mov word [+0x13], bx / pop edi / mov word [+0x19], dx` in the
// original, ours puts the `pop edi` before the x store.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a53c0 {
    unsigned char type;                 // +0x00
    char unknown_01[0x12];
    short x;                            // +0x13
    char unknown_15[2];
    short w;                            // +0x17
    short h;                            // +0x19
    int align;                          // +0x1b
    char unknown_1f[0x9];
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
        int newx = entry->w / 2 + x;
        int half = Measure_004a53c0(entry->b6.text) / 2;
        int nw = half * 2;
        entry->w = (short)nw;
        entry->x = (short)(newx - half);
        entry->h = (short)lh;
        return;
    }
    if (entry->align & 1)
        entry->w = (short)Measure_004a53c0(entry->b6.text);
    entry->x = (short)x;
    entry->h = (short)lh;
}
