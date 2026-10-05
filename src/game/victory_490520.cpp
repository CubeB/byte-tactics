// Decompiled by Opus. Names are provisional.
// Calls the third virtual method of every object in two lists.

class Item_00490520 {
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2(int param_1);
};

class MissionConditions {
public:
    Item_00490520* first[16];          // +0x0
    int firstCount;                    // +0x40
    Item_00490520* second[16];         // +0x44
    int secondCount;                   // +0x84

    void NotifyUnitCaptured(int param_1);
};

// FUNCTION: 0x490520
void MissionConditions::NotifyUnitCaptured(int param_1)
{
    int i;
    for (i = 0; i < firstCount; i++) {
        first[i]->Slot2(param_1);
    }
    for (i = 0; i < secondCount; i++) {
        second[i]->Slot2(param_1);
    }
}
