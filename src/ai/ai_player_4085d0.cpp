// Decompiled by Sonnet and Opus. Names are provisional.
// Class_004085d0 (vtable 0x4fc9a8), derived from SquadTimer (the family is
// listed in 0x407350.cpp) without new fields.

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
    // In ai_player_408100.cpp: it needs the class as include/ta_types.h
    // declares it.
    virtual void OnTimer();                         // slot 0, 0x408100
};

// The base constructor (0x407350, matched in 0x407350.cpp) was defined in the
// same file, and /Ob2 inlines it below.
SquadTimer::SquadTimer(SquadManager* p, void* q)
    : owner(p), field_8(q), field_c(0), field_10(p->field_4)
{
}

// The constructor. Its vtable reference makes the compiler emit the scalar
// deleting destructor here too: the destructor is trivial, so only the inlined
// base destructor's store of 0x4fc980 is left.
// FUNCTION: 0x4085d0
// FUNCTION: 0x408600 ??_GClass_004085d0@@UAEPAXI@Z
Class_004085d0::Class_004085d0(SquadManager* p, void* q)
    : SquadTimer(p, q)
{
}
