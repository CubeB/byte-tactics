// Decompiled by Opus, deepseek-v4.1-flash, GPT-5.6-Terra, claude-sonnet, Space Bunny Free, GPT-6.1-sol, deepseek-v4.1, space-bunny-free, mimo-v2.6-pro, LongCat 2.5 Preview Free, Claude Opus 5.5 and Haiku. Names are provisional.
// The first part of the weapons module (0x499a30 to 0x49be60): the projectile
// array, the weapon fire and hit packets, weapon damage and area damage, the
// ballistic launch-angle solver, the weapon reach tests and the projectile
// compaction and hit packet. ApplyAreaDamage (0x49a120) is a gap region and
// stays in weapons_49a120.cpp; CheckProjectileCollision (0x49b090),
// UpdateProjectiles (0x49b720) and DrawProjectiles (0x49be60) stay in their own
// files: their register plans follow their old files' symbol ids.
#include <string.h>

#pragma pack(push, 1)

// The position type the unit, projectile and game views share: three 16.16
// fixed-point ints, the middle one also read as its high short (a cell
// coordinate), and `+=` so the update loop can step a position.
struct Vec3_0049b720 {
    int x;
    union {
        int y;
        struct {
            unsigned short lo;
            short hi;
        } yw;
    };
    int z;
    Vec3_0049b720& operator+=(const Vec3_0049b720& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
};

struct Unit;
struct Spot_0049a120;
struct FeatureDef_0049a120;
struct MapFeature_0049b090;
struct Cell_0049a120;

// The game state, as the weapon code sees it. The views keep one name each
// (0x49b090's limit is the sea level, 0x49ae20's tracked is the selected
// projectile); 0x49a120's feature pointer and 0x49b090's mapping, in its own
// file, are one union.
struct Game {
    char unknown_0[0xdcb];
    unsigned char palette[0x2a];       // +0xdcb
    char unknown_df5[0x2a43 - 0xdf5];
    unsigned char localPlayer;         // +0x2a43
    unsigned short flags_2a44;         // +0x2a44
    char unknown_2a46[0x141f3 - 0x2a46];
    int projectileCount;               // +0x141f3
    void* projectiles;                 // +0x141f7
    char unknown_141fb[0x1420b - 0x141fb];
    Spot_0049a120* spots;              // +0x1420b
    char unknown_1420f[0x14233 - 0x1420f];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14253 - 0x1423b];
    int featureCount;                  // +0x14253
    char unknown_14257[0x14263 - 0x14257];
    int gravity;                       // +0x14263
    char unknown_14267[0x1426f - 0x14267];
    union {
        FeatureDef_0049a120* features; // +0x1426f
        MapFeature_0049b090* mapping;  // +0x1426f
    };
    char unknown_14273[0x1427f - 0x14273];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280;
    unsigned char viewFlags;           // +0x14281
    char unknown_14282[0x14287 - 0x14282];
    Cell_0049a120* cells;              // +0x14287
    char unknown_1428b[0x142f7 - 0x1428b];
    void* selected;                    // +0x142f7
    char unknown_142fb[0x1431f - 0x142fb];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x1433f - 0x14327];
    Vec3_0049b720 lastPos;             // +0x1433f
    unsigned short lastSound;          // +0x1434b
    char unknown_1434d[0x14357 - 0x1434d];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x147bb - 0x1435b];
    void* gaf_147bb;                   // +0x147bb
    void* gaf_147bf;                   // +0x147bf
    void* gaf_147c3;                   // +0x147c3
    void* gaf_147c7;                   // +0x147c7
    void* gaf_147cb;                   // +0x147cb
    char unknown_147cf[0x147f3 - 0x147cf];
    void* gaf_147f3;                   // +0x147f3
    char unknown_147f7[0x1480f - 0x147f7];
    void* gaf_1480f;                   // +0x1480f
    char unknown_14813[0x1ab9b - 0x14813];
    void* field_1ab9b;                 // +0x1ab9b
    char unknown_1ab9f[0x37e27 - 0x1ab9f];
    char field_37e27[0x37ecc - 0x37e27];
    Vec3_0049b720 wind;                // +0x37ecc
    char unknown_37ed8[0x38a47 - 0x37ed8];
    unsigned int time;                 // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    void* net;                         // +0x391e9
};

#pragma pack(pop)

extern Game* g_game;

void* __cdecl FUN_004d83b0(char* name, unsigned int size);

// Allocates and clears the 0x7d64-byte weapon array, then resets its count.
// FUNCTION: 0x499a30
void AllocWeaponArray(void)
{
    g_game->projectiles = FUN_004d83b0("WEAPON ARRAY", 0x7d64);
    memset(g_game->projectiles, 0, 0x7d64);
    g_game->projectileCount = 0;
}

void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x499a80
void FreeWeaponArray(void)
{
    FUN_004d85a0(g_game->projectiles);
    g_game->projectiles = 0;
}

// A 16.16 fixed-point value: the whole part above the fraction.
#pragma pack(push, 1)
union Fixed_0049abb0 {
    int value;                                      // 16.16 fixed point
    struct {
        unsigned short fraction;
        short whole;
    } parts;
};

struct Vec3_0049abb0 {
    int x;
    Fixed_0049abb0 y;
    int z;
};

// One of a unit's three weapon slots: the weapon def/type pointer at +0x0.
struct Weapon_0049abb0 {
    void* def;                                      // +0x0
    char unknown_4[0x1c - 0x4];
};

struct Vec3_00499ab0 {
    int x;
    int y;
    int z;
};

struct Def_00499ab0 {
    char unknown_0[0x10a];
    unsigned char f10a;              // +0x10a
    char unknown_10b[0x111 - 0x10b];
    unsigned int f111;               // +0x111, bits 30 and 31
};

