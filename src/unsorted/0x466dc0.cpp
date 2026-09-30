// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// PARTIAL 71.3 percent (1641 of 1662 bytes). The whole remaining gap is ONE
// allocation state, the p versus q register choice in the projectile loop. The
// original keeps the position pointer q = p + 0xa in ebx, gives p only ecx and
// reloads p from its stack slot once per iteration at 0x4671c9 (the loop is
// jmp-ed over that reload at 0x4671c7, and the store back is the latch), so
// p has a single use in the body (`mov eax, [ecx]`, p->type) while q has five
// (pos.x, pos.y, pos.z, owner at q+0x48, player at q+0x5c). This file keeps p
// in ebx and q in ecx, which also pushes the inlined OnRadar's PlayerInfo
// pointer onto ebp instead of edx.
//
// What was measured this session (all free scratch scores, best is 71.3):
//   1. The fold of p->player and p->owner onto the q base is NOT the lever.
//      Reading them through a struct pointer biased to q (Tail_00466dc0 at
//      q+0x48 / q+0x5c) makes MSVC emit exactly the original's
//      `mov cl, [ebx+0x5c]` and `mov ecx, [ebx+0x48]`, yet p still wins ebx
//      (70.9 percent, 1641 bytes). The offset reassociation happens anyway.
//   2. What DOES flip it is passing &p->pos to the inlined OnRadar helper
//      instead of px and py by value: MSVC then materialises the biased
//      induction variable, `lea ebx, [ecx+0xa]`, and gives it ebx while p keeps
//      ecx, which is the original's assignment exactly (v10, build/scratch/
//      0x466dc0/v10.cpp). But it costs 22 bytes net and drops to 65.4
//      percent, because the two copies of the helper then re-load the position
//      fields instead of reusing the registers: the original has
//      `movsx ebp, [ebx-4]` once and later `sar ebp, 5; sar edi, 5`, while
//      this shape reloads (`mov ax, [ebx-4]; mov cx, ax; sar cx, 5`).
//      v1 (the same helper, with px and py still read through an explicit
//      short* q so the two expression trees differ) is 67.8 percent and 100
//      bytes long, so the two shapes are NOT the same tree and the load CSE
//      that the original relies on does not fire.
//   3. Reading the position through a real struct member of Projectile
//      (`Position_00466dc0 pos` at +0x4, three shorts at +6, +0xa, +0xe) with
//      px and py by value gives 70.4 percent (v2) with no second induction
//      variable at all, and with a named `Position* pos` local 70.9 percent
//      (v6), but the biased base is then p + 0xe (as in the matched 0x475470)
//      rather than the original's p + 0xa.
//   4. Flipping the multiply operands to `px * zoom` (v11) changed nothing
//      measurable (65.4 percent, same 1684 bytes as v10).
// So the remaining construct is: the biased induction variable at p + 0xa in
// ebx, p reloaded from a stack slot, and the inlined helper seeing the
// position through the same expression tree as the x and y scaling so the
// three loads are shared. That combination has not been found.
//
// New this session (2025, second pass):
//   - The top-of-function hunk (0x466e0c region) is an independent cluster, not
//     part of the projectile loop: the original stores the constant 1 as an
//     immediate twice and reads the flag word with a 16-bit load
//     (`mov ax, word ptr [esi+0x14281]`) before `test al, 3`. Spelling the two
//     low bits as `field_14281.bits.bit0 || field_14281.bits.bit1` reproduces
//     that load and test exactly, but the constant 1 then materialises in ecx
//     (`mov ecx, 1` + `mov [esp+0x1c], ecx`) instead of two immediate stores,
//     which cascades into the field_37f2f read (dx instead of cx) and into the
//     unit loop (`mov al, [ebx+0xff]` instead of `mov dl, ...`), netting 70.4
//     percent (1643 bytes). A 2-bit bitfield read and `(all & 3)` both narrow
//     back to `test byte ptr [esi+0x14281], 3`.
//   - The projectile loop home slots are p=0x18/i=0x1c/q=0x28 here and
//     q=0x18/p=0x1c/i=0x28 in the original, so the allocator's variable order
//     differs and not just its register choice. Declaring q first (over an
//     independent `g_game->projectiles` read, before p and before i) moves the
//     homes but stays at 70.8 percent (1645 bytes).
//
// Still open, smaller:
//   - `enabled`: the original loads 16 bits (`mov ax, word ptr [esi+0x14281]`)
//     and stores the constant 1 as an immediate both times; a 16-bit union
//     read brings the load back but the 1 then materialises in a register
//     (1643 bytes, 70.4 percent), and a named 16-bit local is folded away.
//   - the two FUN_004b7f90 calls load `surface` after `push eax` in the
//     original and before it here.
// Tried and neutral: reading player and owner through a q-biased struct
// (70.9), the union-typed +0x14281 field, a named 16-bit local for it, the
// `!= 0` spelling of the slot->field_e test, plain `p++` for the projectile
// stride, declaring q before p. Tried and worse: OnRadar taking &p->pos with
// px and py read through a different tree (v1, 67.8), the same helper with a
// shared tree (v10 and v11, 65.4), a named `Position* pos` local (v6, 70.9).
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
    return (int)g_game->field_142eb * (int)u->field_6c;
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

    int enabled = 1;
    if ((g_game->field_14281.all & 3) != 0)
        enabled = 0;
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
                        p->player == g_game->currentPlayer) {
                        FUN_004bee60(surface, x, y, base[0xe]);
                    }
                }
            } else {
                if (OnRadar_00466dc0(px, py) ||
                    p->owner->field_ff == g_game->currentPlayer) {
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
