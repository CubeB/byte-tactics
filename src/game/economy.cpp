// Decompiled by Sonnet, Claude Opus 5.5, GPT-6, GPT-6.1-sol, Opus, deepseek-v4.1 and deepseek-v4.1-flash. Names are provisional.
// The compiler-generated `vector constructor iterator` (??_H), which constructs
// each element of an array of a class with a constructor.
//
// UpdatePlayerEconomy (0x401360) and its income helpers stay in
// economy_401360.cpp. The resource account's methods below and the
// end-of-tick update (0x401320) are shared with it by address.
//
// The included headers decide which operand of the float adds is loaded first.
#include <ddraw.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#pragma pack(push, 1)

// The end-of-tick update's view of one 0x18-byte account: the same bytes as
// Res_00401360. Kept a separate name so economy_401360.cpp, which defines the
// Res_00401360 spelling, and this file do not define one symbol twice.
struct Obj_00401320 {
    float x0;                          // +0x00
    float x1;                          // +0x04
    float x2;                          // +0x08
    float x3;                          // +0x0c
    float prev0;                       // +0x10
    float prev1;                       // +0x14
};

#include "../util/hapi_bank.h"

struct UnitInfo {
    char unknown_0[0xa8];
    unsigned short id;                 // +0xa8
};

// A unit's resource accounts; the class is declared in
// src/game/unit_resources.h. units/unit_save.cpp saves and loads the two
// 0x18-byte account blocks through SaveUnitAccounts and LoadUnitAccounts.
#include "unit_resources.h"

#include "../network/player.h"

struct UnitDef_00401360 {
    char unknown_0[0x1c2];
    float energyMake;                  // +0x1c2
    float energyUse;                   // +0x1c6
    float metalMake;                   // +0x1ca
    float extractsMetal;               // +0x1ce
    float windGenerator;               // +0x1d2
    float tidalGenerator;              // +0x1d6
    float cost;                        // +0x1da
    float costActive;                  // +0x1de
    float energyStorage;               // +0x1e2
    float metalStorage;                // +0x1e6
    char unknown_1ea[0x22d - 0x1ea];
    unsigned char makesMetal;          // +0x22d
};

struct Unit {
    char unknown_0[0x58];
    float extraction;                  // +0x58
    char unknown_5c[0x92 - 0x5c];
    UnitDef_00401360* def;             // +0x92
    char unknown_96[0xb0 - 0x96];
    unsigned int nextTick;             // +0xb0
    char unknown_b4[0xbc - 0xb4];
    UnitResources econ;                // +0xbc (player at +0xec)
    char unknown_f0[0x104 - 0xf0];
    float buildLeft;                   // +0x104
    char unknown_108[0x10e - 0x108];
    unsigned char flags10e;            // +0x10e
    char unknown_10f;
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int bits0 : 11;
            unsigned int bit11 : 1;
            unsigned int bits12 : 20;
        };
    };
    char unknown_114[0x118 - 0x114];
    void SetStateBits(int which, int on);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    int CountCargo();
    int CanReclaim(void*);
    void ClaimWeapons(unsigned char);
    void ReleaseWeapons(unsigned char);
};

struct Game {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63
    char unknown_2851[0x14267 - 0x2851];
    float tidal;                       // +0x14267
    char unknown_1426b[0x37ede - 0x1426b];
    float windFraction;                // +0x37ede
    char unknown_37ee2[0x37eee - 0x37ee2];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x38a47 - 0x37ef2];
    unsigned int gameTick;             // +0x38a47
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

// Calls a destructor-like member function on each of `count` objects of
// `stride` bytes, last to first (the counterpart of 0x401000).
class Obj {}; // opaque, non-virtual single-inheritance class
typedef void (Obj::*ThisFn)();

// FUNCTION: 0x401000 ??_H@YGXPAXIHP6EX0@Z@Z
struct Elem_00401000 {
    Elem_00401000();
    ~Elem_00401000();
    int field_0;
};

struct Holder_00401000 {
    Elem_00401000 items[4];
};

Holder_00401000 g_holder_00401000;

// FUNCTION: 0x401030
void __stdcall CallOnEachReverse(Obj* base, int stride, int count, ThisFn func)
{
    base = (Obj*)((char*)base + count * stride);
    while (--count >= 0) {
        base = (Obj*)((char*)base - stride);
        (base->*func)();
    }
}

// FUNCTION: 0x401070
void UnitResources::Reset(unsigned char playerIndex)
{
    memset(this, 0, sizeof(*this));
    player = &g_game->players[playerIndex];
}

// FUNCTION: 0x4010b0
void UnitResources::SaveUnitAccounts(UnitInfo* info, HapiBank* file)
{
    char name[32];
    sprintf(name, "u%04xacc", info->id);
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&energyMake, 0x18);
    file->WriteBox(&metalMake, 0x18);
}

// Load counterpart of 0x4010b0: reads the two 0x18-byte blocks back from the
// unit's "u%04xacc" entry, if it exists.
// FUNCTION: 0x401110
void UnitResources::LoadUnitAccounts(UnitInfo* info, HapiBank* file)
{
    char name[32];
    sprintf(name, "u%04xacc", info->id);
    if (file->OpenNamedBox(name)) {
        file->SeekBox(0);
        file->ReadBox(&energyMake, 0x18);
        file->ReadBox(&metalMake, 0x18);
    }
}

// FUNCTION: 0x401180
int UnitResources::RequestEnergy(UnitResources* r, float amount)
{
    r->energyUse += amount;
    if (r->energyStall > 0.0f)
        return 0;
    r->energyUsePaid += amount;
    return 1;
}

// FUNCTION: 0x4011c0
int UnitResources::RequestEnergyAndMetal(float dx, float dy)
{
    energyUse += dx;
    metalUse += dy;
    if (energyStall <= 0.0f && metalStall <= 0.0f) {
        energyUsePaid += dx;
        metalUsePaid += dy;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x401220
int UnitResources::SpendEnergy(float amount)
{
    if (player->energy >= amount) {
        player->energy -= amount;
        energyUse += amount;
        return 1;
    }
    return 0;
}

// Metal counterpart of 0x401220.
// FUNCTION: 0x401260
int UnitResources::SpendMetal(float amount)
{
    if (player->metal >= amount) {
        player->metal -= amount;
        metalUse += amount;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4012a0
int UnitResources::SpendEnergyAndMetal(float energy, float metal)
{
    if (player->energy >= energy && player->metal >= metal) {
        player->energy -= energy;
        energyUse += energy;
        if (player->metal >= metal) {
            player->metal -= metal;
            metalUse += metal;
        }
        return 1;
    }
    return 0;
}

// FUNCTION: 0x401320
void __stdcall SettleResourceAccount(Obj_00401320* p, float a, float b)
{
    p->prev1 = p->x1;
    p->prev0 = p->x0;
    // One combined expression: as two statements the demand term is evaluated first.
    p->x3 = p->x3 - a * p->x3 + (p->x2 - b * p->x2);
    p->x1 = 0;
    p->x0 = 0;
    p->x2 = 0;
}
