#include <windows.h>
// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by claude-opus-5-5. Names are provisional.
// PARTIAL (60.1%). Rewritten by claude-opus-5-5 (#4290) from 28.2%.
// PARTIAL (60.1%), up from 48.1% at the start of this pass. What still
// differs: register allocation, plus 56 bytes of size. The original's frame is
// one dword (`push ecx` only, no `sub esp`), keeps g_game in ebx from the
// prologue and reloads ebx from the global after the calls inside the inlined
// lookup, puts target in edi, friendly in esi with a home in the push-ecx slot,
// enemy in ebp, def in ecx with a home in target's dead argument slot, and never
// enregisters `unit` (17 reloads from [esp+0x1c]). Ours spreads the same five
// values over a different set and reloads g_game from a frame slot.
// Levers found this pass, in order of what they were worth:
//  - `#include <windows.h>` alone: +3.5 (55.7 -> 59.2). tools/headers.py, 768
//    sets, nothing better; run it BEFORE concluding a source sweep is spent.
//  - the friendly/enemy prologue shape: `friendly = allied ? 1 : 0;
//    enemy = friendly ? 0 : 1;` instead of the if/else chain, +3.
//  - dropping the local `def` and writing `unit->def->` inline: +4 (48.1 -> 52.1
//    on its own). Keeping it costs a second frame slot and rotates everything
//    (34.4% now), even though the original plainly has def in ecx with a home.
//  - two extracted one-line helpers: `Moving(unit)` for case 3's
//    `if (unit->moving)` and `NotCaptureable(unit, friendly)` for case 7's
//    first test, +0.2 together. The same helpers at their other call sites
//    (cases 14, 2, 6, 12, 13) each cost 0.7 to 10 points, so only these two.
//  - tools/permute.py (two 20-minute rounds, resumed): 59.3 -> 60.8, of which
//    +0.9 survives cleaning. Its leftovers worth knowing: re-reading
//    `cell->feature` in Lookup's compare, the `goto skip_...` shape for case 12
//    and the case 1 polarity flip are all free, while undoing its case 13
//    operand swap or its tail local costs 0.2 and 6.6.
// Rejected, all byte-identical or worse: permuting the four declarations, the
// helper parameter orders and the case labels; caching unit->player or
// unit->moving in locals; dropping the local `game` so the helpers read g_game
// themselves (43.7%).
#pragma pack(push, 1)

union Flags110_0043e490 {
    unsigned int raw;
    struct {
        unsigned int bits_0 : 31;
        unsigned int flag_31 : 1;
    };
};

struct Game_0043e490 {
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
    char unknown_14277[0x37efa - 0x14277];
    int flag37efa; // +0x37efa
};

struct Node_0043e490 {
    char unknown_0[0x111];
    union { unsigned int f111; struct { unsigned int b0 : 1; unsigned int b1 : 1; unsigned int b2 : 1; unsigned int b3 : 1; unsigned int b4 : 1; unsigned int b5 : 1; unsigned int b6 : 1; unsigned int b7 : 1; unsigned int b8 : 1; unsigned int b9 : 1; unsigned int b10 : 1; unsigned int b11 : 1; unsigned int b12 : 1; unsigned int b13 : 1; unsigned int b14 : 1; unsigned int b15 : 1; unsigned int b16 : 1; unsigned int b17 : 1; unsigned int b18 : 1; unsigned int b19 : 1; unsigned int b20 : 1; unsigned int b21 : 1; unsigned int b22 : 1; unsigned int b23 : 1; unsigned int b24 : 1; unsigned int b25 : 1; unsigned int b26 : 1; unsigned int b27 : 1; unsigned int b28 : 1; unsigned int b29 : 1; unsigned int b30 : 1; unsigned int b31 : 1;} f111b; }; // +0x111
};

