// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and claude-opus-5-5, verified by GPT-6.1-sol, finished by deepseek-v4.1-flash. Names are provisional.
//
// space-bunny-free retry (#4469): reconfirmed 74.9 percent (265/267), no MATCH. The
// score is decided by WHICH Surface field sits in the locked arm's multiply, not by
// its value. Four shape classes; for each, all six orders of the three addends and
// the parenthesised, unsigned, long and pointer spellings give the same object:
//   mul by pitch (+8),  addend pixels (+0xc)   74.9%  265 bytes  <- the correct value
//   mul by pitch,       addend pitch           93.3%  262 bytes  (C1 folds x*p + p)
//   mul by pixels,      addend pitch           94.9%  267 bytes  <- best score, right size
//   mul by pixels,      addend pixels          92.3%  264 bytes
// The 94.9% shape (dst = `r.top * (int)screen.pixels + r.left + (int)screen.pitch`) is
// the closest to the original found so far: its size is the original's 267 bytes and
// EVERY instruction outside the locked arm's dst matches, the whole else arm included.
// So the frame, the rect copy, the clip/unlock calls, the locked-screen path, the tail
// and the else arm are settled; only the locked arm's dst arithmetic is open. What is
// left in it, against the original:
//  (1) the multiply accumulates in the register that already holds r.top
//      (`imul eax,edx`, eax = top, loaded early); ours accumulates in the register
//      that holds the pitch (`imul eax,ecx`, eax = pitch, loaded late), which forces
//      `push pitch` BEFORE the imul where the original pushes it after;
//  (2) the original loads screen.pixels into edx and adds it as a register
//      (`mov edx,[esp+0x34]; add eax,edx`); we fold the load into the add
//      (`add eax,[esp+0x34]`). That is exactly the 2-byte size difference, and it
//      means the original's pixels NT is not a single-use fusable load;
//  (3) the two adds come out (left, pixels) in the original and (pixels, left) in
//      ours, although both spellings give one and the same tree.
// The original's order is C1's plain right-to-left argument walk with nothing moved
// (table, bottom, top, right, left, pitch, pixels); MSVC hoists our two height loads
// above the table load. Changing the shape of the dst (which field is in the multiply)
// flips the allocation of the WHOLE function, else arm included, so the original's
// allocation is reachable from the source, but only through the four classes above
// and none of them computes the right value. Both the target allocation and the
// unfused pixels load appear together in every wrong-value shape that reaches them.
// New negative results this retry, ~110 spellings, all 74.9% and byte-identical to the
// retained source unless stated: the pointer forms `(int)(screen.pixels + ...)` and
// `(int)((unsigned char*)screen.pixels + r.top * screen.pitch) + r.left`; the callee's
// first parameter typed unsigned char*/char*/void* with a pointer expression and
// unsigned short* (wrong code); `screen.pixels[r.top * screen.pitch + r.left]`
// (77.2% but 270 bytes: it reads the dst as a char, so not kept); unsigned/long casts
// on every term, unsigned Rect fields, an unsigned pitch, an int-typed pixels field
// (so no cast at all); neutral wrappers (1*x, x*1, 0+x, x+0, x-0, x*1/1, ~~x);
// fold-blocking forms x*(p+px-px), (x*p+l)*1+px, x*p+l+px+0*x*p, x*p+l+(px^0);
// correct-semantics forms that repeat an addend so C1 could share its NT
// (px+px-px, px+0*x, x*p+l+px+p-p, x*p-p+l+px+p, (x+1)*p+l+px-p, (x-1)*p+l+px+p,
// x*p+p+l+px-p): C1 folds all of them before the load-fusing decision, so the pixels
// load stays fused; locals for the pitch, pixels, table, w, h, dst, a Surface* and a
// Rect* inside the locked arm, including all five arguments precomputed into
// register locals (74.9, identical object), dst as a local 65.7, pitch 67.3, pixels
// 63.3, table 71.8, h or w 67.0-67.7; and the `1 *`/`+ 0`/`* 1` no-op forms on every
// term. A permuter run (--minutes 12 --stall 8 --jobs 4) stopped on the stall with an
// empty best.diff: no gain over 74.9%. Swapping the two arms' destination spellings,
// and reading every argument through a Rect* or Game* local, are also 74.9%.
// 30-minute checkpoint (space-bunny-free #4469): best is still the retained source at
// 74.9%; the x1 shape (94.9%, wrong value) is recorded above as a probe, not kept.
// Next lead: the original does not fuse the pixels load, so find a value-preserving
// source where screen.pixels is read twice in the locked arm (or where the addend is
// a register NT), since every way of doing that tried here is folded away first.

