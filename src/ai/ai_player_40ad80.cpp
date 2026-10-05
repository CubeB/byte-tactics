// Decompiled by GPT-6. Names are provisional.
// Both loops push_back into the output vector: the first calls insert
// (0x408f30) out of line, the second inlines it and calls _Ucopy (0x406c10),
// _Ufill (0x406c40) and size (0x40c560), all members of std::vector<Unit*>
// (#135).
#include <vector>
struct Vec { int x,y,z; };
#pragma pack(push,1)
struct Unit { char pad[0x6a]; Vec pos; char pad76[0x110-0x76]; unsigned flags; int Ready() const { return (flags&0x10000000) && !(flags&0x4000); } };
struct Owner { char pad[5]; std::vector<Unit*> visible,known; char pad25[0x79-0x25]; int hasSpecial; };
#pragma pack(pop)
extern Owner* g_playerAI[];
// FUNCTION: 0x40ad80
void __stdcall GetVisibleEnemiesInRadius(int player,const Vec* pos,int radius,int flags,std::vector<Unit*>* out)
{
    int radius2=radius*radius;
    std::vector<Unit*>& visible=g_playerAI[player]->visible;
    std::vector<Unit*>::iterator it;
    for(it=visible.begin();it!=visible.end();++it) {
        Unit* unit=*it;
        int dz=pos->z-unit->pos.z;
        int dx=pos->x-unit->pos.x;
        int d=(int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32);
        if(d<=radius2 && unit->Ready()) out->push_back(unit);
    }
    Owner* owner=g_playerAI[player];
    if(owner->hasSpecial && out->empty()) {
        for(it=owner->known.begin();it!=owner->known.end();++it) {
            Unit* unit=*it;
            int dz=pos->z-unit->pos.z;
        int dx=pos->x-unit->pos.x;
        int d=(int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32);
        if(d<=radius2 && unit->Ready()) out->push_back(unit);
        }
    }
}
