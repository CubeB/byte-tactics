// Decompiled by Haiku and Opus. Names are provisional.

#include <string.h>

#pragma pack(push, 1)
struct Info_48efb0 {
    char unknown_0[0x20];
    char name[0x20];                 // +0x20
};

struct Unit {
    char unknown_0[0x92];
    Info_48efb0* info;               // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                  // +0xa6
    char unknown_a8[0xff - 0xa8];
    unsigned char kind;              // +0xff
    char unknown_100[0x118 - 0x100];
};
#pragma pack(pop)

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_48efb0 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

struct UnitList_48efb0 {
    Unit* first;                     // +0x0
    Unit* last;                      // +0x4 (inclusive)

    void ForEach(UnitVisitor_48efb0* visitor)
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
    UnitList_48efb0 units;           // +0x1d15
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

short __stdcall FindUnitTypeId(char* name);
void __stdcall PlaySoundByName(char* str, int flag);

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
    void SetIntegerItem(const char* name, int value);
};

class Condition_48efb0 {
public:
    virtual int IsSatisfied();       // IsSatisfied
    virtual void OnUnitDied(Unit* unit) = 0;
    int satisfied;                   // +0x04
    int celebrated;                  // +0x08
};

// VisitUnit overrides the visitor's slot, so its `this` is the visitor
// subobject (+0xc) and id and count sit at +0x24 and +0x26 from it.
#pragma pack(push, 2)
class VictoryKillAllOfType : public Condition_48efb0, public UnitVisitor_48efb0 {
public:
    char name[0x20];                 // +0x10
    short id;                        // +0x30
    int count;                       // +0x32
    virtual int VisitUnit(Unit* unit);
    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};
#pragma pack(pop)

// Slot 0 of the unit visitor at +0xc of the "kill all of type" victory
// condition (visitor vtable 0x4fd8c8, driven by 0x48efb0): counts the units
// of the condition's type and returns whether at most one has been seen.
// FUNCTION: 0x48ef80
int VictoryKillAllOfType::VisitUnit(Unit* unit)
{
    if (unit->field_a6 == id) {
        count++;
    }
    return count <= 1;
}

// Slot 1 of the "kill all of type" victory condition (vtable 0x4fd8d0,
// visitor vtable 0x4fd8c8 holding 0x48ef80; state saved by 0x48f070).
// FUNCTION: 0x48efb0
void VictoryKillAllOfType::OnUnitDied(Unit* unit)
{
    if (satisfied == 0 && unit->kind == 1 && _strcmpi(name, unit->info->name) == 0) {
        id = FindUnitTypeId(name);
        count = 0;
        g_game->units.ForEach(this);
        if (count <= 1) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
}

// Writes the "kill all of type" victory condition's state to a section;
// the reading counterpart is 0x48f0b0, compare 0x48ef00.
// FUNCTION: 0x48f070
void VictoryKillAllOfType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_KillAllOfType");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48ef40.
// FUNCTION: 0x48f0b0
void VictoryKillAllOfType::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_KillAllOfType");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
