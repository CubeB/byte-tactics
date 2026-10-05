// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The animated-radius clamp has to be a single ternary
//     int radius = (t < 8) ? 8 : t;
// Any if-form makes MSVC spill radius (radius homed in the view argument slot
// and the 8 store hoisted above the flags test) and swap view into ebp, which
// costs the whole block. The ternary keeps radius in ebp and view in edi and
// reproduces the original `mov ebp,8 / cmp edx,8 / jb / mov ebp,edx` exactly.
// `t` is unsigned (jb), r and radius are int (jl), and the position is read as
// `&node->unit->pos` (a `unit` local makes MSVC emit `lea` and drops to 49.6%).
// Suspected bug: the weapon3 test reads slots[0].flags (unit+0x1f) but the
// range from slots[2].weapon (unit+0x48); slots[2].flags is at unit+0x57.
#pragma pack(push, 1)

struct Pos_004390a0 {
    unsigned short x_frac;              // +0x0
    short x;                            // +0x2
    unsigned short y_frac;              // +0x4
    short y;                            // +0x6
    unsigned short z_frac;              // +0x8
    short z;                            // +0xa
};

struct Weapon_004390a0 {
    char unknown_0[0xd6];
    unsigned short field_d6;            // +0xd6
    char unknown_d8[0xdc - 0xd8];
    int range;                          // +0xdc
};

struct Def_004390a0 {
    char unknown_0[0x202];
    short sight;                        // +0x202
    short radar;                        // +0x204
    short sonar;                        // +0x206
    short minCloakDistance;             // +0x208
    short radarJam;                     // +0x20a
    short sonarJam;                     // +0x20c
    char unknown_20e[0x212 - 0x20e];
    unsigned short buildDistance;       // +0x212
    unsigned short maneuver;            // +0x214
    unsigned short attackLength;        // +0x216
    unsigned short kamikazeDistance;    // +0x218
    char unknown_21a[0x220 - 0x21a];
    Weapon_004390a0* weapon_220;        // +0x220
    char unknown_224[0x241 - 0x224];
    unsigned int flags;                 // +0x241
};

struct Slot_004390a0 {
    Weapon_004390a0* weapon;            // +0x0
    char unknown_4[0xf - 4];
    unsigned char flags;                // +0xf
    char unknown_10[0x1c - 0x10];
};

struct Unit_004390a0 {
    int field_0;                        // +0x0
    char unknown_4[0x10 - 4];
    Slot_004390a0 slots[3];             // +0x10
    char unknown_64[0x6a - 0x64];
    Pos_004390a0 pos;                   // +0x6a
    char unknown_76[0x92 - 0x76];
    Def_004390a0* def;                  // +0x92
    char unknown_96[0x10e - 0x96];
    unsigned char field_10e;            // +0x10e
};

struct Node_004390a0 {
    char unknown_0[0xe];
    Unit_004390a0* unit;                // +0xe
};

struct Game_004390a0 {
    char unknown_0[0xdcf];
    unsigned char field_dcf;            // +0xdcf
    char unknown_dd0[0xdd7 - 0xdd0];
    unsigned char field_dd7;            // +0xdd7
    char unknown_dd8[0xdd9 - 0xdd8];
    unsigned char field_dd9;            // +0xdd9
    unsigned char field_dda;            // +0xdda
    char unknown_ddb[0x38a47 - 0xddb];
    unsigned int frame;                 // +0x38a47
    char unknown_38a4b[0x391bf - 0x38a4b];
    int field_391bf;                    // +0x391bf
};

struct View_004390a0;

#pragma pack(pop)

extern Game_004390a0* g_game;

void __stdcall FUN_00438ea0(void* surface, View_004390a0* view, Pos_004390a0* pos,
                            int value, int color, const char* text, int index);

// FUNCTION: 0x4390a0
void __stdcall FUN_004390a0(void* surface, View_004390a0* view, Node_004390a0* node,
                            int unused1, int unused2)
{
    Unit_004390a0* unit = node->unit;
    Def_004390a0* def = unit->def;
    int index = 0;
    if (g_game->field_391bf == 0) {
        short mincloak = def->minCloakDistance;
        if (mincloak != 0 && (unit->field_10e & 4)) {
            FUN_00438ea0(surface, view, &node->unit->pos, mincloak, g_game->field_dda, 0, 0);
        }
        if ((def->flags & 0x10000000) && def->weapon_220 != 0) {
            int r = def->weapon_220->field_d6;
            r = r >> 1;
            unsigned int t = (g_game->frame % 60) * r * 2 / 60;
            int radius = (t < 8) ? 8 : t;
            if (radius >= r)
                radius = r;
            FUN_00438ea0(surface, view, &node->unit->pos, radius, g_game->field_dd7, 0, 0);
            if (unit->field_0 != 0) {
                FUN_00438ea0(surface, view, &node->unit->pos, def->kamikazeDistance,
                             g_game->field_dd7, 0, 0);
                return;
            }
            FUN_00438ea0(surface, view, &node->unit->pos, def->sight, g_game->field_dd7, 0, 0);
            return;
        }
    } else {
        short mincloak = def->minCloakDistance;
        if (mincloak != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, mincloak, g_game->field_dd9,
                         "mincloak", index++);
        }
        if (def->sight != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, def->sight, g_game->field_dd9,
                         "sight", index++);
        }
        if (def->radar != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, def->radar, g_game->field_dd9,
                         "radar", index++);
        }
        if (def->sonar != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, def->sonar, g_game->field_dd9,
                         "sonar", index++);
        }
        if (def->radarJam != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, def->radarJam, g_game->field_dd9,
                         "radarjam", index++);
        }
        if (def->sonarJam != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, def->sonarJam, g_game->field_dd9,
                         "sonarjam", index++);
        }
        if (def->buildDistance != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, def->buildDistance, g_game->field_dd9,
                         "build distance", index++);
        }
        if (def->maneuver != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, def->maneuver, g_game->field_dd9,
                         "maneuver", index++);
        }
        if (def->kamikazeDistance != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, def->kamikazeDistance,
                         g_game->field_dd9, "kamikazedistance", index);
        }
        int color;
        if (g_game->frame & 1)
            color = g_game->field_dcf;
        else
            color = g_game->field_dd7;
        if ((unit->slots[0].flags & 2) && unit->slots[0].weapon->range != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, unit->slots[0].weapon->range, color,
                         "weapon1 range", 0);
        }
        if ((unit->slots[1].flags & 2) && unit->slots[1].weapon->range != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, unit->slots[1].weapon->range, color,
                         "weapon2 range", 1);
        }
        // Original bug: tests slots[0].flags (unit+0x1f) but reads slots[2].weapon
        // (unit+0x48); slots[2].flags is at unit+0x57.
        if ((unit->slots[0].flags & 2) && unit->slots[2].weapon->range != 0) {
            FUN_00438ea0(surface, view, &node->unit->pos, unit->slots[2].weapon->range, color,
                         "weapon3 range", 2);
        }
    }
}
