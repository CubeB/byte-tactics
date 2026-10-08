// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// Draws one depth-tested scanline of a flat-coloured polygon: clips the span
// to the surface, then either depth-tests each pixel against the z buffer or,
// with no z buffer, fills the row. Called once per row by FillFlatPolygon.

// Needed though unused: its declarations move the register allocation.
#include <string.h>

struct Span_004c06e0 { int x1; int x2; char unknown_8[0x18 - 0x8]; int z1; int z2; };
struct Surface_004c06e0 {
    unsigned short pitch;              // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
    // Must stay: without an inline function in the file the row product goes to ebp.
    unsigned short Pitch() { return pitch; }
};

// Not gathered into draw.cpp: its register allocation follows symbol ids.
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
    // Guard on n with `while (n--)` loops inside: gives the original's lea counters.
    if (n > 0) {
        int start = span->x1;
        // Offset written twice: `off` for d, the full expression for p.
        int off = start + row * surf->pitch;
        int z = span->z1;
        p = p + (start + row * surf->pitch);
        if (d != 0) {
            d = d + off;
            while (n--) {
                // No local for the shifted z: it would take ecx.
                if (*d <= (unsigned char)(z >> 16)) { *p = color; *d = (unsigned char)(z >> 16); }
                p++; d++; z += slope;
            }
        } else {
            while (n--) *p++ = color;
        }
    }
}
