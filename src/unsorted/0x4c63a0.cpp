// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by claude-sonnet-5-5. Names are provisional.
// Session claude-sonnet-5-5 (issue 4530, no code change, still 90.7%). NOTE: the `if ((held = Lock()) == 0)
// held = 0;` below is a steering construct (it makes MSVC allocate the branch-2 Lock as ebp/ebx/ebx and so
// merge the two Unlock bodies); without it the file is 85.1% (`LONG held = Lock();`, 1090 bytes). Tried
// without success, all at 85.1% or below: Unlock written with `held` in its stores (folded to literal 0),
// `held` initialised before the bmp checks (0, 1, d->field_dc), Unlock condition spelled `held <= 0`,
// `held < 1`, `!held`, Lock tests spelled `r <= 0` / `!(r > 0)`, Unlock(held) at every leaf of branch 2
// (not merged, 1316 bytes), and tools/permute.py 10 minutes (135 candidates) from the non-steered file.
// The allocation is a pure swap: in the original the group {WaitForSingleObject, held} gets ebx and
// InterlockedExchange gets ebp (as in branch 1, where {WaitForSingleObject, p} gets ebx); ours gives ebx to
// InterlockedExchange. Lead from 0x4c7580: a signed `> 0` compare is a different expression from `!= 0`
// to MSVC's tail merger even though it emits the same test; a spelling of held's use that is a surviving
// but code-free IL use (like that one) is the thing to look for.
// Session Space Bunny Free (issue 4447): 83.0% -> 90.7% (1055 bytes vs 1051), one structural
// difference left. Two findings, both new:
//  1. THE ALLOCATION TRIGGER. Earlier sessions correctly identified the blocker (branch 2's
//     Lock result lands in ebp instead of ebx, so the two Unlock bodies are not byte-identical
//     and MSVC cannot tail-merge them). Found the exact trigger: ANY second read or branch of
//     `held` inside branch 2 makes MSVC 5 allocate the Lock imports as
//     (InterlockedExchange, WaitForSingleObject, held) = (ebp, ebx, ebx), which is exactly the
//     original's. With that, both Unlock bodies compile to the same literal-0 sequence
//     (`push 0; push 0x52a4e8; mov [0x52a4ec],0; call ebp; ...`) and MSVC merges them, so
//     branch 1 ends in `mov eax,[esp+0x10]; test eax,eax; jmp <shared jne>` at 0x4c6470
//     instead of carrying its own copy of the body. That single change is worth ~34 bytes.
//     The cheapest trigger found is the two lines
//         LONG held;
//         if ((held = Lock()) == 0) held = 0;
//     where MSVC deletes the dead store but keeps the test, leaving a degenerate
//     `test ebx,ebx; jne <the next instruction>` (4 bytes) that the original does not have.
//     That is the ONLY remaining difference in this file; remove it and the function should
//     match. Everything below was tried and does NOT give the flip for free (all variants are
//     in build/scratch/0x4c63a0/, 200+ compiles): the same use far from the Lock works
//     (`if (d->field_1ce == 0x7fffffff) held = 0;` just before Unlock, but 8 bytes),
//     `switch (held = Lock()) { case 0x7fffffff: held = 0; }` works (8 bytes), and an empty
//     then-branch, a switch with no case, `(void)held`, `held = held`, `held += 0`, an
//     unconditional dead store, the `register` keyword, a goto, a label, lifting branch 1 or
//     branch 2 into an inline helper, permuting branch 2's declarations, eight Lock-helper
//     spellings (else chains, `!r`, a named constant, `return r`, a declared-outside r), the
//     two bmp-resolution checks reordered or merged into one `||`, the if/else-if/else
//     restructure and the branch-2-only Lock all leave the allocation at (ebx, ebp, ebp).
//     tools/permute.py, 15 minutes over 1269 candidates: no gain.
//  2. THE INLINED UnlockScreen SPELLING. 0x4c5fa0 is MATCHED (src/unsorted/0x4c5fa0.cpp) and
//     it unlocks through a method of an embedded screen struct, which is why the surface
//     pointer is re-read there: `struct Screen { ...; IDirectDrawSurface* surface;
//     void UnlockSurface() { surface->Unlock(0); } };`. Copying that shape here (with
//     `primary` at +0x08 so field_88 keeps offset 0x88) reproduces the original's exact
//     sequence `mov ecx,[eax+0x8c]; test ecx,ecx; je; mov eax,ecx; push 0; push eax;
//     mov ecx,[eax]; call [ecx+0x80]` in both inlined copies. Every other spelling tried (a
//     named local, no local at all, nested ifs, a helper taking the surface or the display, a
//     parameterised UnlockScreen, a void return, the surface test first, the counter guard
//     first) loads the surface straight into eax and loses the `mov eax,ecx`. This fixed
//     four of the wrong instructions.
// Frame and slots are unchanged and correct: 0xf4, with held/bmp/pt sharing 0x10, rect 0x18,
// src 0x28, out 0x38, screen 0x68, desc 0x98.
// For the record: build/scratch/0x4c63a0/u_x_cond0.cpp is this code WITHOUT the nested-screen
// UnlockScreen fix. It measures 1051 bytes and 91.3% by check.py (its four shorter instructions
// make difflib's text ratio happier) but has three differences instead of one, so the version
// kept here is the better starting point.
// Session claude-sonnet-5-5 (issue 4140 retry, no code change, still 83.0%): root cause found for the
// unmerged Unlock. Our branch-2 Lock allocates (iel,wfso,held) = (ebx,ebp,ebp) instead of (ebp,ebx,ebx),
// and our tail then substitutes the proven-zero held register (`push ebp`), so it differs from branch 1's
// literal-0 tail and cross-jumping cannot merge them. Variants tried with --sym, all worse or equal:
// a branch-2-only Lock spelled `LONG r; while(1){ r = IEx(); if (!r){ DAT=C; r=0; break; } if (DAT==C)
// break; Wait; } return r;` (r outlives the loop) DOES merge the tails and gives 1050 bytes with
// iel=ebp, but 75.5% because r becomes a loop-carried register (const moves to ebx, wfso is called
// through memory, zero register moves to edi); `Unlock(held == 0)` via a bool helper 76.5%; Lock via
// an out-parameter, `register`, held declared before bmp, path 3 written first, break/for/do forms of
// Lock, extra Unlock call sites per leaf (1171 bytes): all <= 83.0%. A function-scope `held` with one
// trailing Unlock and if/else-if/else bodies stores held to memory in both branches (frame 0xf8, 56.6%).
// tools/permute.py 15 minutes (234 candidates): no gain.
// PARTIAL 83.0% (1086 bytes vs 1051; all 35 left are the unmerged Unlock plus
// two register-name choices). Frame is right (0xf4: pt@0x10 reused by held/bmp, rect@0x18,
// src@0x28, out@0x38, screen@0x68, desc@0x98, all [esp+N]). The tail loop now has
// the original's shape: `int hr; for(;;){ hr = Blt(...); if (hr==0) return; if (hr
// != 0x887601c2) continue; dd = FUN_004b6220(); if (dd->field_44 == 0) { hr =
// field_88->Restore(); if (hr==0) { hr = surface->Restore(); if (hr==0) { ...;
// UnlockScreen(); } } } else { hr = 0; } if (hr != 0) continue; return; }` keeps
// hr in edi and dd in ebx exactly as the original (0x4c6722-0x4c67aa), and that
// alone took the file from 74.7 to 81.0.
// What still differs (ours 1129 bytes vs the original 1051):
//  - the two inlined Lock()/Unlock(held) pairs are not merged. The original keeps
//    ONE Unlock body (0x4c663f) and branch 1 jumps into it (mov eax,[esp+0x10];
//    test eax,eax; jmp 0x4c6639), and its bmp-path Lock() loads ebp=InterlockedExchange,
//    ebx=WaitForSingleObject with held ending in ebx; ours loads ebx=InterlockedExchange,
//    ebp=WaitForSingleObject and keeps held in ebp, so the two Unlock bodies differ
//    (call ebp vs call ebx) and MSVC emits both (~0x2c + 0x20 bytes).
//  - ours also repeats the whole Blt block once at the loop tail (the trailing
//    `if (hr != 0) continue;` back edge is not folded onto the loop top).
//  - branch 1's jne displacements are 4 bytes wider because of those extra bytes.
// The zero register now matches (xor ebp,ebp / cmp ecx,ebp in the field_dc test),
// but that did not move the bmp-path Lock's (iel,wfso,held) trio off (ebx,ebp,ebp).
// Tested this pass: making the Unlock ONE shared statement (if / else if / else
// plus a single trailing Unlock(held)) DOES unify the bmp Lock to the original's
// (ebp,ebx,ebx), which shows the original's shared Unlock body really is one
// source-level statement; but then held needs a function-wide memory home at
// [esp+0x10], the field_bc temp moves to 0x14 and the frame becomes 0xf8: 67.6%.
// Hoisting a single `LONG held;` while keeping the two Unlock call sites is
// byte-identical to this file. A do-while form of the Blt loop scores 41.3%.
// The locals must stay scoped as they are (desc/out inside the field_dc!=0 block,
// rect/pt plus the Blt loop in a nested block, screen at function scope): declaring
// them at function scope makes the frame 0xf8 and shifts every [esp+N] by 4.
// Session deepseek-v4.1-flash: no code change, 2 check runs, still 81.0%.
// New evidence from the 81.0% diff: our bmp-path Unlock emits `push ebp` and
// `mov [DAT_0052a4ec], ebp` (the register holding held, proven 0 by the test)
// where the original emits `push 0` / `mov [DAT_0052a4ec], 0`. The original's
// shared body also shows the `test held,held` duplicated into each predecessor
// (test eax,eax; jmp shared_jne vs test ebx,ebx; fall into shared_jne), which
// only happens when held lives in different places in the two branches (memory
// at [esp+0x10] in branch 1, ebx in branch 2) and the Unlock is ONE source
// statement after the if/else chain: the value propagation that turns the 0
// immediates into the held register then cannot fire, since the shared body
// has no single name for held. So the remaining shape is confirmed to be a
// single trailing if (held == 0) { unlock } over a function-scope held, but
// that spelling gives held a whole-variable home and pushes bmp to 0x14 (frame
// 0xf8). Not tried this session: a struct home (as in matched 0x4b5510's
// `struct { int lockResult; HDC* dcSlot; } setup`) to force held and bmp into
// one shared slot pair, or splitting held's live range some other way.
// Session deepseek-v4.1-flash (2nd), 81.0 -> 81.3: the full diff confirms the
// accounting. The unmerged branch-1 Unlock costs ~34 bytes (test+jmp 7 bytes in
// the original versus a whole body of ~41) and the duplicated Blt block at the
// loop tail costs ~44 (1129 - 1051 = 78). What fixed 0.3: the tail's
// `if (dd->field_44 == 0) {...} else { hr = 0; }` emitted the `xor edi,edi` at
// the bottom; writing it inverted, `if (dd->field_44 != 0) { hr = 0; } else
// {...restore chain...}`, gives the original's `cmp [ebx+0x44],ebp; je chain;
// xor edi,edi; jmp join` layout. Still open: MSVC rotates the for(;;) and
// appends a second full Blt block at the function end (the bottom `cmp edi,ebp`
// jumps to that copy, whose own `cmp eax,ebp; jne <restore chain>` returns to
// the chain); the original has one Blt block with every back edge jumping to
// it. Tried this session, all worse: `if (hr == 0x887601c2) {...}` guard around
// the whole chain (42.2%), goto-based loop with the label at the Blt (75.2%,
// zero register moves from ebp to ebx), goto-based shared cleanup label (67.6%,
// frame 0xf8 and bmp at 0x14), do/while tail (41.3%), early-exit chain
// `if (field_44 != 0) return;` plus `if (hr != 0) continue;` (75.2%),
// `if (hr == 0) return;` and hr declared inside the loop (both byte-identical
// to this file, 81.3%).
// Session deepseek-v4.1-flash (2nd, cont.), 81.3 -> 83.0: `for (;;)` made MSVC
// rotate the loop and append a whole second Blt block at the function end;
// `while (1)` with the identical body emits the single Blt block the original
// has, and that alone took the file to 1086 bytes. The tail now reads
// `while (1) { hr = Blt(...); if (hr == 0) return; if (hr != 0x887601c2)
// continue; dd = FUN_004b6220(); if (dd->field_44 != 0) { hr = 0; } else
// { ...restore chain... } if (hr != 0) continue; return; }`. Only the unmerged
// Unlock (35 bytes: branch 1 should end in `test eax,eax; jmp <shared jne>`),
// the bmp-path Lock register trio (ours ebx/ebp/ebp, original ebp/ebx/ebx) and
// the UnlockScreen inline's surface register (ours loads `[eax+0x8c]` into eax,
// the original into ecx) are left.

