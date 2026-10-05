// Decompiled by GPT-6. Names are provisional.
struct Vec { int x,y,z; };
#pragma pack(push,1)
struct Unit { char pad[0x6a]; Vec pos; char pad76[0xac-0x76]; int group; char padb0[0x118-0xb0]; };
struct Player { char pad[0x67]; Unit* first; Unit* last; };
#pragma pack(pop)
struct Group { Player* player; int id; };
class SquadTimer { public: char pad[8]; Group* group; int CountGroupUnitsInRadius(Vec*,int); };
// FUNCTION: 0x4074a0
int SquadTimer::CountGroupUnitsInRadius(Vec* pos,int radius)
{
    int count=0;
    int squared=radius*radius;
    Unit* u=group->player->first;
    Unit* last=group->player->last;
    for(;u<=last;++u) {
        if(u->group==group->id) {
            int dz=pos->z-u->pos.z;
            int dx=pos->x-u->pos.x;
            int distance=(int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32);
            if(distance<=squared) ++count;
        }
    }
    return count;
}
