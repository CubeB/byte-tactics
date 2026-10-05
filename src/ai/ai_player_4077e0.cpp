// Decompiled by GPT-6. Names are provisional.
#include <vector>
struct Vec { int x,y,z; };
struct Unit { int unused; };
struct Player;
void __stdcall OrderSquad(Player*,int,unsigned char,int,Unit*,Vec*,int,int);
struct Group {
    Player* player; int id; char pad8[8]; std::vector<Unit*> units;
    void Send(unsigned char mode,int remove,Unit* target,Vec* pos,int flags,int extra) {
        OrderSquad(player,id,mode,remove,target,pos,flags,extra);
    }
};
class SquadTimer { public: int GetAveragePosition(Vec*); };
class Class_00407560 { public: void FUN_00407560(int,int); };
class Class_004071f0 { public: Unit* FindNearestEnemyUnit(Vec); };
#pragma pack(push,1)
struct Owner { char pad[0x15]; SquadTimer* a; char pad19[8]; SquadTimer* b; SquadTimer* c; };
struct Game { char pad[0x38a47]; int time; };
#pragma pack(pop)
static inline int Rally(Owner*& owner, Vec* pos) {
    if(owner->c->GetAveragePosition(pos)) return 1;
    if(owner->a->GetAveragePosition(pos)) return 1;
    if(owner->b->GetAveragePosition(pos)) return 1;
    return 0;
}
extern Game* g_game;
class Class_00407930 { public:
    void* vtable; Owner* owner; Group* group; int next,player,minimum,maximum,limit,kind,attacking;
    void OnTimer();
};
// FUNCTION: 0x4077e0
void Class_00407930::OnTimer()
{
    Vec pos;
    Vec retreat;
    next=g_game->time+300;
    ((Class_00407560*)this)->FUN_00407560(kind,limit);
    if(!group->units.empty()) {
        if((int)group->units.size()<=minimum || (!attacking && (int)group->units.size()<maximum)) {
            if(Rally(owner,&retreat)) {
                attacking=0;
                group->Send(2,0,0,&retreat,160,0);
                return;
            }
        }
        attacking=1;
        ((SquadTimer*)this)->GetAveragePosition(&pos);
        Unit* target=((Class_004071f0*)owner)->FindNearestEnemyUnit(pos);
        if(target) group->Send(3,0,target,0,0,0);
    }
}
