// Decompiled by GPT-6 Astra, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash pass (4 real check runs / many --sym): no improvement
// over 91.3%. New insight into why the explicit operator- flips esi/edi: the
// prologue "mov esi,[esp+0x38]" runs BEFORE "push edi", so it reads arg2 (the
// order) while "mov edi,[esp+0x38]" runs after the push and reads arg1 (the
// unit). The explicit operator- moves "push edi" earlier, so both loads read
// arg1/arg2 the other way and every use swaps register. The two remaining
// diffs are unchanged (state-3 needs x in edx; bounds[0] pos.x/min.x swap).
// Partial 91.3%, 2 real check runs this pass. Two diffs remain, both register
// ties inside state 3:
//   (1) "target->pos - Offset(angle,range)" is one instruction (2 bytes) too
//       long. Original: mov edx,[ecx]; mov edi,[ecx+4]; mov ecx,[ecx+8];
//       sub edx,ebp; sub ecx,eax; push 0x36; then the three stores. Ours adds
//       "mov edx,ecx" and interleaves the loads, the first store and the
//       subs. That extra mov is a direct consequence of our x landing in the
//       same register as the Vec3 base (ecx): MSVC 5 will not fold a memory
//       operand whose base register is the destination of the same
//       instruction, so the base is materialised into edx first. The original
//       puts x in edx and lets the base die on the third load. The +2 bytes
//       shift every later branch target and the jump table.
//   (2) bounds[0] = pos + def->min puts pos.x in edx / min.x in ebp where the
//       original has pos.x in ebp / min.x in edx.
// space-bunny-free pass, 14 scratch variants scored free with --sym, all <=
// 91.3%. Biggest finding: 0x413d80 is a MATCHING sibling whose state 4 is this
// function's state 3, character for character, and it compiles that block with
// the explicit-component operator- "Vec3 r; r.x=x-v.x; r.y=y-v.y; r.z=z-v.z;
// return r;" and a plain "Unit* target" at Order+0x16. Porting that operator-
// here makes our state-3 block byte exact (edx/edi/ecx, subs before the
// stores) but flips esi/edi (order and unit trade places) across the whole
// function and costs 12 bytes in the bounds[1] block: 71.5%. That flip is one
// allocator state, not two bugs, so the two remaining diffs above are very
// likely a single cause as well. The UnitRef plus Get() model of Order+0x16
// is load bearing: 0x413d80's plain "Unit* target" grows this function by
// 12 bytes and drops it to 87.7%.
// Worth knowing: the esi/edi flip comes from the by-value RETURN, not from
// the explicit component arithmetic. An out-param
// Sub(const Vec3&,const Vec3&,Vec3*) keeps esi=order and edi=unit and gets
// x into edx where the original has it, so diff (1) is nearly solved there,
// but it rotates edi/ebx through the build-rate block and the bounds blocks
// instead (86.6%). v0 and that variant are two points in the same allocator
// state, one with the state-3 block right and the rate block right, the
// other the reverse.
// Also tried and worse, so nobody repeats them: no named temp, the ctor
// argument spelled out inline (76.1%); explicit components into pos with a
// named off (82.8%) and with a hoisted target pointer (80.5%) and the same
// without the named temp (74.7%); "Vec3 r; r.x=x-v.x; r.y=y; r.z=z-v.z;"
// (71.5%); "Vec3 r=*this; r.x-=v.x; r.z-=v.z;" (86.6%, it does fix
// bounds[0] but rotates edi/ebx through both bounds blocks); operator-= over
// x and z only (91.3%, output byte identical); Offset writing through a
// pointer (91.3%, output byte identical).
// Earlier passes: GPT-6.1-sol used 7 runs and all 128 header combinations
// without moving off 91.3%. deepseek-v4.1-flash used 15 scratch variants
// (explicit operator- 74.7%, free-function 73.3%, by-value helper 67.7%,
// in-place pos-=off, named-off and explicit-pointer forms 80.7-86.6%, and
// min+pos, a reference or local min and a pointer-taking Add helper for
// bounds[0], all of which keep the same edx/ebp tie).
#include <stdio.h>
struct Point { short x, y; };
struct Vec3 {
    int x, y, z;
    void operator-=(const Vec3& v) { x-=v.x; y-=v.y; z-=v.z; }
    Vec3 operator-(const Vec3& v) const { Vec3 r=*this; r-=v; return r; }
    Vec3 operator+(const Vec3& other) const {
        Vec3 r;
        r.x = x + other.x;
        r.y = y + other.y;
        r.z = z + other.z;
        return r;
    }
};
struct Unit;
class Class_0043d210 { public: char pad0[0x2e]; unsigned char flags; void FUN_0043d210(Unit*,int); };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_0044e730 { public: void FUN_0044e730(int); };
class Class_0044e720 { public: void FUN_0044e720(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_0048b090 { public: void FUN_0048b090(int,int); };
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
class Class_00438ad0 { public: void FUN_00438ad0(Point, Point); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_004898b0 { public: void FUN_004898b0(int); };
class Class_004895c0 { public: void FUN_00489690(Unit*); };
#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x14a]; Point origin;
    char pad14e[8]; int canBuild; char pad15a[4]; Vec3 min, max;
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
struct UnitRef { void* table; Unit* ptr; Unit* Get() { return ptr; } };
struct Order {
    char pad0[5]; unsigned char state;
    unsigned int flags;
    char padA[0x12-0xa]; UnitRef target;
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
Unit* __stdcall FUN_00485f50(unsigned char, short, Vec3, int, int, int);
void __stdcall FUN_0043adc0(Class_00438760, int, Unit*, Unit*, Vec3*, int, int);
int __stdcall FUN_0048a980(Vec3*, Vec3*);
void __stdcall FUN_00438590(Unit*, Order*, short);
int __stdcall FUN_00438700(Unit*, Order*, int);
int __stdcall FUN_0041ba60(Unit*, Unit*, float);
void __stdcall FUN_0043e400(Unit*, Vec3*);
void __stdcall FUN_004720d0(Vec3*, Vec3*, int);
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
void __stdcall FUN_0048aac0(Unit*,Unit*,char,char);
void __stdcall FUN_00414350(Point,Vec3*,Point);
int __cdecl FUN_004b70ef(short,int);
int __cdecl FUN_004b7123(short,int);
short __cdecl FUN_004b715a(int,int);
static inline Vec3 Offset(short angle,int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }
static inline short Angle(Vec3* a,Vec3* b) { return FUN_004b715a(a->x-b->x,a->z-b->z); }
// FUNCTION: 0x414380
int __stdcall FUN_00414380(Unit* unit,Order* order,int flags)
{
    if (order->target.Get() && !(flags&8)) {
        if (flags&2) { FUN_0041c110(unit); return 5; }
        unsigned int state=0; state=order->state;
        switch(state) {
        case 0:
        if (unit->motion && (unit->def->flags&0x800)) {
            if (!unit->def->canBuild) return 7;
            ((Class_00438880*)order)->FUN_00438880("Building");
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
            return 1;
        }
        break;
    case 1: {
        order->retries=0;
        Class_0044e2d0* move=new Class_0044e2d0(order,order->target.Get()->pos);
        ((Class_0044e730*)move)->FUN_0044e730(unit->def->buildRange);
        ((Class_004388d0*)order)->FUN_004388d0((int)move);
        order->flags=0xe0;
        return 1;
    }
    case 2:
        if (flags&0x40) return 8;
        if (order->target.Get()->progress==0.0f) return 5;
        FUN_00438590(unit,order,(short)FUN_0048a980(&unit->pos,&order->target.Get()->pos));
        FUN_0041c110(unit);
        return 1;
    case 3: {
        if (g_game->tick%150==0) {
            int angle=FUN_0048a980(&unit->pos,&order->target.Get()->pos);
            int range=unit->def->buildRange<<16;
            angle+=0xdb6e;
            Vec3 pos=order->target.Get()->pos-Offset(angle,range);
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e720*)move)->FUN_0044e720((unsigned short)angle);
            ((Class_004388d0*)order)->FUN_004388d0((int)move);
        }
        int rate=0; rate=unit->def->buildRate;
        if (FUN_0041ba60(unit,order->target.Get(),(float)(rate/30))) {
            Vec3 start;
            FUN_0043e400(unit,&start);
            Vec3 bounds[2];
            bounds[0]=order->target.Get()->pos+order->target.Get()->def->min;
            bounds[1]=order->target.Get()->pos+order->target.Get()->def->max;
            FUN_004720d0(&start,bounds,6);
        }
        if (order->target.Get()->progress!=0.0f) {
            ((Class_00439e80*)order)->FUN_00439e80(1);
            order->flags|=0xa;
            return 2;
        }
        return 5;
    }
    }
        return 7;
    }
    FUN_0047f780(unit,7,"Construction terminated by hostile action");
    FUN_0041c110(unit);
    return 8;
}
