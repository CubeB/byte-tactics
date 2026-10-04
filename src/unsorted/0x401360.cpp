// Decompiled by Claude Opus 5.5, finished by GPT-6, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// Per-tick economy update for one player: every unit's energy/metal use and
// production is summed, the player's totals and storage are updated, and the
// share of the demand that could be met is fed back into every account.
//
// Partial, 89.0% (2249 of 2239 bytes; was 88.1%, before that 77.2%).
//
// Opus pass on 2026-10-04 (88.1% to 89.0%). First (88.9%): the wind site now calls
// AddIncomeW, which takes a double like AddIncomeD but adds the default and
// non-AI amounts as `(float)v`. That keeps wind's own `fadd [m]; fstp [m]`
// default blocks and lets wind's case blocks merge whole into the else
// branch's, as in the original. C2's cross-jumper (FUN_00432f03, once per
// label; FUN_00446590 merges a pair) compares tuples, not bytes, so the case
// blocks of a float-parameter helper never merge with a double-parameter
// helper's even though the code is identical. AddIncomeW can replace
// AddIncome at every other site with the same bytes. What is left, found by
// adding gdb hooks to a copy of tools/c2prio.py: 0x432f03 (ecx = label),
// 0x446590 (ecx, edx = the two jumps; it reaches 0x446683 when it merges),
// 0x40d397 (attaches a jump to a label) and 0x42f3aa (the block move in
// FUN_0042f060). In a tuple, +8 is the kind (0x10 jmp, 0x19 label), +0x10
// the line counted from the function's first line, and +0x12 its position.
// - FUN_00432f03 pairs the first jump in the join label's predecessor list
//   with each later one. A partial merge needs more than 20 bytes, the jump
//   included. A merge that covers a whole block (an unconditional jump
//   before it) needs no minimum. The list is LIFO (FUN_0040d397 prepends) and
//   is built before register allocation.
// - Here the order is En(else non-AI), T0, T1, Td, ..., Ed, E1, E0, Tn, Ep.
//   Td and then Ed merge into En. Then Tn matches 22 instructions backwards:
//   tidal's whole default/case/dispatch region against the else's. That is
//   the merged tidal dispatch. Without the `done:` label the order has Tn
//   before Ed: the match stops after 16 instructions and gives the original's
//   tidal (cases and default merged, dispatch kept). But then FUN_0042f060
//   (block placement) moves En up behind tidal's `jmp En`, because the block
//   before En ends in a jump, and the else branch ends up after the epilogue
//   (81.9%). An unreferenced label at the join keeps the else in place too
//   (88.9%, same bytes), but an empty statement, block or do/while does not.
// - Then (89.0%): tidal calls AddIncomeD3, with no `default:` case (the
//   switch falls out to the shared `*dst += v`). Tidal's dispatch is no
//   longer merged into the else's. But tidal's c1 is now a `fmul; jmp X`
//   block after its dispatch (`jne En`), where the original has
//   `je E1; jmp En`. That costs 10 bytes. The same change for the else
//   branch moves it after the epilogue (81.9%), and for energyMake it changes
//   nothing.
// - The else branch's default still keeps its own `fstp st(0); jmp` instead
//   of falling into the pop it shares with the backlog > 0 path. That pop
//   block comes after En, and only a whole-block merge into En's tail
//   (POP before En at cross-jump time) would give the original's shape.
// Tried without effect here: case order, nonAI-first and nested AI tests
// (all canonicalised), break/else helpers (72%), goto/labels in the then or
// else arms, block-scoped and value spellings of UseEnergyD, /Gi (69.8%).
//
// Opus pass on 2026-10-04 (no gain, about 2,000 variants scored):
// - Tuple-level view: hook FUN_00432f03 and walk C2's tuple list (+0 next,
//   +0xc prev, +4 opcode: 0x10 jmp, 0xf jcc, 0x60 fld, 0x63 fstp; +8 kind;
//   +0x10 line; +0x12 position). FUN_00446590(ecx = a, edx = b) keeps b and
//   replaces a's matched tail with a jump into b; the 20-byte minimum is
//   waived when the tuple before a's matched run is an unconditional jump.
// - The backlog > 0 pop is a block made by C2's edge splitter (0x440885; it
//   retargets the jcc through FUN_0040d3bc from 0x44096c) just before the
//   join, after En already ends in its own pop. En's `jmp` is created then,
//   which is why En heads the join's list.
// - The pop is shared (En falls into it, as in the original) only when the
//   demand add is non-consuming too: a `double v` with `*used += v;
//   if (backlog <= 0) demand += v;` in place (wrong positive path). A double v
//   with `(float)v` casts gives the original positive path exactly except
//   `fcom qword`, but the pop is separate again. Separate locals, scopes
//   (block, loop, function), in-place negation, CSE of def->energyUse and
//   every spelling of the default add keep it separate: a float value's last
//   add always consumes it, a double one never does.
// - Per-branch exits (the lever that matched 0x4dea00 and 0x40e630) change
//   nothing: `goto done` at arm ends, in each AddIncome case or in the demand
//   path is threaded away before the merge. In-place AI blocks in the
//   0x4237d0 style (float or double local) give exactly the helpers' code.
// - With AddIncomeD for tidal and no label the merges give the original's
//   tidal (Td, then Tn before Ed); block placement then moves En because it
//   ends in a jump. If En fell into the shared pop it could not move, so both
//   differences probably come from how the original reaches the join with
//   the dead value still on the x87 stack.
//
// What got it to 88.1% (earlier passes):
// - The accumulators are separate float[2] arrays, not one struct: with one
//   aggregate MSVC strength-reduces the normalisation loop to a pointer and a
//   countdown, the original keeps `i * 4` in ecx ([esp+ecx+N]). The frame
//   order then comes out right only with two arrays split off into their own
//   variables: `avail` (production plus stock, shares produced's slot) and
//   `demandRatio` (shares used's slot). c2prio --frame: used 14, backlog 13,
//   demand 13, produced 12 (+avail 9), ratio 6.
// - `avail[i] -= take; float left = avail[i];` keeps the remaining amount on
//   the x87 stack for the second half of the loop, as the original does.
// - The unit's resource account is a class at +0xbc whose owner pointer is at
//   +0x30 (unit+0xec); 0x401180..0x4012a0 are its methods and 0x401320 is the
//   end-of-tick update, all defined above without FUNCTION lines (each
//   matches its own original in this file except 0x401320, 66.7%: the two
//   products swap, which <stdlib.h> fixes in 0x401320.cpp but breaks here).
//   The cost block is FUN_00401220 inlined.
// - UseEnergy's positive arm is a helper taking the unit (a pointer argument
//   gives a strength-reduced unit+0xc0 pointer) whose `used` store goes
//   through a float* (otherwise the backlog compare is scheduled above it);
//   the helper's return then comes out as `mov eax, 1; mov edx, eax`.
// - Default and non-AI income adds: a float amount adds straight to the field
//   (`fadd [m]`), a double one keeps the amount and pops it
//   (`fld [m]; fadd st(1); fstp [m]; fstp st(0)`, as in 0x4237d0). The
//   original has the second form for tidal, energyMake and the else-branch
//   UseEnergy, the first everywhere else, hence AddIncomeD/UseEnergyD.
// - With tidal and the else branch both on the double helper MSVC moves the
//   whole else branch after the epilogue (81.6%) unless something ends the
//   if/else chain with its own join: the `done:` label (or a
//   `do { } while (0)` around the chain; `for`/`while` wrappers do not work).
//
// Differences at 88.1% (all tail merging in the wind/tidal/else-branch block;
// the first is fixed above):
// - wind's case blocks keep their own `fmul; jmp` where the original jumps
//   straight into the else branch's;
// - tidal's switch dispatch is merged into the else branch's, where the
//   original keeps `je c0; dec; je c1; jmp K` (its own default block);
// - the else branch's default add keeps its own `fstp st(0); jmp` where the
//   original falls into the pop shared with the backlog > 0 path.
// Tried without effect: header and declaration-count sweeps (windows-class
// headers are best, flat otherwise), the helpers' parameter order, every
// spelling of the gate, return and negation in UseEnergyD, case order and
// break/else forms of AddIncomeD (the 0x4237d0 spelling drops to 71.6% by
// flipping unrelated fadd operand orders), extra labels elsewhere, loop
// wrappers around the function or the unit loop, and a 15-minute permuter run.
// Without the label and with tidal/else on the float helper the same file is
// 87.7% and still has the else branch in place.
#include <ddraw.h>
struct Unit_00401360;
struct Player_00401360;

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

