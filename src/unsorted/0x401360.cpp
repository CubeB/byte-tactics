// Decompiled by Claude Opus 5.5, finished by GPT-6, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash: 77.2% (was 74.4%). Two source changes, both in the statement split/order of an
// inlined helper. (1) Writing EndTick's backlog update as TWO statements
// (`r->backlog -= rBacklog * r->backlog;` then `r->backlog += r->demand - rDemand * r->demand;`)
// instead of one parenthesised expression fixed the two inlined unit-loop EndTick bodies exactly
// (74.4 -> 76.8). (2) Ordering EndTick's two save-then-zero pairs FIRST
// (`lastUsed = used; lastProduced = produced; used = 0; produced = 0;` before the backlog math)
// fixed the player-econ EndTick body too (76.8 -> 77.2). The single parenthesised expression and the
// `save/zero/save/zero` interleave both scheduled their fsubr/faddp/stores wrong.
// Correcting deepseek-v4.1's note: the two EndTick summands must stay backlog-first; reversing them
// (v_e1) is byte-identical to the single-expression form at 74.4, and splitting the update demand-first
// changes nothing. Also re-tried and confirmed: pure-indexing normalization is 62.2 (keep `have`);
// `have[0]` instead of `*have` is 74.4; swapping the two EndTick save statements (`lastProduced` before
// `lastUsed`) is 73.1; an explicit `int ret; return ret;` UseEnergy body and an `unsigned char` return
// are both 77.2, no better.
// Still differs at 77.2% (ours 2154 vs original 2239 bytes): UseEnergy's inlined `used += v` store and
// its `backlog <= 0` test are still scheduled in the opposite order (ours loads backlog first); the
// original materialises the inlined helper's result in eax then copies to edx (`mov eax,1; mov edx,eax`),
// ours keeps it in edx; the normalization loop's register roles are mirrored (original: ecx = i byte
// offset, edx = &produced[i] with `[esp+ecx+off]` addressing; ours: ecx = &produced[i], edx = countdown,
// with pointer-relative `[ecx+off]` addressing, so ours is ~7 bytes/iteration short) and its second
// compare keeps the running produced value on the x87 stack where ours spills it with `fst`.
// Partial: 74.4% (was 70.6%). deepseek-v4.1: the "array shape" is real but is NOT a declaration or
// zero-init order lever: sweeping the declaration order of the four float[2] accumulators (and of
// their inits) leaves the frame slots untouched. Wrapping all five arrays (used, backlog, demand,
// produced, ratio) in ONE struct laid out in that member order puts them at esp+0x10/0x18/0x20/0x28/
// 0x30, exactly the original, and the whole prologue plus init then matches (70.6 -> 70.7, and it
// unmasks the statement order below). Then reordering the econ-merge block to produced, used,
// demand, backlog (ascending unit field offsets) took it to 74.4. Rewriting the normalization loop
// without the `float* have` pointer (pure indexing) drops it to 62.4, so keep `have`.
// Still differs: the loop-tail accumulation behind 0x401877 and the tail EndTick block schedule
// their x87 loads/fxch/stores differently (ours 2158 vs 2239 bytes), UseEnergy materialises its
// result in eax then copies to edx in the original, and ours hoists the backlog compare above the
// `used += v` store.
// Tried by deepseek-v4.1 (no effect, all still 2172 bytes / 70.6): swapping the declaration order of the
// accumulator arrays (used/backlog/demand/produced), swapping their zero-init order, making UseEnergy
// __inline or giving it a single `int r; return r;` body, and reversing the two summands in EndTick's
// backlog expression. Rewriting AddIncome's tail as `*dst = *dst + v;` or `*dst = v + *dst;` also changes
// nothing (the compiler always folds to `fadd dword ptr [dst]`), while the original has the 2-instruction
// `fld dword ptr [dst]; fadd st(1)` form at some of those sites. So the slot layout (orig used@0x10, backlog@0x18, demand@0x20, produced@0x28;
// ours produced@0x10, used@0x18, backlog@0x20, demand@0x28) is not a declaration/init-order lever.
// Still differs: the original materialises the helper result in eax (`mov eax,1; mov edx,eax` / `xor eax,eax; ...; mov edx,eax`) as an inlined callee return, ours assigns edx directly; ours also hoists the backlog compare above the `used += v` store. Ours is 67 bytes shorter (2172 vs 2239). In the tail the demand-ratio argument to EndTick is read from [esp+0x10] in the original while ours reads a different array slot (0x18/0x14 order swaps in the accumulation block at ~0x401889).
// deepseek-v4.1-flash run 3 (10 min box, 1 check run): re-read the full 2239-byte original listing against the
// diff. The whole surviving difference is upstream of the byte-count gap, in the two inlined UseEnergy copies:
// the original keeps the energyUse value (fchs'ed to -v in the negative arm) LIVE on the x87 stack for the
// whole inlined body and pops it once in the shared tail block at 0x4016c3, so its plain AddIncome tail is
// `fld [dst]; fadd st(1); fstp [dst]` (non-popping fadd) only where that leftover must survive; the first
// copy, whose value is consumed, has the folded `fadd [dst]` at 0x401472 instead. Ours never keeps v live,
// which both drops the pop and lets the backlog fcomp sink in front of the `used += v` fstp. No source shape
// reached that in this box; file left at the run-2 best.
// deepseek-v4.1-flash run 2 (900s): baseline 77.2 confirmed. Tried and rejected, all 77.2 or worse:
// normalization loop with indexed compares and `have` only for the subtracts (77.1); a `float& have`
// reference; `*have = *have - take`; `for (i = 0; i != 2; i++)`; `take` declared before `have`;
// `have = acc.produced + i`; UseEnergy with direct `return 1`/`return 0` (the single `int ret` body is
// kept); UseEnergy returning bool (64.7). The N-declarations sweep (0..400 `extern int`s) is flat at
// 77.2 to N=176 then 76.8, so this is not compiler state.
// The normalization loop's induction variable is the one clear lever left: every rewrite
// strength-reduces `have` to the induction pointer (ecx) plus a countdown, where the original keeps
// a 4-byte stride index in ecx and computes `&produced[i]` once per iteration with `lea edx,[esp+ecx+0x28]`.
#include <ddraw.h>
struct Unit_00401360;

