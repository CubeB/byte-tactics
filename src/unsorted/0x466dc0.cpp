// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by
// deepseek-v4.1, GPT-6.1-sol, finished by deepseek-v4.1-flash,
// finished by mimo-v2.6-pro. Names are provisional.
// mimo-v2.6-pro 2026-10-01: 78.3 -> 99.6 percent (1662 bytes, exactly the
// original's size). Two things fixed almost everything:
//   1. OnRadar reshaped: each arm declares tx/ty locals and uses the
//      MapSize::Contains inline method (the matched 0x408090 spelling).
//      That gives the original's destructive `sar ebp, 5; sar edi, 5`, the
//      inline `cmp edi, [edx+0x84]` height compare and the width reload
//      `mov ecx, [edx+0x80]` in the multiply. The byte arm is
//      `if (cond) b = 1; else b = 0;`, the short arm is
//      `if (!Contains) b = 0; else b = expr != 0;` (early-out shape with the
//      zero block between checks and compute, as in 0x408090). The two arms
//      are separate inline helpers taking the PlayerInfo* (pi passed in):
//      that stops the tail merger fusing their identical zero blocks.
//      Helpers that compute pi themselves duplicate the player-index chain
//      and lose 15 percent, so pi must be computed before the dispatch.
//   2. All projectile tail reads (small-branch player, big-branch owner,
//      big-branch player) go through the q Tail struct at p+0xa; p is used
//      only for p->shot. That flips the loop register split to the
//      original's: q stays live in ebx across the latch (`add ebx, 0x6b`),
//      p is memory-resident and reloaded at the loop top
//      (`mov ecx, [esp+0x1c]`), and the big-branch copy restores q with
//      `mov ebx, [esp+0x18]` at 0x4673a9. Hypothesis confirmed: the walker
//      used LAST before the draw calls gets ebx. q must also be declared
//      inside the `if (g_game->projectileCount > 0)` block so its lea
//      lands after the guard like the original's.
// Remaining, ONE site only (2 swapped instructions): the unit-loop ScaleX
// multiply. Original: `movsx eax, [ebx+0x6c]` (u->field_6c) then
// `movsx ecx, [esi+0x142eb]` (zoom); ours loads zoom first (into the imul
// accumulator) either way. Byte-neutral, tried this session on the matched
// base: operand swap at the site, single and double (int) casts, a named
// v/temp local, `int x = u->field_6c; x = x * zoom / scale;` accumulation,
// statement split (`int x = a*b; x = x/c;`), ScaleX helper with and without
// the division, a 3-arg Scale(v,z,s) helper both argument orders, a 2-arg
// Mul(a,b) helper both argument orders, a zoom-first parameter helper, and
// `short* pf = &u->field_6c; *pf * zoom`. This is the same unreachable
// scheduler choice documented at 0x47d0e0 ("the multiply's destination
// register ... is a single scheduling choice that no source shape here
// reaches"; sign-extended movsx operands canonicalise and swapping the
// source operands changes nothing). The other multiplies in this function
// all follow the source's left operand into the accumulator; only this
// load*load node canonicalises the g_game-based operand there.
// deepseek-v4.1-flash 2026-10-01 (retry 7, timeboxed): no gain, stays 78.3 /
// 1646 bytes. Eight scratch probes, all <= 78.3: routing every tail read
// (small-branch player, big-branch player, owner) through the q Tail struct
// 77.1 (note: it moves q to slot 0x18 but p to 0x28 and i to 0x1c); q declared
// inside the if 78.1; q declared first as an independent g_game->projectiles
// load with p second 77.8 (1650 bytes, and p becomes fully memory resident,
// reloaded for p->shot, yet q still lands in ecx); p derived from q 74.8;
// a `for` loop with the increments in the for-clause 77.5; typed `p++` instead
// of the char* cast 78.3 (identical bytes); px/py declared before x/y 76.4;
// q built through a named char* qraw 78.3 (identical bytes). So the ebx choice
// is not use count, not declaration order and not the tail-read base: with p
// reduced to a single loop use MSVC still keeps p in ebx and spills q, which
// matches this function's earlier sessions and points at compiler state, not
// at the source spelling. Four more probes after that note: i declared and
// assigned before p 78.1; p/q/i declared and then assigned in two steps 78.1;
// q outside the if with p inside 77.4 (1650 bytes, and neither pointer reaches
// the preheader in ebx there); both walkers typed char* (reads as
// *(short*)(q - 4) and (*(Shot_00466dc0**)p)->flags) 78.3, byte-identical to
// the base, so the pointer type is not the lever either.
// deepseek-v4.1-flash 2026-10-01 (retry 6, timeboxed): no gain, stays 78.3 /
// 1646 bytes. Seven probes this session were flat or negative: q declared
// before p (77.8), q-first declaration with p assigned first (flat), the
// ScaleX site spelled directly as `u->field_6c * (int)g_game->field_142eb`
// (flat), both projectile-loop player reads routed through the q Tail struct
// (77.9). The p/ebx vs q/ebx base swap stands as the only lever; every
// in-loop instruction follows from it.

