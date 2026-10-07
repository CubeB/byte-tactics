// Decompiled by Claude Opus 5.5, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// MATCH (Claude Opus 5.5, #5644). Slot 0 of Class_004085d0 (vtable 0x4fc9a8),
// derived from SquadTimer (family listed in 0x407350.cpp; the rest of the
// class is in ai_player_4085d0.cpp, this needs include/ta_types.h's view of
// it). Runs every 90
// ticks over the units of this object's group: first gives each unit that
// ChooseBuildOption picks an item for an order (mode 0xe) at the place FindBuildPosition
// finds, within a third of the map size of the player's base (GetBasePosition)
// for flag12 units; then sends the idle units towards the base: flag12 units to
// the point mirrored through it (a random point 0x280 from it when farther),
// the others to the base itself, or when within 0x140 of it, 0x140 onwards in
// its direction.
//
// What the last hunk needed (the u->def load at 0x408334 kept below the three
// `target = origin` stores), read with a tracer of C2's scheduler (hook
// 0x4315f1, which dumps each block's dependence graph):
// - C2 lets a load through a pointer alias an address-taken local only when
//   the local is in scope where the pointer's value was loaded. With `target`
//   declared in the if-block, `u = *it` is loaded outside its scope, so no
//   load through u depends on a store to target and the def load is hoisted
//   above the copy. Declared in the loop body, target is in scope at
//   `u = *it` (scope is per block, not per declaration point), so the def load
//   stays below the copy as in the original. A function-scope `Unit*` assigned
//   in the loop behaves the same, and copies of u (a second `Unit*`, an inline
//   helper's parameter) are folded back into u.
// - The same alias then holds the tail's u->pos loads below the target
//   stores, so the tail must build all three sums before storing:
//   `target = u->pos + d` (operator+ builds its result in a local that is
//   copied into target). That adds three sum candidates to the tail block;
//   with the else arm's direction written out by hand (int ang, direct d.x,
//   d.y, d.z stores), d.x rose to priority 440, above the FixMul temporaries'
//   416, and took edi (82.5%). Written as `d = Direction(...)`, the inline the
//   flag12 arm uses, the direction block only copies the returned temporary
//   into d (d.x 80 there instead of 168, 352 in all) and everything lands.
// - Named sums (`int x = d.x + u->pos.x;` ...) instead leave the sum used by
//   the first store sunk into it; the best of those orders is 99.8% with the
//   stores x, z, y.
//
// Spellings the earlier passes found, all still needed:
// - include/ta_types.h supplies Vec3, Class_00438760 and Class_004085d0 (on
//   Base, whose owner is an AI); Vec3's default constructor and operator+/-
//   and Class_00438760's default constructor are defined here, inline, with no
//   user operator=. <time.h> and <shlobj.h> bring the file total to the window
//   (65232 to 65732 with ta_types.h's current size) where C2 pushes the first
//   _allmul's operands d.x first, as the original does; <shlobj.h> alone flips
//   MapRange. If a regenerated ta_types.h grows by more than about 50 ids,
//   <commctrl.h> with <algorithm> is the set to try. <memory.h> gives
//   MapRange's mapWidth-first load order.
// - Length() takes a const reference to a temporary (pos - origin), so the
//   three fild operands are the temporary's own memory, and names its sum and
//   result (`float sq`, `int len`): that breaks the priority tie between the
//   flag12 arm's inlined angle and d.x, d.y, d.z (ebx for the angle).
// - Direction() is FUN_004103a0's own body, x, y, z order.
// - The range is an inline MapRange() assigned to a local before
//   `origin.y = pos.y`; written in the comparison it is computed after _ftol.
// - Loop 1's register rotation needs a second, foldable use of `range`
//   (`len > range || len > range`, one compare): it ranks range above the unit
//   web and puts `this` in ebx, unit in ebp, as the original does.
#include <ta_types.h>
#include <time.h>
#include <shlobj.h>
#include <memory.h>
#include <math.h>

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
                    int range = MapRange();
                    origin.y = pos.y;
                    int len = Length(pos - origin);
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
        Vec3 target;
        if ((!u->order || (u->order->flags & 0x4000))
            && (!(unsigned char)u->def->flag12 || GetBuilderCount(field_10) >= 5)) {
            target = origin;
            if (u->def->flag12) {
                origin.y = u->pos.y;
                Vec3 d = origin - u->pos;
                if (Length(d) > 0x2800000)
                    d = Direction(RandomInt(0x10000), 0x2800000);
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
