// Decompiled by space-bunny-free, Space Bunny Free, deepseek-v4.1, deepseek-v4.1-flash, mimo-v2.6-pro, Opus, Sonnet and Claude Opus 5.5. Names are provisional.
// A unit's mover (the 0x2f-byte object at unit+0): its velocity, speed and
// turn, the steering for ground units and aircraft, and the position update.
//
// <stdlib.h> is part of UpdatePosition's header state and <memory.h> of
// SteerAircraft's (see there).
#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#pragma pack(push, 1)
struct FP_0043d6d0 {
    unsigned int frac : 16;
    int whole : 16;
};

union Fixed_0043d6d0 {
    int value;
    FP_0043d6d0 parts;
};
#pragma pack(pop)

static inline Fixed_0043d6d0 MakeFixed_0043d6d0(int i)
{
    Fixed_0043d6d0 f;
    f.parts.frac = 0;
    f.parts.whole = i;
    return f;
}

inline int operator>(const Fixed_0043d6d0& a, const Fixed_0043d6d0& b) { return a.value > b.value; }

#define MAXM_0043d6d0(a, b) ((a) > (b) ? (a) : (b))

static inline void MulFixed(int& v, int f)
{
    v = (int)(((__int64)v * f) >> 16);
}

// A 16.16 position or velocity.
struct Vec3 {
    int x;
    union {
        int y;
        Fixed_0043d6d0 fy;
        struct {
            unsigned short yFraction;
            short yWhole;
        };
    };
    int z;

    Vec3() {}
    Vec3(int a, int b, int c) : x(a), y(b), z(c) {}
    int Length() const {
        double a = x, b = y, c = z;
        return (int)sqrt(a * a + b * b + c * c);
    }
    Vec3 operator-(const Vec3& o) const {
        Vec3 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    Vec3& operator+=(const Vec3& o) {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    void Scale(int s) {
        MulFixed(x, s);
        MulFixed(y, s);
        MulFixed(z, s);
    }
    void Add(const Vec3* o)
    {
        x += o->x;
        y += o->y;
        z += o->z;
    }
};

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

struct Point {
    short x, y;
    Point() {}
    Point(int a, int b) : x(a), y(b) {}
};

#pragma pack(push, 1)
struct Short3 {
    short x, y, z;
};

struct Game {
    char unknown_0[0x14263];
    int count2;                        // +0x14263
    char unknown_14267[0x1427f - 0x14267];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x142b7 - 0x14280];
    int field_142b7;                   // +0x142b7
    char unknown_142bb[0x38a47 - 0x142bb];
    int field_38a47;                   // +0x38a47
};

struct UnitType_0043cd20 {
    char unknown_0[0x192];
    int field_192;                     // +0x192, the range
    char unknown_196[0x19a - 0x196];
    int field_19a;                     // +0x19a, the rate (top speed)
    int field_19e;                     // +0x19e, the long-step distance (acceleration)
    int field_1a2;                     // +0x1a2
    int field_1a6;                     // +0x1a6
    char unknown_1aa[0x1ae - 0x1aa];
    int field_1ae;                     // +0x1ae
    int field_1b2;                     // +0x1b2
    int field_1b6;                     // +0x1b6
    unsigned short max_turn;           // +0x1ba
    char unknown_1bc[0x22c - 0x1bc];
    unsigned char draft;               // +0x22c
    char unknown_22d[0x241 - 0x22d];
    union {
        int field_241;                 // +0x241
        struct {
            unsigned int mode_bits : 11;
            unsigned int flag_800 : 1; // bit 11
        };
        struct {
            unsigned int low : 19;
            unsigned int b19 : 1;      // bit 19
            unsigned int rest : 12;
        };
    };
};

struct Target_0043d6d0 {
    int field_0;                       // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
};

struct TargetData_0043d6d0 {
    char unknown_0[8];
    Vec3 velocity;                     // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                      // +0x20
};

struct Path_0043d6d0 {
    TargetData_0043d6d0* field_0;      // +0x0
};
#pragma pack(pop)

class CobScript { public: void StartScript(const char*, int, int); };

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x64];
    union {
        Short3 f64;                    // +0x64
        struct {
            short field_64;            // +0x64
            short heading;             // +0x66
            short field_68;            // +0x68, in 2048ths of a circle
        };
    };
    Vec3 pos;                          // +0x6a
    Point cell;                        // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point draft;                       // +0x7e
    int field_82;                      // +0x82
    Path_0043d6d0* obj;                // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType_0043cd20* type;           // +0x92
    Target_0043d6d0* target;           // +0x96
    CobScript* script;                 // +0x9a
    char unknown_9e[0xa8 - 0x9e];
    unsigned short id;                 // +0xa8
    char unknown_aa[0xf9 - 0xaa];
    signed char index;                 // +0xf9
    char unknown_fa[0x110 - 0xfa];
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int mode : 2;     // +0x110 bits 0-1
            unsigned int flags_2 : 14;
            unsigned int moved : 1;    // +0x110 bit 16
            unsigned int flags_17 : 15;
        };
    };

    void SetStateBits(int param_1, int param_2);
};

// The movement state saved as "u%04xmob".
struct Record_0043dd70 {
    Vec3 velocity;                     // +0x00
    Vec3 p2;                           // +0x0c
    int field_20;                      // +0x18
    short field_24;                    // +0x1c
    int field_26;                      // +0x1e
    unsigned char mode : 2;            // +0x22 bits 0-1
    unsigned char flag : 1;            // +0x22 bit 2
};
#pragma pack(pop)

class HapiBank {
public:
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int ReadBox(void* buf, int size);
    int WriteBox(void* src, int len);
};

extern Game* g_game;
extern signed char DAT_00505205[];

