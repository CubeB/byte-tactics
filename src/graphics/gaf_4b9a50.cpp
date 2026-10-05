// Decompiled by Sonnet 5.5, finished by Space Bunny Free, finished by Sonnet 5.5. Names are provisional.
// The opaque twin of 0x4b9740: draws a bitmap tree scaled by (sx, sy) at
// x, y into `dst` (or the locked screen when null) and copies every pixel that
// is not the transparent colour straight into the surface. A child whose kind
// byte (+0xb) is set is drawn by the blending version 0x4b9740, any other by
// this function again.
// Matching notes: the three rects are declared before w and h, which fixes
// the base/index order of the rect-edge leas (they are not an MSVC tie-break
// that no source reaches, as an earlier note claimed). The two outer loop
// variables are the dead x / y argument slots, and col is declared before
// rowBase inside the body.
#include <windows.h>

struct Rect_004b9a50 {
    int left;
    int top;
    int right;
    int bottom;
};

class Surface {
public:
    char unknown_0[8];
    int pitch;                          // +0x8
    unsigned char* pixels;              // +0xc
    char unknown_10[0x1c - 0x10];
    Rect_004b9a50 field_1c;             // +0x1c

    Rect_004b9a50* GetClipRect(Rect_004b9a50* out);
};

struct Bitmap_004b9a50 {
    unsigned short width;               // +0x0
    unsigned short height;              // +0x2
    short dx;                           // +0x4
    short dy;                           // +0x6
    unsigned char colour;               // +0x8
    unsigned char flag9;                // +0x9
    unsigned char count;                // +0xa
    unsigned char kind;                 // +0xb
    int unknown_c;                      // +0xc
    void* field_10;                     // +0x10
};

struct Surface_004b9a50 {
    int data[12];
};

int __stdcall LockScreen(Surface_004b9a50* out);
int __stdcall UnlockScreen(Surface_004b9a50* s);
void __stdcall DrawFrameScaledBlended(Surface* dst, Bitmap_004b9a50* bmp, int x, int y, double sx, double sy);
void __stdcall ClipRects(Rect_004b9a50* other, Rect_004b9a50* rect, Rect_004b9a50* bounds);

// FUNCTION: 0x4b9a50
void __stdcall DrawFrameScaled(Surface* dst, Bitmap_004b9a50* bmp, int x, int y, double sx, double sy)
{
    Surface_004b9a50 screen;
    if (dst == 0) {
        int ok = LockScreen(&screen);
        if (ok != 0)
            dst = (Surface*)&screen;
    }
    if (bmp->count > 0) {
        for (int i = 0; i < (int)bmp->count; i++) {
            Bitmap_004b9a50* e = ((Bitmap_004b9a50**)bmp->field_10)[i];
            if (e->kind > 0)
                DrawFrameScaledBlended(dst, e, x, y, sx, sy);
            else
                DrawFrameScaled(dst, e, x, y, sx, sy);
        }
    } else {
        Rect_004b9a50 src;
        Rect_004b9a50 dest;
        Rect_004b9a50 bounds;
        int w = (int)(bmp->width * sx);
        int h = (int)(bmp->height * sy);
        int dx = (int)(bmp->dx * sx);
        int dy = (int)(bmp->dy * sy);
        if (w > 0 && h > 0) {
            dest.left = x - dx;
            dest.top = y - dy;
            dest.right = w + dest.left - 1;
            dest.bottom = h + dest.top - 1;
            src.left = 0;
            src.top = 0;
            src.right = w - 1;
            src.bottom = h - 1;
            int stepX = (bmp->width << 16) / w;
            int stepY = (bmp->height << 16) / h;
            dst->GetClipRect(&bounds);
            ClipRects(&src, &dest, &bounds);
            if (dest.right >= dest.left && dest.bottom >= dest.top && src.right >= src.left &&
                src.bottom >= src.top) {
                src.left = (int)(src.left / sx);
                src.top = (int)(src.top / sy);
                src.right = (int)(src.right / sx);
                src.bottom = (int)(src.bottom / sy);
                // The original keeps the two outer loop variables in the dead
                // argument slots of x and y, so they are the parameters
                // themselves, reused once dest.left / dest.top are computed
                // from them. Both 16.16 accumulators live in the increment.
                for (x = dest.top, y = src.top << 16; x <= dest.bottom; x++, y += stepY) {
                    // col first: declaring it here is what puts rowBase in the
                    // add and col in the SIB index of the pixel store.
                    int col = dest.left;
                    int srcRow = (y >> 16) * bmp->width;
                    int rowBase = x * dst->pitch;
                    for (int fx = src.left << 16; col <= dest.right; col++, fx += stepX) {
                        unsigned char c = ((unsigned char*)bmp->field_10)[(fx >> 16) + srcRow];
                        if (c != bmp->colour) {
                            dst->pixels[rowBase + col] = c;
                        }
                    }
                }
            }
        }
    }
    if (dst == (Surface*)&screen)
        UnlockScreen(&screen);
}
