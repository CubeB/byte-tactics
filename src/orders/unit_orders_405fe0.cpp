// Decompiled by GPT-6. Names are provisional.
struct Unit { int active; void ReleaseWeapons(int); };
#pragma pack(push,1)
struct Order { char pad[5]; unsigned char state; unsigned flags; };
#pragma pack(pop)
class Class_00439e80 { public: void FUN_00439e80(int); };
Order* __stdcall FUN_0043b700(Unit*);
int __stdcall FUN_0043b1f0(Unit*,Order*,int);
int __stdcall RandomInt(int);
// FUNCTION: 0x405fe0
int __stdcall StandbyOrder(Unit* unit,Order* order,int flags)
{
    unsigned state=0; state=order->state;
    switch(state) {
    case 0:
        if(!unit->active) return 7;
        ((Unit*)unit)->ReleaseWeapons(3);
        order->flags|=0x10000;
        ((Class_00439e80*)order)->FUN_00439e80(1); return 1;
    case 1:
        {
            Order* next=FUN_0043b700(unit);
            if(next && FUN_0043b1f0(unit,next,0)) return 5;
        }
        order->flags|=0x10000;
        ((Class_00439e80*)order)->FUN_00439e80(RandomInt(30)+30); return 2;
    default: return 7;
    }
}
