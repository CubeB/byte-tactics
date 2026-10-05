// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_004079d0
// (vtable 0x4fc990), derived from SquadTimer (the family is listed in
// 0x407350.cpp). Its destructor is trivial, so only the inlined base
// destructor's store of 0x4fc980 is left.
//
// A trivial destructor never stores this class's vtable, so the constructor
// (0x4079a0, matched in 0x4079a0.cpp) is defined again below, unannotated, to
// emit the vtable and with it this COMDAT.

struct SquadManager {                  // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class SquadTimer {
public:
    SquadManager* owner;               // +0x4
    void* field_8;                     // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10

    SquadTimer(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0
    virtual ~SquadTimer() {}                        // slot 1
};

// Vtable 0x4fc990, constructor 0x4079a0, ??_G 0x4079d0.
class Class_004079d0 : public SquadTimer {
public:
    int field_14;                      // +0x14

    Class_004079d0(SquadManager* p, void* q, int a);
    virtual void OnTimer();                         // slot 0, 0x4079f0
};

// FUNCTION: 0x4079d0 ??_GClass_004079d0@@UAEPAXI@Z
Class_004079d0::Class_004079d0(SquadManager* p, void* q, int a)
    : SquadTimer(p, q), field_14(a)
{
}
