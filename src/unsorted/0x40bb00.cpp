// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// deepseek-v4.1 issue 2638: applied the SHARED.md recipe (min(1000,cap) for the
// caps, fresh clamped locals metal2/energy2), 97.8% to 99.6%. One hunk remains.
// Partial (99.6%): defs[type] addressing is encoded [ecx+eax+0x241], the
// original has [eax+ecx+0x241]. EAX holds the defs pointer and ECX the scaled
// type in both, so only the SIB base/index slot differs. twenty-one spellings
// (subscript vs pointer arithmetic, both subscript orders, (Def*) and
// (unsigned char) casts, explicit char* index arithmetic with sizeof(Def) and
// the literal 585, the address of the member, 0x20 on either side, a nested if, a
// local for type, a Def* local in a helper and an inlined flag getter) all
// emit the same mirrored SIB, and headers.py finds no header set that flips it.
// 2026-09-30 GPT-6.1-sol retry: an additional helper reading defs[type] at
// offset 0x241 dropped to 77.1%; restoring the best retained 99.6% (686/686).
// 2026-10-01 deepseek-v4.1-flash retry 2: `0x20 & flags` is byte-neutral; a
// bitfield flags struct (unsigned :5; downloadable:1) keeps the mirrored SIB
// but also flips mov ebp,eax to mov ebx,eax at +0x85, 77.1% (682 bytes);
// restored the best 686/686 version.
// 2026-10-01 deepseek-v4.1-flash retry: reversed pointer addition
// `(type + g_game->defs)->flags` is byte-neutral: same mirrored SIB
// [ecx+eax+0x241] vs the original [eax+ecx+0x241], 686/686 at 99.6%.
// 2026-10-01 deepseek-v4.1-flash retry 3: binding the flags read as a
// `Def& d = g_game->defs[type];` reference, and as a nested `Def* defs`
// local inside the mode branch, both keep the mirrored SIB [ecx+eax+0x241]
// and additionally perturb the rating multiply (movsx/imul pair order and
// imul operands swap), dropping to 97.8%; restored this 99.6% version.

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
