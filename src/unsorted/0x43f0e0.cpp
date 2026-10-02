// Decompiled by Claude Sonnet 5.5, finished by DeepSeek V4.1 Flash and GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// claude-sonnet-5-5 pass (60.7%, 4384 vs 4420 bytes, was 53.6%). Not a match. Register-agnostic
// structure is ~87% identical (instruction stream with registers/jump targets masked); the prologue
// and all of case 3 (0x43f154..0x43f27e) now match byte for byte. What changed vs the earlier passes:
// - Pick() is `name = vtol; if (def->f241bits.flag_11 ? 0 : 1) name = ground;`: the ternary stops MSVC
//   turning `!bitfield` into `test ch,8` and gives the original's `shr ecx,0xb; test cl,1`.
// - case 3: the reach test is `if (reach < thr) {node/f2c tests}` then `if (reach >= thr) {...}`, no
//   gotos. MSVC threads the first jump past the second compare, which is exactly the original's
//   `jge 0x43f240` plus the surviving `cmp eax,ecx / jl 0x43f27a` (earlier passes chased this with
//   commuted gotos). The friendly path uses the bitfield `node->f111bits.flag_17` (shr form).
// - Lookup(): `if (id < 0xfffb) {...} else if (id == 0xfffe) {...}` order with `int idx = id` shared by
//   the compare and the shift, matching the original's block order.
// - case 2: `if (!target) return Pick(MOVE)` first (original falls through into it), friendly tests
//   written as two separate `friendly && FUN_004899b0(...)` ifs, RECLAIMUNIT as a positive early return.
// - case 8 tests `f104 != 0.0f` (HELPBUILD first). case 1: both `(f245 & 0x10) && enemy` recursions
//   share one block via goto recurse3 (MSVC only merges them when the code is identical).
// - uses of def after calls (cases 12 and 1) go through the `def` local so they reload from the spill
//   slot [esp+0x20] like the original; this also made def land in esi at the top as in the original.
// Still differs: (1) Lookup's result is kept in edx and t is never spilled, the original returns in eax
// and stores t to [esp+0x24] after `test esi,esi` (g_game temp is ebx there, ours ebp); the mapWidth imul
// is `imul ecx,[mem]` in the original. (2) Visible(): the original re-reads p->width for the imul and
// loads pos->z before pos->y; ours CSEs width. (3) the first two RESURRECT sites are `je skip; jmp shared`
// in the original, `jne shared` here (goto resurrect did not change it). (4) case 1a end: the original
// has its own VTOL_MOVE block with unit in edi (reloaded from [esp+0x1c]); ours shares the 1b tail.
// (5) the unit/def reload after the FUN_004899b0 calls in case 2 (ours reloads unit->def, the original
// reads the spill slot) but writing def-> there scored lower overall.
#include <iostream>

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
    union {
        unsigned int f245; // +0x245
        Flags245_0043f0e0 f245bits;
    };
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

static inline int IsVtol(Def_0043f0e0* def) { return def->f241bits.flag_11; }

static inline Class_00438760 Pick(Def_0043f0e0* def, const char* vtol, const char* ground) {
    const char* name = vtol;
    if (def->f241bits.flag_11 ? 0 : 1) name = ground;
    return Class_00438760(name);
}

static inline int Visible(Unit_0043f0e0* unit, Pos_0043f0e0* pos) {
    Player_0043f0e0* p = unit->player;
    int x = pos->x >> 5;
    int y = (pos->z - (pos->y >> 1)) >> 5;
    return (unsigned int)x < p->width && (unsigned int)y < p->height &&
           ((1 << g_game->localPlayerBit) & g_game->visibility[p->width * y + x]) != 0;
}

static inline Thing_0043f0e0* Lookup(Pos_0043f0e0* pos) {
    Cell_0043f0e0* cell;
    int idx;
    cell = FUN_004815a0(pos);
    if (!cell)
        return 0;
    unsigned short id;
    id = cell->feature;
    id = id;
    if (0xfffb > id) {
        idx = id;
        if (idx >= g_game->unitCount)
            return 0;
        return (Thing_0043f0e0*)((idx << 8) + g_game->units);
    }
    if (id != 0xfffe)
        return 0;
    unsigned short tmp0 = (cell - (cell->offsetY * g_game->mapWidth + (int)cell->offsetX))->feature;
    id = tmp0;
    if (id >= 0xfffb)
        return 0;
    return (Thing_0043f0e0*)((id << 8) + g_game->units);
}

static inline int Marked(Thing_0043f0e0* t) { return t && (0x80 & t->ffe); }

