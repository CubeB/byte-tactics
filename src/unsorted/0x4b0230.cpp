// Decompiled by space-bunny-free. Names are provisional.
// Draws a list box's frame. FUN_004a15c0 gives the entry's rectangle; when no
// bitmap arrives, the "Listbox" piece is looked up in the object's GAF and, if
// found, the rectangle is grown by 3 on every side. The destination is the
// surface at entries+0xbc. When FUN_004a18c0 finds a background cell for the
// entry, the area is tiled with it through FUN_004c6b70 and only the bevel
// (FUN_004b0160) is drawn; with no cell and no bitmap the rectangle is filled
// (FUN_004bf6f0) and bevelled; with a bitmap set of more than one child the
// set is laid out as a 3x3 border around the rectangle, rows 0/3/6 and columns
// 0/1/2 of the set, stepping by the first child's width and height, the last
// row and column pinned to the far edges; with one child or less it is blitted
// at the origin. The colours are the object's bytes at +0x8b2 (dark), +0x8c3
// (light) and +0x8c6 (fill).
//
// NOT MATCHED (check.py 46.2%, 625 of 631 bytes).  What is still different:
// 1. The prologue's register assignment, and every later block follows it. The
//    original gives obj ESI, the entries pointer EDI, bmp EBX (loaded after the
//    first call) and keeps `index` in the argument home, reloading it for
//    FUN_004a18c0; this file promotes `index` into a callee-saved register, so
//    it takes EBX, demotes obj to EBP, and puts bmp in ESI, which rotates the
//    three colour temporaries, the tile loop and the 3x3 block with it. The
//    shapes of those blocks are otherwise right.
// 2. The five local slots below the rect.  This file gets row 0x10, x0 0x14,
//    h 0x18, ypos 0x1c, height 0x20; the original has row 0x10, x0 0x14,
//    ypos 0x18, height 0x1c, h 0x20, so only the last three are permuted.  In
//    the original the slots ascend in the order the code touches them (row and
//    x0 and ypos in the inner loop, then height and h in the loop latch, which
//    loads height before h; this file loads h before height there).  The two
//    dead argument homes (arg1 then arg2) hold y0 then the y counter: giving
//    the y counter its own local is what puts it in arg2's slot.
// 3. The row index `(y >= height - h + 1) ? 6 : 3` compiles here to
//    setl/dec/and 3/add 3 (the same value, opposite polarity).  The original
//    emits setge/dec/and 0xfffffffd/add 6.  Neither the ?: nor a nested
//    if/else, nor a chained else-if, nor the two-ternary spelling produced it.
// 4. At the top of the 3x3 block the original pushes both arguments of
//    FUN_004b7f30 before the branch to the single-child case, this file pushes
//    one before and one after.
// Big win worth keeping: holding `obj->holder->entries` in a local
// `char* entries` (and using it for both FUN_004a15c0 and FUN_004a18c0) is
// worth 20 points: it makes entries a value that must live across the first
// call, which gives it EDI and moves &rect from EAX to ECX as the original has
// it.  Rejected: spelling the x position as an accumulating `x0 += x` (with the
// call taking x0) reaches 52.5% of the older baseline and its inner loop is
// the original's plus one store, but the original reloads x0 from its slot in
// every iteration and adds x (the only stores to that slot are in the set-up
// block), so the accumulating form draws the last column in the wrong place.

struct Rect_004b0230 {
    int x0;                          // +0x0
    int y0;                          // +0x4
    int x1;                          // +0x8
    int y1;                          // +0xc
};

struct Pic_004b0230 {
    unsigned short width;            // +0x0
    unsigned short height;           // +0x2
    char unknown_4[0x28 - 0x4];
};

struct Bits_004b0230 {
    unsigned short count;            // +0x0
    char unknown_2[0x28 - 0x2];
    Pic_004b0230* child[1];          // +0x28
};

struct Gaf_004b0230 {
    char unknown_0[0x14];
};

struct Cell_004b0230 {
    int step_x;                      // +0x0
    int step_y;                      // +0x4
};

