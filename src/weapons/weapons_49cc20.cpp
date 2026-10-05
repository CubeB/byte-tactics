// Decompiled by Space Bunny Free. Names are provisional.
// Fires one projectile from a firing unit: takes a slot from the 300 entry
// projectile pool, initialises it through InitProjectile, then plays the aim and
// fire animations of the shooting unit.
// Two shapes here are not the obvious ones:
//  - the per-direction aim data at +0x1a has four byte elements, so the
//    address is `base + index*4` for a 16 bit load. Declaring it as a plain
//    `short` array gives `base + index*2` and does not match.
//  - the flag word at +0x111 is used two ways: bit 27 as a plain int (the
//    inlined FUN_0049c920 gives a dword test) and bit 9 as a bitfield (the
//    original shifts and tests bit 0, no `test ch, 2`). A union holding both
//    views is the only declaration that gives both.
#include <string.h>

#pragma pack(push, 1)
struct Vec3_0049cc20 {
    int x;
    int y;
    int z;
};

struct FlagsBits_0049cc20 {
    unsigned int low : 9;
    unsigned int bit9 : 1;
    unsigned int high : 22;
};

union Flags_0049cc20 {
    unsigned int all;
    FlagsBits_0049cc20 bits;
};

struct UnitType_0049cc20 {
    char unknown_0[0x68];
    int f_68;
    int f_6c;
    int f_70;
    char unknown_74[0xdc - 0x74];
    int f_dc;
    char unknown_e0[0xe6 - 0xe0];
    unsigned short f_e6;
    char unknown_e8[0xea - 0xe8];
    unsigned short f_ea;
    char unknown_ec[0x111 - 0xec];
    Flags_0049cc20 flags;              // +0x111
};

struct Shot_0049cc20 {
    char unknown_0[0x8];
    int field_8;
    UnitType_0049cc20* def;            // +0xc, the unit type that fired
    char unknown_10[0x1b - 0x10];
    unsigned char field_1b;            // +0x1b
};

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
    void StartScript(const char* name, int param_2, int param_3);
};

struct Head_0049cc20 {
    char pad[0x4c];
    short heading;                     // +0x4c
    char pad2[0x80 - 0x4c - 2];
};

struct Unit {
    char unknown_0[0x1a];
    union {
        short aim[4][7][2];            // +0x1a, the four aim directions
        Head_0049cc20 head;            // heading at +0x66
    };
    CobScript* anims;                  // +0x9a
};

struct Proj_0049cc20 {
    UnitType_0049cc20* unit;           // +0x0
    char unknown_4[0x1c - 0x4];
    Vec3_0049cc20 dir;                 // +0x1c
    char unknown_28[0x36 - 0x28];
    short angle;                       // +0x36
    short pitch0;                      // +0x38
    int field_3a;
    char unknown_3e[0x46 - 0x3e];
    int time;                          // +0x46
    char unknown_4a[0x4e - 0x4a];
    int field_4e;
    char unknown_52[0x56 - 0x52];
    int field_56;
    char unknown_5a[0x60 - 0x5a];
    short active;                      // +0x60
    char unknown_62[0x69 - 0x62];
    unsigned short flags;              // +0x69
};

struct Game {
    char unknown_0[0x141f3];
    int projCount;                     // +0x141f3
    Proj_0049cc20* projs;              // +0x141f7
    char unknown_141fb[0x38a47 - 0x141fb];
    int field_38a47;                   // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_00509678[4];

void __stdcall InitProjectile(Proj_0049cc20* proj, UnitType_0049cc20* shot, Vec3_0049cc20* pos,
                            Vec3_0049cc20* aim, int field_5, Unit* unit);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
void __stdcall FUN_004729d0(Vec3_0049cc20* p, short index);

// FUNCTION: 0x49cc20
int __stdcall FireVLaunchProjectile(Shot_0049cc20* shot, Unit* unit, Vec3_0049cc20* pos,
                           Vec3_0049cc20* aim, int param_5, int param_6)
{
    Proj_0049cc20* proj = 0;
    if (g_game->projCount < 300) {
        proj = &g_game->projs[g_game->projCount++];
        proj->flags &= ~2;
        proj->field_4e = 0;
    }
    if (!proj)
        return 0;

    InitProjectile(proj, shot->def, pos, aim, g_game->field_38a47, unit);
    proj->angle = 0;
    proj->pitch0 = 0x4000;
    shot->field_8 = 0;
    if (shot->def->f_6c) {
        proj->field_3a = shot->def->f_6c;
    } else if (shot->def->f_70 == 0) {
        proj->field_3a = shot->def->f_68;
    } else {
        proj->field_3a = 0;
    }
    memset(&proj->dir, 0, 12);
    UnitType_0049cc20* u = proj->unit;
    if (u->f_68 != 0 && !(u->flags.all & 0x8000000)) {
        proj->time = (u->f_dc << 16) / (unsigned int)u->f_68 + g_game->field_38a47;
    } else {
        proj->time = g_game->field_38a47 + u->f_e6;
    }
    proj->field_4e = param_5;
    proj->field_56 = param_6;
    proj->active = shot->def->f_ea;
    ((CobScript*)unit->anims)->StartScript(DAT_00509678[(shot->field_1b >> 2) & 3], 0, 0);
    short angle = unit->aim[(shot->field_1b >> 2) & 3][0][0] - unit->head.heading;
    int a = -FUN_004b70ef(angle, 800);
    int b = -FUN_004b7123(angle, 800);
    unit->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);
    if (shot->def->flags.bits.bit9)
        FUN_004729d0(pos, 9);
    return 1;
}
