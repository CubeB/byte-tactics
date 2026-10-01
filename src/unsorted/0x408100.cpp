// Decompiled by Claude Opus 5.5, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Sonnet 5.5. Names are provisional., finished by deepseek-v4.1-flash
// deepseek-v4.1-flash 10-minute retry (this session): re-confirmed 1221 bytes
// and 98.5%, same three hunks (0x408334 u->def load hoisted above the
// target = origin stores; 0x4083a4 xor ebp,ebp placed after the trig call
// instead of before it; 0x4084f7 _allmul first pair pushed commuted). Two new
// negatives, both 1221 bytes and the same three hunks: the inline Direction()
// helper in x,y,z order (y = 0 statement between the two trig calls) and
// hoisting `Vec3 target = origin;` above the loop-2 condition. The hoist
// regresses to 89.2% / 1234 bytes (the copy then runs for every unit, and the
// condition's def test moves) so the original copy really is inside the taken
// branch. All three hunks are scheduler artifacts of shapes already tried;
// nothing new moved.
// deepseek-v4.1-flash 10-minute retry (this session, second pass): re-confirmed
// 1221 bytes and 98.5% with the same three hunks. Two probes, both byte-neutral
// (1221 bytes, identical hunks): `d.x = FixMul(d.x, s);` for the first FixMul
// (so the commuted _allmul push really is front-end canonicalisation, not source
// order) and expanding the loop-2 flag12 Direction() call into a block with an
// `int ang = FUN_004b6c30(0x10000);` local, the shape that matches in the loop-2
// else branch. The remaining three hunks (0x408334 def-load hoist, 0x4083a4 xor
// placement, 0x4084f7 first _allmul push order) are all scheduler artifacts.
// Sonnet 5.5: 97.8% (was 87.4%). Slot 0 of Class_004085d0 (vtable 0x4fc9a8), derived
// from Class_00407350 (family listed in 0x407350.cpp). Runs every 90 ticks over the
// units of this object's group: first gives each unit that FUN_0040bdb0 picks an item
// for an order (mode 0xe) at the place FUN_0040bfe0 finds, within a third of the map
// size of the player's base (FUN_0040ba80) for flag12 units; then sends the idle units
// towards the base: flag12 units to the point mirrored through it (a random point 0x280
// from it when farther), the others to the base itself, or when within 0x140 of it,
// 0x140 onwards in its direction.
//
// What fixed the loop-1 register rotation (the wall of four earlier passes): the
// allocator ranks loop 1's short webs (unit, idx, ok, range) by weighted use count,
// and the original's `range` outranks the unit. A second, foldable use of `range`
// (`len > range || len > range`, one Length() stored in an int first) swaps unit and
// range into the original's ebp/edi and puts `this` in ebx. The duplicate condition
// compiles to one cmp. Spellings that vanish before the allocator counts them do
// nothing: a copy `int r2 = range`, an inline identity wrapper around range, idx or
// the player argument, `Same(range) < Length(..)`. `len > range && len > range` and
// the `else if (len > range)` form give the same bytes; a real different second use
// (`range > K`) shifts everything, so the weight is wanted, not a second compare.
//
// deepseek-v4.1-flash: 98.5% (was 97.8%). One addition did it: a user-defined
// `Vec3::operator=` (three member stores, returning *this). It flips the loop-2
// flag12 Direction() from ang=ebp (and the xor after `neg eax`) to the original's
// ang=ebx, xor in the gap before the second call, d.z in ebx. With it, the
// helper's own statement order (x,z,y, x,y,z or y,x,z) no longer changes
// anything. The earlier 95.9%-class shapes (helper reordered, aggregate
// initialiser, separate temp `Vec3 e`) keep the extra `mov ebp, ebx`, and a
// hand-written copy constructor is far worse (53.6%).
//
// Still different (3 hunks, 1221 bytes exact):
// - 0x408334: the `u->def` load sits before the three `target = origin` stores
//   instead of after them. Memberwise `target.x = origin.x; ..`, a split
//   declaration, `Vec3 target(origin);` and a `const Vec3&` alias for origin are
//   all byte-identical, and removing the second load (member-scope `def`
//   pointer, flag12 local) does not move it either.
// - 0x4083a4-0x4083d6 (flag12 Direction()): only the schedule of `neg eax` /
//   `add esp,8` / `xor ebp,ebp` differs now (the original fills the gap before
//   the second call with `xor ebp,ebp`, ours runs `neg eax; add esp,8` first).
//   Expanding the call into direct d.x/d.y/d.z stores with an int ang local
//   (the shape that matches at 0x4084c5) is still 71.6% even with operator=:
//   that named web moves loop 1 (this=edi, unit=ebx, ok=ebp).
// - 0x4084f7: _allmul's first pair is pushed d.x-first (original calls
//   _allmul(s, d.x), the second and third are (d.y, s) and (d.z, s) as ours).
//   All FixMul operand orders, a FixMul(__int64, __int64), (__int64)s * d.x,
//   s * (__int64)d.x, a copy `int x = d.x;` and a __int64 temp all give the
//   identical commuted push.
// Earlier findings that still hold:
// - Length() takes a const reference to a temporary (pos - origin): only then are the
//   three fild operands the temporary's own memory, with a stored 0 for y.
// - The range is an inline MapRange() assigned to a local before `origin.y = pos.y`;
//   written in the comparison it is computed after _ftol.
// - <memory.h> gives the mapWidth-first load order in MapRange().
// - The inline Direction() assigns x, z, y (x,y,z puts the loop-2 flag12 branch's xor
//   after the second call). Loop 2's else branch is expanded into direct d.x/d.y/d.z
//   stores with an int ang local.
// - The len<0x1400000 tail is three separate `target.x = u->pos.x + d.x;` stores, not
//   `target = u->pos + d` and not `d.x += u->pos.x`.
// - Tried without effect on the loop-1 rotation (before the range trick): reference or
//   function-scope unit, while loop, goto loop, do/while loop, declaration orders, a
//   `self = this` copy, accessor helpers, iterator forms, continue chains, FixMul
//   operand orders, TooFar() helpers.
#include <memory.h>
#include <vector>
#include <math.h>

