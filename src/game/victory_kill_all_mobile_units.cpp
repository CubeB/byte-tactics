// Decompiled by Haiku and Opus. Names are provisional.

#pragma pack(push, 1)
struct Unit {
    int field_0;                       // +0x00
    char unknown_4[0xa6 - 0x4];
    short field_a6;                    // +0xa6
    char unknown_a8[0xff - 0xa8];
    unsigned char kind;                // +0xff
    char unknown_100[0x118 - 0x100];
};
#pragma pack(pop)

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048ec20 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

struct UnitList_0048ec20 {
    Unit* first;                       // +0x0
    Unit* last;                        // +0x4 (inclusive)

    void ForEach(UnitVisitor_0048ec20* visitor)
    {
        for (Unit* unit = first; unit <= last; unit++) {
            if (unit->field_a6 != 0) {
                int result = visitor->VisitUnit(unit);
                if (!result) {
                    break;
                }
            }
        }
    }
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1d15];
    UnitList_0048ec20 units;           // +0x1d15
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall PlaySoundByName(char* str, int flag);

#include "../util/hapi_bank.h"

extern char DAT_00508f90[]; // "VictoryCondition_KillAllMobileUnits"
extern char DAT_00508f84[]; // "NumUnits"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class Condition_0048ec20 {
public:
    virtual int IsSatisfied();         // IsSatisfied
    virtual void OnUnitDied(Unit* unit);
    int satisfied;                     // +0x04
    int celebrated;                    // +0x08
};

// VisitUnit overrides the visitor's slot, so its `this` is the visitor
// subobject (+0xc) and numUnits sits at +0x4 from it.
class VictoryKillAllMobileUnits : public Condition_0048ec20, public UnitVisitor_0048ec20 {
public:
    int numUnits;                      // +0x10
    virtual int VisitUnit(Unit* unit);
    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};

// Slot 0 of the unit visitor at +0xc of the "kill all mobile units" victory
// condition (visitor vtable 0x4fd920, driven by 0x48ec20): counts the units
// whose first field is set and returns whether at most one has been seen.
// FUNCTION: 0x48ec00
int VictoryKillAllMobileUnits::VisitUnit(Unit* unit)
{
    if (unit->field_0 != 0) {
        numUnits++;
    }
    return numUnits <= 1;
}

// Slot 1 of the "kill all mobile units" victory condition (vtable 0x4fd928,
// visitor vtable 0x4fd920 holding 0x48ec00; state saved by 0x48ecb0): counts
// the live units through the visitor and announces the victory condition
// when at most one is left.
// FUNCTION: 0x48ec20
void VictoryKillAllMobileUnits::OnUnitDied(Unit* unit)
{
    if (unit->kind == 1 && unit->field_0 != 0) {
        numUnits = 0;
        g_game->units.ForEach(this);
        if (numUnits <= 1) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
}

// FUNCTION: 0x48ecb0
void VictoryKillAllMobileUnits::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508f90);
    obj->SetIntegerItem(DAT_00508f84, numUnits);
    obj->SetIntegerItem(DAT_00508f30, satisfied);
    obj->SetIntegerItem(DAT_00508f24, celebrated);
}

// The load counterpart of 0x48ecb0.
// FUNCTION: 0x48ed00
void VictoryKillAllMobileUnits::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_KillAllMobileUnits");
    numUnits = obj->GetIntegerItem("NumUnits", 0);
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