int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
void __cdecl FUN_004b7173(short angle, int* xz);
int __cdecl FUN_004b715a(int x, int z);
int __stdcall GetHeadingBetween(Vec3* from, Vec3* to);
Vec3 __stdcall GetPiecePosition(Path_0043d6d0* obj, int index);
Short3 __stdcall GetPieceAngles(Path_0043d6d0* obj, int index);
void __stdcall SetUnitPosition(Unit* unit, Vec3 pos, int mode);
int __stdcall FUN_0047db70(UnitType_0043cd20* type, short a8, Point cell, int mode);
void __stdcall RemoveUnitFromMap(Unit* unit);
void __stdcall AddUnitToMap(Unit* unit);
void __stdcall UpdateUnitLineOfSight(Unit* unit);

// The behaviour object at +0x0: the path the mover follows.
class Iface_0043dd20 {
public:
    virtual void v0(int);
    virtual void v1();
    virtual void v2();
    virtual void v3(Vec3* out, int first, int count);
    virtual void v4(Vec3* a, Vec3* b, short* heading);
    virtual int v5();
};

class Class_00490880 { public: char unknown[0x27]; Class_00490880(Unit* unit); };
class Class_004907e0 { public: char unknown[0x28]; Class_004907e0(Unit* unit); };
class Class_0044f570 { public: char unknown[0x1c]; Class_0044f570(Unit* unit); };
class Class_0044f010 { public: char unknown[0x65]; Class_0044f010(Unit* unit); };
class Class_0043db50 { public: void UpdateSfxOccupy(Unit* u); };

static inline void ClampToZero(int& value)
{
    if (value < 0)
        value = 0;
}

static inline void AddFixed(int& v, float f)
{
    v += (int)((double)f * 65536.0);
}

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

static inline void ClampToCell(Vec3& pos, Point cell, Point draft)
{
    int cx = (draft.x + cell.x * 2) << 19;
    int cz = (draft.y + cell.y * 2) << 19;
    if (pos.x > cx + 0x7ffff)
        pos.x = cx + 0x7ffff;
    else if (pos.x < cx - 0x7ffff)
        pos.x = cx - 0x7ffff;
    if (pos.z > cz + 0x7ffff)
        pos.z = cz + 0x7ffff;
    else if (pos.z < cz - 0x7ffff)
        pos.z = cz - 0x7ffff;
}

#pragma pack(push, 1)
class UnitMotion {
public:
    Iface_0043dd20* obj;               // +0x0
    int field_4;                       // +0x4
    Vec3 velocity;                     // +0x8
    Vec3 p2;                           // +0x14
    int field_20;                      // +0x20, speed
    short field_24;                    // +0x24, turn this tick
    int field_26;                      // +0x26
    int field_2a;                      // +0x2a
    union {
        unsigned char field_2e;        // +0x2e
        struct {
            unsigned char mode : 2;    // bits 0-1
            unsigned char flag : 1;    // bit 2
            unsigned char rest : 5;
        };
    };

    void FUN_0043cc20(Unit* unit, int amount);
    void SteerGroundUnit(Unit* unit);
    void ApplyBankAndPitch(Unit* owner, Vec3* v);
    void SetFlightMode(Unit* owner, int state);
    void SteerAircraft(Unit* unit);
    void UpdatePosition(Unit* unit);
    void UpdateMoveRate(Unit* unit);
    UnitMotion(Unit* unit);
    void DestroyObject();
    void UpdateMotion(Unit* u);
    void SaveMotion(Unit* info, HapiBank* file);
    void LoadMotion(Unit* unit, HapiBank* file);
};
#pragma pack(pop)

// Adds `amount` to the object's distance accumulator (clamped at zero), limits
// it to a range taken from the table at DAT_00505205, then writes the offset
// for the unit's heading at that distance into the velocity.
// FUNCTION: 0x43cc20
void UnitMotion::FUN_0043cc20(Unit* unit, int amount)
{
    field_20 = field_20 + amount;
    ClampToZero(field_20);

    // Direction index, limited to the eleven entries of the table.
    int idx = unit->field_68 >> 11;
    if (idx < -5)
        idx = -5;
    if (idx > 5)
        idx = 5;

    // 16.16 range from the table entry, halved below sea level.
    int range = (int)(((__int64)(DAT_00505205[idx] << 16) * unit->type->field_192) >> 16);
    range = (int)(((__int64)range << 16) / 0x640000);
    if (unit->pos.yWhole < g_game->seaLevel && !(unit->type->field_241 & 0x81000))
        range = (int)(((__int64)range * 0x8000) >> 16);
    if (field_20 > range)
        field_20 = range;

    int dist = field_20;
    unsigned short angle = unit->heading;
    Vec3 v;
    v.x = -FUN_004b70ef(angle, dist);
    v.y = 0;
    v.z = -FUN_004b7123(angle, dist);
    velocity = v;
}