class Class_00407350;

struct Vec3 {
    Vec3& operator=(const Vec3& o) { x = o.x; y = o.y; z = o.z; return *this; }
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

// deepseek-v4.1-flash timebox pass (97.8%, 1221 bytes): three new neutral shapes,
// so the three hunks stay as listed above. `Vec3 target; target = origin;` (split
// declaration) is byte-identical, `Vec3 target(origin);` is byte-identical, and an
// explicit `(unsigned char)` cast on the inner loop-2 `u->def->flag12` test is
// byte-identical: the def-load hoist over the three target stores is not affected by
// the condition's cast spelling or by how the copy is declared.
// deepseek-v4.1-flash retry: an explicit `d.y = 0;` before the Direction() call in
// the flag12 arm is byte-neutral too (same 3 hunks, 1221 bytes), so the xor
// ebp,ebp placement in hunk 2 is not reachable by pre-zeroing the destination.
// deepseek-v4.1-flash 10-minute pass: two negatives, both 1221 bytes and the same
// three hunks. Hoisting the loop-2 copy by folding it into the order test is worse
// than neutral: writing `if (!u->order || (u->order->flags & 0x4000)) { Vec3 target
// = origin; if (!(unsigned char)u->def->flag12 || FUN_0040c230(field_10) >= 5) {`
// (the nested form, the only source shape that puts the `def` load after the copy,
// as the original 0x408334 does) re-materialises the origin copy for the flag12 arm
// and duplicates the flag12 test: 91.9%, 1235 bytes, so hunk 1 is not the nesting.
// Assigning `v.y = 0;` first in the inline Direction() helper is byte-neutral
// (same hunks), so the flag12 arm's `xor ebp,ebp` before the first trig call is
// not source order either.
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
                    int len = Length(pos - origin);
                    if (len > range || len > range)
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