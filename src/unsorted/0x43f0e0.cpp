// Decompiled by Claude Sonnet 5.5, finished by DeepSeek V4.1 Flash and GPT-6. Names are
// provisional. Partial, 47.5%. Loads the definition after target validation, restoring the native
// frame. Unit/definition registers still differ; iostream improves the corrected implementation.
#include <iostream>
#include <windows.h>

#pragma pack(push, 1)
class Class_00438760 {
  public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760() { index = 0; }
};

union Flags110_0043f0e0 {
    unsigned int raw;
    struct {
        unsigned int bits_0 : 31;
        unsigned int flag_31 : 1;
    };
};

union Flags241_0043f0e0 {
    unsigned int raw;
    struct {
        unsigned int bits_0 : 11;
        unsigned int flag_11 : 1;
        unsigned int bits_12 : 16;
        unsigned int flag_28 : 1;
        unsigned int bits_29 : 3;
    };
};

union Flags111_0043f0e0 {
    unsigned int raw;
    struct {
        unsigned int bits_0 : 8;
        unsigned int flag_8 : 1;
        unsigned int bits_9 : 8;
        unsigned int flag_17 : 1;
        unsigned int bits_18 : 14;
    };
};

struct Game_0043f0e0 {
    char unknown_0[0x2a42];
    unsigned char localPlayer;    // +0x2a42
    unsigned char localPlayerBit; // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int mapWidth; // +0x14233
    char unknown_14237[0x14253 - 0x14237];
    int unitCount; // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    char* units;                // +0x1426f
    unsigned short* visibility; // +0x14273
    char unknown_14277[0x1427f - 0x14277];
    unsigned char threshold; // +0x1427f
    char unknown_14280[0x37efa - 0x14280];
    int flag37efa; // +0x37efa
};

struct Node_0043f0e0 {
    char unknown_0[0x111];
    union {
        unsigned int f111; // +0x111
        Flags111_0043f0e0 f111bits;
    };
};

struct Def_0043f0e0 {
    char unknown_0[0x146];
    char unknown_146[0x156 - 0x146];
    int f156; // +0x156
    char unknown_15a[0x170 - 0x15a];
    short f170; // +0x170
    char unknown_172[0x1ee - 0x172];
    Node_0043f0e0* f1ee; // +0x1ee
    char unknown_1f2[0x1fa - 0x1f2];
    unsigned int f1fa; // +0x1fa
    char unknown_1fe[0x241 - 0x1fe];
    union {
        unsigned int f241; // +0x241
        Flags241_0043f0e0 f241bits;
    };
    unsigned int f245; // +0x245
};

struct Player_0043f0e0 {
    char unknown_0[0x80];
    unsigned int width;  // +0x80
    unsigned int height; // +0x84
    char unknown_88[0x108 - 0x88];
    char allied[1]; // +0x108
    char unknown_109[0x146 - 0x109];
    unsigned char index; // +0x146
};

struct Unit_0043f0e0 {
    int moving; // +0x0
    char unknown_4[0x10 - 0x4];
    Node_0043f0e0* f10; // +0x10
    char unknown_14[0x2c - 0x14];
    Node_0043f0e0* f2c; // +0x2c
    char unknown_30[0x3b - 0x30];
    unsigned char f3b; // +0x3b
    char unknown_3c[0x70 - 0x3c];
    short f70; // +0x70
    char unknown_72[0x86 - 0x72];
    Unit_0043f0e0* f86; // +0x86
    char unknown_8a[0x92 - 0x8a];
    Def_0043f0e0* def;       // +0x92
    Player_0043f0e0* player; // +0x96
    char unknown_9a[0xfb - 0x9a];
    int ffb; // +0xfb
    unsigned char unknown_ff[1];
    char unknown_100[0x104 - 0x100];
    float f104; // +0x104
    short f108; // +0x108
    char unknown_10a[0x110 - 0x10a];
    union {
        unsigned int f110; // +0x110
        Flags110_0043f0e0 f110bits;
    };
};

struct Pos_0043f0e0 {
    short xf, x;
    short yf, y;
    short zf, z;
};

struct Cell_0043f0e0 {
    char unknown_0[8];
    unsigned short feature; // +0x8
    unsigned char offsetY;  // +0xa
    unsigned char offsetX;  // +0xb
    char unknown_c;
};

struct Thing_0043f0e0 {
    char unknown_0[0xfe];
    unsigned char ffe; // +0xfe
};
#pragma pack(pop)

extern Game_0043f0e0* g_game;

Cell_0043f0e0* __stdcall FUN_004815a0(Pos_0043f0e0* pos);
class Class_004899b0 {
  public:
    int FUN_004899b0(Unit_0043f0e0* other);
};
class Class_00489a70 {
  public:
    int FUN_00489a90(Unit_0043f0e0* other);
};
Class_00438760 __stdcall FUN_0043f0e0(unsigned char mode, Unit_0043f0e0* unit,
                                      Unit_0043f0e0* target, Pos_0043f0e0* pos);

