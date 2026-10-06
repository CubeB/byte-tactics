// Decompiled by Opus. Names are provisional.

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
    char unknown_96[0xa6 - 0x96];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};
#pragma pack(pop)

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048f4c0 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

struct UnitRange_0048f530 {
    Unit* begin;                       // +0x0
    Unit* end;                         // +0x4 (last unit, inclusive)

    void ForEach(UnitVisitor_0048f4c0* visitor)
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
    UnitRange_0048f530 units;          // +0x1bca
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall PlaySoundByName(char* str, int flag);

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
    void SetIntegerItem(const char* name, int value);
};

class Condition_0048f4c0 {
public:
    virtual int IsSatisfied();         // IsSatisfied
    int satisfied;                     // +0x04
    int celebrated;                    // +0x08
};

// Same layout as the condition of 0x48ed50. VisitUnit overrides the visitor's
// slot, so its `this` is the visitor subobject (+0xc).
#pragma pack(push, 2)
class VictoryUnitTypePassesZ : public Condition_0048f4c0, public UnitVisitor_0048f4c0 {
public:
    char name[0x20];                   // +0x10
    int field_30;                      // +0x30
    virtual int IsSatisfied();
    virtual int VisitUnit(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};
#pragma pack(pop)

// Slot 0 of the unit visitor at +0xc of the "unit type passes Z" victory
// condition (visitor vtable 0x4fd848, driven by 0x48f530).
// FUNCTION: 0x48f4c0
int VictoryUnitTypePassesZ::VisitUnit(Unit* unit)
{
    if (name[0] == 0 || _strcmpi(name, unit->type->name) == 0) {
        if (abs(unit->field_78 - field_30) <= 2) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
    return satisfied == 0;
}

// Slot 0 of the "unit type passes Z" victory condition (vtable 0x4fd850,
// visitor vtable 0x4fd848 holding 0x48f4c0; state saved by 0x48f590): until
// the condition is met, visits every live unit and returns whether it is met.
// FUNCTION: 0x48f530
int VictoryUnitTypePassesZ::IsSatisfied()
{
    if (satisfied == 0) {
        g_game->units.ForEach(this);
    }
    return satisfied;
}

// Writes the "unit type passes Z" victory condition's state to a section;
// the reading counterpart is 0x48f5d0 (compare 0x48f440).
// FUNCTION: 0x48f590
void VictoryUnitTypePassesZ::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_UnitTypePassesZ");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48f480: reads the "unit type passes Z" victory condition's
// state from a section.
// FUNCTION: 0x48f5d0
void VictoryUnitTypePassesZ::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_UnitTypePassesZ");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
