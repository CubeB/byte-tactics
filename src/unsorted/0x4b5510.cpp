// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
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
