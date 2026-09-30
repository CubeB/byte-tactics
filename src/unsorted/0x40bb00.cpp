// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol issue 1994 refinement: eight checks kept 97.8%; no MATCH.
// Reversing the chained subtraction operands emitted identical code. The
// existing notes below describe the remaining operand-order and store-schedule
// differences; no tested variant improved the retained source.
// Partial (97.8%, eight checks in this retry): three scheduler/encoding diffs remain.
//  - defs[type] addressing is encoded [ecx+eax+0x241], original [eax+ecx+0x241].
//  - the energyCap/metalCap constant store is scheduled before the _ftol call,
//    original stores it after the cmp.
//  - normal uses 100-energy-metal, original 100-metal-energy (same value,
//    different subtraction order).
// Using windef min(1000,x) fixes the constant store but flips the final
// Rating product term order, so the if-form is kept.
#include <windows.h>
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
    int energyCap=1000;
    if((int)p->energyCapacity<=1000) energyCap=(int)p->energyCapacity;
    int metalCap=500;
    if((int)p->metalCapacity<=500) metalCap=(int)p->metalCapacity;
    int energy=(int)Max(0.0f,(energyCap-p->energy)*0.125f);
    int metal=(int)Max(0.0f,(metalCap-p->metal)*0.25f);
    if(FUN_00464ad0(p)<1.0f) energy+=20;
    if(FUN_00464b10(p)<1.0f) metal+=20;
    if(FUN_00464ab0(p)<50.0f) energy+=100;
    else if(FUN_00464ab0(p)<200.0f) energy+=10;
    if(FUN_00464af0(p)<3.0f) metal+=100;
    else if(FUN_00464af0(p)<5.0f) metal+=20;
    metal=min(max(metal,0),100);
    energy=min(max(energy-metal,0),100);
    int normal=max(100-metal-energy,0);
    if(!FUN_00406ee0(player,type,owner->counts[type])) return 0;
    Rating* r=&owner->ratings[type];
    return (normal*r->normal+r->metal*metal+r->energy*energy)*owner->weights[type]/10000;
}
