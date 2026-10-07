// Decompiled by Sonnet and Opus. Names are provisional.

extern void* g_game;

#include "../util/hapi_bank.h"

class VictoryTimerRunsOut {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8
    unsigned int field_c;                // +0xc

    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};

// Slot 0 (IsSatisfied) of the "victory timer runs out" victory condition
// (vtable 0x4fd830): met once the game's tick count reaches the limit.
// FUNCTION: 0x48f610
int VictoryTimerRunsOut::IsSatisfied()
{
    unsigned int game_val = *(unsigned int*)((char*)g_game + 0x38a47);
    return game_val >= field_c;
}

// Same shape as 0x48fd70: writes the "victory timer runs out" victory
// condition's state to a section (its reader is 0x48f670).
// FUNCTION: 0x48f630
void VictoryTimerRunsOut::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_VictoryTimerRunsOut");
    obj->SetIntegerItem("Satisfied", satisfied);
    obj->SetIntegerItem("Celebrated", celebrated);
}

// Same shape as 0x48f480: reads the "victory timer runs out" victory
// condition's state from a section.
// FUNCTION: 0x48f670
void VictoryTimerRunsOut::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_VictoryTimerRunsOut");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
