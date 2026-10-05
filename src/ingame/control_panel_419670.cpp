// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Snaps the position at +0x2caa to the centre of its cell for the unit
// type selected at +0x2cc4, then passes it to FUN_0043afc0 as a
// "MOBILEBUILD" (def flag bit 11 clear) or "VTOL_MOBILEBUILD" (bit 11 set)
// order for each of the local player's units with flag 0x10 whose def has
// flag 0x40. Bit 2 of the argument's field_8 is passed through.
//
// The fixed-point conversion is the WorldToCell/CellToWorld pair of inline
// helpers (as in 0x403a20 and 0x47ddc0), with def->origin read once into a
// local Point. Passing def->origin to each helper instead folds `cell` into
// registers and shrinks the frame from 0x10 to 0xc.
#pragma pack(push, 1)
struct Point { short x, y; };
struct Vec3 { int x, y, z; };

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Flags_00419670 {
    unsigned int bits_0 : 6;
    unsigned int flag_6 : 1;           // bit 6
    unsigned int bits_7 : 4;
    unsigned int flag_11 : 1;          // bit 11
    unsigned int bits_12 : 20;
};

union FlagsU_00419670 {
    unsigned int raw;
    Flags_00419670 bits;
};

struct UnitDef_00419670 {
    char unknown_0[0x14a];
    Point origin;                      // +0x14a
    char unknown_14e[0x241 - 0x14e];
    FlagsU_00419670 flags;             // +0x241
    char unknown_245[0x249 - 0x245];
};

struct Unit {
    char unknown_0[8];
    unsigned int field_8;              // +0x8
    char unknown_c[0x92 - 0xc];
    UnitDef_00419670* def;             // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player_00419670 {
    char unknown_0[0x67];
    Unit* units;                       // +0x67
    Unit* unitsEnd;                    // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00419670 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char field_2a42;          // +0x2a42
    char unknown_2a43[0x2c96 - 0x2a43];
    int field_2c96;                    // +0x2c96
    char unknown_2c9a[0x2caa - 0x2c9a];
    Vec3 pos;                          // +0x2caa
    char unknown_2cb6[0x2cc4 - 0x2cb6];
    unsigned short field_2cc4;         // +0x2cc4
    char unknown_2cc6[0x1439b - 0x2cc6];
    UnitDef_00419670* defs;            // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

struct Arg_00419670 {
    char unknown_0[8];
    unsigned int field_8;              // +0x8
};

void __stdcall FUN_0043afc0(Class_00438760 kind, int remove, Unit* owner,
                            int id, Vec3* pos, int param_6, int param_7);
static inline Point WorldToCell(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}
static inline void CellToWorld(Point origin, Point c, Vec3* v)
{
    v->x = (origin.x + c.x * 2) << 19;
    v->z = (origin.y + c.y * 2) << 19;
}

// FUNCTION: 0x419670
void __stdcall FUN_00419670(Arg_00419670* arg)
{
    unsigned int remove = (arg->field_8 >> 2) & 1;
    unsigned short index = g_game->field_2cc4;
    UnitDef_00419670* def = &g_game->defs[index];
    Vec3 pos = g_game->pos;
    Point origin = def->origin;
    Point cell = WorldToCell(pos, origin);
    CellToWorld(origin, cell, &pos);
    pos.y = g_game->field_2c96 << 16;

    unsigned char team = g_game->field_2a42;
    Player_00419670* p = &g_game->players[team];
    for (Unit* u = p->units; u <= p->unitsEnd; u++) {
        if ((u->flags & 0x10) && (u->def->flags.raw & 0x40)) {
            if (!(u->def->flags.raw & 0x800)) {
                FUN_0043afc0("MOBILEBUILD", remove, u, 0, &pos, index, 0);
            } else {
                Flags_00419670 flags = u->def->flags.bits;
                if (flags.flag_11) {
                    FUN_0043afc0("VTOL_MOBILEBUILD", remove, u, 0, &pos, index, 0);
                }
            }
        }
    }
}
