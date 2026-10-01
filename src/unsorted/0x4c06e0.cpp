// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Space Bunny Free. Names are provisional.
//
// Space Bunny Free pass (issue #4498): 84.2 -> 96.5 percent, 200 of 200 bytes,
// still no MATCH. The count block is now byte-identical, including the two
// instructions nine earlier passes could not get: the surface pointer is
// reloaded into ebp, the 16-bit pitch is read off it twice, the count's x1
// stays in edx, and the offset is `imul eax,[row]; add edx,eax`.
//
// Three things were needed, and only the first is a spelling the source can
// plausibly have:
//
// 1. The count block writes the offset *twice* in two different spellings: a
//    shared local for the depth pointer and the full expression again for the
//    colour pointer, with a local for the start of the span:
//        int start = span->x1;
//        int off = start + row * surf->pitch;
//        p = p + (start + row * surf->pitch);
//        if (d != 0) { d = d + off; ... }
//    `p += off` instead of the repeated expression gives 84.2 percent, and
//    dropping the `start` local (reading span->x1 inline) gives 72.9 percent,
//    so both are load-bearing. What fixes the register colouring is that x1 is
//    a *local* of the count block, read before the count, so the allocator no
//    longer coalesces the count's x1 with the dead head x1 and gives the
//    callee-saved register to the surface pointer instead.
//
// 2. One inline function in the translation unit, of any kind, called or not.
//    With the count block above and no extra function the file scores 88.9
//    percent and 202 bytes: the row product goes to ebp (`mov ebp,[row]; imul
//    ebp,eax; add edx,ebp`) instead of eax. Adding a single `unsigned short
//    Pitch() { return pitch; }` member to Surface_004c06e0 and using it for the
//    two pitch reads is enough to get the original's `imul eax,[esp+0x14]`. A
//    free `static inline unsigned short Pitch(Surface*)` works the same, and so
//    does an inline function that is never called at all, so this is compiler
//    state and not the helper's body: 1 to 6 and 12 uncalled inline functions
//    give 96.5 percent, 8 and 19 give 88.9, 20 gives 81.2, while a `static`
//    function *declaration* or an `extern` variable changes nothing. The member
//    is used twice, so nothing dead is left in the file.
//
// 3. Nothing else: no `int x1 = span->x1;` is needed, and a width local `w`, a
//    `lim` local for the clamp, a `z1` local for the division numerator, `n`
//    declared inside the block or at the top, the arms in either order, the
//    `start + row * surf->pitch` or `surf->pitch * row + start` order, and
//    `p`/`d` declared in either order all still give 96.5 percent.
//
// Still differs, 3.5 percent, three instructions in the loop preheader:
//    original: mov eax,[esp+0x18]     the slope, loaded first
//              lea ebp,[ebx]         the counter copy, inside the arm
//              mov bl,[esp+0x20]     the colour byte, after the copy
//    ours:     mov ebp,ebx           the counter copy, hoisted above add edi,edx
//              mov bl,[esp+0x20]     the colour byte, hoisted above the slope
//    and the fill arm copies the count with `mov ecx,ebp` where the original
//    uses `lea ecx,[ebx]`. MSVC 5 merges the loop's induction copy with the
//    count the fill arm needs, so one copy serves both arms and is hoisted to
//    the common dominator. Every attempt to confine it to the depth arm adds a
//    local, and one extra local reallocates the whole function (the head puts
//    span in esi and surf in ebx, 47 percent), so the lever has to be a
//    spelling that keeps the local count at eight. Tried and worse: a separate
//    `int i = n` counter anywhere (47.3), two locals with the same value so the
//    loop and the fill arm use different ones (57.6 to 61.5), the count
//    recomputed from the fields or from `start` in the fill arm (51.5 and
//    61.5), `while (n-- > 0)` (86.5), `while (n) { ... --n; }` and
//    `for (; n > 0; --n)` (57.3 each), the arms in the other order (74.1), a
//    `goto` between the arms (74.1), the colour byte or the slope copied into a
//    local in the arm (unchanged, folded away), `register` on any of the eight
//    locals (unchanged), and `volatile` on the count (76.7). A permuter run
//    from this file (3771 candidates, 5 minutes) found nothing better either,
//    and neither did all 128 header sets of headers.py (flat at 96.5) or
//    padding declarations: N `extern int`, `extern void __cdecl f(void)`,
//    `extern int __cdecl f(int,int)`, `static int` or `typedef int` lines for
//    N = 0 to 24 all stay at 96.5 percent or drop to 88.9 (the extern form
//    flips at N = 16, the others between 7 and 20).
//    Also tried against this residual, all 96.5 percent or worse: a dead store
//    inside a statically folded branch (`int t = 0; if (t) n = 0;` or
//    `if (t) off = 0;` or `if (t) p = 0;` or a dead store through a dead
//    pointer) in front of the arms, inside the depth arm, in the fill arm and
//    after the clamp; a ternary that folds away (`0 ? a : a` on the start local,
//    `a > b ? a - b : a - b` on the count, and a guard wrapped in a folded
//    conditional); and reading the same field through a second `Span*` or
//    `Surface*` local, which unlike an extra `int` local does not break the
//    head (96.5 percent either way). The last three instructions look
//    unreachable from the source: they are one copy that MSVC 5 merges with the
//    count the fill arm needs, and nothing in the source can stop that merge
//    without adding a local, which reallocates the whole function.
//
// What the 84.2 percent passes established, kept here because it is what makes
// the 96.5 percent version legible: the matched sibling 0x4c0b10 has the same
// count block shape (surface pointer in ebp, the 16-bit pitch read off it
// twice, x1 in edx, the count in ebx), so the target shape does compile in this
// family. What decides it is register *pressure*, not spelling: in 0x4b10 the
// start value is spilled to a stack home right after the count because that
// loop needs all six registers, and a value with a stack home is given a
// scratch register, which frees ebp for the surface pointer.
//
// Scored and tied at 84.2 with the old count block (identical bytes, about 70
// shapes): a `Surface*` local for the pitch reads assigned after the clip
// (`s = surf`, declared-then-assigned, used in the clamp only, in the offset
// only), a `unsigned short* pp = &surf->pitch` read as `*pp`, a reference
// `Surface& s = *surf`, an inline `Pitch(surf)` getter, inline `Off(surf,row,x)`
// and `Cnt(span)` helpers (alone and together), `p = &p[off]`, `p = p + off`,
// `(int)row *`, `unsigned`/`long` off, `surf->pitch * row` and
// `span->x1 + ...` offset orders, all six orders of the off/z/n declarations,
// `n` computed before the guard or after the clamp, a named width local
// (`int w = x2 - x1` and `int w = span->x2 - span->x1`), a named dz local, a
// `z1` local, a dead local, the clamp without the reload of x2, the clamp
// chained as `x2 = span->x2 = ...`, the guard as `span->x2 > span->x1` (83.0
// percent, 200 bytes), the clip testing the x1 local, the offset split into
// two adds, the offset written out twice, both parameters as references, the
// guard and the count through a second `Span*` local, `int&` references to the
// fields, the pitch read as `*(unsigned short*)surf`, `unsigned short&` to the
// fields, `if (d)` instead of `if (d != 0)`, reversed arms, an early `return`
// after the loop arm, both struct definition orders, `#pragma pack(2)`, and
// every one of the 128 header sets headers.py can try (flat at 84.2). Worse,
// and each for a reason worth knowing: a pointer-returning `static inline`
// identity (MSVC 5 does not inline it, so the call plus its `push ecx` stack
// reservation rewrites the prologue, 223 bytes), a `pitch` local (75.3 and
// 71.9), a separate loop counter `int i = n` inside the depth arm (46.8:
// adding a fifth local reallocates the *head*, span moves to esi and surf to
// ebx), the same counter before the arms (51.7), a `for` counter (48.8),
// `while (n-- > 0)` (66.3), a `start` local in the old count block (77.5 and
// 73.3, 202 to 204 bytes), a second `p`/`d` pointer declared inside the count
// block (36.9 and 48.8), reading `surf->depth` twice (63.5), the count from the
// clamped x2 local (67.4), two locals with the same value (52.9), the memset
// count recomputed from the fields (52.9), and 20 or more uncalled
// `static inline` helpers in the file, which is a real compiler-state
// threshold: 1 to 19 of them are 84.2 percent and 20 or more give 72.9 percent
// and 194 bytes (a head change, not a count-block change).
//
// Draws a full horizontal span: every pixel from x1 to x2 gets the colour
// when it passes the depth test (the depth buffer keeps the integer part of
// the 16.16 depth), or the row is filled unconditionally when the surface has
// no depth buffer. Sibling of 0x4c0a90, which plots only the two end points,
// and of 0x4c0b10, which walks two ramps.
//
// Still differs (74.1%, up from 61.5%). The head, the clip and the loop body
// are now instruction for instruction identical to the original. What is left
// is one 3-cycle of registers in the span-count block:
//
//   original: mov ebp,[esp+0x1c] / mov ax,[ebp]  (surf in ebp, pitch off it)
//             mov edx,[ecx] / mov ebx,[ecx+4] / sub ebx,edx   (n in ebx)
//             xor eax,eax / mov ax,[ebp] / imul eax,[esp+0x14]
//             add edx,eax / add edi,edx
//   ours:     mov ebx,[esp+0x1c] / mov ax,[ebx]
//             mov ebp,[ecx] / xor edx,edx / mov dx,[ebx] / sub ebx,ebp
//             imul edx,[esp+0x14] / lea eax,[edx+ebp] / add edi,eax
//
// The original re-reads the pitch off a `surf` pointer it keeps in ebp, and
// computes the byte offset with `add edx, eax` on the x1 already sitting in
// edx; ours keeps the pitch in a `pitch` int, hoists the second pitch read
// up to the first, and reaches the same sum through a `lea` into eax. So the
// original's `surf` is a live callee-saved local in the count block, and our
// `int pitch = surf->pitch;` is what loses it. Every spelling tried for that
// block (an `int pitch` local, a `Surface*` local, a `start` local, the width
// as `x2 - x1` or `span->x2 - span->x1`, the offset as one statement or
// `row * pitch` then `+= span->x1`, the loop as do/while or for, an unsigned
// char pitch, and extracting the whole draw block into a static inline helper)
// stays at 74.1% or below; 74.1% is reached by many shapes and is a ceiling
// for this source family.
//
// A third pass added four more: the pitch read into a local first with the
// clamp against that local (69.0%), the same as a ternary with the clamp
// folded (69.0%), a `unsigned short` local matching the 16-bit load the
// original performs (70.2%), and an unused surface pointer local (74.1%, the
// same bytes). So the remainder really is just which register the surface
// pointer lands in, with the span's x2 field moving between edx and ebp to
// match it, and 74.1% is a ceiling for this source family.
//
// What did move it up, from 61.5%:
// - `int x1 = span->x1; int x2 = span->x2;` and dividing by `(x2 - x1)`. The
//   two named locals are what move the span pointer into ecx and the depth
//   pointer into esi, which fixes the whole prologue and the loop's pointer
//   registers (63.9%).
// - The pitch clamp written on the `x2` local (`if (x2 > pitch - 1) { ... }`
//   followed by `x2 = span->x2;`) and then an `int pitch = surf->pitch;`
//   after it (74.1%). The clamp on the local is what keeps n in ebx through
//   the count block instead of spilling it.
//
// A fourth pass (#1213, deepseek-v4.1-flash) scored four more dead ends, all
// below 74.1%: a `start` local with the offset reading `surf->pitch` inline
// (70.2%, 199 bytes, surf lands in eax and start in ebx), the same with the
// offset as `row * surf->pitch + start` (70.6%), reusing the top-level `x1`
// local for the count after the clamp (70.6%), and the shared `off` local
// (70.6%). So 74.1% still stands.
//
// The head shape is the same as 0x4c0b10's, which is also stuck on an
// ebx/ebp choice at the same two field loads.