// deepseek-v4.1-flash 2026-10-01 (retry 5, timeboxed): no new gains, stays at
// the 78.3% / 1646-byte best. A named `int v = u->field_6c;` local in
// ScaleX_00466dc0 is byte-identical (same 7 hunks), so the movsx eax/ecx swap
// is not a materialization-order lever.
// projectile loop keeps p in ebx and q spilled where the original keeps q in
// ebx and reloads p from [esp+0x1c], and the ScaleX multiply loads zoom into
// eax before u->field_6c.

// deepseek-v4.1-flash 2026-10-01 (retry 3): moving the projectile loop's p/q
// declarations inside the `if (g_game->projectileCount > 0)` block regressed
// 78.3 to 78.1 (same 1646 bytes), so the declarations stay above the if.
// deepseek-v4.1-flash 2026-10-01 (retry 2): dropping the (int) casts in
// ScaleX_00466dc0 (`u->field_6c * g_game->field_142eb`) is byte-identical to
// the cast form (78.3%, 1646 bytes, output diff empty), so the eax/ecx swap
// of the two movsx loads is not the cast spelling.
// deepseek-v4.1-flash retry 2026-10-01: ScaleX_00466dc0 operand swap (zoom first) and swapping the projectile-latch update order (q before p) are both byte-neutral (78.3%, 1646 bytes, same 7 hunks), confirming MSVC canonicalises the commutative multiply and the latch order is not the q-in-ebx lever. Restored base.
// deepseek-v4.1-flash worker retry: best stayed 78.3%. Four free --sym scratch
// variants this session all lost: routing both projectile tail reads (player
// and owner) through the q Tail struct while p serves only p->shot (vA) 77.1;
// typing p as Shot_00466dc0** so the only p use is *p (vC) 77.1; the same with
// q declared before p and p assigned first (vD) 77.1; deriving p from q at the
// latch (vE) 76.0 with a shrunken 0x18 frame. The two open sites are unchanged:
// (1) the projectile loop keeps p in ebx and q spilled where the original keeps
// q in ebx and reloads p from [esp+0x1c], and (2) the ScaleX multiply loads
// zoom into eax before u->field_6c where the original loads field_6c first.
// GPT-6.1-sol retry: best stayed 78.3% after a fresh ScaleX local and p-before-q setup; moving i ahead of p/q scored 78.1%, while initializing q directly from g_game->projectiles scored 77.8%. No exact match. Seven checker invocations total, including one initial call with no output.
// PARTIAL 78.3 percent (1646 of 1662 bytes). This session (deepseek-v4.1-flash
// retry) only gained 0.2: declaring the projectile tail pointer as
//     short* q;
//     Projectile_00466dc0* p = g_game->projectiles;
//     q = (short*)((char*)p + 0xa);
// instead of the old `short* q = ...` inside the if reorders the preheader
// (p load hoisted above the count load, q built before the slot stores) and
// matches a few more preheader bytes. The ebx/ecx swap below is unchanged.
// This session also confirmed it is NOT a use-count or declaration-order tie:
// with p reduced to a single use (all player/owner reads routed through q) MSVC
// still keeps p in ebx; assigning q before p (q = projectiles+0xa, p = q-0xa)
// puts q in ecx and p in edx and leaves ebx unused; spelling px/py as
// *(short*)((char*)p + 6/0xa/0xe) with no q at all puts p in ecx (matching the
// original) but MSVC then folds the offsets instead of forming q. A local
// `Shot* shot = p->shot;` at the top of the loop moves p to eax and q to ebp
// (73.9). None of these reaches q-in-ebx with p-in-ecx.
// PARTIAL 78.1 percent (1646 of 1662 bytes), up from 72.4 this session. The
// top-of-function hunk is FIXED: writing the flag as an if/else
//     int enabled;
//     if (bit0 || bit1) enabled = 0; else enabled = 1;
//     if (bit9) enabled = 1;
// (bitfields, not (all & 3) != 0) made MSVC hoist the else assignment as an
// immediate `mov dword ptr [esp+0x1c], 1` before the load, emit
// `mov ax, word ptr [esi+0x14281]` + `test al, 3`, and stop materialising the
// shared constant 1 into a register. That removed the 5-byte size deficit and
// with it every branch-displacement hunk in the unit loop.
//
// Remaining gap, exactly two sites:
//   1. 0x4671c9 loop preheader / tail: the original keeps q (the `p + 0xa`
//      short pointer) in ebx, reloads p from [esp+0x1c] once per iteration
//      (`mov ecx, [esp+0x1c]`) and folds both tail reads onto ebx; this file
//      keeps p in ebx and loads q from [esp+0x1c] each iteration, so the whole
//      projectile-loop body and both inlined OnRadar copies are scheduled
//      differently (homes here are p=0x18, q=0x1c; the original is p=0x1c,
//      q=0x18). Both tail reads through q (v2) gave the original homes but
//      still p-in-ebx at 72.1 with the old top.
//   2. The ScaleX multiply at the first unit-loop use: the original evaluates
//      `movsx eax, [ebx+0x6c]` (u->field_6c) before `movsx ecx, [esi+0x142eb]`
//      (zoom); this file gets the reverse order. Swapping the operands in
//      ScaleX_00466dc0 changed nothing (MSVC canonicalises the commutative
//      imul), so the order is the allocator's.
//
// Measured this session: `unsigned short flags = g_game->field_14281.all;`
// folds away completely (identical 72.4 bytes); `enabled = (int)1u;` folds to
// the same constant node (no change); bitfield `bit0 || bit1` alone trades the
// correct `mov ax`/`test al,3` for a materialised constant and scores 71.4.
// Earlier sessions: q-based reads alone 70.9; `bits.bit0 || bits.bit1` with the
// old top 70.4 to 70.8; OnRadar taking &p->pos costs 22 bytes (65.4).
// and player reads now go through the q base (struct Tail_00466dc0, owner at
// q+0x48) instead of through p, which is what lifted this file from 71.7 to
// 72.4. The remaining gap is still the base-register decision: the preheader
// here is `mov ebx, [esi+0x141f7]` (p) + `lea ecx, [ebx+0xa]` (q) where the
// original has `mov ecx, [esi+0x141f7]` (p) + `lea ebx, [ecx+0xa]` (q), so the
// original keeps q in ebx, reloads p from its slot once per iteration
// (`mov ecx, [esp+0x1c]` at 0x4671c9) and folds both tail reads onto ebx
// (`mov ecx, [ebx+0x48]`, `mov cl, [ebx+0x5c]`), while this file keeps p in ebx
// and loads q from [esp+0x1c] each iteration. Every in-loop instruction
// follows from that single swap; the home slots themselves already agree
// (q=0x18, p=0x1c, i=0x28 when both tail reads go through q, v2, 72.1).
//
// Measured this session (free scratch scores, best is 72.4):
//   - Both tail reads through q (v2, 72.1) gives the original's homes exactly
//     but the p-in-ebx assignment; keeping the player read on p and the owner
//     read on q (h1) or the reverse (h2) both give 72.4 and keep the transposed
//     homes (p=0x18, q=0x1c). So the two tail reads are worth 0.3 percent for
//     reasons outside the loop, and neither spelling flips the register.
//   - Deriving q from an independent `char* pbase` instead of from p lets MSVC
//     fold p away completely (v1, 70.3 percent, frame 0x18 instead of 0x1c),
//     so p must really be an independent load of g_game->projectiles.
//   - Swapping the two latch increments (q before p) changes nothing (k1).
//   - Earlier sessions: q-based reads alone 70.9; a 16-bit flag read plus
//     `bits.bit0 || bits.bit1` reproduces the original's `mov ax,
//     [esi+0x14281]` + `test al, 3` but materialises the constant 1 in a
//     register (70.4 to 70.8); `u->field_6c * zoom` and a named Position*
//     local are neutral or worse (70.9); OnRadar taking &p->pos costs 22 bytes
//     (65.4).
//
// This session (deepseek-v4.1, retry 2): a 0-63 inert `extern int` decl sweep
// is flat at 78.3 (the 0x47d820 front-end-state lever does not apply here);
// re-deriving p from q inside the body (q loop-carried) is 76.1 and hoisting
// the count into a local (count-then-p load order as in the original) is 77.8.
//
// Still open, exact: get MSVC to hand ebx to q and spill p, and fix the
// top-of-function hunk where the original stores the constant 1 as a literal
// twice (`mov dword ptr [esp+0x1c], 1`) and reads the flag word with a 16-bit
// load (`mov ax, word ptr [esi+0x14281]`).
#pragma pack(push, 1)
#pragma pack(push, 1)
#pragma pack(push, 1)
#pragma pack(push, 1)

