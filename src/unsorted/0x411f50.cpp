// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6, edited by deepseek-v4.1. Names are provisional.
// Started by an earlier partial (Claude Opus 5.5, GPT-6, deepseek-v4.1-flash);
// this version keeps that work and was re-verified by deepseek-v4.1.
// deepseek-v4.1 session 3: the two remaining state-4 sum spellings
// `def->field_216 + 1 + (int)(...)` and `(int)(...) + (def->field_216 + 1)`
// compile byte-identically to the current line, so the integer sum is
// normalised by VC5 (still 96.8%, same 21-line diff).
// "Attacking" order handler of aircraft (VTOL). Interrupts hand over to a
// "VTOL_SEEKATTACK" order; the order follows its target unit and gives up
// outside its range. State 0 prepares the order (FUN_0040f200 is defined here
// because /Ob2 inlined it), states 1 and 2 make attack runs past the target,
// state 4 turns around after a pause, state 5 aims at the target again and
// state 6 flies on and, when the unit is below three quarters of its health,
// sends it to a random repair pad ("VTOL_LANDING").
//
// Partial: 97.1% (deepseek-v4.1 session 4), same 1980-byte size. Session 4 fixed
// BOTH remaining lea-order hunks (0x4121d7, 0x4122b0) by giving cases 1 and 2
// explicit pointer locals that are declared without an initialiser and then
// ASSIGNED, with &order->pos assigned before &unit->pos:
//     Vec3* op; Vec3* up; op = &order->pos; up = &unit->pos;
// In case 1 they sit before the `if`, in case 2 before the `int dist` hypot, so
// both are live across the _hypot call; VC5 then materialises the pair in
// assignment order (lea ebp,[edi+0x22] then lea ebx,[esi+0x6a]) and the earlier
// right-to-left-looking pair vanished. A single declaration with initialisers
// does not do this (it is folded away, 96.8%); the declared-then-assigned form
// is what keeps the value live and orders the leas, and the same form applied
// to unit->def / size / rate in state 4 changes nothing (97.1% either way).
// What still differs (checked against build/scratch/0x411f50/ctx.txt):
//  - 0x4123ad and 0x4123ec: state 4, the turn-time formula. The original keeps
//    unit->def in ebp (mov ebp,[esi+0x92]) and computes
//    `(int)(sqrt(size * 2.0 / rate) * 30.0f * unit->type->field_22) + 1 +
//    def->field_216` as `fmul [30.0]`, `fimul [field_22]`, `inc ebx`,
//    `add ebx, ecx`; VC5 rewrites our spelling into
//    `field_216 - (int)(sqrt(...) * field_22 * -30.0f) + 1` (fmul of the -30.0f
//    constant, `sub ebx, eax`) and puts def in ebx. Earlier passes tried five
//    rewrites plus eleven more source shapes in build/scratch/0x411f50/micro*.cpp
//    and varA..varP.cpp: the (int) result in its own int local, that local
//    passed to an inline helper, the sum split over two statements,
//    `+ def->field_216 + 1`, `1 + ...`, `f216 + 1 + t`, an `unsigned short`
//    copy of field_216, `(int)(float)(...)`, an `unsigned` sum and a sum whose
//    field_216 load is a separate local: every shape materialises the sum in a
//    local first and every one compiles to the negated -30.0f form. The
//    positive +30.0f form appears only when the sum is folded into an `lea`
//    inside a call argument, and the original computes the sum before the
//    if/else (inc/add before the target test), so that shape cannot be used.
//    tools/headers.py over all 128 header sets gave 96.6% for every set (64
//    sets fail to compile without <list>/<vector>), so header state does not
//    flip it either. Naming the product `float sp = (float)sqrt(size * 2.0 /
//    rate) * 30.0f;` does flip VC5 to the original's constant-first multiply
//    order, but it then rewrites fidiv into fdivp/fxch, the field_22 multiply
//    into fild/fmulp and the sum into a single `lea [eax+ecx+1]`, which is 55
//    differing lines against 12 here at the same 97.1%; and writing the sum as
//    `(f216 + 1) - (int)(x * -30.0f)` or as `f216 + (1 - (int)(x * -30.0f))`
//    compiles to the identical negated bytes, so VC5 normalises the sign and
//    the association after instruction selection, not from the source spelling.
//  - 0x4126f0: the switch jump table address still shows as <addr>; check.py
//    resolves relocations only once the code matches, so this may not be a real
//    difference.
//
// Re-verified by deepseek-v4.1-flash: still 96.8% (1980 bytes), first line
// credit kept. This session re-ran the N-declarations sweep (nd1..nd24 flat at
// 96.6%, nd60 down to 93.9%, nd180+ shorter and 83 to 86%) and ~40 more shapes
// in build/scratch/0x411f50/ (cA..cG, h3..h9, kA..kC, q1..q6): pinning the
// product in a float local, reordering the two multiply operands, int/double/
// short/size variants, an inline AngleTo() helper and Vec3* locals for the
// case-1/2 leas. Every one lands on the same 96.8% bytes. The float order IS
// reachable in a function whose size/rate are parameters: `float x =
// (float)sqrt(size * 2.0 / rate) * 30.0f;` then `(int)(x * field_22)` gives the
// original's `fidiv`/`fmul [30.0]`/`fimul [field_22]` (mf.cpp f3), and a micro
// reproducing the whole state-4 body gives the original's positive sum when the
// field_22 multiply becomes `fild`/`fmulp` (micro12 m1/m5/m8/m9/m10). Here every
// shape that keeps `fidiv` reassociates to `fimul field_22; fmul -30.0`, and
// every shape that keeps +30.0 turns the field_22 multiply into `fild`/`fmulp`
// and folds the sum into `lea [eax+ecx+1]`, so the two requirements look coupled
// to file-wide compiler state (the original source's neighbouring functions),
// not to the state-4 expression. The def-in-ebp vs def-in-ebx difference is the
// same swap: original keeps the flags parameter (ebx) through the state-4 test
// and gives def ebp; VC5 reuses the dead ebx here. Forcing flags live with a
// named copy (d1..d4) did not move it.
//
// deepseek-v4.1 session 5 (same 97.1%, 12 differing lines, size still 1980): the
// negation fold is a backend decision, not a source spelling. New evidence from
// build/scratch/0x411f50 (v1..v11, w1..w9, x1..x8, y2..y6, z1..z3, g1..g6,
// q1..q3):
//  - Replacing the literal with a non-const file-scope float (v1) is the only
//    shape found that BLOCKS the negation: the sum then compiles to the positive
//    `lea ebx,[eax+ecx+1]`, but the FP order stays `fimul` then `fmul` and the
//    constant becomes a new data symbol, so it cannot match (93.6%).
//  - Every parenthesised shape that puts the constant multiply first, that is
//    `(S * 30.0f) * f22` in all cast/local/operand-order spellings (x2, x6, x8,
//    q1..q3), does give the original's `fmul [30.0f]` first (93.2%), but VC5 then
//    also reloads rate and turns `fidiv` into `fxch`/`fdivp` and renders the
//    integer multiply as `fild [esp+0x30]; fmulp st(1)` instead of
//    `fimul [esp+0x30]`. So the positive product and the `fimul`/`fidiv` pair are
//    mutually exclusive from this expression.
//  - Statement splits, compound assignment (`time = f216 + 1; time += t`), an int
//    or unsigned short copy of field_22, `1 + f216 + t`, `t + f216 + 1`, an
//    inline helper over size/rate and a helper over all four values all compile
//    to the identical negated bytes (97.1%).
//  - def in ebx vs ebp is a CONSEQUENCE of the fold, not its cause: in the
//    negated form the sum register only becomes live at the `sub`, after def's
//    last use, so VC5 reuses ebx for def; in the original the ftol result goes to
//    ebx immediately, conflicts with def and pushes def to ebp.
// What still differs: the state-4 hunk at 0x4123ad and 0x4123ec (fmul/fimul
// order, `sub`/`-30.0f` versus `inc`/`add`/`+30.0f`, def in ebx), plus the
// unresolved switch jump table address at 0x4126f0.
#include <list>
#include <windows.h>
#include <math.h>
#include <vector>