//
// Fifth pass (#2034, deepseek-v4.1): baseline re-confirmed at 74.1%, then about
// 35 more source shapes swept with a script (each scored by check.py), none above
// 74.1%. Two findings that narrow the remaining gap:
// - The loop increment order is source order. The original emits inc edi; inc esi;
//   add ecx,eax (both pointers, then z += slope); the file's `p++; z += slope; d++;`
//   emits inc edi; add ecx,reg; inc esi. Writing `p++; d++; z += slope;` does move
//   the add after the two incs, but the slope is then reloaded per iteration into
//   ebp, which swaps one mismatch for another, so the score stays 74.1%.
// - The slope is spilled to [esp+0x18] in both versions. The original reloads it
//   once before the loop (`mov eax,[esp+0x18]`, then `add ecx,eax` in the body) and
//   uses eax for it; ours reloads it inside the loop (`mov ebp,[esp+0x18]` with the
//   reordered increments, `mov edx,[esp+0x18]` in the file). Explicit pre-loop
//   copies (`char c = color; int s = slope;` used in the body) are coalesced away
//   and change nothing, so the load placement is the allocator's, not the source's.
// Also scored and all at 74.1% or below: a z1 local used by the count block (65.9%),
// the same keeping span->z1 (66.3%), a counter copy / for / while loop (50 to 50.6%),
// a slope copy (74.1%), `int len` for the guard (50.3%), an offset local (64.7%),
// `memset(p, (unsigned char)color, n)` (72.5%), a char local for memset (74.1%),
// the pitch loaded inline instead of hoisted (70.2%), x2/pitch locals as unsigned
// short (68.6%), a field read in the clamp (66.3%), pointer-order and offset-order
// swaps (74.1%), and `span->z1 -= slope * span->x1;` (74.1%). Every variant that
// ties 74.1% emits identical bytes, so the file is already at a fixed point of the
// shapes tried: what is left is one register-coloring decision (surf reloaded into
// ebp with x1 in edx and the pitch in eax, versus surf in ebx here).

