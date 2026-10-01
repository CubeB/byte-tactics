// Decompiled by space-bunny-free, finished by space-bunny-free, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash., retried by Sonnet 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// Retry (deepseek-v4.1-flash, issue 3003): re-confirmed 89.6% (332 bytes both).
// The only residual is the engine/surface callee-saved swap (original engine=ebp
// and surface=ebx; ours reversed, and all 8 downstream ebx/ebp mentions follow).
// Retried surface-const, engine-reference, screen-before-engine, register
// keywords, a level local, no-clipped local, hoisted table/t/height/p: all
// identical 89.6. Dummy extern-int sweep 130..400 step 10 stays 89.6/86.6, no
// flip. Compiler-state tie, not source-reachable.
// BUG: the three early `return 0` exits (table==0 in each level branch and
// t==0) return after the screen is locked without calling FUN_004c5fa0, so the
// lock leaks on those paths.
// deepseek-v4.1-flash retry (#2838): reconfirmed 89.6%, 332 bytes. The only
// remaining diff is the engine/surface register swap documented below.
// Tried, all still 89.6: permutations of the engine/screen/r declaration
// order; initialised and uninitialised `Surface* s` aliases declared before
// and after the engine line (used for the copy, the tests, or both); engine as
// `Engine&`, `Engine* const`, and `int` with casts; `screen = *surface`
// instead of memcpy; a pointer-form `screen`; inlined identity helpers taking
// `engine` (2, 6 and 10 extra code-neutral uses); and inlined helpers for the
// rect fallback, the height, the pixel pointer, the clip call, lock/unlock,
// the surface copy and the engine getter. Two other findings:
//   * `tools/headers.py --cpp` on this source: 768 sets, best 89.6
//     (`<string.h>`), so no header set fixes the tie any more.
//   * an inlined `pick` helper that returns `t` DOES put engine in ebp (the
//     original assignment; the entire region up to the table selection then
//     matches) but it collapses the three `return 0` exits into one shared
//     epilogue, 83.4% / 317 bytes. Factoring only the table base or the clamp
//     keeps the three returns but leaves engine in ebx. So engine=ebp here
//     rides on the helper's inlined-call allocation, not on any source-side
//     use-count or declaration-order change.
// Sonnet 5.5 retry (#2789), still 89.6%, 0 gains from 3 more table-pointer
// spellings (`&table[level << 8]`, `table + level * 256`, `if (!t)`; all the
// same object). Two facts read from the original that the notes above do not
// state:
//   * the third `return 0` (after `t == 0`) is a bare epilogue with NO
//     `xor eax, eax`: at 0x4bf5ba the original does `mov ebp, eax; test ebp,
//     ebp; jne`, so eax still holds the zero t and the return reuses it. Ours
//     emits `mov ebp, eax; jne` (flags of the `add` reused, no test) and then
//     `xor eax, eax`. So in the original `t` is tested in its own register
//     (ebp, the register `engine` has just died in), which only happens if
//     engine is the ebp variable: this is the same engine/surface swap, not a
//     separate difference.
//   * `mov [esp+0x58], eax` stores the clip result into the dead `rect`
//     parameter slot (the temp shares the slot), and surface (ebx) is reloaded
//     from [esp+0x54] after the loop because the loop's table-index temp
//     (`movsx ebx`) clobbers it; ours keeps surface in ebp across the loop.
// Rechecked for issue #2332 by GPT-6.1-sol: best remains 89.6% (3 check.py
// invocations in this pass). Replacing memcpy with aggregate assignment emitted
// identical code and did not improve the score. The remaining diff is primarily
// the engine/surface register swap (original engine=ebp, surface=ebx; ours
// engine=ebx, surface=ebp), which also changes pointer setup and final cleanup;
// the null test after forming t is optimized away, and the height-loop branch
// lands four bytes earlier. check.py did not print MATCH.

