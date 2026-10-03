// Decompiled by Claude Opus 5.5, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by GPT-6. Names are provisional.
// #5409 Codex retry: 98.5% remains best. The MapRange load order, angle/d
// schedule, and first _allmul call still cannot match together with natural
// source and symbol states.
// #5424 Codex retry: re-confirmed 98.5%; all three scheduling differences remain.
// Claude Opus 5.5 (#5374, 2026-10-04): still 98.5%, file unchanged except this
// note.
// - Hunk 3 (first _allmul) follows C2's own symbol ids alone: 59700 to 60100
//   (and 60500 to 60600) unused externs at the END of the file, which move only
//   the file total and so the ids C2 gives its temporaries, push the first call
//   d.x-first as the original does and leave calls 2 and 3 alone. With the
//   externs before the includes the window is 59700 to 60150 (hunks 1 and 2
//   left), 60200 to 60400 is back to ours and 60450 to 60650 flips all three
//   calls. c2prio --symbols: s 5136, d is no register candidate, file total
//   5568, C2 at 5885 by this function's allocation. No global is an operand,
//   so a block-scope extern cannot move it: it needs about 60000 more symbols
//   (4759 header symbols now, 64459 to 64909 wanted), the lost-header prefix.
// - Block-scope `extern Game_00408100* g_game;` after `origin` and `it`, with
//   MapRange taking the game pointer, is 98.5% with the same three hunks;
//   declared before `origin` it flips MapRange's loads (98.0%), and it still
//   needs <memory.h>. Local declaration order in loop 2 (kind, len, s) is flat.
// - Hunk 2 under c2prio: without the user operator= and with Direction() in
//   x, y, z order the xor lands before the call, but the inlined angle and
//   d.x, d.y, d.z tie at priority 312 (angle: block B28 only, w 4, K 13, cost
//   6; each d field 88 + 104 + 120), angle wins on +0x40 (277 against 260),
//   takes ebp because ebx would cost 6600, and d.y needs a `mov ebp, ebx`.
//   With operator= the v.y temporary is propagated, B28's K is 12, angle drops
//   to 288 and gets ebx, but the xor moves after the call. Flat or worse: 48
//   combinations of int/short ang locals, Direction(int), helper orders, a len
//   local and the three-store target; one, two or block-scoped `kind`s (87 to
//   91%); target copied by memcpy (98.5%), memmove (69%), a pointer or an
//   assignment. The expanded flag12 arm (int ang and direct d stores, as in
//   the else arm) with a plain `len > range` is 76.8% with a 0x5c frame.
// #5336 Codex retry: re-confirmed 98.5%; the three scheduler hunks remain.
// #5352 retry: re-confirmed 98.5%; the target-store ordering and two register
// allocation hunks remain as documented in the C2 notes below.
// claude-opus-5-5 retry (#5274, 2026-10-03): still 98.5%, file unchanged except
// this note. Hunk 3 (the first _allmul pushed d.x first) is a symbol-count
// effect: 59700 to 60100 unused `extern int`s at the top of the file fix it
// (C2's own temporary ids then pass 65536 inside this function) and keep
// MapRange right, 99.0% with hunks 1 and 2 left; no count from 0 to 66000
// moves hunks 1 or 2, and no plausible real header set reaches that count.
// /Gi fixes hunk 3 too, but MapRange's order then follows the path of the
// .pdb that check.py passes with /Fd, not only the source path: a by-value
// `operator=(Vec3 o)` with /Gi is 99.0% (hunks 1 and 2 only) in
// tools/propose.py's object directory at 16 source path lengths, but 98.5%
// through check.py. Hunk 2 under c2prio: without the user operator= and with
// Direction() in x, y, z order the schedule is the original's, but `ang`
// takes ebp because d.z (the one neighbour left with only ebx) interferes with
// it, so v.y gets ebx and a `mov ebp, ebx` follows; with operator= the field
// copies kill d.z in place (no interference, ang in ebx) but v.y = 0 is
// apparently propagated to the copy, so the xor lands after the second call. The
// expanded arm (direct d.x/d.y/d.z stores) raises d.x and d.z to priority 448
// above `u` (308), which is what rotates loop 1. Flat or worse this pass: a
// do/while, `if (0) {}`, a label or a block around `target = origin`, a
// member SetDir(), Direction() taking int, a `short ang` or `int ang` local
// feeding Direction(), and splitting the arm into two `target =` branches
// (68%).
// Codex GPT-6 retry for #5181 (2026-10-03): current main recheck remains
// 98.5%. The three scheduler hunks below have no improved source shape yet.
// GPT-6 retry (#5224): rechecked at 98.5%; the three scheduler hunks remain.
// Claude Opus 5.5 (#4919, 2026-10-03): still 98.5%, file unchanged except this
// note. /Gi trial (judged by differing lines): `// FLAGS: /Gi` removes hunk 3
// (the first _allmul pair is then pushed d.x first, as in the original) and
// leaves hunks 1 and 2 exactly as they are, but it turns MapRange's two loads
// into a tie that the SOURCE FILE'S PATH decides: the same text gives the
// original's mapWidth-first order at some paths and mapHeight-first at others.
// It follows the length of the path, not its characters or depth: one text
// copied to directories named a, aa, ... 24 a's (and b's) is good for 4
// lengths in every 8 and bad for the next 4. Each MapRange spelling (operand
// order, <memory.h>, extra parentheses, a cast, a game-pointer parameter, a
// member function, locals) only shifts that 8-length window, and one (a local
// `Game* g`) is bad at all 8; renaming identifiers shifts it too, and the
// object directory does not matter. Every spelling tried is mapHeight-first
// at this worktree's src/unsorted/ path, so /Gi is 98.5% / 12 lines here and
// 99.0% / 8 lines at other paths, against a path-independent 98.5% / 16 lines
// without it. Not committed for that reason. Neighbours 0x407e90, 0x408090
// (same vtable block) do not match under /Gi; 0x408f30, the
// vector<Unit*>::insert after this TU, does.
// GPT-6 retry: separate target component stores reproduce the known 97.8%
// two-hunk shape; a short angle local and duplicated Length test do not fix
// either remaining scheduler hunk. Keep the 98.5% source below.
// New lead for hunk 1, both flag sets: declaring `Vec3 target;` at loop-body
// scope (assigned `target = origin;` in the branch, slot still esp+0x30)
// keeps the `u->def` load after the three target stores, as the original
// does; the branch-scope copy lets the scheduler hoist it. The cost is the
// len<0x1400000 tail: with loop scope its loads are no longer hoisted above
// the target stores (1219 bytes, `add ebp, [esi+0x6a]`); `target = u->pos + d`,
// `d + u->pos`, a `Vec3 p = u->pos` copy or a `t` temporary give the right
// loads-then-adds-then-stores order but the sums in the pos registers
// (ecx/edx/edi) where the original adds x into d.x's ebp and y, z into
// ecx/edx (96.5%); `d += u->pos; target = d;` loads y, z, x (97.0%). Also
// flat this pass: one Vec3 shared by loop 1's pos and loop 2's target (89%),
// function-scope target (95%, it leaves pos's slot), the sibling 0x407e90's
// Vec3 (default constructor, (x, y, z) constructor, operator+=), and the
// expanded flag12 arm under /Gi (72%, d.y loses its register). Hunk 2 under
// /Gi: without the user operator= and with Direction() in x, y, z order the
// xor does move before the second call, but into ebx with `mov ebp, ebx`
// after it and ang in ebp (1223 bytes); no rename moved that pair.
// mimo-v2.6-pro retry pass: re-confirmed 1221 bytes / 98.5% and the same three
// hunks. New negatives, all unchanged or worse: includes <string.h>, <windows.h>
// and <stdio.h> do not move the _allmul push order; a `UnitDef* def` local before
// the copy, an `int flag` local, stores through `int&` aliases and an inline
// Unit::FillAndFlag(Vec3&, const Vec3&) method (0x463610 pattern) all leave the
// 0x408334 def load hoisted above the target = origin stores (MSVC resolves the
// reference stores back to esp-relative, so no alias barrier survives); the
// nested order-test form (e1) still re-copies origin after the call (91.9%).
// The flag12 Direction() arm expanded to the 0x4084c5 shape (top-level d.y = 0,
// the only shape that puts the xor in the gap) reproduces the known 71.6%
// loop-1 rotation (this=edi, unit=ebx, ok=ebp): neither the len>range trick at
// 1..4 uses, foldable double tests of ok/idx/u, declaration-order swaps of ok,
// idx and range/len, nor scalar DirX/DirZ or a member Vec3::Dir() helper
// (71.1%) restores the original's this=ebx, unit=ebp, ok=ebx web ranking, so
// the rotation is tied to the helper's temporary webs, not to those weights.
// A helper without y=0 plus a caller `d.y = 0` (r1) swaps d.y/d.z registers and
// puts the xor after neg eax (97.0%). All three hunks remain scheduler artifacts
// of shapes already tried by earlier passes.
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
// deepseek-v4.1-flash, nine more probes this pass (all 1221 bytes; the three hunks
// above never moved): the first FixMul spelled `(__int64)d.x * s`, `(__int64)d.x *
// (__int64)s`, `(__int64)s * (__int64)d.x`, an __int64 local, an `(int)` temp and two
// alternative FixMul helpers (a __int64 first parameter, an __int64 product local)
// all emit the identical commuted push, so the original's d.x-first first pair is not
// front-end operand order; a `(unsigned)d.x` shifts the tail (96.5%) and a
// FixDiv-inlined first operand breaks the web (81.8%). For hunk 1, a `def` local
// declared right after the copy is byte-identical and a comma-expression second test
// regresses to 77.6%. An out-param DirectionTo() helper (70.3%) or an expanded flag12
// arm sharing one function-scope `ang` (71.6%) both destroy the loop-1 web, so the
// flag12 arm must stay a returned temporary.
// DeepSeek V4.1 Flash retry pass (this session): permuter 3 min over 2821
// candidates (172 uncompilable, 19 duplicates), 98.5% -> 98.5%. Manual probes,
// all 1221 bytes and the same three hunks unless noted: an inline
// Unit::CopyAndFlag(target, origin) method returning def->flag12 (the 0x463610
// member-method pattern), the target stores through a `Vec3&` and through a
// `const Vec3* p`, a CopyVec(origin) return-by-value helper, `Vec3
// target(origin)`, nested ifs for the loop-2 condition, 0x408090 added before
// this function (with and without a swapped MapRange operand order; unlike the
// earlier note it changed nothing here), and fresh value numbers for the FixMul
// operands (`int sy = s` before and after d.x, `int dy = d.y`, `+s`). Worse:
// FixMul statement order d.y,d.x,d.z and `FixMul(s, d.y)` 84.0% / 1223 bytes;
// operator= y,x,z 98.3% with a new loop-2 hunk; operator= x,z,y 97.0%. Nothing
// moved the three scheduler hunks.
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
//
// Space Bunny Free: 98.5% confirmed and left as the best version (1221 bytes, same
// three hunks). The permuter ran 15.1 min over 825 candidates (47 uncompilable,
// 8 duplicates) and moved nothing, so I stopped rather than spend on the last 1.5%.
// I then built a fast probe instead of check runs: every variant below is compiled
// straight with tools/wcl (/c /O2 /Ob2 /MT /Fa) and only the three hunk windows are
// read, so a probe costs about 5 s. Thirty-four probes were byte-identical to the
// baseline in all three windows (files and specs in build/scratch/0x408100/):
// - hunk 3, first _allmul: eight spellings of the multiply (`FixMul(s, d.x)`,
//   `FixMul(d.x, s)`, `(__int64)s * d.x`, `(__int64)d.x * s`, a `__int64` product
//   local, a separate `int nx` result, `(int)d.x`, and the FixMul body with its two
//   parameters transposed) ALL emit `push ebx; push edi; push edx; push eax`, so
//   the argument order is fixed by C1's commutative canonicalisation and no source
//   spelling reaches the original's `push edx; push eax; push ebx; push edi`.
//   Eleven header sets (<stdlib.h>, <string>, <list>, <map>, <string.h>,
//   <windows.h>, <stdio.h>+<stdlib.h>+<ctype.h>, the game header block, the limits
//   and float headers, <time.h>+<assert.h>+<setjmp.h>, <new>) also leave all three
//   hunks untouched, so the header-state lever from the guide is dead here too.
// - hunk 2, the flag12 Direction() arm: the out-of-line `static inline void
//   SetDirection(Vec3&, short, int)` writing d.x/d.y/d.z in x,y,z and in x,z,y
//   order, a separate `Vec3 e` filled field by field from the returned temporary,
//   an explicit `short a` with direct d.x/d.y/d.z stores, and hoisting the test into
//   an `if/else` that copies d are all identical. The helper's statement order
//   (x,z,y current; x,y,z; y,x,z; z,y,x) makes no difference either, so the
//   `xor ebp,ebp` after `add esp, 8` is not the helper's y=0 ordering.
// - hunk 1, the `u->def` load: an `IsAir(Unit*)` inline getter for the third
//   flag12 test, a second `Unit* v = u` local, `Vec3 target;` declared outside the
//   if with the copy and a `def` local after it, and the three stores written out
//   by hand are all identical, so the hoist over the copy is C1's load preloading,
//   not the order of the statements.
// The one documented lever I did not spend on: the guide's "define the preceding
// function in the same file" (0x408090, 111 bytes, the map bit test, is the real
// neighbour). Compiler state from an earlier function can decide these, but it
// needs that function matched first, which is a job of its own.
//
// Space Bunny Free: HUNK 1 IS REACHABLE, and the lever is the flag12 arm's
// `target = origin + d`. Written as three separate stores
// (`target.x = origin.x + d.x; target.y = origin.y + d.y; target.z = origin.z + d.z;`,
// the same spelling the len<0x1400000 tail already uses and which this file
// matches at 0x40856e) the 0x408334 `u->def` load lands AFTER the three
// `target = origin` stores and hunk 1 disappears: 97.8%, 1221 bytes, two hunks
// (hunk 2 and hunk 3 only). So the copy hoist is not source order and not an
// alias barrier, it is the scheduler's view of the whole taken branch.
// The cost is one register: with the three stores the flag12 arm's `ang` moves
// from ebx to ebp, so the second trig call is `push ebp` where the original has
// `push ebx`. Nothing tried here restores it (45 variants from that shape): the
// expanded `int ang` arm (68.8%), an out-param SetDirection (69.5%), a `Vec3
// dir` temp, the Direction() helper's x,y,z and y,x,z orders, a second copy of
// the helper (Direction2), `Direction(int, int)`, a helper without y=0 plus a
// caller `d.y = 0` (82.8%, the xor still lands after `neg eax`), `d = origin -
// u->pos` split, a `const` copy, an int temp for d.y, a `Vec3&` alias for d,
// `Length(origin - u->pos)` (89.6%), an int len local, reversed store orders
// (96.3%, 96.5%), `target.x += d.x` (68.4%), a store-order tie with a duplicate
// (96.5%), dropping the user-defined operator= (97.8%), `Vec3 t = origin + d;`
// then three stores from t (98.5% again, the temp is folded away), a foldable
// `if (d.y != d.y)`, `d.y = d.y;`, `target.x = target.x;`, both FixMul operand
// orders, and hoisting the flag12 test or the `kind` local. The 97.8% shape
// itself is the closest in bytes (about 20 wrong bytes against 37), so it is
// the one to build on if hunk 2's register ranking is ever solved; everything
// else in hunk 2 is unchanged by the three stores: our `xor ebp,ebp` still sits
// after the second call and `neg eax` still precedes `add esp,8`, so those two
// are one scheduling decision.
// HUNK 3 re-measured, all 1221 bytes and all 98.5% with the identical hunk:
// 16 spellings of the first FixMul (`FixMul(s,d.x)`, `FixMul(d.x,s)`,
// `(__int64)s * d.x`, `(__int64)d.x * s`, both casts to __int64, a __int64
// product local, an int temp, `(int)d.x`, `(int)s`, an `int&` parameter, a
// __int64 first parameter and a __int64 local for s). C1 canonicalises the
// multiply, so the only way left is to stop it being one _allmul call.
// HUNK 1 re-measured from the 98.5% file, all unchanged or worse: a `def`
// local before or after the copy, a dead store in a statically folded branch
// between the load and the copy (`int t = 0; if (t) def->field_152 = 0;`,
// 83.3%), `def = def ? def : def` (83.3%), `target = target ? target : target`
// (does not compile), a dead store before or after the copy, a `const Vec3`
// source copy, `if (def->flag12 != 0)` (77.6%), a foldable `def->field_152`
// test on the copy (77.7%, 86.1%), and hoisting `u->def` to the top of the
// loop body (93.8%). Dead statements MSVC deletes are also neutral here
// (`int t; t = 0;` before or after the copy, `if (t) origin.x = 0;`,
// `origin.y = origin.y;`, `target.z = target.z;`), so only the three-store
// spelling moves the load.
// Also neutral at 98.5% (they change the compiler state but not this
// function): one to three uncalled `static inline` helpers at file scope,
// `FUN_0040b6c30`'s return type as `short` (95.8%), a plain `struct Vec3Dir
// { int x, y, z; }` return type with no operator= behind Direction (the shape
// the matched 0x4103a0 uses, filled in field by field), a second copy of
// Direction with the x,y,z statement order, and a three-argument Vec3
// constructor added alongside the user-defined operator=. Returning
// `Vec3(-f1(a, s), 0, -f2(a, s))` from that constructor is 70.2% / 1231 bytes.
// NOT neutral, worth knowing: adding the *matched* 0x408090 (the preceding
// function, src/unsorted/0x408090.cpp, which only needs playerIndex at
// g_game+0x2a43 and visibilityMask at g_game+0x14273 added to Game_00408100)
// to this file flips MapRange's two loads, 98.0% with mapHeight first, so the
// file's function count really is load-bearing and the mapWidth-first order is
// a knife edge too.
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