// Sixth pass (#2034, deepseek-v4.1): the 0x4c0b10 MATCH recipe does not
// transfer to this function. Applying its head and tail (int w, int start,
// int count, while (count--), unsigned char color, the depth offset reading
// span->x1) scores 44.7 to 52.3% in all combinations, worse than the 74.1%
// baseline, which must keep the x1/x2 locals and the count read from the span.
// Also scored and worse: inline surf->pitch with a separate `int i = n` loop
// counter (46.8%), the same with char color (49.4%), `int n` before the guard
// with the pitch local declared after it (45.0%), the two-step offsets (48.0 to
// 50.6%), and unsigned char color (72.5%, 204 bytes, scored in the fifth pass). The
// 74.1% file is unchanged and is a local optimum: only the register colouring of
// the count block still differs (surf into ebp with x1 in edx there, versus
// surf into ebx with x1 in ebp here).

// Seventh pass (#2426, deepseek-v4.1-flash): baseline 74.1% re-confirmed. Inlining
// `surf->pitch` (dropping the `int pitch` local so the surface pointer would stay
// live into the offset, the shape the original register use implies) gives 70.2% /
// 202 bytes in both addition orders and also through an explicit `Surface* s` local
// used from the clamp onward (70.2%, 202 bytes); using `s` for the p/d init too is
// the same. So the pitch-local shape is required and the residual stays the
// ebx/ebp colouring of `surf` in the count block.

