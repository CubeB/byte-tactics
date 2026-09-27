// Decompiled by Opus. Names are provisional.

struct Weapon_499c70;

class Class_00406f50 {
public:
    void FUN_00406f50(Weapon_499c70* weapon, int a, int b);
};

struct Player_499c70 {
    char unknown_0[0x74];
    Class_00406f50* field_74;          // +0x74
};

#pragma pack(push, 1)
struct Unit_499c70 {
    char unknown_0[0x96];
    Player_499c70* player;             // +0x96
    char unknown_9a[0xff - 0x9a];
    unsigned char owner;               // +0xff
};

struct Weapon_499c70 {
    char unknown_0[0x52];
    Unit_499c70* attacker;             // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char owner;               // +0x66
};
#pragma pack(pop)

unsigned short __stdcall FUN_00499cd0(Weapon_499c70* weapon, Unit_499c70* target, float scale);

// FUNCTION: 0x499c70
void __stdcall FUN_00499c70(Weapon_499c70* weapon, Unit_499c70* target)
{
    unsigned short damage = FUN_00499cd0(weapon, target, 1.0f);
    Unit_499c70* attacker = weapon->attacker;
    if (attacker) {
        unsigned short a = 0;
        unsigned short b = 0;
        if (weapon->owner != target->owner)
            a = damage;
        else
            b = damage;
        attacker->player->field_74->FUN_00406f50(weapon, a, b);
    }
}
