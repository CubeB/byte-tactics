// Decompiled by Opus. Names are provisional.

#include <string.h>

struct Vec3_0048f200 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

#pragma pack(push, 1)
struct Info_0048f250 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Unit {
    char unknown_0[0x86];
    Unit* owner;                       // +0x86
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

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048f250 {
public:
    virtual void VisitUnit(Unit* unit) = 0;
};

void __stdcall ClampWorldPosToTerrain(int x, int z, Vec3_0048f200* out);
void __stdcall VisitObjectsInRange(Vec3_0048f200* pos, int radius, UnitVisitor_0048f250* visitor);
void __stdcall PlaySoundByName(char* str, int flag);

#include "../util/hapi_bank.h"

class Condition_0048f200 {
public:
    virtual int IsSatisfied();         // IsSatisfied
    int satisfied;                     // +0x04
    int celebrated;                    // +0x08
};

// VisitUnit overrides the visitor's slot, so its `this` is the visitor
// subobject and the condition's fields appear at negative offsets.
#pragma pack(push, 2)
class VictoryMoveUnitToRadius : public Condition_0048f200, public UnitVisitor_0048f250 {
public:
    char name[0x20];                   // +0x10
    Vec3_0048f200 pos;                 // +0x30
    int radius;                        // +0x3c
    virtual int IsSatisfied();
    virtual void VisitUnit(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};
#pragma pack(pop)

// The "move unit to radius" victory condition (vtable 0x4fd890, visitor
// vtable 0x4fd888): slot 0 fills in the target height on first use
// (0x12345678 marks it unset), visits the units within the radius and
// returns whether the condition is met.
// FUNCTION: 0x48f200
int VictoryMoveUnitToRadius::IsSatisfied()
{
    if (pos.y == 0x12345678) {
        ClampWorldPosToTerrain(pos.x, pos.z, &pos);
    }
    VisitObjectsInRange(&pos, radius, this);
    return satisfied;
}

// Slot 0 of the unit visitor at +0xc of the "move unit to radius" victory
// condition (visitor vtable 0x4fd888, driven by 0x48f200).
// FUNCTION: 0x48f250
void VictoryMoveUnitToRadius::VisitUnit(Unit* unit)
{
    if (unit->kind == 0) {
        if (name[0] != 0 && _strcmpi(name, unit->info->name) != 0) {
            return;
        }
        if ((unit->flags & 0x20) && unit->field_104 == 0.0f && unit->field_fb == 0
            && (unit->owner == 0 || (unit->owner->flags & 0x40000000))) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
}

// Writes the "move unit to radius" victory condition's state to a section
// (same shape as 0x48f070).
// FUNCTION: 0x48f2f0
void VictoryMoveUnitToRadius::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_MoveUnitToRadius");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48f0b0.
// FUNCTION: 0x48f330
void VictoryMoveUnitToRadius::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_MoveUnitToRadius");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