// THIS SESSION (deepseek-v4.1-flash, 10-minute box): 74.1 -> 75.3 percent, 199
// of 200 bytes. Three changes in one file: the color parameter is now
// `unsigned char` (the fill arm loses its movsx), the fill passes
// `*(int*)&color` to memset (the 0x4c0330 type-pun trick, gives the plain
// `mov al` byte load), and the loop increments are `p++; d++; z += slope;`
// which matches the original's `inc edi; inc esi; add ecx, ...` order. Still
// differs: the span-count block allocation (the original keeps surf in ebp and
// re-reads the pitch off it late with x1 in edx accumulating the offset; ours
// keeps surf in ebx and x1 in ebp with the offset materialised twice), and the
// loop counter stays in ebx instead of being copied to ebp so the color byte
// can land in bl. Inline `surf->pitch` offset shapes (v2 to v4, v6, v8) are all
// 71.6 to 72.4 percent at 211 to 215 bytes, worse.

//
// Ninth pass (deepseek-v4.1-flash, 60-minute box): no MATCH, best score moved
// 75.3 -> 76.6 percent (the file now holds that 76.6 percent shape, 215 bytes).
// A 792-shape in-process sweep of the count block (pitch local before/in/after
// the guard, int/unsigned short/unsigned int pitch, four offset spellings,
// two count spellings, three loop forms, two memset counts) found only one
// family that puts surf in ebp (the register the original keeps it in): the
// offset must read the *head* `x1` local rather than `span->x1`, which is both
// semantically wrong after the clip and reallocates the whole head (59.0
// percent, 200 bytes; it emits `imul ebp,ebx`, `sub ebp,[ecx]` there). Every
// shape that keeps the head at 75.3 percent or above puts surf in ebx or eax.
// A byte-pattern scan of the whole exe shows the original's count block
// (xor eax,eax / mov ecx,[ecx+0x18] / mov ax,[ebp] / imul eax,[esp+0x14]) is
// unique, so there is no sibling to copy from.
//
// The 76.6 percent file below is a length artefact, not a structural step: it
// reads `surf->pitch` inline in the depth offset (so surf stays in a register
// and the loop tail matches: `mov bl,[esp+0x20]`, `add ecx,eax`, `dec ebp`),
// but it spills `n` to [esp+0x1c] and computes `row * pitch` twice. The
// 199-byte base, saved at build/scratch/0x4c06e0/BEST_75.3_199bytes_base.cpp,
// differs in exactly one hunk (the count block) and is the better starting
// point: it has the head, clip, offset and memset arm byte-identical, and
// needs only the `mov ebp,[esp+0x1c]` / `mov edx,[ecx]` colouring. Restore it
// first if you prefer the shorter diff.
//
// Where the 76.6 percent version still differs: surf lands in ebx (not ebp),
// x1 in eax (not edx), the pitch in edx and `n` spilled to [esp+0x1c], the
// pitch local is read early so surf dies before the count, and the memset arm
// copies the count with `mov ecx,ebp` where the original uses `lea ecx,[ebx]`.

