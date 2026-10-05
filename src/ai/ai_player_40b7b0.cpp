// Decompiled by GPT-6. Names are provisional.
#include <vector>
struct Vec { int x,y,z; };
struct Bits { unsigned* words; int Test(unsigned short index) const { return (words[index>>5]&(1u<<(index&31)))!=0; } };
#pragma pack(push,1)
struct Player { int active; char pad4[0x73-4]; unsigned char type; };
struct Weapon { char pad[0x111]; unsigned char flags; };
struct Slot { char pad[12]; Weapon* weapon; char pad10[12]; };
struct Def { char pad[0x202]; short range; char pad204[0x231-0x204]; Bits bad[3]; Bits exclude; unsigned flags; };
struct Unit { char pad[4]; Slot weapons[3]; char pad58[0x6a-0x58]; Vec pos; char pad76[0x92-0x76]; Def* def; Player* owner; char pad9a[12]; unsigned short id; char pada8[0xff-0xa8]; unsigned char player; char pad100[14]; unsigned char status; char pad10f; unsigned flags; };
struct Game { char pad[0x37f30]; unsigned char flags; };
#pragma pack(pop)
extern Game* g_game;
class Class_004800c0 { public: void FUN_004800c0(Unit**); };
int __stdcall FUN_0049adf0(Unit*,unsigned char);
int __stdcall WeaponCanReachUnit(Unit*,Unit*,unsigned char);
int __stdcall RandomInt(int);
void __stdcall GetVisibleEnemiesInRadius(int,Vec*,int,int,std::vector<Unit*>*);
// FUNCTION: 0x40b7b0
Unit* __stdcall FindWeaponTarget(Unit* unit,unsigned char weapon,int useRange)
{
    int ai=unit->owner->active && unit->owner->type==2;
    Unit* fallback=0;
    int fallbackDistance=0x7fffffff;
    int bestDistance=0x7fffffff;
    Unit* best=0;
    std::vector<Unit*> candidates;
    if(useRange) GetVisibleEnemiesInRadius(unit->player,&unit->pos,FUN_0049adf0(unit,weapon),0,&candidates);
    else GetVisibleEnemiesInRadius(unit->player,&unit->pos,unit->def->range,0,&candidates);
    for(int count=0;count<50;++count) {
        if(candidates.empty()) break;
        std::vector<Unit*>::iterator it=candidates.begin()+RandomInt(candidates.size());
        Unit* target=*it;
        ((Class_004800c0*)&candidates)->FUN_004800c0(it);
        if((target->flags&0x10000000) && !(target->flags&0x4000) &&
           ((target->def->flags&0x8000) || ai || (g_game->flags&4)) &&
           ((unit->def->flags&0x10000000) || WeaponCanReachUnit(unit,target,weapon)) &&
           (useRange || !unit->def->exclude.Test(target->id)) &&
           (!(unit->weapons[weapon].weapon->flags&0x80) || !(target->status&0x10))) {
            int dz=unit->pos.z-target->pos.z;
            int dx=unit->pos.x-target->pos.x;
            int d=RandomInt((int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32));
            if(unit->def->bad[weapon].Test(target->id)) {
                if(d<fallbackDistance) { fallbackDistance=d; fallback=target; }
            } else if(d<bestDistance) { bestDistance=d; best=target; }
        }
    }
    if(best) return best;
    return fallback;
}
