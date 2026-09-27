// Decompiled by Opus. Names are provisional.
// Finds the projectile a network packet refers to (by position and type) and
// removes it. The position compare is an inlined helper taking references;
// written inline, MSVC keeps one induction pointer instead of two.

struct UnitType_0049af90 {
    char unknown_0[0x10a];
    unsigned char id;                  // +0x10a
};

struct Vec3_0049af90 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Projectile_0049af90 {
    UnitType_0049af90* type;           // +0x0
    char unknown_4[0x28 - 0x4];
    Vec3_0049af90 pos;                 // +0x28
    char unknown_34[0x6b - 0x34];
};

struct Packet_0049af90 {
    char kind;                         // +0x0
    Vec3_0049af90 pos;                 // +0x1
    unsigned char typeId;              // +0xd
};

struct Game_0049af90 {
    char unknown_0[0x141f3];
    int projectileCount;               // +0x141f3
    Projectile_0049af90* projectiles;  // +0x141f7
};
#pragma pack(pop)

extern Game_0049af90* g_game;

void __stdcall FUN_00499eb0(Projectile_0049af90* proj, int flag);

static inline int SamePos(const Vec3_0049af90& a, const Vec3_0049af90& b)
{
    return a.x == b.x && a.z == b.z && a.y == b.y;
}

// FUNCTION: 0x49af90
void __stdcall FUN_0049af90(int unused, Packet_0049af90* p)
{
    Projectile_0049af90* proj = g_game->projectiles;
    for (int i = 0; i < g_game->projectileCount; i++, proj++) {
        if (SamePos(proj->pos, p->pos) && proj->type->id == p->typeId) {
            FUN_00499eb0(proj, 0);
            return;
        }
    }
}
