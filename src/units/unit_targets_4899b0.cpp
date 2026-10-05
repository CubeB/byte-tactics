// Decompiled by space-bunny-free. Names are provisional.
// Same class as 0x489a90: a `[ecx+0x92]` def pointer, the def flags word at
// +0x245 and the def word at +0x241, and the g_game byte at +0x1427f.
//
// A yes/no test between two units of that class. All six failed checks jump to
// one shared `return 0` block that sits after the `return 1` join, so the
// failures are `goto fail` out of the test and the fall-through is the success.
// The masked `f241 & 0x800` is kept in a register across the branch: it is
// materialised once with `and edx, 0x800` and tested again (`test edx, edx`)
// at the join, which is what leaves the second `if` here testing the same
// condition rather than an `else`.
//
// The one thing that needed `#include <windows.h>` for is the two `movsx` of
// each range test: without the header MSVC canonicalises the commutative `+` the
// wrong way round and emits them in the opposite order, the def-side field at
// +0x170 first, so the sum lands in the other register and the whole function
// comes out 2 bytes shorter. Which operand of a commutative integer operation
// MSVC loads first depends on how many declarations the file has already seen,
// so the header is needed even though nothing here comes from it.
// `tools/headers.py 0x4899b0` found 96 of its 128 sets match, the smallest
// being `<windows.h>` on its own; the other minimal ones are `<ddraw.h>`, and
// `<windows.h>` with any one of `<stdio.h>`, `<stdlib.h>`, `<string.h>`,
// `<math.h>`. Every set without a Windows header (so also no include at all)
// keeps the wrong order, which is why rewording the source never fixed it:
// swapping the operands, accumulating through a local, a static inline helper,
// per-field getters, an explicit `(int)` cast, a nested struct around the field,
// and a local for the def pointer all failed. Only defeating the front end's
// common-subexpression temp for `other->def` reversed the order, and then the
// def pointer was reloaded into a scratch register instead of `edi`.
#include <windows.h>

#pragma pack(push, 1)
struct Def_004899b0 {
    char unknown_0[0x170];
    short f170;                    // +0x170, compared signed
    char unknown_172[0x1be - 0x172];
    short f1be;                    // +0x1be, compared signed
    char unknown_1c0[0x1fa - 0x1c0];
    int f1fa;                      // +0x1fa, compared against a sign extended short
    char unknown_1fe[0x241 - 0x1fe];
    int f241;                      // +0x241, bits 11 and 21
    int f245;                      // +0x245, bit 9
};

struct Game {
    char unknown_0[0x1427f];
    unsigned char f1427f;          // +0x1427f
};

class Unit {
public:
    char unknown_0[0x70];
    short f70;                     // +0x70, compared signed
    char unknown_72[0x92 - 0x72];
    Def_004899b0* def;             // +0x92
    char unknown_96[0x108 - 0x96];
    short f108;                    // +0x108, compared signed
    char unknown_10a[0x110 - 0x10a];
    int f110;                      // +0x110

    int CanRepair(Unit* other);
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x4899b0
int Unit::CanRepair(Unit* other)
{
    if (other
        && (def->f245 & 0x200)
        && (other->f108 != other->def->f1fa)
        && ((other->f110 & 3) != 2)) {
        if (def->f241 & 0x800) {
            if (!(def->f241 & 0x200000) && other->f70 + other->def->f170 < g_game->f1427f)
                goto fail;
        }
        if (!(def->f241 & 0x800)) {
            if (other->f70 + other->def->f170 < g_game->f1427f - def->f1be)
                goto fail;
        }
        return 1;
    }
fail:
    return 0;
}
