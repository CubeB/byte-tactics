// Decompiled by Opus. Names are provisional.
// Constructor of Class_004079d0 (vtable 0x4fc990), derived from
// SquadTimer (the family is listed in 0x407350.cpp). The owner creates
// two of them, with 2 and 6.

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

// The base constructor (0x407350, matched in 0x407350.cpp) was defined in the
// same file, and /Ob2 inlines it below.
SquadTimer::SquadTimer(SquadManager* p, void* q)
    : owner(p), field_8(q), field_c(0), field_10(p->field_4)
{
}

// FUNCTION: 0x4079a0
Class_004079d0::Class_004079d0(SquadManager* p, void* q, int a)
    : SquadTimer(p, q), field_14(a)
{
}
