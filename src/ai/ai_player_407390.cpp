// Decompiled by Sonnet, class family consolidated by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of SquadTimer
// (vtable 0x4fc980), the base of the family listed in 0x407350.cpp. Its
// destructor is empty and inline, so only the vtable store is left.
//
// The constructor (0x407350, matched in 0x407350.cpp) is defined again below,
// unannotated, to emit the vtable and with it this COMDAT.

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

// FUNCTION: 0x407390 ??_GSquadTimer@@UAEPAXI@Z
SquadTimer::SquadTimer(SquadManager* p, void* q)
    : owner(p), field_8(q), field_c(0), field_10(p->field_4)
{
}
