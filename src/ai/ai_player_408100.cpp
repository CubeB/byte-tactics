// Decompiled by Claude Opus 5.5, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Slot 0 of Class_004085d0 (vtable 0x4fc9a8), derived from SquadTimer (family
// listed in ai_player_406c90.cpp, which also holds the class and its
// constructor).
// Runs every 90 ticks over the units of this object's group: first gives each
// unit that ChooseBuildOption picks an item for an order (mode 0xe) at the
// place FindBuildPosition finds, within a third of the map size of the player's
// base (GetBasePosition) for flag12 units; then sends the idle units towards
// the base: flag12 units to the point mirrored through it (a random point 0x280
// from it when farther), the others to the base itself, or when within 0x140 of
// it, 0x140 onwards in its direction.
#include <ta_types.h>
// The system header set decides operand and load order in the _allmul and
// MapRange code.
#include <time.h>
#include <shlobj.h>
#include <memory.h>
#include <math.h>

// Defined inline here, with no user operator=.
inline Vec3::Vec3() {}
inline Vec3 Vec3::operator+(Vec3& o) { Vec3 r; r.x = x + o.x; r.y = y + o.y; r.z = z + o.z; return r; }
inline Vec3 Vec3::operator-(Vec3& o) { Vec3 r; r.x = x - o.x; r.y = y - o.y; r.z = z - o.z; return r; }

#pragma pack(push, 1)
struct UnitDef_00408100 {
    char unknown_0[0x152];
    int field_152;                     // +0x152
    char unknown_156[0x245 - 0x156];
    unsigned int unknown_bits0 : 12;   // +0x245
    unsigned int flag12 : 1;
    unsigned int unknown_bits13 : 19;
};

struct Order_00408100 {
    char unknown_0[0x42];
    unsigned int flags;                // +0x42
};

struct Unit_00408100 {
    char unknown_0[0x5c];
    Order_00408100* order;             // +0x5c
    char unknown_60[0x6a - 0x60];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_00408100* def;             // +0x92
};

struct Item_00408100 {                 // 0x249 bytes
    char unknown_0[0x249];
};

struct Game_00408100 {
    char unknown_0[0x1422b];
    int mapWidth;                      // +0x1422b
    int mapHeight;                     // +0x1422f
    char unknown_14233[0x1439b - 0x14233];
    Item_00408100* items;              // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    unsigned int ticks;                // +0x38a47
};
#pragma pack(pop)

struct Group_00408100 {
    char unknown_0[0x10];
    std::vector<Unit_00408100*> units; // +0x10
};

inline Class_00438760::Class_00438760() {}



extern Game_00408100* g_game;

void __stdcall GetBasePosition(int index, Vec3* out);
unsigned short __stdcall ChooseBuildOption(unsigned int player, Unit_00408100* unit);
int __stdcall FindBuildPosition(unsigned int player, Vec3* from, Item_00408100* item, Vec3* out);
int __stdcall GetBuilderCount(unsigned int player);
Class_00438760 __stdcall GetOrderType(unsigned char mode, Unit_00408100* unit, Unit_00408100* target, Vec3* pos);
void __stdcall AddOrder(Class_00438760 kind, int remove, Unit_00408100* unit, Unit_00408100* target, Vec3* pos, int param_6, int param_7);
int __stdcall RandomInt(int range);

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

// Takes a const reference and names its sum and result: breaks a register
// priority tie in the callers.
static inline int Length(const Vec3& v)
{
    float x = (float)v.x;
    float y = (float)v.y;
    float z = (float)v.z;
    float sq = x * x + y * y + z * z;
    int len = (int)sqrt(sq);
    return len;
}

// Inlined copy of FUN_004103a0.
static inline Vec3 Direction(short angle, int scale)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    v.z = -FUN_004b7123(angle, scale);
    return v;
}

static inline int FixMul(int a, int b)
{
    return (int)(((__int64)a * b) >> 16);
}

static inline int FixDiv(int a, int b)
{
    return (int)(((__int64)a << 16) / b);
}

static inline int MapRange()
{
    return (g_game->mapWidth + g_game->mapHeight) / 3 << 16;
}

// FUNCTION: 0x408100
void Class_004085d0::OnTimer()
{
    field_c = g_game->ticks + 90;
    Vec3 origin;
    GetBasePosition(field_10, &origin);
    std::vector<Unit_00408100*>::iterator it;
    for (it = ((Group_00408100*)field_8)->units.begin(); it != ((Group_00408100*)field_8)->units.end(); ++it) {
        Unit_00408100* u = *it;
        if (u->def->field_152
            && (!(unsigned char)u->def->flag12
                || (GetBuilderCount(field_10) < 5 && g_game->ticks >= (unsigned int)owner->field_d))
            && (!u->order || !(u->order->flags & 8))) {
            unsigned short idx = ChooseBuildOption(field_10, u);
            if (idx) {
                Vec3 pos;
                int ok = FindBuildPosition(field_10, &u->pos, &g_game->items[idx], &pos);
                if (u->def->flag12) {
                    // Assigned to a local first: in the comparison it follows _ftol.
                    int range = MapRange();
                    origin.y = pos.y;
                    int len = Length(pos - origin);
                    // Second use of range is deliberate: fixes the register order.
                    if (len > range || len > range)
                        ok = 0;
                }
                if (ok) {
                    Class_00438760 kind = GetOrderType(0xe, u, 0, &pos);
                    AddOrder(kind, 0, u, 0, &pos, idx, 1);
                }
            }
        }
    }
    for (it = ((Group_00408100*)field_8)->units.begin(); it != ((Group_00408100*)field_8)->units.end(); ++it) {
        Unit_00408100* u = *it;
        // Declared in the loop body: keeps the u->def load below the copy.
        Vec3 target;
        if ((!u->order || (u->order->flags & 0x4000))
            && (!(unsigned char)u->def->flag12 || GetBuilderCount(field_10) >= 5)) {
            target = origin;
            if (u->def->flag12) {
                origin.y = u->pos.y;
                Vec3 d = origin - u->pos;
                if (Length(d) > 0x2800000)
                    d = Direction(RandomInt(0x10000), 0x2800000);
                // operator+ builds all three sums before storing; named sums differ.
                target = origin + d;
                Class_00438760 kind;
                kind = GetOrderType(2, u, 0, &target);
                AddOrder(kind, 0, u, 0, &target, 0, 0);
                kind = GetOrderType(9, u, 0, &origin);
                AddOrder(kind, 1, u, 0, &origin, 0, 0);
            } else {
                Vec3 d = origin - u->pos;
                int len = Length(d);
                if (len < 0x1400000) {
                    if (len < 0x100000) {
                        // Direction(), not written by hand: keeps the direction block small.
                        d = Direction(RandomInt(0x10000), 0x1400000);
                    } else {
                        int s = FixDiv(0x1400000, len);
                        d.x = FixMul(s, d.x);
                        d.y = FixMul(d.y, s);
                        d.z = FixMul(d.z, s);
                    }
                    target = u->pos + d;
                }
                Class_00438760 kind = GetOrderType(9, u, 0, &target);
                AddOrder(kind, 0, u, 0, &target, 0, 0);
            }
        }
    }
}
