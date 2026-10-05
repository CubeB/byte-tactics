// Decompiled by Opus. Names are provisional.
// Finds the projectile a remote event describes: one not owned by the local
// player, at the event's position, fired by a unit with the event's id.
// The position test is an inline helper taking the two Vec3s by reference;
// written in place, MSVC walks the array from +0x28 instead of +0x2c.

struct Vec3_0049d1e0 {
    int x;
    int y;
    int z;
};

struct Unit {
    char unknown_0[0xa8];
    short id;                          // +0xa8
};

#pragma pack(push, 1)
struct Projectile_0049d1e0 {
    char unknown_0[0x28];
    Vec3_0049d1e0 pos;                 // +0x28
    char unknown_34[0x52 - 0x34];
    Unit* owner;                       // +0x52
    char unknown_56[0x66 - 0x56];
    char player;                       // +0x66
    char unknown_67[0x6b - 0x67];
};

struct Event_0049d1e0 {
    char unknown_0[0xd];
    Vec3_0049d1e0 pos;                 // +0xd
    char unknown_19;
    unsigned char flag : 1;            // +0x1a
    char unknown_1b[0x1f - 0x1b];
    short ownerId;                     // +0x1f
};

struct Game {
    char unknown_0[0x2a42];
    char localPlayer;                  // +0x2a42
    char unknown_2a43[0x141f3 - 0x2a43];
    int projectileCount;               // +0x141f3
    Projectile_0049d1e0* projectiles;  // +0x141f7
};
#pragma pack(pop)

extern Game* g_game;

static inline int SamePos(Vec3_0049d1e0& a, Vec3_0049d1e0& b)
{
    return a.x == b.x && a.z == b.z && a.y == b.y;
}

// FUNCTION: 0x49d1e0
Projectile_0049d1e0* __stdcall FUN_0049d1e0(Event_0049d1e0* ev)
{
    if (!ev->flag)
        return 0;
    char me = g_game->localPlayer;
    Projectile_0049d1e0* proj = g_game->projectiles;
    for (int i = 0; i < g_game->projectileCount; i++, proj++) {
        if (proj->player != me && SamePos(proj->pos, ev->pos) && proj->owner->id == ev->ownerId)
            return proj;
    }
    return 0;
}
