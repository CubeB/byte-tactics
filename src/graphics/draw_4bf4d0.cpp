// Decompiled by space-bunny-free, finished by space-bunny-free, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash., retried by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
// Screen fade: applies a 256 entry translate table to every pixel of `rect` in
// `surface` (or in the locked screen when `surface` is 0). `level` selects one
// of 32 fade-in tables at +0xc4 (for negative levels, offset by 32) or one of 32
// fade-out tables at +0xc8.

#include <string.h>

struct Rect_004bf4d0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Surface {
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

Engine_004bf4d0* GetDisplay();
int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipRectangle(Surface* s, Rect_004bf4d0* r);

// FUNCTION: 0x4bf4d0
int __stdcall FadeRectangle(Surface* surface, Rect_004bf4d0* rect, int level)
{
    Engine_004bf4d0* engine = GetDisplay();
    Surface screen;
    Rect_004bf4d0 r;
    if (surface == 0) {
        if (!LockScreen(&screen))
            return 0;
    } else {
        memcpy(&screen, surface, sizeof(screen));
    }
    if (rect == 0) {
        r.top = 0;
        r.left = 0;
        r.right = engine->width;
        r.bottom = engine->height;
        rect = &r;
    }
    int clipped = ClipRectangle(&screen, rect) != 0;
    if (clipped) {
        // height is computed before p.
        int height = rect->bottom - rect->top + 1;
        char* p = screen.pixels + screen.pitch * rect->top + rect->left;
        unsigned char* table;
        unsigned char* t;
        // t is computed in each branch, not once after the merge.
        if (level < 0) {
            if (level < -0x20)
                level = -0x20;
            table = engine->fade_neg;
            level += 0x20;
            if (table == 0)
                return 0;
            t = table + (level << 8);
        } else {
            if (level > 0x1f)
                level = 0x1f;
            table = engine->fade_pos;
            if (table == 0)
                return 0;
            t = table + (level << 8);
        }
        if (t == 0)
            return 0;
        while (height--) {
            // w is declared before next.
            int w = rect->right - rect->left + 1;
            char* next = p + screen.pitch;
            // Advances p itself in the body, not in a for header.
            while (w--) {
                *p = t[*p];
                p++;
            }
            p = next;
        }
    }
    if (surface == 0)
        UnlockScreen(&screen);
    return 1;
}
