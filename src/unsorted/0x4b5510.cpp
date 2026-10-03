// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by DeepSeek V4.1 Flash, checked by GPT-6. Names are provisional.
// Codex GPT-6 retry for #5208 (2026-10-03): reusing the later hdc local
// for the early slot copy adds 4 bytes and scores 85.1%; the existing
// 99.7% source remains best.
// GPT-6 retry (#4917): current check.py confirms 99.7% / 1017 bytes. The
// documented live pointer-copy probe moves the reload but loses the EBP zero
// register used by all DirectDraw result checks, dropping to 62.2%.
// Reconstructed the saved I/T lead: a plain HDC *slot with `(void **)&caps`
// gives the documented single `lea ecx, [esp + 0x1c]` mismatch, but writes the
// DIB bits into `caps` rather than the slot used by the original. Adding the
// documented dead `pad2 = mode` assignment leaves that output unchanged, so
// the semantically correct 99.7% source below remains the best usable version.
// DeepSeek V4.1 Flash session (2026-10-02): permuter 3 min on this file's
// shape and 3 min on the T shape (plain `HDC *slot` at 0x14 with the
// CreateDIBSection arg `(void **)&caps`), no gain, both 99.672131 exact.
// Confirmed the two residuals are mutually exclusive on this compiler: the
// block-head reload `mov edx, [esp+0x14]` needs [esp+0x14] to be a plain
// (non-address-taken) local, while the CreateDIBSection `lea ecx, [esp+0x18]`
// needs an address-taken local at [esp+0x14] (the compiler always places the
// `&local` argument temp at local+4, so a local at 0x14 gives 0x18). One local
// cannot be both, and a separate `void *bits` at 0x18 grows the frame to
// 0x4d4 because MSVC 5 does not overlap it with the dead `caps` slot. Best
// version stays the 99.7 file shape below.
// 2026-10-01 deepseek-v4.1-flash 10-minute retry: still exactly 99.7%
// (1017 = 1017), the same single 3-instruction scheduler rotation at
// 0x4b55af. New measurements this session, all inert unless noted:
//   * compiler flags already known to be inert re-confirmed (/Oy, /Ot,
//     /Ob1, /Gd, /GB, /Og /Oi /Gy, /Gs, /Gf, /G5, /QAieee all 99.7);
//     /Oa drops to 61.4 (1002 bytes), /Ow to 88.3 (1008 bytes) and /G6
//     to 78.8 (1012 bytes), all too aggressive to be the original flags.
//   * a real `void *bits` local for CreateDIBSection, declared at its
//     point of use, still adds a frame slot (frame 0x4d0 -> 0x4d4) and
//     scores 91.8; the address-taken home plus the &-temp are two slots
//     whether bits is declared at the function top or at the call.
//   * plain `int lockResult; HDC *dcSlot;` locals (both declaration
//     orders) swap the homes to lockResult 0x14 / dcSlot 0x10 and score
//     97.4, so the struct wrapper really is required to pin 0x10/0x14.
//   * a scalar `HDC *pdc = &cleanup->dc;` used for the check, the DeleteDC
//     argument and the store, with the CreateDIBSection output going to a
//     separate address-taken local, scores 91.8 for the same frame growth:
//     the extra spilled variable adds a slot and shifts every [esp+N].
// The reload stays glued to the store in every shape whose frame layout is
// correct, so it is the same list-scheduler tie-break the guide records for
// 0x4b6570. Leaving the best (99.7) version in place.
// TIMEBOXED RETRY (deepseek-v4.1-flash): still 99.7% (1017 = 1017), the one
// rotation at 0x4b55af remains: original is load,h,d,push6,store; ours is
// h,d,load,push6,store. New finding from the c/h/d ordering experiment
// (scratch v1.cpp, free --sym diff): with the dc store first in the source
// the list becomes load,push6,store,h,d. Together with the baseline
// (h,d,load,push6,store) this shows `push 6` always lands immediately BEFORE
// the `mov [edx],ebp` store at that store's tree slot, and every other tree
// keeps its source order. So the original's tree order is [load, h, d, store]
// with the load a SEPARATE tree at the block head. Scoring was cut short:
// build/scratch/0x4b5510/v3..v6.cpp (named copy `HDC *slot = setup.dcSlot;`
// placed before/between/after the handle stores, and with the handle stores as
// one chain tree) were written but NOT scored when the timebox fired; v4 is
// the most promising untested shape (trees [load, HD, store], expected load,
// push6, store... verify before trusting). The named copy needs EDX coloring;
// every prior spelling got EAX and recolored the SetWindowPos block (96.4).
// Last session (deepseek-v4.1-flash retry, timebox cut short): still 99.7%,
// same single rotation at 0x4b55af (reload of [esp+0x14] must sit at the block
// head above the hpalette/dib stores). New idea formed but NOT yet scored: the
// reload may be spill-code placement of a named `HDC *pdc = &cleanup->dc;` live
// across the three DeleteObject calls (all four callee-saved registers are
// taken, so pdc spills to [esp+0x14] and the spill pass may reload at the top
// of the block, not at the use). Scratch files build/scratch/0x4b5510/va.cpp
// and vb.cpp hold that shape (pdc used at the if-check, the DeleteDC arg and
// the store) but the timebox fired before scoring. If a later session retries,
// score va.cpp first with --sym; if it fixes the rotation it should be 100.
// deepseek-v4.1-flash retry (2026-09-30): confirmed 99.7 is the ceiling for
// this shape. Re-scored the three statement orders cleanly in isolated files
// (c/h/d, h/c/d, d/c/h) and all are 99.3, so no permutation reaches the
// original load,h,d,push6,store. Plain (non-struct) locals for lockResult and
// dcSlot move the homes to 0x10/0x14 and the &temp to 0x14, so the struct
// wrapper is required for the 0x14/0x18 slot pair. Nothing new beat 99.7.
// deepseek-v4.1-flash session, still 99.7 (1017 bytes), only the 0x4b55af
// reload rotation. New shapes tried, all inert at 99.7 unless noted:
//   * a `static HDC& DcRef(Display*)` accessor for the dc store, and
//     `HDC& dc = cleanup->dc; ... dc = 0;` (85.1, reference recolors);
//   * `*(HDC*)(void*)setup.dcSlot = 0`, `setup.dcSlot[0] = 0` and
//     `*(setup.dcSlot + 0) = 0` (all identical to the plain store);
//   * a two-store `static inline ClearHandles(Display*)` for hpalette+dib with
//     the dc store after it (99.7) or before it (99.3);
//   * arguments-as-side-effect helpers `ZeroSlot(dib=0, h=0, slot)` /
//     `ZeroSlot(h=0, d=0, slot)` with the slot last so MSVC evaluates it first
//     (48.8, helper not inlined the way expected) and `ZeroSlot(slot, h, d)`
//     (92.1);
//   * a separate `void* bits` local so setup.dcSlot is not the address taken
//     by CreateDIBSection (99.7, same rotation);
//   * a plain `HDC* slot = &cleanup->dc;` used for DeleteDC and the store
//     (88.9, frame/coloring shifts);
//   * `{ ... }`, `if (1) { ... }` and `HDC** p = &setup.dcSlot; **p = 0;`
//     wrappers (all 99.7); a one-iteration for loop (93.6);
//   * the whole real preceding source-file functions (0x4b52e0, 0x4b5330,
//     0x4b5370) compiled above ours to change compiler state (99.7, no flip).
// The load is glued to the store statement and MSVC 5 never hoists it above
// the two handle stores; every named-local spelling lands in eax and recolors
// the SetWindowPos argument block. Same tie-break family the guide records for
// 0x4b6570, so this run stopped there rather than burning more shapes.
// Partial: 99.7%. Byte count is exact (1017 = 1017) and every instruction is
// the right one, except one two-slot scheduler rotation at 0x4b55af explained
// at the bottom of this file. The first of the two old scheduler hunks is
// fixed: writing the field_9c store as `dd->field_9c = 0` (through the
// &d->draw pointer) instead of `DAT_0051fbd0->draw.field_9c = 0` puts
// `lea ebx, [esi + 0x84]` before the `push ecx`, 99.3% -> 99.7%.
// The original holds the constant 0 in a callee-saved register (ebp) and tests
// every HRESULT against it with `cmp eax, ebp`, so all ten DirectDraw call
// results go through one named `HRESULT hr` local (see docs/agent-guide.md on
// 0x4b6880).
// deepseek-v4.1 new experiments on the remaining 0x4b55af rotation (all 1017
// bytes, all with the identical 3-instruction diff unless noted):
//   - `*setup.dcSlot = cleanup->dib = cleanup->hpalette = 0;` (chain reversed
//     so the hpalette store comes first) keeps the correct h/d store order but
//     still leaves the reload after them, 99.7, same as the plain three
//     statements; the chain with hpalette outermost emits d then h, 99.3.
//   - Putting the side effects in the RHS comma, `*c = (h = 0, d = 0, 0)`,
//     `*(c) = (...)` and `c[0] = d = h = 0` are all byte-identical to the plain
//     form: MSVC5 always evaluates the RHS before the LHS address, so the
//     reload can never be hoisted from the same statement.
//   - Writing the store first (`*c = 0, h = 0, d = 0;` or c/h/d statement
//     order) does put the reload at the top of the block, but then the store
//     is scheduled right after `push 6` and the two handle stores are pushed
//     to the end (99.3), so the reload is not the only thing that has to move.
//   - A separate `HDC *slot = setup.dcSlot;` before the two handle stores does
//     give the wanted list order, but the local is colored eax (never edx) and
//     the whole SetWindowPos block recolors, 96.4 (same with `HDC *const`, a
//     declaration hoisted to the function top, or an assignment instead of an
//     initializer). Only the address-taken frame slot reload, whose scratch
//     register the code generator picks (edx), keeps the tail intact.
//   - Dead or self-cancelling statements before the stores (`(void)setup.dcSlot;`,
//     `if (setup.dcSlot);`, `setup.dcSlot = setup.dcSlot;`,
//     `setup.dcSlot = setup.dcSlot + 0;`) are all removed by the optimizer and
//     leave the baseline 99.7 bytes and diff; the other two statement orders
//     (h/c/d, d/c/h) put the reload in the right place but move one handle
//     store, 99.3, and c/d/h keeps the reload first while pushing both handle
//     stores after the `push 6`, also 99.3.
//
// space-bunny-free session: the remaining diff is a scheduler tie and the file
// stays at 99.7 (1017 = 1017, one 3-instruction rotation). New measurements,
// all in build/scratch/0x4b5510 (free --sym scores, one real check.py run):
//   - `HDC *slot = setup.dcSlot;` immediately before the two handle stores and
//     `*slot = 0;` after them (the copy that does put the reload at the block
//     head) collapses to 62.2% and grows the function to 1024 bytes. The extra
//     live graph node demotes the constant 0 out of EBP (docs/agent-guide.md
//     item on "any extra live reference demotes a variable one step"): all ten
//     DirectDraw result tests turn from `cmp eax, ebp` into `test eax, eax`,
//     `xor ebp, ebp` moves after the first call, and EBP is reused for a
//     surface pointer. So a live pointer copy CANNOT be used to buy the
//     rotation while the HRESULT-in-EBP trick is in place, and that is why
//     every copy spelling measured by earlier sessions scored 88 to 96.
//   - The inline-helper form of the same idea (a `__inline void ZeroDc(HDC **)`
//     called as `ZeroDc(&setup.dcSlot)`, so the reload becomes an argument
//     temporary) is 89.3% and 1009 bytes for the same reason: the argument
//     temporary is a live node too, and EBP is lost again.
//   - `cleanup->dib = cleanup->hpalette = 0;` (one chain instead of two
//     statements, inner assignment emitted first so the store order stays
//     0x4c then 0x44) is byte exact at 1017 and reproduces the baseline diff
//     exactly, which proves the two handle stores are a SINGLE tree and that
//     the reload still sorts after that whole tree. `cleanup->hpalette =
//     cleanup->dib = 0;` emits 0x44 then 0x4c and scores 99.3, confirming the
//     chain is emitted innermost first. The comma form
//     `a = 0, b = 0, *c = 0;` is byte identical to the baseline.
//   - `*((HDC **)&cleanup->dib)[1] = 0;` for the hpalette store emits
//     `mov [edi + 0x48], ebp`, 99.3, so 0x4c really is a separate field and
//     not the second word of a two-word pair with 0x44.
// Conclusion of this session, worth not repeating: the reload is at the block
// head in the original only if it is its OWN tree coming from an earlier
// source statement. Every dead earlier use of `setup.dcSlot` is deleted by the
// front end before the scheduler runs (so a CSE with the store's address load
// cannot preserve it either), and every live one costs the EBP constant. The
// [load, push 6, store] triple always moves as one unit with the source
// position of the dc-store statement, which is why all six statement orders
// keep the same rotation somewhere in the block. This is the same scheduler
// tie-break the guide records for 0x4b6570, so it was left alone.
//
// deepseek-v4.1-flash retry session: still 99.7 (1017 = 1017), same single
// 3-instruction rotation at 0x4b55af. New shapes, all scored for free with
// --sym and all inert at 99.7 unless noted:
//   * value-typed stores through the same pointer, `*(long *)`, `*(int *)`,
//     `*(void **)`, `*(HDC *)(void *)`, `NULL`, `p[0]`, `0[p]`, `*(p + 0)`,
//     `(&setup.dcSlot)[0][0]`: all byte-identical to `*setup.dcSlot = 0;`.
//     The code-generator temp stays in EDX and stays glued to the store, so
//     the load is emitted after the two handle stores (the baseline diff).
//   * an inline helper that takes the pointer BY VALUE, `Zero(setup.dcSlot)`
//     (99.7, identical diff): the argument temp is glued the same way.
//   * a member inline `void ZeroHandles() { hpalette = 0; dib = 0; }` called
//     on the Display (the 0x463610 / 0x4644d0 two-adjacent-stores idea): 99.7,
//     identical diff, so the stores are not reordered by the method boundary.
//   * a nested `{ ... }` block around the copy and the store: 96.4, like every
//     other named copy. The copy is always colored EAX: after the DeleteObject
//     calls EAX, ECX and EDX are all free and EAX is the allocator's first
//     choice, and that recolors the whole SetWindowPos argument block (hwnd
//     moves ecx -> eax, height ecx -> edx, width edx -> eax) plus one DD block.
//   * `HDC *&p = setup.dcSlot; ... *p = 0;`: 99.7. A reference adds no register
//     node, so the load is again the glued code-gen temp.
//   * an `if (setup.dcSlot) *setup.dcSlot = 0;` guard: 90.8 (adds the branch).
// Conclusion unchanged: the load can only sit above the two handle stores if
// it comes from an earlier source statement, and every earlier-load spelling
// is a named copy whose EAX coloring costs more than the rotation; a code-gen
// temp never hoists above the stores in this build. Same scheduler tie as
// docs/agent-guide.md 0x4b6570.
#include <windows.h>
#include <ddraw.h>

