// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// (base version by GPT-6 Astra; deepseek-v4.1 re-verified and extended the notes)
// deepseek-v4.1-flash, later session: 91.5% -> 92.2%, byte count now exact
//     (1051). The tail's sums are now `pos.x+=off.x; pos.z+=off.z;` on the
//     PosOf() result with NO second Vec3: the compound form lands each sum in
//     the order->pos register, giving the original `add ecx,ebx` / `add edx,eax`
//     (the old `sum.x=pos.x+off.x` form emitted `add ebx,ecx` / `add eax,edx`).
//     Price: pos moves from [esp+0x1c] to [esp+0x10] and one extra
//     `mov [esp+0x10],ecx` appears; that costs less than the old register order.
//     Also tried, all worse: nesting `if (unit->motion) { UnitDef* df=unit->def;`
//     fixes the case-0 load order but re-rotates case 0 (91.0%, 1046 bytes);
//     an explicit `units.~vector();` after the flags store emits the inlined
//     delete AND the scope-exit one (91.2%, 1056 bytes); `Vec3 sum=pos;` with
//     compound adds blows up the register allocation globally (88.8%).
// deepseek-v4.1-flash, issue-3778 pass: case-0 guard spellings tested against
//     the 92.2%/hunk-6 order: splitting the guard into two breaks
//     (`if (!unit->motion) break;` + `if (!(df->flags&0x800)) break;`, body
//     de-nested) regresses to 86.4% / 1050 bytes; the combined
//     `if (unit->motion==0 || (df->flags&0x800)==0) break;` form is
//     byte-identical to the `&&` form (same six hunks), so the guard spelling
//     cannot produce the original's `cmp dword ptr [esi],0` / deferred
//     `mov eax,[esi+0x92]`. File left at the best known 92.2%.
// Best known: 92.2%, 1051 bytes, six hunks left (the case-1 health-test
//     rotation, the FUN_0043f0e0 return-buffer slot, the tail's out-of-line
//     ~vector, the case-0 pos copy and the case-0 motion/def load order).
// Partial (previous best): 91.5%, 1047 bytes versus 1051. Up from 91.0%: case 0 now hoists
//     `UnitDef* df=unit->def;` into a braced `case 0:` and uses `df->flags`.
//     That alone rotates the whole case-0 block onto the original's registers
//     (`mov eax,[esi+0x92]` / `mov ecx,[eax+0x241]` / `test ch,8`, and the pos
//     copy `ecx`/`edx`/`eax`), which is +0.5 points for one byte. A named
//     `unsigned int df=unit->def->flags` does NOT do it (90.6%): it needs the
//     POINTER as the named local, not the value.
// Still differing, five hunks, all one global colouring or outgoing-arg slot:
//  1) case 1 health test: original has def in edx, health in ecx, bound in eax
//     and cmp ecx,eax; ours uses eax/edx/ecx and cmp edx,ecx (same length).
//     Same rotation as case 0, so the same lever probably applies, but a named
//     `df` pointer or named `bound`/`hp` locals in case 1 change nothing
//     (91.0% for each).
//  2) the Class_00438760 return buffer of FUN_0043f0e0 sits at [esp+0x64]
//     (the dead flags argument home) in the original and at [esp+0x68] (a slot
//     above the argument homes) here, so the later reload of kind is
//     [esp+0x6c]/[esp+0x70] too. This is a 1-byte displacement either way; it
//     costs the score only by breaking the instruction alignment.
//  3) the tail's two sums: original `add ecx, ebx` / `add edx, eax` (the sum
//     lands in the order->pos register); ours `add ebx, ecx` / `add eax, edx`.
//     NEW THIS SESSION: `sum.z+=off.z` DOES give the original's `add edx, eax`
//     (w1, x2: 87.0/89.6%) but `sum.x+=off.x` still emits `add ebx, ecx`, and
//     either one alone is a net loss because it moves the pads vector from
//     [esp+0x38] to [esp+0x48]. The three-statement `sum` temp of the base
//     version keeps every offset, so keep it.
//  4) the tail ends with an out-of-line ~vector() call where the original
//     inlines `operator delete(units.first)`: `mov edx,[esp+0x4c]; push edx;
//     call ??3@YAXPAX@Z; add esp,4` (also the source of the last byte, since
//     `or dl,0xf8` is 3 bytes and our `or al,0xf8` is 2).
//  5) case 0's pos copy and the `mov eax,[edi+6]` rotation in the tail: ours
//     uses eax/ecx/edx where the original uses ecx/edx/eax.
// Tried and rejected (all scored lower): giving ~vector() an inline body
//     `operator delete(first);` expands the destructor at ALL four destroy
//     sites (1063 bytes, 82.5%) and a null-guarded body is worse still
//     (1083 bytes, 71.8%); the original expands exactly one of the four, which
//     is not reachable from a visible destructor in this shape. Writing
//     `operator delete` in place of the base destructor means the pads sites
//     can no longer call 0x40c530, so the unit's destructor stays declared-only
//     here. Swapping the operands of each sum (`off.x+pos.x`) changes nothing at
//     all (MSVC 5 canonicalises `a+b`, item 20 of the brief). The inline
//     `order->pos + Offset(...)` with a member operator+ (78.7%), field-wise
//     reads of order->pos (76.8%), a named `dir` local in the water block
//     (79.2%), -FUN_004b70ef()/-FUN_004b7123() passed straight into a helper
//     (77.3%), dropping the redundant `state` local (91.0%, byte for byte the
//     same code) and reversing the health compare (90.8%) all lose. Routing
//     the case-0 field reads through the one-argument PosOf() helper is what
//     holds the parameter register roles, so leave PosOf/MovePos alone.
// deepseek-v4.1-flash, issue-3843 pass: no score change (92.2% / 1051 bytes held).
//     New negative results, so a later pass can skip them: passing the existing
//     MovePos() (unnamed Vec3 temp) straight to new Class_0044e2d0 gives 1053
//     bytes / 72.5%, the tail Vec3 moves to [esp+0x10] and the register roles
//     blow up; the positive combined guard
//     `if (unit->motion!=0 && (df->flags&0x800)!=0) { body; return 1; } break;`
//     compiles byte-identically to the `||`/break form (92.2%); deferring the df
//     assignment into the `||` operand (`df=unit->def,(df->flags&0x800)==0`)
//     reproduces the no-local case exactly (90.3% / 1050 bytes), so the
//     original's `cmp dword ptr [esi],0` plus deferred def load and the case-0
//     register rotation stay mutually exclusive from this source shape, and the
//     right order is always 1 byte short of 1051.
// Suspected original bug: none. The final delete[] takes [esp+0x4c], which is
//     the units vector's first pointer (inlineEmpty reads first at +4 and last
//     at +8 of the object at [esp+0x48]), so it is a correct inlined ~vector().
#include <vector>
struct Unit;
namespace std {
template<> class vector<Unit*,allocator<Unit*> > {
    unsigned char alloc; Unit** first; Unit** last; Unit** endStorage;
public:
    vector(const allocator<Unit*>& = allocator<Unit*>());
    ~vector();
    unsigned int size() const;
    unsigned int count() const { return begin()==0 ? 0 : end()-begin(); }
    bool empty() const { return size()==0; }
    bool inlineEmpty() const { return count()==0; }
    Unit** begin() const { return first; }
    Unit** end() const { return last; }
    Unit*& operator[](unsigned int n) { return *(begin()+n); }
};
}
struct Vec3 {
    int x, y, z;
    void operator+=(const Vec3& v) { x+=v.x; y+=v.y; z+=v.z; }
    Vec3 operator+(const Vec3& v) const { Vec3 r=*this; r+=v; return r; }
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
struct UnitDef { char pad0[0x1fa]; unsigned int maxHealth; char pad1fe[4]; short searchRange; char pad204[0x21c-0x204]; short altitude; char pad21e[0x231-0x21e]; unsigned int* weaponCategories[3]; unsigned int* categories; unsigned int flags; };
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
static inline Vec3 PosOf(Order* o) { Vec3 r; r.x=o->pos.x; r.z=o->pos.z; r.y=o->pos.y; return r; }
static inline Vec3 MovePos(Order* o, const Vec3& off) { Vec3 r=PosOf(o); r.x+=off.x; r.z+=off.z; return r; }
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
static inline Vec3 Offset(short angle, int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }

class Class_00410830 : public std::vector<Unit*> { public: Class_00410830(); };
void __stdcall FUN_0040b530(int, Vec3*, int, std::vector<Unit*>*);
Unit* __stdcall FUN_0043b700(Unit*);

class Class_00410c70 {
public:
    virtual void FUN_00410c70(Unit*);
    Owner* owner; std::vector<Unit*>* units; Unit* self;
    Class_00410c70(Owner* o, std::vector<Unit*>* u, Unit* s) : owner(o), units(u), self(s) {}
};
void __stdcall FUN_0047e890(Vec3*, int, const Class_00410c70&);
// FUNCTION: 0x410850
int __stdcall FUN_00410850(Unit* unit, Order* order, int flags)
{
    if (flags&0x40) return 5;
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
    unsigned int state=0; state=order->state;
    switch(state) {
    case 0: {
        UnitDef* df=unit->def;
        if (unit->motion==0 || (df->flags&0x800)==0) break;
        if (!order->pos.x && !order->pos.z && !order->pos.y) order->pos=unit->pos;
        order->angle=FUN_004b6c30(0x10000);
        order->parity=order->angle&1;
        return 1;
    }
    case 1: {
        if ((unsigned int)unit->health < (unit->def->maxHealth>>2)*3) {
            Class_00410830 pads;
            FUN_0040b530(unit->owner->index,&unit->pos,0xf00,&pads);
            if (!pads.empty()) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                Unit* pad=pads[FUN_004b6c30(pads.count())];
                FUN_0043acb0(unit,new Class_0043a1f0("VTOL_LANDING",pad,0,0,0,0));
                order->flags=0;
                return 0;
            }
        }
        std::vector<Unit*> units;
        int range=unit->def->searchRange<<16;
        FUN_0047e890(&unit->pos,range,Class_00410c70(unit->owner,&units,unit));
        if (!units.inlineEmpty()) {
            ((Class_004388d0*)order)->FUN_004388d0(0);
            Class_00438760 kind=FUN_0043f0e0(7,unit,units[0],0);
            FUN_0043acb0(unit,new Class_0043a1f0(kind,units[0],0,0,0,0));
            order->flags=0;
            return 3;
        }
        if (flags&0xe0) order->angle+=-FUN_004b6c30(0x2000)-0x4000;
        Vec3 off=Offset((short)order->angle,(unit->weapons[0].def->range+160)<<16);
        Vec3 pos=PosOf(order);
        pos.x+=off.x;
        pos.z+=off.z;
        Class_0044e2d0* move=new Class_0044e2d0(order,pos);
        ((Class_0044e730*)move)->FUN_0044e730(128);
        ((Class_004388d0*)order)->FUN_004388d0((int)move);
        ((Class_00439e80*)order)->FUN_00439e80(30);
        order->flags|=0xf8;
        return 2;
    }
    }
    return 7;
}
