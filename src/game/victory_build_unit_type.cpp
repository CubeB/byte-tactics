// Decompiled by Opus. Names are provisional.

struct Unit {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x104 - 0xa8];
    float field_104;                   // +0x104
    char unknown_108[0x118 - 0x108];
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048ed50 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

struct UnitRange_0048edb0 {
    Unit* begin;                       // +0x0
    Unit* end;                         // +0x4 (last unit, inclusive)

    void ForEach(UnitVisitor_0048ed50* visitor)
    {
        for (Unit* u = begin; u <= end; u++) {
            if (u->field_a6 != 0) {
                int result = visitor->VisitUnit(u);
                if (!result) {
                    break;
                }
            }
        }
    }
};

#pragma pack(push, 2)
struct Game {
    char unknown_0[0x1bca];
    UnitRange_0048edb0 units;          // +0x1bca
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall PlaySoundByName(char* str, int flag);
short __stdcall FindUnitTypeId(char* name);

#include "../util/hapi_bank.h"

extern char DAT_00508fb4[]; // "VictoryCondition_BuildUnitType"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class Condition_0048ed50 {
public:
    virtual int IsSatisfied();         // IsSatisfied
    int satisfied;                     // +0x04
    int celebrated;                    // +0x08
};

// The same layout as the other conditions with a unit visitor. VisitUnit
// overrides the visitor's slot, so MSVC passes `this` as the visitor
// subobject (+0xc) and the other fields appear at negative offsets.
#pragma pack(push, 2)
class VictoryBuildUnitType : public Condition_0048ed50, public UnitVisitor_0048ed50 {
public:
    char name[0x20];                   // +0x10
    short id;                          // +0x30
    virtual int IsSatisfied();
    virtual int VisitUnit(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};
#pragma pack(pop)

// Slot 0 of the unit visitor at +0xc of the "build unit type" victory
// condition (visitor vtable 0x4fd900, driven by 0x48edb0).
// FUNCTION: 0x48ed50
int VictoryBuildUnitType::VisitUnit(Unit* unit)
{
    if (unit->field_a6 == id && unit->field_104 == 0.0f) {
        satisfied = 1;
        if (celebrated == 0) {
            PlaySoundByName("Victory Condition", 0);
            celebrated = 1;
        }
    }
    return satisfied == 0;
}

// FUNCTION: 0x48edb0
int VictoryBuildUnitType::IsSatisfied()
{
    if (satisfied != 0) {
        return 1;
    }
    if (id == 0) {
        id = FindUnitTypeId(name);
    }
    g_game->units.ForEach(this);
    return satisfied;
}

// Same shape as 0x48eb80: writes the "build unit type" victory condition's
// state to a section.
// FUNCTION: 0x48ee30
void VictoryBuildUnitType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_BuildUnitType");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Reads the "build unit type" victory condition's state from a section; the
// writing counterpart is 0x48ee30, same shape as 0x48eb00.
// FUNCTION: 0x48ee70
void VictoryBuildUnitType::LoadState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508fb4);
    satisfied = obj->GetIntegerItem(DAT_00508f30, 0);
    celebrated = obj->GetIntegerItem(DAT_00508f24, 0);
}
