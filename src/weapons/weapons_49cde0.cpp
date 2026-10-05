// Decompiled by Space Bunny Free. Names are provisional.
// Fires a shell from a firing unit: takes a free slot in the 300 entry
// projectile array, hands it to FUN_0049c740, gives it a direction built from
// the shot's heading and pitch, works out when it will hit (from the distance
// to the aim point when the unit type's flag bit 23 is set, from a stored
// flight time otherwise), plays the firing and the "RockUnit" anim and drops a
// scorch mark. Returns 0 when no slot was free.
//
// Match notes: three shapes here are not the obvious ones.
// - The `+0x111` flags are read as a bitfield, not masked: the original does
//   `shr ecx, 0x17; test cl, 1` for bit 23 and `shr eax, 9; test al, 1` for
//   bit 9, and `& 0x800000` / `& 0x200` compile to a single `test` instead.
// - The four aim directions are indexed with a 28 byte stride, so they are an
//   `int[4][7]` and the unit's own heading at +0x66 lies inside that block
//   (hence the union).
// - The direction's y term is `sin * scale - (a / b) * game->field`, and only
//   a statement of its own for the whole product puts the division and the
//   multiply before the call; inside the assignment the compiler sinks them
//   after it. The last two stores are the other way round from what reads
//   naturally (`active` then `field_4e`), which is what the original does.
#include <math.h>

#pragma pack(push, 1)
struct Vec3_0049cde0 {
    int x;
    int y;
    int z;
};

struct Flags_0049cde0 {
    unsigned int a : 9;
    unsigned int f9 : 1;               // bit 9
    unsigned int b : 13;
    unsigned int f23 : 1;              // bit 23
    unsigned int c : 8;
};

struct UnitType_0049cde0 {
    char unknown_0[0x68];
    int f_68;                          // the fixed point length
    char unknown_6c[0xe6 - 0x6c];
    unsigned short f_e6;               // +0xe6, the fallback flight time
    char unknown_e8[0xea - 0xe8];
    unsigned short f_ea;               // +0xea
    char unknown_ec[0x111 - 0xec];
    Flags_0049cde0 flags;             // +0x111
};

struct Shot_0049cde0 {
    char unknown_0[0xc];
    UnitType_0049cde0* def;            // +0xc, the unit type that fired
    unsigned int field_10;             // +0x10
    char unknown_14[0x16 - 0x14];
    short heading;                     // +0x16
    short pitch;                       // +0x18
    char unknown_1a[0x1b - 0x1a];
    unsigned char field_1b;            // +0x1b
};

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

class Class_004b0940 {
public:
    void StartScript(const char* name, int param_2, int param_3);
};

struct Heading_0049cde0 {
    char unknown_0[0x4c];              // +0x1a .. +0x65
    short heading;                     // +0x66
};

struct Unit {
    char unknown_0[0x1a];
    // The four aim directions, 0x1c bytes apart, start at +0x1a; the unit's
    // own heading at +0x66 lies inside that block, so the two overlap.
    union {
        int aim[4][7];                 // +0x1a
        Heading_0049cde0 heading;      // +0x1a
    };
    char unknown_8a[0x9a - 0x8a];
    CobScript* anims;                  // +0x9a
};

struct Proj_0049cde0 {
    char unknown_0[0x1c];
    int dirx;                          // +0x1c
    int diry;                          // +0x20
    int dirz;                          // +0x24
    char unknown_28[0x36 - 0x28];
    short angle;                       // +0x36
    short pitch0;                      // +0x38
    char unknown_3a[0x46 - 0x3a];
    int time;                          // +0x46
    char unknown_4a[0x4e - 0x4a];
    int field_4e;
    char unknown_52[0x60 - 0x52];
    short active;                      // +0x60
    char unknown_62[0x69 - 0x62];
    unsigned short flags;              // +0x69
};

struct Game {
    char unknown_0[0x141f3];
    int projCount;                     // +0x141f3
    Proj_0049cde0* projs;              // +0x141f7
    char unknown_141fb[0x14263 - 0x141fb];
    int field_14263;                   // +0x14263
    char unknown_14267[0x38a47 - 0x14267];
    int field_38a47;                   // +0x38a47, the current frame number
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_00509678[4];

void __stdcall FUN_0049c740(Proj_0049cde0* proj, UnitType_0049cde0* shot, Vec3_0049cde0* pos,
                            Vec3_0049cde0* aim, int field_5, Unit* unit);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
void __stdcall FUN_004729d0(Vec3_0049cde0* p, short index);

// FUNCTION: 0x49cde0
int __stdcall FUN_0049cde0(Shot_0049cde0* shot, Unit* unit, Vec3_0049cde0* pos,
                           Vec3_0049cde0* aim, int param_5)
{
    Proj_0049cde0* proj = 0;
    if (g_game->projCount < 300) {
        proj = &g_game->projs[g_game->projCount++];
        proj->flags &= ~2;
        proj->field_4e = 0;
    }
    if (proj) {
        FUN_0049c740(proj, shot->def, pos, 0, g_game->field_38a47, unit);
        proj->angle = shot->heading;
        proj->pitch0 = shot->pitch;
        int q = (shot->field_10 / shot->def->f_68) * g_game->field_14263;
        proj->diry = FUN_004b70ef(shot->pitch, shot->def->f_68) - q;
        int scale = FUN_004b7123(shot->pitch, shot->def->f_68);
        proj->dirx = -FUN_004b70ef(shot->heading, scale);
        proj->dirz = -FUN_004b7123(shot->heading, scale);
        if (shot->def->flags.f23) {
            proj->time = (int)_hypot((double)(pos->x - aim->x), (double)(pos->z - aim->z)) / scale
                       + g_game->field_38a47;
        } else {
            proj->time = g_game->field_38a47 + shot->def->f_e6;
        }
        proj->active = shot->def->f_ea;
        proj->field_4e = param_5;
        ((Class_004b0940*)unit->anims)->StartScript(DAT_00509678[(shot->field_1b >> 2) & 3], 0, 0);
        short angle = unit->aim[(shot->field_1b >> 2) & 3][0] - unit->heading.heading;
        int a = -FUN_004b70ef(angle, 800);
        int b = -FUN_004b7123(angle, 800);
        unit->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);
        if (shot->def->flags.f9)
            FUN_004729d0(pos, 9);
        return 1;
    }
    return 0;
}
