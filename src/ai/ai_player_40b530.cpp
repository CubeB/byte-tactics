// Decompiled by GPT-6. Names are provisional.
struct Unit;
void __stdcall FUN_00406c70(Unit**, Unit* const*);
namespace std {
inline void _Construct(Unit** dest, Unit* const& src) { FUN_00406c70(dest, &src); }
}
#include <vector>
struct Vec { int x,y,z; };
#pragma pack(push,1)
struct Def { char pad[0x241]; unsigned flags; };
struct Unit { char pad[0x6a]; Vec pos; char pad76[0x92-0x76]; Def* def; char pad96[0x10e-0x96]; unsigned char active; };
struct Owner { char pad[0x25]; std::vector<Unit*> factories; };
#pragma pack(pop)
extern Owner* g_playerAI[];
// FUNCTION: 0x40b530
void __stdcall FUN_0040b530(int player,const Vec* pos,int radius,std::vector<Unit*>* out)
{
    std::vector<Unit*>& list=g_playerAI[player]->factories;
    radius*=radius;
    for(std::vector<Unit*>::iterator it=list.begin();it!=list.end();++it) {
        Unit* unit=*it;
        if((unit->def->flags&0x40) && (unit->def->flags&0x200) && (unit->active&1)) {
            int dz=pos->z-unit->pos.z;
            int dx=pos->x-unit->pos.x;
            int d=(int)(((__int64)dx*dx)>>32)+(int)(((__int64)dz*dz)>>32);
            if(d<=radius) out->push_back(*it);
        }
    }
}