#include <string.h>
struct Span_004c06e0 { int x1; int x2; char unknown_8[0x18 - 0x8]; int z1; int z2; };
struct Surface_004c06e0 {
    unsigned short pitch;              // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
    // The pitch accessor is not a claim about the original: one inline function
    // anywhere in the file is what puts the row product in eax (see the notes).
    unsigned short Pitch() { return pitch; }
};

// FUNCTION: 0x4c06e0
void __stdcall FUN_004c06e0(int row, Span_004c06e0* span, Surface_004c06e0* surf, unsigned char color)
{
    unsigned char* p = surf->bits;
    unsigned char* d = surf->depth;
    int x2 = span->x2;
    int slope = (span->z2 - span->z1) / (x2 - span->x1);
    if (span->x1 < 0) {
        span->z1 = span->z1 - span->x1 * slope;
        span->x1 = 0;
    }
    if (x2 > (int)surf->Pitch() - 1) {
        span->x2 = surf->Pitch() - 1;
        x2 = span->x2;
    }
    if (span->x2 - span->x1 > 0) {
        int start = span->x1;
        int off = start + row * surf->pitch;
        int z = span->z1;
        int n = span->x2 - span->x1;
        p = p + (start + row * surf->pitch);
        if (d != 0) {
            d = d + off;
            do {
                unsigned char zi = (unsigned char)(z >> 16);
                if (*d <= zi) { *p = color; *d = zi; }
                p++; d++; z += slope;
            } while (--n);
        } else {
            memset(p, *(int*)&color, n);
        }
    }
}