// Session deepseek-v4.1-flash (4th, edited by deepseek-v4.1-flash): ctx.py shows the
// original branch 2 Lock keeps the 0x4d41494e constant in EDI (mov edi,0x4d41494e at
// 0x4c64b6), InterlockedExchange in ebp (0x4c64aa) and WaitForSingleObject in ebx
// (0x4c64b0), with held ending in ebx (`mov ebx,eax` at 0x4c64e5, `xor ebx,ebx` at
// 0x4c64e1); branch 1's held sits in memory at [esp+0x10] at 0x4c646a, the SAME slot
// branch 2 stores bmp into at 0x4c648b (`mov [esp+0x10],edi` with edi=d->field_bc),
// which is why a function-scope held grows the frame to 0xf8. Also tried writing the
// inlined UnlockScreen surface as an explicit `IDirectDrawSurface* surface` local:
// byte-identical (1086 bytes, 83.0), so the [eax+0x8c] eax-vs-ecx load is not the
// local's spelling. Score unchanged at 83.0, 1086 bytes.
// Session deepseek-v4.1-flash (5th, timebox): re-ran check.py twice, no source change, still
// 83.0 percent / 1086 bytes. Branch 1 is confirmed byte-identical apart from the jne
// displacement: ours already emits `mov eax,[esp+0x10]; test eax,eax; jne skip; <body with
// literal 0 stores>`, and the original reaches the SAME body from branch 2 via the shared
// `jne` at 0x4c6639, so the only thing keeping the two sites apart is that branch 2's Unlock
// still stores/loads `ebp` (a register proven 0) instead of the literals, which in turn is
// the ebp/ebx role swap in branch 2's inlined Lock. No new spelling tried: any variant that
// forces held into a function-scope home was already measured at 67.6 percent (frame 0xf8).
// Session deepseek-v4.1-flash (6th, timebox): re-ran check.py once, no source change, still
// 83.0 percent / 1086 bytes. Confirmed from the disassembly that the original shared Unlock
// body at 0x4c663f uses the literal immediates (`push 0`, `mov [0x52a4ec],0`) precisely
// because it has two predecessors (branch 1 memory test at 0x4c6637 region and branch 2
// ebx test at 0x4c6637), which blocks the known-zero-register peephole; ours keeps two
// separate bodies so each single-predecessor body substitutes the proven-zero register
// (`push ebp` / `mov [0x52a4ec], ebp`), and the two bodies are then not identical enough
// to be unified. The only shape that would unify them is the single shared Unlock statement
// already measured at 67.6 percent (frame 0xf8) because held then cannot share slot 0x10
// with the branch-2 bmp temp. No new variant attempted in this session.
// Session deepseek-v4.1-flash (7th, timebox): retried the merged shape with held scoped to a
// block that closes before the Blt path (`{ LONG held; if/else if/else goto skip_unlock;
// Unlock(held); return; } skip_unlock:`): the compiler DOES merge the two unlock paths into
// one body, 1086 -> 1061 bytes, but held then needs its own live home at 0x10 (bmp moves to
// 0x14, frame grows to 0xf8) because held and bmp are simultaneously live in branch 2, so
// 69.4 percent; reverted to this file.
// Session deepseek-v4.1-flash (3rd): `for (;;)` in the inlined Lock() loop regresses
// 83.0 to 69.1 (1086 -> 1118 bytes: the loop rotates and the import-pointer registers
// move), so `while (1)` in Lock() stays; reverted, 83.0 reconfirmed. Remaining diffs
// unchanged: unmerged branch-1 Unlock, bmp-path Lock register trio (ours ebx/ebp/ebp,
// original ebp/ebx/ebx) and the `[eax+0x8c]` load register in UnlockScreen (ours eax,
// original ecx).
//
#include <windows.h>
#include <ddraw.h>