// The unit, as the weapon code sees it. The views disagree about the bytes at
// +0x4 (0x49b090 reads a position there, 0x499ab0 a def pointer), about
// +0x10 (0x499ab0 reads the fire state there) and about the names of +0x92
// (def/type) and +0x96 (player/holder); the rest keep one name (0x49b090's
// elev is the position's y). Size 0x118, the unit array's stride.
struct Unit {
    char unknown_0[4];
    union {
        Vec3_0049b720 field_4;         // +0x4, 0x49b090's position view
        struct {
            char unknown_4[8];
            Def_00499ab0* def;         // +0xc, 0x499ab0's def
        };
    };
    union {
        Weapon_0049abb0 weapons[3];    // +0x10, stride 0x1c
        struct {
            char unknown_10[6];
            unsigned short f16;        // +0x16
            unsigned short f18;        // +0x18
            unsigned char f1a;         // +0x1a
            unsigned char f1b;         // +0x1b
        };
    };
    char unknown_64[2];
    short heading;                     // +0x66
    short unknown_68;                  // +0x68
    Vec3_0049abb0 pos;                 // +0x6a
    char unknown_76[0x92 - 0x76];
    void* type;                        // +0x92
    void* player;                      // +0x96
    char unknown_9a[0xb8 - 0x9a];
    unsigned short armour;             // +0xb8
    char unknown_ba[0xff - 0xba];
    unsigned char owner;               // +0xff
    char unknown_100[0x110 - 0x100];
    unsigned int state;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Kind_00499ab0 {
    char unknown_0[4];
    int f4;                          // +0x4, the player / "who"
};

struct Obj_00499ab0 {
    char unknown_0[0x96];
    Kind_00499ab0* kind;             // +0x96
    char unknown_9a[0xa8 - 0x9a];
    unsigned short team;             // +0xa8
};

struct Packet_00499ab0 {
    unsigned char type;              // +0x0
    Vec3_00499ab0 a;                 // +0x1
    Vec3_00499ab0 b;                 // +0xd
    unsigned char f19;               // +0x19
    unsigned char flag : 1;          // +0x1a, bit 0
    unsigned short f1b;              // +0x1b
    unsigned short f1d;              // +0x1d
    unsigned short f1f;              // +0x1f
    unsigned short f21;              // +0x21
    unsigned char f23;               // +0x23
};

#pragma pack(pop)

int __stdcall BroadcastPacket(int player, void* data, int size);

// Builds the 0x24 byte "unit status" network message (type 0xd) and hands it
// to BroadcastPacket. Only sent while g_game's bit 0 flag is set. Sibling of
// 0x499ba0, which sends the same type and size.
//
// The byte at packet +0x1a is a 1 bit bitfield that is only ever read and
// write back by the assignment at the end, so its upper seven bits are never
// initialised here; see the note in the bug list.
// FUNCTION: 0x499ab0
void __stdcall SendWeaponFirePacket(Unit* unit, Obj_00499ab0* source,
                            Obj_00499ab0* target, Vec3_00499ab0* a,
                            Vec3_00499ab0* b)
{
    Packet_00499ab0 packet;
    if (g_game->flags_2a44 & 1) {
        packet.type = 0xd;
        packet.a = *a;
        packet.b = *b;
        packet.f19 = unit->def->f10a;
        packet.f23 = (unit->f1b >> 2) & 3;
        packet.f21 = !source ? 0 : source->team;
        packet.f1f = !target ? 0 : target->team;
        packet.f1b = unit->f16;
        packet.f1d = unit->f18;
        packet.flag = unit->def->f111 >> 30;
        BroadcastPacket(source->kind->f4, &packet, 0x24);
    }
}

struct Vec3_00499ba0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Packet_00499ba0 {
    unsigned char type;                // +0x0
    Vec3_00499ba0 a;                   // +0x1
    Vec3_00499ba0 b;                   // +0xd
    char field_19;                     // +0x19
    char unknown_1a[0x24 - 0x1a];
};
#pragma pack(pop)

int __cdecl GetLocalDpid();

// FUNCTION: 0x499ba0
void __stdcall FUN_00499ba0(char param_1, Vec3_00499ba0* a, Vec3_00499ba0* b)
{
    Packet_00499ba0 packet;
    if (g_game->flags_2a44 & 1) {
        packet.type = 0xd;
        packet.a = *a;
        packet.b = *b;
        packet.field_19 = param_1;
        BroadcastPacket(GetLocalDpid(), &packet, 0x24);
    }
}

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

#pragma pack(push, 2)
struct Object_00499c10 {
    char unknown_0[0x66];
    short heading;                     // +0x66
    char unknown_68[0x9a - 0x68];
    CobScript* anims;                  // +0x9a
};
#pragma pack(pop)

struct Source_00499c10 {
    char unknown_0[0x16];
    short heading;                     // +0x16
};

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

// FUNCTION: 0x499c10
void __stdcall FUN_00499c10(Object_00499c10* obj, Source_00499c10* src)
{
    short angle = src->heading - obj->heading;
    int a = -FUN_004b70ef(angle, 800);
    int b = -FUN_004b7123(angle, 800);
    obj->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);
}

struct Weapon_499c70;
struct Projectile_00499eb0;
struct Weapon_0049a120;

class SquadManager {
public:
    void FUN_00406f50(Weapon_499c70* weapon, int a, int b);
    void FUN_00406f50(Projectile_00499eb0* projectile, int a, int b);
    void FUN_00406f50(Weapon_0049a120* weapon, int enemyDamage, int friendlyDamage);
};

struct Player_499c70 {
    char unknown_0[0x74];
    SquadManager* field_74;            // +0x74
};

