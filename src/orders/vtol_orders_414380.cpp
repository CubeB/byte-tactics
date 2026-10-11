// Decompiled by GPT-6 Astra, finished by space-bunny-free, finished by deepseek-v4.1-flash,
// finished by GPT-6.1-sol, finished by Claude Opus 5.5. Names are provisional.
// Kept: the operand order of the six bounds adds follows symbol ids, so this
// header set matters.
#include <memory.h>
#include <windows.h>
struct Point { short x, y; };
// Kept local, not util/vec3.h: the header changes this function's code.
struct Vec3 {
    int x, y, z;
    void operator-=(const Vec3& v) { x-=v.x; y-=v.y; z-=v.z; }
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
class UnitMotion { public: char pad0[0x2e]; unsigned char flags; void SetFlightMode(Unit*,int); };

class Class_0044e730 { public: void SetApproachRadius(int); };
class Class_0044e720 { public: void SetHeading(int); };
class Class_0044e6c0 { public: void SetAltitude(int); };
class MissionType { public: unsigned char index; MissionType(const char*); };

class PathOrderAttach { public: void SetUnit(Unit*); };
#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x14a]; Point origin;
    char pad14e[8]; int canBuild; char pad15a[4]; Vec3 min, max;
    char pad176[0x1fe-0x176]; unsigned short buildRate;
    char pad200[0x212-0x200]; unsigned short buildRange;
    char pad214[8]; short altitude; char pad21e[0x241-0x21e]; unsigned int flags,flags2;
};
struct Unit {
    UnitMotion* motion; char pad4[0x66-4]; short heading;
    char pad68[2]; Vec3 pos;
    char pad76[8]; Point footprint;
    char pad82[4]; int carrier; char pad8a[8]; UnitDef* def;
    char pad96[0xb0-0x96]; int workTime;
    char padb4[0xff-0xb4]; unsigned char playerIndex;
    char pad100[4]; float buildLeft;
    void SetStateBits(int,int);
    void ClaimWeapons(int);
};
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall IsUnitCommander(Unit*);
int __stdcall FindLandingPad(Unit*, int);
unsigned short __stdcall ChooseBuildOption(unsigned int, Unit*);
void __stdcall ClearWeaponTarget(Unit*, int);
void __stdcall DetonateUnitWeapon(Unit*, int);

