// Decompiled by Opus, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Removes the projectiles fired by a unit (the caller passes the dying unit):
// walks the 300-entry projectile array, and for every active projectile owned
// by that unit runs the inlined untrack helper (0x499e50) and compacts the
// array (0x49ae20).

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
// Must end at +0x6b with no trailing padding: the loop stride follows it.
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

struct Game {
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

extern Game* g_game;

void CompactProjectiles();

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
void __stdcall RemoveUnitProjectiles(Unit_0049c880* owner)
{
    Projectile_0049c880* proj = g_game->projectiles;
    for (int i = 0; i < g_game->projectileCount; i++, proj++) {
        if (proj->active != 0 && proj->owner == owner) {
            Untrack_0049c880(proj);
            CompactProjectiles();
        }
    }
}
