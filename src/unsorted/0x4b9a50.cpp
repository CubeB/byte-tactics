// Decompiled by Sonnet 5.5, finished by Space Bunny Free. Names are provisional.
// The opaque twin of 0x4b9740: draws a bitmap tree scaled by (sx, sy) at
// x, y into `dst` (or the locked screen when null) and copies every pixel that
// is not the transparent colour straight into the surface. A child whose kind
// byte (+0xb) is set is drawn by the blending version 0x4b9740, any other by
// this function again.
//
// NOT MATCHED: 98.6%, size exact, 2 bytes left. Both are the SIB byte of
// `lea edx, [edi + edx - 1]` (0x4b9bb8) and `lea ecx, [esi + ecx - 1]`
// (0x4b9bc7): the original puts w / h in the base and the rect edge in the
// index, this source always does the opposite. The value is the same, only
// the SIB role differs. Insensitive to the written operand order (a + b,
// b + a, a - 1 + b, (a - 1) + b, an inline helper, a double cast, unsigned
// qualifiers, extra temporaries, and reordering the eight rect stores all
// produce the same two bytes), so it looks like an MSVC 5 tie-break in the
// ADD to LEA fold that no plain source shape reaches.
// Two findings that did matter, both visible in the frame:
// - The two outer loop variables live in the dead argument slots of x and y
//   (+0x88 and +0x8c), so they are the parameters themselves, reused after
//   dest.left / dest.top have been computed from them. Plain locals land in
//   those two slots the other way round, fy first.
// - Both 16.16 accumulators are written in the for-increment, not as trailing
//   statements, and the inner loop's column is declared in the outer body
//   before rowBase, which is what puts rowBase in the `add ebx, ...` and the
//   column in the SIB index of the pixel store.
//
// The last two bytes are the SIB base/index swap, which is PROVEN unreachable,
// so 98.6% with the size exact is the ceiling for the single-file shape. The
// two lines are
//
//     original   lea edx, [edi + edx - 1]      ours   lea edx, [edx + edi - 1]
//     original   lea ecx, [esi + ecx - 1]      ours   lea ecx, [ecx + esi - 1]
//
// with the same values and the same `- 1`. The proof that the order is not a
// function of the source is 0x4c1480 against 0x4c1760: character-identical
// source, opposite orders, and the matched one is 0x4c1480. This function is
// simply another instance of it, and a particularly clean one, because every
// other byte in the 799 matches.
//
// Measured, and worth recording as the shape of a *finished* search: both
// operand orders, `a - 1 + b`, parenthesised variants, a `static inline`
// helper, an explicit double cast, unsigned qualifiers, extra temporaries, and
// all eight rect-store orderings. Nothing moved it.
//
// Two operational notes, both of which cost time here:
//   - the `call <addr>` line that appears in the raw object diff for the
//     self-recursive call is a CHECKER ARTEFACT, not a difference. The only
//     real differences are the two SIB bytes above. Do not chase it.
//   - the caller's register is the *argument slot* being reused. `x` and `y`
//     are not loop variables in the source's sense; the two outer loop counters
//     are the parameters themselves, reused as the row index and the 16.16
//     source row, in the dead argument slots at +0x88 and +0x8c. Plain locals
//     get those two slots the other way round, which is why the obvious version
//     does not match however it is written.

#include <windows.h>

struct Rect_004b9a50 {
    int left;
    int top;
    int right;
    int bottom;
};

class Class_004c6ae0 {
public:
    char unknown_0[8];
    int pitch;                          // +0x8
    unsigned char* pixels;              // +0xc
    char unknown_10[0x1c - 0x10];
    Rect_004b9a50 field_1c;             // +0x1c

    Rect_004b9a50* FUN_004c6ae0(Rect_004b9a50* out);
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

int __stdcall FUN_004c5e70(Surface_004b9a50* out);
int __stdcall FUN_004c5fa0(Surface_004b9a50* s);
void __stdcall FUN_004b9740(Class_004c6ae0* dst, Bitmap_004b9a50* bmp, int x, int y, double sx, double sy);
void __stdcall FUN_004b7e60(Rect_004b9a50* other, Rect_004b9a50* rect, Rect_004b9a50* bounds);

// FUNCTION: 0x4b9a50
void __stdcall FUN_004b9a50(Class_004c6ae0* dst, Bitmap_004b9a50* bmp, int x, int y, double sx, double sy)
{
    Surface_004b9a50 screen;
    if (dst == 0) {
        int ok = FUN_004c5e70(&screen);
        if (ok != 0)
            dst = (Class_004c6ae0*)&screen;
    }
    if (bmp->count > 0) {
        for (int i = 0; i < (int)bmp->count; i++) {
            Bitmap_004b9a50* e = ((Bitmap_004b9a50**)bmp->field_10)[i];
            if (e->kind > 0)
                FUN_004b9740(dst, e, x, y, sx, sy);
            else
                FUN_004b9a50(dst, e, x, y, sx, sy);
        }
    } else {
        int w = (int)(bmp->width * sx);
        int h = (int)(bmp->height * sy);
        int dx = (int)(bmp->dx * sx);
        int dy = (int)(bmp->dy * sy);
        if (w > 0 && h > 0) {
            Rect_004b9a50 src;
            Rect_004b9a50 dest;
            Rect_004b9a50 bounds;
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
            dst->FUN_004c6ae0(&bounds);
            FUN_004b7e60(&src, &dest, &bounds);
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
    if (dst == (Class_004c6ae0*)&screen)
        FUN_004c5fa0(&screen);
}
