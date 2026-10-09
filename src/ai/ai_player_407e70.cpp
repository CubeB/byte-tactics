// Decompiled by Claude Opus 5.5, deepseek-v4.1-flash and Opus. Names are
// provisional.
// Slot 0 of SpatialTimer (vtable 0x4fc9a0), derived from SquadTimer
// (the family is listed in ai_player.cpp, whose declarations this copies).
// Sets next to 30..179 ticks from now. When the group has units, moves the
// probe point b by the step c (one time in ten it restarts from a with a new
// random direction of length 0x140 map units), and when the owner can see or
// has explored b, keeps b as the new target a if a random roll favours its
// SumUnitRatingsInRange score. Then orders every unit whose def has flag4 set, and
// that is active or can reach a (WeaponCanReachPos), to move to a with the order
// GetOrderType picks.
//
// The explored-map test is the inlined player method 0x475470 describes: a
// {data, width, height} ByteMap at +0x7c.
#include <vector>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14223];
    int baseX;                          // +0x14223
    int baseY;                          // +0x14227
    char unknown_1422b[0x14281 - 0x1422b];
    unsigned short mapFlags;            // +0x14281
    char unknown_14283[0x38a47 - 0x14283];
    int ticks;                          // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

struct Vec3_00407d40 {
    int x, y, z;

    Vec3_00407d40() {}
    Vec3_00407d40(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
    void operator+=(const Vec3_00407d40& v) { x += v.x; y += v.y; z += v.z; }
};

struct Position_00408090 {              // 16.16 fixed point; only high words read
    short xFrac;
    short x;                            // +0x2
    short yFrac;
    short y;                            // +0x6
    short zFrac;
    short z;                            // +0xa
};

struct MapSize_00408090 {
    unsigned int width;                 // +0x0
    unsigned int height;                // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_00408090 {
    unsigned char* data;                // +0x0
    MapSize_00408090 size;              // +0x4

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

// The header's type, kept local: its explored pointer cannot spell the
// ByteMap this fog test needs.
struct Player {
    char unknown_0[0x7c];
    ByteMap_00408090 explored;          // +0x7c
};

int __stdcall IsPointVisible(Player* player, Position_00408090* pos);

static inline int IsExplored(Player* player, Position_00408090* pos)
{
    // Plain int: an unsigned tx shortens the shift to 16 bits.
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (player->explored.size.Contains(tx, ty) && player->explored.Get(tx, ty))
        return 1;
    return 0;
}

static inline int IsVisible(Player* player, Position_00408090* pos)
{
    if ((g_game->mapFlags & 2) == 2)
        return IsExplored(player, pos);
    return IsPointVisible(player, pos);
}

#pragma pack(push, 1)
struct UnitDef_00407e90 {
    char unknown_0[0x245];
    unsigned int unknown_bits : 4;
    unsigned int flag4 : 1;             // +0x245 bit 4
};

struct Unit {
    int motion;                        // +0x0
    char unknown_4[0x6a - 0x4];
    Vec3_00407d40 pos;                  // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_00407e90* def;              // +0x92
};
#pragma pack(pop)

struct Group_00407e90 {
    Player* player;                     // +0x0
    int id;                             // +0x4
    char unknown_8[0x10 - 0x8];
    std::vector<Unit*> units;           // +0x10
};

class MissionType {
public:
    unsigned char index;
    MissionType(const char* name);
    MissionType() : index(0) {}
};

#include "squad_manager.h"

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class SquadTimer {
public:
    SquadManager* owner;                // +0x4
    void* group;                        // +0x8
    int next;                           // +0xc
    unsigned int player;                // +0x10

    SquadTimer(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0
    virtual ~SquadTimer() {}                        // slot 1
};

// Vtable 0x4fc9a0, constructor 0x407d40, ??_G 0x407e70.
class SpatialTimer : public SquadTimer {
public:
    Vec3_00407d40 a;                    // +0x14
    Vec3_00407d40 b;                    // +0x20
    Vec3_00407d40 c;                    // +0x2c
    int field_38;                       // +0x38

    SpatialTimer(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0, 0x407e90
};

int __stdcall RandomInt(int range);
int __cdecl FUN_004b70ef(int angle, int distance);
int __cdecl FUN_004b7123(int angle, int distance);

static inline Vec3_00407d40 Offset(int angle, int distance)
{
    Vec3_00407d40 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

int __stdcall SumUnitRatingsInRange(int index, Vec3_00407d40* pos, int range);
int __stdcall WeaponCanReachPos(Unit* unit, Vec3_00407d40* from, Vec3_00407d40* to, int flags);
MissionType __stdcall GetOrderType(unsigned char mode, Unit* unit,
                                      Unit* target, Vec3_00407d40* pos);
void __stdcall AddOrder(MissionType kind, int remove, Unit* unit,
                            Unit* target, Vec3_00407d40* pos, int a, int b);

// Constructor 0x407d40 defined again, unannotated: emits the vtable and the
// scalar deleting destructor.
// FUNCTION: 0x407e70 ??_GSpatialTimer@@UAEPAXI@Z
SpatialTimer::SpatialTimer(SquadManager* p, void* q)
    : SquadTimer(p, q)
{
    // Half of g_game's baseX and baseY, in 16.16 fixed point.
    int x = (int)(g_game->baseX / 2 * 65536.0);
    a = Vec3_00407d40(x, 0, (int)(g_game->baseY / 2 * 65536.0));
    x = (int)(g_game->baseX / 2 * 65536.0);
    b = Vec3_00407d40(x, 0, (int)(g_game->baseY / 2 * 65536.0));
    x = (int)(g_game->baseX / 2 * 65536.0);
    c = Vec3_00407d40(x, 0, (int)(g_game->baseY / 2 * 65536.0));
    field_38 = 0;
}

// FUNCTION: 0x407e90
void SpatialTimer::OnTimer()
{
    // Computed first or the sum folds into one lea.
    int delay = RandomInt(150) + 30;
    next = g_game->ticks + delay;
    if (((Group_00407e90*)group)->units.empty())
        return;
    // Unused on purpose: it emits the operator delete call after the loop.
    std::vector<Unit*> unused;
    if (RandomInt(10) == 0) {
        b = a;
        int angle = RandomInt(0x10000);
        // Offset() keeps the call order and the zero y.
        c = Offset(angle, 0x1400000);
    }
    b += c;
    if (IsVisible(((Group_00407e90*)group)->player, (Position_00408090*)&b)) {
        int r = SumUnitRatingsInRange(player, &b, 0xa0);
        // Operand order: the field_38 roll is called first.
        if (RandomInt(r) > RandomInt(field_38)) {
            field_38 = r;
            a = b;
        }
    }
    for (std::vector<Unit*>::iterator it = ((Group_00407e90*)group)->units.begin();
         it != ((Group_00407e90*)group)->units.end(); ++it) {
        Unit* u = *it;
        if (u->def->flag4) {
            if (u->motion || WeaponCanReachPos(u, &u->pos, &a, 0)) {
                MissionType kind = GetOrderType(3, u, 0, &a);
                if (kind.index)
                    AddOrder(kind, 0, u, 0, &a, 0, 0);
            }
        }
    }
}
