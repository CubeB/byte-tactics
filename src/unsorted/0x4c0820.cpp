// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free. Names are provisional.
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
//   reads written as `pts[j].x` / `pts[j].z` / `pts[j].y` and let MSVC CSE them.
//
// deepseek-v4.1-flash retry (#3217): re-confirmed 94.1 and the whole 92.1 block-local family. A
// function-scope named pointer for the second edge point (bp = &pts[j] used in both walks) drops to
// 60.7 (609 bytes) because it gives the pts base edi instead of ebx. Block-local y0 with the
// function-scope y0 removed, or with it kept dead, is 92.1 either way; removing the unused `int ay;`
// and swapping the iymin/iymax declaration order in that shape are inert (92.1), so the iymin/iymax
// flip is not a declaration or dead-local parity lever. `--i; i = count - 1;` for the walk1 tail is
// byte-identical to `i = i - 1; i = count; i--;` at 94.1.
// The walk diff is a pure two-way swap, not a rotation: with idx already right in both walks,
// original is (ptr 0x20, idx 0x24, y0 0x28) and (ptr 0x20, y0 0x24, idx 0x28), ours is
// (y0 0x20, idx 0x24, ptr 0x28) and (y0 0x20, ptr 0x24, idx 0x28). The original's &pts[j] keeps
// 0x20 in BOTH walks while y0 changes slot between them, so the next thing to try is a shape that
// makes the &pts[j] value a single variable spanning both walks while y0 is not: the opposite of
// the block-local y0 that was just ruled out. The final ebx/ebp pair (color, surf) is independent:
// it is the reverse of the push order, so it is the register preference for the two argument
// temporaries, and a local copy of each (94.1) does not change it.
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
                           unsigned char color) {
    Span_004c0a90 spans[2048];
    Span_004c0a90* out;
    int ay;
    int y0, y1;
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
            y0 = pts[i].y;
            y1 = pts[j].y;
            if (y0 < y1) {
                int h = y1 - y0;
                x = pts[i].x;
                dx = ((pts[j].x - x) << 16) / h;
                x = (x << 16) + 0xffff;
                int y = pts[i].z << 16;
                int dy = ((pts[j].z << 16) - y) / h;
                int rows = pts[j].y - y0;
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
            y0 = pts[i].y;
            y1 = pts[j].y;
            if (y0 < y1) {
                int h = y1 - y0;
                x = pts[i].x;
                dx = ((pts[j].x - x) << 16) / h;
                x = (x << 16) + 0xffff;
                int y = pts[i].z << 16;
                int dy = ((pts[j].z << 16) - y) / h;
                int rows = pts[j].y - y0;
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