// Claude Opus 5.5, 2026-10-03: the bytes now match (943 bytes, up from
// 94.8%). check.py still prints "bytes match, but a reference is wrong"
// because it reads the two 0x500000 immediates (`gap1 > 0x500000`, `gap1 -
// 0x500000`: 80.0 in 16.16 fixed point) as hard-coded addresses; they are
// plain constants, and no spelling can give them a relocation. Three changes:
//  1. The real preceding function, UnitMotion::FUN_0043cc20 (0x43cc20), is
//     defined above this one. With it in the file the two
//     final calls cross-jump as in the original (one shared `mov
//     ecx,[esp+0x10]; push eax; push edi; call`, the then arm ending in a
//     `jmp`): 94.8 -> 96.7. Its Unit and UnitDef declarations are merged with
//     this file's; +0x70 (the whole part of pos.y that 0x43cc20 reads) is a
//     union view in Vec3.
//  2. The `imul ecx`: VC5 only narrows a 64-bit multiply to a one-operand
//     imul when neither operand's sign extension is shared with another
//     multiply. `(__int64)field_20 * field_20` below used to CSE the same
//     `(__int64)field_20`, which forced `_allmul` (87.7). Reading field_20
//     into a local (`spd`) for that square, and writing the turned product as
//     `(__int64)(adiff & 0xffff) * (__int64)field_20`, gives the original's
//     `mov eax,esi; and eax,0xffff; ... imul ecx` (96.7 -> 98.1). The
//     `(unsigned short)adiff` spelling swaps the operands' registers (95.8).
//  3. The hasPath==0 arm binds its amount to a const reference,
//     `const int& amount = -unit->type->field_19a;` (found by permute.py as an
//     address-taken copy, then reduced to this). The bound temporary is what
//     makes VC5 load unit into ecx before the `mov [esi+0x24],ax` store and
//     keep the rate in eax (98.1 -> bytes match). A plain int, a named rate,
//     `turn = hasPath`, type locals, local unit copies, inline Brake helpers,
//     an out-parameter helper and every shared-call spelling leave unit in eax
//     (or edi) there.
// The turn block also compiles byte-identically as an inlined call of 0x43cbb0
// (Class_0043cbb0::FUN_0043cbb0(unit, diff), the same clamp, defined above).
//
// The older notes below predate these changes; their tail, imul and arm
// findings were measured without the preceding function and no longer hold.
//
// DeepSeek V4.1 Flash, 2026-10-02 (fresh continuation worker, 94.8% kept,
// no new best). Re-ran check.py on the file as it stands: 94.8% (943 original,
// 964 ours), the same three hunks (arm rotation, imul ecx, tail call split).
// stackcmp shows the frame fully aligned (0x44 + 0x10 saved), so only
// registers and instruction order remain. Measurements this pass, all on the
// 94.8 base and all byte-identical to it unless noted:
//  - arm: `turn = 0` (any spelling) still stores ax and keeps the store first;
//    keeping hasPath live through the argument (`-r2 + hasPath`,
//    `-(r2 - hasPath)`, `*hp`) folds to the immediate 0 and drops to 92.9
//    (rate-first) or stays 94.8; per-arm `Unit* u` copies, `&unit`, `unit + 0`
//    and a `Unit** up` all scalarise and sink the load past the store. The
//    original's unit-in-ecx only appears when the rate load is written first,
//    which then folds the store to `mov [esi+0x24], 0`; the two requirements
//    (store ax first, unit loaded before it) remain antagonistic.
//  - tail: EVERY value-select spelling duplicates the call in both arms
//    (if/else amount, ternary as argument or assigned, nested ternary, goto,
//    switch, do/while, block-local amount in each arm, trailing return,
//    inverted condition with bodies swapped). Only the pointer select joins
//    (84.1, 949 bytes) and its join adds a `lea`/load and pushes in edx, not
//    the original's `mov eax,[esp+0x14]; neg eax` phi. All 16 tail rewrites
//    this pass scored <= 94.8 (the two-call base and the inverted-condition
//    variant are byte-identical). Cross-jumping the two identical call tails
//    is what the original shows, but VC5 will not do it from here.
//  - imul: the turned block differs by only `imul ecx` vs `imul eax,ecx; cdq`.
//    The 64-bit form `(__int64)(unsigned short)adiff * field_20` does narrow
//    to `imul ecx` in an isolated function but not inside this one: every
//    in-function spelling (adiff temp of every width, field_20 temp, cast
//    order, operand order, separate __int64 product temp, `prod *= field_20`,
//    an __inline helper, abs respelled as a ternary) raises the score drop to
//    87.7 (972 bytes, extra _allmul). The abs()-derived adiff is the likely
//    trigger: a small standalone with abs() also widens to _allmul, while a
//    plain parameter does not.
//  - tools/permute.py 3 min, 2087 candidates, 2 jobs: no gain (94.8 -> 94.8).
//    tools/headers.py, 256 sets: closest is the current <math.h> at 94.8,
//    no set matches. So this is not compiler state.
// What still differs: the three hunks above. Best left as is.
//
// space-bunny-free, 2026-10-02 (about 15 check/compile rounds, best still
// 94.8%, file unchanged). New measurements, all on the 94.8% base:
//  - arm, the fold can be stopped. Taking the address of the v5 result keeps
//    the `mov [esi+0x24],ax` store: `int* hp = &hasPath; turn = *hp;` compiles
//    the store from ax (VC5's conditional propagation cannot see through the
//    pointer, and the back end still coalesces the load), whether `hp` is
//    declared before the `if` or inside the arm, and also with a local
//    `Unit* u = unit` next to it. So the store form is no longer the blocker.
//    What is still missing is the position: the unit load sinks past the store
//    in every combination (hp alone, u alone, both, and rate-first), and
//    rate-first plus the pointer store is 92.9 (966 bytes), worse than the
//    94.8 store-first version. The two requirements really are antagonistic:
//    store first keeps the rotation (unit=eax, type=ecx, rate=edx), rate
//    first keeps the rotation the original wants (unit=ecx, type=edx,
//    rate=eax) but moves the store after all three loads instead of after the
//    first one, and folds it.
//  - tail, new positive result: the original's diamond (one shared call, the
//    then arm ending in `jmp` over the else arm) IS reachable. Selecting a
//    POINTER rather than a value leaves the join alone:
//      int negrate = -rate;
//      int amount = *(d1 > lim && d2 > r ? &unit->type->field_19e : &negrate);
//    compiles to `mov eax,[edi+0x92]; add eax,0x19e; jmp $L; $L: lea
//    eax,_negrate; $L: mov edx,[eax]; mov ecx,_this; push edx; push edi;
//    call`. Every value phi with two non-empty arms is duplicated into both
//    arms by VC5 instead (if/else assign, ternary assigned or as the argument,
//    braces, else first, goto, switch, and an inline helper returning the
//    amount: all 986 bytes, 82.6%). The join also survives when the else arm
//    is EMPTY, because the CFG is then not a diamond:
//      int amount = -rate; if (d1 > lim && d2 > r) amount = unit->type->field_19e;
//    keeps one call, but hoists `neg` above the tests, lands the phi in esi
//    (callee-saved) and needs no `jmp`. Note the criterion is the empty arm,
//    not a value before the branch: `int amount = rate; if (...) amount =
//    field_19e; else amount = -amount;` still duplicates. So for the original
//    the remaining lead is the duplication threshold: the pointer join is 11
//    instructions against the value join's 10, so if a select can be spelled
//    with one more instruction in the join (or one less in each arm) the
//    shared call may survive.
//  - imul: the one-operand `imul ecx` is reachable from the 64-bit spelling,
//    but not inside this function. Standalone, `(__int64)(unsigned short)a *
//    s->f / s->m` (a, s->f, s->m all different pointers, member field_20
//    through this too) compiles to `and eax,0xffff; imul DWORD PTR [ecx];
//    mov esi,edx; ...; cdq; push edx; push eax; push esi; push ecx; call
//    _alldiv`, exactly the original's shape. Inside 0x43cd20 every spelling
//    drops to `_allmul` with an early `cdq` (87.7%, 972 bytes): the member or
//    a local copy of field_20, either operand order, and an `unsigned short`
//    temp for the other operand all behave the same. So this is register
//    pressure where the multiply sits, not the expression, and it is worth
//    revisiting only together with a fix for the tail (the two interact: the
//    64-bit form moves the _alldiv arguments and the whole tail block with
//    them).
//
// mimo-v2.6-pro, 2026-10-01 third retry (fresh continuation worker): re-ran
// check.py on the file as it stands: 94.8% (original 943, ours 964), kept.
// New measurements this pass (all scored on scratch copies):
//  - tail: every one-call (phi) spelling still sinks the call into both arms
//    AND shifts the whole allocation (lazy callee-saved pushes at the branch
//    target, this at [esp+4], d1 in ebx instead of ebp): nested ternary
//    `d1 > lim ? (d2 > r ? A : B) : B` 81.7, `goto callit` before one call
//    82.6 (same as the if/else select and the ternary argument). New two-call
//    spellings (braced arms + goto to a shared label after (94.8), explicit
//    `return` after the last call (94.8), else arm reloading
//    `-unit->type->field_19a` so both arms read unit->type (94.8), a named
//    amount local in each arm (94.8)) are all byte-identical to this file.
//    Guide research: 0x4034a0's cross-jump merges a call tail AFTER the
//    differing argument's push (RTL push order puts the last parameter's push
//    in the arm: `push x; jmp L` / `L: mov ecx, this; push common; call`), so
//    even a successful two-call merge would push the amount in the arms and
//    share only `push edi; mov ecx, this; call`, which is NOT the original
//    (`mov ecx,[esp+0x10]; push eax; push edi; call` with both pushes in the
//    join). So the original's join shape can only come from a phi feeding one
//    call, and every phi spelling found duplicates that call into the arms.
//  - arm, the key finding: the rotation IS reachable. Writing the rate
//    statement BEFORE the turn store (`int r2 = unit->type->field_19a; turn =
//    hasPath; call(unit, -r2);`) produces the original's rotation exactly:
//    `mov ecx,[esp+0x58]; mov edx,[ecx+0x92]; mov eax,[edx+0x19a]; ...
//    neg eax; push eax; push ecx; mov ecx,esi; call` (92.9). It fails only
//    because the store folds to `mov word ptr [esi+0x24], 0` instead of the
//    original's `mov [esi+0x24], ax`: once the arm's first statement is past,
//    VC5 has propagated `hasPath == 0` from the branch test and constant-folds
//    the assignment. With `turn = hasPath` written FIRST (this file) the store
//    stays `mov [esi+0x24], ax` but the unit load then sinks past it and the
//    rotation goes one step off (unit=eax). Stopping the fold with `short* tp
//    = &turn; *tp = hasPath;` (still `mov [esi+0x24], 0`) and with an inline
//    `SetTurn((short)hasPath)` member (still folds) both stay 92.9. The rule
//    is statement position, not the store's form: inside an inline helper
//    body, `*turnSlot = t` through a pointer parameter keeps the ax store
//    only when it is the body's FIRST statement (receiver load still sinks
//    past it); moved after another statement it folds to the immediate 0
//    again. So the whole arm gap is: keep the store as the FIRST statement
//    (for the ax store) yet make the compiler evaluate `unit` before it and
//    hold it in a register (for the ecx rotation). Likely candidates not yet
//    tried: a value for the store that is in ax but not the branch-zero name
//    (some alias of the v5 result the optimizer cannot see through), or a
//    receiver/argument expression for an inlined helper whose `unit` load
//    cannot sink because it is computed rather than reloaded from the
//    parameter slot.
//  - arm: the store `turn = hasPath` always hoists to the front of its
//    statement no matter where the comma puts it (arg1 comma 94.8, arg2 comma
//    94.8, double comma 94.8), and every copy of `unit` is scalarised with its
//    load sunk past the store: block-scoped `Unit* u = unit;` (94.8), `u =
//    unit + 0` (94.8), rate named before the store (92.9), an inline member
//    helper `SlowStep(unit, hasPath)` whose parameter bind loads unit before
//    the body's store (94.8, still sunk), a free static inline helper with
//    this passed explicitly (91.2). The load-store-rotate sequence
//    (`mov ecx,[esp+0x58]; mov [esi+0x24],ax; mov edx,[ecx+0x92];
//    mov eax,[edx+0x19a]`) is still unmatched.
//  - imul: `((__int64)((unsigned short)adiff) * field_20)` (64-bit product)
//    matches `imul ecx` but sign-extends field_20 early (cdq + spill, _alldiv
//    args reordered) and drags the turned block with it: 87.7 (972 bytes),
//    confirming the earlier combined measurement. Syntax gotcha: VC5 rejects
//    `(__int64)(unsigned short)adiff * field_20` with C2059; it needs
//    `(__int64)((unsigned short)adiff)`.
//
// mimo-v2.6-pro, 2026-10-01 second retry: 94.8% (original 943 bytes, ours
// 964). Two spellings lifted the 81.7% base (the old negative measurements
// below were all made on the 74.8% base and no longer hold):
//  1) the tail is TWO call statements, one in each arm of
//     `if (d1 > lim && d2 > r) call(unit, unit->type->field_19e); else
//     call(unit, -rate);`. Every select spelling (if/else amount plus one
//     call, ternary as the argument or assigned, braced or not) makes MSVC
//     sink the call into both arms with two epilogues (988 bytes, 83.5);
//     with the call written in each arm the whole register allocation falls
//     into place at once: the ebx<->ebp swap, the prologue register saves
//     and the v3 call setup all match the original (81.7 -> 92.9).
//  2) the hasPath arm is `turn = hasPath; int r2 = unit->type->field_19a;
//     call(unit, -r2);` (store first, rate named after it): that stores ax
//     as in the original instead of an immediate 0 (92.9 -> 94.8).
// Still differs (three things, all small):
//  - the arm's load order: the original loads unit into ecx BEFORE the turn
//    store (eax is still busy with the v5 result, so the scratch rotation
//    runs unit=ecx, type=edx, rate=eax with `neg eax; push eax; push ecx;
//    mov ecx,esi`); ours stores first and the rotation is one step off
//    (unit=eax, type=ecx, rate=edx). Forcing the unit load above the store
//    failed: `Unit* u = unit;` is scalarised and its load sinks past the
//    store (94.8 same), a type-pointer copy before the store (87.1), the
//    comma forms `(turn = hasPath, unit)` and `(turn = hasPath,
//    -unit->type->field_19a)` (identical to store-first), an inline rate
//    chain (identical), `turn = 0` with the literal-reuse trick (94.8
//    same), a doubled `turn = hasPath; turn = hasPath;` pair (dead-store
//    folded, identical) and the fold-away `u += 1; u -= 1` pointer pair
//    (91.1; it does not fold and keeps two adds).
//  - `imul ecx` (one-operand 64-bit multiply) against our `imul eax,ecx;
//    cdq`: the `(__int64)(unsigned short)adiff * field_20` spelling matches
//    those two bytes on its own (81.7 -> 84.2) but breaks the tail's
//    register allocation in every combination with the new arm and tail
//    (92.9 -> 85.8, 94.8 -> 87.7), so the int-product spelling stays.
//  - the tail arm layout: the original merges the two arms before ONE call
//    (`mov eax,[ebx+0x19e]; jmp join; mov eax,[esp+0x14]; neg eax; join:
//    mov ecx,[esp+0x10]; push eax; push edi; call`); the two-call spelling
//    leaves the else arm's copy of the whole call tail out of line after
//    `ret 4` (about 27 diff lines, most of the remaining gap). Also tried:
//    switch on the condition (85.8), a goto label before one shared call
//    (82.6), explicit returns in both arms and else-first (both 94.8
//    same), reverse default-first (77.7) and two `if` assignments of
//    -rate (77.9).
// Earlier attempts (deepseek-v4.1-flash et al, 74.8% base) are kept below for
// the history; their measured negatives still hold where re-measured (the
// 64-bit `(__int64)(unsigned short)adiff * field_20` imul spelling: 71.5
// alone, 67.2 with the merged tail; if/else and ternary selects: 62-66; t1
// with the neg before the tests: 73.4; recompute through ppos: 46.5; ppos
// before the v3 call: 69.6; reversed operator- operands: 71.6).
//
// ---------------------------------------------------------------------------
// Earlier notes (deepseek-v4.1-flash et al), kept for the history:
//
// Decompiled by Space Bunny Free, finished by space-bunny-free, edited by
// deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
//
// deepseek-v4.1-flash, 2026-10-01, second probe: `Vec3* const ppos` is
// byte-identical (74.8%, 964 bytes); moving the hasPath==0 FUN_0043cc20 call
// before `turn = hasPath` regresses to 71.2 (966 bytes); hoisting `int rate`
// above the diff block regresses to 71.8 (978 bytes). Best stays the version
// below.
//
// FUNCTION: 0x43cd20
void UnitMotion::SteerGroundUnit(Unit* unit)
{
    if (obj->v5() == 0) {
        field_24 = 0;
        const int& amount = -unit->type->field_19a;
        FUN_0043cc20(unit, amount);
        return;
    }

    Vec3 p[3];
    obj->v3(p, 0, 3);

    Vec3* ppos = &unit->pos;
    Vec3 d = p[1] - *ppos;
    int gap1 = (int)_hypot(d.x, d.z);

    int dz;
    if (gap1 > 0x500000) {
        int dx = p[1].x - p[0].x;
        dz = p[1].z - p[0].z;
        int len = (int)_hypot(dx, dz);
        if (len >= 0x10000) {
            int ndx = (int)(((__int64)dx << 16) / len);
            int ndz = (int)(((__int64)dz << 16) / len);
            int back = gap1 - 0x500000;
            if (back > len)
                back = len;
            p[1].x -= (int)(((__int64)ndx * back) >> 16);
            p[1].z -= (int)(((__int64)ndz * back) >> 16);
        }
    }

    int az = p[1].z - unit->pos.z;
    int ax = p[1].x - unit->pos.x;
    int d1 = (int)(((__int64)ax * ax) >> 32) + (int)(((__int64)az * az) >> 32);

    short ang = (short)GetHeadingBetween(ppos, &p[1]);
    short diff = ang - unit->heading;
    int sdiff = diff;
    int adiff = abs(sdiff);

    int bz = p[2].z - unit->pos.z;
    int bx = p[2].x - unit->pos.x;
    int d2 = (int)(((__int64)bx * bx) >> 32) + (int)(((__int64)bz * bz) >> 32);

    if (diff != 0) {
        unsigned short max = unit->type->max_turn;
        if (sdiff >= max)
            field_24 = max;
        else if (sdiff <= -max)
            field_24 = -max;
        else
            field_24 = diff;
        unit->heading += field_24;
        unit->moved = 1;
    } else {
        field_24 = 0;
    }

    int turned = (int)((((__int64)(adiff & 0xffff) * (__int64)field_20)
                        / unit->type->max_turn));
    int rate = unit->type->field_19a;
    int spd = field_20;
    int t = (int)(((__int64)spd * spd) >> 16);
    int q = (int)(((__int64)t << 16) / (2 * rate));
    int r = (int)(((__int64)q * q) >> 32);
    int lim = (int)(((__int64)turned * turned) >> 32) * 4;

    if (d1 > lim && d2 > r)
        FUN_0043cc20(unit, unit->type->field_19e);
    else
        FUN_0043cc20(unit, -rate);
}