#pragma pack(push, 1)
struct Weapon_499c70 {
    char unknown_0[0x52];
    Unit* attacker;                    // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char owner;               // +0x66
};
#pragma pack(pop)

unsigned short __stdcall ApplyWeaponDamage(Weapon_499c70* weapon, Unit* target, float scale);

// FUNCTION: 0x499c70
void __stdcall ApplyWeaponHit(Weapon_499c70* weapon, Unit* target)
{
    unsigned short damage = ApplyWeaponDamage(weapon, target, 1.0f);
    Unit* attacker = weapon->attacker;
    if (attacker) {
        unsigned short a = 0;
        unsigned short b = 0;
        if (weapon->owner != target->owner)
            a = damage;
        else
            b = damage;
        ((Player_499c70*)attacker->player)->field_74->FUN_00406f50(weapon, a, b);
    }
}

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

struct Pair_00499cd0 {
    char* name;                     // +0x0
    int value;                      // +0x4
};

struct NameLess_00499cd0 {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

#pragma pack(push, 1)
struct Table_00499cd0 {
    char unknown_0[5];
    Pair_00499cd0* first;           // +0x5
    Pair_00499cd0* last;            // +0x9
};

struct Def_00499cd0 {
    char unknown_0[0x64];
    Table_00499cd0* table;          // +0x64
    char unknown_68[0xd4 - 0x68];
    unsigned short field_d4;        // +0xd4
    char unknown_d6[0x111 - 0xd6];
    unsigned int flags;             // +0x111
};

struct UnitDef_00499cd0 {
    char unknown_0[0x20];
    char name[1];                   // +0x20
};

struct Weapon_00499cd0 {
    Def_00499cd0* def;              // +0x0
    int x;                          // +0x4
    int y;                          // +0x8
    int z;                          // +0xc
    char unknown_10[0x52 - 0x10];
    Unit* attacker;                 // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char owner;            // +0x66
};

struct Flags_00499cd0 {
    unsigned short bits0_6 : 7;
    unsigned short flag7 : 1;       // bit 7, 0x80
    unsigned short flag8 : 1;       // bit 8, 0x100
    unsigned short rest : 7;
};
#pragma pack(pop)

short __cdecl FUN_004b715a(int x, int z);
void __stdcall DamageUnit(Unit* source, Unit* target,
                            int amount, int type, unsigned short extra);

static inline int* Find_00499cd0(Table_00499cd0* table, char* name)
{
    NameLess_00499cd0 less;
    Pair_00499cd0* first = table->first;
    Pair_00499cd0* last = table->last;
    while (first != last) {
        Pair_00499cd0* mid = first + (last - first) / 2;
        if (less(mid->name, name))
            first = mid + 1;
        else
            last = mid;
    }
    if (first == table->last || less(name, first->name))
        return 0;
    return &first->value;
}

// Damage a weapon does to a target. The weapon's def holds a base damage
// (+0xd4) and a name-keyed damage table (+0x64); the target's unit type name
// is looked up in it (the same sorted (name, value) array and lower_bound as
// 0x4c4630). The result is scaled, boosted by the attacker's armour
// (6% per point, capped at 5 points), doubled or halved by two global flags,
// and handed to DamageUnit together with the angle from the weapon to the
// target minus the target's heading.
// FUNCTION: 0x499cd0
int __stdcall ApplyWeaponDamage(Weapon_00499cd0* weapon, Unit* target,
                           float scale)
{
    Def_00499cd0* def = weapon->def;
    int damage = def->field_d4;
    Table_00499cd0* table = def->table;
    if (table) {
        int* p = Find_00499cd0(table, ((UnitDef_00499cd0*)target->type)->name);
        if (p)
            damage = *p;
    }
    damage = (int)(damage * scale);
    short angle = FUN_004b715a(weapon->x - target->pos.x,
                               weapon->z - target->pos.z) - target->heading;
    Unit* attacker = weapon->attacker;
    if (attacker) {
        int armour = attacker->armour / 5;
        if (armour > 5)
            armour = 5;
        damage = (armour * 6 + 100) * damage / 100;
    }
    Flags_00499cd0* flags = (Flags_00499cd0*)((char*)g_game + 0x37f2f);
    if (flags->flag7)
        damage *= 2;
    if (flags->flag8)
        damage /= 2;
    bool veteran = (weapon->def->flags >> 7) & 1;
    int type = veteran ? 2 : 1;
    DamageUnit(attacker, target, damage, type, angle);
    return damage;
}

struct UnitType_499e50 {
    char unknown_0[0xfe];
    short value;                 // +0xfe
};

struct Unit_499e50 {
    UnitType_499e50* type;       // +0
    Vec3_0049b720 pos;           // +4
    char unknown_10[0x69 - 0x10];
    unsigned char flags;         // +0x69
};

// FUNCTION: 0x499e50
void __stdcall FUN_00499e50(Unit_499e50* unit)
{
    if (unit == g_game->selected) {
        g_game->lastPos = ((Unit_499e50*)g_game->selected)->pos;
        g_game->lastSound = unit->type->value;
        g_game->selected = 0;
    }
    unit->flags |= 2;
}

#pragma pack(push, 1)

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
    // Bitfield struct: the bit tests need this form.
    struct {
        unsigned int bits0_9 : 10;
        unsigned int bit10 : 1;
        unsigned int bits11_21 : 11;
        unsigned int bit22 : 1;
        unsigned int bits23_31 : 9;
    } flags;
};

