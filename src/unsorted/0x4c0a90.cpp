// Decompiled by Opus, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by GPT-6, re-verified by GPT-6, re-examined by Claude Opus 5.5. Names are provisional.
// Codex / GPT-6 retry for #5459 (2026-10-04): rechecked at 96.2%. The depth
// pointer's early load and the original register pair remain mutually
// exclusive in the documented source shapes. Existing source remains best.
// #5431 Codex retry: re-confirmed 96.2%; only the early depth-load and
// argument-register ordering remain different after the span guard.
// #5443 Codex retry: re-confirmed 96.2%; the same prologue order remains.
// #5422 Codex retry: re-confirmed 96.2% at 118 bytes. The shape with the
// original body has the late depth load; moving that load above the guard
// restores the prologue but swaps the register allocation for the body.
// Plots the two end points of one span row: the pixel at each end gets the
// colour when it passes the depth test (the depth buffer keeps the integer
// part of the 16.16 depth), or unconditionally when the surface has no depth
// buffer.
//
// Status: 96.2% (118 of 118 bytes, ours is already the original's size).
//
// Claude Opus 5.5 pass (#5531, about 35 minutes, still 96.2%), shape B:
// - With c2prio's K counts (B0 7, B1 6, B2 6, B3 4, B4 5, B6 3) the
//   product's priority minus w's is 9*K1 + 3*K2 + K3 - 8*K0 - 4*K4 - 2*K6
//   = -6, and a tie goes to the product (+0x40 18 against 4). So it takes
//   one more candidate in B1, two in B2, or one fewer in both B4 and B6.
// - Only a candidate that FUN_0041a6f8 drops is counted: `int pitch` is
//   forwarded earlier by FUN_0041c72e (c2prio --ids), before the priority
//   pass, and its load then becomes the product's first operand.
//   FUN_0041a6f8 forwards a local whose one use is a conversion it does not
//   do in place. That is a sign extension (`short pitch`: `movsx`, 89.5% in
//   the form with no x1 local). A zero extension of a 16-bit candidate is an
//   in-place `and reg, 0xffff`, so the local is kept (73.6%). The RTM
//   compiler gives the same three scores.
// - `unsigned int pitch : 16; unsigned int pitchHi : 16;` in the surface
//   gives the original's colouring (surf ecx, w edx, product ecx, 83.8%)
//   because the product absorbs the in-place `and ecx, 0xffff` (cost 12 in
//   B1). But the pitch is then a dword load into the product register.
//   A `(unsigned short)` cast on that field goes back to the mirror.
// - Mirror (56.6%) or worse, all normalised to the same IL:
//   `unsigned short pitch : 16` read directly or into short, int or
//   unsigned short locals; int, unsigned or long pitch locals with
//   `(unsigned short)` or `& 0xffff` at the use (43.0%, 79.2%); static
//   inline Mul(int, int) or Mul(int, unsigned short) on the field or a local;
//   a bool for `z != 0` in B1 (42.2%, setcc); a colour copy in B1; int,
//   short and unsigned char locals for span->z1 and *z in B2 (none becomes a
//   candidate); `off += x1; z += off`, `z += off += x1`, `row *= pitch`;
//   `+ (row - row)`, `(row | row)`, `+ (x1 - x1)` and similar (folded);
//   register, const, unsigned and long on w, x1, p and z; p or z rebuilt
//   through int, unsigned long or char* casts; `w >= 1`, `0 < w`, `if (z)`;
//   z1 or z2 written inline (38.9-53.7%); the colour as int or char; a
//   separate depth local with z rebuilt from it in B2 (42.6%).
// - Symbol ids: enum padding that puts row's id below 65536 and the int
//   pitch local's above it, and 0 to 30 dummy externs with `int y = row`,
//   leave the int-pitch product order alone (78.1%), so it is not the
//   operand-order-by-id rule.
// - A 10-minute permuter run from the 89.5% short-local form (10795
//   candidates) reached only 91.6% (121 bytes), by dereferencing the pitch
//   value as a pointer, which is not a lead.
//
// Claude Opus 5.5 pass (#5345), shape B (`unsigned char* z = surf->depth;` above the
// guard, the product written twice), looking for a candidate C2 counts in K(B1) and
// then drops, with a zero-extending forwarded load:
//   * `int pitch = surf->pitch;` (or `unsigned int`, `long`, or `int pi = pitch;` from
//     an unsigned short local) is such a candidate: it is dropped and forwarded, the
//     product temporary goes to 84 and w gets edx as in the original. But the
//     forwarded load becomes the product's first operand (`xor ecx, ecx; mov cx,
//     [surf]; imul ecx, [row]`, surf then in ebx), 80.0%, whatever the source order
//     (`row * pitch`, `r * pitch` with a later `int r = row;`) and the declaration
//     count (0 to 39 and 3776 to 3799 dummy externs swept). /Gi on plain shape B
//     gives the same 80.0% code. The original needs row first (`mov ecx, [row];
//     imul ecx, ebx`), which only the field spelling (with <windows.h>) gives.
//   * Every 16-bit unsigned local, and every zero-extending cast or `& 0xffff` on a
//     short local, is zero-extended in place (`and reg, 0xffff`, 4 references), so it
//     is never dropped (73.6-85.7%). Only a sign extension (`short`, 89.5%) is
//     forwarded as a load, and that is `movsx`.
//   * No change at all (56.6%, the mirror): inline helpers returning the pitch or the
//     row offset (int, unsigned short, unsigned int, short, by reference), the
//     0x4c0b10 sibling's spellings (`p += row * surf->pitch; p += start;`, p or d
//     declared first, a start local), a named or modified `row` (`row *= pitch`),
//     `off` locals, constant-bearing no-ops on the pitch, extra 8/16-bit locals for
//     the first depth test (`unsigned char d1 = *z;`, a short or int step for z1, a
//     colour copy). A bool for `z != 0` emits setcc.
//   * `row *= surf->pitch; row += span->x1; p += row; ... z += row + span->x1;` comes
//     out 88.7% with the original's colouring and every load in place, but it adds x1
//     to z twice, so it is wrong; it shows that one more reference to the product in
//     B1 is all the colouring needs.
// GPT-6 retry (#5230): rechecked at 96.2%; the depth/bits/x2 load order remains.
// #5321 Codex retry: confirmed the documented shape-B `short pitch` candidate
// at 89.5% with `<windows.h>`; making the local unsigned or casting it back
// to unsigned scores 73.6%. Restored the 96.2% shape-A best.
// #5279 Codex retry: re-confirmed 96.2%. The duplicated-product pitch-local
// variant scored 56.3% with explicit unsigned casts and 37.3% without them;
// neither combined the depth-load order with the required surf/w colouring.
//
// Claude Opus 5.5 pass (#5151), with tools/c2prio.py. The surf/w "tie" below
// is not a tie: it is a priority gap, and these are C2's own numbers.
//   Shape B (`unsigned char* z = surf->depth;` above the guard, named
//   `int off = row * surf->pitch;`): z 86, w 66, off 60, p 58. w is coloured
//   before off and takes ecx, off gets edx, surf follows off into edx: the
//   mirror. w's 66 is B0 +56 (K 7, cost 8) -6 -6 -4 (live through B1..B3)
//   +20 (B4) +6 (else). off's 60 is B1 +48 (K 6, cost 8) +12 (B2).
//   Shape A (this file) has K(B0) = 6 (z is not loaded there), so w is 58 and
//   off (60) wins ecx; that is the only reason the colouring is right here.
//   About 150 shape-B spellings (helpers, CSE'd or named offsets, x1 local or
//   field, early returns, named *z and z1 locals, w spellings, int/unsigned
//   types, else-first, pointer locals for bits/depth) all give w 65-67 and
//   off 56-60: the optimizer normalises them to the same IL.
//   What does work is one more candidate counted in K(B1): off then gets
//   +8 and w -1 (68 against 65). A `short pitch = surf->pitch;` local inside
//   the guard, with the product written twice (`p += pitch * row +
//   span->x1;` and `z += pitch * row + span->x1;`, no x1 local), is such a
//   candidate: C2 counts it in K(B1) and then drops it, forwarding the load
//   into its one use. With `#include <windows.h>` (or 78+ dummy externs; with
//   none the product is pitch-first and it is 48.5%) every instruction then
//   matches the original, registers and order, except one: `movsx ebx, word
//   ptr [ecx]` where the original has `xor ebx, ebx / mov bx, [ecx]`.
//   89.5% (116 bytes), so it is a lead, not the file. An unsigned 16-bit local
//   is not dropped (its zero extension is an in-place `and`, 4 references):
//   `unsigned short pitch` there is 73.6% (`mov di,[ecx] ... and edi,0xffff`),
//   and an int or unsigned int local is substituted by the optimizer before
//   C2 counts anything. So the missing piece is a counted-then-dropped
//   candidate in B1 (or two in B2) whose forwarded use is a zero-extending
//   load; nothing tried here (casts, references, pointer locals, a copy of
//   row, a copy of color, inline helpers with short or int parameters) gives
//   one. The product's operand order (row first) also depends on the TU's
//   symbol count: with `int y = row;` it flips every 16 declarations.
// GPT-6 retry (#4970): /Gi on this source is 92.5%; loading depth above the
// guard in a scratch variant with /Gi is 78.1%. The best source remains below.
// What is left is exactly two instructions' worth of order, both in the
// prologue: the original loads surf->depth (`mov eax,[ecx+0x14]`) BEFORE
// `push esi` and loads span->x2 (`mov edx,[ebp+4]`) BEFORE surf->bits
// (`mov esi,[ecx+0x10]`); ours loads bits, then x2, then depth after the
// `jle`. Everything from `xor ebx,ebx` to the last `ret 0x10` matches, with
// the same registers (surf=ecx, span=ebp, w=edx, p=esi, x1=edi, z=eax,
// product=ebx), the same `mov ecx,[row]/imul ecx,ebx/lea ebx,[ecx+edi]`,
// the same `add ecx,edi/add eax,ecx` z advance and the same branch targets.
//
// Two shapes, and they cannot be combined (this is the whole remaining gap):
// A. depth pointer assigned inside `if (w > 0)` (this file): the original's
//    colouring and code body, wrong load order, 96.2%.
// B. depth pointer initialised above the branch (`unsigned char* z =
//    surf->depth;`), which is required to get the load to the top: the first
//    13 instructions then match one for one, and with the offset written
//    inline (`p += surf->pitch * row + x1;` then `z += surf->pitch * row +
//    span->x1;`, the way the MATCHED src/unsorted/0x4c7a20.cpp writes the
//    same struct) the product block and the z advance match too. But the
//    allocator then gives ecx to w and edx to the surf argument, so the whole
//    function is a perfect ecx<->edx mirror of the original and the score is
//    56.6%.
//
// What moves the ecx/edx choice: whether x1 is still live at the branch.
// In B, x1 has to survive the `jle` because it is added to the row offset a
// second time after the test (`add ecx,edi/add eax,ecx` recomputes off+x1
// rather than reusing ebx, which is what the original does), and then the
// argument lands in edx. Every spelling that makes x1 die before the branch
// (folding it into the offset, `int off = row * surf->pitch + x1;` with
// `p += off; z += off;`) puts surf back in ecx, but the z advance collapses
// into the addressing mode (`mov bl,[eax+ecx]`) and the function comes out 116
// bytes, best 85.7%. So the search is for a source with x1 live across the
// branch AND surf in ecx; no declaration order, assignment order, type,
// cast, comparison or operand-order spelling moves it.
//
// Measured and rejected (all with the real checker, via the compile-only
// scorer in build/scratch/0x4c0a90/):
//   256 header combinations (headers.py): 96.2% for every one, including
//     <windows.h>, <stdlib.h>, <string.h>, <memory.h>.
//   permute.py, 15 min, 3918 candidates: 96.2% -> 96.2%, nothing.
//   All 24 declaration orders x {depth inside, depth above}: the shape-A ones
//     score 90.6-96.2% (six orders tie at 96.2%) and never fix the load order;
//     every shape-B one is 56.6%.
//   All 24 assignment orders of the same four locals (declare, then assign):
//     56.6% for all 24.
//   Shape B with the offset inline/named, x1 folded or separate, spelled with
//     the local or the field, `row * pitch` or `pitch * row`, with a cast,
//     unsigned, in a named local, or folded: 45.7-85.7%, never above 96.2%.
//   The test spelled `span->x2 > x1`, `x1 < span->x2`, `0 < span->x2 - x1`
//     or the difference as a common subexpression: 34.6-80.0% (these produce
//     114-129 bytes, so the original really does use a named w).
//   `w = span->x2; ... w -= x1;` (so x2 can be read before bits) and `int x2`
//     as its own local declared first, in both shapes: 78.1-94.3%, and the
//     bits load is still scheduled first.
//   Two-step w, pitch in its own local, an inlined `test_depth()` helper by
//     value, colour as `char`, one reused depth variable, `(unsigned char*)0`
//     instead of `0`: all 56.6%.
//
// Two spellings that must be kept (both cost real bytes when changed):
//   * `z += off + span->x1;` has to read the field. Written with the x1 local
//     the compiler folds the offset into the addressing mode and the whole
//     shape changes (114 bytes instead of 118).
//   * `int off = row * surf->pitch;` computed once, or the inline form; both
//     keep `mov ecx,[row]/imul ecx,ebx`, while `off` including x1 gives the
//     3-instruction `mov ecx,ebx/imul ecx,[row]/add ecx,edi`.
//
// Reusable tooling left in build/scratch/0x4c0a90/: multi.sh compiles a file of
// many variants and scores every function in it at once (about 3 s for 48
// variants instead of 60 s per check.py run), probe.sh prints one variant's
// /Fa listing, sc.sh scores a single scratch file, gen_perm.py,
// gen_assign.py and gen_both.py write the declaration/assignment order sweeps.
//
// DeepSeek V4.1 Flash pass (issue 4814, 3-minute permuter box): best stays
// 96.2%, 118 of 118 bytes. The 3-minute permuter run from this file (2205
// candidates) found nothing, and a run from a clean shape-B seed climbed only
// 56.6 -> 80.0 into implausible source (self-assignments and an empty inline
// helper), so it is not a lead.
// GPT-6 retry recheck: aliasing surf through a local pointer leaves the score
// at 96.2% and schedules the bits load before x2 and depth after the guard;
// keep the source below, which preserves the original register coloring.
//
// New results that map the wall. The prologue needs an entry-block use of
// surf->depth; a load inside `if (w > 0)` cannot be hoisted across the jle, so
// only a shape-B source (z initialised above the branch) reproduces the
// original's depth/x2/bits/x1 load order at all. Shape B with no helper
// variable still colours surf=edx, w=ecx (the mirror); the three instructions
// before `xor ebx,ebx` are the only difference, and its own pitch block already
// matches (`xor ebx,ebx / mov bx,[surf] / mov ecx,[row] / imul ecx,ebx /
// lea ebx,[ecx+edi]`) with surf/w swapped. The whole residual is therefore the
// one coloring decision surf=ecx, w=edx, and the load order is free once that
// is fixed.
//
// One source shape does pin surf=ecx while keeping the entry-block depth load:
// a separate early boolean use of the pointer, `int hd = surf->depth != 0;`
// before the if with `z = surf->depth;` inside. It scores 82.9% (133 bytes):
// the prologue and the whole pitch block then match exactly, but the boolean is
// spilled to a stack slot (`setne dl` / `[esp+0x1c]`) and the arm test becomes
// `test ebx,ebx` instead of `test eax,eax`. Every boolean spelling tried (int,
// long, char, unsigned char, bool, with and without casts, and the condition
// mixed with `z != 0`) is 47.3 to 83.2%, none better than 96.2. So the early
// use forces surf into ecx but costs the pointer register the test needs.
//
// A named pitch local also pins surf=ecx but re-colours the pitch temp. Inside
// the arm, `short pitch = surf->pitch;` with the offsets reading it is 84.6%
// (114 bytes): prologue and tail match, the pitch block is `movsx ecx,[ecx] /
// imul ecx,[row] / lea ebx,[edi+ecx]` instead of the five-instruction original.
// `unsigned short pitch` is 83.8% (120 bytes, `mov cx,[ecx] / and ecx,0xffff`),
// `int pitch` 78.1% (116 bytes, surf moves to ebx and the temp to ecx). A named
// `off` for the product does not separate the two (still 84.6 at best). The
// original's pitch lives in ebx precisely because it is a compiler CSE temp,
// not a source local: naming it lets the allocator reuse dead ecx.
//
// Also swept and flat at 96.2 or worse for this function: shape A with 0-96
// `extern int` declarations and 1-32 uncalled inline functions (no TU state
// hoists the depth load, unlike its sibling 0x4c06e0); shape B with 19 kinds of
// unused declaration at 1-32 copies each (only 116/119-byte recolourings, none
// above 78.1); all 256 header sets on the shape-B named-off form (56.6%) and on
// the inline-offset form (78.1% ceiling). So the only unsolved point is still
// the compiler's surf/w colouring tie, now with the two shapes that each solve
// one half of it recorded above.

struct Span_004c0a90 {
    int x1;                            // +0x0
    int x2;                            // +0x4
    char unknown_8[0x18 - 0x8];
    int z1;                            // +0x18 (16.16)
    int z2;                            // +0x1c (16.16)
};

struct Surface_004c0a90 {
    unsigned short pitch;              // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
};

// FUNCTION: 0x4c0a90
void __stdcall FUN_004c0a90(int row, Span_004c0a90* span, Surface_004c0a90* surf, unsigned char color)
{
    unsigned char* z;
    unsigned char* p = surf->bits;
    int x1 = span->x1;
    int w = span->x2 - x1;
    if (w > 0) {
        z = surf->depth;
        int off = row * surf->pitch;
        p += off + x1;
        if (z != 0) {
            z += off + span->x1;
            unsigned char z1 = span->z1 >> 16;
            if (*z <= z1) {
                *p = color;
                *z = z1;
            }
            z += w;
            p += w;
            unsigned char z2 = span->z2 >> 16;
            if (*z <= z2) {
                *p = color;
                *z = z2;
            }
        } else {
            *p = color;
            p[w] = color;
        }
    }
}
