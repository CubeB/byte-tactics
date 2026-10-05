// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct UnitType_0043cbb0 {
    char unknown_0[0x1ba];
    unsigned short max_turn;           // +0x1ba
};

struct Unit {
    char unknown_0[0x66];
    short heading;                     // +0x66
    char unknown_68[0x92 - 0x68];
    UnitType_0043cbb0* type;           // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags_0 : 16;         // +0x110
    unsigned int moved : 1;            // +0x110 bit 16
    unsigned int flags_17 : 15;
};
#pragma pack(pop)

class Class_0043cbb0 {
public:
    char unknown_0[0x24];
    short turn;                        // +0x24

    void FUN_0043cbb0(Unit* unit, short amount);
};

// Clamps a turn amount to +-the unit type's limit, applies it to the unit's
// heading and flags the unit as moved.
// FUNCTION: 0x43cbb0
void Class_0043cbb0::FUN_0043cbb0(Unit* unit, short amount)
{
    if (amount != 0) {
        unsigned short max = unit->type->max_turn;
        if (amount >= max)
            turn = max;
        else if (amount <= -max)
            turn = -max;
        else
            turn = amount;
        unit->heading += turn;
        unit->moved = 1;
    } else {
        turn = 0;
    }
}
