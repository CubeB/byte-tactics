// Decompiled by Claude Opus 5.5, finished by GPT-6, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Partial: 96.2% (1152 vs 1149 bytes), verified by a real check.py run. The only
// remaining diff is the shared build/help tail, and it is now down to register
// choice, not to size: casting the INTERMEDIATE pointer in the target argument,
// ((Order*)((char*)order->target+0x5c))->Target(), breaks MSVC 5's load
// elimination for the order->target->order chain and recovered 1.1 points.
// The original's 3-byte tail re-loads order->target->order twice
// (mov ecx,[edx+0x5c]; mov edx,[edx+0x5c]), which is what an un-merged pair of
// address expressions plus MSVC's "never fold a memory operand whose base is the
// destination" rule produces. Casting that intermediate is the only spelling
// found so far that stops the merge; the first copy of this file's notes, the
// shared build/help tail:
// the original at 0x406658 reloads order->target->order into two registers
// (mov ecx,[edx+0x5c]; mov edx,[edx+0x5c]; add ecx,0x22; push ecx; mov ecx,[edx+0x16])
// while ours CSEs it to one load and uses esi for the target; only the short jump
// targets after it differ, so matching this block alone should match the function.
// The identical tail appears in the matched 0x40fbe0 at 0x4100d6 (BuildOrder label).
// Tried and rejected (all scored below 95.1): other->Target()/Position() (92.3), raw
// ->target/&->pos (88.9), separate Order*/Unit*/Vec3* locals (71-79), the 0x40fbe0
// goto BuildOrder restructure (76.7; it drops the ebp zero), mixed forms between the
// two branches (87-90), pos-first local (73.3), declarations swapped (71.8).
// The N-unused-declarations sweep 0..400 only re-scores 93.8-95.1 and never changes
// the size, so this is source shape, not compiler state.
// Replacing the direct order expressions with a cached Order* drops the score; explicit follow-position fields also worsen register allocation.
// Second pass (deepseek-v4.1, round 2): the whole 3-byte gap is that one tail block.
// Original (27 bytes): mov edx,[ebx+0x16]; push ebp x3; mov ecx,[edx+0x5c];
// mov edx,[edx+0x5c]; add ecx,0x22; push ecx; mov ecx,[edx+0x16]; mov edx,[esp+0x20];
// push ecx; push edx; mov ecx,eax; call. Ours (24 bytes): mov ecx,[ebx+0x16];
// push ebp x3; mov edx,[ecx+0x5c]; mov ecx,eax; mov esi,[edx+0x16]; add edx,0x22;
// push edx; mov edx,[esp+0x20]; push esi; push edx; call.
// Scored with a scratch harness that calls check.py's compile_source/compare directly
// (base reproduction scores 95.14, equal to check.py): raw fields 88.95 (1145);
// accessor target + raw pos 91.98; raw target + accessor pos 92.11 (1145);
// other->Target()/other->Position() 92.35 (1135); other->target/&other->pos 89.18;
// casts (*(Unit**)((char*)o+0x16), (Vec3*)((char*)o+0x22)) 91.98/92.11;
// by-reference inline helpers TargetOf(Order*&)/PosOf(Order*&) (the 0x4077e0 trick)
// 91.98, inlining collapses them; Order*& oo=order->target->order local 85.68 (1218),
// at function scope 84.06; Unit* tg/oo/tt temps 56.9-89.4; a second inline method with
// an identical body 91.98; cross-body helpers TargetOfU(Unit*)/PosOfU(Unit*) 91.98.
// Every source form either CSEs the second [edx+0x5c] load away or regresses; VC5 keeps
// one load and reorders the two uses (target into esi before the pos add), the original
// emitted two loads and put the target in ecx. Against the note at line 13, this sweep
// points at compiler state (allocator/CSE), not at a missing source spelling.
// Note for the next pass: adding two unused static inline helpers to the file moved this
// function 95.14 -> 92.25 at the same 1149 bytes, so unused inline definitions are not
// inert here; re-check with a bare file before trusting a local comparison.
// Round 3 probes (all 1149 unless noted): Unit::order read once through a new inline
// member accessor and once plain 95.14 (merges); by-reference member binding, comma
// operators and (Vec3*) casts 95.14; the same tail as a single shared goto label 80.16
// (1116 bytes, it also merges the earlier ctor site); accessor + raw mixes 92.77.
// The double-load fingerprint (mov ecx,[edx+0x5c]; mov edx,[edx+0x5c]) occurs exactly
// twice in the whole exe: here and in the matched twin 0x40fbe0 at 0x4100ea. In the twin
// the unit pointer lives in ESI (push esi before FUN_0043acb0) so ESI is busy, while here
// unit is in EDI and ESI is free; an isolated repro of this tail (scratch iso9) also picks
// the free ESI and the cheap schedule, so the twin's extra live register is the best
// remaining explanation, but no source spelling tried here moves it: the cheap schedule
// survives, and VC5 even spills a callee-saved register to keep it (see scratch iso9).
// Also tried and rejected: ternaries around either argument (80.3/77.1, +12 bytes),
// (Unit*) and (Vec3*) casts 92.77, an inline BuildOrder helper 88.0/85.7 (1219), and a
// sweep of 0..40 dummy functions before this one: the score cycles 95.14/91.98/92.77 with
// the count but every state is still 1149 bytes, so the compiler state is reachable, it
// just never buys the second load.
// Round 4 (deepseek-v4.1-flash): ref-returning Position(), this->-qualified accessors, a
// Unit::GetOrder() wrapper, swapped accessor declaration order, out-of-class inline
// definitions, comma operators, an explicit temp for the new result, (Vec3*)&chain->pos
// and (Vec3*)&chain->pos and anonymous-union aliases for target/pos all land on 1145-1149 bytes (88.9-95.1),
// still one [reg+0x5c] load in the tail. Nothing in this round moved the 3 bytes.
// Round 5 (space-bunny-free): the fix is to make the two order->target->order address
// expressions STRUCTURALLY different, which is what stops MSVC 5 merging them. Casting
// the intermediate pointer in the TARGET argument only, (Order*)((char*)order->target+0x5c),
// scores 96.2 at 1149. Casting the intermediate in the POS argument instead
// ((Vec3*)((char*)order->target+0x5c)+0x22) does reach the original's 1152 bytes and
// 95.4, but it wrecks the Vec3-sum block at the follow label (the registers there
// rotate), so it is not the answer either. Casting in both arguments: 92.2 (1138)
// and 89.1 (1145). Two locals for the same pointer, or one local plus one re-derivation,
// are far worse (90.7 at 1213, 57.4 at 1239). Next pass: keep the cast in the target
// argument and look for what makes the pos argument's load land in ECX while the base
// stays in EDX, which is the last register difference left in the tail.
// Round 6 (space-bunny-free): still 96.2%, and the tail is the ONLY thing that
// differs, the twin confirms it is worth chasing. The matched twin 0x40fbe0 emits a
// byte identical tail (0x4100ea: the same [ebx+0x16], the same three pushes, the
// double [edx+0x5c], add 0x22, [edx+0x16], [esp+0x80]) and its MATCHED source
// reaches it through a shared `BuildOrder:` goto with a named `Class_0043a1f0* cmd`
// and an explicit `if (cmd)`. Porting that shape here does not work: with the
// `if (cmd)` the null fallback block stops sharing with the earlier ctor site's null
// path at 0x406514 and is duplicated (1186 bytes, 66.2%), and without it the shared
// null check disappears (1116 bytes, 80.2%). The two inline
// `FUN_0043acb0(unit, new ...)` sites we have let the compiler tail merge all of
// that correctly by itself, so the goto is the wrong tool here.
// A TRAP worth recording: `(((Order*)((char*)order->target))+0x5c)->Target()` scores
// 99.08% at exactly 1152 bytes, because the wrongly typed add needs a 4 byte
// displacement, but it emits `mov esi,[edx+0x193e]` where the original has
// `[edx+0x72]`. The comparison normalises memory operands to their base register, so
// a wrong constant looks like a near match. Do not chase that 99.08: sizeof(Order) is
// 0x46 here, not 0x5c, so that source is simply wrong.
// Rejected this round, none better than 96.19: both class accessors 95.1, raw
// `->target` with the accessor pos 92.1, `other->Target()` 91.0, `other->target`
// 91.0, `(Order*)((void*)T+0x5c)` (does not compile, void* has no size),
// `*(Order**)((char*)T+0x5c)->target` 92.1, `&((Order*)T->order)->pos` in place of
// the accessor 93.0, const and __inline qualified accessors 96.19 (no change at all),
// `(Unit*)`/`(Vec3*)` casts on the accessor returns 93.3, a 0..6 dummy inline function
// sweep 93.0 to 96.19 and never 1152, `else if` instead of a second `if` 93.0, a
// named `Unit*` local for the target 84.1, a named `Order*` local for the inner order
// 89.2, deleting the `other` local so `unit` can take ESI 93.0, a named `cmd` local
// 93.3, `cmd` plus an explicit `if (cmd)` 59.3, and assigning both arguments into
// by-value locals inside the argument list 77.6.
// What is still missing: a spelling that makes the LVP define `order->target->order`
// TWICE in the shared tail, so that the second `[edx+0x5c]` survives. Every spelling
// tried either folds it away to a single `[edx+0x72]` load, or costs 37 to 114 extra
// bytes by spilling a named local. The original's schedule, ECX for the first
// `->order` and EDX for the second, is exactly what a NON foldable second access
// allocates to naturally, since EDX (the target pointer) is dead by then.
#include <stdio.h>
struct Vec3 {
    int x, y, z;
    Vec3 operator+(const Vec3& v) const { Vec3 r; r.x=x+v.x; r.y=y+v.y; r.z=z+v.z; return r; }
};
struct Unit;
struct Order;
class Class_00438760 { public: unsigned char index; Class_00438760() {} Class_00438760(const char*); int operator==(const Class_00438760& v) const { return index==v.index; } };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438930 { public: void FUN_00438930(Vec3*, int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_00489800 { public: void FUN_00489800(int); };
#pragma pack(push, 1)
struct WeaponDef { char pad0[0x111]; unsigned int flags; };
struct Weapon { char pad0[8]; WeaponDef* def; char padc[11]; unsigned char flags; char pad18[4]; };
struct UnitDef { char pad0[0x1fa]; unsigned int maxHealth; char pad1fe[0x231-0x1fe]; unsigned int* weaponCategories[3]; unsigned int* categories; unsigned int flags; };
struct Owner { char pad0[0x108]; unsigned char allied[0x3e]; unsigned char index; };
struct Unit {
    char pad0[8]; Weapon weapons[3]; Order* order;
    char pad60[10]; Vec3 pos; char pad76[8]; short width; char pad80[6]; int busy;
    char pad8a[8]; UnitDef* def; Owner* owner; char pad9a[12]; unsigned short category;
    char pada8[0xf0-0xa8]; Unit* attacker; char padf4[0x108-0xf4]; short health;
    char pad10a[6]; unsigned int flags;
};
struct Order { Unit* Target() { return target; } Vec3* Position() { return &pos; } char pad0[4]; Class_00438760 kind; unsigned char state; unsigned int flags; char pada[12]; Unit* target; char pad1a[8]; Vec3 pos; char pad2e[8]; int radius; char pad3a[8]; unsigned int capabilities; };
class Class_0043a1f0 { public: char data[0x56]; Class_0043a1f0(Class_00438760, Unit*, Vec3*, int, int, int); };
#pragma pack(pop)
int __stdcall FUN_0043b1f0(Unit*, Unit*, int);
Unit* __stdcall FUN_0048a190(Unit*, int);
int __stdcall FUN_0049abb0(Unit*, Unit*, unsigned char);
void __stdcall FUN_0048a060(Unit*, Unit*, int);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char, Unit*, Unit*, int);
void __stdcall FUN_0043acb0(Unit*, Class_0043a1f0*);
int __stdcall FUN_004b6c30(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
static inline int Contains(unsigned int* bits, unsigned short index) { return bits[index >> 5] & (1 << (index & 31)); }
static inline Vec3 Offset(short angle, int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }
// FUNCTION: 0x406300
int __stdcall FUN_00406300(Unit* unit, Order* order, int flags)
{
    if (!order->target) return 5;
    if (unit->busy) return 7;
    if ((unsigned char)(order->target->def->flags >> 11) & 1) return 8;
    unsigned int state=0; state=order->state;
    switch(state) {
    case 0: {
        ((Class_00438880*)order)->FUN_00438880("Guarding");
        ((Class_00489800*)unit)->FUN_00489800(3);
        order->radius=(unit->width + order->target->width + 2) << 4;
        int distance=order->radius << 16;
        short angle=FUN_004b6c30(0x10000);
        order->pos=Offset(angle,distance);
        return 1;
    }
    case 1: {
        Unit* attacker=order->target->attacker;
        if (attacker && !attacker->owner->allied[unit->owner->index] && (flags & 0x10) &&
            !Contains(unit->def->categories,attacker->category)) {
            if (FUN_0043b1f0(unit,attacker,1)) { order->flags=0; return 3; }
            if (unit->flags & 0x300000) {
                for (unsigned char i=0;i<3;++i) {
                    Weapon* weapon=&unit->weapons[i];
                    if ((weapon->flags&2) && (weapon->flags&0x10) && !((unsigned char)(weapon->def->flags >> 26)&1)) {
                        Unit* target=FUN_0048a190(unit,i);
                        if (!target || !FUN_0049abb0(unit,target,i) || Contains(unit->def->weaponCategories[i],target->category))
                            FUN_0048a060(unit,attacker,i);
                    }
                }
            }
        }
        if ((unsigned int)order->target->health < order->target->def->maxHealth && (unit->def->flags&0x40)) {
            Class_00438760 kind=FUN_0043f0e0(8,unit,order->target,0);
            if(kind.index) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                FUN_0043acb0(unit,new Class_0043a1f0(kind,order->target,0,0,0,0));
                order->flags=0; return 3;
            }
        }
        if (order->target->order && order->target->order->kind.index &&
            (unit->def->flags&0x40) && (order->target->def->flags&0x40) &&
            (order->target->order->capabilities&0x100000) && unit!=order->target->order->Target()) {
            int building=order->target->order->kind=="MobileBuild" ||
                         order->target->order->kind=="BuildingBuild";
            Order* other=order->target->order;
            int actionable=((other->capabilities&0x200) && other->target) || (other->capabilities&0x400);
            Class_00438760 kind;
            if (!building && actionable) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                kind=order->target->order->kind;
                FUN_0043acb0(unit,new Class_0043a1f0(kind,((Order*)((char*)order->target+0x5c))->Target(),order->target->order->Position(),0,0,0));
                order->flags=0; return 3;
            }
            if (building && other->target) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                kind=Class_00438760("HelpBuild");
                FUN_0043acb0(unit,new Class_0043a1f0(kind,((Order*)((char*)order->target+0x5c))->Target(),order->target->order->Position(),0,0,0));
                order->flags=0; return 3;
            }
        }
follow:
        Vec3 pos=order->target->pos+order->pos;
        ((Class_00438930*)order)->FUN_00438930(&pos,order->radius/2);
        ((Class_00439e80*)order)->FUN_00439e80(30);
        order->flags|=0x18;
        return 2;
    }
    }
    return 7;
}