static inline int IsVtol(Def_0043f0e0* def) { return (def->f241 >> 11) & 1; }

static inline Class_00438760 Pick(Def_0043f0e0* def, const char* vtol, const char* ground) {
    return Class_00438760(IsVtol(def) ? vtol : ground);
}

static inline int Visible(Unit_0043f0e0* unit, Pos_0043f0e0* pos) {
    Player_0043f0e0* p = unit->player;
    int x = pos->x >> 5;
    int y = (pos->z - (pos->y >> 1)) >> 5;
    return (unsigned int)x < p->width && (unsigned int)y < p->height &&
           ((1 << g_game->localPlayerBit) & g_game->visibility[p->width * y + x]) != 0;
}

static inline Thing_0043f0e0* Lookup(Pos_0043f0e0* pos) {
    Cell_0043f0e0* cell = FUN_004815a0(pos);
    if (!cell)
        return 0;
    unsigned short id = cell->feature;
    if (id >= 0xfffb) {
        if (id != 0xfffe)
            return 0;
        id = (cell - (cell->offsetY * g_game->mapWidth + cell->offsetX))->feature;
        if (id >= 0xfffb)
            return 0;
    } else if ((int)id >= g_game->unitCount) {
        return 0;
    }
    return (Thing_0043f0e0*)(g_game->units + (id << 8));
}

static inline int Marked(Thing_0043f0e0* t) { return t && (t->ffe & 0x80); }

