// Decompiled by Opus. Names are provisional.
// Same shape as 0x48f0b0.

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
};

// The "move unit to radius" victory condition; reads its state from a section.
class VictoryMoveUnitToRadius {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void LoadState(HapiBank* obj);
};

// FUNCTION: 0x48f330
void VictoryMoveUnitToRadius::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_MoveUnitToRadius");
    satisfied = ((HapiBank*)obj)->GetIntegerItem("Satisfied", 0);
    celebrated = ((HapiBank*)obj)->GetIntegerItem("Celebrated", 0);
}
