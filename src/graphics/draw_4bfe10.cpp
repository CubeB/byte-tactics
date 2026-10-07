// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and claude-opus-5-5, verified by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.

// Translates every pixel of `rect` in `surface` through the byte table at
// g_game+0xcc (RemapRect), or in the locked screen (LockScreen /
// UnlockScreen) when `surface` is 0. The rect is copied to a local first
// because the clip helper ClipRectangle clips it in place. The locked path
// returns the lock result.
//
// check.py prints MATCH (267 bytes).
//
// THE LEVER (Space Bunny Free, #4733). Everything up to here was 74.9% and 2
// bytes short, and every attempt to fix it treated the locked arm's `dst`
// arithmetic as the problem. It is not: the arithmetic was already right. The
// only difference in the locked arm was that MSVC 5 folded the `screen.pixels`
// load into the final add (`add eax, dword ptr [esp + 0x34]`) instead of
// loading it into a register first (`mov edx, [esp + 0x34]; add eax, edx`),
// and that one decision moved the whole function's register allocation, the
// else arm's `lea`, the load order and the 2 bytes at once.
//
// The fold is decided by DECLARATION ORDER, and only that. Declaring the
// locked screen local BEFORE the rect copy, at function scope:
//
//     Surface_004bfe10 screen;     <- moved up from inside the `if`
//     Rect_004bfe10 r = *rect;
//
// compiles to 267 bytes and every instruction matches, including the whole
// else arm and the tail. It does not move the frame (still `sub esp, 0x40`,
// `screen` still at [esp+0x18] and `r` at [esp+8]), because MSVC 5 allocates
// a local's slot at its first use in the generated code, not at its
// declaration: the rect copy still codegens first, so the layout is unchanged
// and only C1's internal local/NT ordering changes. A micro-probe isolates
// it (build/scratch/0x4bfe10/micro4.cpp, w4 against w5): two declarations that
// generate identical code and an identical frame, differing only in which of
// `S screen;` and `Rect r = *rect;` is written first, produce the two different
// locked arms. Getting the same effect by moving an unused local, `result`,
// `status`, `g`, or the rect copy does NOT work, and neither does splitting
// the copy into four field assignments.
//
// Previous rounds' dead ends, all measured here and all still dead: every
// parenthesisation and every permutation of the three `dst` addends (MSVC 5
// canonicalises `a + b - c`, so all 6 orders are byte-identical); `int`,
// `unsigned`, `long`, `unsigned char*`, `char*`, `void*` and no-cast types for
// `pitch` and `pixels`; a `static inline` getter for either field, for
// `g->field_cc`, and for the whole `dst`; a `static inline` wrapper around the
// call; `dst`, `pitch`, `pixels`, `w`, `h`, `table`, a `Surface*` and a
// `Rect*` each as a local; declaration order of `result`/`status` (12 orders),
// extra dummy locals, and an uninitialised local of each type; `#include
// <windows.h>` and the header sets; `__cdecl`/variadic/extern "C" prototypes
// for RemapRect; and the two wrong-value shapes that reach the original's
// register allocation without being kept (`r.top * screen.pitch + r.left +
// screen.pitch`, 93.3%, and `r.top * (int)screen.pixels + r.left + (int)
// screen.pitch`, 94.9% and 267 bytes). The 94.9% shape matters as evidence:
// putting `pixels` in the multiply and `pitch` in the add forces C1 to
// materialise the pixels load too, which is why it also reaches the original's
// allocation, and it is what pointed at the fold being the real difference.
//
// Suspected original bug: the caller-surface path returns an uninitialised
// local (`status`, warning C4700). `rect` is dead after the copy, so MSVC 5
// puts `status` in rect's parameter slot, which is why the original ends that
// path with `mov esi, [esp + 0x50]` and in practice returns the rect pointer.
// The one `return result;` at the end is what the machine code wants: the
// lock-success path's separate epilogue in the original is MSVC's tail
// duplication, and writing that `return` explicitly moves `push esi` to the
// top. See the matched siblings 0x4be950 and 0x4bfd60 for the same idiom.

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

struct Game_004bfe10 {
    char unknown_0[0xcc];
    int field_cc;                      // +0xcc
    char unknown_d0[0xf1 - 0xd0];
    unsigned char flags;               // +0xf1
};

Game_004bfe10* GetDisplay();
int __stdcall LockScreen(Surface_004bfe10* out);
int __stdcall UnlockScreen(Surface_004bfe10* s);
int __stdcall ClipRectangle(void* s, Rect_004bfe10* r);
void __cdecl RemapRect(int dst, int pitch, int w, int h, int table);

// FUNCTION: 0x4bfe10
int __stdcall GrayRectangle(Surface_004bfe10* surface, Rect_004bfe10* rect)
{
    Surface_004bfe10 screen;
    Rect_004bfe10 r = *rect;
    Game_004bfe10* g = GetDisplay();
    if ((g->flags & 1) == 0)
        return 0;
    int result;
    int status;
    if (surface == 0) {
        result = LockScreen(&screen);
        if (result != 0) {
            if (ClipRectangle(&screen, &r))
                RemapRect(r.top * screen.pitch + r.left + (int)screen.pixels,
                             screen.pitch, r.right - r.left + 1,
                             r.bottom - r.top + 1, g->field_cc);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipRectangle(surface, &r))
            RemapRect((int)surface->pixels + r.top * surface->pitch + r.left,
                         surface->pitch, r.right - r.left + 1,
                         r.bottom - r.top + 1, g->field_cc);
        result = status;
    }
    return result;
}
