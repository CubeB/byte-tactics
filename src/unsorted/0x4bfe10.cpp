// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and claude-opus-5-5, verified by GPT-6.1-sol, finished by deepseek-v4.1-flash. Names are provisional.
// GPT-6.1-sol issue 3121 retest: two completed checks kept 74.9% (265/267).
// A success-convention rewrite did not improve the retained source; no MATCH.
// #1705 retry by Codex / GPT-6.1-sol: check.py reconfirmed 74.9% (265/267 bytes), no MATCH.
// A success-convention rewrite failed to compile due malformed edit formatting; the saved best was restored.
//
// #2930 retry by deepseek-v4.1-flash: reconfirmed 74.9 percent (265/267), no MATCH.
// This retry exhaustively re-checked that the missing 2 bytes are not reachable
// from the source text. Scored (check.py --sym, so not counted): 8 spellings of
// the two destination sums (pixels-first, left-first, parenthesised groupings,
// pointer arithmetic with int and unsigned char* dst prototypes); pitch, pixels,
// h, w and dst as locals; a `static int` offset helper inlined in both arms;
// rect-field access through a `Rect*` local; `screen` declared at function scope;
// `r = *rect` as copy-init vs assignment; `(r.bottom + 1) - r.top` and
// `(r.right + 1) - r.left`; `pitch * top` vs `top * pitch`; a single
// uninitialised `result` with no `status`; and the 0x4bfd60 codegen probes
// `((int*)&surface)[1]` (same as status) and `((int*)&rect)[0]` (271 bytes,
// 59.7). Every expression, type and spelling variant produced a byte-identical
// 265-byte object (md5 9d113e3576254ce02129c141038045d3). Only changing which
// stack slot the else arm reads moves the object, and neither probe reaches the
// original. Conclusion: the eax/ecx/edx role split and the
// `add eax,[screen.pixels]` fold are compiler state from the original
// translation unit, not a source choice (docs/agent-guide.md, "Operand order
// that nothing changes" and the vector::insert register-state note). A later
// attempt should focus on a different spelling of the else-arm return value,
// which is the only lever found; `((int*)&rect)[0]` proves that spelling can
// move the whole allocation.

// Translates every pixel of `rect` in `surface` through the byte table at
// g_game+0xcc (FUN_004cced5), or in the locked screen (FUN_004c5e70 /
// FUN_004c5fa0) when `surface` is 0. The rect is copied to a local first
// because the clip helper FUN_004bf620 clips it in place. The locked path
// returns the lock result.
//
// Best so far: 74.9 percent. Everything outside the two FUN_004cced5 argument
// blocks matches: the frame, the late `push esi` after the flag test's early
// return, both exits and the shared `mov eax, esi` tail.
//
// Suspected original bug: the caller-surface path returns an uninitialised
// local (`status`, warning C4700). `rect` is dead after the copy, so MSVC 5
// puts `status` in rect's parameter slot, which is why the original ends that
// path with `mov esi, [esp+0x50]` and in practice returns the rect pointer.
// Returning `(int)rect` itself instead keeps rect alive to the end, and MSVC
// then holds it in ebx (the extra `push ebx` of the earlier attempts).
// `((int*)&surface)[1]` (the 0x4bfd60 spelling) compiles the same, but the
// uninitialised local is the plainer explanation.
//
// The other key: ONE `return result;` at the end, as in the siblings
// 0x4be950 and 0x4bfd60. The lock-success path's separate epilogue in the
// original is MSVC's tail duplication, not a second `return`; writing that
// `return` explicitly moves `push esi` to the top (66.3 percent).
//
// deepseek-v4.1-flash retry (#2558): reconfirmed 74.9 percent (265/267).
// Calling convention is right (both ends `ret 8`, __stdcall, mangled YGH).
// The lone ordering difference is that MSVC hoists the two r.bottom / r.top
// stack loads above the `mov eax, [edi+0xcc]` table load in the locked arm
// (which then drags the eax/ecx/edx roles with it). Statement-level locals
// for table/h/w and every argument spelling tried here did not move it.
// What still differs: only register choice and load order inside the two
// argument blocks. Else arm: the original loads r.top before pushing
// field_cc and r.bottom after (so field_cc goes in edx), ours loads r.bottom
// first (field_cc in eax). Locked arm: the original evaluates strictly right
// to left (field_cc, then bottom/top, right/left, then pitch and pixels in
// registers), ours hoists the height loads above the field_cc load and folds
// pixels as a memory operand (2 bytes short). None of these changed a single
// byte: every spelling of width, height and dst (including parenthesised
// pointer offsets); w/h/dst as locals in all orders; pointer, int, unsigned
// and long types for dst, pixels, pitch, the rect fields and the table;
// variadic, extern "C" and int-returning prototypes for FUN_004cced5;
// inline blit helpers taking (surface, rect, table) or (surface, rect, game);
// the header sets from tools/headers.py; and the flags /G3 to /G5, /Ox, /Ob1,
// /Oa, /Ow and /Op (diagnosis only). Helpers taking the pixel pointer as a
// scalar parameter (69.1) or doing the clip inside (57.3) are worse, and
// `short` argument types change the loads themselves. FUN_004cced5
// (hand-written assembly) has no other callers to copy the spelling from.

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
    int result;
    int status;
    if (surface == 0) {
        Surface_004bfe10 screen;
        result = FUN_004c5e70(&screen);
        if (result != 0) {
            if (FUN_004bf620(&screen, &r))
                FUN_004cced5(r.top * screen.pitch + r.left + (int)screen.pixels,
                             screen.pitch, r.right - r.left + 1,
                             r.bottom - r.top + 1, g->field_cc);
            FUN_004c5fa0(&screen);
        }
    } else {
        if (FUN_004bf620(surface, &r))
            FUN_004cced5((int)surface->pixels + r.top * surface->pitch + r.left,
                         surface->pitch, r.right - r.left + 1,
                         r.bottom - r.top + 1, g->field_cc);
        result = status;
    }
    return result;
}