struct Surface_004c63a0 {
    int data[12];
};

struct Out_004c63a0 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int pad[8];
};

#pragma pack(push, 1)
struct Screen_004c63a0 {
    char unknown_0[0x8];
    IDirectDrawSurface* primary;       // +0x08
    IDirectDrawSurface* surface;       // +0x0c
    char unknown_10[0x18 - 0x10];

    void UnlockSurface() { surface->Unlock(0); }
};

struct Display_004c63a0 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
    int field_44;                      // +0x44
    HDC srcDC;                         // +0x48
    HPALETTE palette;                  // +0x4c
    Out_004c63a0 cached;               // +0x50
    Screen_004c63a0 screen;            // +0x80
    Surface_004c63a0* field_98;        // +0x98
    int field_9c;                      // +0x9c
    char unknown_a0[0xbc - 0xa0];
    Surface_004c63a0* field_bc;        // +0xbc
    char unknown_c0[0xd4 - 0xc0];
    int field_d4;                      // +0xd4
    int field_d8;                      // +0xd8
    int field_dc;                      // +0xdc
    char unknown_e0[0xf0 - 0xe0];
    unsigned short flags;              // +0xf0
    char unknown_f2[0x196 - 0xf2];
    int field_196;                     // +0x196
    char unknown_19a[0x1b2 - 0x19a];
    Surface_004c63a0* field_1b2;       // +0x1b2
    int field_1b6;                     // +0x1b6
    int field_1ba;                     // +0x1ba
    int* field_1be;                    // +0x1be
    char unknown_1c2[0x1ce - 0x1c2];
    int field_1ce;                     // +0x1ce
    int field_1d2;                     // +0x1d2
};
#pragma pack(pop)

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;
extern int DAT_0051fe00;

