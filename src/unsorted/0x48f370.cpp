// Decompiled by Opus. Names are provisional.
#include <string.h>
#include <stdlib.h>

struct UnitType_0048f370 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

#pragma pack(push, 1)
struct Unit_0048f370 {
    char unknown_0[0x76];
    short field_76;                    // +0x76
    char unknown_78[0x92 - 0x78];
    UnitType_0048f370* type;           // +0x92
};
#pragma pack(pop)

void __stdcall FUN_0047f1a0(char* str, int flag);

class Condition_0048f370 {
public:
    virtual void Unknown_0();
    int done;                          // +0x04
    int announced;                     // +0x08
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048f370 {
public:
    virtual int FUN_0048f370(Unit_0048f370* unit) = 0;
};

// Same layout as 0x48ed50.cpp; this method overrides the visitor's slot, so
// `this` is the visitor subobject (+0xc) and done/announced sit at -8/-4.
#pragma pack(push, 2)
class Class_0048f370 : public Condition_0048f370, public UnitVisitor_0048f370 {
public:
    char name[0x20];                   // +0x10
    int field_30;                      // +0x30
    int FUN_0048f370(Unit_0048f370* unit);
};
#pragma pack(pop)

// FUNCTION: 0x48f370
int Class_0048f370::FUN_0048f370(Unit_0048f370* unit)
{
    if (name[0] == 0 || _strcmpi(name, unit->type->name) == 0) {
        if (abs(unit->field_76 - field_30) <= 2) {
            done = 1;
            if (announced == 0) {
                FUN_0047f1a0("Victory Condition", 0);
                announced = 1;
            }
        }
    }
    return done == 0;
}
