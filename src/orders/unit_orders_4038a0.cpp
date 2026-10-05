// Decompiled by GPT-6. Names are provisional.
struct Vec { int x,y,z; };
#pragma pack(push,1)
struct Def { char pad[0x241]; unsigned low:11; unsigned flying:1; unsigned high:20; };
struct Unit { int active; char pad4[0x92-4]; Def* def; };
struct Order { char pad[5]; unsigned char state; unsigned flags; char pada[0x22-10]; Vec pos; char pad2e[8]; int weapon; int radius; };
#pragma pack(pop)
class Class_00438880 { public: void FUN_00438880(int); };
class Class_00438930 { public: void FUN_00438930(Vec*,int); };
class Class_00489800 { public: void ReleaseWeapons(int); };
class Class_004898b0 { public: void ClaimWeapons(int); };
void __stdcall SetWeaponTargetPos(Unit*,Vec*,int);
int __stdcall FUN_0049adf0(Unit*,unsigned char);
int __stdcall FUN_004b6c30(int);
// FUNCTION: 0x4038a0
int __stdcall FUN_004038a0(Unit* unit,Order* order,unsigned flags)
{
    if(flags&0x800) return 5;
    unsigned state=0; state=order->state;
    switch(state) {
    case 0:
        if(unit->def->flying) return 8;
        ((Class_00438880*)order)->FUN_00438880(0);
        order->radius=FUN_0049adf0(unit,order->weapon); return 1;
    case 1:
        if(order->weapon==2) {
            ((Class_004898b0*)unit)->ClaimWeapons(3);
            SetWeaponTargetPos(unit,&order->pos,2);
            order->flags=0x1c00; return 1;
        }
        ((Class_004898b0*)unit)->ClaimWeapons(0);
        ((Class_004898b0*)unit)->ClaimWeapons(1);
        SetWeaponTargetPos(unit,&order->pos,0);
        SetWeaponTargetPos(unit,&order->pos,1);
        order->flags=0x1c00; return 1;
    case 2:
        ((Class_00489800*)unit)->ReleaseWeapons(3);
        if(flags&0x400) { order->state=1; return 6; }
        if(unit->active) {
            if(order->radius<=0) return 9;
            ((Class_00438930*)order)->FUN_00438930(&order->pos,order->radius);
            order->flags=0xe0;
            order->radius-=FUN_004b6c30(FUN_0049adf0(unit,order->weapon)/3);
            order->state=1; return 4;
        }
        return 9;
    default: return 7;
    }
}
