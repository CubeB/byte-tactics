// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Sibling of 0x4c0a90 (which plots the two end points). Fills the pixels
// between the span ends: walks the depth ramp at +0x18 and the shade ramp at
// +0x20 (both 16.16) one step per pixel, does the depth test against the
// surface's depth buffer when it exists, and looks the destination colour up
// in the 32 x 256 shaded palette table at app+0xc4.
//
// PARTIAL, 48.2 percent. The whole shape is established (prologue, the two
// divisions by the same span width, the clip of x1 to 0 and of x2 to
// pitch-1, the two do/while loops, the byte depth test, the table index) but
// one register allocation will not budge:
//
//   original: mov ebx,[ecx+4] / mov ebp,[ecx] / sub ebx,ebp / idiv ebx
//   ours:     mov ebp,[ecx+4] / mov ebx,[ecx] / sub ebp,ebx / idiv ebp
//
// i.e. MSVC hands the divisor result to ebp and the long lived span->x1 to
// ebx here, while the original does the opposite. This is the same ebp/ebx
// swap reported in the comments of 0x4bf4d0 and 0x4c0a90, so it is a
// property of this rasteriser family. Everything downstream that mentions
// ebp or ebx follows from it. Also, our frame is 0x10 (four dwords) instead
// of 0xc: the compiler spills the shade slope `ds` to [esp+0x1c] because the
// masked loop uses eax for the shade index, where the original keeps ds in
// eax and spills the loop count to the arg slot instead.
//
// Tried and did not move it: span->x1/span->x2 as explicit locals in either
// declaration order, a `w = x2; w -= x1;` divisor temp, an inlined width
// helper, `si` split out of the shift, `t = app->shade` in the masked draw,
// unsigned char vs int colour, char* vs unsigned char* pixel pointers,
// for-loops instead of do/while, and every combination of the seven common
// headers plus one C++ header (tools/headers.py --cpp, 768 sets).
//
// A non-equivalent variant that only writes `span->x1 = 0;` before the
// `span->s1 -= ds * span->x1;` edit scores 56.4 percent because the compiler
// folds the s1 edit away, matching the original's instruction count in the
// clip block. It is wrong (the original subtracts ds times the old, negative
// span->x1), so it is not kept here.

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
        p += row * surf->pitch + start;
        if (d != 0) {
            d += row * surf->pitch + start;
            do {
                unsigned char zi = z >> 16;
                if (*d <= zi) {
                    int si = s >> 16;
                    *p = app->shade[(si << 8) + (color & 0xff)];
                    *d = zi;
                }
                p++;
                z += dz;
                d++;
                s += ds;
            } while (--count);
        } else {
            int ci = color & 0xff;
            do {
                int si = s >> 16;
                *p++ = app->shade[(si << 8) + ci];
                s += ds;
            } while (--count);
        }
    }
}
