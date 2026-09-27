// Decompiled by Opus. Names are provisional.
// Constructor of the Class_00471cc0 subclass whose vtable is 0x4fd638 (its
// scalar deleting destructor is 0x475110); the same shape as 0x474cd0: an
// empty std::vector of 32-byte records at +0xc, and the current game time
// at +0x8.
#include <vector>

struct Record_004750b0 {
    int unknown[8];
};

extern char* g_game;

class Class_00471cc0 {
public:
    Class_00471cc0();

    virtual void FUN_00475110();
    virtual void FUN_00475600();
    virtual void FUN_00475700();
    virtual void FUN_00475330();
    virtual void FUN_004751c0();
    virtual void FUN_004750f0();
    virtual void FUN_00475150();

    int field_4;                            // +0x04
};

class Class_004750b0 : public Class_00471cc0 {
public:
    int time;                               // +0x08
    std::vector<Record_004750b0> records;   // +0x0c (_First +0x10, _Last +0x14, _End +0x18)

    Class_004750b0();

    virtual void FUN_00475110();
    virtual void FUN_00475600();
    virtual void FUN_00475700();
    virtual void FUN_00475330();
    virtual void FUN_004751c0();
    virtual void FUN_004750f0();
    virtual void FUN_00475150();
};

// FUNCTION: 0x4750b0
Class_004750b0::Class_004750b0()
{
    time = *(int*)(g_game + 0x38a47);
}
