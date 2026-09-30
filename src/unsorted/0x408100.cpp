// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// space-bunny-free: 87.4% by check.py, 309 of 405 original instructions by true
// LCS, total size now exactly 1221 bytes. Three things moved it off 84.1%:
// - The inline Direction() assigns its fields in the order x, z, y (NOT x, y, z).
//   x,y,z puts the flag12 branch's `d.y = 0` and the negation of d.z in the wrong
//   order (neg eax before `add esp,8`, the xor after it) and leaves a 2-byte-long
//   function. x,z,y schedules the xor into the second call's argument gap exactly
//   as the original does, and the three results land in edi/ebp/ebx like the
//   original. Expanding the OTHER Direction() call (loop 2, the len<0x100000 one)
//   into direct d.x/d.y/d.z stores is still right, 84.1% on its own but the two
//   together are what match; expanding this one instead of keeping the struct
//   return costs 19 points (67.3%).
// - The tail of the len<0x1400000 branch is three separate
//   `target.x = u->pos.x + d.x;` component stores. NOT `target = u->pos + d`
//   (85.2%: one `mov ebx, [esp+0x18]` and the add/store order move) and NOT the
//   in-place `d.x += u->pos.x` form (84.1%). The component stores are what put
//   pos.x/y/z in edi/ecx/edx and add into them, as at 0x40855b.
// Still one diff, the first loop's register rotation, and it is the whole of the
// rest (see the hunk list below).
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
// Earlier findings that still hold:
// - Length() takes a const reference to a temporary (pos - origin): only then
//   are the three fild operands the temporary's own memory, with a stored 0
//   for y, as in the original.
// - The range is an inline MapRange() assigned to a local before
//   `origin.y = pos.y`; written in the comparison it is computed after _ftol.
// - <memory.h> gives the mapWidth-first load order in MapRange().
// - Loop 2's else branch computes the difference into registers already holding
//   origin.x/y/z (eax/edi/edx), member by member; a plain `Vec3 d = origin -
//   u->pos;` hoists the loads and scores worse.
//
// Still different (addresses in the original). The size is exact, so all of it
// is register choice and two scheduling spots:
// - 0x40812f-0x4082c6, loop 1: registers rotated. The original keeps `this` in
//   ebx, the unit in ebp, the iterator in edi (spilled to esp+0x14) and `ok` in
//   ebx only after `this` dies; here `this` is in ebp, the unit in edi, `ok` in
//   ebx and the iterator in esi, sharing esi with `idx`. Everything downstream
//   in the hunk (the MapRange value, the ok test, the two calls) rotates with
//   it. In loop 2 the same rotation is one step smaller: the unit is in esi
//   (right) but `this` is in ebp and the iterator in ebx, where the original
//   has `this` in ebx, the iterator in edi and edi unused. Promoting `this` one
//   step in the callee-saved order is therefore worth the whole rest, and no
//   source-level shuffle tried so far moves it.
// - 0x408334: the `u->def` load sits before the three `target = origin` stores
//   instead of after them (pure scheduling of the flag12 test).
// - 0x4084f7: _allmul's first pair is pushed s-first; no source-level FixMul
//   operand order changes it (all four orders emit the same code).
// History: two deepseek-v4.1-flash retries and a GPT-6 retry took this from
// 83.5% to 84.1% (the second one by expanding the loop-2 else-branch
// Direction() call into direct d.x/d.y/d.z stores, angle in an int local, which
// is kept here) without ever moving the allocator.
// Tried without effect: a function-scope unit shared by both loops, a reference
// unit, a while loop, pos/ok/idx declaration order, converting `ok` to
// declaration plus assignment, renaming loop 1's unit, swapping the two
// function-scope declarations, a `Class_00407350* self = this;` copy with every
// field access routed through it (byte-identical output), the iterator as a
// pointer, separate iterators per loop, function-scope ok/idx/kind, `continue`
// chains instead of the && chain, (*it)-> instead of a unit local, direct
// temporaries as FUN_0043adc0 arguments, a Scale helper, FixMul operand orders,
// TooFar() helpers, and Length(d) versus Length(origin - u->pos) in each branch.
// deepseek-v4.1 (2092) retried the last rotation with: one iterator variable per
// for-scope (86.4%), an inline `Player()` accessor for every field_10 read
// (86.9%), an inline `Units()` accessor for every field_8 read (87.4, same
// bytes), and four shapes that compile to the identical 1221 bytes (ok
// pre-initialised to 0, unsigned ok, pos hoisted to the loop-body top, `const u`)
// plus a stray idx local (no change). The rotation is therefore not steered by
// the loop-1 locals, their declaration order or their initialisers.
// Tried and worse: other header sets, the x,y,z or z,y,x orders of Direction()'s
// fields (85.6% and 85.4%), `target = u->pos + d` (85.2%), the in-place
// `d.x += u->pos.x` tail (84.1%), expanding the loop-2 flag12 Direction() call
// into direct stores (67.3%).
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
    v.z = -FUN_004b7123(angle, scale);
    v.y = 0;
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
                        {
                            // Expanded copy of the inlined Direction() call: writing
                            // the three components of d directly avoids the extra
                            // copy and the swapped y/z allocation.
                            int ang = FUN_004b6c30(0x10000);
                            d.x = -FUN_004b70ef(ang, 0x1400000);
                            d.y = 0;
                            d.z = -FUN_004b7123(ang, 0x1400000);
                        }
                    } else {
                        int s = FixDiv(0x1400000, len);
                        d.x = FixMul(s, d.x);
                        d.y = FixMul(d.y, s);
                        d.z = FixMul(d.z, s);
                    }
                    target.x = u->pos.x + d.x;
                    target.y = u->pos.y + d.y;
                    target.z = u->pos.z + d.z;
                }
                Class_00438760 kind = FUN_0043f0e0(9, u, 0, &target);
                FUN_0043adc0(kind, 0, u, 0, &target, 0, 0);
            }
        }
    }
}
