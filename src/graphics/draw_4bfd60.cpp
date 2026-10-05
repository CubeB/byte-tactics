// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.

// Xor-fills `rect` on `surface` (or on the locked screen when `surface` is 0)
// with the byte `value`, through FUN_004cce87. The rect is copied to a local
// first because the clip helper ClipRectangle clips it in place. Returns the
// lock result on the screen path and `value` on the caller-surface path.
//
// check.py prints MATCH (167 bytes).
//
// WARNING: the `result = ((int*)&surface)[2];` in the else arm is a codegen
// probe, not a guess at the original's spelling. It reads exactly the same
// four bytes as `value` (the third parameter's stack slot, at surface+8),
// but through a syntactically different expression, and that difference is
// the whole trick: MSVC 5 unifies two reads of the same parameter into one
// register-allocated value (live across the FUN_004cce87 call, so it needs a
// callee-saved register, hence the extra `push edi`/`pop edi` and the two
// bytes short). A differently spelled read of the same slot stays two
// separate C1 values, so the else arm reloads it into esi, which is the
// shared return-value register here. With the natural `result = value;`
// this function scores 59.0 percent; with `return value;` in an early-return
// else arm it scores 87.6 percent. The original source most likely used some
// spelling this session could not guess (see the notes at the end).
//
// What the machine code fixes, and what to attack first in any rewrite:
// - one saved register only (`push esi`), so the result variable, `surface`
//   and the lock result all share esi, and the frame is 0x40 with the rect
//   copy at [esp+4] and the 0x30-byte screen at [esp+0x14];
// - a single shared tail: both paths reach `mov eax, esi`, which is why the
//   lock failure branches to 0x4bfdfe and not to its own epilogue, so the
//   result is one function-level variable assigned in both arms with the
//   `return` outside them, not an early `return` per arm;
// - the locked path copies the LockScreen result into esi at once
//   (`mov esi, eax; test esi, esi`), and the locked path's fill argument is
//   reloaded from [esp+0x50] even though the value is also read in the else
//   arm, so a parameter read in two arms is not a register variable.
//
// Tried and none of it moved the else arm: shared vs per-arm result
// variables, per-arm `return`, the value's type (int, unsigned, long, a 4-byte
// struct), a local copy of the value used for the fill and/or the return, a
// `static inline` helper for the fill and for the return, an inline helper
// around the whole else arm, `value` read through its own address
// (`*(int*)&value`, `*(&value)`), comma-operator and no-op-cast spellings,
// `+ 0`, the arms swapped, `do`/`goto` forms, a bare `#include <windows.h>`
// and an unsigned return type. deepseek-v4.1-flash had already established
// that the locked path, the two exit blocks, the 3-argument __stdcall
// signature and the 0x30-byte screen match byte for byte.

struct Rect_004bfd60 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipRectangle(Surface* s, Rect_004bfd60* r);
void __cdecl FUN_004cce87(Surface* s, Rect_004bfd60* r, int value);

// FUNCTION: 0x4bfd60
int __stdcall XorRectangle(Surface* surface, Rect_004bfd60* rect, int value)
{
    Rect_004bfd60 r = *rect;
    int result;
    if (surface == 0) {
        Surface screen;
        result = LockScreen(&screen);
        if (result != 0) {
            if (ClipRectangle(&screen, &r))
                FUN_004cce87(&screen, &r, value);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipRectangle(surface, &r))
            FUN_004cce87(surface, &r, value);
        result = ((int*)&surface)[2];
    }
    return result;
}
