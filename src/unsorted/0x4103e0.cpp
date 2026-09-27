// Decompiled by GPT-6 Astra. Names are provisional.
#include <vector>
struct Unit;
namespace std {
template<> class vector<Unit*,allocator<Unit*> > {
    unsigned char alloc; Unit** first; Unit** last; Unit** endStorage;
public:
    ~vector();
    unsigned int size() const;
    unsigned int count() const { return begin()==0 ? 0 : end()-begin(); }
    bool empty() const { return size()==0; }
    Unit** begin() const { return first; }
    Unit** end() const { return last; }
    Unit*& operator[](unsigned int n) { return *(begin()+n); }
};
}
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
static inline Vec3 Offset(short angle, int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }

class Class_00410830 : public std::vector<Unit*> { public: Class_00410830(); };
void __stdcall FUN_0040b530(int, Vec3*, int, std::vector<Unit*>*);
Unit* __stdcall FUN_0043b700(Unit*);
// FUNCTION: 0x4103e0
int __stdcall FUN_004103e0(Unit* unit, Order* order, int flags)
{
    if (flags & 0x40) return 5;
    if (unit->terrain == g_game->water) {
        Vec3 center;
        center.x=(g_game->width/2)<<16;
        center.z=(g_game->height/2)<<16;
        short angle=FUN_0048a980(&unit->pos,&center);
        Vec3 pos=FUN_0040f790(unit->pos,Offset(angle,0x3200000));
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
            if (order->target) {
                if (FUN_0043b1f0(unit,order->target,0)) { order->flags=0; return 0; }
            } else {
                if (!order->pos.x && !order->pos.z && !order->pos.y) order->pos=unit->pos;
                order->angle=FUN_004b6c30(0x10000);
                order->parity=order->angle&1;
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
            }
            return 1;
        }
        break;
    case 1: {
        ((Class_00489800*)unit)->FUN_00489800(3);
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
        Unit* target=FUN_0043b700(unit);
        if (target && FUN_0043b1f0(unit,target,0)) return 5;
        if (flags&0xe0) order->angle+=-FUN_004b6c30(0x2000)-0x5555;
        Vec3 pos=FUN_0040f790(order->pos,Offset((short)order->angle,(unit->weapons[0].def->range+160)<<16));
        Class_0044e2d0* move=new Class_0044e2d0(order,pos);
        ((Class_0044e730*)move)->FUN_0044e730(128);
        ((Class_004388d0*)order)->FUN_004388d0((int)move);
        ((Class_00439e80*)order)->FUN_00439e80(FUN_004b6c30(30)+30);
        order->flags|=0xe0;
        return 2;
    }
    }
    return 7;
}
