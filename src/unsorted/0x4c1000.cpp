// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Screen-space scan conversion of one convex piece: it bounds the vertex list,
// then walks the row range twice, once interpolating each edge (i, i-1) walking
// y upwards into span.x1/span.z1 and once each edge (i, i+1) into span.x2/
// span.z2, then calls FUN_004c06e0 once per row to plot the span.
//
// Partial, 71.0% (797 bytes against 791). The body of both interpolation loops
// is now the sibling 0x4c0c70's matched idiom: hoist `int dy = y1 - y0` first,
// take the start x into a local `x`, compute dx with `((b->x - x) << 16) / dy`,
// then `x = (x << 16) + 0xffff`, then z and dz. That alone was worth 14 points
// (57.3% to 71.0%) and made the loop bodies byte-identical.
//
// What is left is stack-slot allocation only; every remaining diff outside the
// prologue is a mismatch of these offsets, not of code. The original has ymax
// at esp+0x10, xmin at +0x14, ymin at +0x18, the dxdy spill at +0x1c, `out` at
// +0x20, the j/a spills at +0x24/+0x28, imin at +0x2c, imax at +0x30 and lasty
// at +0x34. Ours lands xmin at +0x10, ymax at +0x14, out at +0x18, imin at
// +0x1c, ymin at +0x20, the j/a spills at +0x24/+0x28/+0x34, lasty at +0x2c and
// imax at +0x30, so the two lowest view words are swapped and imin/lasty sit
// one slot low, which drags most of the spill slots with them.
// Tried with no effect: all 24 declaration permutations of ymin/ymax/xmin/xmax,
// moving `out` to function scope, and tools/headers.py (128 sets, none helps).
// The loop2 ternary `int k = j < count ? j : 0;` is much worse (38.5%).
// The two view words must be unsigned: the original zeroes eax and does a
// 16-bit load for both the clip width and the clip height.

struct Surface_004c1000 {
    unsigned short pitch;              // +0x0, also the clip width
    unsigned short field_2;            // +0x2, the clip height
    char unknown_4[0x10 - 0x4];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
};

struct Vertex_004c1000 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

struct Span_004c1000 {
    int x1;                            // +0x0
    int x2;                            // +0x4
    char unknown_8[0x18 - 0x8];
    int z1;                            // +0x18
    int z2;                            // +0x1c
    char unknown_20[0x28 - 0x20];
};

void __stdcall FUN_004c06e0(int row, Span_004c1000* span, Surface_004c1000* surf, int color);

// FUNCTION: 0x4c1000
int __stdcall FUN_004c1000(Surface_004c1000* surf, Vertex_004c1000* verts, int count, int color)
{
    Span_004c1000 spans[2048];
    int ymin = 999999;
    int ymax = -999999;
    int xmin = 999999;
    int xmax = -999999;
    int imin;
    int imax;
    int i;
    for (i = 0; i < count; i++) {
        int y = verts[i].y;
        if (y < ymin) { ymin = y; imin = i; }
        if (y > ymax) { ymax = y; imax = i; }
        int x = verts[i].x;
        if (x > xmax) xmax = x;
        if (x < xmin) xmin = x;
    }
    if (xmin > (int)surf->pitch - 1) {
        return 0;
    }
    if (ymax < 0) {
        return 0;
    }
    int lasty = (int)surf->field_2 - 1;
    if (ymin > lasty) {
        return 0;
    }
    if (ymin < 0) {
        ymin = 0;
    }
    if (ymax > lasty) {
        ymax = lasty;
    }
    if (ymax == ymin) {
        return 0;
    }
    {
        Span_004c1000* out = spans;
        i = imin;
        for (;;) {
            int j = i - 1;
            int k = j;
            if (k < 0) {
                k = count - 1;
            }
            Vertex_004c1000* a = &verts[i];
            Vertex_004c1000* b = &verts[k];
            int y0 = a->y;
            int y1 = b->y;
            if (y0 < y1) {
                int dy = y1 - y0;
                int x = a->x;
                int dxdy = ((b->x - x) << 16) / dy;
                x = (x << 16) + 0xffff;
                int z = a->z << 16;
                int dzdy = ((b->z << 16) - z) / dy;
                if (y0 < 0) {
                    x -= dxdy * y0;
                    z -= dzdy * y0;
                    y0 = 0;
                }
                if (y1 > lasty) {
                    y1 = lasty;
                }
                if (y0 < y1) {
                    int n = y1 - y0;
                    do {
                        out->x1 = x >> 16;
                        out->z1 = z;
                        x += dxdy;
                        z += dzdy;
                        out++;
                    } while (--n);
                }
            }
            i = j;
            if (i < 0) {
                i = count - 1;
            }
            if (i == imax) {
                break;
            }
        }
    }
    {
        Span_004c1000* out = spans;
        i = imin;
        for (;;) {
            int j = i + 1;
            int k = j;
            if (k >= count) {
                k = 0;
            }
            Vertex_004c1000* a = &verts[i];
            Vertex_004c1000* b = &verts[k];
            int y0 = a->y;
            int y1 = b->y;
            if (y0 < y1) {
                int dy = y1 - y0;
                int x = a->x;
                int dxdy = ((b->x - x) << 16) / dy;
                x = (x << 16) + 0xffff;
                int z = a->z << 16;
                int dzdy = ((b->z << 16) - z) / dy;
                if (y0 < 0) {
                    x -= dxdy * y0;
                    z -= dzdy * y0;
                    y0 = 0;
                }
                if (y1 > lasty) {
                    y1 = lasty;
                }
                if (y0 < y1) {
                    int n = y1 - y0;
                    do {
                        out->x2 = x >> 16;
                        out->z2 = z;
                        x += dxdy;
                        z += dzdy;
                        out++;
                    } while (--n);
                }
            }
            i = j;
            if (i >= count) {
                i = 0;
            }
            if (i == imax) {
                break;
            }
        }
    }
    {
        Span_004c1000* out = spans;
        for (i = ymin; i < ymax; i++) {
            if (out->x2 - out->x1 > 0) {
                FUN_004c06e0(i, out, surf, color);
            }
            out++;
        }
    }
    return 1;
}