// FUNCTION: 0x43f0e0
Class_00438760 __stdcall FUN_0043f0e0(unsigned char mode, Unit_0043f0e0* unit,
                                      Unit_0043f0e0* target, Pos_0043f0e0* pos) {
    Def_0043f0e0* def;
    int friendly = 0;
    int enemy = 0;
    if (target) {
        if (!(target->f110 & 0x10000000))
            goto none;
        if (unit->player->allied[target->player->index] != 0)
            friendly = 1;
        else
            enemy = 1;
    }
    def = unit->def;
    switch (mode) {
    case 3: {
        if (!(def->f245 & 0x10))
            break;
        Flags110_0043f0e0 flags;
        flags.raw = unit->f110;
        if (flags.flag_31) {
            Node_0043f0e0* node = unit->f10;
            if (!enemy) {
                if (node->f111bits.flag_17)
                    break;
                if (!(def->f241 & 0x800))
                    return Class_00438760("SUPPRESS");
                if (def->f1ee->f111bits.flag_8)
                    return Class_00438760("AIRSTRIKE");
                return Class_00438760("AIRTOGROUND");
            }
            if ((target->f110 & 3) != 2 && (node->f111 & 0x20000))
                break;
            Def_0043f0e0* tdef = target->def;
            int reach = tdef->f170 + target->f70;
            if (reach >= g_game->threshold)
                goto reached;
            if (!(node->f111 & 0x10000)) {
                if (!(unit->f3b & 2))
                    break;
                if (!(unit->f2c->f111 & 0x10000))
                    break;
            }
            if (reach < g_game->threshold)
                goto after;
        reached:
            if (def->f241 & 0x1000) {
                if (node->f111 & 0x10000)
                    break;
                if ((unit->f3b & 2) && (unit->f2c->f111 & 0x10000))
                    return Class_00438760();
            }
        after:
            unsigned int f = def->f241;
            if (def->f241bits.flag_11) {
                unsigned int air = def->f1ee->f111 & 0x100;
                if (air && !(tdef->f241 & 0x800))
                    return Class_00438760("AIRSTRIKE");
                if (!air && (tdef->f241 & 0x800))
                    return Class_00438760("AIRTOAIR");
                unsigned int tv = tdef->f241 & 0x800;
                if (!tv && !(f & 0x8000000))
                    return Class_00438760("AIRTOGROUND");
                if (!tv && (f & 0x8000000))
                    return Class_00438760("AIRTOGROUNDHOVER");
                break;
            }
            if (unit->moving != 0)
                return Class_00438760("ATTACK_CHASE");
            if (flags.raw & 0x20000000)
                return Class_00438760("ATTACK_NOMOVE");
        }
        if (def->f241bits.flag_28)
            return Class_00438760("ATTACK_KAMIKAZE");
        break;
    }
    case 9:
        if (def->f245 & 0x40) {
            if (unit->moving == 0)
                return Class_00438760("QPATROL");
            if ((def->f245 >> 9) & 1) {
                if ((def->f241 >> 11) & 1)
                    return Class_00438760("VTOL_REPAIRPATROL");
                return Class_00438760("REPAIRPATROL");
            }
            if ((def->f241 >> 11) & 1)
                return Class_00438760("VTOL_PATROL");
            return Class_00438760("PATROL");
        }
        break;
    case 8:
        if (!((Class_004899b0*)unit)->FUN_004899b0(target))
            break;
        if (target->f104 == 0.0f)
            return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
        return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
    case 7:
        if (!(def->f245 & 0x20) || !friendly)
            break;
        return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
    case 12: {
        if (!(def->f245 & 0x400))
            break;
        Thing_0043f0e0* t = Lookup(pos);
        if (pos) {
            if ((def->f245 & 0x800) && Visible(unit, pos) && Marked(t))
                return Class_00438760("RESURRECT");
            if (pos && Visible(unit, pos) && Marked(t))
                return Pick(def, "VTOL_RECLAIM", "RECLAIM");
        }
        if (!target)
            break;
        return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
    }
    case 13:
        if ((def->f245 & 0x1000) && target && unit->player != target->player)
            return Class_00438760("CAPTURE");
        break;
    case 6:
        if (!target || !((Class_00489a70*)unit)->FUN_00489a90(target))
            break;
        return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
    case 5:
        if ((def->f245 & 0x100) && (def->f241 & 0x800) && target && (target->def->f241 & 0x200))
            return Class_00438760("VTOL_LANDING");
        if ((def->f245 >> 8) & 1)
            return Pick(def, "VTOL_UNLOAD", "GROUND_UNLOAD");
        break;
    case 14:
        if (def->f156 == 0 || unit->moving == 0)
            break;
        return Pick(def, "VTOL_MOBILEBUILD", "MOBILEBUILD");
    case 4:
        if ((def->f245 >> 14) & 1)
            return Class_00438760("ATTACKSPECIAL");
        break;
    case 11:
        return Class_00438760("TELEPORT");
    case 10:
        return Class_00438760("STOP");
    case 2:
        if (!(def->f245 & 0x80))
            break;
        if (unit->moving == 0)
            return Class_00438760("QMOVE");
        if (target) {
            if ((def->f245 & 0x1000) && enemy)
                return Class_00438760("CAPTURE");
            if (!((def->f245 & 0x400) && enemy)) {
                if (friendly) {
                    if (((Class_004899b0*)unit)->FUN_004899b0(target) && target->f104 != 0.0f)
                        return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
                    if (((Class_004899b0*)unit)->FUN_004899b0(target) &&
                        (unsigned int)target->f108 < target->def->f1fa)
                        return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
                }
                if ((def->f241 & 0x800) && friendly && (target->def->f241 & 0x200))
                    return Class_00438760("VTOL_LANDING");
                if (((Class_00489a70*)unit)->FUN_00489a90(target))
                    return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
                if ((def->f245 & 0x20) && friendly)
                    return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
                return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
            }
            return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
        }
        return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
    case 1: {
        if (g_game->flag37efa == 1) {
            if ((def->f245 & 0x10) && enemy)
                return FUN_0043f0e0(3, unit, target, pos);
            if ((def->f245 & 0x400) && enemy)
                return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
            if (friendly && ((Class_004899b0*)unit)->FUN_004899b0(target) && target->f104 != 0.0f)
                return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
            if (friendly && ((Class_004899b0*)unit)->FUN_004899b0(target))
                return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
            if ((def->f241 & 0x800) && friendly && (target->def->f241 & 0x200))
                return Class_00438760("VTOL_LANDING");
            if (target && ((Class_00489a70*)unit)->FUN_00489a90(target))
                return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
            if ((def->f245 & 0x20) && friendly)
                return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
            if ((def->f245 & 0x800) && pos && Visible(unit, pos) && Marked(Lookup(pos)))
                return Class_00438760("RESURRECT");
            if ((def->f245 & 0x400) && pos && Visible(unit, pos) && Marked(Lookup(pos)))
                return Pick(def, "VTOL_RECLAIM", "RECLAIM");
            if (!(def->f245 & 0x80) || unit->moving == 0)
                break;
        } else {
            if ((def->f245 & 0x10) && enemy)
                return FUN_0043f0e0(3, unit, target, pos);
            if ((def->f245 & 0x400) && enemy)
                return FUN_0043f0e0(0xc, unit, target, pos);
            if (target && ((Class_004899b0*)unit)->FUN_004899b0(target) && target->f104 != 0.0f)
                return FUN_0043f0e0(8, unit, target, pos);
            if (target && target->unknown_ff[0] == g_game->localPlayer && (target->f110 & 0x20) &&
                target->f104 == 0.0f && target->ffb == 0 &&
                (!target->f86 || (target->f86->f110 & 0x40000000)))
                break;
            if ((def->f245 & 0x800) && pos && Visible(unit, pos) && Marked(Lookup(pos)))
                return Class_00438760("RESURRECT");
            if ((def->f245 & 0x400) && pos && Visible(unit, pos) && Marked(Lookup(pos)))
                return Pick(def, "VTOL_RECLAIM", "RECLAIM");
            if (!(def->f245 & 0x80) || unit->moving == 0)
                break;
        }
        return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
    }
    }
none:
    return Class_00438760();
}
