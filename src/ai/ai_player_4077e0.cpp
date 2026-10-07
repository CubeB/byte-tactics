// Decompiled by GPT-6, Sonnet, class family consolidated by Opus. Names are provisional.

#include <vector>

struct Vec { int x, y, z; };
struct Unit;
struct Player;
void __stdcall OrderSquad(Player*, int, unsigned char, int, Unit*, Vec*, int, int);

struct Group {
    Player* player;                    // +0x0
    int id;                            // +0x4
    char unknown_8[0x10 - 0x8];
    std::vector<Unit*> units;          // +0x10

    void Send(unsigned char mode, int remove, Unit* target, Vec* pos, int flags, int extra)
    {
        OrderSquad(player, id, mode, remove, target, pos, flags, extra);
    }
};

class SquadTimer;

#pragma pack(push, 1)
struct SquadManager {                  // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
    char unknown_5[0x11 - 0x5];
    SquadTimer* objs[10];              // +0x11
};

struct Game {
    char unknown_0[0x38a47];
    int time;                          // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

// Vtable 0x4fc980, constructor 0x407350 (src/ai/squad_timer.cpp).
class SquadTimer {
public:
    SquadManager* owner;               // +0x4
    Group* group;                      // +0x8
    int next;                          // +0xc, the time of the next run
    unsigned int player;               // +0x10

    SquadTimer(SquadManager* p, Group* q);
    virtual void OnTimer();                         // slot 0
    virtual ~SquadTimer() {}                        // slot 1
    int GetAveragePosition(Vec* out);
};

// The base constructor was defined in the same file, and /Ob2 inlines it in
// the derived constructors.
SquadTimer::SquadTimer(SquadManager* p, Group* q)
    : owner(p), group(q), next(0), player(p->field_4)
{
}

class Class_00407560 {
public:
    void FUN_00407560(int, int);
};

class Class_004071f0 {
public:
    Unit* FindNearestEnemyUnit(Vec);
};

// SquadTimer::FUN_004073b0, inlined: the position of the first of three of
// the owner's squads that has one.
static inline int Rally(SquadManager*& owner, Vec* pos)
{
    if (owner->objs[5]->GetAveragePosition(pos))
        return 1;
    if (owner->objs[1]->GetAveragePosition(pos))
        return 1;
    if (owner->objs[4]->GetAveragePosition(pos))
        return 1;
    return 0;
}

// Vtable 0x4fc988, constructor 0x407930, ??_G 0x407980.
class Class_00407930 : public SquadTimer {
public:
    int minimum;                       // +0x14
    int maximum;                       // +0x18
    int limit;                         // +0x1c
    int kind;                          // +0x20
    int attacking;                     // +0x24

    Class_00407930(SquadManager* p, Group* q, int a, int b);
    virtual void OnTimer();                         // slot 0, 0x4077e0
};

// FUNCTION: 0x4077e0
void Class_00407930::OnTimer()
{
    Vec pos;
    Vec retreat;
    next = g_game->time + 300;
    ((Class_00407560*)this)->FUN_00407560(kind, limit);
    if (!group->units.empty()) {
        if ((int)group->units.size() <= minimum || (!attacking && (int)group->units.size() < maximum)) {
            if (Rally(owner, &retreat)) {
                attacking = 0;
                group->Send(2, 0, 0, &retreat, 160, 0);
                return;
            }
        }
        attacking = 1;
        GetAveragePosition(&pos);
        Unit* target = ((Class_004071f0*)owner)->FindNearestEnemyUnit(pos);
        if (target)
            group->Send(3, 0, target, 0, 0, 0);
    }
}

// Constructor of Class_00407930 (vtable 0x4fc988), derived from SquadTimer
// (the family is listed in src/ai/squad_timer.cpp). The owner creates two of
// them, with (3, 20000) and (7, 50000).
// FUNCTION: 0x407930
// FUNCTION: 0x407980 ??_GClass_00407930@@UAEPAXI@Z
Class_00407930::Class_00407930(SquadManager* p, Group* q, int a, int b)
    : SquadTimer(p, q), limit(b), kind(a)
{
    // limit and kind stay in the initialiser list, the rest in the body.
    attacking = 0;
    maximum = 6;
    minimum = 3;
}
