// Decompiled by GPT-6. Names are provisional.
struct Vec { int x,y,z; };
struct Unit { int active; void ReleaseWeapons(int); };
#pragma pack(push,1)
struct Order { char pad[5]; unsigned char state; unsigned flags; char pada[0x22-10]; Vec pos; };
#pragma pack(pop)
class Class_00438880 { public: void FUN_00438880(int); };
class Class_00438930 { public: void FUN_00438930(Vec*,int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
void __stdcall FUN_0043a020(Unit*,Order*);
Order* __stdcall FUN_0043b700(Unit*);
int __stdcall FUN_0043b1f0(Unit*,Order*,int);
int __stdcall RandomInt(int);
// FUNCTION: 0x4033a0
int __stdcall PatrolOrder(Unit* unit,Order* order,int flags)
{
    unsigned state=0; state=order->state;
    switch(state) {
    case 0:
        if(!unit->active) return 7;
        ((Class_00438880*)order)->FUN_00438880(0);
        FUN_0043a020(unit,order);
        ((Class_00439e80*)order)->FUN_00439e80(1); return 1;
    case 1:
        unit->ReleaseWeapons(3);
        ((Class_00438930*)order)->FUN_00438930(&order->pos,0);
        ((Class_00439e80*)order)->FUN_00439e80(15);
        order->flags|=0xe0; return 1;
    case 2:
        if(flags&0xe0) { order->state=1; return 6; }
        {
            Order* next=FUN_0043b700(unit);
            if(next && FUN_0043b1f0(unit,next,0)) { order->flags=0; order->state=1; return 3; }
        }
        ((Class_00439e80*)order)->FUN_00439e80(RandomInt(30)+30);
        order->state=1; return 4;
    default: return 7;
    }
}