// Scales the object's second vector (+0x14) by 0.95 (16.16 fixed point), adds the
// offset `v`, rotates the (x, z) pair by the owner's heading, then derives two
// short offsets from the rotated components and the owner type's fields at
// +0x1a2/+0x1a6.
//
// The two Vec3 helpers must stay as inlined methods: written as three separate
// statements on `p2` the compiler keeps the scaled x and y live in callee
// saved registers and emits an extra push; as methods it stores each field
// immediately, as the original does.
//
// Suspected original bug: the second FUN_004b715a call reads xz[0] (the rotated
// x component) instead of xz[1]. Both reads are of the same stack slot:
//   first  call: mov ecx,[esp+8]          (xz[0])
//   second call: push edi; mov ecx,[esp+0xc]  ([esp+0xc] is the same xz[0]
//                                               after the push)
// so field_68 (the z offset) is computed from the rotated x. Kept as-is to
// match the original.
// The original calls this from SetFlightMode and SteerAircraft rather than
// inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x43d0d0
void UnitMotion::ApplyBankAndPitch(Unit* owner, Vec3* v)
{
    p2.Scale(0xf333);
    p2.Add(v);
    int xz[2];
    xz[0] = p2.x;
    xz[1] = p2.z;
    FUN_004b7173(owner->heading, xz);
    int n = (int)(((__int64)g_game->count2 << 16) / 0xccd);
    owner->field_64 = (short)FUN_004b715a((int)(((__int64)owner->type->field_1a2 * -xz[0]) >> 16), n);
    owner->field_68 = (short)FUN_004b715a((int)(((__int64)owner->type->field_1a6 * -xz[0]) >> 16), n);
}
#pragma auto_inline(on)