// Keeps its own view: the shared header's extra declarations move this
// function's bounds-add register allocation.
struct UnitRef {
    void* table;
    Unit* ptr;
    // The empty do-while is a debug check that compiles to nothing.
    Unit* Get() { do {} while (0); return ptr; }
};
struct Order {
    char pad0[5]; unsigned char state;
    unsigned int flags;
    char padA[0x12-0xa]; UnitRef target;
    char pad1A[8]; Vec3 pos;
    char pad2E[8]; int type;
    int unused; int retries;
    void AnnounceStatusIfFlagged(const char*);
    void SetAttachedFx(int);
    void AttachBuildFootprintMarker(Point, Point);
    void SetDeadlineTicks(int);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void ReattachFxToUnit();
    void MergeFlagsFromTable(int k);
    void AttachRingApproachGoal(Vec3* pos, int radius1, int radius2);
    ~Order();
    Order(Unit* unit, void* file, char* name);
    int SerializeToSave(Unit* unit, void* file, char* name);
    void OrStatusFlags(unsigned int flags);
    void AttachApproachRadiusGoal(Vec3* pos, int radius);
    Unit* Target();
    void Wait();
    Vec3* Position();
    int Advance(int distance);
};
struct Game {
    char pad0[0x1439b]; UnitDef* unitDefs;
    char pad1439f[0x38a47-0x1439f]; unsigned int tick;
};
class Class_0044e2d0 { public: char data[0x36]; Class_0044e2d0(Order*,const Vec3&); };
#pragma pack(pop)
extern Game* g_game;
static inline UnitDef* Definitions() { return g_game->unitDefs; }
union Fixed { int value; struct { unsigned short fraction; short whole; }; };
void __stdcall QueueUnitSpeech(Unit*, int, const char*);
void __stdcall MarkSelectionOrdersDirty(Unit*);
int __stdcall CanPlaceUnitFootprint(UnitDef*, int, Point, int);
void __stdcall SnapWorldPosToFootprint(UnitDef*, Vec3*);
Unit* __stdcall CreateUnit(unsigned char, short, Vec3, int, int, int);
void __stdcall AddOrder(MissionType, int, Unit*, Unit*, Vec3*, int, int);
int __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall StartBuildingScript(Unit*, Order*, short);
int __stdcall WaitIfNotInBuildStance(Unit*, Order*, int);
int __stdcall AddBuildProgress(Unit*, Unit*, float);
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
void __stdcall CellToWorldPos(Point,Vec3*,Point);
int __cdecl FUN_004b70ef(short,int);
int __cdecl FUN_004b7123(short,int);
short __cdecl FUN_004b715a(int,int);
static inline Vec3 Offset(short angle,int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }
static inline short Angle(Vec3* a,Vec3* b) { return FUN_004b715a(a->x-b->x,a->z-b->z); }
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int CheckDirectXVersion(int, int, int, int, int);
struct Feature;
// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x414380
int __stdcall VtolHelpBuildOrder(Unit* unit,Order* order,int flags)
{
    if (order->target.Get() && !(flags&8)) {
        if (flags&2) { MarkSelectionOrdersDirty(unit); return 5; }
        unsigned int state=0; state=order->state;
        switch(state) {
        case 0:
        // Two nested ifs, not one &&: part of what places the bounds adds.
        if (unit->motion) {
            if (unit->def->flags&0x800) {
                if (!unit->def->canBuild) return 7;
                order->AnnounceStatusIfFlagged("Building");
                unit->ClaimWeapons(3);
                if (unit->carrier) AttachUnitToPiece(unit,0,-1,2);
                unit->SetStateBits(1,1);
                if ((unit->motion->flags&3)==1) {
                    unit->motion->SetFlightMode(unit,2);
                    Class_0044e2d0* move=new Class_0044e2d0(order,unit->pos);
                    ((Class_0044e6c0*)move)->SetAltitude(unit->def->altitude/2);
                    order->SetAttachedFx((int)move);
                    order->flags|=0xe0;
                }
                return 1;
            }
        }
        break;
    case 1: {
        order->retries=0;
        Class_0044e2d0* move=new Class_0044e2d0(order,order->target.Get()->pos);
        ((Class_0044e730*)move)->SetApproachRadius(unit->def->buildRange);
        order->SetAttachedFx((int)move);
        order->flags=0xe0;
        return 1;
    }
    case 2:
        if (flags&0x40) return 8;
        if (order->target.Get()->buildLeft==0.0f) return 5;
        StartBuildingScript(unit,order,(short)GetHeadingBetween(&unit->pos,&order->target.Get()->pos));
        MarkSelectionOrdersDirty(unit);
        return 1;
    case 3: {
        if (g_game->tick%150==0) {
            int angle=GetHeadingBetween(&unit->pos,&order->target.Get()->pos);
            int range=unit->def->buildRange<<16;
            angle+=0xdb6e;
            Vec3 pos=order->target.Get()->pos-Offset(angle,range);
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e720*)move)->SetHeading((unsigned short)angle);
            order->SetAttachedFx((int)move);
        }
        int rate=0; rate=unit->def->buildRate;
        // The ok local is needed: it adds a register candidate that keeps order in esi.
        int ok=AddBuildProgress(unit,order->target.Get(),(float)(rate/30));
        if (ok) {
            Vec3 start;
            GetNanoPiecePosition(unit,&start);
            Vec3 bounds[2];
            bounds[0]=order->target.Get()->pos+order->target.Get()->def->min;
            bounds[1]=order->target.Get()->pos+order->target.Get()->def->max;
            EmitNanoParticles(&start,bounds,6);
        }
        if (order->target.Get()->buildLeft!=0.0f) {
            order->SetDeadlineTicks(1);
            order->flags|=0xa;
            return 2;
        }
        return 5;
    }
    }
        return 7;
    }
    QueueUnitSpeech(unit,7,"Construction terminated by hostile action");
    MarkSelectionOrdersDirty(unit);
    return 8;
}
