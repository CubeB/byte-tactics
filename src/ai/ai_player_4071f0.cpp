// Decompiled by GPT-6. Names are provisional.
#include <stdlib.h>
#pragma pack(push,1)
struct Unit {
    char pad[0x6a]; int x,y,z; char pad76[0x108-0x76]; unsigned char allied[6]; unsigned short flags10e;
    union { unsigned flags; struct { unsigned mode:2; unsigned rest:30; }; }; int pad114;
};
struct Player {
    int active; char pad4[0x67-4]; Unit* first; Unit* last; int pad6f; unsigned char state;
    char pad74[0x146-0x74]; unsigned char id; char pad147[4];
};
struct Game { char pad[0x1b63]; Player players[10]; };
#pragma pack(pop)
extern Game* g_game;
class Class_004071f0 { public: Unit* owner; Unit* FindNearestEnemyUnit(int,int,int); };
// FUNCTION: 0x4071f0
Unit* Class_004071f0::FindNearestEnemyUnit(int x,int y,int z)
{
    int best=0x7fffffff;
    Unit* result=0;
    for(unsigned char i=0;i<10;++i) {
        Player* p=&g_game->players[i];
        // The original retains the player-index range check inside the loop.
        if(i>=10) continue;
        if(p->active && (p->state==1 || p->state==2 || p->state==3) && p->id!=10 && !owner->allied[p->id]) {
            Unit* u=p->first;
            Unit* last=p->last;
            for(;u<=last;++u) {
                if((u->flags&0x10000000) && u->mode!=2 && !(u->flags&0x8000) && !(u->flags10e&4)) {
                    int dz=z-u->z;
                    int dx=x-u->x;
                    int distance=(int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32);
                    if(distance<best) { result=u; best=distance; }
                }
            }
        }
    }
    return result;
}
