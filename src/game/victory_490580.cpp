// Decompiled by Opus. Names are provisional.
// Calls the fourth virtual method of every object in two lists (the same
// object, g_game+0x391ed, as 0x490520, which calls the third).

class Item_00490520 {
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2(int param_1);
    virtual void Slot3(int param_1);
};

class Class_00490520 {
public:
    Item_00490520* first[16];          // +0x0
    int firstCount;                    // +0x40
    Item_00490520* second[16];         // +0x44
    int secondCount;                   // +0x84

    void FUN_00490520(int param_1);
    void FUN_00490580(int param_1);
};

// FUNCTION: 0x490580
void Class_00490520::FUN_00490580(int param_1)
{
    int i;
    for (i = 0; i < firstCount; i++) {
        first[i]->Slot3(param_1);
    }
    for (i = 0; i < secondCount; i++) {
        second[i]->Slot3(param_1);
    }
}
