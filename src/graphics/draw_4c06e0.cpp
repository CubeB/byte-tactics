// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// Draws one depth-tested scanline of a flat-coloured polygon: clips the span
// to the surface, then either depth-tests each pixel against the z buffer or,
// with no z buffer, fills the row. Called once per row by FillFlatPolygon.
//
// What matched it (Claude Opus 5.5, issue #4708, after nine passes at 96.5%):
// - Both arms are `while (n--)` loops inside `if (n > 0)`, as in the matched
//   sibling 0x4c0b10. MSVC 5 turns `while (n--) *p++ = color;` into the same
//   `rep stosd` / `rep stosb` sequence as an inlined memset, and a `while
//   (n--)` loop's counter is initialised with `lea reg,[n]` (n plus a folded
//   zero offset), which is where both of the original's `lea reg,[ebx]`
//   copies come from. The earlier `memset` and `do ... while (--n)` shapes
//   could only ever give `mov`.
// - The guard is on `n` itself, so MSVC drops the loops' entry tests.
// - The depth test reads `(unsigned char)(z >> 16)` without a local. With a
//   `zi` local in the compare, its priority (128) beats span's (124) and zi
//   takes ecx, pushing z out of ecx; c2prio.py shows the colouring order.
// - The Pitch() accessor stays: without an inline function in the file the
//   row product goes to ebp (92.4%), as the earlier passes found. The offset
//   written twice (`off` for d, the full expression for p) is also needed
//   (87.7% with `p += off`).
// - <string.h> is needed even though nothing here calls into it (66.7%
//   without it): the header's declarations move the register allocation. The
//   original translation unit includes it for 0x4c0330's memset.

#include <string.h>

struct Span_004c06e0 { int x1; int x2; char unknown_8[0x18 - 0x8]; int z1; int z2; };
struct Surface_004c06e0 {
    unsigned short pitch;              // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
    unsigned short Pitch() { return pitch; }
};

// FUNCTION: 0x4c06e0
void __stdcall FillFlatSpan(int row, Span_004c06e0* span, Surface_004c06e0* surf, unsigned char color)
{
    unsigned char* p = surf->bits;
    unsigned char* d = surf->depth;
    int slope = (span->z2 - span->z1) / (span->x2 - span->x1);
    if (span->x1 < 0) {
        span->z1 = span->z1 - span->x1 * slope;
        span->x1 = 0;
    }
    if (span->x2 > (int)surf->Pitch() - 1)
        span->x2 = surf->Pitch() - 1;
    int n = span->x2 - span->x1;
    if (n > 0) {
        int start = span->x1;
        int off = start + row * surf->pitch;
        int z = span->z1;
        p = p + (start + row * surf->pitch);
        if (d != 0) {
            d = d + off;
            while (n--) {
                if (*d <= (unsigned char)(z >> 16)) { *p = color; *d = (unsigned char)(z >> 16); }
                p++; d++; z += slope;
            }
        } else {
            while (n--) *p++ = color;
        }
    }
}
