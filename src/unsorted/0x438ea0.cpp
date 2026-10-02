// Decompiled by space-bunny-free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, reworked by space-bunny-free. Names are provisional.
// issue 4746 pass: 82.84%, ours 505 bytes against the original's 505 (was 68.25%
// and 501). Two new levers, one correction of an earlier finding, and the
// negative-result list that closes most of what is left.
//
// WHAT CHANGED, 68.25% -> 82.84%:
//
// 1. AN int-RETURNING PREDICATE is what fixed the register swap, and it is
//    worth 10 points on its own (72.78 -> 82.84). The `__max(pos->y,
//    FUN_00485070(&p))` in the original expands to a branch:
//
//      0x438f47  call FUN_00485070
//      0x438f4c  cmp esi, eax
//      0x438f4e  jle 0x438f57
//      0x438f50  mov word [esp+0x2a], si
//      0x438f55  jmp 0x438f66
//      0x438f57  lea edx, [esp+0x24]
//      0x438f5b  push edx
//      0x438f5c  call FUN_00485070
//      0x438f61  mov word [esp+0x2a], ax
//
//    and the branch is what puts `pos` in edi and `rad` in ebx. Spelling the
//    comparison as `Higher_00438ea0(pos->y, FUN_00485070(&p)) ? ... : ...`
//    where the helper RETURNS INT reproduces it exactly:
//
//      static inline int Higher_00438ea0(int a, int b) { return a > b; }
//
//    The return type is the whole lever, and it is worth writing down:
//
//      static inline int  Higher(a, b) { return a > b; }   82.84%, 505 bytes
//      static inline bool Higher(a, b) { return a > b; }   75.00%, 519 bytes
//      static inline char Higher(a, b) { return a > b; }   75.00%, 519 bytes
//      static inline unsigned char Higher(a, b) { return a>b;} 75.00%, 519 B
//
//    A bool (or char) result becomes a materialised flag, and MSVC 5 emits
//    `cmp ecx, eax / setg al / test al, al / je` where the original has
//    `cmp esi, eax / jle`: five instructions against two, 14 bytes across the
//    two sites, and the extra register pressure moves `rad` off ebx again. An
//    int result stays a comparison and the branch survives. Bodies that fold
//    to the same thing are all equal at 82.84%: `a > b`, `!(a <= b)`, an
//    `if (a > b) return 1; return 0;`, and the argument order swapped. Only
//    `a - b > 0` is different (75.44%, 513 bytes), and `a >= b` is 82.25%.
//
// 2. A GETTER for the 16.16 y read, worth 68.25 -> 72.19 and the last four
//    bytes:
//
//      static inline int YFrac_00438ea0(Pos_00438ea0* p) { return *(int*)&p->y_frac; }
//
//    is the only one of the four field reads that works. On x_frac it is 64.09,
//    on z_frac 62.31, on all three fields 58.82, and dropping it from this
//    version costs 17.7 points (82.84 -> 65.28). It works by giving that one
//    load a boundary so MSVC schedules it where the original has it. It does
//    have to be a getter on the LOAD: a setter-shaped `SetYFrac(&p1, vy1)` is
//    68.25, an `int&` returning getter is 68.25, and a by-value `Pos` one is
//    55.91.
//
// 3. CORRECTION: y1 SUBTRACTS scroll_y, NOT the loop counter. The previous
//    header counted a `push` wrong. Walk the tail with the push depth tracked
//    (B = esp after the four callee-saved pushes, so arg1 at B+0x40, view
//    B+0x44, pos B+0x48, radius B+0x4c, color B+0x50, text B+0x54, index
//    B+0x58):
//      0x438fd0  mov edx, [edx+0x30]         ; scroll_y, at depth B
//      0x438fda  mov [esp+0x48], edx         ; B+0x48 = pos's DEAD argument slot
//      0x438fde  movsx eax, word [esp+0x2a]  ; B+0x2a = p1.y
//      0x438fe3  sub esi, edx                ; y2 -= scroll_y, register copy
//      0x438fe5  mov edx, [esp+0x50]         ; B+0x50 = color; edx reused
//      0x438fee  push edx                    ; esp = B-4 from here to the call
//      0x438ff1  movsx edx, word [esp+0x32]  ; B+0x2e = p1.z
//      0x438ffa  mov eax, [esp+0x4c]         ; B+0x48: the scroll_y just stored
//      0x438ffe  sub edx, eax                ; y1 -= scroll_y
//    The old note read `mov eax, [esp + 0x4c]` at depth B and concluded it was
//    the counter `i` (which does live in the radius slot B+0x4c, stored there
//    at 0x438ed4 and 0x439041). It is at depth B-4, so it reads B+0x48. The
//    source is `int y1 = p1.z - (p1.y >> 1) - sy + 0x20;` and the four bytes
//    that were missing were the spill of scroll_y into pos's dead argument slot
//    plus its reload. The same slip is why the old header called the reload
//    pair dead. Note the two displacements differ (0x4c and 0x2a) but are
//    read at different depths, so they are B+0x48 and B+0x2e.
//
//    WHY `- sy` IS NOT IN THE FILE: it is the right source and it costs 37
//    points (82.84 -> 46.02). About 200 spellings of the `- sy` form were
//    measured, at the 68.25% baseline and again at this one, in every shape:
//    a second `sy`, two inline `view->scroll_y` reads, one named and one
//    inline, getters on scroll_x and scroll_y, sx and sy declared in either
//    order, and sx/sy moved to the guard block or before the loop. They land
//    at 34 to 46 percent, and all of them show the same allocator collapse:
//    `rad` drops from ebx to ebp, `angle` loses ebp and is spilled to the
//    radius slot on every iteration, the x and y accumulators take edx and edi,
//    and p1.z is read twice. The original has rad=ebx, pos=edi, angle=ebp. So
//    the file keeps `- i`, which is byte-closer, and this note is the evidence
//    that it is semantically wrong.
//
// STILL DIFFERS (505 bytes against 505, so the sizes agree and zero of the
// remaining diff lines are only moved jump targets; everything below is real
// codegen):
//
// A. p1's block, five instructions. The original:
//      mov ecx,[edi] / mov edx,[edi+8] / sub ecx,esi / mov esi,[edi+4] /
//      add esp,8 / mov [esp+0x24],ecx / neg eax / lea ecx,[esp+0x24] /
//      sub edx,eax / mov [esp+0x28],esi / push ecx /
//      movsx esi,word [edi+6] / mov [esp+0x2c],edx / call
//    Ours reads y_frac into ebx and pos->y into ecx, both above `add esp, 8`,
//    and stores y_frac after z_frac. The original reuses esi for both, because
//    esi's previous value (-nx1) is consumed by `sub ecx, esi` just before.
//
// B. p2's block: ours stores x_frac before `add esp, 8` and y_frac last,
//    the original stores y_frac first, and `lea eax,[esp+0x30]` against
//    `lea ecx,[esp+0x30]` for the else arm's argument. Four instructions.
//
// C. `mov ebx, [esp+0x18]` (the rad reload before the second trig pair) and
//    `add ebp, ecx` are in the other order here. Two instructions. This one is
//    downstream of A: the getter's y_frac load takes ebx, which is `rad`'s
//    register, so `rad` has to be reloaded.
//
// D. The scroll_y spill and reload (finding 3) are missing, four bytes. This is
//    the whole semantic residual.
//
// E. The cold path reloads x2 and y2 out of +0x58 (index's dead slot) here and
//    out of +0x4c (the counter's slot) in the original. Two instructions.
//    Moving x2/y2 past i in the declaration order does not move the slot.
//
// MEASURED AND DEAD. Every one of these is exactly 82.84% and 505 bytes, i.e.
// identical output, or worse. This is the closure list for the next pass.
//
//  * The y_frac getter's position: on p1 only 82.84, on p2 only 65.28, on both
//    82.84; called straight in the store expression (`*(int*)&p1.y_frac =
//    YFrac(pos);`) 82.84 on either block and on both.
//  * Read and store order in p1 and p2: all 36 read/store order pairs per
//    block are 82.84 except the store orders other than x, z, y, and all 180
//    interleavings of the three reads with the three stores (data-dependence
//    respecting) top out at 82.84. Store order x, y, z is the only one that
//    loses (65.88 at this baseline).
//  * A getter on `pos->y` inside Higher: 62.94 with 511 bytes, whether it also
//    replaces the second operand (79.06 / 509, the best "shape" figure measured
//    at 83.2 percent but 8 bytes long).
//  * Getters on x_frac 64.09, on z_frac 62.31, on both 64.09; a setter for
//    y_frac 68.25; an `int&` getter 68.25; a by-value `Pos` getter 55.91.
//  * Getters on view->scroll_x and scroll_y: 82.84, flat.
//  * Getters on p1.x/p1.z and p2.x/p2.z: 82.84, flat. On p1.y and p2.y the
//    getter has to be inside Higher, which is item A's problem.
//  * Folding the two trig results into the struct stores: 43.79 (both folded
//    into p1's stores), 39.88 (x only), 65.28 (z only), 65.28 for the p1 or
//    p2 stores inlined wholesale. Every one loses the getter's schedule.
//  * Parameter bindings: `const Pos*`, `Pos* const`, `View* const` all flat;
//    `unsigned radius` 55.29; `unsigned step`, `unsigned angle`, `rad =
//    radius * 0x10000`, step/rad swapped (82.25) all flat or one notch down.
//  * `if (n >= 0)` for the guard, flat; `if (i < n + 1)` 59.00;
//    `if (i + 1 <= n + 1)` 59.00; `while (i <= n)` 44.71; a plain `for` 38.6.
//  * A by-value inline helper `Offset_00438ea0(pos, angle, rad)` returning the
//    Pos, with and without the max inside it: 55.52, 505 bytes.
//  * A `static inline Ground_00438ea0(p, floor)` helper for the max alone:
//    63.64 with 482 bytes; the argument order swapped 72.78.
//  * Headers at 82.84%: `<windows.h>`, `<windows.h>+<memory.h>`,
//    `<stdlib.h>+<memory.h>`, `<stdlib.h>+<math.h>`. The N-unused-`extern int`
//    compiler-state calibration is flat from N = 0 to 300 at the 68.25%
//    baseline, every one 44.97% with the `- sy` body, so there is no
//    declaration-count window here.
//  * The other comparisons, behind a predicate of their own, to move the
//    allocator without touching the max: `i == index`, `lx == 0 && ly == 0`,
//    `text`, `radius`, the loop latch's `i <= n` and the guard's, each on its
//    own and in pairs. 59 to 67 percent, all worse. Only the max's own
//    comparison moves the allocator, and only with an int result.
//  * Sharing the pos reads between the two points, to free esi the way item A
//    needs: `int vx2 = vx1; int vz2 = vz1;` is 33.92 with 522 bytes, `vx2 =
//    vx1` alone 57.23 with 509, `vy2 = vy1` alone 62.13 with 506, both 57.48.
//    Hoisting all three above both blocks does not compile into anything
//    useful. A single `short py = pos->y` used by both maxima is 82.84 (flat),
//    used by only the second 82.84 (flat), and by only the first 50.59.
//  * Moving p1's x store ahead of the pos reads, so the `sub ecx, esi` that
//    frees esi happens first: 62.91 with 501 bytes. Storing y_frac between the
//    x and z stores instead of after both: 82.84, flat.
//  * Consuming -nx1 inside the read that uses it (`int vx1 = *(int*)&pos->x_frac
//    - nx1;` then `p1.x_frac = vx1`), which is the shortest life for esi's old
//    value: 80.12 with 501 bytes, and the same for both points. Consuming -nz1
//    the same way instead: 78.34. Both at once: 78.34. Each loses four bytes
//    that the original has, so none of them is a step forward.
//
// A PROBE FOR THE REGISTER RULE. build/scratch/438ea0/probe1.cpp compiles six
// minimal stdcall functions with /Fa. With two register-hungry candidates, a
// value used 4 times and a pointer used 8 times inside a loop, MSVC gives the
// VALUE ebx and the POINTER edi (p3), which is the original's choice; with the
// counts swapped (p4) the value takes esi and the pointer goes to memory; add
// a third loop-carried variable (p5) and the pointer wins edi with the value
// spilled. So the allocator is not a fixed preference order. On this function
// the deciding input turned out to be how the terrain-height comparison is
// materialised, which is finding 1; removing the text block and the lx/ly pair
// also moves `rad` into ebx (v/K_0.cpp), but only by deleting the code.
//
// STILL RIGHT: the 0x2c frame, every frame slot (ly +0x10, lx +0x14,
// rad +0x18, step +0x1c, n +0x20, p1 +0x24, p2 +0x30), the 16.16 Pos layout,
// the prologue's `mov ebx, [esp + 0x40]` for radius and the guard's `cmp ebx,
// ebp` between `push esi` and `push edi`, edi = pos and ebx = rad through the
// whole loop, `index *= 3` in arg7's slot, the counter in arg4's dead slot
// with the `i = 0` store before the guard's compare, the `__max(double)` call
// for the terrain height, all four call sites of the two draw and two label
// calls, every callee ret N, and the argument-slot map.
//
// The fmul order from an earlier pass still stands: `d = radius *
// DAT_004fd2b0` then `n = (int)(d * DAT_004fd2b8)`. check.py masks both
// operands as `<addr>`, so the product's operands can be in the wrong order and
// still score, but MSVC 5 evaluates right to left and the relocations would
// then point at the wrong data.
//
// Scratch tooling in build/scratch/438ea0/ (each variant scores in about three
// seconds through score.py, which is not a check.py run):
//     score.py <name=file> ...   percent and size for many variants
//     side.py  <file> [lo] [hi] [o|t]  side-by-side or single listing, with the
//                                   push depth tracked, so a displacement is
//                                   never read at the wrong depth again
//     regs.py  <name=file> ...   compile-only: which register holds pos, rad
//                                   and the cold-path pair, a fast filter
//     top.py   <list> [n]        score a list file, best first
//     raw.py   <addr> ...        the original's bytes with encodings
//     promote.py <file>          copy the best variant into src/unsorted/
//     best.py                     rebuild the best variant from scratch
//     sweep1..sweep43.py         every sweep above, in order
//     v/                         every variant measured, one file each
// (Earlier passes, kept because two of their claims had to be corrected below.)
// issue 4503 pass: 68.25%, ours 501 bytes against the original's 505. Up from
// 54.0%: two source-level facts fell out of re-reading the exe's tail against
// the disassembly instead of against our own listing.
//
// 1. It concluded "y1 subtracts the loop counter, not view->scroll_y". WRONG,
//    see finding 3 of this pass: the original subtracts scroll_y. The whole
//    argument rested on reading `mov eax, [esp + 0x4c]` at push depth B when
//    that instruction is at depth B-4.
//
// 2. `x2` and `y2` are UNINITIALISED (`int x2; int y2;`), which is what
//    produces the original's cold path: the `jl` after the counter's compare
//    jumps to `mov esi, [slot] ; mov ebx, [slot]` and falls into the text
//    block. An earlier header ruled this out on the strength of the two
//    `xor edi,edi` / `xor esi,esi` that `int x2 = 0, y2 = 0` emits; those are
//    MSVC's zero rematerialisation for the two register-only variables, not the
//    initialiser, so dropping `= 0` removes them and the reload pair as well.
//    This one is right and it is still in the file.
//
// B. FIXED in this pass by finding 1, the int-returning predicate. It was the
//    whole of the residual: ours had ebx = pos and edi = rad, the original edi =
//    pos and ebx = rad, and ours emitted a three-instruction shuffle
//    `mov edi, ebx ; mov ebx, [pos] ; mov [rad], edi` where the original has
//    only `shl ebx, 0x10`. The prologue matches now too, so the guard is
//    `cmp ebx, ebp` between `push esi` and `push edi` as the original has it.
//
// C. p1's and p2's loads. The original reads `pos->y_frac` (dword, into esi)
//    immediately after `sub ecx, esi` and stores it at +0x28 before the
//    argument push; ours reads `pos->y` (the short) into ecx after the push and
//    the dword into ebx before `add esp, 8`. p2's block is the mirror image.
//    The getter in finding 2 moved this load without fixing its position: all
//    36 read/store order pairs per block and all 180 interleavings of the three
//    reads with the three stores are measured in the dead list below.
//
// D. The loop tail's sy, which is the `- sy` of finding 3 and the four missing
//    bytes. The previous header's version of this item was wrong in two ways,
//    both from counting a `push` wrong: it read the reload as dead, and it read
//    `mov eax, [esp + 0x4c]` at depth B when it is at B-4.
//
// F. The cold path reloads x2 and y2 out of +0x58 (index's dead slot) here
//    and out of +0x4c (the counter's slot) in the original, so only one of the
//    two loads lines up. Moving `x2`/`y2` past `i` in the declaration order
//    does not move the slot (Z1, Z2: both 68.25%, 501 bytes).
//
// E. `lea eax,[esp+0x30]` against `lea ecx,[esp+0x30]` for p2's else-arm
//    argument, and `mov ebx, [esp+0x18]` (the rad reload before the second trig
//    pair) against `add ebp, ecx` in the other order. Both downstream of A.
//
// F. The cold path reloads x2 and y2 out of +0x58 (index's dead slot) here
//    and out of +0x4c (the counter's slot) in the original, so only one of the
//    two loads lines up. Moving `x2`/`y2` past `i` in the declaration order
//    does not move the slot (Z1, Z2: both 68.25%, 501 bytes).
//
// A CORRECTION TO THE OLD LEAD, so nobody repeats it. It read the original's
// tail field reads as "x1 at +0x2a, y1's shift term at +0x2a and y1's base at
// +0x32" and concluded `y1 = p2.x - (p1.x >> 1) - i + 0x20`. Those three reads
// are at two different push depths: `movsx esi, word [esp+0x3a]` is at depth B
// and is p2.z, `movsx eax, word [esp+0x2a]` at depth B is p1.y, and
// `movsx edx, word [esp+0x32]` at depth B-4 is B+0x2e, which is p1.z. So the
// base is p1.z, as the file has it, and the "p1 and p2 swap their frame slots"
// that the spelling produced was an artefact of the misreading.
//
// DEAD LEVERS RE-SWEPT AT THE 68.25% BASELINE. All of these scored exactly
// 68.25% and 501 bytes, i.e. identical output, so do not spend runs on them:
// `rad`/`step` at function scope; `step`/`rad` swapped in the declaration; a
// dummy uninitialised local inside the guard to shift the callee-saved
// allocation; a two-step `int r = radius; rad = r << 16;`; `(unsigned)radius
// << 16`; a local `Pos*` copy of pos; `sx`/`sy` at function scope; `sx`/`sy`
// declared in the other order; `if (radius != 0)`; `if (radius > 0)`; a local
// copy of radius for both the guard and the shift; `x2`/`y2` at function
// scope; a `(void)i;`; a plain `for (int i = 0; i <= n; i++)` loop (38.6%, MSVC
// peels the first iteration); `angle` inside the guard (45.2%, and the prologue
// rotates). Most of these are re-measured at the 82.84% baseline in the dead
// list above and are still flat.
//

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

