// Decompiled by Opus. Names are provisional.

#include <string.h>

#pragma pack(push, 1)
struct Info_0048eeb0 {
    char unknown_0[0x20];
    char name[0x20];                 // +0x20
};

struct Unit {
    char unknown_0[0x92];
    Info_0048eeb0* info;             // +0x92
    char unknown_96[0xff - 0x96];
    unsigned char kind;              // +0xff
};
#pragma pack(pop)

void __stdcall PlaySoundByName(char* str, int flag);

#include "../util/hapi_bank.h"

// The "capture unit type" victory condition (vtable 0x4fd8e8). Same family
// as VictoryBuildUnitType and VictoryKillAllOfType.
class VictoryCaptureUnitType {
public:
    int satisfied;                        // +0x04
    int celebrated;                       // +0x08
    char name[0x20];                      // +0x0c

    virtual void OnUnitCaptured(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};

// Slot 2 of the condition (state saved by 0x48ef00).
// FUNCTION: 0x48eeb0
void VictoryCaptureUnitType::OnUnitCaptured(Unit* unit)
{
    if (unit->kind == 1 && _strcmpi(name, unit->info->name) == 0) {
        satisfied = 1;
        if (celebrated == 0) {
            PlaySoundByName("Victory Condition", 0);
            celebrated = 1;
        }
    }
}

// Writes the "capture unit type" victory condition's state to a section;
// the reading counterpart is 0x48ef40, compare 0x48f840.
// FUNCTION: 0x48ef00
void VictoryCaptureUnitType::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_CaptureUnitType");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// FUNCTION: 0x48ef40
void VictoryCaptureUnitType::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_CaptureUnitType");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
