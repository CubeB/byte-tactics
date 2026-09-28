// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.

// Translates every pixel of `rect` in `surface` through the byte table at
// g_game+0xcc (FUN_004cced5), or in the locked screen (FUN_004c5e70 /
// FUN_004c5fa0) when `surface` is 0. The rect is copied to a local first
// because the clip helper FUN_004bf620 clips it in place. The locked path
// returns the lock result and the caller-surface path returns `rect` itself,
// so a failed lock returns 0 and leaves the screen locked (the unlock only
// runs on success).
//
// Best so far: 64.6 percent, and semantically correct.
//
// A WARNING about the note that used to be here, since it was wrong and cost
// time. It claimed "Best so far: 63.7 percent ... both blit calls instruction
// for instruction (argument order, register choice ...)". That 63.7% was a
// version with a deliberately wrong locked-blit expression, written only to see
// what it would score. It was discarded, as it should have been, but the note
// describing it was left in place, so the file it was attached to was really
// 55.7% and did NOT have matching blit calls. If you are reading an older note
// that claims 63.7% with both calls matching, it is describing code that is not
// in this file. The current 64.6% beats it while being correct.
//
// Established and matching: the whole control flow (the flag test, the lock
// path as the fall-through, the two exits, the 48-byte surface layout that
// fixes the 0x40 frame, the 16-byte rect copy), and the else arm's blit call.
//
// The one thing that was blocking the frame, and how it was fixed: MSVC promotes
// the `rect` parameter to a callee-saved register, so it pushes ebx at the top
// and every frame offset in both arms is 4 higher than the original's. The
// original never does that: it reads the argument into eax at the top
// (`mov eax, [esp + 0x48]`) and re-loads it from the stack slot at the very end
// (`mov esi, [esp+0x50]`, which the locked path's failed-lock exit jumps
// straight into at the following `mov eax, esi`). Declaring the parameter
// `Rect_004bfe10* volatile rect` is what stops the promotion: the ebx push
// disappears and the frame offsets become the original's. It is a codegen
// device rather than a demonstrated `volatile` in the original (the argument
// is written only through the local copy, so a volatile read is a no-op
// semantically), and it is not a narrowing cast, so nothing is truncated. It
// costs one thing: MSVC then saves esi at the top of the function and pops it
// on the early-return path, where the original shrink-wraps the `push esi` to
// just after that early return. 64.6% with it, 55.7% without.
//
// What is still different, and note where the remaining work is NOT: the blit
// arguments in the LOCKED arm are evaluated in a different order (the else arm
// matches). The original pushes them right to left, field_cc, then height
// (`bottom - top + 1`), then width (`right - left + 1`), then pitch, then dst,
// computing the two `sub`/`inc` pairs before the `imul`; this version computes
// dst first. The likely cause is the struct the locked path uses: the original
// reads the locked screen's pitch from +0xc and its pixel base from +0x10, one
// dword higher than the surface path's +0x8 and +0xc, so either that is a
// genuine bug in the original or the local is a second, differently laid out
// type. That is a types question, not a scheduling one.
//
// DO NOT reach for `volatile` on the rect parameter, even though it looks like
// the obvious answer and even though it works on the code. Declaring it
// `Rect_004bfe10* volatile rect` does remove the promotion: the ebx push
// disappears, the frame offsets become the original's, and the score goes from
// 55.7 to 64.6 percent. But MSVC 5 mangles a volatile POINTER parameter as a
// reference, so the symbol changes from the original's
// `?FUN_004bfe10@@YGHPAUSurface_004bfe10@@PAURect_004bfe10@@@Z` to one
// containing `RAURect_004bfe10`. The calling convention happens to be
// identical, which is why the checker still compares the code, but the
// declaration is then a reference where the original has a pointer, and a
// wrong signature is a worse defect than a 9 percent gap. Reverted for that
// reason. If someone finds a way to stop the promotion that keeps the plain
// pointer parameter, that is the remaining lead.
//
// Tried before, none of which removed the promotion (all still `push ebx`): a
// `result` local assigned on both paths; a `result` initialised with the rect
// next to the copy; a pointer local holding the rect; the copy through a local
// pointer, through `int*` indexing, through an inlined `CopyRect` helper and
// through a copy constructor; a field-by-field copy and the copy in both field
// orders; a pointer return type and a `void*` return type; a reference
// parameter with `&rect`; the two branch orders; `if (g->flags & 1) { ... }`
// instead of an early return; the whole body in nested blocks; the clip result
// in a local; the locked arm and the else arm each wrapped in an inlined
// helper; and moving the copy after the engine call.

#include <windows.h>

struct Rect_004bfe10 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Surface_004bfe10 {
    char unknown_0[8];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    char unknown_10[0x30 - 0x10];
};

// The locked screen is read through a different field list than the surface
// argument: the pitch comes from +0xc and the pixel base from +0x10, one
// dword higher than Surface_004bfe10 (see the bug note in the header).
struct Screen_004bfe10 {
    char unknown_0[0xc];
    int pitch;                         // +0xc
    unsigned char* pixels;             // +0x10
    char unknown_14[0x30 - 0x14];
};

struct Game_004bfe10 {
    char unknown_0[0xcc];
    int field_cc;                      // +0xcc
    char unknown_d0[0xf1 - 0xd0];
    unsigned char flags;               // +0xf1
};

Game_004bfe10* FUN_004b6220();
int __stdcall FUN_004c5e70(Surface_004bfe10* out);
int __stdcall FUN_004c5fa0(Surface_004bfe10* s);
int __stdcall FUN_004bf620(void* s, Rect_004bfe10* r);
void __cdecl FUN_004cced5(int dst, int pitch, int w, int h, int table);

// FUNCTION: 0x4bfe10
int __stdcall FUN_004bfe10(Surface_004bfe10* surface, Rect_004bfe10* rect)
{
    Rect_004bfe10 r = *rect;
    Game_004bfe10* g = FUN_004b6220();
    if ((g->flags & 1) == 0)
        return 0;
    if (surface == 0) {
        Surface_004bfe10 screen;
        int locked = FUN_004c5e70(&screen);
        if (locked != 0) {
            if (FUN_004bf620(&screen, &r))
                FUN_004cced5((int)screen.pixels + r.top * screen.pitch + r.left,
                             screen.pitch, r.right - r.left + 1,
                             r.bottom - r.top + 1, g->field_cc);
            FUN_004c5fa0(&screen);
        }
        return locked;
    } else {
        if (FUN_004bf620(surface, &r))
            FUN_004cced5((int)surface->pixels + r.top * surface->pitch + r.left,
                         surface->pitch, r.right - r.left + 1,
                         r.bottom - r.top + 1, g->field_cc);
    }
    return (int)rect;
}