// The unit's (and the player's) resource accounts: energy, then metal.
class Econ_00401360 {
public:
    Res_00401360 res[2];               // +0x0
    Player_00401360* owner;            // +0x30
    int FUN_00401180(Econ_00401360* e, float amount);
    int FUN_004011c0(float energy, float metal);
    int FUN_00401220(float amount);
    int FUN_00401260(float amount);
    int FUN_004012a0(float energy, float metal);
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

int Econ_00401360::FUN_00401220(float amount)
{
    if (owner->res[0].stored >= amount) {
        owner->res[0].stored -= amount;
        res[0].used += amount;
        return 1;
    }
    return 0;
}

int Econ_00401360::FUN_00401260(float amount)
{
    if (owner->res[1].stored >= amount) {
        owner->res[1].stored -= amount;
        res[1].used += amount;
        return 1;
    }
    return 0;
}

int Econ_00401360::FUN_004012a0(float energy, float metal)
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

static inline void AddIncome(Unit_00401360* u, float* dst, float v)
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

static inline void AddIncomeD(Unit_00401360* u, float* dst, double v)
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

static inline void AddIncomeD3(Unit_00401360* u, float* dst, double v)
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
        }
    }
    *dst += v;
}

static inline void AddIncomeW(Unit_00401360* u, float* dst, double v)
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

