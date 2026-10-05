// Decompiled by Claude Opus 5.5, finished by GPT-6, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5, matched by Claude Opus 5.5. Names are provisional.
// Per-tick economy update for one player: every unit's energy/metal use and
// production is summed, the player's totals and storage are updated, and the
// share of the demand that could be met is fed back into every account.
//
// MATCH (Claude Opus 5.5, #5604). The last difference was tidal's AI
// dispatch (`je E0; dec; je E1; jmp En` in the original, where the case 1
// block is merged whole into the else branch's E1). What gave it:
//  - Tidal and the else branch's income add (UseEnergyD's negative arm) go
//    through AddIncomeDB, the AI scaling written as in the matched 0x4237d0:
//    `if (AI) switch { case ...: ...; break; default: ...; break; } else
//    *dst += v;` with a double amount. With the return form (AddIncomeD) or
//    the form without a default case for either one, tidal's case 1 block
//    keeps its own `fmul; jmp` and the dispatch folds to `jne En`.
//  - <float.h> after <ddraw.h>. Without it the then-branch UseEnergy's x87
//    code switches to the keep-and-pop form (98.0%) or the whole layout
//    changes (84.4%); <time.h> or <malloc.h> there match as well, <math.h>,
//    <stdlib.h> or <windows.h> do not. So the file's symbol count decides it.
//  - The `goto done` exits and the label the 89.2% version needed are gone:
//    `if (ok) AddIncome(...)` gives the same bytes now.
// Earlier findings that still hold:
// - The accumulators are separate float[2] arrays, not one struct: with one
//   aggregate MSVC strength-reduces the normalisation loop to a pointer and a
//   countdown, the original keeps `i * 4` in ecx ([esp+ecx+N]). The frame
//   order comes out right only with `avail` (production plus stock) and
//   `demandRatio` split off into their own arrays.
// - `avail[i] -= take; float left = avail[i];` keeps the remaining amount on
//   the x87 stack for the second half of the loop, as the original does.
// - The unit's resource account is a class at +0xbc whose owner pointer is at
//   +0x30 (unit+0xec); 0x401180..0x4012a0 are its methods and 0x401320 is the
//   end-of-tick update, all defined above without FUNCTION lines. The cost
//   block is SpendEnergy inlined.
// - UseEnergy's positive arm is a helper taking the unit whose `used` store
//   goes through a float* (otherwise the backlog compare is scheduled above
//   it). UseEnergyD's positive arm converts the amount to a double for the
//   demand add, which shares the backlog > 0 pop with the default add.
// - Income adds: a float amount adds straight to the field (`fadd [m]`), a
//   double one keeps the amount and pops it (`fld [m]; fadd st(1); fstp [m];
//   fstp st(0)`). Wind's AddIncomeW takes a double but adds `(float)v` in its
//   default and non-AI paths, so its case blocks merge into the else
//   branch's (C2's cross-jumper compares tuples, not bytes).
#include <ddraw.h>
#include <float.h>
struct Unit;
struct Player_00401360;

#pragma pack(push, 1)
struct Res_00401360 {
    float produced;                    // +0x0
    float used;                        // +0x4
    float demand;                      // +0x8
    float backlog;                     // +0xc
    float lastProduced;                // +0x10
    float lastUsed;                    // +0x14
};