class Class_0048b090 {
public:
    void FUN_0048b090(int which, int on);
};

#pragma pack(push, 1)
struct Acc_00401360 {
    float used[2];
    float backlog[2];
    float demand[2];
    float produced[2];
    float ratio[2];
};

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

static int UseEnergy(Unit_00401360* u, float v)
{
    int ret;
    if (v >= 0) {
        u->econ.res[0].used += v;
        if (u->econ.res[0].backlog <= 0) {
            u->econ.res[0].demand += v;
            ret = 1;
        } else {
            ret = 0;
        }
    } else {
        AddIncome(u, &u->econ.res[0].produced, -v);
        ret = 0;
    }
    return ret;
}

static inline void EndTick(Res_00401360* r, float ratioDemand, float ratioBacklog)
{
    r->lastUsed = r->used;
    r->lastProduced = r->produced;
    r->used = 0;
    r->produced = 0;
    r->backlog = r->backlog - ratioBacklog * r->backlog;
    r->backlog += r->demand - ratioDemand * r->demand;
    r->demand = 0;
}

// FUNCTION: 0x401360
void __stdcall FUN_00401360(Player_00401360* p)
{
    Acc_00401360 acc;
    Unit_00401360* u;
    int i;

    p->storage[1] = 0;
    p->storage[0] = 0;
    acc.produced[0] = 0;
    acc.produced[1] = 0;
    acc.used[0] = 0;
    acc.used[1] = 0;
    acc.demand[0] = 0;
    acc.demand[1] = 0;
    acc.backlog[0] = 0;
    acc.backlog[1] = 0;
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
        acc.produced[0] += u->econ.res[0].produced;
        acc.used[0] += u->econ.res[0].used;
        acc.demand[0] += u->econ.res[0].demand;
        acc.backlog[0] += u->econ.res[0].backlog;
        acc.produced[1] += u->econ.res[1].produced;
        acc.used[1] += u->econ.res[1].used;
        acc.demand[1] += u->econ.res[1].demand;
        acc.backlog[1] += u->econ.res[1].backlog;
    }
    Econ_00401360* e = p->econ;
    acc.produced[0] += e->res[0].produced;
    acc.used[0] += e->res[0].used;
    acc.demand[0] += e->res[0].demand;
    acc.backlog[0] += e->res[0].backlog;
    acc.produced[1] += e->res[1].produced;
    acc.used[1] += e->res[1].used;
    acc.demand[1] += e->res[1].demand;
    acc.backlog[1] += e->res[1].backlog;
    if (p->flags149 & 1) {
        p->storage[0] += p->storageBonus[0];
        p->storage[1] += p->storageBonus[1];
    }
    p->res[0].produced = acc.produced[0];
    p->res[0].used = acc.used[0];
    p->totalProduced[0] += acc.produced[0];
    p->totalUsed[0] += acc.used[0];
    p->res[1].produced = acc.produced[1];
    p->res[1].used = acc.used[1];
    p->totalProduced[1] += acc.produced[1];
    p->totalUsed[1] += acc.used[1];
    acc.produced[0] += p->res[0].stored;
    acc.produced[1] += p->res[1].stored;
    for (i = 0; i < 2; i++) {
        float* have = &acc.produced[i];
        float take;
        if (acc.backlog[i] <= *have) {
            take = acc.backlog[i];
            acc.ratio[i] = 1.0f;
        } else {
            take = *have;
            acc.ratio[i] = *have / acc.backlog[i];
        }
        *have -= take;
        if (acc.demand[i] <= *have) {
            take = acc.demand[i];
            acc.used[i] = 1.0f;
        } else {
            take = *have;
            acc.used[i] = *have / acc.demand[i];
        }
        *have -= take;
    }
    p->res[0].stored = acc.produced[0];
    if (acc.produced[0] > p->storage[0]) {
        p->res[0].stored = p->storage[0];
        p->totalExcess[0] += acc.produced[0] - p->storage[0];
    }
    p->res[1].stored = acc.produced[1];
    if (acc.produced[1] > p->storage[1]) {
        p->res[1].stored = p->storage[1];
        p->totalExcess[1] += acc.produced[1] - p->storage[1];
    }
    for (u = p->units; u <= p->units_end; u++) {
        if (u->flags & 0x10000000) {
            EndTick(&u->econ.res[0], acc.used[0], acc.ratio[0]);
            EndTick(&u->econ.res[1], acc.used[1], acc.ratio[1]);
        }
    }
    EndTick(&p->econ->res[0], acc.used[0], acc.ratio[0]);
    EndTick(&p->econ->res[1], acc.used[1], acc.ratio[1]);
}