struct Projectile_00499eb0 {
    ProjectileType_00499eb0* type;
    Vec3_0049b720 position;
    char unknown_10[0x52 - 0x10];
    Unit* unit;
    char unknown_56[0x66 - 0x56];
    unsigned char owner;
    char unknown_67[2];
    // Stays unsigned short: gives the single byte OR with 2.
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

struct Holder_00499eb0 {
    char unknown_0[0x74];
    SquadManager* object;
};

#pragma pack(pop)

void* __stdcall GetMapCellAtPosition(Vec3_0049b720* position);
void __stdcall FUN_0041c640(int a, int b, int c);void __stdcall AddExplosionEffect(Vec3_0049b720* position, void* value, int a, int b);
void __stdcall EmitWhiteSmoke(Vec3_0049b720* position, int value);
// The sound id stays unsigned int: gives the original's zero extension.
void __stdcall PlaySoundAt(unsigned int sound, Vec3_0049b720* position, int value);
int __stdcall ApplyWeaponDamage(Projectile_00499eb0* projectile, Unit* unit, float scale);
void __stdcall ApplyAreaDamage(Projectile_00499eb0* projectile, Vec3_0049b720* position);

// FUNCTION: 0x499eb0
void __stdcall DetonateProjectile(Projectile_00499eb0* projectile, Unit* unit)
{
    int hostile = 0;
    ProjectileType_00499eb0* type = projectile->type;
    Vec3_0049b720* position = &projectile->position;
    unsigned char* value = (unsigned char*)GetMapCellAtPosition(position);
    if (value != 0)
        hostile = value[5] < g_game->seaLevel;
    if (!type->flags.bit22) {
        if (projectile == g_game->selected) {
            g_game->lastPos = ((Projectile_00499eb0*)g_game->selected)->position;
            g_game->lastSound = *(unsigned short*)((char*)projectile->type + 0xfe);
            g_game->selected = 0;
        }
        projectile->field_69 = projectile->field_69 | 2;
    }
    if (((Net_00499eb0*)g_game->net)->field_d48 && hostile && !unit) {
        if (projectile == g_game->selected) {
            g_game->lastPos = ((Projectile_00499eb0*)g_game->selected)->position;
            g_game->lastSound = *(unsigned short*)((char*)projectile->type + 0xfe);
            g_game->selected = 0;
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
    Player_00499eb0* record = (Player_00499eb0*)((char*)g_game + player * 0x14b + 0x1b63);
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
                ((Holder_00499eb0*)source->player)->object->FUN_00406f50(projectile, a & 0xffff, b & 0xffff);
                return;
            }
        } else {
            ApplyAreaDamage(projectile, position);
        }
    }
}

// Builds a zeroed 0x6b-byte parameter block (owner, position, kind 10) and
// hands it to ApplyAreaDamage (an ebp-framed __stdcall routine in a gap).

struct Vec3_0049a0c0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Params_0049a0c0 {
    void* owner;                       // +0x0
    Vec3_0049a0c0 pos;                 // +0x4
    char unknown_10[0x42];
    int field_52;                      // +0x52
    char unknown_56[0x10];
    char kind;                         // +0x66
    char unknown_67[4];
};
#pragma pack(pop)

void __stdcall ApplyAreaDamage(Params_0049a0c0* params, Vec3_0049a0c0* pos);

// FUNCTION: 0x49a0c0
void __stdcall ApplyAreaDamageAt(void* owner, Vec3_0049a0c0* pos)
{
    Params_0049a0c0 p;
    memset(&p, 0, sizeof(p));
    p.field_52 = 0;
    p.owner = owner;
    p.kind = 10;
    p.pos = *pos;
    ApplyAreaDamage(&p, pos);
}

// 0x49a120's views. The function itself is a gap region (one of
// data/functions.csv's 29 gap rows) and stays in weapons_49a120.cpp; its
// declarations stay here for the symbol ids the functions after it match at.
#include <math.h>

struct Vec3_0049a120 {
    int x;                             // +0x0 (16.16)
    int y;                             // +0x4
    int z;                             // +0x8
};

// A 16.16 fixed-point value and its whole part.
union Fixed_0049a120 {
    int raw;
    struct {
        unsigned short frac;
        short whole;
    } part;
};

struct FixedVec3_0049a120 {
    Fixed_0049a120 x;
    Fixed_0049a120 y;
    Fixed_0049a120 z;
};

struct CellPos_0049a120 {
    short x;
    short z;
};

#pragma pack(push, 1)
struct WeaponDef_0049a120 {
    char unknown_0[0xd6];
    unsigned short radius;             // +0xd6
    float edgeDamage;                  // +0xd8
    char unknown_dc[0x10a - 0xdc];
    unsigned char kind;                // +0x10a
    char unknown_10b[0x111 - 0x10b];
    union {
        unsigned int all;
        struct {
            unsigned int bits0_29 : 30;
            unsigned int detonatesWeapons : 1;
            unsigned int bit31 : 1;
        } bits;
    } flags;                           // +0x111
};

struct UnitDef_0049a120 {
    char unknown_0[0x15e];
    Vec3_0049a120 boxMin;              // +0x15e
    Vec3_0049a120 boxMax;              // +0x16a
};

struct Holder_0049a120;

struct Unit_0049a120 {
    char unknown_0[0x6a];
    Vec3_0049a120 pos;                 // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_0049a120* def;             // +0x92
    Holder_0049a120* holder;           // +0x96
    char unknown_9a[0xff - 0x9a];
    unsigned char owner;               // +0xff
    char unknown_100[0x118 - 0x100];
};

struct Weapon_0049a120 {
    WeaponDef_0049a120* def;           // +0x0
    Vec3_0049a120 pos;                 // +0x4
    char unknown_10[0x28 - 0x10];
    Vec3_0049a120 field_28;            // +0x28
    char unknown_34[0x52 - 0x34];
    Unit_0049a120* attacker;           // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char owner;               // +0x66
    char unknown_67[0x69 - 0x67];
    unsigned short flags;              // +0x69
};

struct Holder_0049a120 {
    char unknown_0[4];
    int playerId;                      // +0x4
    char unknown_8[0x74 - 8];
    SquadManager* object;              // +0x74
};

