// Decompiled by Sonnet, class family consolidated by Opus, Claude Opus 5.5, deepseek-v4.1-flash, GPT-6.1-sol, space-bunny-free and deepseek-v4.1. Names are provisional.

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
    char unknown_1422b[0x38a47 - 0x1422b];
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

struct FixedParts_00407ae0 {
    unsigned int frac : 16;
    int whole : 16;
};

union Fixed_00407ae0 {
    int value;
    FixedParts_00407ae0 parts;
};

static inline Fixed_00407ae0 MakeFixed(int i)
{
    Fixed_00407ae0 f;
    f.parts.frac = 0;
    f.parts.whole = i;
    return f;
}

union Coord_00407ae0 {
    int value;
    struct {
        unsigned short frac;
        short whole;
    } s;
};

struct Pos_00407ae0 {
    Coord_00407ae0 x;
    Coord_00407ae0 y;
    Coord_00407ae0 z;
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x6a];
    Vec pos;                           // +0x6a
};
#pragma pack(pop)

class Class_004071f0 {
public:
    Unit* FindNearestEnemyUnit(Vec pos);
};

int __stdcall RandomInt(int range);

// Copies the 12-byte position at +0x35 of g_playerAI[index] into *out.
void __stdcall GetBasePosition(int index, Pos_00407ae0* out);

static inline Pos_00407ae0 GetRallyPoint(int index)
{
    Pos_00407ae0 p;
    GetBasePosition(index, &p);
    return p;
}

// Vtable 0x4fc998, constructor 0x407a90, ??_G 0x407ac0.
class Class_00407a90 : public SquadTimer {
public:
    Class_00407a90(SquadManager* p, Group* q);
    virtual void OnTimer();                         // slot 0, 0x407ae0
};

// Constructor of Class_00407a90 (vtable 0x4fc998), derived from SquadTimer
// (the family is listed in src/ai/squad_timer.cpp) without new fields.
// FUNCTION: 0x407a90
// FUNCTION: 0x407ac0 ??_GClass_00407a90@@UAEPAXI@Z
Class_00407a90::Class_00407a90(SquadManager* p, Group* q)
    : SquadTimer(p, q)
{
}

// Slot 0 of Class_00407a90 (vtable 0x4fc998), derived from SquadTimer
// (the family is listed in src/ai/squad_timer.cpp).
// Sets `next` to 30..929 ticks from now. A group of fewer than 5 units
// either moves (mode 9) to the unit nearest its average position, or, when
// GetBasePosition gives a rally point for the player, is sent to 2..3 random
// points around it (mode 2 first, then mode 9). A bigger group is sent to a
// random point on the map edge. The unit FindNearestEnemyUnit returns is used
// without a null check.
// FUNCTION: 0x407ae0
void Class_00407a90::OnTimer()
{
    Vec dest;
    // Computed first: in one expression with next it folds into a lea.
    int delay = RandomInt(900) + 30;
    next = g_game->time + delay;
    if ((int)group->units.size() < 5) {
        // Read through an inline returning by value: pos must not be the local
        // whose address goes to GetBasePosition.
        Pos_00407ae0 pos = GetRallyPoint(player);
        if ((pos.x.s.whole | pos.z.s.whole) == 0) {
            GetAveragePosition(&dest);
            Unit* target = ((Class_004071f0*)owner)->FindNearestEnemyUnit(dest);
            group->Send(9, 1, 0, &target->pos, 0, 0);
        } else {
            int n = RandomInt(2) + 2;
            int w = g_game->baseX / 8, h = g_game->baseY / 8;
            for (int i = 0; i < n; i++) {
                // dx and dz stay named: the sum then lands in pos's register.
                int dx = (RandomInt(w) - w / 2) << 16;
                dest.x = pos.x.value + dx;
                dest.y = pos.y.value;
                int dz = (RandomInt(h) - h / 2) << 16;
                dest.z = pos.z.value + dz;
                if (i == 0)
                    group->Send(2, 0, 0, &dest, 0, 0);
                else
                    group->Send(9, 1, 0, &dest, 0, 0);
            }
        }
    } else {
        dest.y = 0;
        if (RandomInt(2)) {
            dest.x = RandomInt(g_game->baseX) << 16;
            dest.z = (RandomInt(2) ? MakeFixed(0) : MakeFixed(g_game->baseY - 1)).value;
        } else {
            dest.x = (RandomInt(2) ? MakeFixed(0) : MakeFixed(g_game->baseX - 1)).value;
            dest.z = RandomInt(g_game->baseY) << 16;
        }
        group->Send(9, 0, 0, &dest, 0, 0);
    }
}
