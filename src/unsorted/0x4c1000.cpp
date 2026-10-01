// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, edited by deepseek-v4.1. Names are
// provisional. PARTIAL 76.1%, 795 of 791 bytes. (deepseek-v4.1: computing lasty before the
// pitch/ymax guards beats 76.0%; the ymin<->out (0x18/0x20) and imin<->lasty (0x2c/0x34) slot swaps
// and the surf-in-eax register pick survive every guard-block reorder tried.) Share y0/y1 between the two edge walks alongside
// the previously shared pointer/index/dx locals. This improves the earlier 73.3%, 809-byte version.
// Remaining differences include clipping register choices, ymin/out and imin/lasty stack-slot swaps
// and scheduling. Partial temporary-sharing combinations, all 24 bound declaration orders, 768
// header sets and surface getters did not improve this version. GPT-6.1-sol refinement variants kept 76.0%; reversing the pitch-bound comparison scored 74.3%. The best source is restored, with ymin/out and imin/lasty allocation shifts remaining.
// deepseek-v4.1-flash pass: the sibling 0x4c0c70 shape (bound order maxX,minY,maxY,minX with
// block-scoped edge walks) scores 74.5%. Permuting the declaration order of imin/imax/i/out/j/k/a/b/dxdy
// (reverse, out-first, dxdy-first, imin-last, i-last) leaves the compiled bytes byte-identical, so MSVC
// 5 assigns these stack slots from code shape, not declaration order. The two remaining mismatches are
// exactly the pairwise slots ymin<->out (0x18/0x20) and imin<->lasty (0x2c/0x34).
// deepseek-v4.1-flash retry (#3217), all inert at 76.1%: a separate scanIndex local (the 0x4c0c70
// shape), declaring lasty with the other locals (it was left out of the earlier declaration-order
// sweep) and assigning it in place, and moving the lasty computation down to its own guard
// (76.0%, 797 bytes, fewer diff lines but no slot flip). The surf register pick is ecx here, as in
// the original; the residual is the slot pair swap plus the merged pitch/height load the hoist needs.

// deepseek-v4.1-flash retry (#3585), still 76.1%: un-hoisting lasty back into the third guard
// restores the original guard structure (surf load, xor+16-bit load, dec, cmp, store between cmp
// and jle) but moves surf to eax and adds a `mov edx,eax`, so it is 797 bytes, 76.0%: worse.
// A dedicated probe (extern sink(&local), 2 to 5 address-taken locals) shows MSVC 5 frame offsets
// are NOT declaration-ordered even when every local is address-taken: reversing the declarations
// leaves the whole image byte-identical, so the offsets follow the pcode use order, not the
// declarations. The ymin/out and imin/lasty slot pair swaps are therefore a source-shape lever,
// not a declaration-order one.

// deepseek-v4.1-flash retry (#3712), still 76.1%: making dxdy block-local in both edge walks
// (removing the function-scope slot) compiles to 795 bytes at 72.9%, so the 4-byte frame excess is
// not that slot; initialising out at its declaration (`Span_004c1000* out = spans;`) and deleting the
// post-guard store is 795 bytes at 72.5%, so the ymin/out slot pair is not first-definition order
// either. Both reverted.

struct Surface_004c1000 {
    unsigned short pitch;   // +0x0, also the clip width
    unsigned short field_2; // +0x2, the clip height
    char unknown_4[0x10 - 0x4];
    unsigned char* bits;  // +0x10
    unsigned char* depth; // +0x14
};

struct Vertex_004c1000 {
    int x; // +0x0
    int y; // +0x4
    int z; // +0x8
};

struct Span_004c1000 {
    int x1; // +0x0
    int x2; // +0x4
    char unknown_8[0x18 - 0x8];
    int z1; // +0x18
    int z2; // +0x1c
    char unknown_20[0x28 - 0x20];
};

void __stdcall FUN_004c06e0(int row, Span_004c1000* span, Surface_004c1000* surf, int color);

// FUNCTION: 0x4c1000
int __stdcall FUN_004c1000(Surface_004c1000* surf, Vertex_004c1000* verts, int count, int color) {
    int y0, y1;
    Span_004c1000 spans[2048];
    int ymin = 999999;
    int ymax = -999999;
    int xmin = 999999;
    int xmax = -999999;
    int imin;
    int imax;
    int i;
    Span_004c1000* out;
    int j;
    int k;
    Vertex_004c1000* a;
    Vertex_004c1000* b;
    int dxdy;
    for (i = 0; i < count; i++) {
        int y = verts[i].y;
        if (y < ymin) {
            ymin = y;
            imin = i;
        }
        if (y > ymax) {
            ymax = y;
            imax = i;
        }
        int x = verts[i].x;
        if (x > xmax)
            xmax = x;
        if (x < xmin)
            xmin = x;
    }
    int lasty = (int)surf->field_2 - 1;
    if (xmin > (int)surf->pitch - 1) {
        return 0;
    }
    if (ymax < 0) {
        return 0;
    }
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
    out = spans;
    i = imin;
    for (;;) {
        j = i - 1;
        k = j;
        if (k < 0) {
            k = count - 1;
        }
        a = &verts[i];
        b = &verts[k];
        y0 = a->y;
        y1 = b->y;
        if (y0 < y1) {
            int dy = y1 - y0;
            int x = a->x;
            dxdy = ((b->x - x) << 16) / dy;
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
    out = spans;
    i = imin;
    for (;;) {
        j = i + 1;
        k = j;
        if (k >= count) {
            k = 0;
        }
        a = &verts[i];
        b = &verts[k];
        y0 = a->y;
        y1 = b->y;
        if (y0 < y1) {
            int dy = y1 - y0;
            int x = a->x;
            dxdy = ((b->x - x) << 16) / dy;
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
    out = spans;
    for (i = ymin; i < ymax; i++) {
        if (out->x2 - out->x1 > 0) {
            FUN_004c06e0(i, out, surf, color);
        }
        out++;
    }
    return 1;
}