// Screen fade: applies a 256 entry translate table to every pixel of `rect` in
// `surface` (or in the locked screen when `surface` is 0). `level` selects one
// of 32 fade-in tables at +0xc4 (for negative levels, offset by 32) or one of 32
// fade-out tables at +0xc8.
//
// STATUS: 89.6 percent, 332 bytes against the original's 332. The three earlier
// "give up" passes left it at 67.7 percent / 333 bytes. Three source changes,
// found in this pass, took it to 89.6 and fixed the pixel-pointer register, the
// table register, the loop body and the loop-tail base/index order:
//
// 1. THE INNER LOOP ADVANCES `p` ITSELF. `while (w--) { *p = t[*p]; p++; }`,
//    not `row = p; *row = t[*row]; row++`. With the second form MSVC keeps the
//    walking pointer in eax and the table index in ebx; with the first it keeps
//    it in ecx, which is the original, and that single change also moves `level`
//    from ecx to eax (the original's register), turns the table build into
//    `mov ebp,eax` of the original, and fixes the tail `mov bl,[ebp+ebx]` (base
//    ebp, index ebx) instead of `[ebx+ebp]` with its extra disp8. Worth
//    67.7 -> 82.1 on its own; the earlier passes had measured `while (w--)`
//    with a separate `row` pointer and never tried advancing `p` in place.
//
// 2. COMPUTE `height` BEFORE `p`. With `p` first, the scheduler loads pixels
//    into ebp while eax still holds `rect->top`; with `height` first it loads
//    bottom early and the pointer block comes out in the original's order
//    (`mov eax,[esi+4]; mov ecx,[esp+0x20]; mov edx,[esi+0xc]; imul ecx,eax;
//    sub edx,eax`). 82.1 -> 84.3.
//
// 3. DECLARE `w` BEFORE `next` in the row body. The original computes the row
//    width first (`mov eax,[esi+8]; mov ebx,[esi]; ... inc eax`) and then
//    `add edi,ecx`; `next` first makes MSVC compute a `lea edi,[ecx+eax]`
//    early. 84.3 -> 89.6.
//
// WHAT IS LEFT, and it is one register tie seen in three places:
//   - `surface` is in ebp and `engine` in ebx; the original is the reverse
//     (`mov ebx,[esp+0x54]` for surface, `mov ebp,eax` for engine). All eight
//     downstream ebx/ebp mentions follow from this one swap.
//   - The pixel temporary in the pointer block is `mov ebp,[esp+0x2c]` here,
//     `mov eax,[esp+0x2c]` in the original. It lands in ebp only because ebp is
//     free (surface is dead there); with surface in ebx and engine live in ebp
//     the scheduler must use eax. So this is downstream of the tie above.
//   - At the `t == 0` check the original keeps `test ebp,ebp` and falls to a
//     bare epilogue; here MSVC reuses the flags of `add eax,edi` and emits an
//     extra `xor eax,eax` on the return path.
//
// The tie is NOT a use-count tie. Adding real uses of `surface` (a compare
// against `rect` or `&screen`) and adding/removing engine reads both leave the
// registers unchanged. It is also flat against: 40+ declaration orders, the
// `engine` call split into a declaration plus assignment, `Engine&` and
// `Engine* const`, `Surface screen = *surface;` vs memcpy, an aliased
// `Surface* s = surface` declared before the call (all 89.6, identical object),
// and a static inline `pick` helper that returns `t` (it puts engine in ebp but
// collapses the three separate `return 0` exits, 62 percent). tools/headers.py
// tried all 128 header sets: the best is 86.6 (and 331 bytes), worse than the
// plain `<string.h>` below; `<windows.h>`, `<string>`, `<ddraw.h>` all give the
// same 86.6. A sweep of 0..129 unused functions, prototypes, structs, classes,
// globals and typedefs before the function does not flip it either. So this is
// compiler state from the original translation unit that the source shape
// cannot reach here; the two tryable levers left are (a) the real preceding
// function FUN_004bf260, which changes the score but not the tie (it gives
// 86.6 at 331 bytes when prepended to this body), and (b) whatever else sat in
// Cavedog's original source file before this function.
//
// Established and matching here: the 48-byte surface layout (pitch at +0x0,
// pixel pointer at +0xc, which fixes the frame at 0x40 and the `rep movsd`
// count at 0xc), the pixel pointer as a signed `char*` against an
// `unsigned char*` table (the original uses `movsx ebx, byte ptr [ecx]`), the
// lock path as the fall-through, the `level < 0` test as `cmp eax,edi` against
// the zeroed edi, and `while (height--)` for the row loop.
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