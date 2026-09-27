// Decompiled by GPT-6. Names are provisional.
// Retains the best earlier partial by Opus.
// Retest: an owner-filtered removal helper and reversing the two predicate
// terms did not fix the ebx/ebp allocation. Retain the original best partial.

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
// Remaining difference: the original keeps the constant 0 in ebx and `owner`
// in ebp; here they are swapped. Any extra use of `owner` inside the loop
// flips them, so the original probably used it once more in a way that folds
// away; duplicate compares, helper predicates, pointer types, loop shapes,
// headers and an out-of-line FUN_00499e50 all left it unchanged.
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