// Changes the 2-bit mode at +0x2e; entering state 1 clears the velocity and
// field_20 and clears flag 1 on the owner, any other state sets it.
// FUNCTION: 0x43d210
void UnitMotion::SetFlightMode(Unit* owner, int newState)
{
    if (mode != newState) {
        if (newState == 1) {
            field_20 = 0;
            Vec3 zero(0, 0, 0);
            velocity = zero;
            ApplyBankAndPitch(owner, &zero);
            owner->SetStateBits(1, 0);
        } else {
            owner->SetStateBits(1, 1);
        }
        mode = newState;
    }
}

// Steering update for the mover object (UnitMotion): in mode 2 it asks the
// path object for a target position `a`, a target velocity `b` and a heading,
// damps the velocity, clamps its horizontal speed (bleeding the excess off
// along the unit's heading), turns the unit towards the heading, then steers
// p1 towards the target: k * (pos - a) - (velocity - b), limited to the acceleration
// f18. Finally it stores the new speed and hands the velocity change on.
//
// What made it match (Claude Opus 5.5; earlier attempts sat at 91.9%):
//  - The steering term is (pos - a) * k - (velocity - b). Earlier versions had the
//    two deltas the other way round, which compiled to the same x87 shape
//    with the operands loaded from each other's slots.
//  - The two deltas are 12-byte Vec3 locals: dax/daz sit 8 bytes apart with
//    the clamp factor's high dword between them (da shares f's slot), and
//    dbx/dbz sit in the final delta's slot. That needs delta in its own
//    block, so the address-taken local can share a slot.
//  - The fixed-point helpers take the component by reference: MulFixed for
//    Scale and the speed clamp, AddFixed for the final x87 adds. Written
//    inline, the clamp's _allmul pushed the component before the factor and
//    the tail forwarded velocity.x in a register instead of reloading it.
//  - da is assigned x, y, z in order, and <memory.h> is needed: without it
//    the |v| > f18 rescale keeps vx and vz in memory (98.2%). A dummy
//    declaration scan shows two states repeating every 512 symbols.
// FUNCTION: 0x43d290
void UnitMotion::SteerAircraft(Unit* unit) {
    if (mode != 2) {
        velocity = Vec3(0, 0, 0);
        field_20 = 0;
        field_24 = 0;
        return;
    }

    Vec3 old = velocity;
    Vec3 a;
    Vec3 b;
    short heading;
    obj->v4(&a, &b, &heading);

    UnitType_0043cd20* type = unit->type;
    const float eps = 1.52587890625e-05f;

    float f18 = (float)type->field_19e * eps;
    int q = (int)(((__int64)type->field_19e << 16) / type->field_192);
    int scale = 0x10000 - q;
    velocity.Scale(scale);

    float dist = (float)_hypot(velocity.x, velocity.z) * eps;
    float maxd = (float)unit->type->field_19a * eps;
    if (dist > maxd) {
        int f = (int)((double)(maxd / dist) * 65536.0);
        MulFixed(velocity.x, f);
        MulFixed(velocity.z, f);
        int g = (int)((double)(dist - maxd) * 65536.0);
        velocity += Offset(unit->f64.y, g);
    }

    Vec3 da = unit->pos - a;
    Vec3 db = velocity - b;

    if (unit->field_82 != g_game->field_142b7) {
        int lim;
        if ((field_20 & -4) < 0x40000)
            lim = 0x10000;
        else
            lim = field_20 >> 2;
        if (da.y <= -lim)
            velocity.y = lim;
        else if (da.y >= lim)
            velocity.y = -lim;
        else
            velocity.y = -da.y;
    }

    float hd = (float)_hypot(da.x, da.z) * eps;
    short d = (short)(heading - unit->f64.y);
    if (d != 0) {
        unsigned short max = unit->type->max_turn;
        if (d >= (int)max)
            field_24 = max;
        else if (d <= -(int)max)
            field_24 = (short)-max;
        else
            field_24 = d;
        unit->f64.y = (short)(unit->f64.y + field_24);
        unit->moved = 1;
    } else {
        field_24 = 0;
    }

    if (hd < 8.0f)
        hd = 8.0f;

    float k = -sqrt((2.0f * f18) / hd);
    float vx = (float)da.x * k * eps - (float)db.x * eps;
    float vz = (float)da.z * k * eps - (float)db.z * eps;
    float mag = (float)_hypot(vx, vz);
    if (mag > f18) {
        vx = vx * (f18 / mag);
        vz = vz * (f18 / mag);
    }

    AddFixed(velocity.x, vx);
    AddFixed(velocity.z, vz);
    field_20 = velocity.Length();

    {
        Vec3 delta = velocity - old;
        ApplyBankAndPitch(unit, &delta);
    }
}

