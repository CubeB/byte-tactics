// Decompiled by GPT-6 Astra. Names are provisional.
#include <stdlib.h>
struct Point { short x, y; };
struct Vec3 {
    int x, y, z;
    Vec3 operator-(const Vec3& v) const { Vec3 r; r.x=x-v.x; r.y=y-v.y; r.z=z-v.z; return r; }
    Vec3 operator+(const Vec3& other) const {
        Vec3 r;
        r.x = x + other.x;
        r.y = y + other.y;
        r.z = z + other.z;
        return r;
    }
};
struct Unit;
class Class_0043d210 { public: char pad0[0x2e]; unsigned char flags; void SetFlightMode(Unit*,int); };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_0044e730 { public: void FUN_0044e730(int); };
class Class_0044e720 { public: void FUN_0044e720(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_0048b090 { public: void SetStateBits(int,int); };
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
class Class_00438ad0 { public: void FUN_00438ad0(Point, Point); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_004898b0 { public: void ClaimWeapons(int); };
class Class_004895c0 { public: void SetUnit(Unit*); };
#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x14a]; Point origin;
    char pad14e[0x15e-0x14e]; Vec3 min, max;
    char pad176[0x1fe-0x176]; unsigned short buildRate;
    char pad200[0x212-0x200]; unsigned short buildRange;
    char pad214[8]; short altitude; char pad21e[0x241-0x21e]; unsigned int flags,flags2;
};
struct Unit {
    Class_0043d210* motion; char pad4[0x66-4]; short angle;
    char pad68[2]; Vec3 pos;
    char pad76[8]; Point footprint;
    char pad82[4]; int busy; char pad8a[8]; UnitDef* def;
    char pad96[0xb0-0x96]; int timeout;
    char padb4[0xff-0xb4]; unsigned char player;
    char pad100[4]; float progress;
};
struct Order {
    char pad0[5]; unsigned char state;
    unsigned int flags;
    char padA[0x16-0xa]; Unit* target;
    char pad1A[8]; Vec3 pos;
    char pad2E[8]; int type;
    int unused; int retries;
};
struct Game {
    char pad0[0x1439b]; UnitDef* defs;
    char pad1439f[0x38a47-0x1439f]; unsigned int tick;
};
class Class_0044e2d0 { public: char data[0x36]; Class_0044e2d0(Order*,const Vec3&); };
#pragma pack(pop)
extern Game* g_game;
static inline UnitDef* Definitions() { return g_game->defs; }
union Fixed { int value; struct { unsigned short fraction; short whole; }; };
void __stdcall FUN_0047f780(Unit*, int, const char*);
void __stdcall FUN_0041c110(Unit*);
int __stdcall FUN_0047db70(UnitDef*, int, Point, int);
void __stdcall FUN_0047ddc0(UnitDef*, Vec3*);
Unit* __stdcall CreateUnit(unsigned char, short, Vec3, int, int, int);
void __stdcall FUN_0043adc0(Class_00438760, int, Unit*, Unit*, Vec3*, int, int);
int __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall FUN_00438590(Unit*, Order*, short);
int __stdcall FUN_00438700(Unit*, Order*, int);
int __stdcall FUN_0041ba60(Unit*, Unit*, float);
void __stdcall GetNanoPiecePosition(Unit*, Vec3*);
void __stdcall EmitNanoParticles(Vec3*, Vec3*, int);
static inline Point WorldToCell(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}
static inline void CellToWorld(Point origin, Point c, Vec3* v)
{
    v->x = (origin.x + c.x * 2) << 19;
    v->z = (origin.y + c.y * 2) << 19;
}

void __stdcall AttachUnitToPiece(Unit*,Unit*,char,char);
void __stdcall FUN_00414350(Point,Vec3*,Point);
int __cdecl FUN_004b70ef(short,int);
int __cdecl FUN_004b7123(short,int);
short __cdecl FUN_004b715a(int,int);
static inline Vec3 Offset(short angle,int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }
static inline short Angle(Vec3* a,Vec3* b) { return FUN_004b715a(a->x-b->x,a->z-b->z); }
// FUNCTION: 0x413d80
int __stdcall FUN_00413d80(Unit* unit,Order* order,int flags)
{
    if (flags&2) { FUN_0041c110(unit); return 5; }
    if (flags&8) { FUN_0047f780(unit,7,"Construction terminated"); FUN_0041c110(unit); return 8; }
    unsigned int state=0; state=order->state;
    switch(state) {
    case 0:
        if (unit->motion && (unit->def->flags&0x800)) {
            ((Class_00438880*)order)->FUN_00438880("Building");
            ((Class_004898b0*)unit)->ClaimWeapons(3);
            if (unit->busy) AttachUnitToPiece(unit,0,-1,2);
            ((Class_0048b090*)unit)->SetStateBits(1,1);
            if ((unit->motion->flags&3)==1) {
                unit->motion->SetFlightMode(unit,2);
                Class_0044e2d0* move=new Class_0044e2d0(order,unit->pos);
                ((Class_0044e6c0*)move)->FUN_0044e6c0(unit->def->altitude/2);
                ((Class_004388d0*)order)->FUN_004388d0((int)move);
                order->flags|=0xe0;
            }
            return 1;
        }
        break;
    case 1: {
        UnitDef* def=&g_game->defs[order->type];
        order->retries=0;
        Point origin=def->origin;
        Point cell=WorldToCell(order->pos,origin);
        FUN_00414350(cell,&order->pos,origin);
        Class_0044e2d0* move=new Class_0044e2d0(order,order->pos);
        ((Class_0044e730*)move)->FUN_0044e730(unit->def->buildRange);
        ((Class_004388d0*)order)->FUN_004388d0((int)move);
        order->flags=0xe0;
        return 1;
    }
    case 2: {
        if (flags&0x40) return 8;
        UnitDef* def=&g_game->defs[order->type];
        if (!FUN_0047db70(def,0,WorldToCell(order->pos,g_game->defs[order->type].origin),1)) {
            if (!order->retries) FUN_0047f780(unit,7,"Waiting for target area to clear");
            else if (order->retries>10) { FUN_0047f780(unit,7,"Target area was blocked"); return 8; }
            ++order->retries;
            ((Class_00439e80*)order)->FUN_00439e80(30);
            return 2;
        }
        FUN_0047ddc0(def,&order->pos);
        ((Class_004895c0*)((char*)order+0x12))->SetUnit(CreateUnit(unit->player,(short)order->type,order->pos,0,1,0));
        if (!order->target) { FUN_0047f780(unit,7,"Unable to create any more units"); return 8; }
        FUN_0047f780(unit,9,"Starting construction");
        FUN_0043adc0("GETBUILT",1,order->target,unit,0,0,0);
        FUN_00438590(unit,order,FUN_004b715a(unit->pos.x-order->target->pos.x,unit->pos.z-order->target->pos.z)-unit->angle);
        FUN_0041c110(unit);
        return 1;
    }
    case 3:
        FUN_00438700(unit,order,10);
    case 4: {
        if (g_game->tick%150==0) {
            int angle=GetHeadingBetween(&unit->pos,&order->target->pos);
            int range=unit->def->buildRange<<16;
            angle+=0xdb6e;
            Vec3 pos=order->target->pos-Offset(angle,range);
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e720*)move)->FUN_0044e720((unsigned short)angle);
            ((Class_004388d0*)order)->FUN_004388d0((int)move);
        }
        int rate=0; rate=unit->def->buildRate;
        if (FUN_0041ba60(unit,order->target,(float)(rate/30))) {
            Vec3 start;
            GetNanoPiecePosition(unit,&start);
            Vec3 bounds[2];
            bounds[0]=order->target->pos+order->target->def->min;
            bounds[1]=order->target->pos+order->target->def->max;
            EmitNanoParticles(&start,bounds,6);
        }
        if (order->target->progress!=0.0f) {
            ((Class_00439e80*)order)->FUN_00439e80(1);
            order->flags|=0xa;
            return 2;
        }
        return 1;
    }
    case 5:
        FUN_0047f780(unit,8,"Building complete");
        return 5;
    }
    return 7;
}
