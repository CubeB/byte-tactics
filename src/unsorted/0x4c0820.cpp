// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free. Names are provisional.
// DeepSeek V4.1 Flash session (45 min): still 99.5, 613 of 613 bytes, the same single hunk (walk1
// latch loads pts then i; original loads i then pts). Re-confirmed nothing in the header state:
// tools/headers.py --cpp (768 sets) is flat at 99.5. Swept and rejected this session, all 99.5 with
// the identical hunk unless noted: all 24 first-scan comparison orders (only the current ABCD
// scores 99.5), all 120 inner-loop statement orders (the current ABCDE is one of four that keep the
// bytes; the rest move the loop's adds, none flips the latch), top-tested while/for latches (66.7),
// goto-test latch, for(;;)/while(1) break latches, ~30 exotic wrap spellings (comma, inner while,
// switch, ternary, eq-zero, if-else, do-while(0), double if, parenthesised count-1, inline
// WrapDown helper, macros), long/short/unsigned/register types for count/color/i/iymin/x and for
// the body locals, pointer arithmetic and &spans[0] and cast spellings, decl-then-assign forms for
// every body local, guard spellings, initialization placement/order, struct definition order,
// dead copy-through-temp probes on pts/i/out/b, 7000 random 1-to-5 combinations of two 38-mutation
// sets, and prepending the matched sibling sources 0x4c0330/0x4c0c70/0x4c1000 above this function.
// A 613-byte reduced copy of the function (walk1 plus first scan only) reproduces the same swap, so
// the cause is inside walk1.
// NEW EVIDENCE (this session): the exe scan in build/scratch/0x4c0820/scan_latch.py shows the sibling
// 0x4c1000 has a walk1 with the SAME shape and instruction order (jne, then the two latch loads,
// then the wrap) but loads pts before i, while 0x4c1000's walk2 loads i before pts. So the order is
// not a walk1 source-shape lever at all: it is a whole-function allocation tie-break that flips
// between functions with identical walk shapes. The register assignments are the same in both
// (pts in ebx, index in edi/esi), so register order is not the discriminator either. Best next
// lead: a change far from walk1 that perturbs the global allocation order (the pcode/live-range
// creation order), or a compiler state change; source-shape edits inside walk1 have been exhausted.
// 30-min checkpoint (deepseek-v4.1-flash, this session): 99.5 percent, 613 of 613 bytes, ONE hunk
// left. THE LEVER FOUND THIS SESSION: the 4th parameter is `int color`, not `unsigned char`.
// Changing it fixes the final call register pick outright (the whole final scan then holds color
// in ebx and surf in ebp exactly as the original). Every earlier session had `unsigned char`
// there, which is why the ebx/ebp pick looked unreachable. The caller (0x458fa0) also declares
// the 4th argument `int param_4`. With that fix only the walk1 latch order remains: original
// loads i from [esp+0x24] then pts from [esp+0x1403c]; ours loads pts first then i.
// Tried this session, all 99.5 / same single hunk: header sweep (128 sets), N unused extern int
// (0..128), 300 declaration-order permutations, predec/minus-eq/plus-count/ternary/if-else/
// eq-minus-1/`i += count` latch spellings, for(;;)+break and while(1) latches, y1-before-y0,
// explicit block-local pa/q pointers, function-scope j, raw-j-plus-wrapped-k, sibling 0x4c0c70
// walk style (50 percent), dead-statement probes, callee prototype type changes, dummy
// struct/function/typedef/class prefixes (0..200 each), struct-as-class and long-field variants,
// walk2 head/inner-loop probes (the compiler normalises walk2's out/i order), first-loop shape
// probes, function-scope j, and function-scope i for both walks (99.5, same hunk). The two
// siblings 0x4c0330 and 0x4c0c70 (matched) both reload the points base BEFORE the index in their
// latch, so our compile follows the family norm and the original's order is the odd one.
// Best next lead: the order is the allocator's emission order inside one block, so look for a
// source or type change that makes the i load be emitted before the live-out pts reload, the way
// a source statement naturally precedes a live-out spill. The `int color` discovery suggests the
// allocator state responds to parameter/type changes rather than statement shapes.
//
// deepseek-v4.1-flash (#3807): 94.1 -> 98.0 percent, 613 of 613 bytes. The edge walks now hold the
// second point in a function-scope pointer b (b = &pts[j], every field read through b) while y0/y1
// became block-local to each walk. That pair is the lever the earlier sessions could not find: it
// lands y0 at 0x28 in walk1 and 0x24 in walk2 with b at 0x20 in both, exactly the original, and the
// frame stays 0x14024. The function-scope `int y0, y1;` was replaced by `Point_004c0820* b;`.
// Still different (both allocator picks): the walk1 tail loads pts from [esp+0x1403c] one instruction
// before reloading i from [esp+0x24] (original: i then pts), and the final call holds surf in ebx and
// color in ebp (original: color in ebx, surf in ebp, pushed in the same order either way).
// Re-tested here: reading y1 before y0 (both walks) is 60.7 percent / 609 bytes, a for-loop final
// scan is byte-identical at 98.0, and hoisting surf into a local fails to compile as edited.
// deepseek-v4.1-flash (#3940): local copies of the call arguments inside the if (c = color; sf = surf;)
// are dead-code eliminated and leave the diff byte-identical at 98.0 / 613, so the surf/color ebx/ebp
// pick is not reachable through the copy-to-locals trick. Swapping walk1's out/i statement order fixes
// the tail hunk but re-allocates the whole walk (86.2 percent / 614 bytes). Both reverted.
//
// PARTIAL 94.1%, 613 of 613 bytes. The two edge walks read pts[i]/pts[j] fields directly
// instead of caching &pts[i]/&pts[j] in pointer locals, share y0/y1/x/dx at function scope, and the
// first walk assigns out = spans before i = iymin (the second keeps the opposite order). Those three
// shape fixes took 60.7 to 78.3 to 89.2 to 94.1. Remaining differences, all allocator picks: the
// walk-local y0 and the internal &pts[j] spill swap slots 0x20/0x28 (0x20/0x24 in walk2), the walk1
// tail loads pts before reloading i, and the final call puts surf in ebx and color in ebp instead of
// color in ebx and surf in ebp. Ruled out in earlier sessions (scores): declaration-order sweeps of
// y0/y1/x/dx/rows/iymin/iymax (94.1 unchanged), block-local y0 (92.1), explicit b = &pts[j] (60.7),
// shared dy/rows/y/h (94.1 unchanged), separate scan index (43.1), scan index before the guard
// (42.9), extern dummy TU-state prefix at N=1/2/8 (60.7 in the 78.3 shape).
//
// Second pass (space-bunny-free), all free --sym scores, none beat 94.1:
// * Declaration order is NOT the lever here: splitting `int iymin, iymax;` into two statements,
//   merging `int y0,y1; int x,dx;` into one, and moving that pair after the initialised bounds
//   each leave the score and the diff set bit for bit at 94.1.
// * tools/headers.py, 128 sets: every set is 94.1%, so this is not header state.
// * Block-local `Point_004c0820* bj = &pts[j]` in each walk (94.1, identical diff: MSVC still
//   emits one CSE temp) and a shared function-scope named `bj` (90.1) do not move the walk slots.
// * Block-local y0 (92.1) and block-local y0+y1 (92.1) rotate the walk slots but ALSO swap the
//   iymin/iymax pair 0x30/0x2c, which is a net loss. Adding any one function-scope local does
//   the same flip, so the frame packing is a parity-like function of how many function-scope
//   locals there are: keep the count as it is.
// * Dropping the two extra braces around the walk blocks (so the walk index is the function-scope
//   i instead of a shadowing one) shrinks the frame by 4 bytes (609) and drops to 63.7%.
// * Naming the &pts[j] temp at its first use INSIDE `if (y0 < y1)` instead of letting MSVC CSE
//   it across the branch drops to 36.1% with a 604-byte frame: the original's lea and its spill
//   are both emitted BEFORE the `jge`, so that value is live into the branch. Keep the field
//   reads written as `b->x` / `b->z` / `b->y` and let MSVC CSE them.
//
// deepseek-v4.1-flash retry (#3217): re-confirmed 94.1 and the whole 92.1 block-local family. A
// function-scope named pointer for the second edge point (bp = &pts[j] used in both walks) drops to
// 60.7 (609 bytes) because it gives the pts base edi instead of ebx. Block-local y0 with the
// function-scope y0 removed, or with it kept dead, is 92.1 either way; removing the unused `int ay;`
// and swapping the iymin/iymax declaration order in that shape are inert (92.1), so the iymin/iymax
// flip is not a declaration or dead-local parity lever. `--i; i = count - 1;` for the walk1 tail is
// byte-identical to `i = i - 1; i = count; i--;` at 94.1.
// deepseek-v4.1-flash (#3585): re-ran the block-local y0/y1 family (both walks, walk1 only,
// walk2 only, walk2 declaring y0/y1 before j, plus one extra function-scope local as a
// parity probe): all land on 92.1 with the same 34-line diff. Every block-local y0 shape
// fixes walk1's slots (ptr 0x20, j 0x24, y0 0x28) but rotates walk2's and flips iymin/iymax
// to 0x2c/0x30, so one function-scope y0 (this shape) stays the best at 94.1.
// The walk diff is a pure two-way swap, not a rotation: with idx already right in both walks,
// original is (ptr 0x20, idx 0x24, y0 0x28) and (ptr 0x20, y0 0x24, idx 0x28), ours is
// (y0 0x20, idx 0x24, ptr 0x28) and (y0 0x20, ptr 0x24, idx 0x28). The original's &pts[j] keeps
// 0x20 in BOTH walks while y0 changes slot between them, so the next thing to try is a shape that
// makes the &pts[j] value a single variable spanning both walks while y0 is not: the opposite of
// the block-local y0 that was just ruled out. The final ebx/ebp pair (color, surf) is independent:
// it is the reverse of the push order, so it is the register preference for the two argument
// temporaries, and a local copy of each (94.1) does not change it.
// deepseek-v4.1-flash (#3889), still 98.0 percent, 613 of 613 bytes: splitting walk1's wrap into
// a raw `int j = i - 1;` plus a wrapped `int k = j;` used for b = &pts[k] and ending the walk with
// `i = j;` (the shape the original's 0x4c0949 reload of [esp+0x24] suggests) regresses to 96.6
// percent at the same 613 bytes, so the two remaining hunks stay as noted: the walk1 tail loads pts
// (ebx) one instruction before reloading the index (original: index then pts) and the final call
// holds surf in ebx / color in ebp (original: color in ebx / surf in ebp).
struct Point_004c0820 {
    int x;
    int y;
    int z;
};

