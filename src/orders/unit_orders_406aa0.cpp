// Decompiled by GPT-6. Names are provisional.
#include <string.h>
#include <math.h>
struct Vec { int x, y, z; Vec() {} Vec(int a,int b,int c):x(a),y(b),z(c) {} };
static inline Vec Add(const Vec& a,const Vec& b) { Vec r; r.x=a.x+b.x; r.y=a.y+b.y; r.z=a.z+b.z; return r; }
static inline Vec Sub(const Vec& a,const Vec& b) { Vec r; r.x=a.x-b.x; r.y=a.y-b.y; r.z=a.z-b.z; return r; }
#pragma pack(push,1)
struct Def { char pad[0x15e]; Vec low, high; };
struct Unit { char pad[0x6a]; Vec pos; char pad76[0x92-0x76]; Def* def; char pad96[0x118-0x96]; };
struct Order { char pad[0x22]; Vec pos; };
struct Game { char pad[0x14357]; Unit* first; Unit* last; };
#pragma pack(pop)
extern Game* g_game;
void __stdcall FUN_00471fd0(Vec*,Vec*,int,int);
void __stdcall FUN_0048a9f0(Unit*,Vec,int);
// FUNCTION: 0x406aa0
int __stdcall FUN_00406aa0(Unit* unit,Order* order,int unused)
{
    Vec low = Add(unit->pos, unit->def->low);
    Vec high = Add(unit->pos, unit->def->high);
    for(Unit* p=g_game->first;p<=g_game->last;++p) {
        if(p->pos.x >= low.x && p->pos.x <= high.x && p->pos.z >= low.z && p->pos.z <= high.z && p->pos.y >= low.y && p->pos.y <= high.y && unit != p) {
            Vec dest = Sub(Add(order->pos,p->pos),unit->pos);
            FUN_00471fd0(&p->pos,&dest,30,5);
            FUN_0048a9f0(p,dest,1);
        }
    }
    return 5;
}