// Moves the unit one step. With a path object it snaps to the path's next
// point (raised to the draft/sea-level floor for units with the type flag at
// +0x241 bit 19) and copies the path's velocity; without one it adds the
// velocity to the position, and if the unit moves into a new cell that the
// target says is blocked it clamps the position to the current cell and caps
// the speed at half the type's range.
//
// MATCH (Claude Opus 5.5, 2026-10-03; earlier attempts stopped at 74.3%).
// What the bytes needed, each measured:
//   - (mimo-v2.6-pro) the floor clamp's address select (`lea eax,[esp+0x24];
//     jmp` / `mov [esp+0x3c],eax; lea eax,[esp+0x3c]`, then one load) is a
//     MAX macro over the Fixed union with a prvalue second operand; an int
//     max hoists the lea and folds the shift chain.
//   - the path branch is `Vec3 v; v = GetPiecePosition(...)`: the copy through the
//     returned pointer. It only lands right once the no-path branch gives the
//     frame its real layout.
//   - the new position is `pos = velocity + u->pos` through an inline
//     `operator+(const Vec3&, const Vec3&)`, assigned (not initialised): the
//     operator's result temporary is the 12-byte object at frame+0x1c whose y
//     the exe spills to its own home and reloads later (the "ny inside the
//     dead GetPiecePosition temp" the old notes could not place), and its
//     reference to velocity is the `lea ecx,[ebp+8]` the exe keeps at frame+0x8 for
//     the later `velocity = vec`. Separate int sums, a named n, `Vec3 pos = ...`
//     and a `Vec3* pp` all schedule the three adds differently.
//   - u->pos is written with whole-struct copies (`u->pos = pos`), which is
//     what gives the clamp tail its `or dword ptr [esi+0x110],0x10000`.
//   - the cell is assigned field by field, with the half-cell offset written
//     as `draft.x * 0x80000` (the `<< 19` spelling evaluates draft.x first).
//   - the clamp to the current cell is an inline helper taking both Points by
//     value: its parameters are the exe's second dword copies of u->cell and
//     u->draft (into the dead parameter slot and draft's slot), read back
//     through the registers.
//   - `u->mode = (short)m`: a narrow source is what makes VC5 store the
//     2-bit field with `and/and/or` instead of its `xor/and/xor` form.
//   - the target test is two nested ifs, the z component of the speed cap
//     goes through an int temporary, and the file includes <stdlib.h> (the
//     header state; 0x43cd20, which uses abs(), likely shared this file).
//     Each of these three alone scores lower; together they match.
// FUNCTION: 0x43d6d0
void UnitMotion::UpdatePosition(Unit* u)
{
    if (u->obj != 0) {
        Vec3 v;
        v = GetPiecePosition(u->obj, u->index);
        if (u->type->b19) {
            v.fy = MAXM_0043d6d0(v.fy, MakeFixed_0043d6d0(u->type->draft * 0xffff + g_game->seaLevel));
        }
        SetUnitPosition(u, v, mode);
        Short3 o = GetPieceAngles(u->obj, u->index);
        u->f64 = o;
        if (u->obj->field_0 != 0) {
            field_20 = u->obj->field_0->field_20;
            velocity = u->obj->field_0->velocity;
        } else {
            field_20 = 0;
            Vec3 zero(0, 0, 0);
            velocity = zero;
        }
        u->moved = 0;
        return;
    }

    Vec3 pos;
    pos = velocity + u->pos;
    int m = mode;
    if (pos.x == u->pos.x && pos.z == u->pos.z && pos.y == u->pos.y && m == u->mode)
        return;

    field_2a = g_game->field_38a47;
    Point draft = u->draft;
    Point cell;
    cell.x = (pos.x - draft.x * 0x80000 + 0x80000) >> 20;
    cell.y = (pos.z - draft.y * 0x80000 + 0x80000) >> 20;
    if (cell.x == u->cell.x && cell.y == u->cell.y && m == u->mode) {
        u->pos = pos;
        u->moved = 1;
        return;
    }

    if (u->target->field_0 != 0) {
        if (u->target->type == 1 || u->target->type == 2)
            flag = FUN_0047db70(u->type, u->id, cell, m) == 0;
    }

    if (flag) {
        ClampToCell(pos, u->cell, u->draft);

        if (field_20 > (u->type->field_192 / 2)) {
            int half = u->type->field_192 / 2;
            field_20 = half;
            unsigned short angle = u->f64.y;
            Vec3 vec;
            vec.x = -FUN_004b70ef(angle, half);
            vec.y = 0;
            int z = -FUN_004b7123(angle, half);
            vec.z = z;
            velocity = vec;
        }
        u->pos = pos;
        u->moved = 1;
        return;
    }

    RemoveUnitFromMap(u);
    u->pos = pos;
    u->cell = cell;
    u->mode = (short)m;
    AddUnitToMap(u);
    u->moved = 1;
    UpdateUnitLineOfSight(u);
}

