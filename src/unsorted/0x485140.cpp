// Decompiled by Opus, finished by deepseek-v4.1-flash, GPT-6.1-sol and space-bunny-free. Names are provisional.
// Codex / GPT-6 retest in #13:
// capturing z first and reversing helper argument order did not
// preserve the position pointer in ecx and tile x in esi.
// GPT-6.1-sol retest in #1672: the baseline helper version remains best at 72.7%.
// Inlining the bounds and cell-index expression fell to 46.0%; restoring the
// helper and trying the original x/y declaration order again stayed at 72.7%.
// DeepSeek retest in #1334: the whole diff is still one allocation tie. p
// wants ecx (short lived, then reused for width) and x wants esi (long
// lived). Compiling the real preceding function 0x485070 in the same file
// (same g_game, same struct layouts) did not change it, nor did a CSE'd
// extra use of x at the GetCell call, a reference/copy of p, raw field
// access, single-declaration locals, or uninitialised cell/result locals
// (those last three drop to 68.2%). The instruction sequence is otherwise
// identical, so this is compiler register priority, not source shape.
// Space Bunny Free retest in #1887: the helper version is still the best of
// everything tried, at 72.7%, and the whole diff is still the single p/x
// allocation tie. <windows.h> is the best header of the 128 headers.py tries
// (all 128 score 68.2% or 72.7%, none match), and the N-declarations test is
// flat (0 to 5 dummy int locals: 72.7, 72.7, 68.2, 68.2, 72.7, 72.7), which
// the guide says means the compiler state is already in the right phase and
// the source shape is wrong rather than the header block. The guide's "go
// bigger" lever also does not fire here: <string>, <vector>+<map>,
// <iostream>, no include at all, and 500, 2000 and 4000 extern ints plus
// 3000 unused prototypes all score 68.2% or 72.7%.
// About twenty further shapes also stayed at or below 72.7%: the helper
// taking the position pointer instead of the two tile coordinates (68.2%),
// one declaration statement for both coordinates, a const reference
// parameter, a const pointer local, a reference to the position, the raw
// fixed-point halves in short or int locals, two helpers one per coordinate,
// a short-to-tile conversion helper, the division written as an explicit
// round-toward-zero shift (36.6%, wrong size), the bounds check and index
// inlined in the function (68.2%), the same as a macro, the index in its own
// local (35.6%, wrong size), y computed before x (59.8%, wrong size),
// uninitialised locals declared at the top in both orders, throwaway extra
// uses of x and of y folded away by the optimiser, an early-return helper
// that fails with two explicit guards (68.2%), the cell address written out
// in full at each use (50.0%), and the helper failing with `return 0` before
// the index. The division and both compares are settled: the cdq/and 0xf/add
// pair is a signed /16 on a short, and the two `test reg,reg / jl` against
// width and height are signed bounds checks in that order.
// Returns the average of the two height bytes (+5, +6) of the map cell under
// a 16.16 fixed-point position, or -1 off the map.
// Not matched: the original keeps the position pointer in ecx and the tile x
// in esi; every rewrite tried (locals, helpers, macro, method, header sets,
// N-declarations, extern counts) swaps them. <windows.h> only fixes the lea
// order of the index.
// GPT-6.1-sol retest in #2483: the 72.7% baseline remains best. Reversing
// coordinate evaluation scored 59.8%; a separate GetX helper scored 68.2%.
// Const qualification and uninitialized x/y declarations both scored 72.7%
// but retained the same pointer/x register swap.
// DeepSeek V4.1 Flash retest in #3007 (30-min checkpoint: baseline still
// best at 72.7%, no scratch variant scored higher). New evidence on the
// allocation lever: `return p->x.whole / 16 + p->z.whole / 16;` (no named
// locals, no branch) compiles to the original prologue byte for byte
// (mov ecx,[esp+4]; push esi; movsx eax,[ecx+2]; ...; mov esi,eax; movsx
// eax,[ecx+0xa]) and p stays in ecx, so the whole difference is caused by
// the bounds test on x: any branch that reads x (even one added after the
// divisions, or a test of a plain copy) flips p to esi. The one shape that
// keeps p in ecx *with* the bounds test is an extra live boolean: a caller
// `bool ok = <bounds>; if (ok) ...` (71.9%), a helper-side `bool ok`, or a
// bool-returning InBounds helper all put p in ecx and x in esi, but VC5
// materialises the bool (mov cl,1 / xor cl,cl / test cl,cl) and hoists the
// g_game load above the sars, so they emit 6 to 14 bytes more and score
// below the baseline. Getting the original code shape (single out-of-line
// xor ecx,ecx failure path, no bool materialisation) *and* the original
// allocation is what is still missing; the next attempt should look for a
// source form where the bounds test reads a value other than the register
// that holds x, or a variable that shifts the allocator order without
// surviving into the output.
#include <windows.h>

#pragma pack(push, 1)
struct Cell_00485140 {
    char unknown_0[0x5];
    unsigned char field_5;              // +0x5
    unsigned char field_6;              // +0x6
    char unknown_7[0xd - 0x7];
};

struct Game_00485140 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00485140* cells;               // +0x14287
};
#pragma pack(pop)

struct Fixed_00485140 {
    unsigned short frac;
    short whole;
};

struct Pos_00485140 {
    Fixed_00485140 x;                   // +0x0
    Fixed_00485140 y;                   // +0x4
    Fixed_00485140 z;                   // +0x8
};

extern Game_00485140* g_game;

static inline Cell_00485140* GetCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[g_game->width * y + x];
    return 0;
}

// FUNCTION: 0x485140
int __stdcall FUN_00485140(Pos_00485140* p)
{
    int x = p->x.whole / 16;
    int y = p->z.whole / 16;
    Cell_00485140* cell = GetCell(x, y);
    if (cell)
        return (cell->field_5 + cell->field_6) >> 1;
    return -1;
}
