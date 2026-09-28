// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
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
// The head shape is the same as 0x4c0b10's, which is also stuck on an
// ebx/ebp choice at the same two field loads.

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
