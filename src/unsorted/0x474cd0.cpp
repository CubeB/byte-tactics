// Decompiled by Opus. Names are provisional.
// Constructor of the Class_00471cc0 subclass whose vtable is 0x4fd618 (its
// scalar deleting destructor is 0x474d10): an empty std::vector of 32-byte
// records at +0xc, and the current game time at +0x8.
#include <vector>

struct Record_00474cd0 {
    int unknown[8];
};

extern char* g_game;

class Class_00471cc0 {
public:
    Class_00471cc0();

    virtual void FUN_00474d10();
    virtual void FUN_00475340();
    virtual void FUN_00475470();
    virtual void FUN_00474f80();
    virtual void FUN_00474df0();
    virtual void FUN_00475440();
    virtual void FUN_00474d50();

    int field_4;                            // +0x04
};

class Class_00474cd0 : public Class_00471cc0 {
public:
    int time;                               // +0x08
    std::vector<Record_00474cd0> records;   // +0x0c (_First +0x10, _Last +0x14, _End +0x18)

    Class_00474cd0();

    virtual void FUN_00474d10();
    virtual void FUN_00475340();
    virtual void FUN_00475470();
    virtual void FUN_00474f80();
    virtual void FUN_00474df0();
    virtual void FUN_00475440();
    virtual void FUN_00474d50();
};

// FUNCTION: 0x474cd0
Class_00474cd0::Class_00474cd0()
{
    time = *(int*)(g_game + 0x38a47);
}