struct Point { short x, y; };
struct Vec3 {
    int x, y, z;
    Vec3 operator+(const Vec3& other) const {
        Vec3 r;
        r.x = x + other.x;
        r.y = y + other.y;
        r.z = z + other.z;
        return r;
    }
};
// A 16.16 fixed-point position seen as its fraction and whole halves.
struct FixedVec3 {
    unsigned short xf;
    short x;
    unsigned short yf;
    short y;
    unsigned short zf;
    short z;
};

struct Unit;
class Class_0043d210 {
public:
    char unknown_0[0x22];
    short field_22;                    // +0x22
    char unknown_24[0x2e - 0x24];
    unsigned char field_2e;            // +0x2e
    void FUN_0043d210(Unit* unit, int state);
};
class Class_004895c0 {
public:
    Unit* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc
    virtual ~Class_004895c0();
    void FUN_00489690(Unit* o);
};
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_0044e730 { public: void FUN_0044e730(short); };
class Class_00489800 { public: void FUN_00489800(int); };
class Class_004898b0 { public: void FUN_004898b0(int); };
class Class_0048b090 { public: void FUN_0048b090(int, int); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x1fa]; unsigned int maxHealth;
    char pad1fe[0x216 - 0x1fe]; unsigned short field_216;
    char pad218[0x21c - 0x218]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Struct_Unit96 {
    char pad0[0x146]; unsigned char field_146;
};
struct Unit {
    Class_0043d210* type;
    char pad4[0x66 - 4]; short angle;
    char pad68[2];
    union {
        Vec3 pos;                      // +0x6a
        FixedVec3 fixedPos;
    };
    char pad76[0x86 - 0x76]; int field_86;
    char pad8a[8]; UnitDef* def;
    Struct_Unit96* field_96;
    char pad9a[0x108 - 0x9a]; short health;
    char pad10a[0x110 - 0x10a]; unsigned int flags;
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x12 - 0xa]; Class_004895c0 target;
    Vec3 pos;
    Point start;                       // +0x2e
    char pad32[0x3e - 0x32]; int range;
    unsigned int field_42;
    char pad46[0x4a - 0x46]; int field_4a;
};
struct Struct_Game391e9 {
    char pad0[0xd3c]; int field_d3c;
};
struct Game {
    char pad0[0x391e9]; Struct_Game391e9* field_391e9;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
class Class_0044e190 {
public:
    char unknown_0[0x36];
    Class_0044e190(Order* order, Unit* unit);
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, Unit* target, Vec3* pos, int c, int d, int e);
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_004b6c30(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall FUN_0048a980(Vec3*, Vec3*);
void __stdcall FUN_0048a0a0(Unit*, Vec3*, int);
void __stdcall FUN_0048a0f0(Unit*, int);
void __stdcall FUN_0048aac0(Unit* unit, Unit* target, char p3, char p4);
void __stdcall FUN_0043acb0(Unit*, Class_0043a1f0*);
void __stdcall FUN_0043ad10(Unit*, Class_0043a1f0*);
void __stdcall FUN_0040b530(int player, Vec3* pos, int range, std::vector<Unit*>* out);

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

// 0x40f200, matched in 0x40f200.cpp; inlined into the state 0 case below.
void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    ((Class_004898b0*)unit)->FUN_004898b0(3);
    if (unit->field_86)
        FUN_0048aac0(unit, 0, -1, 2);
    ((Class_0048b090*)unit)->FUN_0048b090(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->FUN_0043d210(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// FUNCTION: 0x411f50
int __stdcall FUN_00411f50(Unit* unit, Order* order, unsigned int flags)
{
    if (flags & 0x1000a) {
        if (!order->field_4a && (unit->flags & 0x300000))
            FUN_0043ad10(unit, new Class_0043a1f0("VTOL_SEEKATTACK", order->target.owner, &order->pos, 0, 0, 0));
        return 5;
    }
    Unit* target = order->target.owner;
    if (!target && (order->field_42 & 0x200)) {
        if (!order->field_4a)
            FUN_0043ad10(unit, new Class_0043a1f0("VTOL_SEEKATTACK", 0, &unit->pos, 0, 0, 0));
        return 5;
    }
    if (target)
        order->pos = target->pos;
    if (order->range && (int)_hypot(unit->fixedPos.x - order->start.x, unit->fixedPos.z - order->start.y) >= order->range)
        return 5;
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            ((Class_00438880*)order)->FUN_00438880("Attacking");
            FUN_0040f200(unit, order, 0);
            return 1;
        }
        break;
    case 1: {
        ((Class_00489800*)unit)->FUN_00489800(3);
        ((Class_004898b0*)unit)->FUN_004898b0(0);
        Vec3* op;
        Vec3* up;
        op = &order->pos;
        up = &unit->pos;
        if ((int)_hypot(order->pos.x - unit->pos.x, order->pos.z - unit->pos.z) < 0x1e00000) {
            int angle = FUN_0048a980(up, op);
            Vec3 dest = unit->pos + Offset(angle, 0x8c00000);
            Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
            ((Class_0044e730*)obj)->FUN_0044e730(0x3c0);
            ((Class_004388d0*)order)->FUN_004388d0((int)obj);
            order->flags |= 0xe2;
            return 1;
        }
        return 1;
    }
    case 2: {
        Vec3* op2;
        Vec3* up2;
        op2 = &order->pos;
        up2 = &unit->pos;
        int dist = (int)_hypot(order->pos.x - unit->pos.x, order->pos.z - unit->pos.z);
        int angle = FUN_0048a980(up2, op2);
        int radius = dist / 2;
        Vec3 dest = unit->pos + Offset(FUN_004b6c30(0x4000) + angle - 0x2000, radius);
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x1e0);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0x100e8;
        return 1;
    }
    case 3:
        return 1;
    case 4: {
        if (flags & 0xe0)
            return 1;
        UnitDef* def = unit->def;
        int size = def->field_21c;
        int rate = g_game->field_391e9->field_d3c;
        if (!rate)
            break;
        int time = (int)((float)sqrt(size * 2.0 / rate) * 30.0f * unit->type->field_22) + 1 + def->field_216;
        Class_0044e2d0* obj;
        if (order->target.owner)
            obj = (Class_0044e2d0*)new Class_0044e190(order, order->target.owner);
        else
            obj = new Class_0044e2d0(order, order->pos);
        ((Class_0044e730*)obj)->FUN_0044e730(time);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        ((Class_00439e80*)order)->FUN_00439e80(1);
        order->flags |= 0x100e8;
        return 2;
    }
    case 5: {
        ((Class_004898b0*)unit)->FUN_004898b0(0);
        FUN_0048a0a0(unit, &order->pos, 0);
        int angle = FUN_0048a980(&unit->pos, &order->pos);
        Vec3 dest = unit->pos + Offset(angle, (unit->def->field_216 + 0x3c0) << 16);
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x3c0);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0xe2;
        return 1;
    }
    case 6: {
        FUN_0048a0f0(unit, 0);
        Vec3 dest = unit->pos + Offset(unit->angle, 0x5a00000);
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x80);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0xe2;
        if (unit->health < unit->def->maxHealth / 4 * 3) {
            std::vector<Unit*> pads;
            FUN_0040b530(unit->field_96->field_146, &unit->pos, 0xf00, &pads);
            if (!pads.empty()) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                Unit* pad = pads[FUN_004b6c30(pads.size())];
                FUN_0043acb0(unit, new Class_0043a1f0("VTOL_LANDING", pad, 0, 0, 0, 0));
                order->flags = 0;
                return 0;
            }
            return 0;
        }
        order->state = 3;
        return 2;
    }
    }
    return 7;
}
