// Decompiled by Claude Opus 5.5. Names are provisional.
// Method of the Class_00407350 family (see 0x407350.cpp), called by
// Class_00407930::FUN_00407380 (0x4077e0) with its kind and limit fields.
// Balances this object's group against the owner's member of the given kind:
// takes one unit from it when this group is empty, sends the unit farthest
// from the group's centre to that kind while its squared distance is at least
// limit * group size, then takes over every unit of the other group that lies
// closer than that. FUN_00480250(unit, id) moves a unit to a group.
// In the last scan the squared distance must be its own statement (`int d`);
// written inside the comparison, MSVC computes the size() ternary first.
#include <vector>

#pragma pack(push, 1)
struct Unit_00407560 {
    char unknown_0[0x6c];
    short x;                                // +0x6c
    char unknown_6e[6];
    short z;                                // +0x74
};
#pragma pack(pop)

struct Group_00407560 {
    void* player;                           // +0x0
    int id;                                 // +0x4
    char unknown_8[8];
    std::vector<Unit_00407560*> units;      // +0x10
};

class Class_00407560;

#pragma pack(push, 1)
struct Owner_00407560 {
    char unknown_0[0x11];
    Class_00407560* members[8];             // +0x11
};
#pragma pack(pop)

void __stdcall FUN_00480250(Unit_00407560* u, int id);

class Class_00407560 {
public:
    void* vtable;
    Owner_00407560* owner;                  // +0x4
    Group_00407560* group;                  // +0x8

    void FUN_00407560(int kind, int limit);
};

// FUNCTION: 0x407560
void Class_00407560::FUN_00407560(int kind, int limit)
{
    Class_00407560* other = owner->members[kind];
    if (other == this)
        return;
    if (group->units.empty()) {
        if (other->group->units.empty())
            return;
        FUN_00480250(other->group->units[0], group->id);
    }
    int sx = 0, sz = 0;
    std::vector<Unit_00407560*>::iterator it;
    for (it = group->units.begin(); it != group->units.end(); ++it) {
        sx += (*it)->x;
        sz += (*it)->z;
    }
    int cx, cz;
    while (1) {
        Group_00407560* g = group;
        cx = sx / (int)g->units.size();
        cz = sz / (int)g->units.size();
        if (g->units.size() == 1)
            break;
        int maxd = 0;
        std::vector<Unit_00407560*>::iterator best = g->units.end();
        for (it = g->units.begin(); it != g->units.end(); ++it) {
            int dx = (*it)->x - cx;
            int dz = (*it)->z - cz;
            int d = dx * dx + dz * dz;
            if (d > maxd) {
                maxd = d;
                best = it;
            }
        }
        if (maxd < limit * (int)g->units.size())
            break;
        FUN_00480250(*best, kind);
        sx -= (*best)->x;
        sz -= (*best)->z;
    }
    std::vector<Unit_00407560*> list;
    for (it = other->group->units.begin(); it != other->group->units.end(); ++it) {
        int dx = (*it)->x - cx;
        int dz = (*it)->z - cz;
        int d = dx * dx + dz * dz;
        if (d < limit * (int)group->units.size())
            list.push_back(*it);
    }
    for (it = list.begin(); it != list.end(); ++it)
        FUN_00480250(*it, group->id);
}
