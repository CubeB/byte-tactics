// Decompiled by Sonnet, class family consolidated by Opus, Haiku, Opus and GPT-6. Names are provisional.

#include <vector>

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

struct Vec { int x, y, z; };

// A 16.16 fixed-point coordinate.
union Fixed {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    };
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x6a];
    Fixed x;                           // +0x6a
    Fixed y;                           // +0x6e
    Fixed z;                           // +0x72
    char unknown_76[0xac - 0x76];
    int group;                         // +0xac
    char unknown_b0[0x118 - 0xb0];
};

struct Player {
    char unknown_0[0x67];
    Unit* first;                       // +0x67
    Unit* last;                        // +0x6b
};
#pragma pack(pop)

struct Group {
    Player* player;                    // +0x0
    int id;                            // +0x4
    char unknown_8[0x10 - 0x8];
    std::vector<Unit*> units;          // +0x10
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class SquadTimer {
public:
    SquadManager* owner;               // +0x4
    Group* group;                      // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10

    SquadTimer(SquadManager* p, Group* q);
    virtual void OnTimer();                         // slot 0
    virtual ~SquadTimer() {}                        // slot 1
    int FUN_004073b0(Vec3_00407410* out);
    int GetAveragePosition(Vec3_00407410* out);
    int CountGroupUnitsInRadius(Vec* pos, int radius);
};

// Constructor of SquadTimer, the base of a family of small classes with
// two virtual slots: slot 0 is a method each class overrides, slot 1 the
// virtual destructor.
//
// The derived classes' files copy the declarations below.
//
//   class           vtable    constructor  ??_G      slot 0
//   SquadTimer  0x4fc980  0x407350     0x407390  0x407380 (empty)
//   Class_00407930  0x4fc988  0x407930     0x407980  0x4077e0
//   Class_004079d0  0x4fc990  0x4079a0     0x4079d0  0x4079f0
//   Class_00407a90  0x4fc998  0x407a90     0x407ac0  0x407ae0
//   Class_00407d40  0x4fc9a0  0x407d40     0x407e70  0x407e90
//   Class_004085d0  0x4fc9a8  0x4085d0     0x408600  0x408100
//   Class_00408810  0x4fc9b0  0x4087e0     0x408810  0x4086d0
// FUNCTION: 0x407350
SquadTimer::SquadTimer(SquadManager* p, Group* q)
    : owner(p), group(q), field_c(0), field_10(p->field_4)
{
}

// Slot 0 of SquadTimer (vtable 0x4fc980), empty in the base class; every
// derived class overrides it (the family is listed at the constructor).
// FUNCTION: 0x407380
void SquadTimer::OnTimer()
{
}

// The compiler-generated scalar deleting destructor of SquadTimer
// (vtable 0x4fc980), the base of the family listed at the constructor. Its
// destructor is empty and inline, so only the vtable store is left.
// FUNCTION: 0x407390 ??_GSquadTimer@@UAEPAXI@Z

// Asks three of the owner's objects in turn
// for their average position (0x407410) and returns 1 as soon as one has it.
// The owner's constructor (0x408cb0) fills an array of ten object pointers at
// +0x11, so the three used here are entries 5, 1 and 4.
// FUNCTION: 0x4073b0
int SquadTimer::FUN_004073b0(Vec3_00407410* out)
{
    if (owner->objs[5]->GetAveragePosition(out))
        return 1;
    if (owner->objs[1]->GetAveragePosition(out))
        return 1;
    return owner->objs[4]->GetAveragePosition(out) != 0;
}

// Averages the positions (16.16 fixed point, integer parts at
// +0x6c/+0x70/+0x74) of the units in the group that `group` points to; returns
// 0 when the group is empty.
// FUNCTION: 0x407410
int SquadTimer::GetAveragePosition(Vec3_00407410* out)
{
    Group* g = group;
    int n = g->units.size();
    if (n == 0)
        return 0;
    int x = 0, y = 0, z = 0;
    for (std::vector<Unit*>::iterator it = g->units.begin(); it != g->units.end(); ++it) {
        x += (*it)->x.whole;
        y += (*it)->y.whole;
        z += (*it)->z.whole;
    }
    // Built as a temporary and assigned whole: per-field stores interleave with the divisions.
    *out = Vec3_00407410(x / n << 16, y / n << 16, z / n << 16);
    return 1;
}

// FUNCTION: 0x4074a0
int SquadTimer::CountGroupUnitsInRadius(Vec* pos, int radius)
{
    int count = 0;
    int squared = radius * radius;
    Unit* u = group->player->first;
    Unit* last = group->player->last;
    for (; u <= last; ++u) {
        if (u->group == group->id) {
            int dz = pos->z - u->z.value;
            int dx = pos->x - u->x.value;
            int distance = (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
            if (distance <= squared)
                ++count;
        }
    }
    return count;
}