struct Def_0043e490 {
    char unknown_0[0x146];
    unsigned char index; // +0x146
    char unknown_147[0x156 - 0x147];
    int f156; // +0x156
    char unknown_15a[0x1ee - 0x15a];
    Node_0043e490* f1ee; // +0x1ee
    char unknown_1f2[0x241 - 0x1f2];
    union { unsigned int f241; struct { unsigned int b0 : 1; unsigned int b1 : 1; unsigned int b2 : 1; unsigned int b3 : 1; unsigned int b4 : 1; unsigned int b5 : 1; unsigned int b6 : 1; unsigned int b7 : 1; unsigned int b8 : 1; unsigned int b9 : 1; unsigned int b10 : 1; unsigned int b11 : 1; unsigned int b12 : 1; unsigned int b13 : 1; unsigned int b14 : 1; unsigned int b15 : 1; unsigned int b16 : 1; unsigned int b17 : 1; unsigned int b18 : 1; unsigned int b19 : 1; unsigned int b20 : 1; unsigned int b21 : 1; unsigned int b22 : 1; unsigned int b23 : 1; unsigned int b24 : 1; unsigned int b25 : 1; unsigned int b26 : 1; unsigned int b27 : 1; unsigned int b28 : 1; unsigned int b29 : 1; unsigned int b30 : 1; unsigned int b31 : 1;} f241b; }; // +0x241
    union { unsigned int f245; struct { unsigned int b0 : 1; unsigned int b1 : 1; unsigned int b2 : 1; unsigned int b3 : 1; unsigned int b4 : 1; unsigned int b5 : 1; unsigned int b6 : 1; unsigned int b7 : 1; unsigned int b8 : 1; unsigned int b9 : 1; unsigned int b10 : 1; unsigned int b11 : 1; unsigned int b12 : 1; unsigned int b13 : 1; unsigned int b14 : 1; unsigned int b15 : 1; unsigned int b16 : 1; unsigned int b17 : 1; unsigned int b18 : 1; unsigned int b19 : 1; unsigned int b20 : 1; unsigned int b21 : 1; unsigned int b22 : 1; unsigned int b23 : 1; unsigned int b24 : 1; unsigned int b25 : 1; unsigned int b26 : 1; unsigned int b27 : 1; unsigned int b28 : 1; unsigned int b29 : 1; unsigned int b30 : 1; unsigned int b31 : 1;} f245b; }; // +0x245
};

struct Player_0043e490 {
    char unknown_0[0x80];
    unsigned int width;  // +0x80
    unsigned int height; // +0x84
    char unknown_88[0x108 - 0x88];
    char allied[1]; // +0x108
    char unknown_109[0x146 - 0x109];
    unsigned char index; // +0x146
};

struct Unit_0043e490 {
    int moving; // +0x0
    char unknown_4[0x10 - 0x4];
    Node_0043e490* f10; // +0x10
    char unknown_14[0x48 - 0x14];
    void* f48; // +0x48
    char unknown_4c[0x86 - 0x4c];
    Unit_0043e490* f86; // +0x86
    char unknown_8a[0x92 - 0x8a];
    Def_0043e490* def;       // +0x92
    Player_0043e490* player; // +0x96
    char unknown_9a[0xec - 0x9a];
    void* fec; // +0xec
    char unknown_f0[0xfb - 0xf0];
    int ffb;             // +0xfb
    unsigned char owner; // +0xff
    char unknown_100[0x104 - 0x100];
    float f104; // +0x104
    short f108; // +0x108
    char unknown_10a[0x110 - 0x10a];
    union {
        unsigned int f110; // +0x110
        Flags110_0043e490 f110bits;
    };
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
    char unknown_c;
};

struct Thing_0043e490 {
    char unknown_0[0xfe];
    unsigned char ffe; // +0xfe
    char unknown_ff;
};
#pragma pack(pop)

extern Game_0043e490* g_game;
extern const float DAT_004fd2e8; // 0.0f