struct Class_004c6a60;

struct Surface_004b5510 {
    int data[12];
};

struct BitmapInfo_004b5510 {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[256];
};

struct DirectDrawState {
    IDirectDraw *ddraw;          // +0x84
    IDirectDrawSurface *primary; // +0x88
    IDirectDrawSurface *back;    // +0x8c
    IDirectDrawClipper *clipper; // +0x90
    IDirectDrawPalette *palette; // +0x94
    void *field_98;              // +0x98
    int field_9c;                // +0x9c
};

struct Display_004b5510 {
    char unknown_0[0x40];
    HWND hwnd;         // +0x40
    HBITMAP dib;       // +0x44
    HDC dc;            // +0x48
    HPALETTE hpalette; // +0x4c
    char unknown_50[0x84 - 0x50];
    DirectDrawState draw;
    char unknown_a0[0xd4 - 0xa0];
    int width;  // +0xd4
    int height; // +0xd8
    char unknown_dc[0xf0 - 0xdc];
    unsigned short field_f0; // +0xf0
    char unknown_f2[0x214 - 0xf2];
    PALETTEENTRY entries[256]; // +0x214
};

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;
extern Display_004b5510 *DAT_0051fbd0;

int __stdcall FUN_0049f710(int guid, void *display, int zero);
void __stdcall FUN_004b4ff0(Display_004b5510 *d);
void __stdcall FUN_004c6a60(Class_004c6a60 *s, int width, int height, int a, int b);
int __stdcall FUN_004c5e70(Surface_004b5510 *s);
void __cdecl FUN_004cbbe0(Surface_004b5510 *dst, void *src, int x, int y);
int __stdcall FUN_004c5fa0(Surface_004b5510 *s);
int __stdcall FUN_004ba200(PALETTEENTRY *entries, int start, int count);

