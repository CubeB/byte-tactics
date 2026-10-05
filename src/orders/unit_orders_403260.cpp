// Decompiled by GPT-6. Names are provisional.
struct Vec { int x,y,z; };
struct Unit;
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
#pragma pack(push,2)
class Class_0043a1f0 { public: char pad[0x56]; Class_0043a1f0(Class_00438760,Unit*,Vec*,int,int,int); };
#pragma pack(pop)
#pragma pack(push,1)
struct Def { char pad[0x218]; unsigned short radius; };
struct Unit { char pad[0x6a]; Vec pos; char pad76[0x86-0x76]; int blocked; char pad8a[8]; Def* def; };
struct Order { char pad[5]; unsigned char state; unsigned flags; char pada[0x16-10]; Unit* target; char pad1a[8]; Vec pos; };
#pragma pack(pop)
class Class_00438880 { public: void FUN_00438880(int); };
class Class_00438930 { public: void FUN_00438930(Vec*,int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
void __stdcall FUN_0047f780(Unit*,int,void*);
void __stdcall AppendOrder(Unit*,Class_0043a1f0*);
// FUNCTION: 0x403260
int __stdcall AttackKamikazeOrder(Unit* unit,Order* order,unsigned flags)
{
    if(flags&0x10008) return 5;
    if(order->target) order->pos=order->target->pos;
    unsigned state=0; state=order->state;
    switch(state) {
    case 0:
        if(unit->blocked) return 7;
        ((Class_00438880*)order)->FUN_00438880(0);
        ((Class_00438930*)order)->FUN_00438930(&order->pos,unit->def->radius<16?16:unit->def->radius);
        ((Class_00439e80*)order)->FUN_00439e80(60);
        order->flags|=0xe0; return 1;
    case 1:
        if(flags&0x20) {
            FUN_0047f780(unit,6,0);
            AppendOrder(unit,new Class_0043a1f0("SELFDESTRUCT",0,0,1,0,0));
            return 5;
        }
        if(flags&0x40) return 8;
        order->state=0; return 2;
    default: return 7;
    }
}
