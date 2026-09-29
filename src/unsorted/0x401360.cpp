// Decompiled by Claude Opus 5.5, finished by GPT-6. Names are provisional.
// Partial: 68.4%. Income branches and x87 scheduling still differ. Reordered resource resets and accumulation; ddraw.h improves compiler state.
#include <ddraw.h>
struct Unit_00401360;

class Class_0048b090 {
public:
    void FUN_0048b090(int which, int on);
};

#pragma pack(push, 1)
struct Res_00401360 {
    float produced;                    // +0x0
    float used;                        // +0x4
    float demand;                      // +0x8
    float backlog;                     // +0xc
    float lastProduced;                // +0x10
    float lastUsed;                    // +0x14
};

struct Econ_00401360 {
    Res_00401360 res[2];               // energy, metal
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
    Unit_00401360* units;              // +0x67
    Unit_00401360* units_end;          // +0x6b
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

struct Unit_00401360 {
    char unknown_0[0x58];
    float extraction;                  // +0x58
    char unknown_5c[0x92 - 0x5c];
    UnitDef_00401360* def;             // +0x92
    char unknown_96[0xb0 - 0x96];
    unsigned int nextTick;             // +0xb0
    char unknown_b4[0xbc - 0xb4];
    Econ_00401360 econ;                // +0xbc
    Player_00401360* owner;            // +0xec
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
};

struct Game_00401360 {
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

extern Game_00401360* g_game;

static inline void AddIncome(Unit_00401360* u, float* dst, float v)
{
    Player_00401360* o = u->owner;
    if (o->active && o->type == 2) {
        switch (g_game->difficulty) {
        case 0:
            *dst += v * 0.5;
            return;
        case 1:
            *dst += v * 0.7;
            return;
        }
    }
    *dst += v;
}

static inline int UseEnergy(Unit_00401360* u, float v)
{
    if (v < 0) {
        AddIncome(u, &u->econ.res[0].produced, -v);
        return 0;
    }
    u->econ.res[0].used += v;
    if (u->econ.res[0].backlog > 0) return 0;
    u->econ.res[0].demand += v;
    return 1;
}

static inline void EndTick(Res_00401360* r, float ratioDemand, float ratioBacklog)
{
    r->lastUsed = r->used;
    r->used = 0;
    r->lastProduced = r->produced;
    r->produced = 0;
    r->backlog = (r->demand - ratioDemand * r->demand) + (r->backlog - ratioBacklog * r->backlog);
    r->demand = 0;
}

// FUNCTION: 0x401360
void __stdcall FUN_00401360(Player_00401360* p)
{
    float used[2];
    float backlog[2];
    float demand[2];
    float produced[2];
    float ratio[2];
    Unit_00401360* u;
    int i;

    p->storage[1] = 0;
    p->storage[0] = 0;
    demand[0] = 0;
    demand[1] = 0;
    produced[0] = 0;
    produced[1] = 0;
    backlog[0] = 0;
    backlog[1] = 0;
    used[0] = 0;
    used[1] = 0;
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
                    AddIncome(u, &u->econ.res[0].produced, g_game->wind * u->def->windGenerator);
                } else if (u->def->tidalGenerator > 0) {
                    AddIncome(u, &u->econ.res[0].produced, g_game->tidal * u->def->tidalGenerator);
                }
            }
        } else if ((u->flags10e & 1) || (u->flags & 0xc) > 0) {
            UseEnergy(u, u->def->energyUse);
        }
        if (u->buildLeft == 0) {
            AddIncome(u, &u->econ.res[0].produced, u->def->energyMake);
            AddIncome(u, &u->econ.res[1].produced, u->def->metalMake);
            p->storage[1] += u->def->metalStorage;
            p->storage[0] += u->def->energyStorage;
        }
        if (!(p->active && p->type == 3)) {
            if (u->bit11) {
                if (!(u->flags & 0x1000) && u->nextTick <= g_game->ticks) {
                    int cost = (int)((u->flags & 0xc) > 0 ? u->def->costActive : u->def->cost);
                    Player_00401360* o = u->owner;
                    int ok;
                    if (cost <= o->res[0].stored) {
                        o->res[0].stored -= cost;
                        u->econ.res[0].used += cost;
                        ok = 1;
                    } else
                        ok = 0;
                    ((Class_0048b090*)u)->FUN_0048b090(4, ok);
                } else
                    ((Class_0048b090*)u)->FUN_0048b090(4, 0);
            } else
                ((Class_0048b090*)u)->FUN_0048b090(4, 0);
        }
        produced[0] += u->econ.res[0].produced;
        used[0] += u->econ.res[0].used;
        demand[0] += u->econ.res[0].demand;
        backlog[0] += u->econ.res[0].backlog;
        produced[1] += u->econ.res[1].produced;
        used[1] += u->econ.res[1].used;
        demand[1] += u->econ.res[1].demand;
        backlog[1] += u->econ.res[1].backlog;
    }
    Econ_00401360* e = p->econ;
    demand[0] += e->res[0].demand;
    produced[0] += e->res[0].produced;
    backlog[0] += e->res[0].backlog;
    used[0] += e->res[0].used;
    demand[1] += e->res[1].demand;
    produced[1] += e->res[1].produced;
    backlog[1] += e->res[1].backlog;
    used[1] += e->res[1].used;
    if (p->flags149 & 1) {
        p->storage[0] += p->storageBonus[0];
        p->storage[1] += p->storageBonus[1];
    }
    p->res[0].produced = produced[0];
    p->res[0].used = used[0];
    p->totalProduced[0] += produced[0];
    p->totalUsed[0] += used[0];
    p->res[1].produced = produced[1];
    p->res[1].used = used[1];
    p->totalProduced[1] += produced[1];
    p->totalUsed[1] += used[1];
    produced[0] += p->res[0].stored;
    produced[1] += p->res[1].stored;
    for (i = 0; i < 2; i++) {
        float* have = &produced[i];
        float take;
        if (backlog[i] <= *have) {
            take = backlog[i];
            ratio[i] = 1.0f;
        } else {
            take = *have;
            ratio[i] = *have / backlog[i];
        }
        *have -= take;
        if (demand[i] <= *have) {
            take = demand[i];
            used[i] = 1.0f;
        } else {
            take = *have;
            used[i] = *have / demand[i];
        }
        *have -= take;
    }
    p->res[0].stored = produced[0];
    if (produced[0] > p->storage[0]) {
        p->res[0].stored = p->storage[0];
        p->totalExcess[0] += produced[0] - p->storage[0];
    }
    p->res[1].stored = produced[1];
    if (produced[1] > p->storage[1]) {
        p->res[1].stored = p->storage[1];
        p->totalExcess[1] += produced[1] - p->storage[1];
    }
    for (u = p->units; u <= p->units_end; u++) {
        if (u->flags & 0x10000000) {
            EndTick(&u->econ.res[0], used[0], ratio[0]);
            EndTick(&u->econ.res[1], used[1], ratio[1]);
        }
    }
    EndTick(&p->econ->res[0], used[0], ratio[0]);
    EndTick(&p->econ->res[1], used[1], ratio[1]);
}