struct Shot_00466dc0;

union Flags110_00466dc0 {
    unsigned int all;
    struct {
        unsigned int :4;
        unsigned int bit4 : 1;
        unsigned int :27;
    } bits;
};

struct Flags241_00466dc0 {
    unsigned int :29;
    unsigned int bit29 : 1;
    unsigned int :2;
};

union Flags111_00466dc0 {
    unsigned int all;
    struct {
        unsigned int :30;
        unsigned int bit30 : 1;
        unsigned int :1;
    } bits;
};

union Flags142f0_00466dc0 {
    unsigned char bytes[2];
    struct {
        unsigned char lo;                // +0x142f0
        unsigned char hi;                // +0x142f1
    } b;
    struct {
        unsigned short :8;
        unsigned short bit0 : 1;         // +0x142f1 bit 0
        unsigned short bit1 : 1;         // +0x142f1 bit 1
        unsigned short :6;
    } bits;
};

union Flags14281_00466dc0 {
    unsigned short all;
    struct {
        unsigned short bit0 : 1;
        unsigned short bit1 : 1;
        unsigned short :7;
        unsigned short bit9 : 1;
        unsigned short :5;
    } bits;
};

struct Player_00466dc0 {
    char unknown_0[0x96];
    unsigned char field_96;              // +0x96
};

