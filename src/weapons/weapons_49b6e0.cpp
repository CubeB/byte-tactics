// Decompiled by Opus. Names are provisional.
// Takes the next free entry of the 300-entry projectile array (see
// 0x499a30), resets two of its fields and returns it, or 0 when full.

#pragma pack(push, 1)
struct Projectile_0049b6e0 {
    char unknown_0[0x4e];
    int field_4e;                      // +0x4e
    char unknown_52[0x69 - 0x52];
    unsigned short flags;              // +0x69
};

struct Game {
    char unknown_0[0x141f3];
    int projectileCount;               // +0x141f3
    Projectile_0049b6e0* projectiles;  // +0x141f7
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x49b6e0
Projectile_0049b6e0* AllocProjectile()
{
    Projectile_0049b6e0* p = 0;
    if (g_game->projectileCount < 300) {
        p = &g_game->projectiles[g_game->projectileCount++];
        p->flags &= ~2;
        p->field_4e = 0;
    }
    return p;
}
