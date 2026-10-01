// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
//
// Space Bunny Free pass (#4226): best 76.6 -> 84.2 percent, still 200 of 200
// bytes, no MATCH. THE WHOLE IMPROVEMENT IS ONE LINE: sharing the byte offset
// in a single `int off` local, used by both `p += off` and `d += off`, instead
// of writing `row * pitch + span->x1` twice. The old shape computed the product
// twice, which put the pitch in edx, spilled `n` and emitted 215 bytes. The
// shared local gives the original's `add edi, edx` / `add esi, edx` pair (one
// add destination for both pointers) and the original's loop tail
// (`inc edi; inc esi; add ecx, eax; dec ebp; jne`). Semantically correct: it
// reads `span->x1` after both clips, as the original does. The variant is in
// build/scratch/0x4c06e0/BEST_84.2.cpp.
//
// Still differs, all of it one register-colouring decision, 9 instructions:
//
//   original: mov ebp,[esp+0x1c]     surf -> ebp
//             mov edx,[ecx]          x1  -> edx
//             mov ebx,[ecx+4] / sub ebx,edx        n -> ebx
//             mov ax,[ebp] / imul eax,[esp+0x14]   eax = row * pitch
//             add edx, eax           edx = x1 + row*pitch = off
//             lea ebp,[ebx]          loop counter copy, lea not mov
//   ours:     mov edx,[esp+0x1c]     surf -> edx
//             mov ebp,[ecx]          x1  -> ebp
//             mov ebx,[ecx+4] / sub ebx,ebp        n -> ebx
//             mov edx,[esp+0x14] / imul edx,eax    edx = row * pitch
//             add edx, ebp           edx = x1 + row*pitch = off
//             mov ebp, ebx           loop counter copy, and placed early
//
// So the original keeps `surf` in the callee-saved ebp and re-reads the pitch
// off it twice (`mov ax,[ebp]` at 0x4c072d and 0x4c0749), leaving edx free to
// hold x1 and accumulate the offset with `add edx, eax` (the in-place `imul
// eax,[row]` form, so the product lands in eax and x1 in edx is the add
// destination). Ours puts surf in edx, x1 in ebp, and reaches the same sum by
// loading row into edx and multiplying there. The second, smaller difference
// is the loop-counter copy: the original emits `lea ebp,[ebx]` (a register copy
// that does not touch flags) and ours emits `mov ebp,ebx`, placed before the
// `test esi,esi` branch instead of after it.
//
// What was tried this pass, all scored with check.py, none better than 84.2%:
// 108 declaration-order x offset-spelling shapes (nine spellings of the offset
// including `surf->pitch * row` first and `(int)`/`(unsigned int)` casts, two
// spellings of the count, all six orders of the off/z/n locals), 72 more with
// a `start` local standing in for span->x1, an `int`/`unsigned short` pitch
// local, both together, and the offset derived from `p - surf->bits`; every
// one ties 84.2% at 200 bytes, i.e. they all emit the same bytes. Notably a
// `start` local never helps: it is what forces x1 into ebp. The offset has to
// read `span->x1` through the pointer, exactly as in the matched sibling
// 0x4c0b10, whose note records the same finding (`d += row * surf->pitch +
// span->x1;` reading the field rather than a cached local is what flips the
// add destination into edx). That sibling is a MATCH and its head allocates
// the same way, so it is the place to look next: its depth offset is a single
// statement written against `span->x1`, and this function's `off` local is
// still one step away from that shape.
//
// Best lead for the next attempt: the original's count block is `surf` in ebp,
// `x1` in edx, `n` in ebx, `z` in ecx, i.e. x1 survives from the `n` computation
// into the `add edx, eax`, so the offset is `span->x1 + row * surf->pitch` with
// the pitch re-read from ebp late. A shape that keeps surf live in a
// callee-saved register across the whole count block, rather than letting the
// allocator park it in edx, is what is still missing. Note that all the
// semantically correct spellings tried give x1 to ebp; the earlier 792-shape
// sweep found the only families that put surf in ebp do so by reading the
// pre-clip head `x1` local in the offset, which is wrong after the clip.
// `unitmap.py --at 0x4c06e0` reports no matched member and no unit, so the
// Surface/Span layouts here are local guesses; they agree with the matched
// sibling 0x4c0b10 (pitch +0x0, bits +0x10, depth +0x14, z1 +0x18, z2 +0x1c)
// and the pitch load is the original's 16-bit `mov ax,[..]`, so the layout is
// not the residual.
//
// GPT-6.1-sol retry for issue #3189: best remains 74.1%, no MATCH. Six checker invocations: baseline, maxx/local-x rewrite (50.0%), unsigned-short pitch guard (68.6%), inlined Plot and Surface* alias (both tied at 74.1%), and a failed declaration reorder compile. Remaining difference is register allocation across the clipped-span count/offset and loop.
//
// Eighth pass (#2984, deepseek-v4.1-flash): baseline re-confirmed at 74.1%.
// Swept ~60 more shapes with a generator (all scored via check.py --sym, free):
// clamp on the x2 local versus on span->x2, slope from span fields versus
// locals, a `pitch` local declared before the slope / after the two pointers /
// after the clamp, `unsigned short` and cast pitch, inline surf->pitch at the
// offset, a separate `int i = n` loop counter, a for-loop, the four increment
// orders (p++;z;d++ / p++;d++;z / ++p;++d;z / p=p+1...), asymmetric offsets
// (p by pitch, d by surf->pitch and vice versa), and four memset spellings
// (plain, cast, (unsigned int)n, (void*)p). Nothing beat 74.1%. Every shape
// that ties emits the same 200 bytes, so this is a fixed point of the compiler:
// the residual is only the allocator's choice of a register for `surf` in the
// count block (original keeps it in ebp and re-reads the pitch off it, ours
// frees ebp and reuses it for x1). See the earlier passes below for the full
// list; no source spelling found across 8 passes moves it.
// Claude Sonnet 5.5 pass (#694): still 74.1%, 200 bytes. Compiler state ruled out:
// N unused `extern int` declarations (0 to 400 in steps of 8) give 200 bytes and 74.1%
// for N = 0 to 48 and again from about 296, and a shorter 193 bytes and 59.2%
// (194 and 66.3% at N = 56) in between, never better; all 128 header sets from
// headers.py give 74.1% or less. What the original does that ours does not: `surf`
// is reloaded into ebp (`mov ebp,[esp+0x1c]`) and stays live in ebp until the pitch
// is re-read right before `imul eax,[row]` (`xor eax,eax; mov ax,[ebp]`), so it
// overlaps `n` (ebx = x2 - x1, x1 in edx); ours reads the pitch early into edx (so
// surf dies early and shares ebx with n) and x1 lands in ebp. The original also
// shares one offset between the two pointers (`add edx,eax; add edi,edx; ...; add
// esi,edx`) where ours does `lea eax,[edx+ebp]` for the bits and `add edx,ebp` for
// the depth, and its memset arm loads `color` with `mov al,[esp+0x20]` (no movsx).
// Scored, all worse than the file: pitch read inline (`row * surf->pitch`, 70.2%),
// with `n` first (45.0%), a shared `int off` (70.6%; 47.3% with n first), `n`
// computed before the `if (n > 0)` (44 to 50.6%), the `int pitch` local declared
// inside the block (68.2 to 70.2%; as `unsigned short` 73.3%), an inlined
// `Offset(surf, row, x)` helper (47.3 to 70.6%), and `unsigned char color` (72.5%,
// 204 bytes: a zero-extending load appears but the loop grows).
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
struct Surface_004c06e0 { unsigned short pitch; char unknown_2[0x10 - 0x2]; unsigned char* bits; unsigned char* depth; };

// FUNCTION: 0x4c06e0
void __stdcall FUN_004c06e0(int row, Span_004c06e0* span, Surface_004c06e0* surf, unsigned char color)
{
    unsigned char* d = surf->depth;
    unsigned char* p = surf->bits;
    int x1 = span->x1;
    int x2 = span->x2;
    int slope = (span->z2 - span->z1) / (x2 - x1);
    if (span->x1 < 0) {
        span->z1 = span->z1 - slope * span->x1;
        span->x1 = 0;
    }
    if (x2 > (int)surf->pitch - 1) {
        span->x2 = surf->pitch - 1;
        x2 = span->x2;
    }
    if (span->x2 - span->x1 > 0) {
        int off = row * surf->pitch + span->x1;
        int z = span->z1;
        int n = span->x2 - span->x1;
        p += off;
        if (d != 0) {
            d += off;
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
