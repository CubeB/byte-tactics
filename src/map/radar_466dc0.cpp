// Decompiled by deepseek-v4.1-flash, with space-bunny-free, GPT-6, GPT-6.1-sol,
// mimo-v2.6-pro and DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
//
// MATCH (Claude Opus 5.5, #5292). The file sat at 99.6% for a dozen passes on
// one pair of swapped loads in the unit loop's x multiply
// (`movsx eax, [ebx+0x6c]; movsx ecx, [esi+0x142eb]` in the original). What
// decides that order, measured with reduced copies of this function:
//   - MSVC 5 orders the two operands of `u->x * g_game->width` by a key built
//     from the symbol ids of their bases plus the displacement, so in a small
//     function the order flips with the number of declarations in front of it
//     (a 20000-entry enum in front gives the original's order).
//   - In this function the key never moved, because C2 rebuilt the unit loop's
//     field accesses on derived induction temporaries (the dropped `temp`
//     candidates c2prio lists over the loop's lines) once `u->type` was read
//     in two or more blocks after calls. Reading the type through a reference
//     taken at the top of the loop body, `UnitType*& type = u->type;`, emits
//     the same loads but leaves the x multiply on `u`, so it follows the ids.
//   - <windows.h> then supplies the declaration count that gives the
//     original's order (g_game's id near 29000).
// That header state moved two other spots, which the original's spellings
// put back: the radar helpers use the ByteMap Index/Get methods of the
// matched 0x475470 (width materialised, `mov ecx, [edx+0x80]; imul ecx, edi`),
// and the slot loop reads `slot->shot` at each use instead of a `shot` local
// (zoom loaded first in the range multiply, `slot` kept in its stack slot).
#include <windows.h>

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

struct ByteMap_00466dc0 {
    unsigned char* data;                 // +0x0
    MapSize_00466dc0 size;               // +0x4

    int Index(int x, int y) { return size.width * y + x; }
    unsigned char Get(int x, int y) { return data[Index(x, y)]; }
};

struct PlayerInfo_00466dc0 {
    char unknown_0[0x27];
    Player_00466dc0* data;               // +0x27
    char unknown_2b[0x7c - 0x2b];
    ByteMap_00466dc0 explored;           // +0x7c
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

struct Unit {
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
    Unit* owner;                         // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char player;                // +0x66
    char unknown_67[0x6b - 0x67];
};


struct Tail_00466dc0 {
    char unknown_0[0x48];
    Unit* owner;                         // q+0x48
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
    Unit* units;                         // +0x14357
    Unit* unitsEnd;                      // +0x1435b
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
    if (pi->explored.size.Contains(tx, ty) && pi->explored.Get(tx, ty))
        return 1;
    return 0;
}

static inline int OnRadarShort_00466dc0(PlayerInfo_00466dc0* pi, int px, int py)
{
    int tx = px >> 5;
    int ty = py >> 5;
    if (!pi->explored.size.Contains(tx, ty)) {
        return 0;
    }
    ByteMap_00466dc0* b = &pi->explored;
    return (g_game->field_14273[b->Index(tx, ty)] &
            (1 << g_game->currentPlayer)) != 0;
}

static inline int OnRadar_00466dc0(int px, int py)
{
    PlayerInfo_00466dc0* pi = PlayerInfo_00466dc0_Get(g_game->currentPlayer);
    if ((g_game->field_14281.all & 2) == 2)
        return OnRadarByte_00466dc0(pi, px, py);
    return OnRadarShort_00466dc0(pi, px, py);
}

// A unit's screen y (height folded into z) times the minimap scale.
static inline int ScaleY_00466dc0(Unit* u)
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

    Unit* u = g_game->units;
    Unit* end = g_game->unitsEnd;
    if (u <= end) {
        do {
            UnitType_00466dc0*& type = u->type;
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
                            (type->field_245 & 4) == 0) {
                            if (type->field_204 != 0)
                                FUN_004c0070(surface, x, y,
                                    (int)g_game->field_142eb * type->field_204 /
                                    g_game->field_1422b, base[0xa]);
                            if (type->field_206 != 0)
                                FUN_004c0070(surface, x, y,
                                    (int)g_game->field_142eb * type->field_206 /
                                    g_game->field_1422b, base[0xa]);
                            if (type->field_20a != 0)
                                FUN_004c0070(surface, x, y,
                                    (int)g_game->field_142eb * type->field_20a /
                                    g_game->field_1422b, base[0xc]);
                            if (type->field_20c != 0)
                                FUN_004c0070(surface, x, y,
                                    (int)g_game->field_142eb * type->field_20c /
                                    g_game->field_1422b, base[0xc]);
                        }
                        if (type->flags_241.bit29) {
                            Slot_00466dc0* slot = u->slots;
                            int n = 3;
                            do {
                                if (slot->shot->flags.bits.bit30) {
                                    int r = ((int)g_game->field_142eb *
                                             (slot->shot->field_e0 - 0x200)) /
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
            u = (Unit*)((char*)u + 0x118);
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

