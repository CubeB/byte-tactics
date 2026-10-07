// Decompiled by Claude Sonnet 5.5, finished by DeepSeek V4.1 Flash and GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by claude-opus-5-5. Names are provisional.
//
// Returns the order type (Class_00438760, built from its name) for an order of
// type `mode` given by `unit` on `target` / `pos`. GetOrderCursor, the function just before this one
// in the exe, is its sibling that returns the cursor code.
// Keep the headers and the unused declarations below: their count before the function matters.
#include <windows.h>
#include <stdio.h>
#include <math.h>

#pragma pack(push, 1)

struct Feature_0043e490 {
    char unknown_0[0xfe];
    unsigned char flags; // +0xfe
    char unknown_ff;
};

struct Node_0043e490 {
    char unknown_0[0x111];
    union {
        unsigned int f111; // +0x111
        struct {
            unsigned int b0_7 : 8;
            unsigned int b8 : 1;
            unsigned int b9_31 : 23;
        } f111b;
    };
};

struct Flags_0043e490 {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int b8 : 1;
    unsigned int b9 : 1;
    unsigned int b10 : 1;
    unsigned int b11 : 1;
    unsigned int b12 : 1;
    unsigned int b13 : 1;
    unsigned int b14 : 1;
    unsigned int b15_31 : 17;
};

struct Def_0043e490 {
    char unknown_0[0x156];
    int f156; // +0x156
    char unknown_15a[0x1ee - 0x15a];
    Node_0043e490* f1ee; // +0x1ee
    char unknown_1f2[0x241 - 0x1f2];
    union {
        unsigned int f241; // +0x241
        Flags_0043e490 f241b;
    };
    union {
        unsigned int f245; // +0x245
        Flags_0043e490 f245b;
    };
};

struct Size_0043e490 {
    unsigned int width;  // +0x0
    unsigned int height; // +0x4
};

struct Player_0043e490 {
    char unknown_0[0x80];
    Size_0043e490 size; // +0x80
    char unknown_88[0x108 - 0x88];
    char allied[0x3e]; // +0x108
    unsigned char index; // +0x146
};

struct Stats_0043e490 {
    char unknown_0[0x8c];
    float f8c; // +0x8c
    char unknown_90[0x98 - 0x90];
    float f98; // +0x98
};

struct Limits_0043e490 {
    char unknown_0[0xc0];
    float fc0; // +0xc0
    float fc4; // +0xc4
};

struct Unit_0043e490 {
    int moving; // +0x0
    char unknown_4[0x10 - 0x4];
    Node_0043e490* f10; // +0x10
    char unknown_14[0x48 - 0x14];
    Limits_0043e490* f48; // +0x48
    char unknown_4c[0x6a - 0x4c];
    char f6a[0x86 - 0x6a]; // +0x6a
    Unit_0043e490* f86; // +0x86
    char unknown_8a[0x92 - 0x8a];
    Def_0043e490* def;       // +0x92
    Player_0043e490* player; // +0x96
    char unknown_9a[0xec - 0x9a];
    Stats_0043e490* fec; // +0xec
    char unknown_f0[0xfb - 0xf0];
    int ffb;             // +0xfb
    unsigned char owner; // +0xff
    char unknown_100[0x104 - 0x100];
    float f104; // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int f110; // +0x110
};

struct Pos_0043e490 {
    short xf, x;
    short yf, y;
    short zf, z;
};

struct Cell_0043e490 {
    char unknown_0[8];
    unsigned short feature; // +0x8
    unsigned char offsetY;  // +0xa
    unsigned char offsetX;  // +0xb
    unsigned char flags;    // +0xc
};
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

union Flags245_0043f0e0 {
    unsigned int raw;
    struct {
        unsigned int bits_0 : 4;
        unsigned int flag_4 : 1;
        unsigned int flag_5 : 1;
        unsigned int flag_6 : 1;
        unsigned int flag_7 : 1;
        unsigned int flag_8 : 1;
        unsigned int flag_9 : 1;
        unsigned int flag_10 : 1;
        unsigned int flag_11 : 1;
        unsigned int flag_12 : 1;
        unsigned int bits_13 : 1;
        unsigned int flag_14 : 1;
        unsigned int bits_15 : 17;
    };
};

