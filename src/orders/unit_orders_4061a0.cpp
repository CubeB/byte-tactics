// Decompiled by GPT-6. Names are provisional.
struct Point { short x,y; };
struct Vec { int x,y,z; };
static inline Point MakePoint(int x,int y) { Point p; p.x=x; p.y=y; return p; }
static inline Point Sub(Point a,Point b) { Point p; p.x=a.x-b.x; p.y=a.y-b.y; return p; }
static inline Point Convert(Vec v) { Point p; p.x=(unsigned)v.x>>20; p.y=(unsigned)v.z>>20; return p; }
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
class Class_00438b90 { public: void FUN_00438b90(Class_00438760); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_00438ad0 { public: void FUN_00438ad0(Point,Point); };
#pragma pack(push,1)
struct Def { char pad[0x1c0]; short height; char pad1c2[0x241-0x1c2]; unsigned unused:11; unsigned flying:1; unsigned rest:20; };
struct Unit { int valid; char pad4[0x6a-4]; Vec pos; char pad76[8]; short size; char pad80[0x92-0x80]; Def* def; };
struct Order { char pad[5]; unsigned char state; unsigned flags; char pada[0x22-10]; Vec pos; char pad2e[0x4a-0x2e]; void* next; };
#pragma pack(pop)
// FUNCTION: 0x4061a0
int __stdcall ParkOrder(Unit* unit,Order* order,int flags)
{
    switch(order->state) {
    case 0:
        if(!unit->valid) return 7;
        if(unit->def->flying) {
            order->pos=unit->pos;
            ((Class_00438b90*)order)->FUN_00438b90("VTOL_MOVE");
            return 0;
        }
        {
            int size=unit->size;
            if(unit->def->height>=0) size+=3;
            Point start=Sub(Convert(unit->pos),MakePoint(size*4,size*3));
            Point extent=MakePoint(size*8,size*6);
            ((Class_00438ad0*)order)->FUN_00438ad0(start,extent);
            order->flags=0xe0;
            return 1;
        }
    case 1:
        if(flags&0x20) return 5;
        if(order->next) return 5;
        ((Class_00439e80*)order)->FUN_00439e80(30); return 0;
    default: return 7;
    }
}