static int Use_00401180(Unit_00401360* u, float amount)
{
    float* used = &u->econ.res[0].used;
    *used += amount;
    if (u->econ.res[0].backlog > 0.0f)
        return 0;
    u->econ.res[0].demand += amount;
    return 1;
}

static int UseEnergy(Unit_00401360* u, float v)
{
    if (v >= 0)
        return Use_00401180(u, v);
    AddIncome(u, &u->econ.res[0].produced, -v);
    return 0;
}

static int UseEnergyD(Unit_00401360* u, float v)
{
    if (v >= 0)
        return Use_00401180(u, v);
    AddIncomeD(u, &u->econ.res[0].produced, -v);
    return 0;
}

// FUNCTION: 0x401360
void __stdcall FUN_00401360(Player_00401360* p)
{
    float usedA[2];
    float backlogA[2];
    float demandA[2];
    float producedA[2];
    float ratioA[2];
    float avail[2];
    float demandRatio[2];
    Unit_00401360* u;
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
                    if (!ok)
                        goto done;
                    AddIncome(u, &u->econ.res[1].produced, u->extraction);
                } else if (u->def->makesMetal) {
                    if (!ok)
                        goto done;
                    AddIncome(u, &u->econ.res[1].produced, u->def->makesMetal);
                } else if (u->def->windGenerator > 0) {
                    AddIncomeW(u, &u->econ.res[0].produced, g_game->wind * u->def->windGenerator);
                } else if (u->def->tidalGenerator > 0) {
                    AddIncomeD3(u, &u->econ.res[0].produced, g_game->tidal * u->def->tidalGenerator);
                }
            }
        } else if ((u->flags10e & 1) || (u->flags & 0xc) > 0) {
            UseEnergyD(u, u->def->energyUse);
        }
        // This label's join keeps the else branch in place (see the top).
    done:
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
                    ((Class_0048b090*)u)->FUN_0048b090(4, u->econ.FUN_00401220(cost));
                } else
                    ((Class_0048b090*)u)->FUN_0048b090(4, 0);
            } else
                ((Class_0048b090*)u)->FUN_0048b090(4, 0);
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
