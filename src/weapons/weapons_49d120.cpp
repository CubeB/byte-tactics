// Decompiled by space-bunny-free. Names are provisional.
//
// Finds the projectile in range of a per-player entry that nothing else
// refers to: the entry gives a position and a radius, every projectile that
// belongs to another player, carries weapon flag bit 29 and lies inside that
// radius is a candidate, and the first candidate that no other projectile
// points at is returned.
#pragma pack(push, 1)
struct Vec3_0049d120 {
    int x;
    int y;
    int z;
};

struct Flags_0049d120 {
    unsigned int unknown_0 : 29;
    unsigned int hitscan : 1;        // +0x111
    unsigned int unknown_30 : 2;
};

struct Def_0049d120 {
    char unknown_0[0xe0];
    int radius;                      // +0xe0
    char unknown_e4[0x111 - 0xe4];
    Flags_0049d120 flags;            // +0x111
};

struct Entry_0049d120 {              // 0x1c bytes
    char unknown_0[0xc];
    Def_0049d120* def;               // +0xc
    char unknown_10[0x1a - 0x10];
    unsigned char active;            // +0x1a
    char unknown_1b;
};

struct Table_0049d120 {
    char unknown_0[4];
    // Only three entries are declared: the index is masked to a byte, but the
    // fields below have to land on 0x6a and 0xff, which fixes the array size.
    Entry_0049d120 entries[3];       // +0x4
    char unknown_58[0x6a - 0x58];
    Vec3_0049d120 pos;               // +0x6a
    char unknown_76[0xff - 0x76];
    char player;                     // +0xff
};

struct Proj_0049d120 {               // 0x6b bytes
    Def_0049d120* def;               // +0x0
    char unknown_4[0x28 - 0x4];
    Vec3_0049d120 pos;               // +0x28
    char unknown_34[0x56 - 0x34];
    Proj_0049d120* ref;              // +0x56
    char unknown_5a[0x66 - 0x5a];
    char player;                     // +0x66
    char unknown_67[0x6b - 0x67];
};

struct Game {
    char unknown_0[0x141f3];
    int projCount;                   // +0x141f3
    Proj_0049d120* projs;            // +0x141f7
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x49d120
Proj_0049d120* __stdcall FindTargetableProjectile(Table_0049d120* table, int index)
{
    Entry_0049d120* entry = &table->entries[index & 0xff];
    // Flag read before the radius, shift inside its initialiser: else the flag
    // load hoists into the prologue.
    if (!entry->active)
        return 0;
    int radius = entry->def->radius << 16;
    // projs is read before projCount: keeps g_game in ecx.
    Proj_0049d120* proj = g_game->projs;
    int count = g_game->projCount;
    for (int i = 0; i < count; i++, proj++) {
        if (proj->player != table->player) {
            // Nested ifs, not one && chain: else the bitfield test folds into a mem test.
            if (proj->def->flags.hitscan) {
                if ((unsigned)(table->pos.x - proj->pos.x + radius) <= 2u * radius) {
                    if ((unsigned)(table->pos.z - proj->pos.z + radius) <= 2u * radius) {
                        int j;
                        for (j = 0; j < g_game->projCount; j++)
                            if (g_game->projs[j].ref == proj)
                                break;
                        if (j == g_game->projCount)
                            return proj;
                    }
                }
            }
        }
    }
    return 0;
}
