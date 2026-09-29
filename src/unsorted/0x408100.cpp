// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// GPT-6 retry: eligibility, position-search and loop-body helpers, grouped result
// locals, vector constructors/copy operators and direction variants did not improve
// 83.5%. Preserve this version; the first loop register rotation remains.
// Slot 0 of Class_004085d0 (vtable 0x4fc9a8), derived from Class_00407350
// (the family is listed in 0x407350.cpp, whose declarations this copies).
// Runs every 90 ticks over the units of this object's group: first gives each
// unit that FUN_0040bdb0 picks an item for an order (mode 0xe) at the place
// FUN_0040bfe0 finds, within a third of the map size of the player's base
// (FUN_0040ba80) for flag12 units; then sends the idle units towards the
// base: flag12 units to the point mirrored through it (a random point 0x280
// from it when farther), the others to the base itself, or when within 0x140
// of it, 0x140 onwards in its direction.
//
// Partial (83.5%): the control flow, the stack frame and most of both loops
// match. What fixed parts of it:
// - Length() takes a const reference to a temporary (pos - origin): only then
//   are the three fild operands the temporary's own memory, with a stored 0
//   for y, as in the original.
// - The range is an inline MapRange() assigned to a local before
//   `origin.y = pos.y`; written in the comparison it is computed after _ftol.
// - <memory.h> gives the mapWidth-first load order in MapRange().
// Still different:
// - loop 1: registers are rotated: the original keeps the unit in ebp, `ok` in
//   ebx and the iterator in edi (spilled); here the iterator is in esi (spilled
//   around the idx load), the unit in edi and `ok` in ebp. Loop 2 is already
//   right (it in edi, unit in esi), which shows the two loops allocate the unit
//   independently, so the divergence is loop 1's own live ranges.
// - loop 2: origin is copied to target with origin.y in edi, which the else
//   branch reuses (here it is reloaded); scratch registers are rotated by one
//   at the flag12 test; the Direction() results use ebx/ebp swapped; the first
//   FixMul pushes its operands in the other order (s first).
// Retry by deepseek-v4.1-flash: every source-level shuffle left the allocator
// decision untouched (byte-identical output, all 83.5%), so loop 1's register
// choice is not driven by statement order. Tried without effect: a
// function-scope unit shared by both loops, a reference unit, a while loop,
// pos/ok/idx declaration order, converting `ok` to declaration plus assignment,
// and renaming loop 1's unit.
// Tried without effect or worse: other header sets, the iterator as a pointer,
// separate iterators per loop, a unit variable shared by both loops,
// function-scope ok/idx/kind, `continue` chains instead of the && chain,
// (*it)-> instead of a unit local, direct temporaries as FUN_0043adc0
// arguments, a Scale helper, FixMul operand orders, TooFar() helpers, and
// Length(d) versus Length(origin - u->pos) in each branch.
#include <memory.h>
#include <vector>
#include <math.h>

class Class_00407350;

struct Vec3 {
    int x, y, z;
    Vec3 operator+(const Vec3& o) const { Vec3 r; r.x = x + o.x; r.y = y + o.y; r.z = z + o.z; return r; }
    Vec3 operator-(const Vec3& o) const { Vec3 r; r.x = x - o.x; r.y = y - o.y; r.z = z - o.z; return r; }
};

#pragma pack(push, 1)
struct Class_00408cb0 {                // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
    char unknown_5[0xd - 0x5];
    unsigned int field_d;              // +0xd
};

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

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760() {}
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class Class_00407350 {
public:
    Class_00408cb0* owner;             // +0x4
    void* field_8;                     // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10

    Class_00407350(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0
    virtual ~Class_00407350() {}                    // slot 1
};

// Vtable 0x4fc9a8, constructor 0x4085d0, ??_G 0x408600.
class Class_004085d0 : public Class_00407350 {
public:
    Class_004085d0(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0, 0x408100
};

extern Game_00408100* g_game;

void __stdcall FUN_0040ba80(int index, Vec3* out);
unsigned short __stdcall FUN_0040bdb0(unsigned int player, Unit_00408100* unit);
int __stdcall FUN_0040bfe0(unsigned int player, Vec3* from, Item_00408100* item, Vec3* out);
int __stdcall FUN_0040c230(unsigned int player);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char mode, Unit_00408100* unit, Unit_00408100* target, Vec3* pos);
void __stdcall FUN_0043adc0(Class_00438760 kind, int remove, Unit_00408100* unit, Unit_00408100* target, Vec3* pos, int param_6, int param_7);
int __stdcall FUN_004b6c30(int range);

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

static inline int Length(const Vec3& v)
{
    float x = (float)v.x;
    float y = (float)v.y;
    float z = (float)v.z;
    return (int)sqrt(x * x + y * y + z * z);
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
void Class_004085d0::FUN_00407380()
{
    field_c = g_game->ticks + 90;
    Vec3 origin;
    FUN_0040ba80(field_10, &origin);
    std::vector<Unit_00408100*>::iterator it;
    for (it = ((Group_00408100*)field_8)->units.begin(); it != ((Group_00408100*)field_8)->units.end(); ++it) {
        Unit_00408100* u = *it;
        if (u->def->field_152
            && (!(unsigned char)u->def->flag12
                || (FUN_0040c230(field_10) < 5 && g_game->ticks >= owner->field_d))
            && (!u->order || !(u->order->flags & 8))) {
            unsigned short idx = FUN_0040bdb0(field_10, u);
            if (idx) {
                Vec3 pos;
                int ok = FUN_0040bfe0(field_10, &u->pos, &g_game->items[idx], &pos);
                if (u->def->flag12) {
                    int range = MapRange();
                    origin.y = pos.y;
                    if (Length(pos - origin) > range)
                        ok = 0;
                }
                if (ok) {
                    Class_00438760 kind = FUN_0043f0e0(0xe, u, 0, &pos);
                    FUN_0043adc0(kind, 0, u, 0, &pos, idx, 1);
                }
            }
        }
    }
    for (it = ((Group_00408100*)field_8)->units.begin(); it != ((Group_00408100*)field_8)->units.end(); ++it) {
        Unit_00408100* u = *it;
        if ((!u->order || (u->order->flags & 0x4000))
            && (!(unsigned char)u->def->flag12 || FUN_0040c230(field_10) >= 5)) {
            Vec3 target = origin;
            if (u->def->flag12) {
                origin.y = u->pos.y;
                Vec3 d = origin - u->pos;
                if (Length(d) > 0x2800000)
                    d = Direction(FUN_004b6c30(0x10000), 0x2800000);
                target = origin + d;
                Class_00438760 kind;
                kind = FUN_0043f0e0(2, u, 0, &target);
                FUN_0043adc0(kind, 0, u, 0, &target, 0, 0);
                kind = FUN_0043f0e0(9, u, 0, &origin);
                FUN_0043adc0(kind, 1, u, 0, &origin, 0, 0);
            } else {
                Vec3 d = origin - u->pos;
                int len = Length(d);
                if (len < 0x1400000) {
                    if (len < 0x100000) {
                        d = Direction(FUN_004b6c30(0x10000), 0x1400000);
                    } else {
                        int s = FixDiv(0x1400000, len);
                        d.x = FixMul(s, d.x);
                        d.y = FixMul(d.y, s);
                        d.z = FixMul(d.z, s);
                    }
                    target = u->pos + d;
                }
                Class_00438760 kind = FUN_0043f0e0(9, u, 0, &target);
                FUN_0043adc0(kind, 0, u, 0, &target, 0, 0);
            }
        }
    }
}
