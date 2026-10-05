// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_00407930
// (vtable 0x4fc988), derived from SquadTimer (the family is listed in
// 0x407350.cpp). Its destructor is trivial: the dead store of this class's
// vtable disappears and only the inlined base destructor's store of 0x4fc980
// is left.
//
// A trivial destructor never stores this class's vtable, so the constructor
// (0x407930, matched in 0x407930.cpp) is defined again below, unannotated, to
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

// Vtable 0x4fc988, constructor 0x407930, ??_G 0x407980.
class Class_00407930 : public SquadTimer {
public:
    int field_14;                      // +0x14
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24

    Class_00407930(SquadManager* p, void* q, int a, int b);
    virtual void OnTimer();                         // slot 0, 0x4077e0
};

// FUNCTION: 0x407980 ??_GClass_00407930@@UAEPAXI@Z
Class_00407930::Class_00407930(SquadManager* p, void* q, int a, int b)
    : SquadTimer(p, q), field_1c(b), field_20(a)
{
    field_24 = 0;
    field_18 = 6;
    field_14 = 3;
}
