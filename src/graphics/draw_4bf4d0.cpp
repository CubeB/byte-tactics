// Decompiled by space-bunny-free, finished by space-bunny-free, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash., retried by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
// Screen fade: applies a 256 entry translate table to every pixel of `rect` in
// `surface` (or in the locked screen when `surface` is 0). `level` selects one
// of 32 fade-in tables at +0xc4 (for negative levels, offset by 32) or one of 32
// fade-out tables at +0xc8.
//
// MATCHED (mimo-v2.6-pro): earlier passes sat at 89.6 percent with the
// engine/surface registers swapped (ours engine=ebx, surface=ebp; the original
// is the reverse) and the `t == 0` null test folded into the flags of the
// `add`, dropping `test ebp, ebp` and adding `xor eax, eax` at that return. The
// fix is the one docs/agent-guide.md records for this address: compute
// `t = table + (level << 8)` in EACH branch of the level test instead of once
// after the merge. The null test then cannot fold into the add's flags (`test
// ebp, ebp` stays and the bare return reuses the zero eax proves to hold), and
// the callee-saved allocation falls into the original's engine=ebp,
// surface=ebx, which also restores the pointer block's `mov eax, [esp+0x2c]`
// and the `je` that skips the surface reload on the zero-height path.
// Still load-bearing from the earlier passes: the inner loop advances `p`
// itself (`while (w--) { *p = t[*p]; p++; }`), `height` is computed before `p`,
// and `w` is declared before `next` in the row body.
//
// The 0x4bf4d0 entry in docs/bugs.md (three failure exits skip the unlock, and
// `movsx` indexes the table with a sign-extended byte) is confirmed by the
// disassembly and is reproduced here.

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
    Rect_004bf4d0 r;
    if (surface == 0) {
        if (!FUN_004c5e70(&screen))
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
    int clipped = FUN_004bf620(&screen, rect) != 0;
    if (clipped) {
        int height = rect->bottom - rect->top + 1;
        char* p = screen.pixels + screen.pitch * rect->top + rect->left;
        unsigned char* table;
        unsigned char* t;
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
            int w = rect->right - rect->left + 1;
            char* next = p + screen.pitch;
            while (w--) {
                *p = t[*p];
                p++;
            }
            p = next;
        }
    }
    if (surface == 0)
        FUN_004c5fa0(&screen);
    return 1;
}
