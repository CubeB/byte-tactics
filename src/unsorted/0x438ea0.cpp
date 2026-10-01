// Decompiled by space-bunny-free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.
// Space Bunny Free pass (issue 4404): still 54.0% and 500 bytes, but the one
// real bug in the previous best is fixed, and the register wall is now pinned
// down exactly (scratch under build/scratch/0x438ea0/, about 80 scratch
// variants scored, 4 scoring runs of this file; see the tools at the end of
// this header).
//
// FIXED HERE, invisible to the similarity score: the two fmul constants were
// emitted in the opposite order. check.py masks both operands as `<addr>`, so
// `radius * DAT_004fd2b0 * DAT_004fd2b8` scored exactly the same 54.0% as the
// right order, but the relocations pointed at the wrong data and check.py
// would report `DAT_004fd2b8 is 0x4fd2b0` the moment the bytes matched. MSVC 5
// evaluates that product's operands right to left; naming the first product
// (`double d = radius * DAT_004fd2b0; int n = (int)(d * DAT_004fd2b8);`)
// restores the original's `fmul 0x4fd2b0 ; fmul 0x4fd2b8` at the same 500 bytes.
// Do not write that expression inline again. (Verified in the /Fa listing of
// build/scratch/0x438ea0/{base,fm1,fm2,fm3}.asm.)
//
// THE WHOLE REMAINING DIFFERENCE IS ONE REGISTER SWAP, and it is the same in
// every variant tried. Callee-saved roles, ours vs original:
//     ours      ebx = pos, edi = rad then x2, esi = y2, ebp = angle (SPILLED)
//     original  ebx = rad then x2, ebp = angle, edi = pos, esi = y2
// ebp = angle and esi = y2 already agree; pos and rad trade ebx against edi.
// Ours then loses ebp, because with the angle spilled MSVC keeps a zero in it
// for the text block (`xor ebp, ebp` after the loop, `cmp eax, ebp` where the
// original has `test eax, eax`, and `cmp [lx], ebp` where the original loads
// lx into ecx and tests it), and the loop tail computes y1 in ebp where the
// original uses edx and keeps ebp for the angle. Everything else in the diff
// (the two `xor edi,edi / xor esi,esi`, the i = 0 store sinking past the
// guard's compare, the missing `mov [esp+0x48], edx` spill of sy into pos's
// dead argument slot, the p1/p2 store and load orders, and the missing
// `mov esi, [esp+0x4c] ; mov ebx, [esp+0x4c]` cold path) follows from that one
// choice; see items 1 to 3 below for the size of each.
//
// 1. The once-cold path (about 8 of the 157 diff lines). The original's is
//    `jl 0x43904d` -> `mov esi, [arg4] ; mov ebx, [arg4]` -> `jmp 0x439055`,
//    i.e. x2 and y2 are read out of the loop counter's dead argument slot,
//    which is only possible if they have no frame home at all and the i = 0
//    store lands BEFORE the guard's compare. Ours has neither: MSVC materialises
//    the two zeros up front and sinks i = 0 below the cmp.
//    `int x2; int y2;` (uninitialised) does emit the original's `mov reg, [slot]`
//    pair, but the shared home lands on index's slot (+0x58) not the counter's
//    (+0x4c), the block moves past the epilogue, and the whole callee-saved pool
//    rotates (zero to ebx, radius to ebp, angle spilled to arg5): 45.0%.
//    Declaring them uninitialised at FUNCTION scope (build/scratch/0x438ea0/x4.cpp)
//    compiles byte-identically to `int x2 = 0, y2 = 0`, which settles the point:
//    the two `xor`es are MSVC's zero rematerialisation for register-only
//    locals, not the initialiser, so no spelling of the initialiser removes them.
//
// 2. p1's store order is x, y, z in the original (`[+0x14]`, `[+0x18]`, `[+0x1c]`)
//    against x, z, y here, but its LOAD order is x, z, y-dword, y-short, which
//    the temporaries give. Reordering the stores to x, y, z (ps1.cpp) makes MSVC
//    hoist the y dword load with its store and the pos reads come out x, y, z:
//    51.6%. The two orders cannot be spelled independently here.
//
// 3. The tail spills sy into pos's argument slot (`mov [esp+0x48], edx` then
//    `mov eax, [esp+0x4c]`), because edx is needed for x1 while sy is still live.
//    Ours keeps sy in edx. This needs pos's argument slot to be free, which it
//    is only once the angle is out of memory (item 1 above is separate).
//
// DEAD LEVERS, RE-SWEPT THIS PASS, DO NOT SPEND RUNS ON THEM AGAIN:
//  * Compiler state. The N-unused-`extern int` calibration is flat to negative:
//    N = 0 and N = 700 give 54.0%, N = 400, 1000, 1400, 1800, 2400 and 3000 all
//    give 52.2%. Combined with the earlier 768-set headers.py sweep (also flat at
//    54.0%) this is a source-shape difference, not compiler state.
//  * Loop form. A plain `for (int i = 0; i <= n; i++)` is much worse (38.6%):
//    MSVC peels the first iteration, so `index *= 3` and the loop head end up
//    inside the body instead of before the `jmp`. The explicit `i = 0; if (i <= n)`
//    guard with a `do { } while (i <= n);` is right.
//  * The pos/rad tie. Tried and all 54.0%: `radius << 16` vs `radius * 0x10000`
//    vs `(int)(radius << 16)` vs a two-step `int r = radius; rad = r << 16;`
//    (identical output, so rad is not re-spelt); extra uses of rad through
//    `static inline` wrappers with a local copy of either argument (w1, w2); a
//    `Pos*` local copy of pos (pp); `unsigned` step and rad (u5); the coordinate
//    computation order in the tail (t1: MSVC schedules them itself, no effect);
//    step/rad declaration order (53.4%); p1/p2 at function scope (no change);
//    inline `RingPoint` helpers, both reading pos inside (48.7%) and taking the
//    values as arguments (48.7%); a wrapper returning the negation (54.0%).
//  * `int angle` inside the guard instead of at function scope: 45.2%, 498 bytes,
//    and the prologue rotates (radius to ebp, zero to ebx). At function scope the
//    prologue matches the original exactly, so keep it there.
//  * The two max() forms: `__max(v, f())` is right (`cmp v, f(); jle`), swapping
//    the operands costs 1.2 points and a ternary costs 1.2 (m1, m2, h4).
//
// What is already right: the 0x2c frame, every frame slot (ly +0x00, lx +0x04,
// rad +0x08, step +0x0c, n +0x10, p1 +0x14, p2 +0x20), the 16.16 Pos layout, the
// counter in arg4's dead slot, index *= 3 in arg7's slot, the x, z, y load order
// in both point copies, the __max double call for the terrain height, the guard,
// both draw calls, the argument list and every callee ret N.
//
// Scratch tooling left in build/scratch/0x438ea0/ (score many variants without
// spending check runs):
//   many.sh '<glob>'          one line per variant: status, size, diff lines
//   many.sh --side NAME       the original and ours aligned line by line
//   gen.sh <specfile>         generate variants by textual substitution
//   nsweep.sh 400 800 ...     the N-unused-declarations calibration
//   fa.sh NAME                compile one variant to a /Fa listing
#include <stdlib.h>

