// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, best 80.0%, and 235 bytes against the original's 234: the WHOLE LOOP
// IS NOW BYTE EXACT and the prologue is one construct away. Where the last
// pass stood (76.3%), and what moved it:
//
//  1. THE INNER LOOP TRIP COUNT IS SOLVED (70.7 -> 77.4 with everything else
//     below). Write it as `int i = n; while (i != 0) { ...; i--; }` and the
//     guard becomes the original's `test ebx,ebx / je`. A
//     `for (int i = 0; i < n; i++)` gives `jle`, because MSVC strength-reduces
//     it to a countdown with a signed trip-count test, and an explicit
//     `if (i) do {} while (--i);` is worse still: it grows the function to 256
//     bytes. The `while` form is byte exact for the whole inner loop, the
//     outer loop and the `n` clamp, so nothing but the prologue is left.
//
//  2. THE CLIP MUST BE WRITTEN AS AN INITIALISATION PLUS ONE OVERRIDE, and
//     that is what puts the x clip in its original position (worth 2.6
//     points). The original emits, in one basic block: the x tree, the y tree,
//     THE Y CLIP, THEN THE X CLIP. So the source order has to be x tree, y
//     tree, y clip, x clip, and the original's `neg ecx` (0x4b9daa) with its
//     store to the dead argument slot [esp+0x14] is the x clip's first
//     instruction, not the x tree's.
//
//     Writing the y clip as
//         int sy = (dst->y - src->y) - y;
//         srcRow = 0;
//         dstRow = sy;
//         if (sy < 0) { srcRow = -sy; dstRow = 0; }
//     is what stops MSVC 5 from hoisting the y tree's loads into the x tree's
//     leaf sequence, and it lands the x clip exactly where the original has
//     it, in ECX, with the same two `neg`s. Every plain `if (sy < 0) {...} else
//     {...}` spelling, with the y clip first, hoists FOUR loads before the
//     first `sub` and spills the x argument to EBP (10 forms in this pass, 40
//     in the previous one, all byte identical: src->x->EDX, dst->x->EAX,
//     src->y->ECX, x->EBP), and scores 52.7% or less.
//
//  3. WHAT IS STILL WRONG, and it is 1 byte plus one register order:
//     a. This file's y clip is the shape from note 2, and its pre-test
//        `srcRow = 0;` costs `mov ebp,0` (5 bytes) where the original's else
//        arm has `xor ebp,ebp` (2 bytes) plus a `jmp` (2) that this shape does
//        not need: 15 bytes against the original's 14, hence 235 against 234,
//        hence every jump target in the loop is off by one. The original's y
//        clip is definitely the ordinary if/else, so the shape in note 2 is a
//        diagnostic, not the answer.
//     b. The x tree still allocates src->x->ECX, dst->x->EAX, src->y->EDX,
//        x->EBP where the original has src->x->EDX, dst->x->EAX, x->ECX and
//        then the y tree. The original spills nothing here, so the fix is to
//        stop the y tree's first load from being hoisted at all (note 2 shows
//        the init form stops it, but only by changing the block shape).
//     c. The order of the four field/argument loads is decided before the
//        allocator runs, by a load scheduler, not by the allocator. Fixing b
//        is probably a scheduling fix, not a register fix.
//
//  4. A 234-byte version of this file, the one the previous pass left, is in
//     build/scratch/0x4b9d70/best_234.cpp. It scores 77.4% (plain if/else y
//     clip, exact size, exact y clip block) and differs from the original only
//     in that the x clip is emitted before the y tree and its neg is on EAX
//     instead of ECX.
//
//  5. The original's x clip is `dstCol = -sx;` where
//     `int sx = x + (src->x - dst->x);`: unconditional `neg`, store, `jns`,
//     then a second `neg` into [esp+0x18], which is `srcCol = -dstCol`. No
//     other spelling survives, and in the shape above the neg comes for free
//     (no dead second use of sx needed any more; in the 234-byte shape
//     `if (sx == 0) sx = 0;` is what keeps it, worth 6.7 points).
//
//  6. THIS PASS RULED OUT (do not repeat):
//     a. Every incremental arithmetic spelling of `sx` and `sy`: `x + (a-b)`,
//        `(a-b) + x`, `x - (b-a)`, a `dx`/`dy` temporary, `sx` split into
//        `int sx = x; sx += ...;`, and combined temps all compile to the SAME
//        prologue at 80%. Parenthesisation is not a lever (guide item 20).
//     b. Local declaration order (srcCol/dstCol first or last, split lines,
//        one line) does not change the register choice.
//     c. `tools/headers.py`: 128 sets tried, none reaches MATCH, best 80.0%.
//     d. The N-declarations test is FLAT between the two source states only:
//        N = 0..408 in steps of 4 gives either 80.0% or 56.0% (never MATCH),
//        so this is a source-shape problem, not compiler state.
//     e. All plain-if/else y-clip forms (`h_dstfirst`, `h_sxfirst`, `h_sy2`,
//        `h_yfirst2`, a `static inline` subtraction helper) hoist and score
//        52.7%. The pre-initialisation in note 2 is the only shape above that.
//     f. `dstRow = sy; srcRow = 0;` (no srcRow pre-init, so the else would be
//        `xor ebp,ebp`) gives a THIRD allocation: x lands in EBP and the whole
//        `srcy` move is merged into EBP. Also 80%, different bytes.
//
//  7. Remaining diff, in one sentence: the original's x-tree temporary `x`
//     lives in ECX and its `src->y` load is scheduled AFTER the `add ecx,edx`
//     that finishes sx; ours puts `x` in EBP and hoists `src->y` above that
//     add (the y clip itself, x clip, loop and frame are all byte exact).
//
//  8. THIS PASS (deepseek-v4.1-flash) confirmed the two halves are mutually
//     exclusive with this compiler. The plain if/else y clip IS the original's
//     14-byte block, and it does produce the original's first two loads
//     (src->x in EDX, dst->x in EAX) at 234 bytes, but it then hoists src->y
//     into ECX and leaks the x clip before the y tree: 52.7%. The
//     init-plus-override shape here keeps the block order and scores 80.0% but
//     loses those two registers. All of these leave the prologue byte
//     identical at 80.0%: statement order, `int sx = x; sx += ...;`,
//     `(src->x - dst->x) + x`, `x - (dst->x - src->x)`, a dx local, x/y copied
//     to locals, declaration-vs-assignment locals, and
//     `int srcRow = 0, dstRow = sy;`. The sy statement first is 79.6%. An
//     `else { srcRow = 0; }` on the y clip gives the exact 234 bytes but
//     74.2%. Dropping <string.h> drops to 56.0%, so the include is part of the
//     header state and must stay. Nothing reached both the exact y-clip block
//     and the original's x register at once.
//
//  9. THIS PASS ALSO RULED OUT (plain if/else y clip, all still 52.7% or
//     worse): an inline `dsub`/getter helper for the two field differences,
//     `x`/`y` copied into locals, second pointer locals `s`/`d`, no
//     parentheses, `dst->y`/`src->y` read into locals first, a `static inline`
//     x/y-offset helper writing both results through int* out-params, a
//     `static inline` clip-helper taking `int*`, a ternary per row
//     (`srcRow = sy < 0 ? -sy : 0;`), dstRow-assignment-first arms, reversed
//     branches, and a dummy function prepended to the file. Confirmed: any
//     plain-if/else spelling keeps `src->y` in ECX before the sx add and
//     spills dstRow to ECX (the original has src->y in EDX late and dstRow in
//     EDX). The init shape here is the only one that keeps sx in ECX. The
//     remaining gap is a load-scheduler tie-break between the x and y trees.
//
// Arg slots: the original reads its four incoming values at [esp+0x10],
// [esp+0x18], [esp+0x1c] and [esp+0x20] after its four pushes, skipping
// [esp+0x14]. A 4-argument __stdcall declaration compiles to exactly those
// offsets in this toolchain, so the signature below is right as written, and
// the four argument slots are then reused as the spill slots for dstCol,
// srcCol, n and the loop counter i.
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
