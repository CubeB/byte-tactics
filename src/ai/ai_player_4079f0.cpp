// Decompiled by Opus. Names are provisional.
// Slot 0 of Class_004079d0 (vtable 0x4fc990), derived from SquadTimer
// (the family is listed in 0x407350.cpp, whose declarations this copies).
// Sets field_c to 150 ticks from now; when both this object's group and the
// group of the owner's member field_14 have units, passes the other group's
// average position (GetAveragePosition) to OrderSquad for this group.
#include <vector>

class SquadTimer;

#pragma pack(push, 1)
struct SquadManager {                  // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
    char unknown_5[0x11 - 0x5];
    SquadTimer* members[8];            // +0x11
};

struct Game {
    char unknown_0[0x38a47];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

struct Vec3_00407410 {
    int x;
    int y;
    int z;

    Vec3_00407410() {}
    Vec3_00407410(int a, int b, int c) : x(a), y(b), z(c) {}
};

struct Unit;

struct Group_00407410 {
    void* field_0;                     // +0x0
    void* field_4;                     // +0x4
    char unknown_8[0x10 - 0x8];
    std::vector<Unit*> units; // +0x10
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

// Vtable 0x4fc990, constructor 0x4079a0, ??_G 0x4079d0.
class Class_004079d0 : public SquadTimer {
public:
    int field_14;                      // +0x14

    Class_004079d0(SquadManager* p, void* q, int a);
    virtual void OnTimer();                         // slot 0, 0x4079f0
};

extern Game* g_game;

void __stdcall OrderSquad(void* a, void* b, int c, int d, int* e, Vec3_00407410* pos, int f, int g);

// FUNCTION: 0x4079f0
void Class_004079d0::OnTimer()
{
    field_c = g_game->ticks + 150;
    SquadTimer* other = owner->members[field_14];
    if (!((Group_00407410*)field_8)->units.empty()
        && !((Group_00407410*)other->field_8)->units.empty()) {
        Vec3_00407410 pos;
        if (other->GetAveragePosition(&pos)) {
            Group_00407410* g = (Group_00407410*)field_8;
            OrderSquad(g->field_0, g->field_4, 2, 0, 0, &pos, 0, 0);
        }
    }
}