// FUNCTION: 0x43da70
void UnitMotion::UpdateMoveRate(Unit* unit)
{
    int rate;
    if ((field_2e & 4) == 0 && unit->obj == 0
        && (field_20 != 0 || field_24 != 0)) {
        if (field_20 <= unit->type->field_1ae) {
            rate = 1;
        } else {
            rate = 2 + (field_20 > unit->type->field_1b2);
        }
    } else {
        rate = 0;
    }
    if (rate == (int)((unit->flags >> 2) & 3))
        return;
    if (rate == 0) {
        unit->script->StartScript("StopMoving", rate, 1);
    } else if ((unit->flags & 0xc) == 0) {
        unit->script->StartScript("StartMoving", 0, 1);
    }
    switch (rate) {
    case 1:
        unit->script->StartScript("MoveRate1", 0, 1);
        break;
    case 2:
        unit->script->StartScript("MoveRate2", 0, 1);
        break;
    case 3:
        unit->script->StartScript("MoveRate3", 0, 1);
        break;
    }
    unit->flags = (unit->flags & 0xfffffff3) | ((rate & 3) << 2);
}

// Constructor of the 0x2f-byte behaviour holder stored at unit+0. It zeroes two
// Vec3-shaped triples and a few scalars, mirrors a value from the unit type and
// then creates one of four behaviour objects, chosen by the unit's target type
// (byte at +0x73 == 3) and bit 11 of the unit type's flags at +0x241.
// FUNCTION: 0x43dc00
UnitMotion::UnitMotion(Unit* unit)
{
    velocity = Vec3(0, 0, 0);
    field_20 = 0;
    field_24 = 0;
    field_26 = 0;
    p2 = Vec3(0, 0, 0);
    mode = 1;
    flag = 0;
    field_4 = unit->type->field_1b6;
    if (unit->target->field_0 != 0 && unit->target->type == 3) {
        if (unit->type->flag_800)
            obj = (Iface_0043dd20*)new Class_00490880(unit);
        else
            obj = (Iface_0043dd20*)new Class_0044f570(unit);
    } else {
        if (unit->type->flag_800)
            obj = (Iface_0043dd20*)new Class_004907e0(unit);
        else
            obj = (Iface_0043dd20*)new Class_0044f010(unit);
    }
}

