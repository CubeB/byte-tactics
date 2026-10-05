// Decompiled by mimo-v2.6-pro. Names are provisional.
// Checkerboard eraser: clears destination pixels to 0 where a bitmap's
// pixels differ from its transparent colour, stepping by two so only one
// parity of the checkerboard is touched (parity comes from the last
// argument). Like 0x4b7f90 it draws a bitmap (or a list of nested
// bitmaps) into `dst`, or into the screen when `dst` is null: the screen
// is then locked with LockScreen and unlocked with UnlockScreen at the
// end (which is why the unlock is guarded by a comparison of `dst` with
// the address of the local surface). The whole body is skipped when the
// byte at +9 is set. Suspected original bug: that test dereferences the
// record (byte at +9) before the later `bmp != 0` check on the same
// pointer, so a null record would crash before the null check. A record
// with a child count draws each child through this function (recursion).
// A leaf builds the source rect (0, 0, w - 1, h - 1) and the destination
// rect (x - dx, y - dy, ...), clips the destination to the destination's
// own clip rect with ClipRects, then walks the clipped rows and
// columns. The source row index has to be a plain local in the row loop's
// initialiser (`sy`), not `other.top++`: that is what pins the rects'
// stack slots, the loop latch's instruction order and the spill of the
// row index into the dead `x` argument slot.
#include <windows.h>

struct Rect_004b88d0 {
    int left;
    int top;
    int right;
    int bottom;
};

class Surface {
public:
    char unknown_0[8];
    int field_8;                    // pitch
    int field_c;                    // bits
    char unknown_10[0x1c - 0x10];
    Rect_004b88d0 field_1c;         // +0x1c

    Rect_004b88d0* GetClipRect(Rect_004b88d0* out);
};

struct Bitmap_004b88d0 {
    unsigned short width;           // +0x0
    unsigned short height;          // +0x2
    short dx;                       // +0x4
    short dy;                       // +0x6
    unsigned char colour;           // +0x8
    unsigned char flag9;            // +0x9
    unsigned char count;            // +0xa
    unsigned char kind;             // +0xb
    int unknown_c;                  // +0xc
    void* field_10;                 // +0x10
};

struct Surface_004b88d0 {
    int data[12];
};

int __stdcall LockScreen(Surface_004b88d0* out);
int __stdcall UnlockScreen(Surface_004b88d0* s);
void __stdcall ClipRects(Rect_004b88d0* other, Rect_004b88d0* rect, Rect_004b88d0* bounds);

// FUNCTION: 0x4b88d0
void __stdcall EraseFrameDithered(Surface* dst, Bitmap_004b88d0* bmp, int x, int y, int parity)
{
    Surface_004b88d0 screen;
    if (bmp->flag9 == 0) {
        if (dst == 0) {
            int ok = LockScreen(&screen);
            if (ok != 0)
                dst = (Surface*)&screen;
        }
        if (bmp != 0) {
            if (bmp->count > 0) {
                for (int i = 0; i < (int)bmp->count; i++) {
                    Bitmap_004b88d0* e = ((Bitmap_004b88d0**)bmp->field_10)[i];
                    EraseFrameDithered(dst, e, x, y, parity);
                }
            } else {
                Rect_004b88d0 rect;
                Rect_004b88d0 other;
                Rect_004b88d0 bounds;
                other.left = 0;
                other.top = 0;
                other.right = bmp->width - 1;
                other.bottom = bmp->height - 1;
                rect.left = x - bmp->dx;
                rect.top = y - bmp->dy;
                rect.right = rect.left + bmp->width - 1;
                rect.bottom = rect.top + bmp->height - 1;
                dst->GetClipRect(&bounds);
                ClipRects(&other, &rect, &bounds);
                if (rect.right >= rect.left && rect.bottom >= rect.top &&
                    other.right >= other.left && other.bottom >= other.top) {
                    for (int yy = rect.top, sy = other.top; yy <= rect.bottom; yy++, sy++) {
                        unsigned char* s = (unsigned char*)bmp->field_10
                            + sy * bmp->width + other.left;
                        unsigned char* d = (unsigned char*)dst->field_c
                            + yy * dst->field_8 + rect.left;
                        unsigned char* end = d + rect.right - rect.left;
                        if (((yy + rect.left + parity) & 1) != 0) {
                            d++;
                            s++;
                        }
                        while (d <= end) {
                            if (*s != bmp->colour)
                                *d = 0;
                            d += 2;
                            s += 2;
                        }
                    }
                }
            }
        }
        if (dst == (Surface*)&screen)
            UnlockScreen(&screen);
    }
}
