// Decompiled by GPT-6. Names are provisional.
// Stays in its own file: it needs this file's Vec with a constructor, where unit_orders.cpp's Vec3 shares one load of the unit position between both corners.
#include <string.h>
#include <math.h>
struct Vec { int x, y, z; Vec() {} Vec(int a,int b,int c):x(a),y(b),z(c) {} };
static inline Vec Add(const Vec& a,const Vec& b) { Vec r; r.x=a.x+b.x; r.y=a.y+b.y; r.z=a.z+b.z; return r; }
static inline Vec Sub(const Vec& a,const Vec& b) { Vec r; r.x=a.x-b.x; r.y=a.y-b.y; r.z=a.z-b.z; return r; }
#pragma pack(push,1)
struct Def { char pad[0x15e]; Vec low, high; };
struct Unit { char pad[0x6a]; Vec pos; char pad76[0x92-0x76]; Def* def; char pad96[0x118-0x96]; };
struct Order { char pad[0x22]; Vec pos; };
struct Game { char pad[0x14357]; Unit* units; Unit* unitsEnd; };
#pragma pack(pop)
extern Game* g_game;
void __stdcall EmitTeleportParticles(Vec*,Vec*,int,int);
void __stdcall SetUnitPosition(Unit*,Vec,int);
// FUNCTION: 0x406aa0
int __stdcall TeleportOrder(Unit* unit,Order* order,int unused)
{
    Vec low = Add(unit->pos, unit->def->low);
    Vec high = Add(unit->pos, unit->def->high);
    for(Unit* p=g_game->units;p<=g_game->unitsEnd;++p) {
        if(p->pos.x >= low.x && p->pos.x <= high.x && p->pos.z >= low.z && p->pos.z <= high.z && p->pos.y >= low.y && p->pos.y <= high.y && unit != p) {
            Vec dest = Sub(Add(order->pos,p->pos),unit->pos);
            EmitTeleportParticles(&p->pos,&dest,30,5);
            SetUnitPosition(p,dest,1);
        }
    }
    return 5;
}