// FUNCTION: 0x43dd10
void UnitMotion::DestroyObject()
{
    if (obj != 0) {
        obj->v0(1);
    }
}

// FUNCTION: 0x43dd20
void UnitMotion::UpdateMotion(Unit* u)
{
    obj->v2();
    if (u->type->flag_800)
        SteerAircraft(u);
    else
        SteerGroundUnit(u);
    UpdatePosition(u);
    UpdateMoveRate(u);
    ((Class_0043db50*)this)->UpdateSfxOccupy(u);
}

// FUNCTION: 0x43dd70
void UnitMotion::SaveMotion(Unit* info, HapiBank* file)
{
    char name[32];
    Record_0043dd70 hdr;
    hdr.velocity = velocity;
    hdr.p2 = p2;
    hdr.field_20 = field_20;
    hdr.field_24 = field_24;
    hdr.field_26 = field_26;
    hdr.mode = mode;
    hdr.flag = flag;
    sprintf(name, "u%04xmob", info->id);
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&hdr, 0x23);
}

// Load counterpart of 0x43dd70: reads this unit type's movement state back
// from the unit's "u%04xmob" entry.
// FUNCTION: 0x43de30
void UnitMotion::LoadMotion(Unit* unit, HapiBank* file)
{
    char name[32];
    Record_0043dd70 rec;
    sprintf(name, "u%04xmob", unit->id);
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->ReadBox(&rec, 0x23);
    velocity = rec.velocity;
    p2 = rec.p2;
    field_20 = rec.field_20;
    field_24 = rec.field_24;
    field_26 = rec.field_26;
    mode = rec.mode;
    flag = rec.flag;
}
