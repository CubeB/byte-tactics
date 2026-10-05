// Decompiled by GPT-6. Names are provisional.
#pragma pack(push,1)
struct Def { char pad[0x245]; unsigned low:20; unsigned countdown:3; unsigned high:9; };
struct Unit { char pad[0x92]; Def* def; char pad96[0x110-0x96]; unsigned flags; };
struct Order { char pad[6]; unsigned flags; char pada[0x36-10]; int done; unsigned count; };
#pragma pack(pop)
class Class_00439e80 { public: void FUN_00439e80(int); };
void __stdcall QueueUnitSpeech(Unit*,int,void*);
void __stdcall DamageUnit(Unit*,Unit*,int,int,int);
int __stdcall FUN_004b6c30(int);
// FUNCTION: 0x402010
int __stdcall SelfDestructOrder(Unit* unit,Order* order,int flags)
{
    if(!(order->count&0xf0000000)) order->count=unit->def->countdown|0xf0000000;
    if(!order->done && unit->def->countdown>0) {
        int sounds[6]={22,21,20,19,18,17};
        if(flags&2) {
            if(!(unit->flags&0x4000)) { QueueUnitSpeech(unit,23,0); return 5; }
        } else {
            int count=order->count&0x0fffffff;
            if(!count) order->done=1;
            else order->count=(count-1)|0xf0000000;
            if(count>=0) {
                QueueUnitSpeech(unit,sounds[count],0);
                if(count>0) {
                    ((Class_00439e80*)order)->FUN_00439e80(30);
                    order->flags|=2; return 1;
                }
                if(count==0) {
                    ((Class_00439e80*)order)->FUN_00439e80(FUN_004b6c30(15));
                    order->flags|=2; return 1;
                }
            }
            DamageUnit(unit,unit,30000,3,0);
        }
    } else DamageUnit(unit,unit,30000,3,0);
    return 5;
}