Cell_0043e490* __stdcall FUN_004815a0(Pos_0043e490* pos);
int __stdcall FUN_0049aa80(Unit_0043e490* unit, void* slot, Pos_0043e490* pos, int which);
int __stdcall FUN_0049abb0(Unit_0043e490* unit, Unit_0043e490* target, int which);
class Class_00489960 {
  public:
    int FUN_00489960(Unit_0043e490* other);
};
class Class_004899b0 {
  public:
    int FUN_004899b0(Unit_0043e490* other);
};
class Class_00489a70 {
  public:
    int FUN_00489a90(Unit_0043e490* other);
};

static inline int Visible(Game_0043e490* game, Unit_0043e490* unit, Pos_0043e490* pos) {
    int y, x;
    x = pos->x >> 5;
    Player_0043e490* p = unit->player;
    y = (pos->z - (pos->y >> 1)) >> 5;
    return (unsigned int)x < p->width && (unsigned int)y < p->height &&
           ((1 << game->localPlayerBit) & g_game->visibility[x + unit->player->width * y]) != 0;
}

static inline Thing_0043e490* Lookup(Pos_0043e490* pos) {
    Cell_0043e490* cell = FUN_004815a0(((Pos_0043e490*)pos));
    if (!cell)
        return (Thing_0043e490*)cell;
    unsigned short id;
    id = cell->feature;
    if (0xfffb > cell->feature) {
        if ((int)id >= g_game->unitCount)
            return 0;
        return id + (Thing_0043e490*)g_game->units;
    }
    if (id != 0xfffe)
        return 0;
    unsigned short id2;
    id2 = (cell - (cell->offsetX + g_game->mapWidth * cell->offsetY))->feature;
    if (0xfffb <= id2)
        return 0;
    return (Thing_0043e490*)g_game->units + ((unsigned short)id2);
}

static inline int Marked(Game_0043e490* game, Unit_0043e490* unit, Pos_0043e490* pos) {
    if (Visible(game, unit, pos)) {
        Thing_0043e490* t;
        t = Lookup(pos);
        if (t && (0x80 & t->ffe))
            return 1;
    }
    return 0;
}

static inline int Capturable(Game_0043e490* game, Unit_0043e490* t) {
    return t && game->localPlayer == t->owner && (t->f110 & 0x20) && t->f104 == 0.0f &&
           t->ffb == 0 && (t->f86 == 0 || (t->f86->f110 & 0x40000000));
}

static inline bool MultiMode(Game_0043e490* game) { return game->flag37efa == 1; }

static inline int Moving(Unit_0043e490* unit) { return unit->moving; }

static inline bool NotCaptureable(Unit_0043e490* unit, int friendly) {
    return !(unit->def->f245 & 0x20) || friendly == 0;
}

