// Decompiled by GPT-5.6-Terra, finished by claude-sonnet. Names are provisional.
// Matched. The flags dword at +0x111 is a bitfield struct (bit10 and bit22): a bitfield
// test gives the original's shr/test al,1. The sound id is passed as unsigned int, which
// gives the original's xor reg,reg / mov reg16 zero extension. The +0x69 flag is a plain
// unsigned short OR'd with 2 (see git history for the measurements); its width is a shape
// that reproduces the single 'or byte ptr [esi+0x69], 2', not a known declaration.
#pragma pack(push, 1)

struct Vec3_00499eb0 {
    int x;
    int y;
    int z;
};

struct ProjectileType_00499eb0 {
    char unknown_0[0x78];
    void* field_78;
    void* field_7c;
    char unknown_80[0xcc - 0x80];
    int field_cc;
    int field_d0;
    char unknown_d4[0xd6 - 0xd4];
    unsigned short field_d6;
    char unknown_d8[0xf6 - 0xd8];
    unsigned short sound1;
    unsigned short sound2;
    char unknown_fa[0x111 - 0xfa];
    struct {
        unsigned int bits0_9 : 10;
        unsigned int bit10 : 1;
        unsigned int bits11_21 : 11;
        unsigned int bit22 : 1;
        unsigned int bits23_31 : 9;
    } flags;
};

struct Unit {
    char unknown_0[0x96];
    struct Holder_00499eb0* field_96;
    char unknown_9a[0xff - 0x9a];
    unsigned char owner;
};

struct Projectile_00499eb0 {
    ProjectileType_00499eb0* type;
    Vec3_00499eb0 position;
    char unknown_10[0x52 - 0x10];
    Unit* unit;
    char unknown_56[0x66 - 0x56];
    unsigned char owner;
    char unknown_67[2];
    unsigned short field_69;
};

struct Net_00499eb0 {
    char unknown_0[0xd48];
    int field_d48;
};

struct Player_00499eb0 {
    int field_0;
    char unknown_4[0x73 - 4];
    unsigned char state;
    char unknown_74[0x14b - 0x74];
};

struct SquadManager {
    void FUN_00406f50(Projectile_00499eb0* projectile, int a, int b);
};

struct Holder_00499eb0 {
    char unknown_0[0x74];
    SquadManager* object;
};

#pragma pack(pop)

extern char* g_game;

void* __stdcall GetMapCellAtPosition(Vec3_00499eb0* position);
void __stdcall FUN_0041c640(int a, int b, int c);
void __stdcall AddExplosionEffect(Vec3_00499eb0* position, void* value, int a, int b);
void __stdcall EmitWhiteSmoke(Vec3_00499eb0* position, int value);
void __stdcall PlaySoundAt(unsigned int sound, Vec3_00499eb0* position, int value);
int __stdcall ApplyWeaponDamage(Projectile_00499eb0* projectile, Unit* unit, float scale);
void __stdcall ApplyAreaDamage(Projectile_00499eb0* projectile, Vec3_00499eb0* position);

// FUNCTION: 0x499eb0
void __stdcall DetonateProjectile(Projectile_00499eb0* projectile, Unit* unit)
{
    int hostile = 0;
    ProjectileType_00499eb0* type = projectile->type;
    Vec3_00499eb0* position = &projectile->position;
    unsigned char* value = (unsigned char*)GetMapCellAtPosition(position);
    if (value != 0)
        hostile = value[5] < *(unsigned char*)(g_game + 0x1427f);
    if (!type->flags.bit22) {
        if (projectile == *(Projectile_00499eb0**)(g_game + 0x142f7)) {
            *(Vec3_00499eb0*)(g_game + 0x1433f) = (*(Projectile_00499eb0**)(g_game + 0x142f7))->position;
            *(unsigned short*)(g_game + 0x1434b) = *(unsigned short*)((char*)projectile->type + 0xfe);
            *(Projectile_00499eb0**)(g_game + 0x142f7) = 0;
        }
        projectile->field_69 = projectile->field_69 | 2;
    }
    if (((Net_00499eb0*)*(void**)(g_game + 0x391e9))->field_d48 && hostile && !unit) {
        if (projectile == *(Projectile_00499eb0**)(g_game + 0x142f7)) {
            *(Vec3_00499eb0*)(g_game + 0x1433f) = (*(Projectile_00499eb0**)(g_game + 0x142f7))->position;
            *(unsigned short*)(g_game + 0x1434b) = *(unsigned short*)((char*)projectile->type + 0xfe);
            *(Projectile_00499eb0**)(g_game + 0x142f7) = 0;
        }
        projectile->field_69 = projectile->field_69 | 2;
        return;
    }
    FUN_0041c640(type->field_cc, type->field_cc, type->field_d0);
    if (hostile && !unit) {
        PlaySoundAt(type->sound2, position, 0);
        AddExplosionEffect(position, type->field_7c, 0, hostile);
    } else {
        PlaySoundAt(type->sound1, position, 0);
        if (type->flags.bit10)
            EmitWhiteSmoke(position, 9);
        else
            AddExplosionEffect(position, type->field_78, 0, hostile);
    }
    unsigned int player = projectile->owner;
    Player_00499eb0* record = (Player_00499eb0*)(g_game + player * 0x14b + 0x1b63);
    if (!record->field_0 || record->state != 3) {
        if (type->field_d6 <= 0x10 && unit) {
            int damage = ApplyWeaponDamage(projectile, unit, 1.0f);
            Unit* source = projectile->unit;
            if (source) {
                int a = 0;
                int b = 0;
                if (projectile->owner != unit->owner)
                    a = damage;
                else
                    b = damage;
                source->field_96->object->FUN_00406f50(projectile, a & 0xffff, b & 0xffff);
                return;
            }
        } else {
            ApplyAreaDamage(projectile, position);
        }
    }
}
