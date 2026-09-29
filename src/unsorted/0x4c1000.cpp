// Decompiled by space-bunny-free. Names are provisional.
// Screen-space scan conversion of one convex piece: it bounds the vertex list,
// then walks the row range twice, once interpolating each edge (i, i-1) walking
// y upwards into span.x1/span.z1 and once each edge (i, i+1) into span.x2/
// span.z2, then calls FUN_004c06e0 once per row to plot the span.
// Still differs (56.4%, 809 bytes against 791). The whole instruction
// sequence of the scan, of both interpolation loops and of the row loop is
// present and in the original's order; what is left is allocation:
//  - the frame slots for the four accumulators are permuted. The original
//    has ymax at esp+0x10, xmin at +0x14, ymin at +0x18, imin at +0x2c, imax
//    at +0x30 and lasty at +0x34, so ymin, ymax and xmin own the three lowest
//    slots, the four loop temporaries take +0x1c..+0x28 and the two index
//    variables come last. Declaring them in any other order shifts which of
//    ymax/xmin lands on +0x10 (ymax,xmin,ymin and ymax,ymin,xmin both give
//    56.4%; xmin,ymax,ymin and ymin,ymax,xmin give 55.7% because MSVC then
//    materialises 999999 into ebp before copying it to esi).
//  - in the interpolation body the original gives the row y to edi and the
//    difference y2-y to esi, this one gives y to esi and the difference to
//    edi, which moves the whole body's temporaries with it. Hoisting
//    `int y = verts[i].y` above `int j = i - 1` to change which of the two is
//    defined first scores 36.3%, so the original's order really is j first.
//  - writing the wrap as `if (k < 0) k = count - 1;` instead of the ternary
//    scores 53.8% against 55.7% for the ternary, so the original does use the
//    conditional expression; what is left of the difference there is a `jmp`
//    in the join block and `dec` against the original's `lea reg,[edx-1]`.
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
            int k = j < 0 ? count - 1 : j;
            Vertex_004c1000* a = &verts[i];
            Vertex_004c1000* b = &verts[k];
            int y = a->y;
            int y2 = b->y;
            if (y < y2) {
                int dxdy = ((b->x - a->x) << 16) / (y2 - y);
                int x = (a->x << 16) + 0xffff;
                int z = a->z << 16;
                int dzdy = ((b->z << 16) - z) / (y2 - y);
                if (y < 0) {
                    x -= dxdy * y;
                    z -= dzdy * y;
                    y = 0;
                }
                if (y2 > lasty) {
                    y2 = lasty;
                }
                if (y < y2) {
                    int n = y2 - y;
                    do {
                        out->x1 = x >> 16;
                        out->z1 = z;
                        x += dxdy;
                        z += dzdy;
                        out++;
                    } while (--n);
                }
            }
            i = j < 0 ? count - 1 : j;
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
            if (j >= count) {
                j = 0;
            }
            Vertex_004c1000* a = &verts[i];
            Vertex_004c1000* b = &verts[j];
            int y = a->y;
            int y2 = b->y;
            if (y < y2) {
                int dxdy = ((b->x - a->x) << 16) / (y2 - y);
                int x = (a->x << 16) + 0xffff;
                int z = a->z << 16;
                int dzdy = ((b->z << 16) - z) / (y2 - y);
                if (y < 0) {
                    x -= dxdy * y;
                    z -= dzdy * y;
                    y = 0;
                }
                if (y2 > lasty) {
                    y2 = lasty;
                }
                if (y < y2) {
                    int n = y2 - y;
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
