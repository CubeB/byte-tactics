// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Converts the clipped part of `rect` on `surface` (or on the locked screen
// when `surface` is 0) through FUN_004cced5. The rect is copied to a local
// first because the clip helper modifies it in place. Returns the lock result
// on the locked path and `rect` on the caller-supplied-surface path.
//
// 59.7 percent. Established and matching: the whole control flow (flag test,
// the lock path as the fall-through, the single `result` variable so the
// failed-lock case cross-jumps to the caller-surface epilogue, the 48-byte
// screen layout and the 0x40 frame, and the 16-byte rect copy).
//
// What still differs, and the one thing to attack first: MSVC keeps `rect`
// live across the calls in ebx here, so it pushes ebx at the top and all the
// frame offsets shift by 4 (`mov eax,[esp+0x48]` becomes `mov ebx,[esp+0x4c]`,
// and the reload `mov esi,[esp+0x50]` of the original disappears). That one
// register is what moves the blit argument computation (`mov edx,[esp+0x18]`
// before `mov ecx,[esp+0x10]` in ours against `mov eax,[edi+0xcc]` first in
// the original) and the branch targets. Removing it would line everything up.
//
// Tried, none of which moved it: copy spellings (`*rect`, `rect[0]`, a cast,
// field by field, memcpy, a user copy constructor and operator=, an inline
// copy helper, a local pointer, an int pointer round trip, a reference
// parameter, `Rect* const`, an array parameter, an int parameter); return
// spellings (through a `result` local, a pointer return, `&rect[0]`, an inline
// identity helper); the two branch orders; declaring `result` at the top and
// initialising it from `rect`; extra folded uses of `surface` and `g`; the
// N-declarations sweep (0 to 3000 unused externs), function-prototype sweeps,
// and every headers.py set.

struct Rect_004bfe10 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Surface_004bfe10 {
    char unknown_0[8];
    int stride;                        // +0x8
    int base;                          // +0xc
};

struct Screen_004bfe10 {
    char unknown_0[8];
    int stride;                        // +0x8
    char unknown_c[4];
    int base;                          // +0x10
    int unknown_14[7];                 // +0x14, 48 bytes total
};

struct Game_004bfe10 {
    char unknown_0[0xcc];
    int field_cc;                      // +0xcc
    char unknown_d0[0xf1 - 0xd0];
    unsigned char flags;               // +0xf1
};

Game_004bfe10* FUN_004b6220(void);
int __stdcall FUN_004c5e70(Screen_004bfe10* out);
int __stdcall FUN_004c5fa0(Screen_004bfe10* s);
int __stdcall FUN_004bf620(void* s, Rect_004bfe10* r);
void __cdecl FUN_004cced5(int dst, int stride, int w, int h, int table);

// FUNCTION: 0x4bfe10
int __stdcall FUN_004bfe10(Surface_004bfe10* surface, Rect_004bfe10* rect)
{
    Rect_004bfe10 r = *rect;
    Game_004bfe10* g = FUN_004b6220();
    if ((g->flags & 1) == 0)
        return 0;
    int result;
    if (surface == 0) {
        Screen_004bfe10 screen;
        result = FUN_004c5e70(&screen);
        if (result) {
            if (FUN_004bf620(&screen, &r))
                FUN_004cced5(r.top * screen.stride + r.left + screen.base,
                             screen.stride, r.right - r.left + 1,
                             r.bottom - r.top + 1, g->field_cc);
            FUN_004c5fa0(&screen);
        }
    } else {
        if (FUN_004bf620(surface, &r))
            FUN_004cced5(r.top * surface->stride + surface->base + r.left,
                         surface->stride, r.right - r.left + 1,
                         r.bottom - r.top + 1, g->field_cc);
        result = (int)rect;
    }
    return result;
}
