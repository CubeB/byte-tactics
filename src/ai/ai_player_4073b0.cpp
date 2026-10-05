// Decompiled by Opus. Names are provisional.
// Method of SquadTimer, the base of the family listed in 0x407350.cpp
// (whose declarations this copies). Asks three of the owner's objects in turn
// for their average position (0x407410) and returns 1 as soon as one has it.
// The owner's constructor (0x408cb0) fills an array of ten object pointers at
// +0x11, so the three used here are entries 5, 1 and 4.

class SquadTimer;

#pragma pack(push, 1)
struct SquadManager {                  // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
    char unknown_5[0x11 - 0x5];
    SquadTimer* objs[10];              // +0x11
};
#pragma pack(pop)

struct Vec3_00407410 {
    int x;
    int y;
    int z;

    Vec3_00407410(int a, int b, int c) : x(a), y(b), z(c) {}
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

    int FUN_004073b0(Vec3_00407410* out);
    int GetAveragePosition(Vec3_00407410* out);
};

// FUNCTION: 0x4073b0
int SquadTimer::FUN_004073b0(Vec3_00407410* out)
{
    if (owner->objs[5]->GetAveragePosition(out))
        return 1;
    if (owner->objs[1]->GetAveragePosition(out))
        return 1;
    return owner->objs[4]->GetAveragePosition(out) != 0;
}
