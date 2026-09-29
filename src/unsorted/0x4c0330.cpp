// Decompiled by Sonnet 5.5, finished by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash. Names are provisional.
// Fills a convex polygon with one colour, into `surface` or into the locked
// screen when `surface` is null. It finds the top and bottom vertices and the
// horizontal extent, rejects the polygon when it lies wholly outside the
// surface's clip rect, then walks the left edges (backwards from the top
// vertex to the bottom one) and the right edges (forwards) into a table of
// per-scanline x positions with 16.16 steps, and finally fills each scanline
// span clipped to the rect. Returns 1 when something was drawn.
//
// NOT MATCHED (best): 80.6%, 944 of 932 bytes (2 real check.py runs this
// session). Frame matches exactly: 0x14060, twelve dword locals at
// [esp+0x10..0x3c], the 0x30-byte locked-screen struct at 0x40, the
// 0x800 x 0x28 span table at 0x70, args at 0x14064. Init sequence, clip
// rejects, clamps, both edge-walk bodies and the span fill all follow the
// original instruction for instruction apart from the three items below.
//
// WHAT MOVED THE NUMBER THIS SESSION (79.3 -> 80.6): the frame-slot
// permutation is NOT reachable by declaration order and NOT by variable
// names (I re-verified both: 8+ declaration permutations and three full
// renames all compile byte-identically). It IS reachable by the ORDER OF THE
// FOUR COMPARISONS IN THE BOUNDS LOOP. The original's emitted order is
// y<minY, y>maxY, x>maxX, x<minX, but writing the source as
// y>maxY, y<minY, x>maxX, x<minX puts minX at 0x10, locked at 0x14 and minY
// at 0x1c, all three correct, and is worth +1.3 net. The cost is that the
// emitted tests come out as jle/jge instead of the original's jge/jle in the
// min/max pair, so the win is not free. A full 24-permutation sweep of that
// loop is in build/scratch/0x4c0330/sweep24.py: y>maxY,y<minY,x>maxX,x<minX
// (80.6) beats y<minY,y>maxY,x>maxX,x<minX (79.3) and everything else is
// 57-72%. A 24-permutation sweep of the four clip-reject tests
// (build/scratch/0x4c0330/clip.py) found nothing better than the original
// order.
//
// STILL DIFFERS, three items, all believed to be ONE cause:
// (1) HOME-SLOT PERMUTATION. Original: minX=0x10, locked=0x14, maxY=0x18,
// minY=0x1c, j=0x20, maxIdx=0x24, minIdx=0x28. This build: minX=0x10,
// locked=0x14, j=0x18, minY=0x1c, maxY=0x20, minIdx=0x24, maxIdx=0x28.
// Note the pairs move together: the original has maxY BELOW minY and maxIdx
// BELOW minIdx, this build has both the other way round, and j sits where
// maxY should be. That is the same "reverse the two halves of a pair"
// pattern as the bounds-loop order above, which is the strongest hint that
// the remaining lever is the order of the min/max PAIRS somewhere in the
// walks, not the bounds loop.
// (2) FILL LOOP REGISTER ALLOCATION. The original keeps the scanline counter
// in ebp and re-reads everything else each iteration:
// `mov ebp,[minY] / mov eax,[maxY] / cmp ebp,eax / ... / mov al,[esp+0x14080]
// / imul edi,ebp / ... / mov eax,[maxY] / add esi,0x28 / inc ebp / cmp ebp,eax`.
// This build puts the COLOUR BYTE in ebp (`mov ebp,[color]` hoisted out of
// the loop, used as `mov eax,ebp / and eax,0xff`) and leaves the counter
// memory-resident in minY's reused slot, with maxY hoisted into ecx. So ebp
// is the ONE contested callee-saved register: esi holds s, ebx holds surface
// and edi holds the destination in both builds. Every spelling I tried of
// the fill loop gave MSVC the same allocation (see below).
// (3) THE TWO EXTRA STORES, 12 bytes, the entire size difference. Both walks
// store the WRAPPED index back to j's home on the wrap path
// (`mov [esp+0x20],edx` after the `lea edx,[ecx-1]` and after the `xor edx,edx`).
// The original stores only the unwrapped value and re-derives the wrap at the
// loop latch: `mov eax,[j] / test eax,eax / jge / mov eax,[n] / dec eax /
// cmp eax,maxIdx / jne`. So the original's source has a separate loop-carried
// variable holding the wrapped index and a named local holding the UNWRAPPED
// one, which is 13 dwords of locals rather than 12. Adding that extra local
// shrinks the frame to 0x1405c and moves every aggregate, so the two cannot
// be satisfied at once with the current 12-local layout; that is likely the
// same single root cause as (1) and (2).
//
// TRIED AND REJECTED THIS SESSION (all scored with the free --sym path):
// A separate wrapped-index local in the walks: 57.6% (it changes the frame to
// 0x1405c, span moves to 0x6c and screen to 0x3c). `j = (i+1) % n` for the
// forward wrap: 76.4% and it introduces an idiv, so its 80.2% in an earlier
// base was an alignment artefact, not a real shape. A ternary for the wrap
// index, `j -= n`, `if (!(j < n))`, `if (n <= j)`, `if (j > n - 1)`, and a
// redundant `else j = j;` self-correction: all 54-80%, none better than the
// plain `if`. `while (1) { ...; if (i == maxIdx) break; }` instead of
// `do {} while`: byte-identical to the do-while, so the latch shape is not
// the lever. For the fill loop: `int y` / `int row` in the for-init instead
// of the shared `i`, a hoisted `unsigned char c = color`, a hoisted
// `unsigned char* rowp`, `int pitch = surface->pitch * i`, swapping the two
// clamps, `if (s->right > s->left) memset(..., s->right - s->left)` with no
// `w` local, and a `char` or `int` colour parameter: ALL of them leave the
// allocation exactly as it was, none of them ever makes the counter reach ebp.
// `char` and `int` colour parameters are neutral at 80.6%, so the parameter
// type is not what decides between `mov al,[stack]` and `mov eax,ebp/and`.
// The obvious next lever, per the register-preference rule, is to add ONE
// more live value inside the fill loop so that whichever variable currently
// holds ebp is demoted to memory; nothing I wrote produced that extra live
// node. A `memset` with a hand-rolled dword loop was not tried.
//
// SECOND SESSION (deepseek-v4.1-flash, same model, 900s): no further gain,
// still 80.6. What was ruled out, so it is not repeated:
// - The four initialiser assignments do NOT move the slots: all 24 orders of
//   `minY/maxY/minX/maxX = +/-999999` compile to the same frame (checked with
//   build/scratch/0x4c0330/initsweep.py).
// - The N-declarations compiler-state test is unusable here: 4 or more unused
//   `extern int dummyN;` in front drop the score to 56.2 at every N up to 400
//   (build/scratch/0x4c0330/nsweep.py), so compiler state is not the lever.
// - With the current walk spelling, a fresh 24-way sweep of the bounds-loop
//   test order (psweep.py) still puts YMAX,YMIN,XMAX,XMIN at 80.6. The runner
//   up, XMAX,YMAX,YMIN,XMIN, gets maxY to the right home 0x18 but rotates
//   minY/j to 0x20/0x1c and falls to 71.5, so the home layout is coupled to
//   the emitted branch directions and cannot be bought separately.
// - Declaring the walk's `j` inside the loop body, or a top-tested `while`,
//   collapses the function to 45 to 51 percent, so `j` must stay at function
//   scope. `for (int y = ...)` in the fill and swapping the pitch multiply to
//   `i * surface->pitch` are neutral (80.6 and 80.2).
// The three remaining diffs (home permutation, fill counter vs colour in ebp,
// the two wrapped-j stores) still look like one allocator state. The only
// remaining untried lever I can see is forcing the seven scalar homes with a
// struct local laid out in the original order, which risks changing every
// register in the function.
#include <string.h>