// claude-opus-5-5 (#4406): still 74.9%. The screen-branch destination adds screen.pixels
// from memory where the original loads it into edx first; every operand order, char*
// pointer arithmetic and a shared inline Shade(Surface*, Rect*, Game*) helper compile
// to identical code. A 10-minute permuter run (264 candidates) found nothing.
//
// #3306 retry by deepseek-v4.1-flash: reconfirmed 74.9 percent (265/267), no MATCH.
// This retry found the lever the #2930 note below says does not exist: the whole
// function's eax/ecx/edx allocation IS reachable from the locked arm's source,
// but only through expressions with wrong semantics, so none is retained. Probes
// (scored with a scratch driver, sources in build/scratch/0x4bfe10/):
//   w2: locked-arm dst written as `r.top * screen.pitch + r.left + screen.pitch`
//       (adds the pitch instead of the pixels) scores 93.3 percent. MSVC's C1
//       factors `top*pitch + pitch` into `(top+1)*pitch`; that factored tree
//       flips the C2 register allocation for the WHOLE function to the original's
//       (table load first, bottom in ecx, top in eax, left in ecx, pitch in edx),
//       and the else arm then matches byte for byte. The only remaining
//       difference is the factored arithmetic (extra `inc eax`, no pixels load).
//   z10: dst `r.top * (int)screen.pixels + r.left + (int)screen.pixels` scores
//       92.3 percent, same mechanism (factors top*pixels + pixels).
//   a3 (`+ r.top`) 91.3, b4 (`+ (int)screen.pitch`) 93.3, a1/a2/a6 (last addend
//       r.bottom / r.right / g->field_cc) 61 to 66, constants 75 to 78.
//   So the good allocation appears exactly when the locked arm's last addend is
//   one of the multiply's operands, i.e. when C1 can factor the dst.
// The correct dst `top*pitch + left + pixels` cannot factor, and every correct
// spelling tried keeps 74.9 (see the long lists below and the #2930 note).
// Next lead: an algebraically equal spelling of the locked-arm dst that still
// makes C1 factor (or otherwise reproduce the factored tree) without changing
// the value, for example a form where `pixels` participates in the multiply.
// Note also that more than the two call setups differ: the unlock call's `lea`
// register (original eax, ours ecx) and the else arm's clip `lea` (original ecx,
// ours edx) are off too, i.e. the whole function's allocation is shifted, and
// the w2 probe fixes all of those at once.
// New negative results this retry: dst spellings through `*(int*)((char*)&screen
// + 8)`, `((int*)&screen)[2]`, `*(&screen.pitch)` and comma/cast/no-op forms;
// `(r.top + 1) * screen.pitch - screen.pitch` forms (C1 cancels them back to the
// plain tree); `int pitch/px` and `unsigned char* px` locals inside the if;
// table spelled nine ways (casts, `*(int*)((char*)g + 0xcc)`, `+ 0`, `| 0`,
// `* 1`); including stdio/string/math/stdlib/time/mmsystem/ddraw headers; 1 to
// 50 extra extern/function/class declarations before the function; the Surface
// struct's field declaration order swapped (with the call rewritten to keep the
// same machine code); the rect copy as memcpy / four field stores / copy-ctor;
// reference parameters; screen as an array, at function scope, or through a
// local pointer; the else arm's return as `*(int*)&rect` (271 bytes, 59.7),
// `*(int*)((char*)&surface + 4)` (74.9) and `*(int*)&surface` (259 bytes, 72.5);
// `int status = 0;` (73.8), 36 h/w spellings; and the flags/if spellings.
//
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