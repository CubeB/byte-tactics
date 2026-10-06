// Decompiled by Haiku, space-bunny-free and Opus. Names are provisional.

#include <string.h>

#pragma pack(push, 1)
struct Info_0048f9d0 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Unit {
    char unknown_0[0x92];
    Info_0048f9d0* info;               // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                    // +0xa6
    char unknown_a8[0xff - 0xa8];
    unsigned char kind;                // +0xff
    char unknown_100[0x118 - 0x100];
};
#pragma pack(pop)

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048f9d0 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

struct UnitList_0048f9d0 {
    Unit* first;                       // +0x0
    Unit* last;                        // +0x4 (inclusive)

    void ForEach(UnitVisitor_0048f9d0* visitor)
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
    char unknown_0[0x1bca];
    UnitList_0048f9d0 units;           // +0x1bca
    char unknown_1bd2[0x1d15 - 0x1bd2];
    UnitList_0048f9d0 units2;          // +0x1d15
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

short __stdcall FindUnitTypeId(char* name);

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
    void SetIntegerItem(const char* name, int value);
};

class Condition_0048f9d0 {
public:
    virtual int IsSatisfied();         // IsSatisfied
    virtual void OnUnitDied(Unit* unit) = 0;
    int satisfied;                     // +0x04
    int celebrated;                    // +0x08
};

// VisitUnit overrides the visitor's slot, so its `this` is the visitor
// subobject (+0xc) and id and count sit at +0x24 and +0x26 from it.
#pragma pack(push, 2)
class DefeatAllUnitsKilledOfType : public Condition_0048f9d0, public UnitVisitor_0048f9d0 {
public:
    char name[0x20];                   // +0x10
    short id;                          // +0x30
    int count;                         // +0x32
    virtual int VisitUnit(Unit* unit);
    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};
#pragma pack(pop)

// Slot 0 of the unit visitor at +0xc of the "all units killed of type"
// defeat condition (visitor vtable 0x4fd7d8, driven by 0x48f9d0): counts
// the units of the condition's type and returns whether at most one has
// been seen.
// FUNCTION: 0x48f9a0
int DefeatAllUnitsKilledOfType::VisitUnit(Unit* unit)
{
    if (unit->field_a6 == id) {
        count++;
    }
    return count <= 1;
}

// Slot 1 of the "all units killed of type" defeat condition (vtable
// 0x4fd7e0, visitor vtable 0x4fd7d8 holding 0x48f9a0; state saved by
// 0x48fab0). Like 0x48efb0, but it counts the live units of the two lists in
// g_game (the ones at +0x1bca and +0x1d15) and only then decides.
// Each ForEach call converts `this` to the visitor subobject at +0xc, and the
// compiler emits a null test of `this` per conversion, so a null `this` runs
// both loops with a null visitor pointer.
// FUNCTION: 0x48f9d0
void DefeatAllUnitsKilledOfType::OnUnitDied(Unit* unit)
{
    if (_strcmpi(name, unit->info->name) == 0) {
        id = FindUnitTypeId(name);
        count = 0;
        g_game->units.ForEach(this);
        g_game->units2.ForEach(this);
        if (count <= 1) {
            satisfied = 1;
        }
    }
}

// Same shape as 0x48eb80: writes the "all units killed of type" defeat
// condition's state to a section.
// FUNCTION: 0x48fab0
void DefeatAllUnitsKilledOfType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AllUnitsKilledOfType");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48ef40.
// FUNCTION: 0x48faf0
void DefeatAllUnitsKilledOfType::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AllUnitsKilledOfType");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