struct Surface_004b0230 {
    int tiles_x;                     // +0x0
    int tiles_y;                     // +0x4
    char unknown_8[0xbc - 0x8];
};

struct Holder_004b0230 {
    char unknown_0[4];
    char* entries;                   // +0x4
};

struct Object_004b0230 {
    char unknown_0[4];
    Gaf_004b0230* gaf;               // +0x4
    char unknown_8[0x18 - 0x8];
    Holder_004b0230* holder;         // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char dark;              // +0x8b2
    char unknown_8b3[0x8c3 - 0x8b3];
    unsigned char light;             // +0x8c3
    char unknown_8c4[2];
    unsigned char fill;              // +0x8c6
};

void __stdcall FUN_004a15c0(char* entries, int index, Rect_004b0230* out);
int __stdcall FUN_004a18c0(char* entries, int index);
Bits_004b0230* __stdcall FUN_004b8d40(Gaf_004b0230* gaf, const char* name);
void __stdcall FUN_004c6b70(Surface_004b0230* dst, Cell_004b0230* cell, int x, int y);
Pic_004b0230* __stdcall FUN_004b7f30(Bits_004b0230* bits, int index);
void __stdcall FUN_004b7f90(Surface_004b0230* dst, Pic_004b0230* pic, int x, int y);
void __stdcall FUN_004b0160(Surface_004b0230* surface, Rect_004b0230* rect, int dark, int light, int fill);
int __stdcall FUN_004bf6f0(Surface_004b0230* surface, Rect_004b0230* rect, int colour);

// FUNCTION: 0x4b0230
void __stdcall FUN_004b0230(Object_004b0230* obj, int index, Bits_004b0230* bmp)
{
    Rect_004b0230 rect;
    char* entries = obj->holder->entries;
    FUN_004a15c0(entries, index, &rect);

    if (bmp == 0) {
        if (obj->gaf == 0)
            return;
        bmp = FUN_004b8d40(obj->gaf, "Listbox");
        if (bmp == 0)
            return;
        rect.x0 -= 3;
        rect.y0 -= 3;
        rect.x1 += 3;
        rect.y1 += 3;
    }

    Surface_004b0230* surface = *(Surface_004b0230**)((char*)entries + 0xbc);
    Cell_004b0230* cell = (Cell_004b0230*)FUN_004a18c0(entries, index);

    if (cell != 0) {
        for (int x = 0; x < surface->tiles_x; x += cell->step_x) {
            for (int y = 0; y < surface->tiles_y; y += cell->step_y) {
                FUN_004c6b70(surface, cell, x, y);
            }
        }
        FUN_004b0160(surface, &rect, obj->dark, obj->light, obj->fill);
    } else if (bmp == 0) {
        FUN_004bf6f0(surface, &rect, obj->fill);
        FUN_004b0160(surface, &rect, obj->dark, obj->light, obj->fill);
    } else if (bmp->count > 1) {
        Pic_004b0230* sub = FUN_004b7f30(bmp, 0);
        int w = sub->width;
        int h = sub->height;
        int height;
        int ypos;
        int x0;
        int y;
        int row;
        int y0;
        int width;
        if (index != 0) {
            y0 = rect.y0;
            x0 = rect.x0;
        } else {
            y0 = 0;
            x0 = 0;
        }
        height = rect.y1 - rect.y0 + 1;
        width = rect.x1 - rect.x0 + 1;
        y = 0;
        while (y < height) {
            if (y != 0)
                row = (y >= height - h + 1) ? 6 : 3;
            else
                row = 0;
            if (y + h > height)
                y = height - h;
            ypos = y0 + y;
            for (int x = 0; x < width; x += w) {
                int col;
                if (x + w >= width) {
                    x = width - w;
                    col = 2;
                } else {
                    col = (x != 0) ? 1 : 0;
                }
                Pic_004b0230* tile = FUN_004b7f30(bmp, row + col);
                FUN_004b7f90(surface, tile, x0 + x, ypos);
            }
            y += h;
        }
    } else {
        FUN_004b7f90(surface, FUN_004b7f30(bmp, 0), 0, 0);
    }
}
