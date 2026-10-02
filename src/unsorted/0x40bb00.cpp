// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by space-bunny-free. Names are provisional.
// MATCH (2026-10-02). The last hunk for four rounds was the SIB base/index
// slot of `test byte ptr [eax + ecx + 0x241], 0x20`: the original wants the
// defs pointer in base and the type*585 temp in index, and every source
// spelling emitted the pair the other way round (subscript vs pointer
// arithmetic, Def& / Def* locals, char* index arithmetic, inline flag
// helpers, unsigned char and bitfield flags, nested ifs, a local for type).
// The lever is the header set, not the spelling: with <windows.h> MSVC gives
// [ecx+eax+0x241], and it gives the wanted [eax+ecx+0x241] for <minmax.h>,
// <string.h>, <math.h>, <memory.h> or no include at all. What matters is the
// pair: <minmax.h> alone fixes the slot but drops the rest to 77.5% (682
// bytes, energy lands in ebx instead of ebp), <minmax.h> + <stdlib.h> gets to
// 98.2%, and <minmax.h> + <math.h> keeps both and matches. headers.py could
// not find this because it only tries windows.h/stdio.h/stdlib.h/string.h/
// math.h/memory.h/ddraw.h, and the file's min/max come from <minmax.h>, which
// is not in its list.
//
// Everything else is from the earlier rounds: the SHARED.md recipe
// (min(1000,cap) / min(500,cap) for the caps, fresh clamped locals metal2 and
// energy2, owner->counts[type] sign-extended at the FUN_00406ee0 call), Max()
// for the two float clamps, and the /10000 divide as the multiply by
// 0x68db8bad with the signed shift round-up.
#include <minmax.h>
#include <math.h>
struct Rating { signed char normal,metal,energy; };
class Class_00435100 { public: int FUN_00435100(); };
#pragma pack(push,1)
struct Player { char pad[0x8c]; float energy; char pad90[8]; float metal; char pad9c[8]; float energyCapacity,metalCapacity; char padac[0x14b-0xac]; };
struct Def { char pad[0x241]; unsigned flags; char pad245[4]; };
struct Game { char pad[0x1b63]; Player players[10]; char pad2851[0x1439b-0x1b63-10*0x14b]; Def* defs; char pad1439f[0x391e9-0x1439f]; Class_00435100* mode; };
struct Owner { char pad[0x69]; Rating* ratings; char pad6d[0x81-0x6d]; short* counts; char pad85[0xb1-0x85]; unsigned char* weights; };
#pragma pack(pop)
extern Game* g_game;
extern Owner* DAT_005119c0[];
float __stdcall FUN_00464ad0(Player*);
float __stdcall FUN_00464b10(Player*);
float __stdcall FUN_00464ab0(Player*);
float __stdcall FUN_00464af0(Player*);
int __stdcall FUN_00406ee0(int,unsigned short,int);
static inline float Max(float a,float b) { return a>b?a:b; }
// FUNCTION: 0x40bb00
int __stdcall FUN_0040bb00(int player,unsigned short type)
{
    Owner* owner=DAT_005119c0[player];
    Player* p=&g_game->players[player];
    if(p->energy<50.0f) return 0;
    if(p->metal<25.0f) return 0;
    if(g_game->mode->FUN_00435100()==1 && (g_game->defs[type].flags&0x20)) return 0;
    int energyCap=min(1000,(int)p->energyCapacity);
    int metalCap=min(500,(int)p->metalCapacity);
    int energy=(int)Max(0.0f,(energyCap-p->energy)*0.125f);
    int metal=(int)Max(0.0f,(metalCap-p->metal)*0.25f);
    if(FUN_00464ad0(p)<1.0f) energy+=20;
    if(FUN_00464b10(p)<1.0f) metal+=20;
    if(FUN_00464ab0(p)<50.0f) energy+=100;
    else if(FUN_00464ab0(p)<200.0f) energy+=10;
    if(FUN_00464af0(p)<3.0f) metal+=100;
    else if(FUN_00464af0(p)<5.0f) metal+=20;
    int metal2=min(max(metal,0),100);
    int energy2=min(max(energy-metal2,0),100);
    int normal=max(100-metal2-energy2,0);
    if(!FUN_00406ee0(player,type,owner->counts[type])) return 0;
    Rating* r=&owner->ratings[type];
    return (normal*r->normal+r->metal*metal2+r->energy*energy2)*owner->weights[type]/10000;
}
