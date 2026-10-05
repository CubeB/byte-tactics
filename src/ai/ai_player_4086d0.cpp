// Decompiled by GPT-6. Names are provisional.
#include <vector>
#pragma pack(push,1)
struct Def { char pad[0x152]; int building; char pad156[0x22d-0x156]; char converter; char pad22e[0x249-0x22e]; };
struct Economy { char pad[0x8c]; float energy; char pad90[8]; float cost; };
struct Unit { char pad[0x5c]; void* orders; char pad60[0x92-0x60]; Def* def; Economy* economy; char pad9a[0x110-0x9a]; unsigned flags; };
struct Game { char pad[0x1439b]; Def* defs; char pad1439f[0x38a47-0x1439f]; int time; };
#pragma pack(pop)
extern Game* g_game;
struct Group { char pad[0x10]; std::vector<Unit*> units; };
class Class_00408810 { public: char pad[8]; Group* group; int next; unsigned player; void OnTimer(); };
class Class_0048b090 { public: void SetStateBits(int,int); };
float __stdcall FUN_00464ad0(Economy*);
int __stdcall FUN_004b6c30(int);
unsigned short __stdcall ChooseBuildOption(unsigned,Unit*);
void __stdcall QueueBuildOrder(char*,Unit*,int);
// FUNCTION: 0x4086d0
void Class_00408810::OnTimer()
{
    next=g_game->time+30;
    for(std::vector<Unit*>::iterator it=group->units.begin();it!=group->units.end();++it) {
        Unit* u=*it;
        if((u->flags&0x20000000) && (u->flags&0x10000000) && !(u->flags&0x4000)) {
            if(u->def->converter) {
                if(u->economy->cost+u->economy->cost < u->economy->energy) {
                    if(FUN_00464ad0(u->economy)>0.0f && FUN_004b6c30(5))
                        ((Class_0048b090*)u)->SetStateBits(1,1);
                } else ((Class_0048b090*)u)->SetStateBits(1,0);
            } else if(u->def->building && !u->orders) {
                unsigned short id=ChooseBuildOption(player,u);
                if(id) QueueBuildOrder((char*)&g_game->defs[id]+0x20,u,1);
            }
        }
    }
}
