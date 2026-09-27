// Decompiled by deepseek-v4.1-flash. Names are provisional.
// 75.0%: the flags/loop/argument setup now match the original exactly
// (`test ah,8; jne` then the MOBILEBUILD block, and a second `shr eax,0xb;
// test al,1` guard for VTOL_MOBILEBUILD, both calls reaching a shared tail).
// What still differs is only the fixed-point WorldToCell/CellToWorld block:
// the original keeps the def pointer in edx and spills `cell.x` to
// [esp+0x10] (frame 0x10), giving the origin.x-in-esi / origin.y-in-ebx
// schedule. Written as explicit field assignments the compiler puts def in
// esi and the pos pointer in edx; written with the WorldToCell/CellToWorld
// inline helpers (the 0x403a20 style) it keeps the def pointer in edx but
// folds `cell` into registers and drops the frame to 0xc, which shifts every
// subsequent stack offset by 4. All other bytes match.

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

struct Unit_00419670 {
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
    Unit_00419670* units;              // +0x67
    Unit_00419670* unitsEnd;           // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_00419670 {
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

extern Game_00419670* g_game;

struct Arg_00419670 {
    char unknown_0[8];
    unsigned int field_8;              // +0x8
};

void __stdcall FUN_0043afc0(Class_00438760 kind, int remove, Unit_00419670* owner,
                            int id, Vec3* pos, int param_6, int param_7);

// FUNCTION: 0x419670
void __stdcall FUN_00419670(Arg_00419670* arg)
{
    unsigned int remove = (arg->field_8 >> 2) & 1;
    unsigned short index = g_game->field_2cc4;
    UnitDef_00419670* def = &g_game->defs[index];
    Point cell;
    Vec3 pos = g_game->pos;
    Point origin = def->origin;
    cell.x = (pos.x - (origin.x << 19) + 0x80000) >> 20;
    cell.y = (pos.z - (origin.y << 19) + 0x80000) >> 20;
    pos.x = (origin.x + cell.x * 2) << 19;
    pos.z = (origin.y + cell.y * 2) << 19;
    pos.y = g_game->field_2c96 << 16;

    unsigned char team = g_game->field_2a42;
    Player_00419670* p = &g_game->players[team];
    for (Unit_00419670* u = p->units; u <= p->unitsEnd; u++) {
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
