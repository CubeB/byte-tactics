// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_004085d0
// (vtable 0x4fc9a8), derived from SquadTimer (the family is listed in
// 0x407350.cpp). Its destructor is trivial, so only the inlined base
// destructor's store of 0x4fc980 is left.
//
// A trivial destructor never stores this class's vtable, so the constructor
// (0x4085d0, matched in 0x4085d0.cpp) is defined again below, unannotated, to
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

// Vtable 0x4fc9a8, constructor 0x4085d0, ??_G 0x408600.
class Class_004085d0 : public SquadTimer {
public:
    Class_004085d0(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0, 0x408100
};

// FUNCTION: 0x408600 ??_GClass_004085d0@@UAEPAXI@Z
Class_004085d0::Class_004085d0(SquadManager* p, void* q)
    : SquadTimer(p, q)
{
}
