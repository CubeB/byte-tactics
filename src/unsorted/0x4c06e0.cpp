// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
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

#include <string.h>

struct Span_004c06e0 {
    int x1;
    int x2;
    char unknown_8[0x18 - 0x8];
    int z1;
    int z2;
};

struct Surface_004c06e0 {
    unsigned short pitch;
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;
    unsigned char* depth;
};

static inline void Plot_004c06e0(unsigned char* p, unsigned char* d, int z, unsigned char color)
{
    unsigned char zi = (unsigned char)(z >> 16);
    if (*d <= zi) { *p = color; *d = zi; }
}

// FUNCTION: 0x4c06e0
void __stdcall FUN_004c06e0(int row, Span_004c06e0* span, Surface_004c06e0* surf, char color)
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
    int pitch = surf->pitch;
    if (span->x2 - span->x1 > 0) {
        int z = span->z1;
        int n = span->x2 - span->x1;
        p += row * pitch + span->x1;
        if (d != 0) {
            d += row * pitch + span->x1;
            do {
                Plot_004c06e0(p, d, z, color);
                p++; z += slope; d++;
            } while (--n);
        } else {
            memset(p, color, n);
        }
    }
}
