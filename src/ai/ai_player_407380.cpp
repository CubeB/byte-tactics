// Decompiled by Haiku, class family consolidated by Opus. Names are provisional.
// Slot 0 of SquadTimer (vtable 0x4fc980), empty in the base class; every
// derived class overrides it (the family is listed in 0x407350.cpp).

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

// FUNCTION: 0x407380
void SquadTimer::OnTimer()
{
}
