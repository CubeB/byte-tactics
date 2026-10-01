// Decompiled by Opus, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Removes the projectiles fired by a unit (the caller passes the dying unit):
// walks the 300-entry projectile array, and for every active projectile owned
// by that unit runs the inlined untrack helper (0x499e50) and compacts the
// array (0x49ae20).
//
// MATCH (147 of 147 bytes). The whole function was held back by one
// declaration: the flag word at +0x69 is a 16-bit `unsigned short`, not a
// `char`/`unsigned char`. With the byte field the loop's constant 0 is parked
// in ebp and `owner` in ebx (78.3%, byte-identical to the original except that
// one register pair); with the 16-bit field the allocator puts the 0 in ebx
// and `owner` in ebp, exactly as the original does. The emitted OR is the same
// four-byte `or byte ptr [esi+0x69], 2` either way, because the immediate's
// high byte is 0, so MSVC 5 narrows the 16-bit `|= 2` to a byte OR. The struct
// must end at +0x6b (the flag word occupies +0x69..+0x6a), which is what makes
// the loop stride 0x6b; a trailing padding byte gives `add esi, 0x6c` (97.8%).
//
// How it was found, in case a sibling needs the same: a micro-function
// reproducing this loop (build/scratch/0x49c880/micro/) showed the pair is
// decided by the *width of the flags field's OR*, not by any source shape:
// `or byte [p+0x69], 2` keeps the zero in ebp while a word/dword OR of the same
// shape puts it in ebx. A byte-typed flags field therefore cannot produce the
// original's allocation; the 16-bit field can, and still compiles to the byte
// OR. The struct offsets in this file are byte-exact, so only the field type
// mattered.
//
// Measured and rejected on the way (all with check.py or check.py --sym):
// the constant 0 as a named int/short/pointer/bool/char local (folds away),
// a bool/char local for the active test (forces the zero into ebx via
// `cmp cl, bl` but costs seven bytes of setne, 88.4%), `(unsigned char)` on
// the active test (146 bytes, 89.1%, a metric artifact: the original compares
// a word), the flags field as a `char` (78.3%) or a `char` bitfield (78.3%),
// duplicated tests, extra depth-1 zero stores, `g_game`/`Game*` aliases,
// accessor helpers, a `char*` walk with `+= 0x6b`, for/while/do-while/
// while(1)+break loops, an index-only loop, and `int`/`void*`/`Unit* const`
// parameter types. tools/headers.py --cpp (all 768 sets) was flat.
// Earlier passes (deepseek-v4.1-flash, GPT-6.1-sol, Codex / GPT-6,
// space-bunny-free in #1887) reached 78.3% and left the notes that the tie was
// source-shape insensitive; it was, because the lever is a declaration in the
// struct, not the body.

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
    unsigned short flags;              // +0x69, 16-bit: the byte form changes
                                       // the register allocation of 0x49c880
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
