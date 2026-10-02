#include <windows.h>
// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by claude-opus-5-5, finished by Space Bunny Free. Names are provisional.
// PARTIAL (68.2%), up from 63.6% at the start of this pass. 3180 bytes against
// the original's 3152, and 84.0% of the residual is codegen rather than moved
// jump targets. What still differs, in one sentence: the original keeps
// g_game in ebx, def in ecx with a home in target's dead argument slot, and
// never enregisters `unit` at all (17 reloads from [esp+0x1c]), while ours
// enregisters `unit` in ebx and spills g_game to the push-ecx slot, so each of
// the six inlined Visible/Lookup pairs pays one or two extra instructions.
// Levers found this pass, in order of what they were worth:
//  - the friendly/enemy prologue written as if/else on the allied byte,
//      `if (unit->player->allied[target->player->index]) friendly = 1; else
//      enemy = 1;`, instead of `friendly = allied; enemy = friendly ? 0 : 1;`:
//      +2.5 on its own and it moved the register allocation three registers
//      towards the original's (target edi, friendly esi, enemy ebp all start
//      matching). MSVC hoists the shared constant 1 into edx, worth +1 byte,
//      which is cheaper than the `sete`/`mov ebp, eax` the ternary produced.
//  - `game->visibility` instead of `g_game->visibility` in Visible: +0.7, and
//      it makes MSVC treat the local as the CSE'd value at four more sites.
//  - case 4's guard written `if (!(unit->def->f245b.b14))` instead of the
//      `? 0 : 1` form it inherited: +0.2.
//  - case 7 as three separate early returns instead of one NotCaptureable
//      helper: +0.1; the helper's `!(a) || b` emitted xor/jmp/mov/test/jne.
//  - case 1's MultiMode helper inlined to `game->flag37efa == 1`: +0.3.
//  - tools/permute.py, three rounds: 28 minutes from the 63.6% file (63.6 ->
//      63.9 by its own score), one from that result, and a 25-minute round from
//      the 68.1% file (68.1 -> 68.2, a single `unit->player == target->player`
//      swap in case 13). The first round's leftovers survive cleaning; its
//      `do {} while (0)` wrappers and the `tmp` locals in cases 4 and 12 are
//      load-bearing and must stay, while the same treatment in the three
//      helpers is free and was removed.
// Rejected, all measured worse or byte-identical: the `def` local (57.6%, and
// it costs the `sub esp, 8` the original does not have); dropping the `game`
// local so the helpers read the global (compile-shaping attempts at 57-67%);
// a `Player* pl = unit->player` local passed to Visible (34.1%); giving `pos`
// its own local; `Moving(unit)` at cases 14 and 2 (65.6%); every one of the
// twenty commutative-operand swaps tried individually; all 24 orders of the
// four prologue declarations; dropping the `inl1` pointer helper.
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
           0 != ((1 << game->localPlayerBit) & game->visibility[x + y * unit->player->width]);
}

static inline Thing_0043e490* Lookup(Pos_0043e490* pos) {
    Cell_0043e490* cell = FUN_004815a0(pos);
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
    id2 = (cell - (cell->offsetX + (cell->offsetY * g_game->mapWidth)))->feature;
    if (((unsigned short)id2) >= 0xfffb)
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
    return t && t->owner == game->localPlayer && (0x20 & t->f110) != 0 && t->f104 == 0.0f &&
           t->ffb == 0 && (t->f86 == 0 || (t->f86->f110 & 0x40000000));
}

static inline int Moving(Unit_0043e490* unit) { return unit->moving; }

static inline char* inl1(char* base) { return base + 0x6a; }