static inline bool inl1(Unit_0043f0e0*unit, Unit_0043f0e0*target) { return target->player != unit->player; }

// FUNCTION: 0x43f0e0
Class_00438760 __stdcall FUN_0043f0e0(unsigned char mode, Unit_0043f0e0* unit,
                                      Unit_0043f0e0* target, Pos_0043f0e0* pos) {
    unsigned int tmp3;
    unsigned int air;
    int friendly;
    friendly = 0;
    Node_0043f0e0* node;
    Def_0043f0e0* def;
    int enemy;
    enemy = 0;
    unsigned int f;
    if (target) {
        if (!(target->f110 & 0x10000000))
            goto none;
        if (((int)(unit->player->allied[target->player->index] != 0)))
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
            node = unit->f10;
            if (!enemy) {
                if (node->f111bits.flag_17)
                    break;
                if (!(def->f241 & 0x800))
                    return Class_00438760("SUPPRESS");
                if (def->f1ee->f111bits.flag_8)
                    return Class_00438760("AIRSTRIKE");
                return Class_00438760("AIRTOGROUND");
            }
            {
            if (2 != (target->f110 & 3)) {
                if (0x20000 & node->f111)
                    break;
            }
            if (g_game->threshold > target->def->f170 + target->f70) {
                if (!(node->f111 & 0x10000)) {
                    if (!(unit->f3b & 2))
                        break;
                    if ((0x10000 & unit->f2c->f111) == 0)
                        break;
                }
            }
            if (target->def->f170 + target->f70 >= g_game->threshold) {
                if (def->f241 & 0x1000) {
                    if (0x10000 & node->f111)
                        break;
                    int tmp1;
                    tmp1 = 2 & unit->f3b;
                    if (0 != tmp1 && (unit->f2c->f111 & 0x10000))
                        return Class_00438760();
                }
            }
            f = def->f241;
            f = f;
            if (def->f241bits.flag_11) {
                air = def->f1ee->f111 & 0x100;
                air = air;
                if (air && !(target->def->f241 & 0x800))
                    return Class_00438760("AIRSTRIKE");
                if (!air && (target->def->f241 & 0x800))
                    return Class_00438760("AIRTOAIR");
                unsigned int tv = target->def->f241 & 0x800;
                if (!tv) {
                    if (!(0x8000000 & f)) return Class_00438760("AIRTOGROUND");
                }
                if (!tv && (((unsigned int)f) & 0x8000000))
                    return Class_00438760("AIRTOGROUNDHOVER");
                break;
            }
            bool tmp6;
            tmp6 = unit->moving != 0;
            if (tmp6)
                return Class_00438760("ATTACK_CHASE");
            if (0x20000000 & flags.raw)
                return Class_00438760("ATTACK_NOMOVE");
            }
        }
        if (def->f241bits.flag_28)
            return Class_00438760("ATTACK_KAMIKAZE");
        break;
    }
    case 9:
        if (unit->def->f245 & 0x40) {
            if (0 == unit->moving)
                return Class_00438760("QPATROL");
            if (unit->def->f245bits.flag_9) {
                if (unit->def->f241bits.flag_11)
                    return Class_00438760("VTOL_REPAIRPATROL");
                return Class_00438760("REPAIRPATROL");
            }
            if (unit->def->f241bits.flag_11)
                return Class_00438760("VTOL_PATROL");
            return Class_00438760("PATROL");
        }
        break;
    case 8:
        if (!((Class_004899b0*)unit)->FUN_004899b0(target))
            break;
        if (target->f104 != 0.0f)
            return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
        return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
    case 7:
        if (!(unit->def->f245 & 0x20) || !friendly)
            break;
        return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
    case 12: {
        Thing_0043f0e0* t;
        if (!(def->f245 & 0x400))
            break;
        t = Lookup(pos);
        if (pos) {
            if ((0x800 & def->f245) && Visible(unit, pos) && Marked(t))
                return Class_00438760("RESURRECT");
            if (pos && Visible(unit, pos)) {
                unsigned int tmp4 = Marked(t);
                if (tmp4) return Pick((Def_0043f0e0*)def, "VTOL_RECLAIM", "RECLAIM");
            }
        }
        if (!target)
            break;
        return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
    }
    case 13:
        if ((unit->def->f245 & 0x1000) && ((Unit_0043f0e0*)target)) {
            if (inl1(unit, target)) return Class_00438760("CAPTURE");
        }
        break;
    case 6:
        if (!target || !((Class_00489a70*)unit)->FUN_00489a90(target)) break;
        return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
    case 5:
        if ((unit->def->f245 & 0x100) && (unit->def->f241 & 0x800) && target && (target->def->f241 & 0x200))
            return Class_00438760("VTOL_LANDING");
        if (unit->def->f245bits.flag_8)
            return Pick(def, "VTOL_UNLOAD", "GROUND_UNLOAD");
        break;
    case 14:
        if (unit->def->f156 == 0 || unit->moving == 0)
            break;
        return Pick(def, "VTOL_MOBILEBUILD", "MOBILEBUILD");
    case 4:
        if (unit->def->f245bits.flag_14)
            return Class_00438760("ATTACKSPECIAL");
        break;
    case 11:
        return Class_00438760("TELEPORT");
    case 10:
        return Class_00438760("STOP");
    case 2:
        if (!(unit->def->f245 & 0x80))
            break;
        if (unit->moving == 0)
            return Class_00438760("QMOVE");
        if (!target)
            return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
        {
            if ((unit->def->f245 & 0x1000) && enemy)
                return Class_00438760("CAPTURE");
            if ((0x400 & unit->def->f245) && enemy) return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
            if (friendly && ((Class_004899b0*)unit)->FUN_004899b0(target) && target->f104 != 0.0f)
                return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
            if (friendly && ((Class_004899b0*)unit)->FUN_004899b0(target) &&
                (unsigned int)target->f108 < target->def->f1fa)
                return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
            if ((0x800 & unit->def->f241) && friendly) {
                if ((target->def->f241 & 0x200)) return Class_00438760("VTOL_LANDING");
            }
            tmp3 = ((Class_00489a70*)unit)->FUN_00489a90(target);
            if (tmp3)
                return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
            if ((0x20 & unit->def->f245)) {
                if (friendly) return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
            }
            return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
        }
    case 1: {
        if (g_game->flag37efa == 1) {
            if ((def->f245 & 0x10)) {
                if (enemy) goto recurse3;
            }
            if ((def->f245 & 0x400) && enemy)
                return Pick(def, "VTOL_RECLAIMUNIT", "RECLAIMUNIT");
            if (friendly && ((Class_004899b0*)unit)->FUN_004899b0(target) && target->f104 != 0.0f)
                return Pick(def, "VTOL_HELPBUILD", "HELPBUILD");
            if (friendly && ((Class_004899b0*)unit)->FUN_004899b0(target) != 0)
                return Pick(def, "VTOL_REPAIRUNIT", "REPAIRUNIT");
            if ((def->f241 & 0x800) && friendly && (target->def->f241 & 0x200))
                return Class_00438760("VTOL_LANDING");
            if (target) {
                if (((Class_00489a70*)unit)->FUN_00489a90(target)) return Pick(def, "VTOL_PICKUP", "GROUND_PICKUP");
            }
            if ((0x20 & def->f245) && friendly)
                return Pick(def, "VTOL_FOLLOW", "FOLLOW_GROUND");
            if ((def->f245 & 0x800) && pos && Visible(unit, pos) && Marked(Lookup(pos)))
                return Class_00438760("RESURRECT");
            if ((def->f245 & 0x400) && pos && Visible(unit, pos) && Marked(Lookup(pos)))
                return Pick(def, "VTOL_RECLAIM", "RECLAIM");
            int tmp9 = !(0x80 & def->f245);
            if (tmp9 != 0 || unit->moving == 0)
                break;
        } else {
            if ((0x10 & def->f245) && enemy)
            recurse3:
                return FUN_0043f0e0(3, unit, target, pos);
            if ((def->f245 & 0x400)) {
                if (enemy) return FUN_0043f0e0(0xc, unit, target, pos);
            }
            if (target && ((Class_004899b0*)unit)->FUN_004899b0(target)) {
                if (target->f104 != 0.0f) return FUN_0043f0e0(8, unit, target, pos);
            }
            if (target && target->unknown_ff[0] == g_game->localPlayer && (0x20 & target->f110) &&
                target->f104 == 0.0f && target->ffb == 0) {
                bool tmp8 = !target->f86;
                if ((tmp8 || (target->f86->f110 & 0x40000000) != 0)) break;
            }
            if ((def->f245 & 0x800) != 0 && pos && Visible(unit, (Pos_0043f0e0*)pos) && Marked(Lookup(pos)))
                return Class_00438760("RESURRECT");
            if ((0x400 & def->f245) && pos && Visible(unit, pos)) {
                if (Marked(Lookup(pos))) return Pick(def, "VTOL_RECLAIM", "RECLAIM");
            }
            if (!(0x80 & def->f245) || 0 == unit->moving)
                break;
        }
        return Pick(def, "VTOL_MOVE", "MOVE_GROUND");
    }
    }
none:
    return Class_00438760();
}