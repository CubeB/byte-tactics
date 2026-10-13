// Decompiled by deepseek-v4.1-flash, space-bunny-free, Sonnet 5.5, GPT-6.1-sol, opus and Opus. Names are provisional.

#include "../graphics/rect.h"
#include "../graphics/gaf_frame.h"

struct Bits_004b0230 {
    unsigned short count;            // +0x0
    char unknown_2[0x28 - 0x2];
    GafFrame* child[1];              // +0x28
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

int __stdcall DrawLine(void* surface, int x0, int y0, int x1, int y1, unsigned char color);
void __stdcall FillRectangle(void* surface, void* rect, int color);
void __stdcall GetGadgetRectByIndex(char* entries, int index, Rect* out);
int __stdcall FindBackgroundCell(char* entries, int index);
Bits_004b0230* __stdcall FindGafEntry(Gaf_004b0230* gaf, const char* name);
void __stdcall DrawSurface(Surface_004b0230* dst, Cell_004b0230* cell, int x, int y);
GafFrame* __stdcall GetGafFrame(Bits_004b0230* bits, int index);
void __stdcall DrawFrame(Surface_004b0230* dst, GafFrame* pic, int x, int y);

// Draws a double (two-line) bevelled border: the top and left pairs of edges
// use the third argument's colour, the right and bottom pairs the fourth's.
// FUNCTION: 0x4b0090
void __stdcall DrawBevelBorder(void* surface, Rect* rect, int light,
                            unsigned char dark, int unused)
{
    DrawLine(surface, rect->left, rect->top, rect->right, rect->top, light);
    DrawLine(surface, rect->left, rect->top + 1, rect->right - 1, rect->top + 1, light);
    DrawLine(surface, rect->left, rect->top, rect->left, rect->bottom, light);
    DrawLine(surface, rect->left + 1, rect->top, rect->left + 1, rect->bottom - 1, light);
    DrawLine(surface, rect->right, rect->top + 1, rect->right, rect->bottom, dark);
    DrawLine(surface, rect->right - 1, rect->top + 2, rect->right - 1, rect->bottom, dark);
    DrawLine(surface, rect->left + 1, rect->bottom, rect->right, rect->bottom, dark);
    DrawLine(surface, rect->left + 2, rect->bottom - 1, rect->right, rect->bottom - 1, dark);
}

// Draws a two-pixel bevelled frame around a rectangle: the top and left edges
// use `light`, the right and bottom edges use `dark`. The fifth argument is
// not read by the original.
// FUNCTION: 0x4b0160
void __stdcall DrawBevelBorderDarkFirst(void* surface, Rect* rect, int dark, int light, int unused)
{
    DrawLine(surface, rect->left, rect->top, rect->right, rect->top, light);
    DrawLine(surface, rect->left, rect->top + 1, rect->right - 1, rect->top + 1, light);
    DrawLine(surface, rect->left, rect->top, rect->left, rect->bottom, light);
    DrawLine(surface, rect->left + 1, rect->top, rect->left + 1, rect->bottom - 1, light);
    DrawLine(surface, rect->right, rect->top + 1, rect->right, rect->bottom, dark);
    DrawLine(surface, rect->right - 1, rect->top + 2, rect->right - 1, rect->bottom, dark);
    DrawLine(surface, rect->left + 1, rect->bottom, rect->right, rect->bottom, dark);
    DrawLine(surface, rect->left + 2, rect->bottom - 1, rect->right, rect->bottom - 1, dark);
}

// Draws a list box's frame. GetGadgetRectByIndex gives the entry's rectangle; when no
// bitmap arrives, the "Listbox" piece is looked up in the object's GAF and, if
// found, the rectangle is grown by 3 on every side. The destination is the
// surface at entries+0xbc. When FindBackgroundCell finds a background cell for the
// entry, the area is tiled with it through DrawSurface and only the bevel
// (DrawBevelBorderDarkFirst) is drawn; with no cell and no bitmap the rectangle is filled
// (FillRectangle) and bevelled; with a bitmap set of more than one child the
// set is laid out as a 3x3 border around the rectangle, rows 0/3/6 and columns
// 0/1/2 of the set, stepping by the first child's width and height, the last
// row and column pinned to the far edges; with one child or less it is blitted
// at the origin. The colours are the object's bytes at +0x8b2 (dark), +0x8c3
// (light) and +0x8c6 (fill).
// FUNCTION: 0x4b0230
void __stdcall DrawListboxFrame(Object_004b0230* obj, int index, Bits_004b0230* bmp)
{
    char* entries;
    int col;
    Surface_004b0230* surface;
    int height;
    int x;
    int width;
    int x0;
    Rect rect;
    GafFrame* tile;
    int w;
    // Own local, not the reused index parameter: orders the loop-latch reloads.
    int row;
    int y;
    int yoff;
    GafFrame* p;
    int y0;
    Cell_004b0230* cell;
    int h;
    GafFrame* sub;
    entries = obj->holder->entries;
    GetGadgetRectByIndex(entries, index, &rect);

    if (bmp == 0) {
        if (obj->gaf == 0)
            return;
        bmp = FindGafEntry(obj->gaf, "Listbox");
        if (bmp == 0)
            return;
        rect.left -= 3;
        rect.top -= 3;
        rect.right += 3;
        rect.bottom += 3;
    }

    surface = *(Surface_004b0230**)(obj->holder->entries + 0xbc);
    cell = (Cell_004b0230*)FindBackgroundCell(entries, index);

    if (cell != 0) {
        for (x = 0; x < surface->tiles_x; x += cell->step_x) {
            for (y = 0; y < surface->tiles_y; y += cell->step_y) {
                DrawSurface(surface, cell, x, y);
            }
        }
        DrawBevelBorderDarkFirst(surface, &rect, obj->dark, obj->light, obj->fill);
    } else if (bmp != 0) {
        if (bmp->count > 1) {
            sub = GetGafFrame(bmp, 0);
            w = sub->width;
            h = sub->height;
            if (index != 0) {
                y0 = rect.top;
                x0 = rect.left;
            } else {
                y0 = 0;
                x0 = 0;
            }
            // Extents follow the x0/y0 branch, height first, then yoff = 0;
            // the yoff == 0 arm of the row choice comes first.
            height = rect.bottom - rect.top + 1;
            width = rect.right - rect.left + 1;
            yoff = 0;
            while (yoff < height) {
                if (yoff == 0)
                    row = 0;
                else
                    row = (yoff < height - h + 1) ? 3 : 6;
                if (yoff + h > height)
                    yoff = height - h;
                for (x = 0; x < width; x += w) {
                    if (x + w >= width) {
                        x = width - w;
                        col = 2;
                    } else {
                        col = (x != 0) ? 1 : 0;
                    }
                    tile = GetGafFrame(bmp, row + col);
                    // y0 + yoff stays in the call, not a local: it is a hoisted loop invariant.
                    DrawFrame(surface, tile, x0 + x, y0 + yoff);
                }
                yoff += h;
            }
        } else {
            p = GetGafFrame(bmp, 0);
            DrawFrame(surface, p, 0, 0);
        }
    } else {
        FillRectangle(surface, &rect, obj->fill);
        DrawBevelBorderDarkFirst(surface, &rect, obj->dark, obj->light, obj->fill);
    }
}

// Fills a rectangle, then draws its bevelled border through DrawBevelBorder
// (compare 0x4b0510 and 0x4b0590).
// FUNCTION: 0x4b04b0
void __stdcall FillBevelBox(void* surface, Rect* rect, int light, int dark, int fill)
{
    FillRectangle(surface, rect, fill);
    DrawBevelBorder(surface, rect, light, dark, fill);
}

// Fills a rectangle, then draws its border through DrawBevelBorderDarkFirst (same shape
// as 0x4b04b0, which uses DrawBevelBorder).
// FUNCTION: 0x4b04e0
void __stdcall FillBevelBoxDarkFirst(void* surface, Rect* rect, int light, int dark, int fill)
{
    FillRectangle(surface, rect, fill);
    DrawBevelBorderDarkFirst(surface, rect, light, dark, fill);
}

// Fills a rectangle and draws a bevelled border: top and left edges in one
// colour, right and bottom edges in another.
// FUNCTION: 0x4b0510
void __stdcall DrawRaisedBox(void* surface, Rect* rect, int light, unsigned char dark, int fill)
{
    FillRectangle(surface, rect, fill);
    DrawLine(surface, rect->left, rect->top, rect->right, rect->top, light);
    DrawLine(surface, rect->left, rect->top, rect->left, rect->bottom, light);
    DrawLine(surface, rect->right, rect->top + 1, rect->right, rect->bottom, dark);
    DrawLine(surface, rect->left + 1, rect->bottom, rect->right, rect->bottom, dark);
}

// Sunken counterpart of DrawRaisedBox: fills a rectangle and draws its top and
// left edges in the fourth argument's colour, the right and bottom edges in
// the third's.
// FUNCTION: 0x4b0590
void __stdcall DrawSunkenBox(void* surface, Rect* rect, int light, int dark, int fill)
{
    FillRectangle(surface, rect, fill);
    DrawLine(surface, rect->left, rect->top, rect->right, rect->top, dark);
    DrawLine(surface, rect->left, rect->top, rect->left, rect->bottom, dark);
    DrawLine(surface, rect->right, rect->top + 1, rect->right, rect->bottom, light);
    DrawLine(surface, rect->left + 1, rect->bottom, rect->right, rect->bottom, light);
}
