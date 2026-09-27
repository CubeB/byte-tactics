// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Info_0048f250 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Unit_0048f250 {
    char unknown_0[0x86];
    Unit_0048f250* owner;              // +0x86
    char unknown_8a[0x92 - 0x8a];
    Info_0048f250* info;               // +0x92
    char unknown_96[0xfb - 0x96];
    int field_fb;                      // +0xfb
    unsigned char kind;                // +0xff
    char unknown_100[0x104 - 0x100];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                // +0x110
};
#pragma pack(pop)

void __stdcall FUN_0047f1a0(char* str, int flag);

class Condition_0048f250 {
public:
    virtual void Unknown_0();
    int done;                          // +0x04
    int announced;                     // +0x08
};

// Interface at +0xc of the object (see 0x48ed50.cpp): this method overrides
// its slot, so `this` is the subobject and the condition's fields appear at
// negative offsets.
class UnitVisitor_0048f250 {
public:
    virtual void FUN_0048f250(Unit_0048f250* unit) = 0;
};

#pragma pack(push, 2)
class Class_0048f250 : public Condition_0048f250, public UnitVisitor_0048f250 {
public:
    char name[0x20];                   // +0x10
    void FUN_0048f250(Unit_0048f250* unit);
};
#pragma pack(pop)

// FUNCTION: 0x48f250
void Class_0048f250::FUN_0048f250(Unit_0048f250* unit)
{
    if (unit->kind == 0) {
        if (name[0] != 0 && _strcmpi(name, unit->info->name) != 0) {
            return;
        }
        if ((unit->flags & 0x20) && unit->field_104 == 0.0f && unit->field_fb == 0
            && (unit->owner == 0 || (unit->owner->flags & 0x40000000))) {
            done = 1;
            if (announced == 0) {
                FUN_0047f1a0("Victory Condition", 0);
                announced = 1;
            }
        }
    }
}