// Two accessors, and both of them are load-bearing; see findings 1 and 2 at
// the top of the file. YFrac gives the 16.16 y read a boundary so MSVC
// schedules its load where the original has it. Higher must return int, not
// bool: that is what keeps the terrain-height comparison a branch instead of a
// `setg`, and the branch is what puts pos in edi and rad in ebx.
static inline int YFrac_00438ea0(Pos_00438ea0* p) { return *(int*)&p->y_frac; }

static inline int Higher_00438ea0(int a, int b) { return a > b; }

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
        // x2 and y2 are uninitialised on purpose: that is what produces the
        // original's cold path, where the `jl` over the loop jumps to
        // `mov esi, [slot] ; mov ebx, [slot]` and falls into the text block.
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
                int vy1 = YFrac_00438ea0(pos);
                *(int*)&p1.x_frac = vx1 - nx1;
                *(int*)&p1.z_frac = vz1 - nz1;
                *(int*)&p1.y_frac = vy1;
                p1.y = Higher_00438ea0(pos->y, FUN_00485070(&p1))
                        ? pos->y : FUN_00485070(&p1);
                angle += step;
                Pos_00438ea0 p2;
                int nx2 = -FUN_004b70ef(angle, rad);
                int nz2 = -FUN_004b7123(angle, rad);
                int vx2 = *(int*)&pos->x_frac;
                int vz2 = *(int*)&pos->z_frac;
                int vy2 = YFrac_00438ea0(pos);
                *(int*)&p2.x_frac = vx2 - nx2;
                *(int*)&p2.z_frac = vz2 - nz2;
                *(int*)&p2.y_frac = vy2;
                p2.y = Higher_00438ea0(pos->y, FUN_00485070(&p2))
                        ? pos->y : FUN_00485070(&p2);
                int sx = view->scroll_x;
                int sy = view->scroll_y;
                int x1 = p1.x - sx + 0x80;
                // KNOWN WRONG, and the original is `- sy`: see finding 3 at the
                // top of the file. The original spills scroll_y into pos's dead
                // argument slot at 0x438fda and reloads it from there at
                // 0x438ffa for exactly this subtraction. Spelling it correctly
                // costs 37 points to MSVC 5's register allocator, in about 200
                // different spellings, so the byte-closer `- i` stays here.
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
