// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// PARTIAL, best 80.0%, and 235 bytes against the original's 234: the LOOP, the
// n CLAMP, both clips and the frame are byte exact, and the whole remaining
// diff is the 29-byte PROLOGUE (the order of the six field/argument loads) plus
// the one extra byte that shifts every later jump target.
//
// Retry (deepseek-v4.1-flash, issue 2414): re-confirmed 80.0%, no variant
// improved it. Two suspects found while reading the clips:
//   * n is clamped to dst->width without subtracting dstCol, so when dstCol > 0
//     the loop writes up to dstCol bytes past the destination row end (the
//     vertical clip does account for dstRow).
//   * the inner loop guards n == 0 rather than n <= 0, so a negative n
//     (srcCol > src->width) counts down through wraparound instead of stopping.
//
// WHAT IS STILL WRONG, precisely: the original's prologue is
//     movsx edx,[esi+4] / movsx eax,[edi+4] / mov ecx,[esp+0x1c] / sub edx,eax
//     movsx eax,[edi+6] / add ecx,edx / movsx edx,[esi+6] / sub eax,edx
//     mov edx,[esp+0x20] / sub eax,edx
// so the x tree's three leaves are requested and consumed in source order
// (src->x, dst->x, x in ECX) and only then does the y tree start. This file
// asks for them in the order src->x, dst->x, src->y, x, dst->y, y, which puts
// x in EBP and the y clip's dstRow in EDX only by luck of the init form.
//
// THE Y CLIP IS THE WHOLE STORY, and the two halves of it are mutually
// exclusive with this compiler (three passes, about 700 scored shapes):
//   * The plain if/else spelling
//         if (sy < 0) { srcRow = -sy; dstRow = 0; } else { srcRow = 0; dstRow = sy; }
//     is the only one that is the original's exact 14-byte block, and it gives
//     the original's 234 bytes. But it puts dstRow in ECX and sx in EDX, which
//     moves dstCol into x's dead argument slot and n out of the stack slot the
//     loop reads it from: 52.7%.
//   * The initialisation plus override below keeps the original's register
//     choice (dstRow in EDX, sx in ECX, the x clip landing in arg1's slot) and
//     scores 80.0%, but its pre-test `srcRow = 0` needs `mov ebp,0` (5 bytes)
//     where the original's else arm has `xor ebp,ebp` (2), so the block is 15
//     bytes and everything after it is off by one. MSVC 5 cannot use the xor
//     before the branch, because EBP's incoming value is still live there and
//     only dead in the else arm.
//
// RULED OUT THIS PASS (do not repeat):
//   * 7 spellings of sx x 4 of sy x 2 statement orders x 2 tree orders:
//     identical 80.0%/235 for every one. Parenthesisation and commutativity are
//     not levers; MSVC 5 canonicalises the arithmetic (guide item 20).
//   * 12 y-clip forms (if/else, init+override, >= 0, two separate ifs, a temp,
//     both ternaries, both arm orders) x 6 x-clip forms: best is this file's
//     combination at 80.0%, the next best is the same with the init form's two
//     assignments swapped (78.9%), the plain if/else with the arms' stores
//     swapped is 48.1% at 238 bytes.
//   * All 24 declaration orders of srcCol/dstCol/srcRow/dstRow: byte identical.
//     The frame slots do not follow the declaration order here, they follow the
//     order the variables are first written, so dstCol takes src's dead
//     argument slot and srcCol takes dst's, as the original does.
//   * All 128 header sets from tools/headers.py, crossed with the plain if/else
//     form, the init form and the swapped-store form: every set gives the same
//     score, so no include flips this allocation. (Crossed with 6 C++ headers
//     for the init form too: still 80.0%.)
//
// The inner loop trip count is solved and must stay: `int i = n; while (i != 0)`
// gives the original's `test ebx,ebx / je`. A `for (i = 0; i < n; i++)` gives
// `jle` (MSVC strength-reduces it to a signed countdown) and grows the function.
//
// Arg slots: the original reads its four incoming values at [esp+0x10],
// [esp+0x18], [esp+0x1c] and [esp+0x20] after its four pushes, skipping
// [esp+0x14]. A 4-argument __stdcall declaration compiles to exactly those
// offsets in this toolchain, so the signature below is right as written, and
// the four argument slots are then reused as the spill slots for dstCol,
// srcCol, n and the loop counter i, in that order.
//
// HISTORY, kept short because the notes above supersede it:
//  1. deepseek-v4.1-flash took this from 70.7% to 77.4% by solving the inner
//     loop trip count, then to 80.0% with the y clip as an initialisation plus
//     one override (the shape kept below).
//  2. Earlier passes also ruled out, and none of it is worth repeating: an
//     inline subtraction helper, x/y copied to locals, second pointer locals,
//     the y fields read into locals first, a `static inline` clip helper
//     taking `int*`, a `static inline` x/y-offset helper with int* out-params,
//     reversed comparison branches, a ternary per row, a dummy function
//     prepended to the file, and the N-declarations test (N = 0..408 in steps
//     of 4 gives only 80.0% or 56.0%, so this is a source-shape problem, not
//     compiler state).
//  3. `<string.h>` is part of the answer here: dropping it drops the file to
//     56.0%, so keep the include.
//
// Suspected original bugs:
//  - The horizontal copy count `n` is clamped to dst->width but dstCol is not
//    subtracted from it, so when dstCol > 0 the loop writes up to dstCol bytes
//    past the end of the destination row. The vertical clip does account for
//    dstRow. Kept as the original does.
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
    int sx = x + (src->x - dst->x);
    int sy = (dst->y - src->y) - y;
    srcRow = 0;
    dstRow = sy;
    if (sy < 0) {
        srcRow = -sy;
        dstRow = 0;
    }
    dstCol = -sx;
    if (dstCol < 0) {
        srcCol = -dstCol;
        dstCol = 0;
    } else {
        srcCol = 0;
    }

    int n = dst->width;
    if (src->width - srcCol <= n)
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
