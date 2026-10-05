// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// MATCH, 234 of 234 bytes.
//
// The two spellings below are what make this compile to the original's bytes,
// and neither one is the obvious way to write it, so both are load bearing:
//
//  * The y clip is the plain if/else
//        if (y < 0) { srcRow = -y; dstRow = 0; } else { srcRow = 0; dstRow = y; }
//    which is the only shape that gives the original's 14 byte block
//    (jns / neg eax / mov ebp,eax / xor edx,edx / jmp / xor ebp,ebp / mov edx,eax).
//    Written instead as an initialisation plus one override
//        srcRow = 0; dstRow = y; if (y < 0) { srcRow = -y; dstRow = 0; }
//    it compiles to the same semantics but one byte longer (235) and scores
//    85.4%: MSVC hoists `mov ebp,0 / mov edx,eax` ahead of the jns, because the
//    pre-test `srcRow = 0` must use `mov ebp,0` (the xor's flags are the ones the
//    jns reads).
//
//  * The copy-count clamp is the if/else with the arms the other way round,
//        if (src->width - srcCol > dst->width) n = dst->width;
//        else n = src->width - srcCol;
//    This is the surprising one. The obvious spelling,
//        int n = dst->width;
//        if (src->width - srcCol <= n) n = src->width - srcCol;
//    emits the same code and is byte for byte the original's layout on its own,
//    but combined with the plain if/else y clip above it drops the whole file to
//    50.5%: MSVC then puts x in EDX and dstRow in ECX and every later block moves
//    with them. Writing the clamp as an if/else instead puts the allocator back
//    where the original has it (x in ECX, srcRow in EBP, dstRow in EDX) and every
//    byte falls into place. The 96.8% version below with the clamp spelled the
//    other way round was one instruction block away, and the arms swapped as well
//    as the comparison sense gives the same 234 bytes at 97.8%.
//
// Ruled out this pass, all of them byte identical or lower: the x expression
// (`x += src->x - dst->x`, `x = x + (src->x - dst->x)`,
// `x = (src->x - dst->x) + x`, and a temporary), the inner loop trip count
// (`int i = n; while (i != 0)` gives the original's `test ebx,ebx / je`; a
// `for (i = 0; i < n; i++)` gives `jle` and grows the function), the two pointer
// expressions, the `for` head, `int i` declared three ways, and 24 declaration
// orders of srcCol/dstCol/srcRow/dstRow.
//
// `<string.h>` is part of the answer here: dropping it drops the file to 56.0%.
//
// Arg slots: the original reads its four incoming values at [esp+0x10],
// [esp+0x18], [esp+0x1c] and [esp+0x20] after its four pushes, skipping
// [esp+0x14]. A 4-argument __stdcall declaration compiles to exactly those
// offsets in this toolchain, so the signature below is right as written, and the
// four argument slots are then reused as the spill slots for dstCol, srcCol, n
// and the loop counter i, in that order.
//
// Suspected bugs:
//  - The horizontal copy count `n` is clamped to dst->width but dstCol is not
//    subtracted from it, so when dstCol > 0 the inner loop writes up to dstCol
//    bytes past the end of the destination row. The vertical clip does account
//    for dstRow. Kept as the original does.
//  - The inner loop guards on `n == 0` and not on `n <= 0`, so a negative n
//    (possible when srcCol > src->width, i.e. when x is large) counts down to
//    zero and wraps round, overwriting the row about 2^32 times.

#include <string.h>

struct Image_004b9d70 {
    unsigned short width;   // +0x0
    unsigned short height;  // +0x2
    short x;                // +0x4
    short y;                // +0x6
    unsigned char colorKey; // +0x8
    char unknown_9[7];      // +0x9
    unsigned char* data;    // +0x10
};

// Clipped 8-bit sprite blit: copies the part of `src` that overlaps `dst`,
// skipping source pixels equal to the source colour key and writing the
// destination colour key.
// FUNCTION: 0x4b9d70
void __stdcall FUN_004b9d70(Image_004b9d70* src, Image_004b9d70* dst, int x, int y)
{
    int srcCol, dstCol, srcRow, dstRow;
    x += src->x - dst->x;
    y = (dst->y - src->y) - y;
    if (y < 0) {
        srcRow = -y;
        dstRow = 0;
    } else {
        srcRow = 0;
        dstRow = y;
    }

    dstCol = -x;
    if (dstCol < 0) { srcCol = -dstCol; dstCol = 0; } else { srcCol = 0; }

    int n;
    if (src->width - srcCol > dst->width)
        n = dst->width;
    else
        n = src->width - srcCol;

    for (; srcRow < src->height; srcRow++, dstRow++) {
        if (dstRow >= dst->height)
            break;
        unsigned char* s = src->data + srcRow * src->width + srcCol;
        unsigned char* d = dst->data + dstRow * dst->width + dstCol;
        int i = n;
        while (i != 0) {
            if (*s != src->colorKey)
                *d = dst->colorKey;
            s++;
            d++;
            i--;
        }
    }
}