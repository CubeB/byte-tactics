// Decompiled by Sonnet, class family consolidated by Opus. Names are provisional.
// Constructor of SquadTimer, the base of a family of small classes with
// two virtual slots: slot 0 is a method each class overrides, slot 1 the
// virtual destructor. The owner's constructor (0x408cb0) creates one object
// of each class; it calls this constructor out of line once and inlines the
// others.
//
// Every derived destructor is trivial, so each class's scalar deleting
// destructor only stores the base vtable 0x4fc980 (the inlined base
// destructor). All files of the family copy the declarations below verbatim.
//
//   class           vtable    constructor  ??_G      slot 0
//   SquadTimer  0x4fc980  0x407350     0x407390  0x407380 (empty)
//   Class_00407930  0x4fc988  0x407930     0x407980  0x4077e0
//   Class_004079d0  0x4fc990  0x4079a0     0x4079d0  0x4079f0
//   Class_00407a90  0x4fc998  0x407a90     0x407ac0  0x407ae0
//   Class_00407d40  0x4fc9a0  0x407d40     0x407e70  0x407e90
//   Class_004085d0  0x4fc9a8  0x4085d0     0x408600  0x408100
//   Class_00408810  0x4fc9b0  0x4087e0     0x408810  0x4086d0
//
// Class_004079d0 and Class_00408810 keep the names their scalar deleting
// destructors gave them first. The derived constructors inline this one
// (/Ob2), so their files define it again, unannotated, as the original
// translation unit did.

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

// FUNCTION: 0x407350
SquadTimer::SquadTimer(SquadManager* p, void* q)
    : owner(p), field_8(q), field_c(0), field_10(p->field_4)
{
}
