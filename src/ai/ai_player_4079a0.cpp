// Decompiled by Opus and Sonnet, class family consolidated by Opus. Names are provisional.

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

// Vtable 0x4fc990, constructor 0x4079a0, ??_G 0x4079d0.
class Class_004079d0 : public SquadTimer {
public:
    int other;                         // +0x14, the owner's squad to follow

    Class_004079d0(SquadManager* p, Group* q, int a);
    virtual void OnTimer();                         // slot 0, 0x4079f0
};

// Constructor of Class_004079d0 (vtable 0x4fc990), derived from SquadTimer
// (the family is listed in src/ai/squad_timer.cpp). The owner creates two of
// them, with 2 and 6.
// FUNCTION: 0x4079a0
// FUNCTION: 0x4079d0 ??_GClass_004079d0@@UAEPAXI@Z
Class_004079d0::Class_004079d0(SquadManager* p, Group* q, int a)
    : SquadTimer(p, q), other(a)
{
}

// Slot 0: every 150 ticks, when both this squad and the owner's squad
// `other` have units, sends this squad to the other's average position.
// FUNCTION: 0x4079f0
void Class_004079d0::OnTimer()
{
    next = g_game->time + 150;
    SquadTimer* o = owner->objs[other];
    if (!group->units.empty()
        && !o->group->units.empty()) {
        Vec pos;
        if (o->GetAveragePosition(&pos)) {
            Group* g = group;
            OrderSquad(g->player, g->id, 2, 0, 0, &pos, 0, 0);
        }
    }
}
