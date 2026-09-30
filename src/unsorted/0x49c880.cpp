// Decompiled by Opus, finished by space-bunny-free. Names are provisional.
// (Earlier passes: deepseek-v4.1-flash, GPT-6.1-sol, Codex / GPT-6.)
// 78.3%, and the only difference left is which of two values live across the
// call to 0x49ae20 gets ebx: the target keeps the constant 0 in ebx and the
// `owner` parameter in ebp, this build keeps 0 in ebp and `owner` in ebx.
// The constant needs a callee-saved register because it is live across the
// call (the `tracked = 0` store) as well as across the loop back edge.
//
// space-bunny-free in #1887, and this tie is source-shape insensitive:
// about sixty rewritten shapes all compile to BYTE-IDENTICAL objects, so none
// of them is the fix. Tried, all byte-identical or worse: for/while/do-while/
// while(1)+break/pointer-range loops, an index local instead of a pointer
// walk, the predicate as &&/nested if/continue/De Morgan/ternary, reversed
// term order, the whole loop body, the predicate, the untrack test, the flags
// store and the owner compare each moved into their own static inline helper,
// an out-of-line helper, `int`/`void*`/`Unit* const`/`unsigned int` parameter
// types, an unsigned index, a `g_game` alias and a `Game*` local, `active` as
// int, the constant as a named local or a `static const`, folded duplicate
// uses of each side, and uninitialised locals / externs / structs / typedefs /
// enums / unused static functions at counts 0 to 575.
// tools/headers.py --cpp (all 768 sets, 0 to match) and 337 further header
// sets outside its list (assert.h, ctype.h, errno.h, float.h, io.h, limits.h,
// locale.h, setjmp.h, signal.h, stdarg.h, stddef.h, time.h, sys/types.h,
// excpt.h, winnt.h, objbase.h, ole2.h, mmsystem.h, process.h, share.h,
// malloc.h, new.h, dos.h, fcntl.h, search.h, direct.h, alone and in pairs and
// triples) are flat too. The loop shape, the predicate and the inline helper
// boundary are therefore believed correct; what is missing is a compiler
// state that orders the two live-across-call values the other way, and no
// source or declaration change tried here reaches it.

struct Vec3_0049c880 {
    int x;
    int y;
    int z;
};

struct Unit_0049c880;

struct UnitType_0049c880 {
    char unknown_0[0xfe];
    short value;                       // +0xfe
};

#pragma pack(push, 1)
struct Projectile_0049c880 {
    UnitType_0049c880* type;           // +0x0
    Vec3_0049c880 pos;                 // +0x4
    char unknown_10[0x52 - 0x10];
    Unit_0049c880* owner;              // +0x52, the unit that fired it
    char unknown_56[0x60 - 0x56];
    short active;                      // +0x60
    char unknown_62[0x69 - 0x62];
    unsigned char flags;               // +0x69
    char unknown_6a;
};

struct Game_0049c880 {
    char unknown_0[0x141f3];
    int projectileCount;               // +0x141f3
    Projectile_0049c880* projectiles;  // +0x141f7
    char unknown_141fb[0x142f7 - 0x141fb];
    Projectile_0049c880* tracked;      // +0x142f7
    char unknown_142fb[0x1433f - 0x142fb];
    Vec3_0049c880 trackedPos;          // +0x1433f
    short trackedValue;                // +0x1434b
};
#pragma pack(pop)

extern Game_0049c880* g_game;

void FUN_0049ae20();

// Same body as the matched 0x499e50.cpp, which the original inlined here.
static inline void Untrack_0049c880(Projectile_0049c880* proj)
{
    if (proj == g_game->tracked) {
        g_game->trackedPos = g_game->tracked->pos;
        g_game->trackedValue = proj->type->value;
        g_game->tracked = 0;
    }
    proj->flags |= 2;
}

// Removes the projectiles fired by a unit (the caller passes the dying unit).
// Remaining difference: the ebx/ebp tie described at the top of this file.
// FUNCTION: 0x49c880
void __stdcall FUN_0049c880(Unit_0049c880* owner)
{
    Projectile_0049c880* proj = g_game->projectiles;
    for (int i = 0; i < g_game->projectileCount; i++, proj++) {
        if (proj->active != 0 && proj->owner == owner) {
            Untrack_0049c880(proj);
            FUN_0049ae20();
        }
    }
}