struct Cell_0049a120 {
    unsigned short unit;               // +0x0
    unsigned short unit2;              // +0x2
    char unknown_4[4];
    unsigned short feature;            // +0x8
    union {
        unsigned short spot;           // +0xa
        struct {
            unsigned char offsetZ;     // +0xa
            unsigned char offsetX;     // +0xb
        } origin;
    };
    unsigned char flags;               // +0xc
};

struct Spot_0049a120 {
    char unknown_0[8];
    Vec3_0049a120 pos;                 // +0x8
    char unknown_14[0x30 - 0x14];
};

struct FeatureDef_0049a120 {
    char unknown_0[0x100];
};

struct Packet_0049a120 {
    unsigned char type;
    Vec3_0049a120 pos;
    unsigned char kind;
};
#pragma pack(pop)

// The system headers come after the game's own declarations, and only these
// (lean windows.h, vector): the symbol ids they give cell, feature and g_game
// decide the spot and feature address arithmetic.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>

// What one explosion has already hit, so nothing takes damage twice.
struct Hits_0049a120 {
    int numUnits;
    int numFeatures;
    Unit_0049a120* units[20];
    Cell_0049a120* features[64];

    // Each returns 0 if the thing was hit already, else records it (while
    // there is room) and returns 1.
    int AddUnit(Unit_0049a120* unit)
    {
        for (int k = 0; k < numUnits; k++) {
            if (units[k] == unit)
                return 0;
        }
        if (numUnits < 20) {
            units[numUnits] = unit;
            numUnits++;
        }
        return 1;
    }
    int AddFeature(Cell_0049a120* cell)
    {
        for (int k = 0; k < numFeatures; k++) {
            if (features[k] == cell)
                return 0;
        }
        if (numFeatures < 64) {
            features[numFeatures] = cell;
            numFeatures++;
        }
        return 1;
    }
};

Cell_0049a120* __stdcall GetMapCell(int x, int y);
int __stdcall ApplyWeaponDamage(Weapon_0049a120* weapon, Unit_0049a120* target, float scale);
void __stdcall DetonateProjectile(Weapon_0049a120* weapon, Unit_0049a120* unit);
int __stdcall VectorLength(Vec3_0049a120* v);
Vec3_0049a120 __stdcall GetFootprintCentre(CellPos_0049a120* cell, FeatureDef_0049a120* def);
void __stdcall DamageFeature(Cell_0049a120* cell, int x, int z, WeaponDef_0049a120* def);
int __stdcall BroadcastPacket(int id, void* data, int size);

static inline int Length(Vec3_0049a120* v)
{
    double x = v->x;
    double y = v->y;
    double z = v->z;
    return (int)sqrt(x * x + y * y + z * z);
}

static inline Unit_0049a120* UnitFromId(unsigned short id)
{
    if (id == 0)
        return 0;
    return (Unit_0049a120*)&g_game->units[id];
}

