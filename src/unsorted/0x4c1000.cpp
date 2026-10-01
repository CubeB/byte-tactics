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

// deepseek-v4.1-flash (#3807), still 86.3 percent / 799 of 791 bytes: deleting the lasty local
// (using (int)surf->field_2 - 1 at all four uses) and swapping walk2 to `i = imin;` before
// `out = spans;` are both byte-flat at 86.3 percent / 799 bytes, so neither the (ymin, out) nor the
// (imin, lasty) slot pair is reachable that way; dropping the `k = j;` copy in walk2 (wrap j itself)
// regresses to 81.3 percent / 807 bytes. The residual is the two slot pairs, the second walk's
// extra `mov ecx, esi`, and the two `test` before the `mov` orderings.
// Also tried here: walk2 computing k = i + 1 before j = k drops the walk2 `mov ecx, esi` and compiles
// to the original 791 bytes, but it shifts ymin to [esp+0x1c], flips the imin/lasty pair the other
// way and scores 82.3 percent, so the byte count and the slot pairs cannot be won together that way.
// The 86.3 percent shape stays.

// deepseek-v4.1-flash (#4031), 86.3 -> 91.4 percent, 799 of 791 bytes: the walk bodies are now
// wrapped in real C++ blocks ({ }) with the first-point pointer `a` and the second-point pointer
// `b` declared block-local inside each walk (j, k, i, out stay function-scope). That one scope
// change restores every stack slot the earlier sessions could not move: ymin 0x18, out 0x20,
// imin 0x2c, lasty 0x34, a 0x28/0x24, and the walk jge/rel32 size shrinks back to the original
// 791-byte shape except for the two extra movs below. Declaring a/b at function scope instead
// (the old shape) is 86.3; a or b block-local alone is worse; a 12-bit scope sweep over
// {out,i,j,k,a,b}x{block,function} for each walk peaks at exactly this shape.
// Still different (four hunks, all one allocator state):
//  1. the freed xmin slot 0x14 goes to the raw index j in ours and to the b pointer in the
//     original, so j sits at 0x14/0x14 and b at 0x24/0x28 instead of b 0x14/0x14 and
//     j 0x24/0x28;
//  2. walk1 head: ours tests j and copies to k after the spill store, the original copies
//     (mov ecx,esi) first, stores, then tests the copy;
//  3. walk1 tail: ours tests j then copies to i, the original copies to i and tests i;
//  4. walk2 keeps j and k apart (extra mov ecx,esi / mov eax,esi, 4 bytes) where the original
//     wraps one register in place, and the 4 extra bytes push walk2's jge from rel8 to rel32
//     (4 more), hence 799 vs 791.
// Tried and inert at 91.4: declaration order of j/k/a/b, a/b block vs function scope once the
// blocks exist, i/out block-local, do/while for both walks, hoisting the inner if locals,
// `k = j = i - 1`, an empty else arm, swapping the a/b assignment order, 60 unused extern ints,
// the a1/a2/a7 ternaries, `if (k <= -1)` (88.1), b function-scope with a block-local (86.7),
// j block-local in either walk (84.5 or less; the outer slots shift to ymin 0x24, out 0x1c,
// imin 0x18, lasty 0x2c), walk2 with a single index variable (80.6-84.2, 807 bytes, its wrap
// goes through memory), and reversed j/k naming (72.5, outer slots shift).
// Next lead: the allocator hands 0x14 to whichever variable it sees first at the walk head; the
// original gives it to b. Make b's live range start before the walk head (assign b at the head
// before j, or keep the previous iteration's b) with j block-local and a compensating block-local
// so the outer slots stay.

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

// deepseek-v4.1-flash (#3755): 76.1 -> 86.3 percent (799 bytes). Applied the 0x4c0c70 fix:
// test the surface height inline in the ymin guard and assign lasty from the same expression
// afterwards, so the pitch and height 16-bit loads stop merging (pitch guard, ymax guard,
// height load + dec + cmp + store). Remaining diffs: the ymin<->out (0x18/0x20) and
// imin<->lasty (0x2c/0x34) slot pairs, the ymin = 0 store slot, and the test-register pick
// at the walk setup (test esi vs test ecx).
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
    if (xmin > (int)surf->pitch - 1) {
        return 0;
    }
    if (ymax < 0) {
        return 0;
    }
    if (ymin > (int)surf->field_2 - 1) {
        return 0;
    }
    int lasty = (int)surf->field_2 - 1;
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
        out = spans;
        i = imin;
        for (;;) {
            j = i - 1;
            k = j;
            if (k < 0) {
                k = count - 1;
            }
            Vertex_004c1000* a = &verts[i];
            Vertex_004c1000* b = &verts[k];
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
    }
    {
        out = spans;
        i = imin;
        for (;;) {
            j = i + 1;
            k = j;
            if (k >= count) {
                k = 0;
            }
            Vertex_004c1000* a = &verts[i];
            Vertex_004c1000* b = &verts[k];
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