struct Span_004c0a90 {
    int x1; // +0x0
    int x2; // +0x4
    char unknown_8[0x18 - 0x8];
    int z1; // +0x18 (16.16)
    int z2; // +0x1c (16.16)
    char unknown_20[0x28 - 0x20];
};

struct Surface_004c0a90 {
    unsigned short pitch; // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;  // +0x10
    unsigned char* depth; // +0x14
};

void __stdcall FUN_004c0a90(int row, Span_004c0a90* span, Surface_004c0a90* surf,
                            unsigned char color);

// FUNCTION: 0x4c0820
int __stdcall FUN_004c0820(Surface_004c0a90* surf, Point_004c0820* pts, int count,
                           int color) {
    Span_004c0a90 spans[2048];
    Span_004c0a90* out;
    int ay;
    Point_004c0820* b;
    int x, dx;
    int ymin = 999999;
    int xmax = -999999;
    int ymax = -999999;
    int xmin = 999999;
    int iymin, iymax;
    int y;
    Span_004c0a90* s;
    int i;
    Point_004c0820* p;
    i = 0;
    if (count > 0) {
        p = pts;
        do {
            if (p->y < ymin) {
                ymin = p->y;
                iymin = i;
            }
            if (p->y > ymax) {
                ymax = p->y;
                iymax = i;
            }
            if (p->x > xmax)
                xmax = p->x;
            if (p->x < xmin)
                xmin = p->x;
            i++;
            p++;
        } while (i < count);
    }
    if (ymax == ymin)
        return 0;
    {
        out = spans;
        int i = iymin;
        do {
            int j = i - 1;
            if (j < 0) {
                j = count;
                j--;
            }
            int y0 = pts[i].y;
            b = &pts[j];
            int y1 = b->y;
            if (y0 < y1) {
                int h = y1 - y0;
                x = pts[i].x;
                dx = ((b->x - x) << 16) / h;
                x = (x << 16) + 0xffff;
                int y = pts[i].z << 16;
                int dy = ((b->z << 16) - y) / h;
                int rows = b->y - y0;
                do {
                    out->x1 = x >> 16;
                    out->z1 = y;
                    x += dx;
                    y += dy;
                    out++;
                } while (--rows);
            }
            i = i - 1;
            if (i < 0) {
                i = count;
                i--;
            }
        } while (i != iymax);
    }
    {
        int i = iymin;
        out = spans;
        do {
            int j = i + 1;
            if (j >= count)
                j = 0;
            int y0 = pts[i].y;
            b = &pts[j];
            int y1 = b->y;
            if (y0 < y1) {
                int h = y1 - y0;
                x = pts[i].x;
                dx = ((b->x - x) << 16) / h;
                x = (x << 16) + 0xffff;
                int y = pts[i].z << 16;
                int dy = ((b->z << 16) - y) / h;
                int rows = b->y - y0;
                do {
                    out->x2 = x >> 16;
                    out->z2 = y;
                    x += dx;
                    y += dy;
                    out++;
                } while (--rows);
            }
            i = i + 1;
            if (i >= count)
                i = 0;
        } while (i != iymax);
    }
    {
        s = spans;
        y = ymin;
        while (y < ymax) {
            if (s->x2 - s->x1 > 0)
                FUN_004c0a90(y, s, surf, color);
            s++;
            y++;
        }
    }
    return 1;
}
