// Decompiled by space-bunny-free. Names are provisional.
// Works out how much damage one unit does to another and hands the result to
// ApplyUnitDamage as a 9 byte record. Damage type 10 skips the whole calculation
// and passes the amount through; any other type scales the amount by the
// target's 16.16 damage scale and then takes off armour, four percent per
// point of (armour / 5), capped at five points.
// Afterwards the damage is spread to every unit of the target's kind, but only
// when the target's kind is flagged (f0 set, f73 == 3) and the type is not 11.
// Every caller found fills the low byte, so the record's
// byte +7 is zero on all of them; see the note in the bug list.

#pragma pack(push, 1)
struct Def_00489bb0 {
    char unknown_0[0x1aa];
    int f1aa;                      // +0x1aa, damage scale as 16.16 fixed point
};

struct Kind_00489bb0 {
    int f0;                        // +0x0, zero means the kind is not in use
    int f4;                        // +0x4, becomes the "who" of the spread damage
    char unknown_8[0x73 - 8];
    unsigned char f73;             // +0x73, 3 lets the damage spread to the kind
};

class Unit {
public:
    char unknown_0[0x92];
    Def_00489bb0* def;             // +0x92
    Kind_00489bb0* kind;           // +0x96
    char unknown_9a[0xa8 - 0x9a];
    short team;                    // +0xa8
    char unknown_aa[0xb8 - 0xaa];
    unsigned short f_b8;           // +0xb8, armour: divided by 5, capped at 5
    char unknown_ba[0x10e - 0xba];
    unsigned char f10e;            // +0x10e, bit 1 applies the damage scale
};

struct Dmg_00489bb0 {              // 9 bytes, the record ApplyUnitDamage takes
    unsigned char kind;            // +0x0, 11 here, never read by that callee
    short team_target;             // +0x1
    short team_source;             // +0x3
    short amount;                  // +0x5
    unsigned char extra;           // +0x7
    unsigned char type;            // +0x8, the damage type
};
#pragma pack(pop)

void __stdcall ApplyUnitDamage(Dmg_00489bb0* dmg);
void __stdcall BroadcastPacket(int who, Dmg_00489bb0* dmg, int size);
int __cdecl GetLocalDpid(void);

// FUNCTION: 0x489bb0
void __stdcall DamageUnit(Unit* source, Unit* target, int amount, int type, unsigned short extra)
{
    int dmg;
    if (type != 10) {
        if ((target->f10e & 2) && amount < 0x7530)
            amount = (int)(((__int64)target->def->f1aa * amount) >> 0x10);
        int armour = target->f_b8 / 5;
        if (armour > 5)
            armour = 5;
        dmg = (25 - armour) * amount * 4 / 100;
    } else {
        dmg = amount;
    }
    Dmg_00489bb0 d;
    d.kind = 11;
    d.team_target = !target ? 0 : target->team;
    d.team_source = !source ? 0 : source->team;
    d.amount = dmg;
    // extra is a 16-bit value of which only the high byte reaches the record.
    d.extra = (unsigned char)(extra >> 8);
    d.type = type;
    ApplyUnitDamage(&d);
    if (target->kind->f0 != 0 && target->kind->f73 == 3 && type != 11) {
        // The call is written in both arms: a ternary argument merges the tails.
        if (source)
            BroadcastPacket(source->kind->f4, &d, 9);
        else
            BroadcastPacket(GetLocalDpid(), &d, 9);
    }
}
