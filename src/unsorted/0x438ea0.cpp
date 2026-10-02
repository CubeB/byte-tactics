// Decompiled by space-bunny-free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, reworked by space-bunny-free. Names are provisional.
// issue 4503 pass: 68.25%, ours 501 bytes against the original's 505. Up from
// 54.0%: two source-level facts fell out of re-reading the exe's tail against
// the disassembly instead of against our own listing, and the whole lower half
// of the function moved at once. The old header's claim that the whole residual
// was one register swap was right about the register and wrong about the rest.
//
// WHAT CHANGED, 54.0% -> 68.25%:
//
// 1. y1 subtracts the loop counter, not view->scroll_y. The original's y1 is
//    `movsx edx, p2.x ; sar eax, 1 ; sub edx, eax ; mov eax, [esp+0x4c] ;
//    sub edx, eax ; add edx, 0x20`, and [esp+0x4c] is the dead argument slot
//    the counter `i` lives in (`mov [esp+0x4c], ebp` before the guard's test,
//    `mov [esp+0x4c], eax` in the loop tail). So the source is
//    `int y1 = p1.z - (p1.y >> 1) - i + 0x20;`, not `- sy`. 54.0 -> 64.3.
//    This also stops MSVC needing a register for the angle: ebp now holds the
//    angle across the whole loop, and the text block gets the original's
//    `test eax, eax` / `test ecx, ecx` instead of `cmp reg, ebp` plus the
//    `xor ebp, ebp` that went with it.
//
// 2. `x2` and `y2` are UNINITIALISED (`int x2; int y2;` where the function
//    starts), which is what produces the original's cold path: the `jl` after
//    the counter's compare jumps to `mov esi, [slot] ; mov ebx, [slot]` and
//    falls into the text block. The old header ruled this out on the strength
//    of the two `xor edi,edi` / `xor esi,esi` that `int x2 = 0, y2 = 0` emits;
//    those are MSVC's zero rematerialisation for the two register-only
//    variables, not the initialiser, so dropping `= 0` removes them and the
//    reload pair as well. 64.3 -> 68.25, and the angle stops being spilled.
//
// STILL DIFFERS (501 bytes against 505, so four bytes of code are missing):
//
// A. The prologue, which is the ebx/edi choice below showing up early. The
//    original tests radius in ebx, loaded right after `push ebx`, so the guard
//    is `cmp ebx, ebp` between `push esi` and `push edi`; ours loads it into
//    edi after all four pushes. Nothing tried moves it (see the dead list).
//
// B. One register swap, everything else in the loop follows from it. Ours:
//    ebx = pos, edi = rad then x2, esi = y, ebp = angle. The original: edi =
//    pos, ebx = rad then x2, esi = y, ebp = angle. That is why ours emits the
//    three-instruction shuffle `mov edi, ebx ; mov ebx, [pos] ;
//    mov [rad], edi` where the original has only `shl ebx, 0x10`. Apply the
//    swap to our listing and 140 of the original's 169 instructions match in
//    shape (`python build/scratch/438ea0/shape2.py <file> swap`), so this one
//    choice is worth about ten instruction lines and nothing else.
//
// C. p1's and p2's loads. The original reads `pos->y_frac` (dword, into esi)
//    immediately after `sub ecx, esi` and stores it at +0x28 before the
//    argument push; ours reads `pos->y` (the short) there and the dword after
//    the push. p2's block is the mirror image. The store order x, y, z instead
//    of x, z, y is worth 61.9%, not more; declaring the y dword load first is
//    exactly flat; a named `pos->y` temporary is exactly flat; both points at
//    once is 62.0%.
//
// D. The loop tail's sy. The original loads scroll_y into edx and spills it to
//    [esp+0x48], pos's dead argument slot, before pushing the colour; ours
//    keeps it in eax and has no spill. The spill is four bytes and looks like
//    the whole of the 501 against 505. Note that the spill's value is never
//    read back (the text call reloads [esp+0x48] with esp eight lower, which is
//    the first argument's slot, not this one), so it is dead in the original.
//
// E. `step` is loaded into ecx in the original and edx here
//    (`mov ecx, [esp+0x1c] ; add ebp, ecx`), and p2's second
//    `FUN_00485070` argument is `lea ecx, [esp+0x30]` there and
//    `lea edx, [esp+0x30]` here. Both follow from the same register choices.
//
// F. The cold path reloads x2 and y2 out of +0x58 (index's dead slot) here
//    and out of +0x4c (the counter's slot) in the original, so only one of the
//    two loads lines up. Moving `x2`/`y2` past `i` in the declaration order
//    does not move the slot (Z1, Z2: both 68.25%, 501 bytes).
//
// A LEAD FOR THE NEXT ATTEMPT, from the field offsets MSVC really emits. For
// p1 (based at +0x24) MSVC 5 puts our `p1.x` at +0x2a, our `p1.y` at +0x2e and
// our `p1.z` at +0x2e, while for p2 (at +0x30) `p2.x` is at +0x32 and `p2.z`
// at +0x3a as the struct says. The original's x1 reads +0x2a, its y1 shift
// term reads +0x2a and its y1 base reads +0x32. So x1 already lines up, and
// y1 wants a base that MSVC emits at p2+2 and a shift term at p1+6, i.e.
// `y1 = p2.x - (p1.x >> 1) - i + 0x20`. That spelling drops to 35.0% and 497
// bytes: p1 and p2 swap their frame slots (p1 goes to +0x30 and p2 to +0x24)
// and the tail falls apart with them. Declaring p2 before p1 to force the
// other order back does not help either, same 35.0%. Somebody should find a
// shape that gets those two field reads without moving the two structs.
//
// DEAD LEVERS, RE-SWEPT THIS PASS. All of these score exactly 68.25% and 501
// bytes, i.e. identical output, so do not spend runs on them: `rad`/`step` at
// function scope; `step`/`rad` swapped in the declaration; a dummy
// uninitialised local inside the guard to shift the callee-saved allocation; a
// two-step `int r = radius; rad = r << 16;`; `(unsigned)radius << 16`; a local
// `Pos*` copy of pos; `sx`/`sy` at function scope; `sx`/`sy` declared in the
// other order; `if (radius != 0)`; `if (radius > 0)`; a local copy of radius
// for both the guard and the shift; `x2`/`y2` at function scope; a `(void)i;`.
// Also still dead from the previous pass: the N-unused-`extern int` compiler
// state calibration (flat to negative), a plain `for (int i = 0; i <= n; i++)`
// loop (38.6%, MSVC peels the first iteration), `angle` inside the guard
// (45.2%, and the prologue rotates), and the `__max(v, f())` spelling
// (swapping the operands or using a ternary each costs 1.2 points).
//
// STILL RIGHT: the 0x2c frame, every frame slot (ly +0x10, lx +0x14,
// rad +0x18, step +0x1c, n +0x20, p1 +0x24, p2 +0x30), the 16.16 Pos layout,
// the counter in arg4's dead slot, `index *= 3` in arg7's slot, the counter in
// arg4's slot with the `i = 0` store before the guard's compare, the
// `__max(double)` call for the terrain height, both draw calls and the
// argument list, every callee ret N, and the argument-slot map. That last one
// is worth writing down because it is easy to get wrong: the prologue's
// `mov ebx, [esp+0x40]` is `radius` (only `push ebx` has happened, so the
// displacement is four larger than it looks), and after all four pushes
// [esp+0x4c] is the same argument while [esp+0x40] is the first one.
//
// The fmul order from the previous pass still stands: `d = radius * DAT_004fd2b0`
// then `n = (int)(d * DAT_004fd2b8)`. check.py masks both operands as `<addr>`,
// so the product's operands can be in the wrong order and still score, but MSVC
// 5 evaluates right to left and the relocations then point at the wrong data.
//
// Scratch tooling in build/scratch/438ea0/ (scores many variants in about a
// third of a second each, which does not count as a check.py run):
//   score.py <name=file> ...   one line per variant: percent and size
//   shape2.py <file> [swap]    shape diff, optionally renaming ebx<->edi in ours
//   align.py <file>            byte-accurate side-by-side with a drift column
//   dump.py <addr> <file>      both listings with bytes
//   grep.py                    filtered listing; V=<file> PAT=<substr> [ALL=1]
//   v/                         every variant tried this pass and the last
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
        int x2;
        int y2;
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
                int y1 = p1.z - (p1.y >> 1) - i + 0x20;
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
