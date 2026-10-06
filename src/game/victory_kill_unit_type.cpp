// Decompiled by Opus. Names are provisional.

#include <string.h>

#pragma pack(push, 1)
struct Info_0048f0f0 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Unit {
    char unknown_0[0x92];
    Info_0048f0f0* info;               // +0x92
    char unknown_96[0xff - 0x96];
    unsigned char kind;                // +0xff
};
#pragma pack(pop)

void __stdcall PlaySoundByName(char* str, int flag);

class Condition_0048f0f0 {
public:
    virtual int IsSatisfied();           // IsSatisfied
    virtual void OnUnitDied(Unit* unit);
    int satisfied;                       // +0x04
    int celebrated;                      // +0x08
};

#include "../util/hapi_bank.h"

extern char DAT_00509028[]; // "VictoryCondition_KillUnitType"
extern char DAT_00509018[]; // "NumLeftToKill"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class VictoryKillUnitType : public Condition_0048f0f0 {
public:
    char name[0x20];                   // +0x0c
    int numLeftToKill;                 // +0x2c

    virtual void OnUnitDied(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};

// Slot 1 of the "kill unit type" victory condition (vtable 0x4fd8b0, state
// saved by 0x48f160): called with a unit; counts the named unit type down and
// announces the victory condition when the count runs out.
// FUNCTION: 0x48f0f0
void VictoryKillUnitType::OnUnitDied(Unit* unit)
{
    if (numLeftToKill > 0 && unit->kind == 1 && _strcmpi(name, unit->info->name) == 0) {
        if (--numLeftToKill <= 0) {
            satisfied = 1;
            if (celebrated == 0) {
                PlaySoundByName("Victory Condition", 0);
                celebrated = 1;
            }
        }
    }
}

// FUNCTION: 0x48f160
void VictoryKillUnitType::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00509028);
    obj->SetIntegerItem(DAT_00509018, numLeftToKill);
    obj->SetIntegerItem(DAT_00508f30, satisfied);
    obj->SetIntegerItem(DAT_00508f24, celebrated);
}

// Load counterpart of 0x48f160 (the "kill unit type" victory condition).
// FUNCTION: 0x48f1b0
void VictoryKillUnitType::LoadState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00509028);
    numLeftToKill = obj->GetIntegerItem(DAT_00509018, 0);
    satisfied = obj->GetIntegerItem(DAT_00508f30, 0);
    celebrated = obj->GetIntegerItem(DAT_00508f24, 0);
}
