// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
//
// Slot 0 of Class_00407a90 (vtable 0x4fc998), derived from SquadTimer
// (the family is listed in 0x407350.cpp, whose declarations this copies).
// Sets field_c to 30..929 ticks from now. A group of fewer than 5 units
// either moves (mode 9) to the unit nearest its average position, or, when
// GetBasePosition gives a rally point for field_10, is sent to 2..3 random points
// around it (mode 2 first, then mode 9). A bigger group is sent to a random
// point on the map edge. The unit FindNearestEnemyUnit returns is used without a null
// check.
//
// Matched (Claude Opus 5.5, 2026-10-03; nine passes had stopped at 91.1-92.1%).
// The earlier notes put the residual on the scatter loop: the original tests
// the loop with `cmp ecx, ebx` before computing w/2 and h/2, keeps w/2 in ebp
// and spills `this`, and no spelling of the loop gave both. The loop was never
// the cause. Deleting statements one at a time and recording only where the
// loop's values landed showed that the loop allocation flips to the original's
// as soon as `pos` stops being the local whose address goes to GetBasePosition.
// Reading the rally point through a small inline that returns it by value
// (GetRallyPoint) does that, and then the plain `for` loop with `w / 2` in the
// body (hoisted by the compiler after the entry test) gives the original's
// preheader. The last hunk was the sum's register: `dest.x = pos.x.value + dx`
// with `dx` holding the shifted offset puts the sum in pos.x's register, as
// the original has; shifting inside the sum does not. Both `dx` and `dz` have
// to be named.
//
// `field_c = g_game->ticks + FUN_004b6c30(900) + 30` in one expression folds
// to `lea eax, [eax+edx+0x1e]`; the delay has to be computed first. The final
// MakeFixed ternaries give the `lea eax, [tmp]; mov ecx, [eax]` selection.
// Group::Send is the inline from the matched sibling 0x4077e0 (calling
// OrderSquad directly compiles to the same bytes).
#include <vector>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    char unknown_1422b[0x38a47 - 0x1422b];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

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

struct Vec3_00407410 {
    int x;
    int y;
    int z;

    Vec3_00407410() {}
    Vec3_00407410(int a, int b, int c) : x(a), y(b), z(c) {}
};

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
    Vec3_00407410 pos;                 // +0x6a
};
#pragma pack(pop)

struct Player_00407ae0;

void __stdcall OrderSquad(Player_00407ae0* player, int id, unsigned char mode, int remove,
                            Unit* target, Vec3_00407410* pos, int flags, int extra);

struct Group_00407ae0 {
    Player_00407ae0* player;           // +0x0
    int id;                            // +0x4
    char unknown_8[0x10 - 0x8];
    std::vector<Unit*> units; // +0x10
    void Send(unsigned char mode, int remove, Unit* target, Vec3_00407410* pos,
              int flags, int extra)
    {
        OrderSquad(player, id, mode, remove, target, pos, flags, extra);
    }
};

struct SquadManager {                  // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
};

class Class_004071f0 {
public:
    Unit* FindNearestEnemyUnit(Vec3_00407410 pos);
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class SquadTimer {
public:
    SquadManager* owner;               // +0x4
    Group_00407ae0* group;             // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10

    SquadTimer(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0
    virtual ~SquadTimer() {}                        // slot 1

    int GetAveragePosition(Vec3_00407410* out);
};

// Vtable 0x4fc998, constructor 0x407a90, ??_G 0x407ac0.
class Class_00407a90 : public SquadTimer {
public:
    Class_00407a90(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0, 0x407ae0
};

int __stdcall FUN_004b6c30(int range);

// Copies the 12-byte position at +0x35 of g_playerAI[index] into *out.
void __stdcall GetBasePosition(int index, Pos_00407ae0* out);

static inline Pos_00407ae0 GetRallyPoint(int index)
{
    Pos_00407ae0 p;
    GetBasePosition(index, &p);
    return p;
}

// FUNCTION: 0x407ae0
void Class_00407a90::OnTimer()
{
    Vec3_00407410 dest;
    int delay = FUN_004b6c30(900) + 30;
    field_c = g_game->ticks + delay;
    if ((int)group->units.size() < 5) {
        Pos_00407ae0 pos = GetRallyPoint(field_10);
        if ((pos.x.s.whole | pos.z.s.whole) == 0) {
            GetAveragePosition(&dest);
            Unit* target = ((Class_004071f0*)owner)->FindNearestEnemyUnit(dest);
            group->Send(9, 1, 0, &target->pos, 0, 0);
        } else {
            int n = FUN_004b6c30(2) + 2;
            int w = g_game->baseX / 8, h = g_game->baseY / 8;
            for (int i = 0; i < n; i++) {
                int dx = (FUN_004b6c30(w) - w / 2) << 16;
                dest.x = pos.x.value + dx;
                dest.y = pos.y.value;
                int dz = (FUN_004b6c30(h) - h / 2) << 16;
                dest.z = pos.z.value + dz;
                if (i == 0)
                    group->Send(2, 0, 0, &dest, 0, 0);
                else
                    group->Send(9, 1, 0, &dest, 0, 0);
            }
        }
    } else {
        dest.y = 0;
        if (FUN_004b6c30(2)) {
            dest.x = FUN_004b6c30(g_game->baseX) << 16;
            dest.z = (FUN_004b6c30(2) ? MakeFixed(0) : MakeFixed(g_game->baseY - 1)).value;
        } else {
            dest.x = (FUN_004b6c30(2) ? MakeFixed(0) : MakeFixed(g_game->baseX - 1)).value;
            dest.z = FUN_004b6c30(g_game->baseY) << 16;
        }
        group->Send(9, 0, 0, &dest, 0, 0);
    }
}
