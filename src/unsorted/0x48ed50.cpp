// Decompiled by Opus. Names are provisional.
// Slot 0 of the unit visitor at +0xc of the "build unit type" victory
// condition (visitor vtable 0x4fd900, driven by 0x48edb0).

struct Unit {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x104 - 0xa8];
    float field_104;                   // +0x104
    char unknown_108[0x118 - 0x108];
};

void __stdcall FUN_0047f1a0(char* str, int flag);

class Condition_0048ed50 {
public:
    virtual int FUN_0048ea00();          // IsSatisfied
    int done;                          // +0x04
    int announced;                     // +0x08
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048ed50 {
public:
    virtual int FUN_0048f790(Unit* unit) = 0;
};

// The same layout as the classes in 0x48edb0.cpp and 0x48efb0.cpp. This
// method overrides the visitor's slot, so MSVC passes `this` as the visitor
// subobject (+0xc) and the other fields appear at negative offsets.
#pragma pack(push, 2)
class Class_0048edb0 : public Condition_0048ed50, public UnitVisitor_0048ed50 {
public:
    char name[0x20];                   // +0x10
    short id;                          // +0x30
    virtual int FUN_0048f790(Unit* unit);
};
#pragma pack(pop)

// FUNCTION: 0x48ed50
int Class_0048edb0::FUN_0048f790(Unit* unit)
{
    if (unit->field_a6 == id && unit->field_104 == 0.0f) {
        done = 1;
        if (announced == 0) {
            FUN_0047f1a0("Victory Condition", 0);
            announced = 1;
        }
    }
    return done == 0;
}