// FUNCTION: 0x43e490
int __stdcall FUN_0043e490(unsigned char mode, Unit_0043e490* unit, Unit_0043e490* target,
                           Pos_0043e490* pos) {
    Game_0043e490* game;
    int enemy;
    int friendly;
    game = g_game;

restart:
    friendly = 0;
    enemy = 0;
    if (target) {
        friendly = (unsigned char)unit->player->allied[target->player->index];
        enemy = friendly ? 0 : 1;
    }

    switch (mode) {
    case 3:
        if ((unit->def->f245 & 0x10) && unit->def->f1ee->f111b.b8)
            return 2;
        if (unit->def->f245b.b4) {
            Node_0043e490* node;
            node = unit->f10;
            if (Moving(unit))
                return 1;
            if (target)
                { if (!FUN_0049abb0(unit, target, 0)) { return 3; } else { return 1; } }
            if (!FUN_0049aa80(unit, (char*)unit + 0x6a, pos, 0) || (node->f111 & 0x20000))
                return 3;
            return 1;
        }
        return 0x13;
    case 9:
        return unit->def->f245b.b6 ? 7 : 0x13;
    case 8:
        return ((Class_004899b0*)unit)->FUN_004899b0(target) ? 6 : 0x13;
    case 7:
        if (NotCaptureable(unit, friendly))
            return 0x13;
        if ((0x800 & unit->def->f241) || !(target->def->f241 & 0x800))
            return 5;
        return 0x13;
    case 12:
        if (!(unit->def->f245 & 0x400))
            goto skip_marked;
        if (Marked(game, unit, pos))
            return 0xb;
    skip_marked:
        if (target) {
            if (((Class_00489960*)unit)->FUN_00489960(((Unit_0043e490*)target))) return 0xb;
        }
        return 0x13;
    case 13:
        if (!(0x1000 & unit->def->f245) || !target || target->player == unit->player)
            return 0x13;
        return 4;
    case 6:
        if (!target || !((Class_00489a70*)unit)->FUN_00489a90(target))
            return 0x13;
        return unit->def->f241b.b11 ? 8 : 0xc;
    case 5:
        return unit->def->f245b.b8 ? 0xd : 0x13;
    case 14:
        if (unit->def->f156 == 0 || unit->moving == 0)
            return 0x13;
        return 0x10;
    case 4:
        if (unit->def->f245b.b14 ? 0 : 1)
            return 0x13;
        if (*(float*)((char*)unit->fec + 0x8c) < *(float*)((char*)unit->f48 + 0xc0) ||
            *(float*)((char*)unit->fec + 0x98) < *(float*)((char*)unit->f48 + 0xc4))
            return 3;
        return 1;
    case 11:
        return 9;
    case 2:
        if (!(unit->def->f245 & 0x80))
            return 0x13;
        if (unit->def->f245 & 0x800) {
            if (Marked(game, unit, pos))
                return 0xa;
        }
        if (!target || !unit->moving)
            return 0xe;
        if (unit->def->f245 & 0x1000) {
            if (enemy)
                return 4;
        } else if (enemy && ((Class_00489960*)unit)->FUN_00489960(target)) {
            return 0xb;
        }
        if (friendly && ((Class_004899b0*)unit)->FUN_004899b0(target) && target->f104 != 0.0f)
            return 6;
        if (((int)friendly) && ((Class_004899b0*)unit)->FUN_004899b0(target))
            return 6;
        if ((unit->def->f241 & 0x800) && (target->def->f241 & 0x200))
            return 0xd;
        if (((Class_00489a70*)unit)->FUN_00489a90(target))
            return unit->def->f241b.b11 ? 8 : 0xc;
        if ((unit->def->f245 & 0x20) && friendly)
            return 5;
        return 0xe;
    case 1:
        if (MultiMode(game))
            goto multi;
        if ((unit->def->f245 & 0x10) && enemy) {
        } else
            goto skip_case3;
        mode = 3;
        goto restart;
    skip_case3:
        if (!(unit->def->f245 & 0x400) || !enemy)
            goto single;
        mode = 0xc;
        goto restart;
    default:
        return 0x13;
    }

multi:
    if (Capturable(game, target))
        return 0xf;
    if (enemy)
        return 0x11;
    if (friendly != 0)
        return 0x12;
    if (unit->def->f245 & 0x800) {
        if (Marked(game, unit, pos))
            return 0x12;
    }
    if ((0x400 & unit->def->f245) && Marked(game, unit, pos))
        return 0x12;
    return 0x13;

single:
    if (target && ((Class_004899b0*)unit)->FUN_004899b0(target) && target->f104 != 0.0f)
            return 6;
    if (Capturable(game, target))
        return 0xf;
    if ((unit->def->f245 & 0x800)) {
        if (Marked(game, unit, pos)) return 0xa;
    }
    if ((unit->def->f245 & 0x400) && Marked(game, unit, pos))
        return 0xb;
    // The named local is needed for the tail's instruction order.
    int tail = unit->def->f245b.b7 ? 0xe : 0x13;
    return tail;
}