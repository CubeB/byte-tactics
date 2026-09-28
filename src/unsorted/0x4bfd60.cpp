// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Xor-fills `rect` in `surface` (or in the locked screen when `surface` is 0)
// with the byte `value`, using FUN_004cce87. The rect is copied to a local
// first because the clip helper FUN_004bf620 clips it in place.
//
// Best so far: 87.6 percent. Established and matching: the 0x30-byte surface
// the lock helper fills, the return of the lock result on the locked path and
// of `value` on the caller-surface path, the 3-argument __stdcall signature,
// the two exit blocks, and the whole locked path byte for byte.
//
// What still differs, and the one thing to attack first: in the else arm the
// original reloads `value` into esi after the blit call (0x4bfdfa:
// `mov esi, [esp+0x50]`), reusing the register the surface pointer occupied;
// this compiler instead hoists the load into edi before the blit test
// (`mov edi, [esp+0x54]`) and saves/restores edi, so the function is 2 bytes
// short and the failure `je` lands on the locked path's own epilogue instead
// of the shared one. The original never touches edi; its else tail is
// `mov esi, [esp+0x50]; mov eax, esi`.
//
// Tried, none of which moved it (all score 87.6 or lower, same edi usage):
// - an early `return locked;` versus a shared `int result;`, with `result`
//   assigned on both paths (0x4bf6f0, a matched sibling, does need two
//   registers because its else result is live across the blit; here the else
//   result is assigned after it, yet MSVC still preloads it)
// - reusing the `surface` parameter as the result variable
// - separate locals in each arm, address-taking `value`, comma-operator
//   variants, a static inline Fill helper returning value, do/while and goto
//   forms, and swapping the arms
// - the 0x4befe0 sibling's uninitialised-local trick does not apply: that
//   function's lock-failure path is what makes MSVC give the local a stack
//   slot (and hence no edi), while here the failure path returns the lock
//   result already in esi.

struct Rect_004bfd60 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Surface_004bfd60 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall FUN_004c5e70(Surface_004bfd60* out);
int __stdcall FUN_004c5fa0(Surface_004bfd60* s);
int __stdcall FUN_004bf620(Surface_004bfd60* s, Rect_004bfd60* r);
void __cdecl FUN_004cce87(Surface_004bfd60* s, Rect_004bfd60* r, int value);

// FUNCTION: 0x4bfd60
int __stdcall FUN_004bfd60(Surface_004bfd60* surface, Rect_004bfd60* rect, int value)
{
    Rect_004bfd60 r = *rect;
    if (surface == 0) {
        Surface_004bfd60 screen;
        int locked = FUN_004c5e70(&screen);
        if (locked != 0) {
            if (FUN_004bf620(&screen, &r))
                FUN_004cce87(&screen, &r, value);
            FUN_004c5fa0(&screen);
        }
        return locked;
    } else {
        if (FUN_004bf620(surface, &r))
            FUN_004cce87(surface, &r, value);
        return value;
    }
}