struct Rect_004c0330 {
    int left;
    int top;
    int right;
    int bottom;
};

class Class_004c6ae0 {
public:
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];                 // 0x30 bytes, the lock descriptor

    Rect_004c0330* FUN_004c6ae0(Rect_004c0330* out);
};

struct Point_004c0330 {
    int x;
    int y;
};

struct Span_004c0330 {
    int left;                          // +0x0
    int right;                         // +0x4
    int unknown_8[8];
};

int __stdcall FUN_004c5e70(Class_004c6ae0* out);
int __stdcall FUN_004c5fa0(Class_004c6ae0* s);

// FUNCTION: 0x4c0330
int __stdcall FUN_004c0330(Class_004c6ae0* surface, Point_004c0330* points, int n, unsigned char color)
{
    Class_004c6ae0 screen;
    Rect_004c0330 clip;
    Span_004c0330 span[0x800];
    int locked;
    int minY;
    int maxY;
    int minX;
    int maxX;
    int minIdx;
    int maxIdx;
    int i;
    int j;
    int dy;

    if (surface == 0) {
        if (FUN_004c5e70(&screen) == 0)
            return 0;
        locked = 1;
        surface = &screen;
    } else {
        locked = 0;
    }

    minY = 999999;
    maxY = -999999;
    minX = 999999;
    maxX = -999999;
    for (i = 0; i < n; i++) {
        int y = points[i].y;
        if (y > maxY) {
            maxY = y;
            maxIdx = i;
        }
        if (y < minY) {
            minY = y;
            minIdx = i;
        }
        int x = points[i].x;
        if (x > maxX)
            maxX = x;
        if (x < minX)
            minX = x;
    }
    surface->FUN_004c6ae0(&clip);
    if (maxX < clip.left) {
        if (locked)
            FUN_004c5fa0(&screen);
        return 0;
    }
    if (minX > clip.right) {
        if (locked)
            FUN_004c5fa0(&screen);
        return 0;
    }
    if (maxY < clip.top) {
        if (locked)
            FUN_004c5fa0(&screen);
        return 0;
    }
    if (minY > clip.bottom) {
        if (locked)
            FUN_004c5fa0(&screen);
        return 0;
    }
    if (minY < clip.top)
        minY = clip.top;
    if (maxY > clip.bottom)
        maxY = clip.bottom;
    if (maxY == minY) {
        if (locked)
            FUN_004c5fa0(&screen);
        return 0;
    }

    Span_004c0330* sp = span;
    i = minIdx;
    do {
        j = i - 1;
        if (j < 0)
            j = n - 1;
        int y1 = points[j].y;
        int y0 = points[i].y;
        if (y1 > clip.top && y0 < y1) {
            int x0 = points[i].x;
            int x1 = points[j].x;
            dy = y1 - y0;
            int slope = ((x1 - x0) << 16) / dy;
            int fx = (x0 << 16) + 0xffff;
            if (y0 < clip.top) {
                fx += (clip.top - y0) * slope;
                y0 = clip.top;
            }
            if (y1 > clip.bottom)
                y1 = clip.bottom;
            for (int y = y0; y < y1; y++) {
                sp->left = fx >> 16;
                fx += slope;
                sp++;
            }
        }
        i = j;
    } while (i != maxIdx);

    sp = span;
    i = minIdx;
    do {
        j = i + 1;
        if (j >= n)
            j = 0;
        int y1 = points[j].y;
        int y0 = points[i].y;
        if (y1 > clip.top && y0 < y1) {
            int x0 = points[i].x;
            int x1 = points[j].x;
            dy = y1 - y0;
            int slope = ((x1 - x0) << 16) / dy;
            int fx = (x0 << 16) + 0xffff;
            if (y0 < clip.top) {
                fx += (clip.top - y0) * slope;
                y0 = clip.top;
            }
            if (y1 > clip.bottom)
                y1 = clip.bottom;
            for (int y = y0; y < y1; y++) {
                sp->right = fx >> 16;
                fx += slope;
                sp++;
            }
        }
        i = j;
    } while (i != maxIdx);

    Span_004c0330* s = span;
    for (i = minY; i < maxY; i++) {
        if (s->right > clip.right)
            s->right = clip.right;
        if (s->left < clip.left)
            s->left = clip.left;
        int w = s->right - s->left;
        if (w > 0)
            memset(surface->pixels + surface->pitch * i + s->left, color, w);
        s++;
    }
    if (locked)
        FUN_004c5fa0(&screen);
    return 1;
}
