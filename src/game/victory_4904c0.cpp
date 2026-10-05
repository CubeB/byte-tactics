// Decompiled by Opus. Names are provisional.
// Calls virtual slot 1 with `param` on every object in two fixed arrays.

class Item_004904c0 {
public:
    virtual void Slot0();
    virtual void Slot1(int param);
};

class Class_004904c0 {
public:
    Item_004904c0* a[16];               // +0x0
    int count_a;                        // +0x40
    Item_004904c0* b[16];               // +0x44
    int count_b;                        // +0x84

    void NotifyUnitDied(int param);
};

// FUNCTION: 0x4904c0
void Class_004904c0::NotifyUnitDied(int param)
{
    int i;
    for (i = 0; i < count_a; i++)
        a[i]->Slot1(param);
    for (i = 0; i < count_b; i++)
        b[i]->Slot1(param);
}
