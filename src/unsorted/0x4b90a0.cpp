// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Partial, best 82.8% (re-checked by space-bunny-free, no better form found).
//
// Byte accounting: ours is 256 bytes, the original 260. Everything matches
// except the row loop head, where the original is 8 bytes longer; in exchange
// our prologue is 3 bytes longer (we spill yoff before the two `jl` guards,
// the original spills it at the top of the loop head). Two differences remain:
//  1. The destination row pointers: the original adds xoff to the plane first
//     and the row stride last ((xoff + plane) + stride), evaluating the two
//     rows one after the other. Every source form tried here compiles to
//     ((plane + stride) + xoff), with both plane loads hoisted to the top of
//     the loop head and the adds grouped by term, which is MSVC 5 SP3's
//     canonical order for the expression. Tried and all identical: every
//     operand order of `xoff + plane + stride`, pointer+int both ways,
//     `+=` per term, int/pointer temps and casts, `&plane[i]` indexing,
//     an inline `yoff * dst->width` instead of the `stride` local (that one
//     moves the imul and costs 30-odd percent elsewhere), a static inline
//     row-pointer helper, and <windows.h>/<stdio.h>/<string.h>/<math.h> in
//     every combination (all much worse, 22-53%). /Ob0, /Ob1, /Ob2, /Ob3,
//     /O1 and /O all give the same code, so it is not an optimisation level.
//  2. The threshold compare. Both spellings of the same test
//     (`*sp1 + level >= *dp1` and `*dp1 <= *sp1 + level`, plus
//     `level + *sp1 >= *dp1`) load level and *sp1 into ebx/edx in the same
//     order; the original adds them into edx and compares ebx (*dp1) with it
//     (`jg`), ours adds into ebx. Operand order alone never moves the add's
//     destination register, so this looks like the same allocation decision
//     as (1): the original was built by a compiler whose canonical operand
//     order for a commutative/associative chain differs from the SP3 build
//     in this repo. (Header-block state is the guide's other explanation.)
//
// The semantics below are exact, they are what the original does: the first
// argument supplies the source planes, the colour key, the width (inner count)
// and the height (outer count); the second supplies the destination row
// pointers and the row stride. The dead `mov edx, [esp+0x20]` after the
// inner loop is a reload of x, present in both.

struct Bitmap_004b90a0 {
    unsigned short width;      // +0x0
    unsigned short height;     // +0x2
    short field_4;             // +0x4
    short field_6;             // +0x6
    unsigned char colorKey;    // +0x8
    char unknown_9[7];         // +0x9
    unsigned char* plane0;     // +0x10
    unsigned char* plane1;     // +0x14
};

// FUNCTION: 0x4b90a0
void __stdcall FUN_004b90a0(Bitmap_004b90a0* src, Bitmap_004b90a0* dst,
                            int x, int y, int level)
{
    int xoff = dst->field_4 - src->field_4 + x;
    int yoff = dst->field_6 - src->field_6 + y;
    if (xoff < 0 || yoff < 0) {
        return;
    }
    unsigned char* sp0 = src->plane0;
    unsigned char* sp1 = src->plane1;
    for (int row = 0; row < src->height; row++, yoff++) {
        int stride = dst->width * yoff;
        unsigned char* dp0 = xoff + dst->plane0 + stride;
        unsigned char* dp1 = xoff + dst->plane1 + stride;
        int n = src->width;
        while (n--) {
            unsigned char c = *sp0;
            if (c != src->colorKey && *sp1 + level >= *dp1) {
                *dp0 = c;
                *dp1 = *sp1 + level;
            }
            dp0++;
            sp0++;
            dp1++;
            sp1++;
        }
    }
}
