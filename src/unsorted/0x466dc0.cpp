// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Draws the radar/minimap. The first loop walks the unit list (stride 0x118):
// for each unit visible to the current player it blits the unit's radar logo
// (and the "high" logo when the unit id matches g_game+0x2cba), draws the four
// weapon-range circles of the unit type and, for units with the 0x20000000
// flag, up to three turret markers, then appends a 10-byte blip record
// (short id, int x, int y) to the buffer at g_game+0x14363. The second loop
// walks the projectile array (stride 0x6b) and blits a logo for every
// projectile visible to the current player. Finally the "radar dirty" bit is
// set in g_game+0x142f1.
//
// PARTIAL (49.8%). Structure is right (both loops, all callees, the output
// records and the final dirty-bit set). What still differs:
// - My frame is 0x18, the original 0x1c (7 locals): one more live local is
//   needed across the whole function.
// - The top `(g_game->field_14281 & 3)` compiles to `test byte ptr`, the
//   original loads `mov ax` and tests `al`; the 0x37f2f bit test likewise.
// - The first loop keeps `surface` in a register in places where the original
//   reloads it from its stack slot.
// - The inlined radar predicate compiles the byte-array case to `setne`; the
//   original uses a branch (`je; mov edx,1; jmp; xor edx,edx`).
// - Unit list iteration: original advances ebx and re-reads unitsEnd at the
//   bottom; mine keeps the end pointer in eax/ecx differently.
// - The second loop in the original walks base+0xa in ebx with the element
//   pointer in a stack slot; mine keeps a single element pointer.
// - Projectile positions are the high halves of the ints at +4/+8/+0xc. They
//   are written as `(short)(p->pos >> 16)`; the original may have had a helper.
#pragma pack(push, 1)

struct Shot_00466dc0;

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
    unsigned int flags;                  // +0x111
};

struct UnitType_00466dc0 {
    char unknown_0[0x204];
    short field_204;                     // +0x204
    short field_206;                     // +0x206
    char unknown_208[0x20a - 0x208];
    short field_20a;                     // +0x20a
    short field_20c;                     // +0x20c
    char unknown_20e[0x241 - 0x20e];
    unsigned int flags_241;              // +0x241
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
    unsigned int flags_110;              // +0x110
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
    unsigned short field_14281;          // +0x14281
    char unknown_14283[0x142db - 0x14283];
    void* field_142db;                   // +0x142db
    void* field_142df;                   // +0x142df
    char unknown_142e3[0x142e7 - 0x142e3];
    short field_142e7;                   // +0x142e7
    short field_142e9;                   // +0x142e9
    short field_142eb;                   // +0x142eb
    short field_142ed;                   // +0x142ed
    char unknown_142ef[0x142f1 - 0x142ef];
    unsigned char field_142f1;           // +0x142f1
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
    unsigned short field_37f2f;          // +0x37f2f
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
    if ((g_game->field_14281 & 2) == 2) {
        unsigned int x = (unsigned int)(px >> 5);
        unsigned int y = (unsigned int)(py >> 5);
        PlayerInfo_00466dc0* pi = PlayerInfo_00466dc0_Get(g_game->currentPlayer);
        if (x >= (unsigned int)pi->width)
            return 0;
        if (y >= (unsigned int)pi->height)
            return 0;
        return pi->los[y * pi->width + x] != 0;
    } else {
        unsigned int x = (unsigned int)(px >> 5);
        unsigned int y = (unsigned int)(py >> 5);
        PlayerInfo_00466dc0* pi = PlayerInfo_00466dc0_Get(g_game->currentPlayer);
        if (x >= (unsigned int)pi->width)
            return 0;
        if (y >= (unsigned int)pi->height)
            return 0;
        return (g_game->field_14273[y * pi->width + x] &
                (1 << g_game->currentPlayer)) != 0;
    }
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
    if ((g_game->field_14281 & 3) != 0)
        enabled = 0;
    if ((g_game->field_37f2f >> 9) & 1)
        enabled = 1;

    Unit_00466dc0* u = g_game->units;
    Unit_00466dc0* end = g_game->unitsEnd;
    if (u <= end) {
        do {
            if (u->field_a6 != 0) {
                if (enabled != 0 || (u->flags_110 & 0x300) != 0 ||
                    u->field_ff == g_game->currentPlayer) {
                    int x = ((int)u->field_6c * (int)g_game->field_142eb) /
                            g_game->field_1422b;
                    int y = (((int)u->field_74 - ((int)u->field_70 >> 1)) *
                             (int)g_game->field_142ed) / g_game->field_1422f;
                    if (u->field_fa == 0 || (g_game->field_142f1 & 1) != 0) {
                        FUN_004b7f90(surface,
                            FUN_004b7f30(g_game->field_147df,
                                PlayerInfo_00466dc0_Get(u->field_ff)->data->field_96),
                            x, y);
                    }
                    if (u->field_a8 == g_game->field_2cba) {
                        FUN_004b7f90(surface,
                            FUN_004b7f30(g_game->field_147e3, 0), x, y);
                    }
                    if ((u->flags_110 >> 4) & 1) {
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
                        if ((u->type->flags_241 >> 29) & 1) {
                            Slot_00466dc0* slot = u->slots;
                            int n = 3;
                            do {
                                Shot_00466dc0* shot = slot->shot;
                                if ((shot->flags >> 30) & 1) {
                                    int r = ((int)g_game->field_142eb *
                                             (shot->field_e0 - 0x200)) /
                                            g_game->field_1422b;
                                    if (slot->field_e == 0)
                                        FUN_004c0070(surface, x, y, r, base[0xf]);
                                    else
                                        FUN_004c01a0(surface, x, y, r, base[0xf],
                                                     0x20,
                                                     g_game->field_142f1 & 1);
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
    for (int i = 0; i < g_game->projectileCount; i++) {
        int px = (short)(p->posx >> 16);
        int pz = (short)(p->posz >> 16) - ((short)(p->posy >> 16) >> 1);
        int x = (int)g_game->field_142eb * px / g_game->field_1422b;
        int y = (int)g_game->field_142ed * pz / g_game->field_1422f;
        Shot_00466dc0* shot = p->shot;
        if ((shot->flags & 0x60000000) == 0) {
            if ((shot->flags & 0x40) == 0) {
                if (OnRadar_00466dc0(px, pz) ||
                    p->player == g_game->currentPlayer) {
                    FUN_004bee60(surface, x, y, base[0xe]);
                }
            }
        } else {
            if (OnRadar_00466dc0(px, pz) ||
                p->owner->field_ff == g_game->currentPlayer) {
                FUN_004b7f90(surface,
                    FUN_004b7f30(g_game->field_147e7,
                        PlayerInfo_00466dc0_Get(p->player)->data->field_96),
                    x, y);
            }
        }
        p = (Projectile_00466dc0*)((char*)p + 0x6b);
    }

    g_game->field_142f1 |= 2;
}
