// Decompiled by Opus. Names are provisional.
// Slot 0 of the unit visitor at +0xc of the "unit type passes Z" victory
// condition (visitor vtable 0x4fd848, driven by 0x48f530).
#include <string.h>
#include <stdlib.h>

struct UnitType_0048f4c0 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x78];
    short field_78;                    // +0x78
    char unknown_7a[0x92 - 0x7a];
    UnitType_0048f4c0* type;           // +0x92
};
#pragma pack(pop)

void __stdcall FUN_0047f1a0(char* str, int flag);

class Condition_0048f4c0 {
public:
    virtual int FUN_0048ea00();          // IsSatisfied
    int done;                          // +0x04
    int announced;                     // +0x08
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048f4c0 {
public:
    virtual int FUN_0048f790(Unit* unit) = 0;
};

// Same layout as 0x48ed50.cpp; this method overrides the visitor's slot, so
// `this` is the visitor subobject (+0xc) and done/announced sit at -8/-4.
#pragma pack(push, 2)
class Class_0048f530 : public Condition_0048f4c0, public UnitVisitor_0048f4c0 {
public:
    char name[0x20];                   // +0x10
    int field_30;                      // +0x30
    virtual int FUN_0048f790(Unit* unit);
};
#pragma pack(pop)

// FUNCTION: 0x48f4c0
int Class_0048f530::FUN_0048f790(Unit* unit)
{
    if (name[0] == 0 || _strcmpi(name, unit->type->name) == 0) {
        if (abs(unit->field_78 - field_30) <= 2) {
            done = 1;
            if (announced == 0) {
                FUN_0047f1a0("Victory Condition", 0);
                announced = 1;
            }
        }
    }
    return done == 0;
}
