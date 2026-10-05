// Decompiled by GPT-6 Astra. Names are provisional.
struct Unit;
struct Vec3 { int x, y, z; };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_00438930 { public: void FUN_00438930(Vec3*, int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_004895c0 { public: void SetUnit(Unit*); };
class CobScript { public: int StartScriptWithArgs(char*, void*, int, int, int, int, int, int); };
#pragma pack(push, 1)
struct Def { char pad0[0x180]; short height; char pad182[0x241-0x182]; unsigned int flags, flags2; };
struct Unit {
    int valid; char pad4[0x86-4]; Unit* transport; Unit* cargo; char pad8e[4]; Def* def;
    int pad96; CobScript* script; char pad9e[10]; unsigned short id;
};
struct Order { char pad0[5]; unsigned char state; unsigned int flags; char pada[8]; int ref; Unit* target; char pad1a[8]; Vec3 pos; char pad2e[8]; int attempts; };
#pragma pack(pop)
void __stdcall FUN_0047f780(Unit*, int, const char*);
int __stdcall FUN_00438730(Unit*, Order*, int);
// FUNCTION: 0x406900
int __stdcall FUN_00406900(Unit* unit, Order* order, int flags)
{
    if (flags&8) {
        FUN_0047f780(unit,7,"Unloading process is proceeding non-optimally");
        return 8;
    }
    switch(order->state) {
    case 0:
        if (!unit->valid) break;
        if (!(unit->def->flags2&0x100)) break;
        ((Class_004895c0*)&order->ref)->SetUnit(unit->cargo);
        if (!order->target) return 5;
        ((Class_00438880*)order)->FUN_00438880("Unloading");
        unit->script->StartScriptWithArgs("TransportDrop",0,1,1,order->target->id,
            (order->pos.x&0xffff0000)+(order->pos.z>>16),0,0);
        ++order->attempts;
        ((Class_00439e80*)order)->FUN_00439e80(15);
        return 1;
    case 1: return FUN_00438730(unit,order,8);
    case 2:
        if (order->target->transport!=unit) { FUN_0047f780(unit,13,0); return 5; }
        if (order->attempts>=3) return 9;
        if ((unsigned char)(unit->def->flags>>12)&1)
            ((Class_00438930*)order)->FUN_00438930(&order->pos,(int)(unit->def->height*1.5));
        else ((Class_00438930*)order)->FUN_00438930(&order->pos,0);
        order->flags=0xe8;
        return 1;
    case 3: return 0;
    }
    return 7;
}