// A 16.16 world position: the frac/whole halves share one dword, so the code
// adds whole values through *(int*)&frac and reads the whole part back.
struct Pos_00438ea0 {
    unsigned short x_frac;               // +0x0
    short x;                             // +0x2
    unsigned short y_frac;               // +0x4
    short y;                             // +0x6
    unsigned short z_frac;               // +0x8
    short z;                             // +0xa
};

struct View_00438ea0 {
    char unknown_0[0x2c];
    int scroll_x;                        // +0x2c
    int scroll_y;                        // +0x30
};

extern double DAT_004fd2b0;              // 6.28318530717958
extern double DAT_004fd2b8;              // 0.125

int __cdecl FUN_004b70ef(int angle, int radius);
int __cdecl FUN_004b7123(int angle, int radius);
int __stdcall FUN_00485070(Pos_00438ea0* pos);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, int color);
void __stdcall FUN_004c14f0(void* surface, const char* text, int x, int y, int maxWidth);

// Draws a ring of n + 1 line segments around a 16.16 map position, where
// n = radius * pi / 4, and the label of the segment number index * 3 under it.
// FUNCTION: 0x438ea0
void __stdcall FUN_00438ea0(void* surface, View_00438ea0* view, Pos_00438ea0* pos, int radius,
                            int color, const char* text, int index)
{
    int angle = 0;
    if (radius) {
        int lx = 0;
        int ly = 0;
        int i;
        // The named product is what makes MSVC 5 multiply by the two constants
        // in the original's order; see the note at the top of the file.
        double d = radius * DAT_004fd2b0;
        int n = (int)(d * DAT_004fd2b8);
        i = 0;
        int x2 = 0;
        int y2 = 0;
        if (i <= n) {
            int step = 0x10000 / n;
            int rad = radius << 16;
            index *= 3;
            do {
                Pos_00438ea0 p1;
                int nx1 = -FUN_004b70ef(angle, rad);
                int nz1 = -FUN_004b7123(angle, rad);
                int vx1 = *(int*)&pos->x_frac;
                int vz1 = *(int*)&pos->z_frac;
                int vy1 = *(int*)&pos->y_frac;
                *(int*)&p1.x_frac = vx1 - nx1;
                *(int*)&p1.z_frac = vz1 - nz1;
                *(int*)&p1.y_frac = vy1;
                p1.y = __max(pos->y, FUN_00485070(&p1));
                angle += step;
                Pos_00438ea0 p2;
                int nx2 = -FUN_004b70ef(angle, rad);
                int nz2 = -FUN_004b7123(angle, rad);
                int vx2 = *(int*)&pos->x_frac;
                int vz2 = *(int*)&pos->z_frac;
                int vy2 = *(int*)&pos->y_frac;
                *(int*)&p2.y_frac = vy2;
                *(int*)&p2.z_frac = vz2 - nz2;
                *(int*)&p2.x_frac = vx2 - nx2;
                p2.y = __max(pos->y, FUN_00485070(&p2));
                int sx = view->scroll_x;
                int sy = view->scroll_y;
                int x1 = p1.x - sx + 0x80;
                int y1 = p1.z - (p1.y >> 1) - sy + 0x20;
                x2 = p2.x - sx + 0x80;
                y2 = p2.z - (p2.y >> 1) - sy + 0x20;
                FUN_004be950(surface, x1, y1, x2, y2, color);
                if (i == index) {
                    lx = x2;
                    ly = y2;
                }
                i++;
            } while (i <= n);
        }
        if (text) {
            if (lx == 0 && ly == 0) {
                lx = x2;
                ly = y2;
            }
            FUN_004c14f0(surface, text, lx, ly + 4, -1);
        }
    }
}