Display_004c63a0* FUN_004b6220(void);
int FUN_004b6700(void);
int FUN_004b6710(void);
int __stdcall FUN_004c5e70(Surface_004c63a0* out);
void __stdcall FUN_004c6b70(Surface_004c63a0* dst, Surface_004c63a0* bmp, int x, int y);
void __stdcall FUN_004c67c0(Display_004c63a0* obj, void* dst);
void __cdecl FUN_004cbbe0(Surface_004c63a0* dst, Surface_004c63a0* src, int x, int y);

static inline LONG Lock()
{
    while (1) {
        LONG r = InterlockedExchange(&DAT_0052a4e8, 0x4d41494e);
        if (r == 0) {
            DAT_0052a4ec = 0x4d41494e;
            return 0;
        }
        if (DAT_0052a4ec == 0x4d41494e)
            return r;
        WaitForSingleObject(DAT_0052a4f0, INFINITE);
    }
}

static inline void Unlock(LONG held)
{
    if (held == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
}

// 0x4c5fa0, inlined here.
static inline int UnlockScreen()
{
    Display_004c63a0* d = FUN_004b6220();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (DAT_0051fe00 > 0)
            DAT_0051fe00--;
    }
    return 1;
}

struct Desc {
    DWORD dwSize, dwFlags, height, width;
    LONG lPitch;
    DWORD backbuffers, mipmaps, alpha, reserved;
    void* lpSurface;
    char fields[68];
};