struct Game {
    char unknown_0[0x2a42];
    unsigned char localPlayer;    // +0x2a42
    unsigned char localPlayerBit; // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int mapWidth; // +0x14233
    char unknown_14237[0x14253 - 0x14237];
    union {
        int unitCount;    // +0x14253
        int featureCount;
    };
    char unknown_14257[0x1426f - 0x14257];
    union {
        char* units; // +0x1426f
        struct Feature_0043e490* features;
    };
    unsigned short* visibility; // +0x14273
    char unknown_14277[0x1427f - 0x14277];
    unsigned char threshold; // +0x1427f
    char unknown_14280[0x37efa - 0x14280];
    union {
        int flag37efa; // +0x37efa
        int multiplayer;
    };
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
    union {
        unsigned int f245; // +0x245
        Flags245_0043f0e0 f245bits;
    };
};

struct Size_0043f0e0 {
    unsigned int width;  // +0x0
    unsigned int height; // +0x4
};

struct Player_0043f0e0 {
    char unknown_0[0x80];
    Size_0043f0e0 size; // +0x80
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

extern Game* g_game;

Cell_0043f0e0* __stdcall GetMapCellAtPosition(Pos_0043f0e0* pos);
class Unit {
  public:
    int CanLoad(Unit_0043f0e0* other);
    int CanLoad(Unit_0043e490* other);
    int CanRepair(Unit_0043f0e0* other);
    int CanRepair(Unit_0043e490* other);
    int CanReclaim(Unit_0043e490* other);
};
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall IsUnitCommander(Unit*);
int __stdcall FindLandingPad(Unit*, int);
unsigned short __stdcall ChooseBuildOption(unsigned int, Unit*);
void __stdcall ClearWeaponTarget(Unit*, int);
void __stdcall DetonateUnitWeapon(Unit*, int);
void __stdcall DrawUnit(void*, Unit*);

Class_00438760 __stdcall GetOrderType(unsigned char mode, Unit_0043f0e0* unit,
                                      Unit_0043f0e0* target, Pos_0043f0e0* pos);

// GetOrderCursor as in 0x43e490.cpp (the annotated copy). It precedes GetOrderType in the
// original file and is compiled first here for the compiler state it leaves; see the top.
Cell_0043e490* __stdcall GetMapCellAtPosition(Pos_0043e490* pos);
int __stdcall WeaponCanReachPos(Unit_0043e490* unit, void* slot, Pos_0043e490* pos, int which);
int __stdcall WeaponCanReachUnit(Unit_0043e490* unit, Unit_0043e490* target, int which);
int __stdcall GetOrderCursor(unsigned char mode, Unit_0043e490* unit, Unit_0043e490* target,
                           Pos_0043e490* pos);

// The same test as IsPointVisible: is the map square under pos in sight of the
// local player? The width is read again through unit->player for the index;
// MSVC folds it back into the CSE'd load, which it then reloads into edx for
// the imul as the original does.
static inline int Visible(Unit_0043e490* unit, Pos_0043e490* pos) {
    Player_0043e490* p = unit->player;
    int y, x;
    x = pos->x >> 5;
    y = (pos->z - (pos->y >> 1)) >> 5;
    return (unsigned int)x < p->size.width && (unsigned int)y < p->size.height &&
           0 != ((1 << g_game->localPlayerBit) & g_game->visibility[x + y * unit->player->size.width]);
}

// The feature on a map cell, as GetFeature in 0x4237d0.cpp but with the id in
// a local: 0xfffe marks a cell covered by a larger feature whose origin cell
// lies (offsetY, offsetX) cells back.
static inline Feature_0043e490* GetFeature(Cell_0043e490* cell) {
    if (cell == 0)
        return 0;
    unsigned short id = cell->feature;
    if (id < 0xfffb) {
        if (id >= g_game->featureCount)
            return 0;
        return &g_game->features[id];
    }
    if (id != 0xfffe)
        return 0;
    id = (cell - (cell->offsetY * g_game->mapWidth + cell->offsetX))->feature;
    if (id >= 0xfffb)
        return 0;
    return &g_game->features[id];
}

// Returns `result` from the calling function when the unit's def has `mask`
// set and pos holds a visible feature flagged 0x80. A statement macro in the
// usual do/while(0) form: the loop emits no code, but it is load-bearing for
// register allocation. Without it (the same if-statement written out at each
// site) case 2 loads unit into ebx before the target test instead of after it,
// and the extra reload block costs 20 bytes (85.0%).
#define RECLAIM_CHECK(def, unit, pos, mask, result)                  \
    do {                                                             \
        if (((def)->f245 & (mask)) && Visible((unit), (pos))) {      \
            Feature_0043e490* f = GetFeature(GetMapCellAtPosition(pos));     \
            if (f && (f->flags & 0x80))                              \
                return (result);                                     \
        }                                                            \
    } while (0)

static inline int Selectable(Unit_0043e490* t) {
    return t && t->owner == g_game->localPlayer && (t->f110 & 0x20) && t->f104 == 0.0f && t->ffb == 0 &&
           (t->f86 == 0 || (t->f86->f110 & 0x40000000));
}

int __stdcall GetOrderCursor(unsigned char mode, Unit_0043e490* unit, Unit_0043e490* target,
                           Pos_0043e490* pos) {
    Def_0043e490* def;
    Node_0043e490* node;
    int friendly;
    int enemy;
    friendly = 0;
    enemy = 0;
    def = unit->def;
    if (target) {
        if (unit->player->allied[target->player->index])
            friendly = 1;
        else
            enemy = 1;
    }

    switch (mode) {
    case 3:
        if ((def->f245 & 0x10) && def->f1ee->f111b.b8)
            return 2;
        if (def->f245b.b4) {
            node = unit->f10;
            if (unit->moving)
                return 1;
            if (target)
                return WeaponCanReachUnit(unit, target, 0) ? 1 : 3;
            if (!WeaponCanReachPos(unit, unit->f6a, pos, 0) || (node->f111 & 0x20000))
                return 3;
            return 1;
        }
        break;
    case 9:
        return def->f245b.b6 ? 7 : 0x13;
    case 8:
        return ((Unit*)unit)->CanRepair(target) ? 6 : 0x13;
    case 7:
        if (!(def->f245 & 0x20) || !friendly)
            break;
        if ((def->f241 & 0x800) || !(target->def->f241 & 0x800))
            return 5;
        break;
    case 12:
        RECLAIM_CHECK(def, unit, pos, 0x400, 0xb);
        if (target && ((Unit*)unit)->CanReclaim(target))
            return 0xb;
        break;
    case 13:
        if ((def->f245 & 0x1000) && target && unit->player != target->player)
            return 4;
        break;
    case 6:
        if (!target || !((Unit*)unit)->CanLoad(target))
            break;
        return def->f241b.b11 ? 8 : 0xc;
    case 5:
        return def->f245b.b8 ? 0xd : 0x13;
    case 14:
        if (def->f156 == 0 || unit->moving == 0)
            break;
        return 0x10;
    case 4:
        if (def->f245b.b14) {
            Limits_0043e490* l = unit->f48;
            Stats_0043e490* s = unit->fec;
            if (s->f8c < l->fc0 || s->f98 < l->fc4)
                return 3;
            return 1;
        }
        break;
    case 11:
        return 9;
    case 2:
        if (!(def->f245 & 0x80))
            break;
        RECLAIM_CHECK(def, unit, pos, 0x800, 0xa);
        if (!target || !unit->moving)
            return 0xe;
        if ((def->f245 & 0x1000) && enemy)
            return 4;
        if (enemy && ((Unit*)unit)->CanReclaim(target))
            return 0xb;
        if (friendly && ((Unit*)unit)->CanRepair(target) && target->f104 != 0.0f)
            return 6;
        if (friendly && ((Unit*)unit)->CanRepair(target))
            return 6;
        if ((def->f241 & 0x800) && (target->def->f241 & 0x200))
            return 0xd;
        if (((Unit*)unit)->CanLoad(target))
            return def->f241b.b11 ? 8 : 0xc;
        if ((def->f245 & 0x20) && friendly)
            return 5;
        return 0xe;
    case 1:
        if (g_game->multiplayer == 1) {
            if (Selectable(target))
                return 0xf;
            if (enemy)
                return 0x11;
            if (friendly)
                return 0x12;
            RECLAIM_CHECK(def, unit, pos, 0x800, 0x12);
            RECLAIM_CHECK(def, unit, pos, 0x400, 0x12);
            break;
        }
        if ((def->f245 & 0x10) && enemy)
            return GetOrderCursor(3, unit, target, pos);
        if ((def->f245 & 0x400) && enemy)
            return GetOrderCursor(0xc, unit, target, pos);
        if (target && ((Unit*)unit)->CanRepair(target) && target->f104 != 0.0f)
            return 6;
        if (Selectable(target))
            return 0xf;
        RECLAIM_CHECK(def, unit, pos, 0x800, 0xa);
        RECLAIM_CHECK(def, unit, pos, 0x400, 0xb);
        return def->f245b.b7 ? 0xe : 0x13;
    }
    return 0x13;
}

// A ternary: the flag is tested before the name is loaded.
static inline Class_00438760 Pick(Def_0043f0e0* def, const char* vtol, const char* ground) {
    return Class_00438760(def->f241bits.flag_11 ? vtol : ground);
}

static inline int Visible(Unit_0043f0e0* unit, Pos_0043f0e0* pos) {
    Player_0043f0e0* p = unit->player;
    int x = pos->x >> 5;
    int y = (pos->z - (pos->y >> 1)) >> 5;
    return (unsigned int)x < p->size.width && (unsigned int)y < p->size.height &&
           0 != ((1 << g_game->localPlayerBit) & g_game->visibility[x + y * unit->player->size.width]);
}

#define FEATURE_CHECK(def, unit, pos, mask, result)                                            \
    do {                                                                                       \
        if (((def)->f245 & (mask)) && (pos) && Visible((unit), (pos)) && Marked(Lookup(pos)))  \
            return (result);                                                                   \
    } while (0)

static inline int Marked(Thing_0043f0e0* t) { return t && (t->ffe & 0x80); }

static inline Thing_0043f0e0* Lookup(Pos_0043f0e0* pos) {
    Cell_0043f0e0* cell = GetMapCellAtPosition(pos);
    if (!cell)
        return 0;
    unsigned short id = cell->feature;
    if (id < 0xfffb) {
        if (id >= g_game->unitCount)
            return 0;
        return (Thing_0043f0e0*)(g_game->units + (id << 8));
    }
    if (id != 0xfffe)
        return 0;
    id = (cell - (cell->offsetY * g_game->mapWidth + cell->offsetX))->feature;
    if (id >= 0xfffb)
        return 0;
    return (Thing_0043f0e0*)(g_game->units + (id << 8));
}

// FUNCTION: 0x43f0e0
Class_00438760 __stdcall GetOrderType(unsigned char mode, Unit_0043f0e0* unit,
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
            if ((target->f110 & 3) != 2) {
                if (node->f111 & 0x20000)
                    break;
            }
            if (target->def->f170 + target->f70 < g_game->threshold) {
                if (!(node->f111 & 0x10000)) {
                    if (!(unit->f3b & 2))
                        break;
                    if (!(unit->f2c->f111 & 0x10000))
                        break;
                }
            }
            if (target->def->f170 + target->f70 >= g_game->threshold) {
                if (def->f241 & 0x1000) {
                    if (node->f111 & 0x10000)
                        break;
                    if ((unit->f3b & 2) && (unit->f2c->f111 & 0x10000))
                        return Class_00438760();
                }
            }
            unsigned int f = def->f241;
            if (def->f241bits.flag_11) {
                unsigned int air = def->f1ee->f111 & 0x100;
                if (air && !(target->def->f241 & 0x800))
                    return Class_00438760("AIRSTRIKE");
                if (!air && (target->def->f241 & 0x800))
                    return Class_00438760("AIRTOAIR");
                unsigned int tv = target->def->f241 & 0x800;
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
            if (def->f245bits.flag_9) {
                if (def->f241bits.flag_11)
                    return Class_00438760("VTOL_REPAIRPATROL");
                return Class_00438760("REPAIRPATROL");
            }
            if (def->f241bits.flag_11)
                return Class_00438760("VTOL_PATROL");
            return Class_00438760("PATROL");
        }
        break;
    case 8:
        if (!((Unit*)unit)->CanRepair(target))
            break;
        if (target->f104 != 0.0f)
            return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
        return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
    case 7:
        if (!(def->f245 & 0x20) || !friendly)
            break;
        return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
    case 12: {
        if (!(def->f245 & 0x400))
            break;
        // Suspected original bug: the lookup runs before pos is tested, and GetMapCellAtPosition reads
        // [pos] unchecked (0x4815a5), so a null pos crashes here although the tests below allow it.
        Thing_0043f0e0* t = Lookup(pos);
        // RESURRECT check stays nested, with pos tested again for RECLAIM.
        if (pos && (def->f245 & 0x800)) {
            if (Visible(unit, pos) && t && (t->ffe & 0x80))
                return Class_00438760("RESURRECT");
        }
        if (pos && Visible(unit, pos) && t && (t->ffe & 0x80))
            return Pick(def, "VTOL_RECLAIM", "RECLAIM");
        if (!target)
            break;
        return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
    }
    case 13:
        if ((def->f245 & 0x1000) && target && unit->player != target->player)
            return Class_00438760("CAPTURE");
        break;
    case 6:
        if (!target || !((Unit*)unit)->CanLoad(target))
            break;
        return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
    case 5:
        if ((def->f245 & 0x100) && (def->f241 & 0x800) && target && (target->def->f241 & 0x200))
            return Class_00438760("VTOL_LANDING");
        if (def->f245bits.flag_8)
            return Pick(def, "VTOL_UNLOAD", "GROUND_UNLOAD");
        break;
    case 14:
        if (def->f156 == 0 || unit->moving == 0)
            break;
        return Pick(def, "VTOL_MOBILEBUILD", "MOBILEBUILD");
    case 4:
        if (def->f245bits.flag_14)
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
        if (!target)
            return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
        if ((def->f245 & 0x1000) && enemy)
            return Class_00438760("CAPTURE");
        if ((def->f245 & 0x400) && enemy)
            return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
        if (friendly && ((Unit*)unit)->CanRepair(target) && target->f104 != 0.0f)
            return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
        if (friendly && ((Unit*)unit)->CanRepair(target) &&
            (unsigned int)target->f108 < target->def->f1fa)
            return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
        if ((def->f241 & 0x800) && friendly && (target->def->f241 & 0x200))
            return Class_00438760("VTOL_LANDING");
        if (((Unit*)unit)->CanLoad(target))
            return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
        if ((def->f245 & 0x20) && friendly)
            return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
        return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
    case 1: {
        if (g_game->flag37efa == 1) {
            // Both arms write out their recursive calls; a shared goto breaks register use.
            if ((def->f245 & 0x10) && enemy)
                return GetOrderType(3, unit, target, pos);
            if ((def->f245 & 0x400) && enemy)
                return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
            if (friendly && ((Unit*)unit)->CanRepair(target) && target->f104 != 0.0f)
                return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
            if (friendly && ((Unit*)unit)->CanRepair(target))
                return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
            if ((def->f241 & 0x800) && friendly && (target->def->f241 & 0x200))
                return Class_00438760("VTOL_LANDING");
            if (target && ((Unit*)unit)->CanLoad(target))
                return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
            if ((def->f245 & 0x20) && friendly)
                return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
            FEATURE_CHECK(def, unit, pos, 0x800, Class_00438760("RESURRECT"));
            FEATURE_CHECK(def, unit, pos, 0x400, Pick(def, "VTOL_RECLAIM", "RECLAIM"));
            if (!(def->f245 & 0x80) || unit->moving == 0)
                break;
            return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
        } else {
            if ((def->f245 & 0x10) && enemy)
                return GetOrderType(3, unit, target, pos);
            if ((def->f245 & 0x400) && enemy)
                return GetOrderType(0xc, unit, target, pos);
            if (target && ((Unit*)unit)->CanRepair(target) && target->f104 != 0.0f)
                return GetOrderType(8, unit, target, pos);
            if (target && target->unknown_ff[0] == g_game->localPlayer && (target->f110 & 0x20) &&
                target->f104 == 0.0f && target->ffb == 0 &&
                (!target->f86 || (target->f86->f110 & 0x40000000)))
                break;
            FEATURE_CHECK(def, unit, pos, 0x800, Class_00438760("RESURRECT"));
            FEATURE_CHECK(def, unit, pos, 0x400, Pick(def, "VTOL_RECLAIM", "RECLAIM"));
            if (!(def->f245 & 0x80) || unit->moving == 0)
                break;
            return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
        }
    }
    }
none:
    return Class_00438760();
}