// The unit's (and the player's) resource accounts: energy, then metal.
class Econ_00401360 {
public:
    Res_00401360 res[2];               // +0x0
    Player_00401360* owner;            // +0x30
    int FUN_00401180(Econ_00401360* e, float amount);
    int FUN_004011c0(float energy, float metal);
    int SpendEnergy(float amount);
    int SpendMetal(float amount);
    int SpendEnergyAndMetal(float energy, float metal);
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

struct PlayerRes_00401360 {
    float stored;                      // +0x0
    float produced;                    // +0x4
    float used;                        // +0x8
};

struct Player_00401360 {
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
    Econ_00401360* econ;               // +0xec
    char unknown_f0[0x149 - 0xf0];
    unsigned char flags149;            // +0x149
};

struct Unit {
    char unknown_0[0x58];
    float extraction;                  // +0x58
    char unknown_5c[0x92 - 0x5c];
    UnitDef_00401360* def;             // +0x92
    char unknown_96[0xb0 - 0x96];
    unsigned int nextTick;             // +0xb0
    char unknown_b4[0xbc - 0xb4];
    Econ_00401360 econ;                // +0xbc (owner at +0xec)
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
    char unknown_0[0x14267];
    float tidal;                       // +0x14267
    char unknown_1426b[0x37ede - 0x1426b];
    float wind;                        // +0x37ede
    char unknown_37ee2[0x37eee - 0x37ee2];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x38a47 - 0x37ef2];
    unsigned int ticks;                // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

// The functions before 0x401360 in the original file (each matched in its own
// file under another class name), defined here without FUNCTION lines.
int Econ_00401360::FUN_00401180(Econ_00401360* e, float amount)
{
    e->res[0].used += amount;
    if (e->res[0].backlog > 0.0f)
        return 0;
    e->res[0].demand += amount;
    return 1;
}

int Econ_00401360::FUN_004011c0(float energy, float metal)
{
    res[0].used += energy;
    res[1].used += metal;
    if (res[0].backlog <= 0.0f && res[1].backlog <= 0.0f) {
        res[0].demand += energy;
        res[1].demand += metal;
        return 1;
    }
    return 0;
}

int Econ_00401360::SpendEnergy(float amount)
{
    if (owner->res[0].stored >= amount) {
        owner->res[0].stored -= amount;
        res[0].used += amount;
        return 1;
    }
    return 0;
}

int Econ_00401360::SpendMetal(float amount)
{
    if (owner->res[1].stored >= amount) {
        owner->res[1].stored -= amount;
        res[1].used += amount;
        return 1;
    }
    return 0;
}

int Econ_00401360::SpendEnergyAndMetal(float energy, float metal)
{
    if (owner->res[0].stored >= energy && owner->res[1].stored >= metal) {
        owner->res[0].stored -= energy;
        res[0].used += energy;
        if (owner->res[1].stored >= metal) {
            owner->res[1].stored -= metal;
            res[1].used += metal;
        }
        return 1;
    }
    return 0;
}

void __stdcall FUN_00401320(Res_00401360* r, float ratioBacklog, float ratioDemand)
{
    r->lastUsed = r->used;
    r->lastProduced = r->produced;
    r->used = 0;
    r->produced = 0;
    r->backlog = r->backlog - ratioBacklog * r->backlog;
    r->backlog += r->demand - ratioDemand * r->demand;
    r->demand = 0;
}

static inline void AddIncome(Unit* u, float* dst, float v)
{
    Player_00401360* o = u->econ.owner;
    if (o->active && o->type == 2) {
        switch (g_game->difficulty) {
        case 0:
            *dst += v * 0.5;
            return;
        case 1:
            *dst += v * 0.7;
            return;
        default:
            *dst += v;
            return;
        }
    }
    *dst += v;
}

static inline void AddIncomeD(Unit* u, float* dst, double v)
{
    Player_00401360* o = u->econ.owner;
    if (o->active && o->type == 2) {
        switch (g_game->difficulty) {
        case 0:
            *dst += v * 0.5;
            return;
        case 1:
            *dst += v * 0.7;
            return;
        default:
            *dst += v;
            return;
        }
    }
    *dst += v;
}

// The 0x4237d0 layout (break and else) with a double amount: tidal and the
// else branch (see the top).
static inline void AddIncomeDB(Unit* u, float* dst, double v)
{
    Player_00401360* o = u->econ.owner;
    if (o->active && o->type == 2) {
        switch (g_game->difficulty) {
        case 0:
            *dst += v * 0.5;
            break;
        case 1:
            *dst += v * 0.7;
            break;
        default:
            *dst += v;
            break;
        }
    } else
        *dst += v;
}

static inline void AddIncomeW(Unit* u, float* dst, double v)
{
    Player_00401360* o = u->econ.owner;
    if (o->active && o->type == 2) {
        switch (g_game->difficulty) {
        case 0:
            *dst += v * 0.5;
            return;
        case 1:
            *dst += v * 0.7;
            return;
        default:
            *dst += (float)v;
            return;
        }
    }
    *dst += (float)v;
}

static int Use_00401180(Unit* u, float amount)
{
    float* used = &u->econ.res[0].used;
    *used += amount;
    if (u->econ.res[0].backlog > 0.0f)
        return 0;
    u->econ.res[0].demand += amount;
    return 1;
}

static int UseEnergy(Unit* u, float v)
{
    if (v >= 0)
        return Use_00401180(u, v);
    AddIncome(u, &u->econ.res[0].produced, -v);
    return 0;
}

// The demand add goes through a double copy of the amount: it makes the
// backlog > 0 pop shared with the default add's.
static int UseEnergyD(Unit* u, float v)
{
    if (v >= 0) {
        double a = v;
        float* used = &u->econ.res[0].used;
        *used += v;
        if (u->econ.res[0].backlog > 0.0f)
            return 0;
        u->econ.res[0].demand += (float)a;
        return 1;
    }
    AddIncomeDB(u, &u->econ.res[0].produced, -v);
    return 0;
}

// FUNCTION: 0x401360
void __stdcall UpdatePlayerEconomy(Player_00401360* p)
{
    float usedA[2];
    float backlogA[2];
    float demandA[2];
    float producedA[2];
    float ratioA[2];
    float avail[2];
    float demandRatio[2];
    Unit* u;
    int i;

    p->storage[1] = 0;
    p->storage[0] = 0;
    producedA[0] = 0;
    producedA[1] = 0;
    usedA[0] = 0;
    usedA[1] = 0;
    demandA[0] = 0;
    demandA[1] = 0;
    backlogA[0] = 0;
    backlogA[1] = 0;
    for (u = p->units; u <= p->units_end; u++) {
        if (!(u->flags & 0x10000000))
            continue;
        if (u->flags & 0x20000000) {
            if (u->flags10e & 1) {
                int ok = UseEnergy(u, u->def->energyUse);
                if (u->def->extractsMetal > 0) {
                    if (ok)
                        AddIncome(u, &u->econ.res[1].produced, u->extraction);
                } else if (u->def->makesMetal) {
                    if (ok)
                        AddIncome(u, &u->econ.res[1].produced, u->def->makesMetal);
                } else if (u->def->windGenerator > 0) {
                    AddIncomeW(u, &u->econ.res[0].produced, g_game->wind * u->def->windGenerator);
                } else if (u->def->tidalGenerator > 0) {
                    AddIncomeDB(u, &u->econ.res[0].produced, g_game->tidal * u->def->tidalGenerator);
                }
            }
        } else if ((u->flags10e & 1) || (u->flags & 0xc) > 0) {
            UseEnergyD(u, u->def->energyUse);
        }
        if (u->buildLeft == 0) {
            AddIncomeD(u, &u->econ.res[0].produced, u->def->energyMake);
            AddIncome(u, &u->econ.res[1].produced, u->def->metalMake);
            p->storage[1] += u->def->metalStorage;
            p->storage[0] += u->def->energyStorage;
        }
        if (!(p->active && p->type == 3)) {
            if (u->bit11) {
                if (!(u->flags & 0x1000) && u->nextTick <= g_game->ticks) {
                    int cost = (int)((u->flags & 0xc) > 0 ? u->def->costActive : u->def->cost);
                    ((Unit*)u)->SetStateBits(4, u->econ.SpendEnergy(cost));
                } else
                    ((Unit*)u)->SetStateBits(4, 0);
            } else
                ((Unit*)u)->SetStateBits(4, 0);
        }
        producedA[0] += u->econ.res[0].produced;
        usedA[0] += u->econ.res[0].used;
        demandA[0] += u->econ.res[0].demand;
        backlogA[0] += u->econ.res[0].backlog;
        producedA[1] += u->econ.res[1].produced;
        usedA[1] += u->econ.res[1].used;
        demandA[1] += u->econ.res[1].demand;
        backlogA[1] += u->econ.res[1].backlog;
    }
    Econ_00401360* e = p->econ;
    producedA[0] += e->res[0].produced;
    usedA[0] += e->res[0].used;
    demandA[0] += e->res[0].demand;
    backlogA[0] += e->res[0].backlog;
    producedA[1] += e->res[1].produced;
    usedA[1] += e->res[1].used;
    demandA[1] += e->res[1].demand;
    backlogA[1] += e->res[1].backlog;
    if (p->flags149 & 1) {
        p->storage[0] += p->storageBonus[0];
        p->storage[1] += p->storageBonus[1];
    }
    p->res[0].produced = producedA[0];
    p->res[0].used = usedA[0];
    p->totalProduced[0] += producedA[0];
    p->totalUsed[0] += usedA[0];
    p->res[1].produced = producedA[1];
    p->res[1].used = usedA[1];
    p->totalProduced[1] += producedA[1];
    p->totalUsed[1] += usedA[1];
    avail[0] = producedA[0] + p->res[0].stored;
    avail[1] = producedA[1] + p->res[1].stored;
    for (i = 0; i < 2; i++) {
        float take;
        if (backlogA[i] <= avail[i]) {
            take = backlogA[i];
            ratioA[i] = 1.0f;
        } else {
            take = avail[i];
            ratioA[i] = avail[i] / backlogA[i];
        }
        avail[i] -= take;
        float left = avail[i];
        if (demandA[i] <= left) {
            take = demandA[i];
            demandRatio[i] = 1.0f;
        } else {
            take = left;
            demandRatio[i] = left / demandA[i];
        }
        avail[i] = left - take;
    }
    p->res[0].stored = avail[0];
    if (avail[0] > p->storage[0]) {
        p->res[0].stored = p->storage[0];
        p->totalExcess[0] += avail[0] - p->storage[0];
    }
    p->res[1].stored = avail[1];
    if (avail[1] > p->storage[1]) {
        p->res[1].stored = p->storage[1];
        p->totalExcess[1] += avail[1] - p->storage[1];
    }
    for (u = p->units; u <= p->units_end; u++) {
        if (u->flags & 0x10000000) {
            FUN_00401320(&u->econ.res[0], ratioA[0], demandRatio[0]);
            FUN_00401320(&u->econ.res[1], ratioA[1], demandRatio[1]);
        }
    }
    FUN_00401320(&p->econ->res[0], ratioA[0], demandRatio[0]);
    FUN_00401320(&p->econ->res[1], ratioA[1], demandRatio[1]);
}
