// Decompiled by Opus. Names are provisional.
// Creates the unit's 0x2f-byte object (constructor 0x43dc00) and copies a
// value from the unit type.

struct Unit;

class Class_0043dc00 {
public:
    char unknown_0[0x2f];

    Class_0043dc00(Unit* unit);
};

#pragma pack(push, 1)
struct UnitType_00485e50 {
    char unknown_0[0x210];
    short field_210;                   // +0x210
};

struct Unit {
    Class_0043dc00* obj;               // +0x0
    char unknown_4[0x66 - 0x4];
    short field_66;                    // +0x66
    char unknown_68[0x92 - 0x68];
    UnitType_00485e50* type;           // +0x92
};
#pragma pack(pop)

// FUNCTION: 0x485e50
void __stdcall FUN_00485e50(Unit* unit)
{
    unit->obj = new Class_0043dc00(unit);
    unit->field_66 = unit->type->field_210;
}
