// Decompiled by Opus. Names are provisional.
// Writes the "move unit to radius" victory condition's state to a section
// (same shape as 0x48f070).

class HapiBank {
public:
    void OpenAccount(const char* name);
};

class Class_004b4630 {
public:
    void SetIntegerItem(const char* name, int value);
};

class VictoryMoveUnitToRadius {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual void SaveState(HapiBank* obj);
};

// FUNCTION: 0x48f2f0
void VictoryMoveUnitToRadius::SaveState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_MoveUnitToRadius");
    ((Class_004b4630*)obj)->SetIntegerItem("Satisfied", satisfied);
    ((Class_004b4630*)obj)->SetIntegerItem("Celebrated", celebrated);
}
