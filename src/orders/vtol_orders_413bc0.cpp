// Decompiled by GPT-6 Astra. Names are provisional.
struct Vec3 {
    int x,y,z;
    Vec3 operator+(const Vec3& v) const { Vec3 r; r.x=x+v.x; r.y=y+v.y; r.z=z+v.z; return r; }
};
#pragma pack(push,1)
struct WeaponDef { char pad0[0xdc]; int range; };
struct UnitDef { char pad0[0x241]; unsigned int flags; };
struct Unit { void* motion; char pad4[12]; WeaponDef* weapon; char pad14[0x66-0x14]; short heading; char pad68[2]; Vec3 pos; char pad76[0x92-0x76]; UnitDef* def; };
struct Order { char pad0[5]; unsigned char state; unsigned int flags; char pada[12]; Unit* target; char pad1a[0x36-0x1a]; int side; };
class Class_0044e2d0 { public: char data[0x36]; Class_0044e2d0(Order*,const Vec3&); };
#pragma pack(pop)
class Class_0044e730 { public: void FUN_0044e730(int); };
class Class_004388d0 { public: void FUN_004388d0(int); };
int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short,int);
int __cdecl FUN_004b7123(short,int);
static inline Vec3 Offset(short angle,int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }
// FUNCTION: 0x413bc0
int __stdcall VtolEvadeOrder(Unit* unit,Order* order,int flags)
{
    int range=unit->weapon->range;
    if (order->target && !(flags&0x10008)) {
    unsigned int state=0; state=order->state;
    switch(state) {
    case 0:
        if (unit->motion && (unit->def->flags&0x800)) {
            order->side=RandomInt(2);
            Vec3 pos;
        if (order->side) pos=unit->pos+Offset(unit->heading-0x4000,range<<16);
        else pos=unit->pos+Offset(unit->heading+0x4000,range<<16);
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e730*)move)->FUN_0044e730(128);
            ((Class_004388d0*)order)->FUN_004388d0((int)move);
            order->flags=0x100e8;
            return 1;
        }
        goto invalid;
    case 1: {
        Vec3 pos;
        if (order->side) pos=unit->pos+Offset(unit->heading-0x4000,range<<17);
        else pos=unit->pos+Offset(unit->heading+0x4000,range<<17);
        Class_0044e2d0* move=new Class_0044e2d0(order,pos);
        ((Class_0044e730*)move)->FUN_0044e730(128);
        ((Class_004388d0*)order)->FUN_004388d0((int)move);
        order->flags=0x100e8;
        return 1;
    }
    case 2: break;
    default: invalid: return 7;
    }
    }
    return 5;
}