// FUNCTION: 0x4c63a0
void FUN_004c63a0(void)
{
    Display_004c63a0* d = FUN_004b6220();
    unsigned short flags = d->flags;

    if ((flags & 2) == 0) {
        LONG held = Lock();
        Out_004c63a0* p = &d->cached;
        FUN_004c6b70((Surface_004c63a0*)p, d->field_bc, 0, 0);
        FUN_004c67c0(d, p);
        HDC hdc = GetDC(d->hwnd);
        SelectPalette(hdc, d->palette, 0);
        RealizePalette(hdc);
        BitBlt(hdc, 0, 0, p->field_0, d->cached.field_4, d->srcDC, 0, 0, SRCCOPY);
        ReleaseDC(d->hwnd, hdc);
        Unlock(held);
        return;
    }

    Surface_004c63a0 screen;

    if (d->field_dc != 0) {
        Desc desc;
        Surface_004c63a0 out;
        Surface_004c63a0* bmp = d->field_bc;
        if (bmp->data[0] != FUN_004b6700())
            return;
        if (bmp->data[1] != FUN_004b6710())
            return;

        // The test reads the lock result again straight after the Lock; that read is
        // what makes MSVC give the Lock result ebx (see the header notes), which is in
        // turn what lets it tail-merge the two Unlock bodies. The assignment in the
        // then-arm is dead and MSVC drops it, leaving only the test.
        LONG held;
        if ((held = Lock()) == 0)
            held = 0;
        desc.dwSize = sizeof(desc);
        unsigned long lr = d->screen.primary->Lock(0, (DDSURFACEDESC*)&desc, 1, 0);
        if (lr == 0) {
            out.data[0] = d->field_d4;
            out.data[1] = d->field_d8;
            out.data[2] = desc.lPitch;
            out.data[3] = (int)desc.lpSurface;
            FUN_004c67c0(d, bmp);
            FUN_004cbbe0(&out, bmp, 0, 0);
            if (d->field_1ce != 0 && d->field_1d2 != 0)
                FUN_004c6b70(bmp, (Surface_004c63a0*)d->field_1be, d->field_1b6, d->field_1ba);
            d->screen.primary->Unlock(0);
        } else if (lr == 0x887601c2) {
            Display_004c63a0* dd = FUN_004b6220();
            if (dd->field_44 == 0) {
                if (d->screen.primary->Restore() == 0) {
                    if (d->screen.surface->Restore() == 0) {
                        FUN_004c5e70(&screen);
                        FUN_004cbbe0(&screen, dd->field_98, 0, 0);
                        UnlockScreen();
                    }
                }
            }
        }
        Unlock(held);
        return;
    }

    if (d->field_9c != 0 && (flags & 1) != 0) {
        d->screen.primary->Flip(0, 1);
        return;
    }

    {
    RECT rect;
    POINT pt;
    GetClientRect(d->hwnd, &rect);
    pt.x = 0;
    pt.y = 0;
    RECT src = rect;
    ClientToScreen(d->hwnd, &pt);
    OffsetRect(&rect, pt.x, pt.y);

    int hr;
    while (1) {
        hr = d->screen.primary->Blt(&rect, d->screen.surface, &src, 0x1000000, 0);
        if (hr == 0)
            return;
        if (hr != 0x887601c2)
            continue;
        Display_004c63a0* dd = FUN_004b6220();
        if (dd->field_44 != 0) {
            hr = 0;
        } else {
            hr = d->screen.primary->Restore();
            if (hr == 0) {
                hr = d->screen.surface->Restore();
                if (hr == 0) {
                    FUN_004c5e70(&screen);
                    FUN_004cbbe0(&screen, dd->field_98, 0, 0);
                    UnlockScreen();
                }
            }
        }
        if (hr != 0)
            continue;
        return;
    }
    }
}