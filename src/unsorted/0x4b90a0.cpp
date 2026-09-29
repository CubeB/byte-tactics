// Decompiled by deepseek-v4.1-flash, finished by Claude Sonnet 5.5, finished by space-bunny-free. Names are provisional.
// Partial, best 83.9% (re-checked by space-bunny-free, no better form found).
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
//
// Claude Sonnet 5.5 pass (#589): 82.8 to 83.9 percent by declaring the locals at
// function scope in the order `yoff, stride, xoff, dp0, dp1` (that is what the
// file now has). All 120 orders of {xoff, yoff, stride, dp0, dp1} were scored:
// 60 give 83.9, 60 give 82.8, and the 83.9 ones are exactly those where `stride`
// is declared before `xoff` (and yoff before dp0/dp1), so the first operand of
// the row-pointer sum moves. The remaining diff is then only (1) the plane load
// (`mov esi,[plane0]; add esi,xoff` here, `mov esi,ebx; ...; add esi,eax` in the
// original, i.e. the original copies xoff and adds the plane afterwards), (2) the
// `yoff` spill after the guards, and (3) the add destination of the threshold
// compare. Crossed with all 6 orders of the row-pointer sum and the 4 spellings
// of the compare (24 files): no change (the two `*dp1 <=` spellings give 82.8).
// N unused `extern int` declarations, N = 8 to 208 step 8: 83.9 at best, the
// rest much worse (22 to 68), so it is not the declaration-count state that
// helped 0x4b9360 in the same issue.
//
// space-bunny-free pass (#1117), 83.9% confirmed again, nothing better found.
// Independent check of the argument roles and slots, since a misread there
// would have made every row-pointer form look hopeless: with 2 dword locals and
// the 4 pushed registers the first argument is read at [esp+0x10] before the
// pushes and the second at [esp+0x14] after two pushes, both of which resolve
// to E0+8 and E0+4, so the bitmap at E0+4 is the parameter this file calls
// `src` (its color key, width and height drive the loops) and the one at E0+8
// is `dst` (its +4/+6 offsets, its width as the row stride). The file already
// had the roles the right way round. The three int parameters sit at E0+0xc,
// E0+0x10 and E0+0x14, and the first two of those slots are reused as the
// inner and outer loop counters, which is why the level is reloaded from
// [esp+0x2c] twice inside the inner loop. Nothing is read past the arguments,
// so there is no over-read bug here.
// New results this pass, all scored for free with check.py --sym:
//   * rows written as three statements each, `base + plane` then `+= stride`
//     for both, and the int sum cast to unsigned char* before adding stride:
//     all 83.9, the diff in the loop head is unchanged.
//   * the two row statements in the other order: 83.9, no change.
//   * dst->plane0 and dst->plane1 read into two locals first: 83.9.
//   * `stride = yoff * dst->width` instead of `dst->width * yoff`: 83.9.
//   * `int stride` declared in the loop body instead of at function scope:
//     82.8, so the function-scope declaration is worth 1.1 points.
//   * the row count as `while (1) { ...; row++; yoff++; if (row >= h) break; }`
//     per item 9 of the guide: 78.3, and `yoff++` as its own statement at the
//     end of the body: 77.4. The `for` form is right.
//   * the threshold compare hoisted into a local (`int t = *sp1 + level;`):
//     72.7. It has to stay inline in the condition.
//   * `stride` inlined into the first row only: 45.2, the imul moves out of
//     the loop head.
// The three remaining differences are each a single instruction group and none
// of them moves under any spelling tried, which supports the reading that the
// original was not built by this compiler build (or at least not by MSVC 5
// SP3 with this header state): the row pointers, the yoff spill and the add
// destination of the threshold compare are all decided by the same allocator.

// deepseek-v4.1-flash pass (#1281), 83.9% confirmed a fourth time. The whole
// 4-byte shortfall is the row-pointer block: the original makes xoff the add
// destination (`mov esi,ebx; mov eax,[edx+0x10]; add esi,eax; mov eax,ebx`)
// while this toolchain always canonicalises int+pointer to pointer-first
// (`mov esi,[edx+0x10]; add esi,ebx`). Tried this pass, all 83.9 and byte for
// byte the same: the plane fields as `int` with casts, `unsigned int` casts on
// both addends, separate int/unsigned temps assigned in one statement and
// materialised in the next (dp0/dp1 first, stride first, stride between),
// `(unsigned char*)xoff`, `+=` chains, a dst pointer alias, a local plane
// pointer pair, and recomputing the xoff expression in the row pointer. The
// yoff spill placement and the threshold add destination move with the same
// allocator decision. This is the compiler-state plateau the guide describes;
// it should resolve when the file is regrouped into its original translation
// unit.
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
    int yoff;
    int stride;
    int xoff;
    unsigned char* dp0;
    unsigned char* dp1;
    xoff = dst->field_4 - src->field_4 + x;
    yoff = dst->field_6 - src->field_6 + y;
    if (xoff < 0 || yoff < 0) {
        return;
    }
    unsigned char* sp0 = src->plane0;
    unsigned char* sp1 = src->plane1;
    for (int row = 0; row < src->height; row++, yoff++) {
        stride = dst->width * yoff;
        dp0 = xoff + dst->plane0 + stride;
        dp1 = xoff + dst->plane1 + stride;
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
