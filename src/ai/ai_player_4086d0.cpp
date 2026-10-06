// Decompiled by GPT-6, Sonnet, class family consolidated by Opus and Haiku. Names are provisional.

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
    char unknown_0[0x14223];
    int baseX;                         // +0x14223, the map's size
    int baseY;                         // +0x14227
    char unknown_1422b[0x1439b - 0x1422b];
    struct UnitDef* defs;              // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
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

#pragma pack(push, 1)
struct UnitDef {
    char unknown_0[0x152];
    int building;                      // +0x152
    char unknown_156[0x22d - 0x156];
    char converter;                    // +0x22d
    char unknown_22e[0x249 - 0x22e];
};

struct Economy {
    char unknown_0[0x8c];
    float energy;                      // +0x8c
    char unknown_90[8];
    float cost;                        // +0x98
};

struct Unit {
    char unknown_0[0x5c];
    void* orders;                      // +0x5c
    char unknown_60[0x92 - 0x60];
    UnitDef* def;                      // +0x92
    Economy* economy;                  // +0x96
    char unknown_9a[0x110 - 0x9a];
    unsigned int flags;                // +0x110

    void SetStateBits(int which, int on);
};
#pragma pack(pop)

float __stdcall FUN_00464ad0(Economy* economy);
int __stdcall RandomInt(int range);
unsigned short __stdcall ChooseBuildOption(unsigned int player, Unit* unit);
void __stdcall QueueBuildOrder(char* name, Unit* unit, int count);

// Vtable 0x4fc9b0, constructor 0x4087e0, ??_G 0x408810. The class keeps the
// name its scalar deleting destructor gave it.
class Class_00408810 : public SquadTimer {
public:
    Class_00408810(SquadManager* p, Group* q);
    virtual void OnTimer();                         // slot 0, 0x4086d0
};

// Slot 0: every 30 ticks, switches each idle converter on when energy is
// plentiful and off otherwise, and queues a build order for each idle
// builder.
// FUNCTION: 0x4086d0
void Class_00408810::OnTimer()
{
    next = g_game->time + 30;
    for (std::vector<Unit*>::iterator it = group->units.begin(); it != group->units.end(); ++it) {
        Unit* u = *it;
        if ((u->flags & 0x20000000) && (u->flags & 0x10000000) && !(u->flags & 0x4000)) {
            if (u->def->converter) {
                if (u->economy->cost + u->economy->cost < u->economy->energy) {
                    if (FUN_00464ad0(u->economy) > 0.0f && RandomInt(5))
                        u->SetStateBits(1, 1);
                } else
                    u->SetStateBits(1, 0);
            } else if (u->def->building && !u->orders) {
                unsigned short id = ChooseBuildOption(player, u);
                if (id)
                    QueueBuildOrder((char*)&g_game->defs[id] + 0x20, u, 1);
            }
        }
    }
}

// Constructor of Class_00408810 (vtable 0x4fc9b0), derived from SquadTimer
// (the family is listed in src/ai/squad_timer.cpp) without new fields.
// FUNCTION: 0x4087e0
// FUNCTION: 0x408810 ??_GClass_00408810@@UAEPAXI@Z
Class_00408810::Class_00408810(SquadManager* p, Group* q)
    : SquadTimer(p, q)
{
}
