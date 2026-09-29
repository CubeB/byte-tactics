// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, reworked by Claude Sonnet 5.5. Names are provisional.

// Sibling of 0x4c0a90 (which plots the two end points). Fills the pixels
// between the span ends: walks the depth ramp at +0x18 and the shade ramp at
// +0x20 (both 16.16) one step per pixel, does the depth test against the
// surface's depth buffer when it exists, and looks the destination colour up
// in the 32 x 256 shaded palette table at app+0xc4.
//
// PARTIAL, 98.4 percent, 352 of 352 bytes (was 55.8 percent, 344 bytes). Claude
// Sonnet 5.5 pass (#694). Four changes did all of it:
//  1. Compiler state: the function is very sensitive to it (with N unused
//     `extern int` declarations the score runs between 52 and 62 percent and
//     the size between 334 and 348 bytes, with a period of 32 in N), and
//     `#include <windows.h>` alone (or `<stdio.h>` with `<math.h>`) reaches the
//     state that makes the rest work. headers.py reports the same state for many
//     sets.
//  2. The span width is a named local, `int w = span->x2 - span->x1;` used by both
//     divisions (55.8 to 89.3 percent; x1/x2 named locals, the width written out
//     twice, and reading x2 first all stay at 37 percent). It puts x2 in ebx and
//     x1 in ebp as the original does.
//  3. No `color & 0xff` local: the original reloads and masks the colour argument
//     inside the depth loop (`mov ebx, [esp+0x2c]; and ebx, 0xff`). The old
//     hoisted `int ci = color & 0xff;` was a hack that flipped the ebx/ebp choice
//     of the head; with the width local it is not needed and it costs the frame
//     size (0x10 instead of the original's 0xc).
//  4. `d++` before `z += dz` in the depth loop (96.1 to 98.4 percent; the other
//     23 orders of the four increments score 95 to 97.6).
//
// What still differs (one operand order): the depth pointer offset. The original
// does `add edx, ebp; add edi, edx` (row * pitch, which is shared with the colour
// pointer, plus start), ours does `add ebp, edx; add edi, ebp`. The colour pointer
// offset above it already matches. Spelled as `d += row * pitch + start`,
// `d += start + row * pitch`, two statements in either order, `d = d + (...)`,
// all give identical bytes, so it is a register tie-break rather than an
// expression order.
//
// Inert: `(app->shade + (si << 8))[color]` and `*(app->shade + (si << 8) + color)`
// for the shade lookup, do/while for the depth loop (73.2 percent), `for (; count;
// count--)`, an `int zi`, a `w`-less head with cached z1/s1/x1/x2 locals
// (47 to 52 percent).

#include <windows.h>

struct Span_004c0b10 {
    int x1;                            // +0x0
    int x2;                            // +0x4
    char unknown_8[0x18 - 0x8];
    int z1;                            // +0x18 (16.16)
    int z2;                            // +0x1c (16.16)
    int s1;                            // +0x20 (16.16)
    int s2;                            // +0x24 (16.16)
};

struct Surface_004c0b10 {
    unsigned short pitch;              // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
};

struct App_004c0b10 {
    char unknown_0[0xc4];
    unsigned char* shade;              // +0xc4
};

App_004c0b10* FUN_004b6220();

// FUNCTION: 0x4c0b10
void __stdcall FUN_004c0b10(int row, Span_004c0b10* span, Surface_004c0b10* surf, unsigned char color)
{
    unsigned char* p = surf->bits;
    unsigned char* d = surf->depth;
    App_004c0b10* app = FUN_004b6220();
    int w = span->x2 - span->x1;
    int dz = (span->z2 - span->z1) / w;
    int ds = (span->s2 - span->s1) / w;
    if (span->x1 < 0) {
        span->z1 = span->z1 - dz * span->x1;
        span->s1 = span->s1 - ds * span->x1;
        span->x1 = 0;
    }
    if (span->x2 > (int)surf->pitch - 1)
        span->x2 = surf->pitch - 1;
    int start = span->x1;
    int count = span->x2 - start;
    if (count > 0) {
        int z = span->z1;
        int s = span->s1;
        p += row * surf->pitch;
        p += start;
        if (d != 0) {
            d += row * surf->pitch;
            d += start;
            while (count--) {
                unsigned char zi = z >> 16;
                if (*d <= zi) {
                    int si = s >> 16;
                    *p = app->shade[(si << 8) + color];
                    *d = zi;
                }
                p++;
                d++;
                z += dz;
                s += ds;
            }
        } else {
            while (count--) {
                int si = s >> 16;
                *p++ = app->shade[(si << 8) + color];
                s += ds;
            }
        }
    }
}