// FUNCTION: 0x43e490
int __stdcall FUN_0043e490(unsigned char mode, Unit_0043e490* unit, Unit_0043e490* target,
                           Pos_0043e490* pos) {
    Game_0043e490* game;
    game = g_game;
    int friendly;
    int enemy;

restart:
    friendly = 0;
    enemy = 0;
    if (target) {
        if (unit->player->allied[target->player->index])
            friendly = 1;
        else
            enemy = 1;
    }

    switch (mode) {
    case 3:
        if ((0x10 & unit->def->f245) && unit->def->f1ee->f111b.b8)
            return 2;
        if (unit->def->f245b.b4) {
            Node_0043e490* node;
            node = unit->f10;
            if (Moving(unit))
                return 1;
            if (target)
                { 
                if (FUN_0049abb0(unit, target, 0)) { do return 1; while (0); } else { return 3; } }
            if (!(FUN_0049aa80(unit, inl1((char*)unit), pos, 0) != 0) || (node->f111 & 0x20000))
                return 3;
            return 1;
        }
        return 0x13;
    case 9:
        return unit->def->f245b.b6 ? 7 : 0x13;
    case 8:
        return 0 != ((Class_004899b0*)unit)->FUN_004899b0(target) ? 6 : 0x13;
    case 7:
        if (!(unit->def->f245 & 0x20))
            return 0x13;
        if (friendly == 0)
            return 0x13;
        if ((unit->def->f241 & 0x800) || !(target->def->f241 & 0x800))
            return 5;
        return 0x13;
    case 12:
        if (!(0x400 & unit->def->f245)) { goto skip_marked; }
        if (Marked(game, unit, pos))
            return 0xb;
    skip_marked:
        if (0 != ((Unit_0043e490*)target)) {
            unsigned int tmp1;
            tmp1 = ((Class_00489960*)unit)->FUN_00489960(target);
            if (tmp1) return 0xb;
        }
        return 0x13;
    case 13:
        if (!(0x1000 & unit->def->f245) || !target || unit->player == target->player)
            return 0x13;
        return 4;
    case 6:
        if (!target || !((Class_00489a70*)unit)->FUN_00489a90(target))
            return 0x13;
        return unit->def->f241b.b11 ? 8 : 0xc;
    case 5:
        return unit->def->f245b.b8 ? 0xd : 0x13;
    case 14:
        if (0 == unit->def->f156 || 0 == unit->moving) {
            return 0x13;
        }
        return 0x10;
    case 4:
        if (!(unit->def->f245b.b14))
            return 0x13;
        char* tmp0;
        tmp0 = 0x8c + (char*)unit->fec;
        if (*(float*)tmp0 < *(float*)(0xc0 + (char*)unit->f48) ||
            *(float*)((char*)unit->fec + 0x98) < *(float*)((char*)unit->f48 + 0xc4))
            return 3;
        return 1;
    case 11:
        return 9;
    case 2:
        if (!(unit->def->f245 & 0x80))
            return 0x13;
        if (unit->def->f245 & 0x800) {
            if (Marked(game, unit, pos)) return 0xa;
        }
        if (0 == target || 0 == unit->moving)
            return 0xe;
        if (unit->def->f245 & 0x1000) {
            if (enemy) return 4;
        } else if (enemy) {
            do {
                    if (((Class_00489960*)unit)->FUN_00489960(target)) return 0xb;
                } while (0);
        }
        if ((friendly && ((Class_004899b0*)unit)->FUN_004899b0(target)) != 0) {
            if (target->f104 != 0.0f) return 6;
        }
        if (((int)friendly) != 0) {
            if (((Class_004899b0*)unit)->FUN_004899b0(target)) return 6;
        }
        if ((unit->def->f241 & 0x800) && (target->def->f241 & 0x200) != 0) return 0xd;
        if (((Class_00489a70*)unit)->FUN_00489a90(target))
            return unit->def->f241b.b11 ? 8 : 0xc;
        if ((0x20 & unit->def->f245) && friendly) return 5;
        return 0xe;
    case 1:
        if (game->flag37efa == 1)
            goto multi;
        if (!((unit->def->f245 & 0x10) && enemy)) goto skip_case3;
        mode = 3;
        goto restart;
    skip_case3:
        if (!(unit->def->f245 & 0x400) || enemy == 0)
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
    if ((0x800 & unit->def->f245) != 0) {
        if (Marked(game, unit, pos)) return 0x12;
    }
    if ((unit->def->f245 & 0x400) != 0 && Marked(game, unit, pos)) return 0x12;
    return 0x13;

single:
    if (0 != target && 0 != ((Class_004899b0*)unit)->FUN_004899b0(target)) {
            if (target->f104 != 0.0f) return 6;
        }
    if (Capturable(game, target))
        return 0xf;
    if ((unit->def->f245 & 0x800)) {
        if (Marked(game, unit, pos)) {
            return 0xa;
        }
    }
    if (unit->def->f245 & 0x400) {
        do {
            if (Marked(game, unit, pos)) return 0xb;
        } while (0);
    }
    // The named local is needed for the tail's instruction order.
    int tail = unit->def->f245b.b7 ? 0xe : 0x13;
    return tail;
}