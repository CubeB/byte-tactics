// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Damage a weapon does to a target. The weapon's def holds a base damage
// (+0xd4) and a name-keyed damage table (+0x64); the target's unit type name
// is looked up in it (the same sorted (name, value) array and lower_bound as
// 0x4c4630). The result is scaled, boosted by the attacker's armour
// (6% per point, capped at 5 points), doubled or halved by two global flags,
// and handed to FUN_00489bb0 together with the angle from the weapon to the
// target minus the target's heading.
#include <string.h>

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

struct Unit {
    char unknown_0[0x66];
    short heading;                  // +0x66
    short unknown_68;               // +0x68
    int x;                          // +0x6a
    int y;                          // +0x6e
    int z;                          // +0x72
    char unknown_76[0x92 - 0x76];
    UnitDef_00499cd0* def;          // +0x92
    char unknown_96[0xb8 - 0x96];
    unsigned short armour;          // +0xb8
    char unknown_ba[0xff - 0xba];
    unsigned char owner;            // +0xff
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

extern char* g_game;

short __cdecl FUN_004b715a(int x, int z);
void __stdcall FUN_00489bb0(Unit* source, Unit* target,
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

// FUNCTION: 0x499cd0
int __stdcall FUN_00499cd0(Weapon_00499cd0* weapon, Unit* target,
                           float scale)
{
    Def_00499cd0* def = weapon->def;
    int damage = def->field_d4;
    Table_00499cd0* table = def->table;
    if (table) {
        int* p = Find_00499cd0(table, target->def->name);
        if (p)
            damage = *p;
    }
    damage = (int)(damage * scale);
    short angle = FUN_004b715a(weapon->x - target->x,
                               weapon->z - target->z) - target->heading;
    Unit* attacker = weapon->attacker;
    if (attacker) {
        int armour = attacker->armour / 5;
        if (armour > 5)
            armour = 5;
        damage = (armour * 6 + 100) * damage / 100;
    }
    Flags_00499cd0* flags = (Flags_00499cd0*)(g_game + 0x37f2f);
    if (flags->flag7)
        damage *= 2;
    if (flags->flag8)
        damage /= 2;
    bool veteran = (weapon->def->flags >> 7) & 1;
    int type = veteran ? 2 : 1;
    FUN_00489bb0(attacker, target, damage, type, angle);
    return damage;
}
