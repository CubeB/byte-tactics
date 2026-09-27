// Decompiled by Space Bunny Free. Names are provisional.
// Steps every element of the vector at +0x8, dropping the ones the element's
// own test rejects (erase in place, so the element now at the cursor is tested
// again), then hands over to the virtual at +0x14, which asks the one at
// +0x10 to rebuild.
#include <vector>

#pragma pack(push, 1)
struct Game_00473170 {
    char unknown_0[0x38a47];
    int field_38a47;                      // +0x38a47
};
#pragma pack(pop)

extern Game_00473170* g_game;

class Class_00474580 {
public:
    char unknown_0[0x44];

    void FUN_00474580();
};

class Class_00474720 {
public:
    char unknown_0[0x44];

    int FUN_00474720(int param_1);
};

class Class_00473170 {
public:
    char unknown_0[8];
    std::vector<Class_00474580> items;   // +0x8 (_First +0x10, _Last +0x14)

    virtual void FUN_004717e0();         // +0x00
    virtual void FUN_00473170();         // +0x04
    virtual void FUN_00473250(int param_1); // +0x08
    virtual int FUN_00473290();          // +0x0c
    virtual void FUN_00474880();         // +0x10
    virtual int FUN_00473220();          // +0x14
    virtual void FUN_00474760();         // +0x18
};

// FUNCTION: 0x473170
void Class_00473170::FUN_00473170()
{
    std::vector<Class_00474580>::iterator it = items.begin();

    while (it != items.end()) {
        it->FUN_00474580();
        if (((Class_00474720*)it)->FUN_00474720(g_game->field_38a47)) {
            items.erase(it);
        } else {
            ++it;
        }
    }
    if (FUN_00473220()) {
        FUN_00474880();
    }
}