static inline Vec3_0049a120 Sub(const Vec3_0049a120& a, const Vec3_0049a120& b)
{
    Vec3_0049a120 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

// <time.h> and <malloc.h> are here for their symbol ids: they decide
// SolveLaunchAngle's x87 schedule and the reach tests' register plans.
#include <time.h>
#include <malloc.h>

// The length of an integer vector, truncated to an int.

struct Vec3i_0049a850 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

// FUNCTION: 0x49a850
int __stdcall VectorLength(Vec3i_0049a850* v)
{
    double x = v->x;
    double y = v->y;
    double z = v->z;
    return (int)sqrt(x * x + y * y + z * z);
}

// Ballistic launch-angle solver: the two roots
// of the trajectory quadratic, each turned into a launch angle with
// acos(sqrt(root) / speed), pi/2 when a root is not positive.
#include <stdio.h>

// PI stays this literal and `use / PI` a separate statement from
// `use * 32768.0`: the constant pool depends on it.
#define PI 3.14159265358979

extern "C" double __cdecl _hypot(double x, double y);

// FUNCTION: 0x49a890
short __stdcall SolveLaunchAngle(int x, int height, int z, int speed, float angle)
{
    int g = g_game->gravity;
    int gg = g * g;
    // The redundant parentheses stay, with these exact counts: they steer the
    // x87 schedule. disc is expanded as (s2 + 2*gh) * s2 + h2 * gg.
    double distance = (((_hypot(x, z))));
    double d = distance * distance;
    double gh = (double)g * (double)height;
    double h2 = (((((double)height * (double)height))));
    double s2 = (double)speed * (double)speed;
    double sum = h2 + d;
    double disc = (((((s2 + 2*gh) * s2)) + h2 * gg)) * (d * d) - (((d * d * gg)) * sum);
    if (disc < 0.0)
        return 0x8000;
    disc = sqrt(disc);
    d = (s2 + gh) * d;
    double high = (disc + d) / (2*sum);
    double low = (d - disc) / (2*sum);
    double highAngle;
    double lowAngle;
    if (high > 0.0)
        highAngle = acos(sqrt(high) / (double)speed);
    else
        highAngle = PI / 2;
    if (low > 0.0)
        lowAngle = acos(sqrt(low) / (double)speed);
    else
        lowAngle = PI / 2;
    double use;
    if (angle < highAngle && highAngle <= PI / 4)
        use = highAngle;
    else if (lowAngle > angle && lowAngle <= PI / 4)
        use = lowAngle;
    else
        return 0x8000;
    use = use * 32768.0;
    return (short)(use / PI);
}

// 0x49aa80's file included <windows.h> and <stdio.h> here for the operand
// order in the height check; both are already included above.
#pragma pack(push, 1)

union Fixed_0049aa80 {
    int value;
    struct { unsigned short fraction; short whole; } parts;
};

struct Vec3_0049aa80 {
    int x;                             // +0x0 (16.16 fixed point)
    Fixed_0049aa80 y;                  // +0x4
    int z;                             // +0x8
};

struct WeaponDef_0049aa80 {
    char unknown_0[0x68];
    int field_68;                      // +0x68
    char unknown_6c[0xc8 - 0x6c];
    int field_c8;                      // +0xc8
    char unknown_cc[0xdc - 0xcc];
    int range;                         // +0xdc
    char unknown_e0[0x111 - 0xe0];
    struct {
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;         // tested here (line of fire)
        unsigned int bit2_15 : 14;
        unsigned int bit16 : 1;        // tested here (skip the team check)
        unsigned int bit17_31 : 15;
    } flags;                           // +0x111
};

struct UnitDef_0049aa80 {
    char unknown_0[0x170];
    short field_170;                   // +0x170
};

#pragma pack(pop)

short __stdcall SolveLaunchAngle(int dx, int dy, int dz, int a, int b);

// The subtraction of two points is a real by-value operator taking both
// operands by value and returning the point by value, and the line-of-fire test
// goes through a by-value helper, both exactly as in the matching sibling
// 0x49abb0 (whose MATCH this idiom produced). Those two boundaries are what put
// a2 in esi and a3 in edi for the whole function and keep the weapon def in
// ebx, which is the first block's register allocation (see the pass note at the
// top of the file).
inline Vec3_0049aa80 operator-(Vec3_0049aa80 a, Vec3_0049aa80 b)
{
    Vec3_0049aa80 r;
    r.x = b.x - a.x;
    r.y.value = b.y.value - a.y.value;
    r.z = b.z - a.z;
    return r;
}

static inline short LineOfFire_0049aa80(Vec3_0049aa80 to, Vec3_0049aa80 from, int s, int f)
{
    Vec3_0049aa80 d = from - to;
    return SolveLaunchAngle(d.x, d.y.value, d.z, s, f);
}

// FUNCTION: 0x49aa80
int __stdcall WeaponCanReachPos(Unit* a1, Vec3_0049aa80* a2, Vec3_0049aa80* a3, int a4)
{
    WeaponDef_0049aa80* wdef = (WeaponDef_0049aa80*)a1->weapons[a4 & 0xff].def;

    // z difference first, as named int locals: the original's order.
    int dz = a3->z - a2->z;
    int dx = a3->x - a2->x;
    if ((int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32) > wdef->range * wdef->range)
        return 0;

    if (wdef->flags.bit16)
        return 1;

    if (a2->y.parts.whole + ((UnitDef_0049aa80*)a1->type)->field_170 <= g_game->seaLevel)
        return 0;

    if (wdef->flags.bit1) {
        if (LineOfFire_0049aa80(*a2, *a3, wdef->field_68, wdef->field_c8) == (short)0x8000)
            return 0;
    }
    return 1;
}

// Both includes change the register choice in the sea-level tests.
#include <stdlib.h>

#pragma pack(push, 1)

struct WeaponDef_0049abb0 {
    char unknown_0[0x68];
    int field_68;                                   // +0x68
    char unknown_6c[0xc8 - 0x6c];
    int field_c8;                                 // +0xc8
    char unknown_cc[0xdc - 0xcc];
    int range;                                      // +0xdc
    char unknown_e0[0x111 - 0xe0];
    struct {
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;                      // line of fire
        unsigned int bit2_15 : 14;
        unsigned int bit16 : 1;                     // skip the ground test
        unsigned int bit17 : 1;                     // target must be landed
        unsigned int bit18_31 : 14;
    } flags;                                        // +0x111
};

struct UnitDef_0049abb0 {
    char unknown_0[0x170];
    short height;                                   // +0x170
    char unknown_172[0x241 - 0x172];
    struct {
        unsigned int bit0_11 : 12;
        unsigned int bit12 : 1;                     // half height counts
        unsigned int bit13_18 : 6;
        unsigned int bit19 : 1;                     // ignore sea level
        unsigned int bit20_31 : 12;
    } flags;                                        // +0x241
};

#pragma pack(pop)

// The vector from point `a` to point `b`. Both operands and the result are by
// value, which is what makes the line-of-fire block below compile the way the
// original does: MSVC builds the two 12-byte argument copies as one setup unit
// instead of turning the first aggregate into a live pointer, so `unit2` stays in
// ebx across the call and the second distance tail needs no reload. Note the
// operands are named in the order the callers pass them (shooter, target) while
// the result is target - shooter, because the copy order, not the arithmetic, is
// what the register allocation is sensitive to here.
inline Vec3_0049abb0 operator-(Vec3_0049abb0 a, Vec3_0049abb0 b)
{
    Vec3_0049abb0 r;
    r.x = b.x - a.x;
    r.y.value = b.y.value - a.y.value;
    r.z = b.z - a.z;
    return r;
}

// Can the shooter hit the target? Both positions are taken by value, which is
// what puts the 12-byte copies in the frame while the units themselves are read
// where they are. The parameters are named for the order the caller passes them
// in, and the subtraction gives the target relative to the shooter.
static inline short LineOfFire_0049abb0(Vec3_0049abb0 to, Vec3_0049abb0 from, int s, int f)
{
    Vec3_0049abb0 d = from - to;
    return SolveLaunchAngle(d.x, d.y.value, d.z, s, f);
}

static inline int Dist2_0049abb0(Vec3_0049abb0* b, Vec3_0049abb0* a)
{
    int dz = a->z - b->z;
    int dx = a->x - b->x;
    return (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
}

// FUNCTION: 0x49abb0
int __stdcall WeaponCanReachUnit(Unit* unit1, Unit* unit2, unsigned char weapon)
{
    WeaponDef_0049abb0* w = (WeaponDef_0049abb0*)unit1->weapons[weapon].def;

    if (w->flags.bit16) {
        if (!((UnitDef_0049abb0*)unit2->type)->flags.bit19 && unit2->pos.y.parts.whole > g_game->seaLevel)
            return 0;
        // Written (height >> 1) + whole: sets the add's operand order.
        if (((UnitDef_0049abb0*)unit2->type)->flags.bit12 && (((UnitDef_0049abb0*)unit2->type)->height >> 1) + unit2->pos.y.parts.whole > g_game->seaLevel)
            return 0;
        return Dist2_0049abb0(&unit1->pos, &unit2->pos) <= w->range * w->range;
    }

    if (unit1->pos.y.parts.whole + ((UnitDef_0049abb0*)unit1->type)->height <= g_game->seaLevel)
        return 0;
    if (unit2->pos.y.parts.whole + ((UnitDef_0049abb0*)unit2->type)->height <= g_game->seaLevel)
        return 0;
    if (w->flags.bit17 && (unit2->state & 3) != 2)
        return 0;
    // Nested ifs, not one && condition: the flag test codegen differs.
    if (w->flags.bit1) {
        if (LineOfFire_0049abb0(unit1->pos, unit2->pos, w->field_68, w->field_c8) == (short)0x8000)
            return 0;
    }
    return Dist2_0049abb0(&unit1->pos, &unit2->pos) <= w->range * w->range;
}

// FUNCTION: 0x49adf0
int __stdcall FUN_0049adf0(int param1, unsigned int param2)
{
    unsigned char idx = (unsigned char)param2;
    int offset = idx * 7;
    int* intermediate = (int*)((char*)param1 + offset * 4 + 0x10);
    int result = *(int*)((char*)*intermediate + 0xdc);
    return result;
}

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
#pragma pack(pop)

// Compacts the projectile array: entries whose flags bit 1 is set are dropped
// and the remaining ones are moved down. Each projectile's own old index is
// written to field_67 first, so the pointers held at field_56 (relinked to the
// moved targets in the second pass) can be resolved by searching for that
// index.
// FUNCTION: 0x49ae20
void CompactProjectiles()
{
    Projectile_0049ae20* projectiles = (Projectile_0049ae20*)g_game->projectiles;
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
                if (g_game->selected == p) {
                    g_game->selected = dest;
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
#pragma pack(pop)

void __stdcall DetonateProjectile(Projectile_0049af90* proj, int flag);

static inline int SamePos(const Vec3_0049af90& a, const Vec3_0049af90& b)
{
    return a.x == b.x && a.z == b.z && a.y == b.y;
}

// Finds the projectile a network packet refers to (by position and type) and
// removes it. The position compare is an inlined helper taking references;
// written inline, MSVC keeps one induction pointer instead of two.
// FUNCTION: 0x49af90
void __stdcall ApplyProjectileHitPacket(int unused, Packet_0049af90* p)
{
    Projectile_0049af90* proj = (Projectile_0049af90*)g_game->projectiles;
    for (int i = 0; i < g_game->projectileCount; i++, proj++) {
        if (SamePos(proj->pos, p->pos) && proj->type->id == p->typeId) {
            DetonateProjectile(proj, 0);
            return;
        }
    }
}

struct Weapon_0049b000;

struct UnitType_0049b000 {
    char unknown_0[0x220];
    Weapon_0049b000* weapon1;          // +0x220
    Weapon_0049b000* weapon2;          // +0x224
};

#pragma pack(push, 1)
struct Projectile_0049b000 {
    Weapon_0049b000* weapon;           // +0x0
    Vec3_0049abb0 pos;                 // +0x4
    Vec3_0049abb0 start;               // +0x10
    char unknown_1c[0x4e - 0x1c];
    int field_4e;                      // +0x4e
    int field_52;                      // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char owner;               // +0x66
    char unknown_67[0x6b - 0x67];
};
#pragma pack(pop)

void __stdcall DetonateProjectile(Projectile_0049b000* proj, int flag);

// Fires one of a unit's two weapons (by type) from the unit's position,
// building the projectile on the stack; see 0x49af90 for the projectile.
// FUNCTION: 0x49b000
void __stdcall DetonateUnitWeapon(Unit* unit, int second)
{
    Weapon_0049b000* weapon = second ? ((UnitType_0049b000*)unit->type)->weapon2
                                     : ((UnitType_0049b000*)unit->type)->weapon1;
    if (weapon) {
        Projectile_0049b000 proj;
        proj.weapon = weapon;
        proj.pos = unit->pos;
        proj.start = unit->pos;
        proj.field_4e = 0;
        proj.field_52 = 0;
        proj.owner = unit->owner;
        DetonateProjectile(&proj, 0);
    }
}

#pragma pack(push, 1)

#define max(a, b) (((a) > (b)) ? (a) : (b))

// 16.16 fixed point seen as the short above the short below.
union Fix_0049b3e0 {
    int whole;
    short half[2];
};

struct Vec3_0049b3e0 {
    int x, y, z;

    Vec3_0049b3e0 operator-(const Vec3_0049b3e0& o) const
    {
        Vec3_0049b3e0 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    Fix_0049b3e0 Length() const
    {
        double fx = x;
        double fy = y;
        double fz = z;
        Fix_0049b3e0 r;
        r.whole = (int)sqrt(fx * fx + fy * fy + fz * fz);
        return r;
    }
};

struct Weapon_0049b3e0 {
    char unknown_0[0x111];
    unsigned int flags;                // +0x111
};

struct Obj_0049b3e0 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

struct Proj_0049b3e0 {
    Weapon_0049b3e0* weapon;           // +0x0
    Vec3_0049b3e0 pos;                 // +0x4
    Vec3_0049b3e0 start;               // +0x10
    char unknown_1c[0x28 - 0x1c];
    Vec3_0049b3e0 target;              // +0x28
    char unknown_34[0x4e - 0x34];
    Obj_0049b3e0* field_4e;            // +0x4e
    char unknown_52[0x56 - 0x52];
    int* field_56;                     // +0x56
};
#pragma pack(pop)

int __stdcall GetGroundHeight(Vec3_0049b3e0* pos);

// FUNCTION: 0x49b3e0
Vec3_0049b3e0* __stdcall GetProjectileAimPoint(Proj_0049b3e0* p)
{
    unsigned char flag = (unsigned char)((p->weapon->flags >> 0x19) & 1);
    if (flag) {
        Fix_0049b3e0 dist = (p->pos - p->target).Length();
        if (dist.half[1] > 0x400) {
            p->start = p->target;
            ((Fix_0049b3e0*)&p->start.y)->half[1] = 0x2bc;
            return &p->start;
        }
    } else {
        if (p->field_56 != 0)
            return (Vec3_0049b3e0*)(p->field_56 + 1);
        Obj_0049b3e0* q = p->field_4e;
        if (q != 0 && (q->flags & 0x10000000) != 0)
            return (Vec3_0049b3e0*)((char*)q + 0x6a);
    }
    if (flag) {
        p->start = p->target;
        p->start.y = max(GetGroundHeight(&p->target), g_game->seaLevel) << 16;
        return &p->start;
    }
    return &p->target;
}

struct Vec3_0049b520 {
    int x, y, z;
};

#pragma pack(push, 1)
struct UnitType_0049b520 {
    char unknown_0[0xe8];
    unsigned short turnRate;           // +0xe8
    char unknown_ea[0x111 - 0xea];
    unsigned int flags;                // +0x111
};
#pragma pack(pop)

struct Unit_0049b520 {
    UnitType_0049b520* type;           // +0x0
    int x;                             // +0x4
    int y;                             // +0x8
    int z;                             // +0xc
    char unknown_10[0x36 - 0x10];
    short heading;                     // +0x36
    short pitch;                       // +0x38
};

// 16.16 value in a 4 byte union: an 8 byte one (or short[4]) changes the frame.
union Fixed_0049b520 {
    int value;
    struct { unsigned short fraction; short whole; };
};

// Aim a unit's two turret angles at a target point. The heading (+0x36) is
// atan2(dx, dz); the pitch (+0x38) comes from the vertical difference against
// the horizontal distance. Each angle is then moved towards the wanted one by
// at most the unit type's turn rate (+0xE8), and if the wanted angle is more
// than 27000 (about 148 degrees) away while the type's flag bit 22 of +0x111 is
// set, the function gives up and returns 0. Otherwise it returns 1.
// FUNCTION: 0x49b520
int __stdcall TurnUnitTowardsPoint(Unit_0049b520* unit, Vec3_0049b520* target)
{
    UnitType_0049b520* type = unit->type;
    // Declared dx, dy, dz in this order: it decides the stack slot assignment.
    int dx = unit->x - target->x;
    Fixed_0049b520 dy;
    dy.value = unit->y - target->y;
    int dz = unit->z - target->z;
    short a1 = FUN_004b715a(dx, dz);
    int dist = (int)_hypot((double)dx, (double)dz);
    short a2 = FUN_004b715a(-dy.whole, (short)(dist >> 16));

    short diff1 = a1 - unit->heading;
    short adiff1 = abs(diff1);
    if (adiff1 > 27000 && (type->flags & 0x800000))
        return 0;
    if (adiff1 < type->turnRate)
        unit->heading = a1;
    else if (diff1 < 0)
        unit->heading = unit->heading - type->turnRate;
    else
        unit->heading = unit->heading + type->turnRate;

    short diff2 = a2 - unit->pitch;
    short adiff2 = abs(diff2);
    if (adiff2 > 27000 && (type->flags & 0x800000))
        return 0;
    if (adiff2 < type->turnRate)
        unit->pitch = a2;
    else if (diff2 < 0)
        unit->pitch = unit->pitch - type->turnRate;
    else
        unit->pitch = unit->pitch + type->turnRate;
    return 1;
}

#pragma pack(push, 2)
struct Object_0049b680 {
    char unknown_0[0x1c];
    int x;                             // +0x1c
    int y;                             // +0x20
    int z;                             // +0x24
    char unknown_28[0x36 - 0x28];
    short angle1;                      // +0x36
    short angle2;                      // +0x38
    int length;                        // +0x3a
};
#pragma pack(pop)

// FUNCTION: 0x49b680
void __stdcall FUN_0049b680(Object_0049b680* obj)
{
    obj->y = FUN_004b70ef(obj->angle2, obj->length);
    int r = FUN_004b7123(obj->angle2, obj->length);
    obj->x = -FUN_004b70ef(obj->angle1, r);
    obj->z = -FUN_004b7123(obj->angle1, r);
}

#pragma pack(push, 1)
struct Projectile_0049b6e0 {
    char unknown_0[0x4e];
    int field_4e;                      // +0x4e
    char unknown_52[0x69 - 0x52];
    unsigned short flags;              // +0x69
};
#pragma pack(pop)

// Takes the next free entry of the 300-entry projectile array (see
// 0x499a30), resets two of its fields and returns it, or 0 when full.
// FUNCTION: 0x49b6e0
Projectile_0049b6e0* AllocProjectile()
{
    Projectile_0049b6e0* p = 0;
    if (g_game->projectileCount < 300) {
        p = &((Projectile_0049b6e0*)g_game->projectiles)[g_game->projectileCount++];
        p->flags &= ~2;
        p->field_4e = 0;
    }
    return p;
}

