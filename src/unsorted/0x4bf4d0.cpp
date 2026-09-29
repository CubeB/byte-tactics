// Decompiled by space-bunny-free, finished by space-bunny-free. Names are provisional.

// Screen fade: applies a 256 entry translate table to every pixel of `rect` in
// `surface` (or in the locked screen when `surface` is 0). `level` selects one
// of 32 fade-in tables at +0xc4 (for negative levels, offset by 32) or one of 32
// fade-out tables at +0xc8.
//
// STATUS: 67.7 percent, 333 bytes against the original's 332. Gave up (the
// earlier "gave up, Claude Sonnet 5.5 pass" note below was 69.4 percent on 341
// bytes, i.e. a WORSE object; the current file is the better one).
//
// The three source changes the earlier pass identified as structurally right
// but left unapplied ARE applied now. Together they take the file from 341 to
// 333 bytes and fix the loop, the exits, the default-rect block and the tail:
// 1. `Rect_004bf4d0 r;` is declared at FUNCTION scope next to `screen`, with
//    the default-rect block assigning into it and then `rect = &r`. This makes
//    the default-rect block interleave loads and stores like the original
//    (top, left, load width, lea esi, right, load height, bottom).
// 2. The inner loop is `while (w--) { *row = t[*row]; row++; }`, matching the
//    original's `mov ebx,eax; dec eax; test ebx,ebx; je; inc eax`.
// 3. The table is folded into one pointer, `unsigned char* t = table + (level <<
//    8); if (t == 0) return 0;`, and the loop indexes `t[*row]`. That is the
//    original's `add eax,edi; mov ebp,eax; test ebp,ebp` with `[ebp+ebx]`.
//
// WHAT IS LEFT, and it is one thing seen two ways:
//
// (a) The callee-saved pool hands ebp to `surface` and ebx to `engine`; the
//     original does the opposite (`mov ebx,[esp+0x54]` for surface, `mov ebp,eax`
//     for engine). Everything downstream that mentions ebp or ebx follows from
//     that one swap.
//
// (b) The pixel-pointer block has eax and ecx rotated. Original: top into eax,
//     pitch into ecx, `imul ecx,eax`, so the pointer accumulates in ecx and
//     `level` ends up in eax; the same ecx is then the loop's row pointer. Here:
//     top into ecx, pitch into eax, `imul eax,ecx`, pointer accumulates in eax
//     and `level` ends up in ecx, so the loop has to move the pointer back.
//     Consequence worth naming: the original's `t == 0` exit is a bare
//     `pop edi / pop esi / pop ebp / pop ebx / add esp,0x40 / ret 0xc` with NO
//     `xor eax,eax`, because value numbering already knows eax is 0 there (eax
//     was `level` and `mov ebp,eax` made ebp the same value). With the pointer
//     in eax instead, eax is junk on that path and MSVC must emit
//     `xor eax,eax`. That single instruction is the whole 333 against 332.
//
// A SECOND PASS ON (a), THE TYPE AND LIVENESS AXIS, ALSO FLAT. Someone will
// otherwise try these, because they are the project's highest-value rule
// applied to a "register allocation" difference. gen_types.py and gen_live.py,
// every one free-scored, and every one the SAME 333-byte object at 67.7%:
//   const engine pointer; engine assigned in a separate statement; surface
//   aliased through a `Surface_004bf4d0* const s`; the engine/screen
//   declaration order swapped; engine initialised through an explicit cast;
//   engine's field read early to lengthen its live range; `surface` tested
//   before the engine call; a `int nosurf` boolean for the surface test (this
//   one is worse at 344 bytes / 55.7%, so the boolean itself is wrong).
// So (a) resists type, const, order, cast and live-range, on top of everything
// recorded below. That is 13+ further shapes at exactly one score, which makes
// (a) a register-priority tie rather than a missing source idea.
//
// TRAP, RECORDED SO IT IS NOT REDISCOVERED: making FUN_004b6220 a
// `static inline` that returns a global compiles to EXACTLY 332 bytes, the
// original's size, and scores 61.9%. It is worse, not better. The size matches
// only because the call disappears and is replaced by a global load, so the
// engine pointer is read from a different address than the call's eax, and one
// real instruction is lost. See build/scratch/0x4bf4d0/v_inline.TRAP.md. This
// is the mirror image of the usual "a higher score came from deleting code"
// warning: here an exact size came from deleting the CALL.
//
// MEASURED THIS PASS (all compiled, all free-scored, every one 333 bytes / 67.7
// percent, i.e. the identical object, so the difference (a)+(b) is NOT reachable
// from any of these):
// - Declaration order: engine/screen swapped, engine/screen/r permuted, and
//   `clipped`, `table`, `t`, `p`, `height` each moved in and out of the
//   declaration block. No effect. This confirms and extends the earlier
//   "40 permutations, all identical" measurement.
// - Reference counts do not drive the pool either. Adding one or two extra
//   `if (surface) { }` reads, adding an `if (engine->width) { }` read, and
//   putting a bogus `if (surface) { }` inside the row loop and inside the
//   negative-level arm: all 333 bytes, identical. So the earlier "one more or
//   one fewer use of engine or surface" measurement is now explained: it is not
//   a use count.
// - The pixel pointer is completely canonicalised by MSVC. All of
//   `pixels + pitch*top + left`, `pitch*top + pixels + left`,
//   `pixels + top*pitch + left`, `pixels + (pitch*top + left)`,
//   `pixels + left + pitch*top`, `pixels + (left + pitch*top)`,
//   `left + pixels + pitch*top`, `pixels + (top*pitch) + left` and the split
//   forms `p = ...; p += left;` compile to the same bytes, with and without
//   parentheses, and with a hoisted `int pitch = screen.pitch;`. So "which side
//   of the multiply is written first" is NOT the lever, and neither is
//   hoisting the pitch.
// - Statement order of `p` and `height`, `while (w)` vs `while (w--)` for the
//   inner loop, `height` as `bottom - top` plus a separate `height++`, and
//   `height` as `(bottom - top) + 1`: all 333.
// - `int clipped = FUN_004bf620(...)` needs the `!= 0`. Dropping it gives 325
//   bytes / 60.2 percent, so the `neg eax / sbb eax,eax / neg eax` sequence is
//   right. (Re-confirms the earlier measurement.)
// - The `static inline` helper DOES flip (a): passing `engine` into
//   `static unsigned char* pick_004bf4d0(Engine*, int)` makes the engine loads
//   become `[ebp+0xc4]` and `[ebp+0xc8]`, the original's register. But it costs
//   the three separate failure exits: the helper's two `return 0` become
//   `xor ebp,ebp / jmp` into one join plus a `test ebp,ebp`, giving 319 bytes
//   / 61.5 percent. Keep it in build/scratch/0x4bf4d0/h1.cpp. Note (b) does
//   NOT move with it: the helper version still computes the pixel pointer into
//   eax. So (a) and (b) are independent, and fixing (a) alone is not enough.
// - Untried: a helper around only the pixel loop (it might carry (a)'s weight
//   without collapsing the exits), and giving up `engine` as a variable
//   altogether by reordering so the two `engine->...` loads are the only uses.
//
// Established and matching here: the 48-byte surface layout (pitch at +0x0,
// pixel pointer at +0xc, clip rect at +0x1c, which fixes the frame at 0x40 and
// the `rep movsd` count at 0xc), the pixel pointer as a signed `char*` against
// an `unsigned char*` table (the original uses `movsx ebx, byte ptr [ecx]`), the
// lock path as the fall-through, `while (height--)` for the row loop, the
// `level < 0` test as `cmp eax,edi` against the zeroed edi that `xor edi,edi`
// set earlier, and `screen = *surface` against `memcpy(&screen, surface, ...)`
// which produce identical bytes.
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
            char* row = p;
            char* next = p + screen.pitch;
            int w = rect->right - rect->left + 1;
            while (w--) {
                *row = t[*row];
                row++;
            }
            p = next;
        }
    }
    if (surface == 0)
        FUN_004c5fa0(&screen);
    return 1;
}
