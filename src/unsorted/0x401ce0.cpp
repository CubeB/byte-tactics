// Decompiled by GPT-6. Names are provisional.
#include <vector>
struct Vec { int x,y,z; };
#pragma pack(push,1)
struct Unit { char pad[0x6a]; Vec pos; char pad76[0xff-0x76]; unsigned char player; };
struct Order { char pad[5]; unsigned char state; char pad6[0x36-6]; int wait; int radius; };
#pragma pack(pop)
class Class_00439e80 { public: void FUN_00439e80(int); };
void __stdcall FUN_0040ad80(int,Vec*,int,int,std::vector<Unit*>*);
int __stdcall FUN_004b6c30(int);
// FUNCTION: 0x401ce0
int __stdcall FUN_00401ce0(Unit* unit,Order* order,int flags)
{
    if(order->radius) {
        std::vector<Unit*> units;
        FUN_0040ad80(unit->player,&unit->pos,order->radius,0,&units);
        if(!units.empty()) return 5;
        if(order->wait<=0) return 5;
        int delay=FUN_004b6c30(30)+150;
        order->wait-=delay;
        ((Class_00439e80*)order)->FUN_00439e80(delay);
        return 2;
    }
    unsigned state=0;
    state=order->state;
    switch(state) {
    case 0: ((Class_00439e80*)order)->FUN_00439e80(order->wait); return 1;
    case 1: return 5;
    default: return 7;
    }
}
