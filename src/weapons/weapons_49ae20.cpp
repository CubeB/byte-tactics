// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Compacts the projectile array: entries whose flags bit 1 is set are dropped
// and the remaining ones are moved down. Each projectile's own old index is
// written to field_67 first, so the pointers held at field_56 (relinked to the
// moved targets in the second pass) can be resolved by searching for that
// index.

#pragma pack(push, 1)
struct Projectile_0049ae20 {
    char unknown_0[0x56];
    Projectile_0049ae20* field_56;     // +0x56
    char unknown_5a[0x67 - 0x5a];
    short field_67;                    // +0x67, this projectile's old index
    unsigned short flag0 : 1;
    unsigned short dead : 1;           // +0x69 bit 1
    unsigned short flagRest : 14;
};

struct Game {
    char unknown_0[0x141f3];
    int projectileCount;                        // +0x141f3
    Projectile_0049ae20* projectiles;           // +0x141f7
    char unknown_141fb[0x142f7 - 0x141fb];
    Projectile_0049ae20* tracked;               // +0x142f7
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x49ae20
void CompactProjectiles()
{
    Projectile_0049ae20* projectiles = g_game->projectiles;
    int count = g_game->projectileCount;
    int n = 0;
    Projectile_0049ae20* dest = 0;
    int i = 0;
    short a[300];
    short b[300];

    for (i = 0; i < g_game->projectileCount; i++) {
        Projectile_0049ae20* p = &projectiles[i];
        // Read the bitfield into a bool first: gives the shr/test pair.
        bool dead = p->dead;
        p->field_67 = (short)i;
        if (dead) {
            if (!dest) {
                dest = p;
            }
            count--;
        } else {
            if (dest) {
                if (g_game->tracked == p) {
                    g_game->tracked = dest;
                }
                if (p->field_56) {
                    a[n] = (short)(dest - projectiles);
                    b[n] = (short)(p->field_56 - projectiles);
                    n++;
                }
                *dest = *p;
                dest++;
            }
        }
    }

    if (n != 0) {
        int found = 0;
        for (int j = 0; j < count; j++) {
            for (int k = 0; k < n; k++) {
                if (projectiles[j].field_67 == b[k]) {
                    projectiles[a[k]].field_56 = &projectiles[j];
                    if (++found == n) {
                        goto done;
                    }
                }
            }
        }
    }
done:
    g_game->projectileCount = count;
}
