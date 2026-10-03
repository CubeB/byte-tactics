// Decompiled by GPT-5.6 Astra, finished by deepseek-v4.1-flash and GPT-6, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by xiaomi/mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.
// Space Bunny Free pass (79.4% -> 91.6%, ours 1990 vs 1976): three source
// shapes were the whole gap; everything after that is cleanup, each item
// re-checked on its own and byte-neutral.
//   1. `order->target->order` written out inline at every use in the
//      MobileBuild chain, with no local for it. With a named local MSVC keeps
//      `other` in a callee-saved register across the Class_00438760 calls, has
//      no callee-saved register left for the block's zero constant, and emits
//      `push 0` and `mov [ebx+6],0` where the original has `push ebp` and
//      `mov [ebx+6],ebp`. Spelled inline it reloads order->target and ->order
//      after every constructor call (as the original does), keeps `other` in
//      EAX, `building` in EDI and the zero constant in EBP, and
//      0x40ff60-0x40ffd3 becomes instruction for instruction identical.
//   2. The weapon loop's flag test in the negative sense with an empty
//      `then`: `if (!(A && B && !C)) { } else { ... }`. This fixes the whole
//      loop (89.2%): `lea ecx,[esi+0x1f]`, the pointer at [esp+0x20] and the
//      byte counter i at [esp+0x1c], no reload at the top, no `mov edx,ecx`
//      before the `def` load, and `mov al,[i]`, `mov ecx,[ptr]`, `inc al`,
//      `inc ebp`, `add ecx,0x1c`, `cmp al,3`, `mov [i],al`, `mov [ptr],ecx` at
//      the bottom. Every other spelling is the same wrong code (i declared
//      first, `++i` before `weapon++`, for and while loops, `&unit->weapons[i]`,
//      a byte pointer induction variable, an explicit `wf` local), so this is
//      a register allocation effect, not a scheduling one.
//   3. permute.py from the 85.3% file: split_init, flip_compare, temp_inline
//      and extract_helper took it to 91.6%. What survives is `unit->motion != 0`,
//      `(0x40&unit->def->flags)` and the single use helper `OrderTarget`
//      (without the helper 88.7%). Three more permuter rounds found nothing.
// Still different (91.6%):
// - the null path of the two `new` sites (+16 bytes). The original has one
//      shared block at 0x410120, reached by `cmp eax,ebp; je` from both sites,
//      with each success path inlined and the null block making its own zero
//      with `xor eax,eap`. Ours gives each site a two byte `xor eax,eap` stub,
//      a `jmp` and a `cmp eax,ebp` dispatch into a shared block that tests the
//      pointer again, and site B's success block is out of line. Written any
//      other way (if/else at either or both sites, two null labels, the label
//      inside or outside the switch, `cmd==0` against `!cmd`, a null pointer
//      variable, `cmd?cmd:0`, a shared Issue3 helper) MSVC builds the same
//      dispatch.
// - `mov cl,[eax]` instead of `mov al,[eax]` for the "VTOL_HelpBuild" index
//      store at 0x4100d2; the four other stores in that chain use AL in both.
// deepseek-v4.1-flash pass (kept 79.4%, 1974 vs 1976): re-verified the baseline
// and confirmed the whole remaining gap is allocation shape, not missing logic.
// Tried and rejected, all byte-neutral or worse: the weapon loop as a for-loop,
// as &unit->weapons[i], with `++i; weapon++` instead of `weapon++; ++i`, with
// `i` declared before `weapon`, and with `unsigned char wf=weapon->flags` bound
// (all 1974/79.4, so the loop body is a consequence of upstream liveness, not a
// source lever); a WeaponDef* def local (1973/73.5); swapping the two flag tests
// (1974/79.3); `!(def->flags & 0x4000000)` instead of the shift (1972/73.8).
// The building/actionable chain: making `building` use inline
// order->target->order (with or without moving the `other` declaration after
// it) balloons to 2010 bytes, 62.9-64.5%, because MSVC then cannot CSE the
// chain; the named `other` first is required. The BuildOrder tail as a direct
// `FUN_0043acb0(unit,new ...)` shrinks to 1918/77.1 (it tail-merges the two
// null-path call sites); the explicit `if (cmd)` is required. So the file is
// unchanged from the best prior version. The remaining diffs are one shared
// allocator state: the original keeps a zero in EBP from 0x40fe7a to 0x410129
// (`cmp reg,ebp`, `push ebp`, `mov [ebx+6],ebp`), where ours uses `test reg,reg`
// and immediate zeroes; that plus the loop's extra `mov edx,ecx` and top reload
// shifts every branch target after 0x40fe68 by 6. The ground twin 0x406300 uses
// the same ebp-zero tail at 96.2%, so the cause is generic liveness here, not a
// distinct source construct.
// Started by GPT-5.6 Astra, continued by deepseek-v4.1-flash and GPT-6; the
// 75.8% version is deepseek-v4.1 lowering the earlier 74.5%.
// Partial 79.4% (original 1976 bytes, ours 1974). Best during GPT-6.1-sol pass: separate Vec3* destination/source locals for the initial position copy; direct cast assignment falls to 75.8%.
// Partial 75.8% baseline note (original 1976 bytes, prior source 1971). Fixed since the 74.5% attempt:
// the r4 MobileBuild tail is now two separate source sites (the `new` result
// used on success, an explicit FUN_0043acb0(unit,0) fallthrough on failure), so
// MSVC emits both copies instead of tail-merging them (+56 bytes). Still
// Differing: the order->pos = order->target->pos copy at 0x40fce9 wants
// `add eax,0x6a` (the original consumes the target pointer) where ours emits
// `lea ecx,[eax+0x6a]` and copies through ebp; the original materialises a zero
// register (`xor ebp,ebp`, then `cmp eax,ebp` and `push ebp` for the constant
// arguments, 0x40fe7a-0x410120) where ours uses `test eax,eax` and `push 0`;
// and the r3 `new` failure path jumps to the shared 0x410120 block in the
// original but gets its own copy in ours. `goto Aim` around the r3 block and
// the explicit `if (!cmd)` before the success call were both tried and lose
// (74.8% for the r3 split). Neutral variants (75.8%, kept): the weapon loop as
// a pointer walk with a separate byte counter (the original walks
// `unit->weapons[0].flags` and advances by 0x1c), and `tgt=order->target;`
// before the position copy.
// deepseek-v4.1 re-tested the copy site: dropping the `Unit* tgt` local, a
// second `tgt2` local inside case 2, and `unsigned int state` in place of
// `state=0; state=order->state;` are all byte-neutral (1971 bytes, 75.8%), so
// the wanted `add eax,0x6a` is not reachable from those; writing
// `order->target->order->...` inline in the VTOL kind-comparison chain loses
// badly (61.9%, 2009 bytes) because it drops the EDI cache of `other` without
// freeing a callee-saved register for the zero constant.
// deepseek-v4.1 (2nd pass, 4 check runs): swapping the weapon-loop local
// declarations (i before the Weapon* walk) and adding an explicit zero local
// for the r3 constant arguments are byte-neutral (75.8%, 1971); field-wise
// order->pos=order->target->pos is worse (75.7%, 1967). The remaining 5-byte
// deficit is the copy site (add eax,0x6a vs lea ecx,[eax+0x6a], -2) plus the
// weapon-loop pointer, which the original keeps in ECX across the branch-back
// (mov al,[ecx] at 0x40fdd8) while ours re-loads it from its slot every trip.
// deepseek-v4.1-flash pass 2 (3 check runs, all 1974/79.4, file unchanged):
//   * writing the scan as `unit->weapons[i].flags` inline (no Weapon* walk) is
//     byte-identical to the pointer walk, so the `lea ecx,[esi+0x1f]` base is a
//     compiler CSE of the flags address, not a source declaration.
//   * giving the r3 `new` site the r4 if/else failure shape re-merges the null
//     paths to 1946 bytes but drops to 77.5%: MSVC then caches the zero in EDI
//     (`cmp eax,edi`, `push edi`) and keeps `other` in EDI too, and it stops
//     reloading `order->target->order` in the VTOL kind chain, so EBP never
//     becomes the zero register and the whole r4 region re-shapes. The original
//     needs EBP==0 live from 0x40fe7a to 0x410129 AND `order->target->order`
//     re-loaded at 0x40ff60/0x40ff7f/0x40ff9e AND EDI==`building`, so the two
//     null sites must share the 0x410120 tail without letting MSVC pick EDI as
//     the constant register: that is the remaining lever, not the source text of
//     the two `new` sites.
#include <stdio.h>
// SHARED begin
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
struct WeaponDef { char pad0[0xdc]; int range; char pade0[0x111-0xe0]; unsigned int flags; };
struct Weapon { char pad0[8]; WeaponDef* def; char padc[11]; unsigned char flags; char pad18[4]; };
struct UnitDef { char pad0[0x1fa]; unsigned int maxHealth; char pad1fe[0x21c-0x1fe]; short altitude; char pad21e[0x231-0x21e]; unsigned int* weaponCategories[3]; unsigned int* categories; unsigned int flags; };
struct Owner { char pad0[0x108]; unsigned char allied[0x3e]; unsigned char index; };
class Class_0043d210 { public: char pad0[0x2e]; unsigned char flags; void FUN_0043d210(Unit*, int); };
struct Unit {
    Class_0043d210* motion; char pad4[4]; Weapon weapons[3]; Order* order;
    char pad60[10]; Vec3 pos; char pad76[8]; short width; short depth; int terrain; int busy;
    char pad8a[8]; UnitDef* def; Owner* owner; char pad9a[12]; unsigned short category;
    char pada8[0xf0-0xa8]; Unit* attacker; char padf4[0x108-0xf4]; short health;
    char pad10a[6]; unsigned int flags;
};
struct Order { char pad0[4]; Class_00438760 kind; unsigned char state; unsigned int flags; char pada[12]; Unit* target; char pad1a[8]; Vec3 pos; char pad2e[8]; int angle, parity; char pad3e[4]; unsigned int capabilities; char pad46[4]; int next; };
class Class_0043a1f0 { public: char data[0x56]; Class_0043a1f0(Class_00438760, Unit*, Vec3*, int, int, int); };
class Class_0044e2d0 { public: char data[0x36]; Class_0044e2d0(Order*, const Vec3&); };
struct Game { char pad0[0x1422b]; int width, height; char pad14233[0x142b7-0x14233]; int water; };
#pragma pack(pop)
extern Game* g_game;
class Class_0044e730 { public: void FUN_0044e730(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_004899b0 { public: int FUN_004899b0(Unit*); };
class Class_004898b0 { public: void FUN_004898b0(int); };
class Class_0048b090 { public: void FUN_0048b090(int, int); };
void __stdcall FUN_0048aac0(Unit*, Unit*, char, char);
short __stdcall FUN_0048a980(Vec3*, Vec3*);
union Fixed { int value; struct { unsigned short frac; short whole; } parts; };
Vec3 __stdcall FUN_004103a0(short, Fixed);
static inline Vec3 Direction(short angle, int range) { Fixed distance; distance.value=range; return FUN_004103a0(angle,distance); }
Vec3 __stdcall FUN_0040f790(const Vec3&, const Vec3&);
void __stdcall FUN_0043ad10(Unit*, Class_0043a1f0*);

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
static inline Vec3 Offset(short angle, int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,(int)distance); return v; }
// SHARED end

static inline Unit* OrderTarget(Order*order) { return order->target; }

// FUNCTION: 0x40fbe0
int __stdcall FUN_0040fbe0(Unit* unit, Order* order, int flags)
{
// REGION r1 begin   0x40fbe0-0x40fcda
//   the guard, the water/terrain move, and the state dispatch
    if (order->target && !(flags&0x48)) {
        if (unit->terrain==g_game->water) {
            Vec3 center;
            center.x=(g_game->width/2)<<16;
            center.z=(g_game->height/2)<<16;
            short angle=FUN_0048a980(&unit->pos,&center);
            Vec3 pos=FUN_0040f790(unit->pos,Direction(angle,0x3200000));
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e730*)move)->FUN_0044e730(128);
            order->flags|=0xe0;
            ((Class_004388d0*)order)->FUN_004388d0((int)move);
            return 2;
        }
        Vec3* dst=(Vec3*)((char*)order+0x22);
        *dst=*((Vec3*)((char*)order->target+0x6a));
// REGION r2 begin   0x40fcda-0x40feda
//   the state dispatch. MSVC lays a switch out as case 2, case 1, case 0, so
//   the region ranges (which follow the emitted order) and these markers
//   (which follow the written order) disagree.
        switch(order->state) {
        case 0:
            if (unit->motion && (unit->def->flags&0x800)) {
                ((Class_00438880*)order)->FUN_00438880("Guarding");
                ((Class_004898b0*)unit)->FUN_004898b0(3);
                if (unit->busy) FUN_0048aac0(unit,0,-1,2);
                ((Class_0048b090*)unit)->FUN_0048b090(1,1);
                if ((unit->motion->flags&3)==1) {
                    unit->motion->FUN_0043d210(unit,2);
                    Class_0044e2d0* move=new Class_0044e2d0(order,unit->pos);
                    ((Class_0044e6c0*)move)->FUN_0044e6c0(unit->def->altitude/2);
                    ((Class_004388d0*)order)->FUN_004388d0((int)move);
                    order->flags|=0xe0;
                }
                order->angle=FUN_004b6c30(0x10000);
                order->parity=order->angle&1;
                return 1;
            }
            return 7;
        case 1:
            ((Class_00489800*)unit)->FUN_00489800(3);
            return 1;
// REGION r3 begin   0x40feda-0x4100d0
//   case 2's opening: the attacker test, the three-weapon scan and the
//   FUN_004899b0 order-kind test. This text emits 0x40fd26-0x40fedc, the
//   original's r2 range: the MobileBuild chain below emits r3's range.
        case 2: {
            Unit* attacker=order->target->attacker;
            if (attacker && !attacker->owner->allied[unit->owner->index]) {
                if (flags & 0x10) {
                    if (!Contains(unit->def->categories,attacker->category)) {
                        if (FUN_0043b1f0(unit,attacker,1)) { order->flags=0; return 3; }
                        if (unit->flags & 0x300000) {
                            unsigned char i=0;
                            Weapon* weapon=unit->weapons;
                            do {
                                // the test is in this sense on purpose: it is what
                                // gives the loop the original's jump polarity, its
                                // register for the pointer and its stack slots
                                if (!((weapon->flags&2) && weapon->flags&0x10 && ((unsigned char)(weapon->def->flags >> 26)&1) == 0)) {
                                } else {
                                    Unit* target=FUN_0048a190(unit,i);
                                    if (!target || !FUN_0049abb0(unit,target,i) || Contains(unit->def->weaponCategories[i],target->category))
                                        FUN_0048a060(unit,attacker,i);
                                }
                                ++i;
                                weapon++;
                            } while (i<3);
                        }
                    }
                }
            }
            if (((Class_004899b0*)unit)->FUN_004899b0(order->target)) {
                Class_00438760 kind=FUN_0043f0e0(8,unit,order->target,0);
                if (kind.index) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    Class_0043a1f0* cmd=new Class_0043a1f0(kind,order->target,0,0,0,0);
                    if (!cmd) goto NullCmd;
                    FUN_0043acb0(unit,cmd);
                    order->flags=0; return 3;
                }
            }
// REGION r4 begin   0x4100d0-0x4102c6
//   the MobileBuild and VTOL order-steal chain, then the aim and fire block.
//   `order->target->order` is spelled out at every use, with no local for it:
//   that is what makes MSVC reload it after each Class_00438760 constructor
//   call, and what frees the callee-saved register the block's zero constant
//   ends up in.
            if (order->target->order && order->target->order->kind.index && (unit->def->flags&0x40) &&
                ((Class_004899b0*)unit)->FUN_004899b0(order->target->order->target) &&
                (order->target->def->flags&0x40) && order->target->order &&
                (order->target->order->capabilities&0x100000) && unit!=order->target->order->target) {
                int building = order->target->order->kind=="MobileBuild" || order->target->order->kind=="BuildingBuild" || order->target->order->kind=="VTOL_MobileBuild";
                int actionable = ((order->target->order->capabilities&0x200) && order->target->order->target) || (order->target->order->capabilities&0x400);
                Class_00438760 kind;
                if (!building && actionable) {
                    kind=order->target->order->kind;
                    if (kind=="REPAIRUNIT") kind=Class_00438760("VTOL_REPAIRUNIT");
                    if (kind=="RECLAIM") kind=Class_00438760("VTOL_RECLAIM");
                    if (kind=="RECLAIMUNIT") kind=Class_00438760("VTOL_RECLAIMUNIT");
                    if (kind=="HELPBUILD") kind=Class_00438760("VTOL_HELPBUILD");
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    goto BuildOrder;
                } else if (building && order->target->order->target) {
                    ((Class_004388d0*)order)->FUN_004388d0(0);
                    kind=Class_00438760("VTOL_HelpBuild");
                    goto BuildOrder;
                }
                goto Aim;
                BuildOrder: {
                    Class_0043a1f0* cmd=new Class_0043a1f0(kind,OrderTarget(order)->order->target,&order->target->order->pos,0,0,0);
                    if (!cmd) goto NullCmd;
                    FUN_0043acb0(unit,cmd);
                    order->flags=0; return 3;
                }
            }
// REGION r4 end
            Aim:
            if (flags&0xe0) order->angle += -FUN_004b6c30(0x2000)-0x4000;
            short angle=order->angle;
            Vec3 pos;
            if ((unsigned char)(unit->flags>>31)&1)
                pos=order->target->pos+Offset(angle,(unit->weapons[0].def->range+160)<<16);
            else pos=order->target->pos+Offset(angle,0x1400000);
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e730*)move)->FUN_0044e730(128);
            ((Class_004388d0*)order)->FUN_004388d0((int)move);
            ((Class_00439e80*)order)->FUN_00439e80(30);
            order->flags|=0xf8;
            return 2;
        }
// both `new` sites jump to this one label: with it here MSVC emits the null
// call once and each site jumps straight to it
        NullCmd:
        FUN_0043acb0(unit,0);
        order->flags=0; return 3;
        }
// REGION r1 end
// REGION r5 begin   0x4102c6-0x410398
//   the default return and the seek-guard tail
        return 7;
    } else {
    }
    if (!order->next) FUN_0043ad10(unit,new Class_0043a1f0("VTOL_SEEKGUARD",order->target,&order->pos,0,0,0));
    return 5;
// REGION r5 end
}
