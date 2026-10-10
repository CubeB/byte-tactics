// Decompiled by GPT-6, Claude Opus 5.5, space-bunny-free, deepseek-v4.1, deepseek-v4.1-flash and DeepSeek V4.1 Flash. Names are provisional.
// PlayerAI's RefreshUnitLists, which matches only in a file of its own: the
// addressing mode of its weight load follows this file's symbol ids. The rest
// of the class, its constructor and its other methods are in
// ai_player.cpp.
#include <windows.h>
#include <vector>
#include <math.h>
// Only for its symbol ids: RefreshUnitLists matches only in a window of the
// symbol count.
#include <io.h>

#include "../util/vec3.h"

#pragma pack(push,1)
struct Player { char pad[0x108]; unsigned char allied[0x3e]; unsigned char index; int IsAllied(unsigned char p) const { return allied[p]; } };
struct Def { char pad[0x156]; int builder; char pad15a[0x22f-0x15a]; char mobile; char pad230[0x241-0x230]; unsigned flags; char pad245[0x249-0x245]; };
struct Unit { char pad[0x6a]; Vec3 pos; char pad76[0x92-0x76]; Def* def; Player* player; char pad9a[12]; unsigned short unitDefIndex; char pada8[0x104-0xa8]; float buildLeft; char pad108[6]; unsigned char activateFlags; char pad10f; unsigned flags; int pad114; unsigned char PlayerIndex() const { return player->index; } int Ready() const { return (flags&0x10000000) && !(flags&0x4000); } };

#include "../units/unit_def.h"

#include "../map/mission.h"

#include "../map/feature.h"

#include "../map/cell.h"

struct Game {
    char unknown_0[0x14233];
    int mapWidthTiles;                 // +0x14233
    int mapHeightTiles;                // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    Feature* features;                 // +0x1426f
    char unknown_14273[0x14357 - 0x14273];
    Unit* units;                       // +0x14357
    Unit* unitsEnd;                    // +0x1435b
    char unknown_1435f[0x1438f - 0x1435f];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def* defs;                         // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Mission* net;                      // +0x391e9
};

#include "player_ai.h"
#pragma pack(pop)

extern Game* g_game;

int __stdcall RandomInt(int range);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
int __stdcall CanPlaceUnitFootprint(UnitDef* type, short a, Point16 cell, int b);
int __stdcall CanBuildAt(UnitDef* type, Point16 cell, int a, int b);
int GetBuildSiteMetal(void);
void __stdcall MakeHeap(Elem_0040cc40* first, Elem_0040cc40* last, int*, Elem_0040cc40*);
void __stdcall PopHeapFirst(Elem_0040cc40* first, Elem_0040cc40* last, Elem_0040cc40* dest,
                            Elem_0040cc40 val, int*);
Cell* __stdcall GetMapCell(int x, int y);
int __stdcall IsUnitVisibleToPlayer(Player*,Unit*);

static inline Point16 WorldToCell(Vec3 v, Point16 origin)
{
    Point16 c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

static inline int DistSq(const Point16& a, const Point16& b)
{
    int dy = a.y - b.y;
    int dx = a.x - b.x;
    return dx * dx + dy * dy;
}

// std::pop_heap(f, l) as the inline template expands it.
static inline void PopHeap(Elem_0040cc40* f, Elem_0040cc40* l)
{
    PopHeapFirst(f, l - 1, l - 1, Elem_0040cc40(*(l - 1)), (int*)0);
}

static inline Vec3 Direction(short angle, int scale)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    v.z = -FUN_004b7123(angle, scale);
    return v;
}

// Clears the three unit lists (+0x05, +0x15, +0x25) through
// std::vector<Unit*>::erase (0x40c9f0) and refills them with insert
// (0x408f30).
// FUNCTION: 0x40aa40
void PlayerAI::RefreshUnitLists()
{
    visible.Clear();
    factories.Clear();
    known.Clear();
    builders=0; hasSpecial=0;
    struct { float x,y,z,total; } sum={0,0,0,0};
    std::fill(counts.begin(),counts.end(),(short)0);
    for(Unit* u=g_game->units+1;u<=g_game->unitsEnd;++u) {
        if(u->Ready()) {
            if(!owner->IsAllied(u->PlayerIndex())) {
                if(IsUnitVisibleToPlayer(owner,u) && !(u->flags&0x8000)) visible.units.push_back(u);
                if((unsigned char)(u->flags>>8)&1) known.units.push_back(u);
            } else if(u->PlayerIndex()==owner->index && u->buildLeft==0.0) {
                ++counts[u->unitDefIndex];
                if(u->def->builder) ++builders;
                if((u->def->flags&0x40) && (u->def->flags&0x200) && (u->activateFlags&1)) factories.list.units.push_back(u);
                if((u->def->flags&0x400) && (u->activateFlags&1)) hasSpecial=1;
                // The table holds signed bytes: the cast gives the sign-extending load.
                float weight=(char)weights[u->unitDefIndex];
                sum.total+=weight;
                sum.x-=(float)u->pos.x*weight*-0.0000152587890625f;
                sum.y-=(float)u->pos.y*weight*-0.0000152587890625f;
                sum.z-=(float)u->pos.z*weight*-0.0000152587890625f;
            }
        }
    }
    if(sum.total!=0.0f) { sum.x/=sum.total; sum.y/=sum.total; sum.z/=sum.total; }
    int ix=(int)(sum.x*65536.0);
    int iy=(int)(sum.y*65536.0);
    int iz=(int)(sum.z*65536.0);
    centre=Vec3_00409160(ix,iy,iz);
}
