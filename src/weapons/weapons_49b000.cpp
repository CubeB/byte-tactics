// Decompiled by Opus. Names are provisional.
// Fires one of a unit's two weapons (by type) from the unit's position,
// building the projectile on the stack; see 0x49af90 for the projectile.

struct Weapon_0049b000;

struct Vec3_0049b000 {
    int x, y, z;
};

struct UnitType_0049b000 {
    char unknown_0[0x220];
    Weapon_0049b000* weapon1;          // +0x220
    Weapon_0049b000* weapon2;          // +0x224
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x6a];
    Vec3_0049b000 pos;                 // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitType_0049b000* type;           // +0x92
    char unknown_96[0xff - 0x96];
    unsigned char owner;               // +0xff
};

struct Projectile_0049b000 {
    Weapon_0049b000* weapon;           // +0x0
    Vec3_0049b000 pos;                 // +0x4
    Vec3_0049b000 start;               // +0x10
    char unknown_1c[0x4e - 0x1c];
    int field_4e;                      // +0x4e
    int field_52;                      // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char owner;               // +0x66
    char unknown_67[0x6b - 0x67];
};
#pragma pack(pop)

void __stdcall FUN_00499eb0(Projectile_0049b000* proj, int flag);

// FUNCTION: 0x49b000
void __stdcall FUN_0049b000(Unit* unit, int second)
{
    Weapon_0049b000* weapon = second ? unit->type->weapon2 : unit->type->weapon1;
    if (weapon) {
        Projectile_0049b000 proj;
        proj.weapon = weapon;
        proj.pos = unit->pos;
        proj.start = unit->pos;
        proj.field_4e = 0;
        proj.field_52 = 0;
        proj.owner = unit->owner;
        FUN_00499eb0(&proj, 0);
    }
}
