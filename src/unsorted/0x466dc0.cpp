// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, GPT-6.1-sol. Names are provisional.
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

struct PlayerInfo_00466dc0 {
    char unknown_0[0x27];
    Player_00466dc0* data;               // +0x27
    char unknown_2b[0x7c - 0x2b];
    unsigned char* los;                  // +0x7c
    int width;                           // +0x80
    int height;                          // +0x84
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
static inline int OnRadar_00466dc0(int px, int py)
{
    PlayerInfo_00466dc0* pi = PlayerInfo_00466dc0_Get(g_game->currentPlayer);
    int b;
    if ((g_game->field_14281.all & 2) == 2) {
        b = (unsigned int)(px >> 5) < (unsigned int)pi->width &&
            (unsigned int)(py >> 5) < (unsigned int)pi->height &&
            pi->los[(py >> 5) * pi->width + (px >> 5)] != 0;
    } else {
        if ((unsigned int)(px >> 5) < (unsigned int)pi->width &&
            (unsigned int)(py >> 5) < (unsigned int)pi->height)
            b = (g_game->field_14273[(py >> 5) * pi->width + (px >> 5)] &
                 (1 << g_game->currentPlayer)) != 0;
        else
            b = 0;
    }
    return b;
}

// Scale a unit's world coordinate by the current zoom, keeping the source
// order of the multiply so the operand lands in the right register.
static inline int ScaleX_00466dc0(Unit_00466dc0* u)
{
    return u->field_6c * g_game->field_142eb;
}

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
                    int x = ScaleX_00466dc0(u) / g_game->field_1422b;
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
    short* q = (short*)((char*)p + 0xa);
    int i = 0;
    if (g_game->projectileCount > 0) {
        do {
            int px = q[-2];
            int x = (int)g_game->field_142eb * px / g_game->field_1422b;
            int py = q[2] - ((int)q[0] >> 1);
            int y = (int)g_game->field_142ed * py / g_game->field_1422f;
            if ((p->shot->flags.all & 0x60000000) == 0) {
                if ((p->shot->flags.all & 0x40) == 0) {
                    if (OnRadar_00466dc0(px, py) ||
                        p->player == g_game->currentPlayer) {
                        FUN_004bee60(surface, x, y, base[0xe]);
                    }
                }
            } else {
                if (OnRadar_00466dc0(px, py) ||
                    ((Tail_00466dc0*)((char*)q))->owner->field_ff ==
                        g_game->currentPlayer) {
                    FUN_004b7f90(surface,
                        FUN_004b7f30(g_game->field_147e7,
                            PlayerInfo_00466dc0_Get(p->player)->data->field_96),
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
