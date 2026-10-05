// Decompiled by GPT-6. Names are provisional.
// Clears the three unit lists (+0x05, +0x15, +0x25) through
// std::vector<Unit*>::erase (0x40c9f0) and refills them with insert
// (0x408f30), the members named in #135.
#include <vector>
#include <math.h>
template<class T> struct List : std::vector<T> { void Clear() { clear(); } };
struct FloatVec { float x,y,z; FloatVec():x(0),y(0),z(0){} };
struct Vec { int x,y,z; Vec(int a,int b,int c):x(a),y(b),z(c){} };
#pragma pack(push,1)
struct Player { char pad[0x108]; unsigned char allied[0x3e]; unsigned char index; int IsAllied(unsigned char p) const { return allied[p]; } };
struct Def { char pad[0x156]; int builder; char pad15a[0x241-0x15a]; unsigned flags; };
struct Unit { char pad[0x6a]; int x,y,z; char pad76[0x92-0x76]; Def* def; Player* owner; char pad9a[12]; unsigned short id; char pada8[0x104-0xa8]; float progress; char pad108[6]; unsigned char active; char pad10f; unsigned flags; int pad114; unsigned char PlayerIndex() const { return owner->index; } int Ready() const { return (flags&0x10000000) && !(flags&0x4000); } };
struct Game { char pad[0x14357]; Unit* units; Unit* end; };
class Class_0040aa40 {
public:
    Player* owner; char pad4;
    List<Unit*> visible, known, factories;
    Vec centre;
    char pad41[0x75-0x41];
    int builders,hasSpecial;
    std::vector<short> counts;
    char pad8d[16];
    std::vector<char> weights;
    void RefreshUnitLists();
};
#pragma pack(pop)
extern Game* g_game;
int __stdcall FUN_00465ac0(Player*,Unit*);
// FUNCTION: 0x40aa40
void Class_0040aa40::RefreshUnitLists()
{
    visible.Clear();
    factories.Clear();
    known.Clear();
    builders=0; hasSpecial=0;
    struct { float x,y,z,total; } sum={0,0,0,0};
    std::fill(counts.begin(),counts.end(),(short)0);
    for(Unit* u=g_game->units+1;u<=g_game->end;++u) {
        if(u->Ready()) {
            if(!owner->IsAllied(u->PlayerIndex())) {
                if(FUN_00465ac0(owner,u) && !(u->flags&0x8000)) visible.push_back(u);
                if((unsigned char)(u->flags>>8)&1) known.push_back(u);
            } else if(u->PlayerIndex()==owner->index && u->progress==0.0) {
                ++counts[u->id];
                if(u->def->builder) ++builders;
                if((u->def->flags&0x40) && (u->def->flags&0x200) && (u->active&1)) factories.push_back(u);
                if((u->def->flags&0x400) && (u->active&1)) hasSpecial=1;
                float weight=weights[u->id];
                sum.total+=weight;
                sum.x-=(float)u->x*weight*-0.0000152587890625f;
                sum.y-=(float)u->y*weight*-0.0000152587890625f;
                sum.z-=(float)u->z*weight*-0.0000152587890625f;
            }
        }
    }
    if(sum.total!=0.0f) { sum.x/=sum.total; sum.y/=sum.total; sum.z/=sum.total; }
    int ix=(int)(sum.x*65536.0);
    int iy=(int)(sum.y*65536.0);
    int iz=(int)(sum.z*65536.0);
    centre=Vec(ix,iy,iz);
}
