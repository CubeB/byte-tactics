// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free. Names are provisional.
// (base version by GPT-6 Astra; deepseek-v4.1 re-verified and extended the notes)
// Partial: 90.3%, 1050 bytes versus 1051. Still differing:
//  1) case 1 health test: original has def in edx, health in ecx, bound in eax
//     and cmp ecx,eax; ours uses eax/edx/ecx and cmp edx,ecx (same length).
//  2) the Class_00438760 return buffer of FUN_0043f0e0 sits at [esp+0x64]
//     (unit's dead argument home) in the original and at [esp+0x68] (order's
//     home) here, so the later reload of kind is [esp+0x6c]/[esp+0x70] too.
//  3) the case 1 tail: the original's pos temp is at [esp+0x1c] and it stores
//     x, y, z after the push; ours lands it at [esp+0x10] (reusing the water
//     block's Direction temp slot) and stores y, x, z with a spill of x. The
//     final `or dl, 0xf8` also wants edx, and the last call is
//     `operator delete[]([esp+0x4c])` (the vector's LAST pointer) in the
//     original where we emit the out-of-line ~vector().
// Tried and rejected (all scored lower): the inline `order->pos + Offset(...)`
//     with a member operator+ (78.7%): it fixes region 3's three direct loads
//     but MSVC then gives ESI to `order` and EDI to `unit` instead of the
//     other way round, which costs far more. Field-wise reads of order->pos
//     written straight in the body do the same (76.8%), but routing them
//     through the one-argument PosOf() helper below keeps the parameter
//     register roles and scores 90.3%. Naming the offset `off` costs nothing
//     there; a named `dir` local in the water block (79.2%) and passing
//     -FUN_004b70ef()/-FUN_004b7123() straight into a helper (77.3%) both
//     lose the esi/edi roles. Region 2 alone did not move under any spelling
//     tried. A named `bound` local for the health compare is unchanged at
//     89.9%, as is reversing the compare to (maxHealth>>2)*3 > (unsigned)health.
//     "Sum into a temporary, then copy" for the tail (87.0%) loses the roles too.
// Tried after the 90.3% state: PosOf() inlined by hand at the case 1 tail,
//     field-wise pos.x=order->pos.x+off.x in statement order (78.2% and 78.7%:
//     both flip esi to order and edi to unit), an inline ~vector() body
//     `delete[] first` with the derived class dtor left implicit (89.1%) or
//     declared out-of-line (89.0%): the first fixes the tail's inlined
//     delete[] but inlines the three pads destroy sites too. 90.3% remains best.
// Tried in a later pass, all reverted: removing the named `range` local and
//     passing unit->def->searchRange<<16 straight to FUN_0047e890 drops to
//     87.3% (the slot that local occupies is needed), and dropping the named
//     `kind` local by inlining FUN_0043f0e0 into the Class_0043a1f0 argument
//     list drops to 81.8% (1050 -> 1047 bytes), so kind's [esp+0x64] slot is
//     load-bearing. Spelling the case 0 test as
//     `unit->motion!=0 && (unit->def->flags&0x800)!=0` and hoisting the health
//     cast into `unsigned int hp=(unsigned int)unit->health;` both leave the
//     output byte-for-byte identical at 90.3%. Reordering/renaming PosOf's
//     load order is not a lever either: hunk 4 is the front end's copy from
//     PosOf's temp into pos, which collapses into the same slot.
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
    case 0:
        if (unit->motion && (unit->def->flags&0x800)) {
            if (!order->pos.x && !order->pos.z && !order->pos.y) order->pos=unit->pos;
            order->angle=FUN_004b6c30(0x10000);
            order->parity=order->angle&1;
            return 1;
        }
        break;
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