// FUNCTION: 0x4b5510
int __stdcall FUN_004b5510(int mode) {
    struct {
        int lockResult;
        HDC *dcSlot;
    } setup;
    Display_004b5510 *d;
    DDSURFACEDESC ddsd;
    BitmapInfo_004b5510 bmi;
    Surface_004b5510 surf;
    DDSCAPS caps;
    HRESULT hr;

    while (1) {
        int result = InterlockedExchange(&DAT_0052a4e8, 0x4d41494e);
        if (result == 0) {
            DAT_0052a4ec = 0x4d41494e;
            setup.lockResult = 0;
            break;
        }
        if (DAT_0052a4ec == 0x4d41494e) {
            setup.lockResult = result;
            break;
        }
        WaitForSingleObject(DAT_0052a4f0, INFINITE);
    }
    d = DAT_0051fbd0;
    DirectDrawState *dd = &d->draw;
    dd->field_9c = 0;
    FUN_004b4ff0(DAT_0051fbd0);

    Display_004b5510 *cleanup = DAT_0051fbd0;
    setup.dcSlot = &cleanup->dc;
    if (*setup.dcSlot)
        DeleteDC(*setup.dcSlot);
    if (cleanup->hpalette)
        DeleteObject(cleanup->hpalette);
    if (cleanup->dib)
        DeleteObject(cleanup->dib);
    cleanup->hpalette = 0;
    cleanup->dib = 0;
    *setup.dcSlot = 0;
    SetWindowPos(d->hwnd, NULL, 0, 0, DAT_0051fbd0->width, DAT_0051fbd0->height,
                 SWP_NOZORDER | SWP_NOMOVE);

    if (mode != 0) {
        DAT_0051fbd0->field_f0 |= 2;

        hr = FUN_0049f710(0, &dd->ddraw, 0);
        if (hr == DD_OK) {
            hr = dd->ddraw->SetCooperativeLevel(d->hwnd, 0x53);
            if (hr == DD_OK) {
                hr = dd->ddraw->SetDisplayMode(DAT_0051fbd0->width, DAT_0051fbd0->height, 8);
                if (hr == DD_OK) {

                    ZeroMemory(&ddsd, sizeof(ddsd));
                    ddsd.dwSize = sizeof(ddsd);
                    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
                    ddsd.dwWidth = DAT_0051fbd0->width;
                    ddsd.dwHeight = DAT_0051fbd0->height;
                    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
                    ddsd.dwBackBufferCount = 1;
                    hr = dd->ddraw->CreateSurface(&ddsd, &dd->primary, NULL);
                    if (hr == DD_OK) {

                        caps.dwCaps = DDSCAPS_BACKBUFFER;
                        hr = dd->primary->GetAttachedSurface(&caps, &dd->back);
                        if (hr == DD_OK) {

                            dd->field_9c = 1;
                            hr = dd->ddraw->CreateClipper(0, &dd->clipper, NULL);
                            if (hr == DD_OK) {
                                hr = dd->clipper->SetHWnd(0, d->hwnd);
                                if (hr == DD_OK) {
                                    hr = dd->primary->SetClipper(dd->clipper);
                                    if (hr == DD_OK) {

                                        hr = dd->ddraw->CreatePalette(4, DAT_0051fbd0->entries,
                                                                      &dd->palette, NULL);
                                        if (hr == DD_OK) {
                                            hr = dd->primary->SetPalette(dd->palette);
                                            if (hr != DD_OK)
                                                goto fail;
                                        }

                                        if (DAT_0051fbd0->draw.field_98) {
                                            FUN_004c5e70(&surf);
                                            FUN_004cbbe0(&surf, DAT_0051fbd0->draw.field_98, 0, 0);
                                            FUN_004c5fa0(&surf);
                                        }
                                    } else
                                        goto fail;
                                } else
                                    goto fail;
                            } else
                                goto fail;
                        } else
                            goto fail;
                    } else
                        goto fail;
                } else
                    goto fail;
            } else
                goto fail;
        } else
            goto fail;
    } else {
        DAT_0051fbd0->field_f0 &= ~2;

        HDC hdc = GetDC(d->hwnd);
        d->dc = CreateCompatibleDC(hdc);
        ReleaseDC(d->hwnd, hdc);

        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = DAT_0051fbd0->width;
        bmi.bmiHeader.biHeight = -DAT_0051fbd0->height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biCompression = 0;
        bmi.bmiHeader.biBitCount = 8;
        bmi.bmiHeader.biSizeImage = 0;
        bmi.bmiHeader.biClrUsed = 0;
        bmi.bmiHeader.biClrImportant = 0;
        ZeroMemory(bmi.bmiColors, sizeof(bmi.bmiColors));
        d->dib = CreateDIBSection(d->dc, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, (void **)&setup.dcSlot,
                                  NULL, 0);
        FUN_004c6a60((Class_004c6a60 *)&d->unknown_50[0], DAT_0051fbd0->width, DAT_0051fbd0->height,
                     (DAT_0051fbd0->width + 3) & ~3, (int)setup.dcSlot);
        SelectObject(d->dc, d->dib);
        SetWindowPos(d->hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    FUN_004ba200(DAT_0051fbd0->entries, 0, 0x100);
    if (setup.lockResult == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
    return 1;

fail:
    if (setup.lockResult == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
    return 0;
}

// Session 4 (deepseek-v4.1), additional measurements on the same rotation.
// The whole group emitted by the three zero stores plus the next statement is
// source order with exactly ONE scheduler move: `push 6` (the first push of the
// SetWindowPos statement) is always inserted immediately before the `mov [edx],
// ebp` store, and the address load stays glued to that store. Measured on all
// six statement orders (c/h/d permutations): c first gives load,push6,store,h,d
// and the load is at the block head; h,d,c (this file) gives h,d,load,push6,
// store; h,c,d gives h,load,push6,store,d; d,c,h gives d,load,push6,store,h; and
// the two comma/chained shapes reproduce those. So the original's list was
// load,h,d,store: the load is materialized one statement before its store, which
// only an explicit value copy does in this compiler.
// The copy is the dead end: every real copy (HDC *p = setup.dcSlot; in 20
// spellings: HDC*/int/void*/int cast, const, reference-to-pointer, union,
// function scope, nested scope, register, an inline identity wrapper, the same
// copy with the store before/after the two handle stores) emits the load at the
// block head but the allocator always puts it in EAX, which recolours the whole
// SetWindowPos block (hwnd into ecx, width into eax) and scores 96.4%; only a
// direct store-through-pointer gets EDX, and that form always emits the load at
// the store's own position. The one variant where a copied pointer did land in
// edx (copy immediately followed by its store) was copy-propagated straight
// back into the base form.
// Other levers measured this session, all inert or worse: address-side-effect
// trees (`*(setup.dcSlot + (int)(h = 0) + (int)(d = 0)) = 0` and index forms
// fold to the base order, 99.7), `(h = d = *setup.dcSlot = 0)`-style chains
// (99.3, handle stores swap), foldable arithmetic on the pointer used as the
// stored value (`(int)setup.dcSlot * 0`, `& 0`, `- itself`) after which c2
// deletes the load, dead double stores (kept, 1023 bytes), do/while(0),
// switch(0) and if(1) wrappers (all 99.7), /Gz, /Gr and /Gd flags (all 99.7),
// BT_TOOLCHAIN=msvc5-rtm (99.0), and tools/headers.py (all 128 sets 99.7).
// Frame constraint found while testing the "two variables" reading of the
// 0x14/0x18 slot pair (pdc at 0x14, bits at 0x18): giving CreateDIBSection its
// own real local (a third struct member or a plain local) makes MSVC allocate
// both a home for it AND a separate temp for its address, growing the frame to
// 0x4d4 and shifting every [esp+N] (90.8/91.8), so the original really had one
// address-taken local at [esp+0x14] with the compiler's &temp at [esp+0x18].
//
// Still differing, one 2-slot rotation at 0x4b55af. The original reloads the
// stack slot `mov edx, dword ptr [esp + 0x14]` (the address of setup.dcSlot)
// two instructions before its store, above the hpalette/dib zero stores, and
// the store stays after `push 6`:
//   mov edx, [esp + 0x14] / mov [edi + 0x4c], ebp / mov [edi + 0x44], ebp /
//   push 6 / mov [edx], ebp
// The rotation is really the basic block's first instruction: `je 0x4b55af`
// above it targets the reload in the original and the hpalette store here, so
// the block starts at the reload there and one instruction later here. Same
// scheduler tie as 0x4b6570 in docs/agent-guide.md, where a reload of an
// address-taken local drifted by a couple of instructions. The list order is
// forced: the reload is only generated at the use of setup.dcSlot, the only
// use in this block is the `*setup.dcSlot = 0` store, and MSVC 5 never hoists
// that load above the two `mov [edi+0x4c/0x44], ebp` stores (it cannot prove
// the frame slot does not alias them, and it does not hoist loads at all: a
// load stays glued to the statement that needs it in every variant here).
// Tried and rejected: every source order of the three stores (h/d/c, c/h/d,
// d/h/c, h/c/d, d/c/h, c/d/h), `*c = h = d = 0` and `c = h = *d = 0` (the
// first gets the reload to the top of the block but emits the two handle
// stores in the wrong order, 99.3%), an explicit `HDC *slot` local (the
// pointer then lives in eax and the whole SetWindowPos block is recoloured,
// 96.1%), an inline helper holding the three stores (96.1%), hoisting the
// load by reading the slot in an earlier statement (extra frame slot, 84.8%),
// and two comma-expression forms that try to get the reload's tree in front
// of the two stores while the store stays behind them:
// `*((HDC *)(c, h = d = 0)) = 0` makes MSVC fold the discarded c to 0 and
// emit `mov ds:0, ebp` (95.9%), and `*((HDC *)(h = 0, d = 0, c)) = 0` has
// the discarded load deleted and is identical to the plain form (99.7%).
//
// Frame slot note, verified in the /Fa listing and useful elsewhere: for a
// local whose address is taken, MSVC 5 keeps the variable's own home and the
// `&var` it hands to a callee in TWO different frame slots. Here
// `setup.dcSlot` lives at [esp+0x14] (written at 0x4b5582, reloaded at
// 0x4b55af and read for the FUN_004c6a60 argument at 0x4b5879) while
// `&setup.dcSlot` passed to CreateDIBSection is a separate slot at [esp+0x18]
// (`lea ecx, [esp+0x18]` at 0x4b585a). So the exe really does read a
// different dword for the bits pointer than the one it passed to
// CreateDIBSection.
//
// space-bunny-free session 2: still 99.7% (1017 = 1017), the one 3-instruction
// rotation at 0x4b55af. New, and the strongest lead so far: the tree ORDER is
// plain source order, and the only moving part is WHERE `push 6` lands, so the
// original's list is [load, h, d, store] with the load the FIRST tree of the
// block. Measured on all six statement orders again with /Fa listings rather
// than scores (build/scratch/0x4b5510/gen2.py, key.py prints L/H/D/P6/S):
//   h,d,c -> H D L P6 S      c,h,d -> L P6 S H D
//   h,c,d -> H L P6 S D      c,d,h -> L P6 S D H
//   d,c,h -> D L P6 S H      d,h,c -> D H L P6 S
// Every one is exactly source order with `push 6` spliced in immediately before
// the dc store's own store tree, so the residual is purely "the dc store's
// address load is 4 trees later than in the original", i.e. MSVC put the load
// in the source slot of the dc-store statement and the original materialised
// that load before the two handle stores. Nothing about the handle stores can
// be blamed: a dead `(void)setup.dcSlot;`, `setup.dcSlot = setup.dcSlot;` and a
// fourth `cleanup->dc = 0;` around the group are all inert (99.7, same diff),
// and deleting statements one at a time (build/scratch/0x4b5510/gen.py) never
// flips it: dropping either handle store, both DeleteObject checks, the
// lock loop, the tail call or the SetWindowPos all keep H D L P6 S (only the
// scratch register for the address changes, edx -> ecx, which is why the
// "no anchor" rows in that run are not order changes).
// The one real order change is the in-place-store effect, which also shows the
// compiler-side rule: with a 25-line cut-down repro
// (build/scratch/0x4b5510/mini2.cpp) writing `*slot = 0` FIRST and then the
// two `p->hpalette`/`p->dib` stores, MSVC emits the through-pointer store at
// the block head and DEFERS the two in-place stores to the last moment before
// the SetWindowPos call:
//   mov [ebx],0 / mov eax,[esi+84] / mov ecx,[esi+80] / mov edx,[esi+64] /
//   push 6 / push eax / push ecx / push 0 / push 0 / push 0 / push edx /
//   mov [esi+76],0 / mov [esi+68],0 / call SetWindowPos
// In this function the two handle stores are in the reverse situation (they
// are emitted early, at the head of the block, and the through-pointer store
// last), so the deferred-store window is not what is wrong here; what the
// repro confirms is that an address load for an in-place store DOES get
// materialised at the block head when that store is the block's first tree,
// which is exactly the position the original's `mov edx, [esp + 0x14]` is in.
// space-bunny-free session 3: 96.4% is the best the copy spelling reaches, so
// the file keeps the 99.7% version. What is new is that the copy spelling is
// now MEASURED in the /Fa listing rather than inferred, and it is exactly the
// original's tree order. A 30-line cut-down repro reproduces the residual
// byte for byte (build/scratch/0x4b5510/mk4.py, m4_base.cpp emits
//   mov DWORD PTR [esi+76],0 / mov DWORD PTR [esi+68],0 /
//   mov eax, DWORD PTR _slot$[esp+12] / push 6 / mov DWORD PTR [eax],0
// which is H D L P6 S, the same as this function's residual), and adding ONE
// statement to it fixes the order completely (m4_c2.cpp):
//   HDC *sp = slot;        <- its own statement, before the two handle stores
//   p->hpalette = 0; p->dib = 0; *sp = 0;
//   ->  mov eax, DWORD PTR _slot$[esp+12] / mov DWORD PTR [esi+76],0 /
//       mov DWORD PTR [esi+68],0 / push 6 / mov DWORD PTR [eax],0
// which is EXACTLY the original's load, h, d, push 6, store. So the original
// really did materialise the slot's value in its own statement before the two
// handle stores; every statement ORDER of the three stores cannot do it, which
// is why six sessions of reordering all stayed at 99.7.
// In the real function that exact shape (build/scratch/0x4b5510/r1.cpp) fixes
// the rotation and keeps `cmp eax, ebp`, 1017 bytes, but scores 96.4: a named
// copy is allocated EAX, never EDX, and losing EAX to it recolours the whole
// SetWindowPos argument block (hwnd ecx->eax? no: hwnd into ecx, the height
// and width loads swap out of ecx/edx, the argument pushes reorder) and the
// SetCooperativeLevel virtual call (ecx -> edx for the vtable). Chained copies
// do not help: s2/s3/s4 (two, three and four copies) all propagate back to one
// load in EAX. Putting the copy in a third member of the setup struct (s6.cpp)
// DOES restore the SetWindowPos block exactly (ecx = height, edx = width,
// eax = hwnd, the pushes in the original order) but grows the frame by one
// dword, so every [esp+N] moves, and it emits two loads (the copy's own in edx
// plus the store's address load in eax).
// Conclusion worth carrying on: what is needed is the address materialised by
// a CODE-GENERATOR temp in EDX at the block head (the baseline's own store temp
// is EDX, just four trees too late), and a named local never gets EDX because
// EAX is the allocator's first choice there and EAX is also what the following
// SetWindowPos block needs. Reusing the else-branch `hdc` local for the copy
// (u1.cpp, the live ranges do not overlap so no frame slot should be added)
// still failed to compile inside the timebox and is the obvious next probe.
//
// space-bunny-free session 4 (this is the strongest lead yet, 99.7% with the
// rotation FIXED and only ONE instruction left, see I.cpp below):
// The compiler rule behind the rotation is now measured, and it is not a
// scheduler tie at all. MSVC 5 materialises the LOAD of a local's value at the
// point where the local is DEFINED if the local is a plain (non-address-taken)
// local, and at the point where it is USED if the local's address is taken
// (an address-taken variable must stay in memory, so every use re-reads it).
// So the original's block-head `mov edx, [esp + 0x14]` is the definition-point
// load of a plain local that was assigned &cleanup->dc BEFORE the two handle
// stores, and the `mov [edx], ebp` three trees later is the use of it. Proof
// from the /Fa listings in build/scratch/0x4b5510:
//   I.cpp    `struct { int lockResult; } setup;` + a plain `HDC *slot;`
//            -> mov edx, _slot$[esp+...] / mov [edi+76], ebp /
//               mov [edi+68], ebp / push 6 / mov [edx], ebp
//            which is the original's five instructions, byte for byte, with
//            EDX, and it keeps the frame at 0x4d0 and `cmp eax, ebp`;
//   v1.cpp   the same but `slot` IS address-taken -> the load goes back to
//            after the two handle stores (position 3, the 99.7% shape here);
//   t1.cpp   `HDC *sp = setup.dcSlot;` (a copy, address-taken or not, of a
//            variable that is) -> same order, but the copy is a register and
//            MSVC picks EAX, which recolours the whole SetWindowPos block;
//   z1/z2/z3 a copy placed BEFORE the DeleteObject calls so it must survive
//            them -> MSVC spills it to a new frame slot (0x4d4) and then the
//            load IS in EDX at the block head, but every [esp+N] moves.
// I.cpp scores 99.7% / 1017 bytes and its entire residual is:
//   -lea ecx, [esp + 0x18]      <- the CreateDIBSection bits out-param address
//   +lea ecx, [esp + 0x1c]
// i.e. the only thing still wrong is WHICH slot holds the address handed to
// CreateDIBSection as its `void **` argument. In I.cpp that argument is spelled
// `(void **)&caps` (caps already lives at [esp + 0x18]) precisely to avoid a new
// local, because a real `void *bits` local grows the frame to 0x4d4 and wrecks
// every offset (measured: s6, v3, A, B, C, D, G and the notes' earlier `bits`
// attempts all do). So the remaining task for a next attempt is: keep the store
// reading a non-address-taken plain local at [esp + 0x14] (that is what forces
// the hoisted EDX load) AND land the CreateDIBSection `void **` address on
// [esp + 0x18] with no extra local. The &temp is the only thing to move: in the
// 99.7% file `(void **)&setup.dcSlot` gives it [esp + 0x18] for free, so the two
// requirements look compatible and it is worth hunting for the spelling.
// Also confirmed here, so it need not be re-measured: the tree order of that
// block is strict source order with `push 6` spliced in immediately before the
// through-pointer store, for all six orders of the three zero stores, for extra
// stores before and after the group (x1..x4 in mk10.py), and in a 30-line
// cut-down repro of the whole pattern (m4_base.cpp reproduces this function's
// residual exactly, m4_c2.cpp is the copy and reproduces the original's order).
// That is why no statement order of the three stores can ever fix it: the load
// and the store have to come from two different source statements.
// space-bunny-free session 4, part 2: the two halves of I.cpp turn out to be
// mutually exclusive in every spelling tried, and the conflict is now measured
// so nobody has to re-derive it. The CreateDIBSection `void **` argument gets a
// COMPILER TEMP whose slot is always the address-taken variable's own slot plus
// four: `(void **)&setup.dcSlot` with dcSlot at 0x14 gives [esp + 0x18] (the
// 99.7% file), `(void **)&caps` with caps at 0x18 gives [esp + 0x1c] (I.cpp),
// `(void **)&setup` with setup at 0x10 gives [esp + 0x14] (M.cpp). And the
// hoisted block-head load needs the slot's value to come from a local that is
// NOT address taken: O.cpp and Q.cpp make that same plain local address taken
// (so the CreateDIBSection temp would land at 0x18) and the load drops straight
// back to position 3, i.e. the 99.7% shape. So `slot` cannot be both at 0x14
// and address taken, and no third local helps: MSVC 5 does not give a new local
// a slot that a live local already has, and any extra dword pushes the frame
// from 0x4d0 to 0x4d4 (measured for `void *bits`, a third struct member, a
// local declared in the else block, and `HDC *pad`, which MSVC drops entirely).
// The next thing to try is therefore NOT a local at all: it is a spelling of
// the CreateDIBSection argument that makes MSVC use [esp + 0x18] as the frame
// address of a real variable rather than a temp, with `slot` left
// non-address-taken at 0x14 (I.cpp is that experiment with the address taken
// wrong, and it is one instruction from the original).
// space-bunny-free session 5, wrap-up. Best lead is T.cpp (build/scratch/0x4b5510):
// `struct { int lockResult; } setup;` + a plain, NOT address-taken `HDC *slot;`
// + `int pad2;` (a dead `pad2 = mode;` is enough to make MSVC 5 lay the slots
// out in declaration order, which is what puts slot at [esp+0x14] and
// setup.lockResult at [esp+0x10]), the block then emits the original's five
// instructions exactly, in EDX, and T.cpp's WHOLE residual is one instruction:
//   -lea ecx, [esp + 0x18]   /  +lea ecx, [esp + 0x1c]
// i.e. the address handed to CreateDIBSection as its `void **`. It is not put in
// the file because `(void **)&caps` is not source anyone wrote (it stores the
// DIB bits pointer over caps.dwCaps) and because it scores the same 99.7% as the
// version in this file, which keeps the plausible spelling; S1.cpp is the same
// thing with `slot` declared at function scope and is equally close.
// Also measured, do not repeat: permute.py over t1.cpp (the EAX copy) ran 11155
// candidates in 20 minutes and never beat 96.4%, and over S1.cpp (the shape one
// instruction from the match, score 5) it ran 3023 candidates in 12 minutes with
// no gain either, so the last instruction is not reachable by permuting the
// source as it stands; tools/headers.py --cpp is flat
// at 99.7% for both the file in src/ and T.cpp, so the header set is not the
// lever; and making the plain local address taken to move the `void **` temp
// from 0x1c to 0x18 (U, V3, V4, O, Q) ALWAYS moves the slot pair back to
// slot 0x10 / setup 0x14 AND puts the reload back at position 3, because MSVC 5
// allocates an address-taken local in a different pass. That is the whole wall:
// the hoisted reload needs a plain local, and the 0x18 `void **` temp needs an
// address-taken variable at 0x14.
// Final ranking of every shape that reaches the original's five instructions:
// T.cpp / S1.cpp / I.cpp / A-D (one instruction off, implausible bits argument),
// z1/z2/z3 (right instructions and registers, frame 0x4d4), t1 (96.4%, EAX),
// the version in this file (99.7%, three-instruction rotation).
