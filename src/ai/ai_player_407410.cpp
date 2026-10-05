// Decompiled by Opus. Names are provisional.
// Method of SquadTimer, the base of the family listed in 0x407350.cpp
// (whose declarations this copies). Averages the positions (16.16 fixed
// point, integer parts at +0x6c/+0x70/+0x74) of the units in the group that
// field_8 points to; returns 0 when the group is empty.
// The result must be built as a temporary and assigned whole: storing the
// three fields one by one interleaves the stores with the divisions.
#include <vector>

struct SquadManager {                  // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
};

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

    int GetAveragePosition(Vec3_00407410* out);
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x6c];
    short x;                           // +0x6c
    char unknown_6e[2];
    short y;                           // +0x70
    char unknown_72[2];
    short z;                           // +0x74
};
#pragma pack(pop)

struct Group_00407410 {
    char unknown_0[0x10];
    std::vector<Unit*> units; // +0x10
};

// FUNCTION: 0x407410
int SquadTimer::GetAveragePosition(Vec3_00407410* out)
{
    Group_00407410* g = (Group_00407410*)field_8;
    int n = g->units.size();
    if (n == 0)
        return 0;
    int x = 0, y = 0, z = 0;
    for (std::vector<Unit*>::iterator it = g->units.begin(); it != g->units.end(); ++it) {
        x += (*it)->x;
        y += (*it)->y;
        z += (*it)->z;
    }
    *out = Vec3_00407410(x / n << 16, y / n << 16, z / n << 16);
    return 1;
}
