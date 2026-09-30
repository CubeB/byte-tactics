// Decompiled by space-bunny-free, finished by space-bunny-free, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.

// Screen fade: applies a 256 entry translate table to every pixel of `rect` in
// `surface` (or in the locked screen when `surface` is 0). `level` selects one
// of 32 fade-in tables at +0xc4 (for negative levels, offset by 32) or one of 32
// fade-out tables at +0xc8.
//
// STATUS: 87.3 percent, 332 bytes, the original's size (was 67.7 percent on 333
// bytes before this pass). The object is byte-identical to the original except
// for one register swap, described at the end of this comment.
//
// WHAT THIS PASS FIXED, and it is the loop, not the pointer arithmetic.
//
// 1. The row loop must walk `p` itself, not a copy. The old shape kept a second
//    pointer (`char* row = p; ... row++`) and that alone cost everything:
//        while (height--) {
//            int w = rect->right - rect->left + 1;
//            char* next = p + screen.pitch;
//            while (w--) { *p = t[*p]; p++; }
//            p = next;
//        }
//    With `p` as the moving pointer the whole register picture falls into place:
//    `p` lands in ecx (so the pointer block is the original's
//    `imul ecx,eax` / `add ecx,pixels` / `add ecx,left`), the level stays in eax
//    (so `shl eax,8 / add eax,table / mov ebp,eax` is the original's) and the
//    table ends up in ebp for `[ebp+ebx]`. The order of the statements matters
//    as much: `p`, then `height`, then the level block, then `t`.
//    Measured, all free-scored: `p`-in-the-inner-loop with that order is 87.3
//    percent on 332 bytes; the same loop with the pointer AFTER the level block
//    is 70.4 percent on 329, and with the old `row` copy 67.7 on 333. The
//    pointer expression itself is still flat: `pixels + pitch*top + left`,
//    `&pixels[pitch*top + left]`, the two-step `p += ...; p += ...` and
//    `pixels + left + pitch*top` all give the identical 87.3 percent object
//    (Q1..Q4 in build/scratch/0x4bf4d0).
//
// WHAT IS LEFT: the callee-saved pool. The original hands ebx to `surface` and
// ebp to `engine` (`mov ebx,[esp+0x54]` / `mov ebp,eax`); this file hands ebp
// to `surface` and ebx to `engine`. Everything downstream follows: because ebp
// holds a value that dies early here, MSVC reuses ebp for the pixel pointer in
// the pointer block (`mov ebp,[esp+0x2c]`) where the original uses eax, and it
// reloads `surface` into ebp at the tail where the original reloads ebx.
//
// This pass re-measured that axis on the new 87.3 percent object and it is
// still flat, at exactly 87.3 percent on exactly 332 bytes, for all of:
//   - `const` on the engine pointer, engine/screen/r declaration order
//     permutations, engine declared then assigned as a second statement;
//   - `surface == 0` vs `!surface` vs `surface != 0` vs `surface`, and
//     `memcpy(&screen, surface, sizeof screen)` vs `screen = *surface` vs a
//     literal 0x30 size, and `!FUN_004c5e70(...)` vs `FUN_004c5e70(...) == 0`;
//   - `int clipped = ... != 0;` vs `if (... != 0)` vs the raw int result.
// So it is a priority tie in the register allocator, not a missing source idea,
// and the earlier passes' 40+ declaration-order and use-count permutations on
// the 67.7 percent object should not be repeated either. `if (surface != 0)`
// and `if (surface) { screen = *surface; }` (branch inverted) are both much
// worse, 340 bytes / 64.9 percent: the lock path has to be the fall-through.
//
// Established here and matching: the 48 byte surface layout (pitch at +0x0, the
// pixel pointer at +0xc, the clip rect at +0x1c, which fixes `sub esp,0x40` and
// `rep movsd` count 0xc), the pixel pointer as a signed `char*` against an
// `unsigned char*` table (the original's `movsx ebx, byte ptr [ecx]`), the
// `neg/sbb/neg` boolean for FUN_004bf620, the default-rect local `r` at
// esp+0x10 with the store order top, left, right, bottom, the row countdown
// `mov eax,edx / dec edx / test eax,eax` and the row advance `p + screen.pitch`
// computed before the inner loop and reloaded into the row pointer after it.
//
// The two entries docs/bugs.md has for this address are re-checked against the
// disassembly below.
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
        char* p = screen.pixels + screen.pitch * rect->top + rect->left;
        int height = rect->bottom - rect->top + 1;
        unsigned char* table;
        unsigned char* t;
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
        t = table + (level << 8);
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
