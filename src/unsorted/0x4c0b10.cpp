// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.

// Sibling of 0x4c0a90 (which plots the two end points). Fills the pixels
// between the span ends: walks the depth ramp at +0x18 and the shade ramp at
// +0x20 (both 16.16) one step per pixel, does the depth test against the
// surface's depth buffer when it exists, and looks the destination colour up
// in the 32 x 256 shaded palette table at app+0xc4.
//
// PARTIAL, 55.8 percent. What still differs, all of it following from two
// register choices:
//
// 1. The head. The original puts span->x2 in ebx and span->x1 in ebp for the
//    two divisions:
//        original: mov ebx,[ecx+4] / mov ebp,[ecx] / sub ebx,ebp / idiv ebx
//        ours:     mov ebp,[ecx+4] / mov ebx,[ecx] / sub ebp,ebx / idiv ebp
//    Everything downstream that mentions ebx or ebp follows from it (the
//    pitch clip wants surf in ebp and x2 in ebx, and later the loop count in
//    ebx, which we give ebp).
//
// 2. The depth loop's shade index. The original computes it in ebx and ebp
//    (which leaves eax free, so `ds` stays in eax for the whole loop and the
//    loop count is spilled to the span argument slot). We compute it in eax,
//    so `ds` is spilled to [esp+0x1c] and the frame is 0x10 instead of the
//    original's 0xc. Every stack reference below the head is shifted by that
//    one extra dword.
//
// A third pass added four spellings of the head's two field loads, none of
// which moves the ebx/ebp assignment: the span width as a named local with the
// two fields read x1 then x2, the same with the reads in the order the original
// emits them, the width written out at both divisions, and the subtraction
// reversed with a negation at the division. All give 55.8%, and reversing the
// subtraction is much worse at 34.0%. The two register choices above are the
// whole remainder.
//
// What did move it from 48.2 to 55.8 percent:
// - `int ci = color & 0xff;` hoisted above the `if (d != 0)` test. That one
//   extra early use flips MSVC's ebx/ebp preference for the rest of the
//   function: the pitch clip then loads surf into ebp and span->x2 into ebx,
//   as the original does.
// - `p += row * surf->pitch; p += start;` as two statements instead of
//   `p += row * surf->pitch + start;`, and `while (count--)` in both loops
//   instead of `do { } while (--count)`.
//
// Inert (all of them compile to identical code, 55.8 percent): a `w` temp for
// the divisor in either form, `start`/`count` declared in any order,
// `count = span->x2 - span->x1` instead of `span->x2 - start`.

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
    int dz = (span->z2 - span->z1) / (span->x2 - span->x1);
    int ds = (span->s2 - span->s1) / (span->x2 - span->x1);
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
        int ci = color & 0xff;
        if (d != 0) {
            d += row * surf->pitch;
            d += start;
            while (count--) {
                unsigned char zi = z >> 16;
                if (*d <= zi) {
                    int si = s >> 16;
                    *p = app->shade[(si << 8) + ci];
                    *d = zi;
                }
                p++;
                z += dz;
                d++;
                s += ds;
            }
        } else {
            while (count--) {
                int si = s >> 16;
                *p++ = app->shade[(si << 8) + ci];
                s += ds;
            }
        }
    }
}
