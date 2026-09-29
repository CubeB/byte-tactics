// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash pass: no score change, best stays 86.3% / 318 bytes.
// New evidence for the next attempt:
// - The source is NOT std::remove_if. A faithful std::remove_if + predicate
//   reproduces the whole structure but the predicate test compiles to
//   `sbb ecx,ecx / neg ecx / test cl,cl / jne` (the `!_P(...)` bool), while the
//   original has the direct `cmp [esi+0x1c],ecx / jb`. So it is a hand loop.
// - That remove_if build DID reach the desired allocation in the copy: it
//   spilled the advanced p to a stack slot and reused esi for &d->screenPos,
//   exactly like the original. It only differed in the predicate bytes and the
//   spill slot number. So the copy allocation is reachable; it needs the same
//   extra register pressure (remove_if carried _First/_Last in ebx/ebp across
//   the loop) from a construct that keeps the direct `expires >= ticks` test.
// - Tried with no change (all 318 bytes, 86.3%): `Eye* d = p++`, separate
//   named `sp`/`fp` address locals + pointer stores, `operator=` inlined via
//   `*p++ = *src`, `p = p + 1`, `++src`, a fresh phase-2 variable, a
//   function-scope `src`, Vec3/Pos copy temporaries, and byte/short field
//   hoists. Tried and worse: a function-scope destination `d` (61%), a
//   function-scope `base = g_game->eyes` (65%), p++ after the stores (66%),
//   `*p++ = *src` with a free helper (failed to build), and an extra live
//   local in the copy (56 to 58%).
// Claude Sonnet 5.5 pass (#554): compiler state ruled out (0 to 400 unused
// `extern int` declarations in steps of 8, and all 128 header sets from headers.py,
// all give 318 bytes and 86.3%). Four more source shapes scored without change:
// an `Expired(const Eye*)` predicate helper (340 bytes, 62.0%, worse), the copy as a
// `CopyEye(p++, src)` static inline helper (86.3%), the first loop with the
// `p++` moved out of the for header (85.3%) and the compaction loop as a
// do/while (86.3%). The shape is std::remove_if plus vector::erase (find the first
// expired eye, then copy every later live one down with an operator= that re-points
// the two self pointers), which the code below already follows.
// 86.3%, 318 bytes, same size as the original but three register-allocation
// details still differ (see "What still differs" at the bottom).
// What is established:
// - `expires` and `ticks` are unsigned: the loops use jae/jb, not jge/jl.
// - `changed` is never initialised in the source. MSVC therefore has to read
//   its (garbage) stack slot at the loop join, which is what the `mov eax,
//   [esp+0x10]` after `test eax, eax` is. That slot is later reused by `src`,
//   which is why the frame is only three dwords.
// - The copy in the compaction loop is written as
//       Eye* d = p;
//       p++;
//       d->player = ...; d->screen = &d->screenPos; ... d->flagB = ...;
//   The saved destination `d` plus the early increment is what produces
//   `mov eax, esi / add esi, 0x24 / mov [esp+0x18], esi` at the top of the
//   copy, and it makes our code exactly as long as the original (303 -> 318).
//   The field order of the copy is the store order in the original.
// What still differs: only the register allocation around the `end` and `p`
// stack slots. The original keeps `end` in the slot at [esp+0x14] and spills
// the advanced `p` to [esp+0x18]; ours gives [esp+0x14] to a spill slot for
// the &d->flagB address and puts `end` at [esp+0x18]. Because the original
// frees esi (p's register) at the increment, both self-pointers
// (&d->screenPos in esi, &d->flagB in edi) stay in registers; ours keeps p in
// esi, so the &d->flagB address is spilled and the screenPos value goes
// through edx instead of eax. The sign fix-up of the final /36 also uses ecx
// instead of eax, following from the same difference. Splits into two pointer
// variables, block scoping, a fresh phase-2 variable, the increment forms
// (p++, ++p, p += 1, p = p + 1, Eye* d = p++), a copy helper (free function
// and method), named locals for the two self-pointers and every
// for/while/do-while spelling all give the same 86.3%.
#include <stddef.h>

#pragma pack(push, 1)
struct Pos_00482130 {
    short x;
    short y;
};

struct Vec3_00482130 {
    int x;
    int y;
    int z;
};

// One of the "eyeball" records of the array at g_game + 0x1427b. The two
// pointer fields point into the record itself: +4 at its screen position, +0xc
// at the byte flag, so the copy has to re-point them at the destination.
struct Eye_00482130 {
    void* player;                          // +0x00
    Pos_00482130* screen;                  // +0x04, &screenPos
    short x;                               // +0x08
    unsigned char flagA;                   // +0x0a
    char flagB;                            // +0x0b
    char* flagPtr;                         // +0x0c, &flagB
    Vec3_00482130 v;                       // +0x10
    unsigned int expires;                  // +0x1c
    Pos_00482130 screenPos;                // +0x20
};

struct Game_00482130 {
    char unknown_0[0x14277];
    int count;                             // +0x14277
    Eye_00482130* eyes;                    // +0x1427b
    char unknown_1427f[0x38a47 - 0x1427f];
    unsigned int ticks;                    // +0x38a47
};
#pragma pack(pop)

extern Game_00482130* g_game;

void __stdcall FUN_00481d50(Eye_00482130* eye);

// FUNCTION: 0x482130
void FUN_00482130()
{
    int changed;                           // never initialised, as in the original
    Eye_00482130* p = g_game->eyes;

    for (int i = 0; i < g_game->count; i++, p++) {
        if (p->expires < g_game->ticks) {
            FUN_00481d50(p);
            changed = 1;
        }
    }
    if (!changed)
        return;

    Eye_00482130* end = g_game->eyes + g_game->count;
    p = g_game->eyes;
    while (p != end && p->expires >= g_game->ticks)
        p++;
    if (p != end) {
        Eye_00482130* src = p + 1;
        if (src != end) {
            for (; src != end; src++) {
                if (src->expires >= g_game->ticks) {
                    Eye_00482130* d = p;
                    p++;
                    d->player = src->player;
                    d->screen = &d->screenPos;
                    d->x = src->x;
                    d->flagPtr = &d->flagB;
                    d->v = src->v;
                    d->flagA = src->flagA;
                    d->expires = src->expires;
                    d->screenPos = src->screenPos;
                    d->flagB = src->flagB;
                }
            }
        }
    }
    g_game->count = (int)((char*)p - (char*)g_game->eyes) / 0x24;
}
