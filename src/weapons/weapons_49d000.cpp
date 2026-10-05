// Decompiled by space-bunny-free. Names are provisional.
#pragma pack(push, 1)
struct Vec3_0049d000 {
    int x;
    int y;
    int z;
};

struct UnitType_0049d000 {
    char unknown_0[0x20];
    int range;                        // +0x20
};

struct Unit {
    UnitType_0049d000* type;          // +0x0
    char unknown_4[0x66 - 4];
    short angle;                      // +0x66
    char unknown_68[0x110 - 0x68];
    unsigned int flags;               // +0x110
};

struct Shot_0049d000 {
    char unknown_0[0xc];
    void* weapon;                     // +0xc
};

struct Proj_0049d000 {
    char unknown_0[0x1c];
    int dirX;                         // +0x1c
    int field_20;
    int dirZ;                         // +0x24
    char unknown_28[0x36 - 0x28];
    short angle;                      // +0x36
    char unknown_38[0x3a - 0x38];
    int field_3a;
    char unknown_3e[0x4e - 0x3e];
    int field_4e;
    char unknown_52[0x69 - 0x52];
    unsigned short flags;             // +0x69
    // Only inside an inline method does MSVC 5 put the angle load before the
    // +0x3a store, as the original does; inline it in the caller and it sinks.
    void Setup(Unit* u) { angle = u->angle; field_3a = 0; field_20 = 0; }
};

struct Game_0049d000 {
    char unknown_0[0x141f3];
    int projCount;                    // +0x141f3
    Proj_0049d000* projs;             // +0x141f7
    char unknown_141fb[0x38a47 - 0x141fb];
    int field_38a47;
};
#pragma pack(pop)

extern Game_0049d000* g_game;

void __stdcall FUN_0049c740(Proj_0049d000*, void*, void*, int, int, Unit*);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);

// FUNCTION: 0x49d000
int __stdcall FUN_0049d000(Shot_0049d000* shot, Unit* unit, Vec3_0049d000* pos)
{
    Proj_0049d000* proj = 0;
    if (g_game->projCount < 300) {
        proj = &g_game->projs[g_game->projCount++];
        proj->flags &= ~2;
        proj->field_4e = 0;
    }
    if (proj) {
        FUN_0049c740(proj, shot->weapon, pos, 0, g_game->field_38a47, unit);
        proj->Setup(unit);
        proj->dirX = -FUN_004b70ef(proj->angle, unit->type->range);
        proj->dirZ = -FUN_004b7123(proj->angle, unit->type->range);
        return 1;
    }
    return 0;
}