struct MapSize_00466dc0 {
    unsigned int width;                  // +0x80
    unsigned int height;                 // +0x84

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct PlayerInfo_00466dc0 {
    char unknown_0[0x27];
    Player_00466dc0* data;               // +0x27
    char unknown_2b[0x7c - 0x2b];
    unsigned char* los;                  // +0x7c
    MapSize_00466dc0 size;               // +0x80
    char unknown_88[0x14b - 0x88];
};

struct Slot_00466dc0 {
    Shot_00466dc0* shot;                 // +0x0
    char unknown_4[0xe - 4];
    unsigned char field_e;               // +0xe
    char unknown_f[0x1c - 0xf];
};

struct Shot_00466dc0 {
    char unknown_0[0xe0];
    int field_e0;                        // +0xe0
    char unknown_e4[0x111 - 0xe4];
    Flags111_00466dc0 flags;             // +0x111
};

struct UnitType_00466dc0 {
    char unknown_0[0x204];
    short field_204;                     // +0x204
    short field_206;                     // +0x206
    char unknown_208[0x20a - 0x208];
    short field_20a;                     // +0x20a
    short field_20c;                     // +0x20c
    char unknown_20e[0x241 - 0x20e];
    Flags241_00466dc0 flags_241;         // +0x241
    unsigned char field_245;             // +0x245
};

struct Unit_00466dc0 {
    char unknown_0[0x10];
    Slot_00466dc0 slots[3];              // +0x10
    char unknown_64[0x8];
    short field_6c;                      // +0x6c
    char unknown_6e[2];
    short field_70;                      // +0x70
    char unknown_72[2];
    short field_74;                      // +0x74
    char unknown_76[0x1c];
    UnitType_00466dc0* type;             // +0x92
    char unknown_96[0x10];
    short field_a6;                      // +0xa6
    short field_a8;                      // +0xa8
    char unknown_aa[0x50];
    unsigned char field_fa;              // +0xfa
    char unknown_fb[0x4];
    unsigned char field_ff;              // +0xff
    char unknown_100[0xe];
    unsigned char field_10e;             // +0x10e
    char unknown_10f[0x1];
    Flags110_00466dc0 flags_110;         // +0x110
    char unknown_114[0x4];
};

struct Projectile_00466dc0 {
    Shot_00466dc0* shot;                 // +0x0
    int posx;                            // +0x4
    int posy;                            // +0x8
    int posz;                            // +0xc
    char unknown_10[0x52 - 0x10];
    Unit_00466dc0* owner;                // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char player;                // +0x66
    char unknown_67[0x6b - 0x67];
};


struct Tail_00466dc0 {
    char unknown_0[0x48];
    Unit_00466dc0* owner;                // q+0x48
    char unknown_4c[0x5c - 0x4c];
    unsigned char player;                // q+0x5c
};

struct Blip_00466dc0 {
    short id;                            // +0x0
    int x;                               // +0x2
    int y;                               // +0x6
};

struct Game_00466dc0 {
    char unknown_0[0x2a43];
    unsigned char currentPlayer;         // +0x2a43
    char unknown_2a44[0x2cba - 0x2a44];
    short field_2cba;                    // +0x2cba
    char unknown_2cbc[0x141f3 - 0x2cbc];
    int projectileCount;                 // +0x141f3
    Projectile_00466dc0* projectiles;    // +0x141f7
    char unknown_141fb[0x1422b - 0x141fb];
    int field_1422b;                     // +0x1422b
    int field_1422f;                     // +0x1422f
    char unknown_14233[0x14273 - 0x14233];
    unsigned short* field_14273;         // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    Flags14281_00466dc0 field_14281;      // +0x14281
    char unknown_14283[0x142db - 0x14283];
    void* field_142db;                   // +0x142db
    void* field_142df;                   // +0x142df
    char unknown_142e3[0x142e7 - 0x142e3];
    short field_142e7;                   // +0x142e7
    short field_142e9;                   // +0x142e9
    short field_142eb;                   // +0x142eb
    short field_142ed;                   // +0x142ed
    char unknown_142ef[1];
    Flags142f0_00466dc0 field_142f0;     // +0x142f0
    char unknown_142f2[0x14357 - 0x142f2];
    Unit_00466dc0* units;                // +0x14357
    Unit_00466dc0* unitsEnd;             // +0x1435b
    char unknown_1435f[0x14363 - 0x1435f];
    unsigned short* field_14363;         // +0x14363
    char unknown_14367[0x1436b - 0x14367];
    int field_1436b;                     // +0x1436b
    char unknown_1436f[0x147df - 0x1436f];
    void* field_147df;                   // +0x147df
    void* field_147e3;                   // +0x147e3
    void* field_147e7;                   // +0x147e7
    char unknown_147eb[0x37f2f - 0x147eb];
    Flags14281_00466dc0 field_37f2f;     // +0x37f2f
};
#pragma pack(pop)

extern Game_00466dc0* g_game;

void* __stdcall FUN_004b7f30(void* a, int index);
void __stdcall FUN_004b7f90(void* surface, void* bmp, int x, int y);
void __stdcall FUN_004bee60(void* surface, int x, int y, int color);
void __stdcall FUN_004c0070(void* surface, int x, int y, int radius, int color);
void __stdcall FUN_004c01a0(void* surface, int x, int y, int radius, int color,
                            int a6, int a7);
void __stdcall FUN_004c6b70(void* dst, void* bmp, int x, int y);

static PlayerInfo_00466dc0* PlayerInfo_00466dc0_Get(unsigned char p)
{
    return (PlayerInfo_00466dc0*)((char*)g_game + 0x1b63) + p;
}

// True when (px, py) is inside the current player's visible area. The two
// halves match the uint8 terrain bitmap and the packed 16-bit bitfield variant.
static inline int OnRadarByte_00466dc0(PlayerInfo_00466dc0* pi, int px, int py)
{
    int tx = px >> 5;
    int ty = py >> 5;
    if (pi->size.Contains(tx, ty) && pi->los[pi->size.width * ty + tx] != 0)
        return 1;
    return 0;
}

static inline int OnRadarShort_00466dc0(PlayerInfo_00466dc0* pi, int px, int py)
{
    int tx = px >> 5;
    int ty = py >> 5;
    if (!pi->size.Contains(tx, ty))
        return 0;
    return (g_game->field_14273[pi->size.width * ty + tx] &
            (1 << g_game->currentPlayer)) != 0;
}

static inline int OnRadar_00466dc0(int px, int py)
{
    PlayerInfo_00466dc0* pi = PlayerInfo_00466dc0_Get(g_game->currentPlayer);
    if ((g_game->field_14281.all & 2) == 2)
        return OnRadarByte_00466dc0(pi, px, py);
    return OnRadarShort_00466dc0(pi, px, py);
}

// Scale a unit's world coordinate by the current zoom, keeping the source
// order of the multiply so the operand lands in the right register.
static inline int ScaleY_00466dc0(Unit_00466dc0* u)
{
    return ((int)u->field_74 - ((int)u->field_70 >> 1)) * (int)g_game->field_142ed;
}

// FUNCTION: 0x466dc0
void FUN_00466dc0(void)
{
    unsigned char* base = (unsigned char*)g_game + 0xdcb;
    unsigned short* out = g_game->field_14363;

    g_game->field_1436b = 0;
    void* surface = g_game->field_142db;
    FUN_004c6b70(surface, g_game->field_142df, 0, 0);

    int enabled;
    if (g_game->field_14281.bits.bit0 || g_game->field_14281.bits.bit1)
        enabled = 0;
    else
        enabled = 1;
    if (g_game->field_37f2f.bits.bit9)
        enabled = 1;

    Unit_00466dc0* u = g_game->units;
    Unit_00466dc0* end = g_game->unitsEnd;
    if (u <= end) {
        do {
            if (u->field_a6 != 0) {
                if (enabled != 0 || (u->flags_110.all & 0x300) != 0 ||
                    u->field_ff == g_game->currentPlayer) {
                    int x = u->field_6c * g_game->field_142eb /
                            g_game->field_1422b;
                    int y = ScaleY_00466dc0(u) / g_game->field_1422f;
                    if (u->field_fa == 0 ||
                        (g_game->field_142f0.b.hi & 1) != 0) {
                        FUN_004b7f90(surface,
                            FUN_004b7f30(g_game->field_147df,
                                PlayerInfo_00466dc0_Get(u->field_ff)->data->field_96),
                            x, y);
                    }
                    if (u->field_a8 == g_game->field_2cba) {
                        FUN_004b7f90(surface,
                            FUN_004b7f30(g_game->field_147e3, 0), x, y);
                    }
                    if (u->flags_110.bits.bit4) {
                        if ((u->field_10e & 1) != 0 ||
                            (u->type->field_245 & 4) == 0) {
                            if (u->type->field_204 != 0)
                                FUN_004c0070(surface, x, y,
                                    (int)g_game->field_142eb * u->type->field_204 /
                                    g_game->field_1422b, base[0xa]);
                            if (u->type->field_206 != 0)
                                FUN_004c0070(surface, x, y,
                                    (int)g_game->field_142eb * u->type->field_206 /
                                    g_game->field_1422b, base[0xa]);
                            if (u->type->field_20a != 0)
                                FUN_004c0070(surface, x, y,
                                    (int)g_game->field_142eb * u->type->field_20a /
                                    g_game->field_1422b, base[0xc]);
                            if (u->type->field_20c != 0)
                                FUN_004c0070(surface, x, y,
                                    (int)g_game->field_142eb * u->type->field_20c /
                                    g_game->field_1422b, base[0xc]);
                        }
                        if (u->type->flags_241.bit29) {
                            Slot_00466dc0* slot = u->slots;
                            int n = 3;
                            do {
                                Shot_00466dc0* shot = slot->shot;
                                if (shot->flags.bits.bit30) {
                                    int r = ((int)g_game->field_142eb *
                                             (shot->field_e0 - 0x200)) /
                                            g_game->field_1422b;
                                    if (slot->field_e != 0)
                                        FUN_004c01a0(surface, x, y, r, base[0xf],
                                                     0x20,
                                                     g_game->field_142f0.b.hi & 1);
                                    else
                                        FUN_004c0070(surface, x, y, r, base[0xf]);
                                }
                                slot++;
                                n--;
                            } while (n != 0);
                        }
                    }
                    out[0] = u->field_a8;
                    *(int*)(out + 1) = g_game->field_142e7 + x;
                    *(int*)(out + 3) = g_game->field_142e9 + y;
                    out += 5;
                    g_game->field_1436b++;
                }
            }
            u = (Unit_00466dc0*)((char*)u + 0x118);
        } while (u <= g_game->unitsEnd);
    }

    Projectile_00466dc0* p = g_game->projectiles;
    int i = 0;
    if (g_game->projectileCount > 0) {
        short* q = (short*)((char*)p + 0xa);
        do {
            int px = q[-2];
            int x = (int)g_game->field_142eb * px / g_game->field_1422b;
            int py = q[2] - ((int)q[0] >> 1);
            int y = (int)g_game->field_142ed * py / g_game->field_1422f;
            if ((p->shot->flags.all & 0x60000000) == 0) {
                if ((p->shot->flags.all & 0x40) == 0) {
                    if (OnRadar_00466dc0(px, py) ||
                        ((Tail_00466dc0*)q)->player ==
                            g_game->currentPlayer) {
                        FUN_004bee60(surface, x, y, base[0xe]);
                    }
                }
            } else {
                if (OnRadar_00466dc0(px, py) ||
                    ((Tail_00466dc0*)((char*)q))->owner->field_ff ==
                        g_game->currentPlayer) {
                    FUN_004b7f90(surface,
                        FUN_004b7f30(g_game->field_147e7,
                            PlayerInfo_00466dc0_Get(
                                ((Tail_00466dc0*)q)->player)->data->field_96),
                        x, y);
                }
            }
            i++;
            p = (Projectile_00466dc0*)((char*)p + 0x6b);
            q = (short*)((char*)q + 0x6b);
        } while (i < g_game->projectileCount);
    }

    g_game->field_142f0.bits.bit1 = 1;
}
