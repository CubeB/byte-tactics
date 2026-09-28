// Decompiled by GPT-5.6-Terra. Names are provisional.
// Partial: 85.1%; remaining differences are the early flag RMW and branch register allocation around sound/effect dispatch.
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
    unsigned int flags;
};

struct Unit_00499eb0 {
    char unknown_0[0x96];
    struct Holder_00499eb0* field_96;
    char unknown_9a[0xff - 0x9a];
    unsigned char owner;
};

struct Projectile_00499eb0 {
    ProjectileType_00499eb0* type;
    Vec3_00499eb0 position;
    char unknown_10[0x52 - 0x10];
    Unit_00499eb0* unit;
    char unknown_56[0x66 - 0x56];
    unsigned char owner;
    char unknown_67[2];
    unsigned char unknown_69 : 1;
    unsigned char flag_69 : 1;
    unsigned char unknown_69_rest : 6;
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

struct Class_00406f50 {
    void FUN_00406f50(Projectile_00499eb0* projectile, int a, int b);
};

struct Holder_00499eb0 {
    char unknown_0[0x74];
    Class_00406f50* object;
};

#pragma pack(pop)

extern char* g_game;

void* __stdcall FUN_004815a0(Vec3_00499eb0* position);
void __stdcall FUN_0041c640(int a, int b, int c);
void __stdcall FUN_00420a30(Vec3_00499eb0* position, void* value, int a, int b);
void __stdcall FUN_00472810(Vec3_00499eb0* position, int value);
void __stdcall FUN_0047f300(unsigned short sound, Vec3_00499eb0* position, int value);
int __stdcall FUN_00499cd0(Projectile_00499eb0* projectile, Unit_00499eb0* unit, float scale);
void __stdcall FUN_0049a120(Projectile_00499eb0* projectile, Vec3_00499eb0* position);

// FUNCTION: 0x499eb0
void __stdcall FUN_00499eb0(Projectile_00499eb0* projectile, Unit_00499eb0* unit)
{
    int hostile = 0;
    ProjectileType_00499eb0* type = projectile->type;
    Vec3_00499eb0* position = &projectile->position;
    unsigned char* value = (unsigned char*)FUN_004815a0(position);
    if (value != 0)
        hostile = value[5] < *(unsigned char*)(g_game + 0x1427f);
    if (!(type->flags & 0x400000)) {
        if (projectile == *(Projectile_00499eb0**)(g_game + 0x142f7)) {
            *(Vec3_00499eb0*)(g_game + 0x1433f) = (*(Projectile_00499eb0**)(g_game + 0x142f7))->position;
            *(unsigned short*)(g_game + 0x1434b) = *(unsigned short*)((char*)projectile->type + 0xfe);
            *(Projectile_00499eb0**)(g_game + 0x142f7) = 0;
        }
        projectile->flag_69 = 1;
    }
    if (((Net_00499eb0*)*(void**)(g_game + 0x391e9))->field_d48 && hostile && !unit) {
        if (projectile == *(Projectile_00499eb0**)(g_game + 0x142f7)) {
            *(Vec3_00499eb0*)(g_game + 0x1433f) = (*(Projectile_00499eb0**)(g_game + 0x142f7))->position;
            *(unsigned short*)(g_game + 0x1434b) = *(unsigned short*)((char*)projectile->type + 0xfe);
            *(Projectile_00499eb0**)(g_game + 0x142f7) = 0;
        }
        projectile->flag_69 = 1;
        return;
    }
    FUN_0041c640(type->field_cc, type->field_cc, type->field_d0);
    if (hostile && !unit) {
        unsigned int sound = 0;
        sound = type->sound2;
        FUN_0047f300(sound, position, 0);
        FUN_00420a30(position, type->field_7c, 0, hostile);
    } else {
        unsigned int sound = 0;
        sound = type->sound1;
        FUN_0047f300(sound, position, 0);
        unsigned int effect = type->flags;
        effect >>= 10;
        if (effect & 1)
            FUN_00472810(position, 9);
        else
            FUN_00420a30(position, type->field_78, 0, hostile);
    }
    unsigned int player = projectile->owner;
    Player_00499eb0* record = (Player_00499eb0*)(g_game + player * 0x14b + 0x1b63);
    if (!record->field_0 || record->state != 3) {
        if (type->field_d6 <= 0x10 && unit) {
            int damage = FUN_00499cd0(projectile, unit, 1.0f);
            Unit_00499eb0* source = projectile->unit;
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
            FUN_0049a120(projectile, position);
        }
    }
}
