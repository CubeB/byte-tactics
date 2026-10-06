// Decompiled by Opus. Names are provisional.

#include <string.h>

#pragma pack(push, 1)
struct Info_0048f8c0 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Unit {
    char unknown_0[0x92];
    Info_0048f8c0* info;               // +0x92
};
#pragma pack(pop)

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
    int GetIntegerItem(char* name, int def);
};

class DefeatUnitTypeKilled {
public:
    int satisfied;                     // +0x4
    int celebrated;                    // +0x8
    char name[0x20];                   // +0xc
    int numLeftToKill;                 // +0x2c

    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};

// Slot 1 of the "unit type killed" defeat condition (vtable 0x4fd7c0, state
// saved by 0x48f900): when a unit of the named type dies, counts it down and
// marks the condition satisfied once none are left.
// FUNCTION: 0x48f8c0
void DefeatUnitTypeKilled::OnUnitDied(Unit* unit)
{
    if (_strcmpi(name, unit->info->name) == 0) {
        if (--numLeftToKill <= 0) {
            satisfied = 1;
        }
    }
}

// FUNCTION: 0x48f900
void DefeatUnitTypeKilled::SaveState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_UnitTypeKilled");
    obj->SetIntegerItem("NumLeftToKill", numLeftToKill);
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Reads the "unit type killed" defeat condition's state from a section;
// the writing counterpart is 0x48f900, compare 0x48f880.
// FUNCTION: 0x48f950
void DefeatUnitTypeKilled::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_UnitTypeKilled");
    numLeftToKill = obj->GetIntegerItem("NumLeftToKill", 0);
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
