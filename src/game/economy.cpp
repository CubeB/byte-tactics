// Decompiled by Sonnet, Claude Opus 5.5, GPT-6, GPT-6.1-sol, Opus, deepseek-v4.1 and deepseek-v4.1-flash. Names are provisional.
// The compiler-generated `vector constructor iterator` (??_H), which constructs
// each element of an array of a class with a constructor. MSVC 5 inlines it as
// a loop unless /Ob2's inline budget has run out; an element type with both a
// constructor and a destructor in an array member of a global's class keeps
// it out of line.
//
// UpdatePlayerEconomy (0x401360) and its income helpers stay in
// economy_401360.cpp: that function does not reproduce byte for byte in this
// merged file (its best is 95.3%, an x87 operand order in the inlined income
// adds), so its original file is kept. The resource account's methods below
// and the end-of-tick update (0x401320) are shared with it by address.
#include <ddraw.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Unit;
struct Player;

#pragma pack(push, 1)

// The unit's (and the player's) resource accounts: energy, then metal.
struct Res_00401360 {
    float produced;                    // +0x0
    float used;                        // +0x4
    float demand;                      // +0x8
    float backlog;                     // +0xc
    float lastProduced;                // +0x10
    float lastUsed;                    // +0x14
};

struct PlayerRes_00401360 {
    float stored;                      // +0x0
    float produced;                    // +0x4
    float used;                        // +0x8
};

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

class HapiBank {
public:
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int WriteBox(void* data, int size);
    int ReadBox(void* buf, int size);
};

struct UnitInfo {
    char unknown_0[0xa8];
    unsigned short id;                 // +0xa8
};

// The same resource object as UnitResources, under the names units/unit_save.cpp
// calls: its save and load each write the two 0x18-byte account blocks.
class Class_004010b0 {
public:
    char acc0[0x18];                   // +0x00
    char acc1[0x18];                   // +0x18
    void SaveUnitAccounts(UnitInfo* info, HapiBank* file);
};

class Class_00401110 {
public:
    char acc0[0x18];                   // +0x00
    char acc1[0x18];                   // +0x18
    void LoadUnitAccounts(UnitInfo* info, HapiBank* file);
};

class UnitResources {
public:
    Res_00401360 res[2];               // +0x0
    Player* player;                    // +0x30

    void Reset(unsigned char playerIndex);
    int FUN_00401180(UnitResources* r, float amount);
    int FUN_004011c0(float dx, float dy);
    int SpendEnergy(float amount);
    int SpendMetal(float amount);
    int SpendEnergyAndMetal(float energy, float metal);
};

struct Player {
    int active;                        // +0x0
    char unknown_4[0x67 - 4];
    Unit* units;                       // +0x67
    Unit* units_end;                   // +0x6b
    char unknown_6f[0x73 - 0x6f];
    unsigned char type;                // +0x73
    char unknown_74[0x8c - 0x74];
    PlayerRes_00401360 res[2];         // +0x8c
    float storage[2];                  // +0xa4
    double totalProduced[2];           // +0xac
    double totalUsed[2];               // +0xbc
    double totalExcess[2];             // +0xcc
    float storageBonus[2];             // +0xdc
    char unknown_e4[0xec - 0xe4];
    UnitResources* econ;               // +0xec
    char unknown_f0[0x149 - 0xf0];
    unsigned char flags149;            // +0x149
    char unknown_14a;
};

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
    float wind;                        // +0x37ede
    char unknown_37ee2[0x37eee - 0x37ee2];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x38a47 - 0x37ef2];
    unsigned int ticks;                // +0x38a47
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
void __stdcall FUN_00401030(Obj* base, int stride, int count, ThisFn func)
{
    base = (Obj*)((char*)base + count * stride);
    while (--count >= 0) {
        base = (Obj*)((char*)base - stride);
        (base->*func)();
    }
}

// Pilot functions used to validate the toolchain. Names are provisional.
// FUNCTION: 0x401070
void UnitResources::Reset(unsigned char playerIndex)
{
    memset(this, 0, sizeof(*this));
    player = &g_game->players[playerIndex];
}

// FUNCTION: 0x4010b0
void Class_004010b0::SaveUnitAccounts(UnitInfo* info, HapiBank* file)
{
    char name[32];
    sprintf(name, "u%04xacc", info->id);
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    ((HapiBank*)file)->WriteBox(acc0, 0x18);
    ((HapiBank*)file)->WriteBox(acc1, 0x18);
}

// Load counterpart of 0x4010b0: reads the two 0x18-byte blocks back from the
// unit's "u%04xacc" entry, if it exists.
// FUNCTION: 0x401110
void Class_00401110::LoadUnitAccounts(UnitInfo* info, HapiBank* file)
{
    char name[32];
    sprintf(name, "u%04xacc", info->id);
    if (file->OpenNamedBox(name)) {
        ((HapiBank*)file)->SeekBox(0);
        ((HapiBank*)file)->ReadBox(acc0, 0x18);
        ((HapiBank*)file)->ReadBox(acc1, 0x18);
    }
}

// As in 0x4011c0, the header include decides whether MSVC loads the float
// parameter or the field first.
// FUNCTION: 0x401180
int UnitResources::FUN_00401180(UnitResources* r, float amount)
{
    r->res[0].used += amount;
    if (r->res[0].backlog > 0.0f)
        return 0;
    r->res[0].demand += amount;
    return 1;
}

// Any header include matters here: with the larger symbol table MSVC loads
// the float parameter first (fld dx; fadd [ecx+4]) instead of the field.
// FUNCTION: 0x4011c0
int UnitResources::FUN_004011c0(float dx, float dy)
{
    res[0].used += dx;
    res[1].used += dy;
    if (res[0].backlog <= 0.0f && res[1].backlog <= 0.0f) {
        res[0].demand += dx;
        res[1].demand += dy;
        return 1;
    }
    return 0;
}

// As in 0x4012a0, the header include decides whether MSVC loads the float
// parameter or the field first.
// FUNCTION: 0x401220
int UnitResources::SpendEnergy(float amount)
{
    if (player->res[0].stored >= amount) {
        player->res[0].stored -= amount;
        res[0].used += amount;
        return 1;
    }
    return 0;
}

// Metal counterpart of 0x401220.
// FUNCTION: 0x401260
int UnitResources::SpendMetal(float amount)
{
    if (player->res[1].stored >= amount) {
        player->res[1].stored -= amount;
        res[1].used += amount;
        return 1;
    }
    return 0;
}

// The header include matters, as in 0x4011c0: without it MSVC loads the
// field before the parameter in energyUsed += energy.
// FUNCTION: 0x4012a0
int UnitResources::SpendEnergyAndMetal(float energy, float metal)
{
    if (player->res[0].stored >= energy && player->res[1].stored >= metal) {
        player->res[0].stored -= energy;
        res[0].used += energy;
        if (player->res[1].stored >= metal) {
            player->res[1].stored -= metal;
            res[1].used += metal;
        }
        return 1;
    }
    return 0;
}

// As in 0x401220 and 0x4012a0, the header include decides which operand
// MSVC loads first (<stdlib.h> here). The update is written as one combined
// expression: as two statements this merged file's symbol state evaluates the
// demand term before the backlog one and only reaches 66.7%.
// FUNCTION: 0x401320
void __stdcall FUN_00401320(Obj_00401320* p, float a, float b)
{
    p->prev1 = p->x1;
    p->prev0 = p->x0;
    p->x3 = p->x3 - a * p->x3 + (p->x2 - b * p->x2);
    p->x1 = 0;
    p->x0 = 0;
    p->x2 = 0;
}
