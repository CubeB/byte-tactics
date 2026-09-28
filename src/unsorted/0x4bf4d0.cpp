// Decompiled by space-bunny-free, finished by space-bunny-free. Names are provisional.

// Screen fade: applies a 256 entry translate table to every pixel of `rect` in
// `surface` (or in the locked screen when `surface` is 0). `level` selects one
// of 32 fade-in tables at +0xc4 (for negative levels, offset by 32) or one of 32
// fade-out tables at +0xc8.
//
// 69.4 percent. Established and matching: the 48-byte surface layout (pitch at
// +0x0, pixel pointer at +0xc, clip rect at +0x1c, which is what fixes the
// frame at 0x40 and the `rep movsd` count at 0xc), the pixel pointer as a
// signed `char*` against an `unsigned char*` table (the original uses
// `movsx ebx, byte ptr [ecx]`), `int clipped = FUN_004bf620(...) != 0;`
// assigned to a local (a bare `if (f())` folds to `test eax,eax`, a `bool`
// return gives `test al,al`), the lock path as the fall-through, the
// `while (height--)` row loop, the default-rect field order, and `while (w)`
// for the inner loop.
//
// What still differs, and the one thing to attack first: the callee-saved pool
// hands ebp to `surface` and ebx to `engine` here, while the original does the
// opposite (`mov ebx, [esp+0x54]` for surface, `mov ebp, eax` for engine).
// Everything downstream that mentions ebp or ebx is a consequence of that one
// swap, including the default-rect block, the pixel-pointer accumulation
// (`mov edi,[esp+0x20]; add edi,ecx` against `lea edi,[ecx+eax]`), the table
// pointer (`add eax,edi; mov ebp,eax` against `lea ebp,[eax+edi]`) and the
// inner load's SIB order (`[ebp+ebx]` against `[ebx+ebp]`). Two differences
// are independent of it: the default-rect block hoists both engine loads to
// the top instead of interleaving each load with its store, and the
// `level < 0` test is `cmp eax,edi` rather than `test eax,eax`.
//
// Tried in a later pass, none of which moved it:
// - The default rect as an aggregate, `Rect_004bf4d0 r = {0, 0, w, h};`:
//   68.7 percent, slightly worse.
// - `screen = *surface;` instead of `memcpy`: compiles to the identical bytes
//   (same `rep movsd`, same count), so the copy is not what decides the
//   allocation. 69.4 percent, unchanged.
// - Swapping the declaration order of `engine` and `screen`: 69.4 percent,
//   unchanged. So unlike the SIB tie-break in 0x490080, declaration order does
//   not decide this one, which is worth knowing before trying it again.

#include <string.h>

struct Rect_004bf4d0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Surface_004bf4d0 {
    int pitch;                         // +0x0
    char unknown_4[0x8];
    char* pixels;                      // +0xc
    char unknown_10[0x1c - 0x10];
    Rect_004bf4d0 rect;                // +0x1c
    char unknown_2c[0x30 - 0x2c];
};

struct Engine_004bf4d0 {
    char unknown_0[0xc4];
    unsigned char* fade_neg;           // +0xc4
    unsigned char* fade_pos;           // +0xc8
    char unknown_cc[0xd4 - 0xcc];
    int width;                         // +0xd4
    int height;                        // +0xd8
};

Engine_004bf4d0* FUN_004b6220();
int __stdcall FUN_004c5e70(Surface_004bf4d0* out);
int __stdcall FUN_004c5fa0(Surface_004bf4d0* s);
int __stdcall FUN_004bf620(Surface_004bf4d0* s, Rect_004bf4d0* r);

// FUNCTION: 0x4bf4d0
int __stdcall FUN_004bf4d0(Surface_004bf4d0* surface, Rect_004bf4d0* rect, int level)
{
    Engine_004bf4d0* engine = FUN_004b6220();
    Surface_004bf4d0 screen;
    if (surface == 0) {
        if (!FUN_004c5e70(&screen))
            return 0;
    } else {
        memcpy(&screen, surface, sizeof(screen));
    }
    if (rect == 0) {
        Rect_004bf4d0 r;
        r.top = 0;
        r.left = 0;
        r.right = engine->width;
        r.bottom = engine->height;
        rect = &r;
    }
    int clipped = FUN_004bf620(&screen, rect) != 0;
    if (clipped) {
        char* p = screen.pixels + rect->top * screen.pitch + rect->left;
        int height = rect->bottom - rect->top + 1;
        unsigned char* table;
        if (level < 0) {
            if (level < -0x20)
                level = -0x20;
            table = engine->fade_neg;
            level += 0x20;
            if (table == 0)
                return 0;
        } else {
            if (level > 0x1f)
                level = 0x1f;
            table = engine->fade_pos;
            if (table == 0)
                return 0;
        }
        if (table + (level << 8) == 0)
            return 0;
        while (height--) {
            char* row = p;
            char* next = p + screen.pitch;
            int w = rect->right - rect->left + 1;
            while (w) {
                *row = (table + (level << 8))[*row];
                row++;
                w--;
            }
            p = next;
        }
    }
    if (surface == 0)
        FUN_004c5fa0(&screen);
    return 1;
}
