// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by claude-opus-5-5, finished by Space Bunny Free, finished by claude-opus-5-5. Names are provisional.
//
// MATCH (3152 bytes). Returns the cursor/action code for an order of type
// `mode` given by `unit` on `target` / `pos`; FUN_0043f0e0 is the sibling that
// returns the action's name. What the earlier 68.2% version was missing:
//  - case 1 recurses with `return GetOrderCursor(3/0xc, unit, target, pos)`. MSVC
//    turns the self tail calls into `mov byte [esp+0x18], 3; jmp` back to the
//    top, which is the original's loop; a hand-written goto loop allocates
//    differently.
//  - `def = unit->def` is a real local (its home is target's dead argument
//    slot), g_game is read directly everywhere (no `game` local), and the
//    declarations are friendly, enemy, then `def = unit->def`.
//  - Visible re-reads the width through unit->player (see the helper).
//  - GetFeature is the plain multi-return form, and it only puts its result in
//    eax (with the shape-B `xor eax, eax` blocks at the first of two
//    consecutive checks) under the compiler state <windows.h> gives. Without
//    the header the result lands in ecx at four of the six sites.
//  - the six visible-feature checks are one RECLAIM_CHECK statement macro;
//    its do/while(0) is load-bearing (see the macro).
//  - case 4 declares the f48 pointer before the fec one (later-declared
//    loads first).
// Every Reclaim-style helper that wrapped Visible and GetFeature in one inline
// function exceeded MSVC 5's per-function inline budget and left the first
// GetFeature as a call; writing each check out (here through the macro) keeps
// all six inline.
#include <windows.h>
#pragma pack(push, 1)

struct Game {
    char unknown_0[0x2a42];
    unsigned char localPlayer;    // +0x2a42
    unsigned char localPlayerBit; // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int mapWidth; // +0x14233
    char unknown_14237[0x14253 - 0x14237];
    int featureCount; // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    struct Feature_0043e490* features; // +0x1426f
    unsigned short* visibility;        // +0x14273
    char unknown_14277[0x37efa - 0x14277];
    int multiplayer; // +0x37efa
};

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
#pragma pack(pop)

extern Game* g_game;

Cell_0043e490* __stdcall FUN_004815a0(Pos_0043e490* pos);
int __stdcall WeaponCanReachPos(Unit_0043e490* unit, void* slot, Pos_0043e490* pos, int which);
int __stdcall WeaponCanReachUnit(Unit_0043e490* unit, Unit_0043e490* target, int which);
class Class_00489960 {
  public:
    int CanReclaim(Unit_0043e490* other);
};
class Class_004899b0 {
  public:
    int CanRepair(Unit_0043e490* other);
};
class Unit {
  public:
    int CanLoad(Unit_0043e490* other);
};
int __stdcall GetOrderCursor(unsigned char mode, Unit_0043e490* unit, Unit_0043e490* target,
                           Pos_0043e490* pos);

// The same test as FUN_00408090: is the map square under pos in sight of the
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
            Feature_0043e490* f = GetFeature(FUN_004815a0(pos));     \
            if (f && (f->flags & 0x80))                              \
                return (result);                                     \
        }                                                            \
    } while (0)

static inline int Selectable(Unit_0043e490* t) {
    return t && t->owner == g_game->localPlayer && (t->f110 & 0x20) && t->f104 == 0.0f && t->ffb == 0 &&
           (t->f86 == 0 || (t->f86->f110 & 0x40000000));
}

// FUNCTION: 0x43e490
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
        return ((Class_004899b0*)unit)->CanRepair(target) ? 6 : 0x13;
    case 7:
        if (!(def->f245 & 0x20) || !friendly)
            break;
        if ((def->f241 & 0x800) || !(target->def->f241 & 0x800))
            return 5;
        break;
    case 12:
        RECLAIM_CHECK(def, unit, pos, 0x400, 0xb);
        if (target && ((Class_00489960*)unit)->CanReclaim(target))
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
        if (enemy && ((Class_00489960*)unit)->CanReclaim(target))
            return 0xb;
        if (friendly && ((Class_004899b0*)unit)->CanRepair(target) && target->f104 != 0.0f)
            return 6;
        if (friendly && ((Class_004899b0*)unit)->CanRepair(target))
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
        if (target && ((Class_004899b0*)unit)->CanRepair(target) && target->f104 != 0.0f)
            return 6;
        if (Selectable(target))
            return 0xf;
        RECLAIM_CHECK(def, unit, pos, 0x800, 0xa);
        RECLAIM_CHECK(def, unit, pos, 0x400, 0xb);
        return def->f245b.b7 ? 0xe : 0x13;
    }
    return 0x13;
}
