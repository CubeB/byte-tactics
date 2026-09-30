// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1. Names are provisional.
// Started by deepseek-v4.1-flash and continued by GPT-6 before this pass. Partial, 23.3%.
// This pass reordered the switch case bodies to the original's source order read off the jump
// table at 0x43f0a8: cases 1, 3, 9, 8, 7, 12, 13, 6, 5, 14, 4, 11, 2, then default. That alone
// took the function from 11.1% to 23.3% (3064 -> 3080 bytes against 3152), because MSVC emits
// case bodies in source order and the numeric order had every block in the wrong place.
// The body shape is right (3064 bytes against 3152) but almost no instruction text lines up,
// so the registers differ throughout. What is known, and what still differs:
//  - Case bodies are emitted in source order 1,3,9,8,7,12,13,6,5,14,4,11,2,10 (jump table at
//    0x43f0a8), not in numeric order; case 10 falls into the shared `return 0x13` at 0x43f098
//    and case 2's body is the last and the largest (0x43e8bb to 0x43f098).
//  - The prologue is `push ecx; push ebx; mov ebx,[g_game]; push ebp; push esi; push edi`, so
//    g_game is cached in ebx, target in edi and there is exactly one 4-byte local. Ours emits
//    `sub esp,8` (two locals) with g_game in ebp and target in ebx.
//  - The friendly flag lives in esi and in the [esp+0x10] local at once (stored on both paths of
//    the allied test, reloaded after the FUN_004815a0 call at 0x43e9dc, line 431 of ctx.txt);
//    the enemy flag is ebp.
//  - unit->def is kept in ecx across the jump table and spilled into the dead target argument
//    slot [esp+0x20], reloaded after every __thiscall (0x43e7ed).
//  - The boolean returns come back as `neg al; sbb eax,eax; and al,imm; add eax,0x13` (case 5 at
//    0x43e80c, case 9 at 0x43e5de), a mask form a plain `?:` does not produce.
//  - Cases 1 and 2 are still approximations of the original block order.
//  - deepseek-v4.1 pass: the original frame is exactly ONE local (push ecx). friendly has its home
//    at [esp+0x10] (stores on both allied-test paths, reload at 0x43e9dc after the inlined Lookup
//    steals esi), enemy lives only in ebp with no home, target in edi, g_game in ebx (loaded in the
//    prologue), and def (ecx) is spilled into the DEAD target parameter slot [esp+0x20] and reloaded
//    after every call (0x43e7ed, 0x43e9f5, 0x43ea7f, 0x43eab8, 0x43ead7, 0x43ec9a, 0x43ee4f,
//    0x43ef68). Ours emits sub esp,8 (two locals): enemy got a memory home at [esp+0x10] and friendly
//    another at [esp+0x14], def at [esp+0x24], and mode sits in ebx while the original reloads it
//    from [esp+0x18] at every dispatch (mov eax,[esp+0x18]; and eax,0xff; dec; cmp; ja; jmp table).
//    Getting the allocator to hand ebx to g_game and ebp to enemy (so the frame collapses to one
//    dword and every [esp+N] offset shifts back by 4) is the key remaining problem; declaration
//    order (game, def, friendly, enemy) did not do it.
//  - Fixed this pass: the tail of the case-2 block returns (f245 & 0x80) ? 0xe : 0x13, not 8
//    (the original is `neg; sbb eax,eax; and al,0xfb; add eax,0x13` at 0x43f07c, pseudo-C mask
//    0xfffffffb).
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
    unsigned int f111; // +0x111
};

struct Def_0043e490 {
    char unknown_0[0x146];
    unsigned char index; // +0x146
    char unknown_147[0x156 - 0x147];
    int f156; // +0x156
    char unknown_15a[0x1ee - 0x15a];
    Node_0043e490* f1ee; // +0x1ee
    char unknown_1f2[0x241 - 0x1f2];
    unsigned int f241; // +0x241
    unsigned int f245; // +0x245
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

static inline int Vtol(Def_0043e490* def) { return (def->f241 >> 11) & 1; }

static inline int Visible(Game_0043e490* game, Unit_0043e490* unit, Pos_0043e490* pos) {
    Player_0043e490* p = unit->player;
    int x = pos->x >> 5;
    int y = (pos->z - (pos->y >> 1)) >> 5;
    return (unsigned int)x < p->width && (unsigned int)y < p->height &&
           ((1 << game->localPlayerBit) & game->visibility[p->width * y + x]) != 0;
}

static inline Thing_0043e490* Lookup(Game_0043e490* game, Pos_0043e490* pos) {
    Cell_0043e490* cell = FUN_004815a0(pos);
    if (!cell)
        return 0;
    unsigned short id = cell->feature;
    if (id >= 0xfffb) {
        if (id != 0xfffe)
            return 0;
        id = (cell - (cell->offsetY * game->mapWidth + cell->offsetX))->feature;
        if (id >= 0xfffb)
            return 0;
    } else if ((int)id >= game->unitCount) {
        return 0;
    }
    return (Thing_0043e490*)(game->units + (id << 8));
}

static inline int Marked(Thing_0043e490* t) { return t && (t->ffe & 0x80); }

// FUNCTION: 0x43e490
int __stdcall FUN_0043e490(unsigned char mode, Unit_0043e490* unit, Unit_0043e490* target,
                           Pos_0043e490* pos) {
    Game_0043e490* game = g_game;
    Def_0043e490* def;
    int friendly = 0;
    int enemy = 0;

restart:
    friendly = 0;
    enemy = 0;
    def = unit->def;
    if (target) {
        if (unit->player->allied[target->player->index] != 0)
            friendly = 1;
        else
            enemy = 1;
    }

    switch (mode) {
    case 1: { // approximate, not matching
        if (game->flag37efa == 1)
            goto L43eb02;
        if ((def->f245 & 0x10) && enemy) {
            mode = 3;
            goto restart;
        }
        if ((def->f245 & 0x400) && enemy) {
            mode = 0xc;
            goto restart;
        }
        goto L43edb6;
    }
    case 3:
        if (def->f245 & 0x10) {
            if (def->f1ee->f111 & 0x100)
                return 2;
        }
        if ((def->f245 >> 4) & 1) {
            Node_0043e490* node = unit->f10;
            if (unit->moving)
                return 1;
            if (!target) {
                if (FUN_0049aa80(unit, (char*)unit + 0x6a, pos, 0) == 0)
                    return 3;
                if (node->f111 & 0x20000)
                    return 3;
                return 1;
            }
            if (FUN_0049abb0(unit, target, 0) == 0)
                return 3;
            return 1;
        }
        goto L43f098;
    case 9:
        return (def->f245 & 0x40) ? 7 : 0x13;
    case 8:
        if (((Class_004899b0*)unit)->FUN_004899b0(target))
            return 6;
        return 0x13;
    case 7:
        if (!(def->f245 & 0x20))
            goto L43f098;
        if (!friendly)
            goto L43f098;
        if (def->f241 & 0x800)
            goto L43eae8;
        if (target->def->f241 & 0x800)
            goto L43f098;
        return 5;
    case 12:
        if (def->f245 & 0x400) {
            if (Visible(game, unit, pos) && Marked(Lookup(game, pos)))
                return 0xb;
        }
        if (!target)
            return 0x13;
        if (((Class_00489960*)unit)->FUN_00489960(target))
            return 0xb;
        return 0x13;
    case 13:
        if (!(def->f245 & 0x1000))
            return 0x13;
        if (!target)
            return 0x13;
        if (unit->player == target->player)
            return 0x13;
        return 4;
    case 6: {
        if (!target)
            return 0x13;
        if (!((Class_00489a70*)unit)->FUN_00489a90(target))
            return 0x13;
        return ((def->f241 >> 11) & 1) ? 8 : 0xc;
    }
    case 5:
        return (def->f245 & 0x100) ? 0xd : 0x13;
    case 14:
        if (def->f156 == 0)
            return 0x13;
        if (unit->moving == 0)
            return 0x13;
        return 0x10;
    case 4: {
        if (!(def->f245 & 0x4000))
            return 0x13;
        float a = *(float*)((char*)unit->fec + 0x8c);
        float b = *(float*)((char*)unit->f48 + 0xc0);
        if (a < b)
            return 3;
        float c = *(float*)((char*)unit->fec + 0x98);
        float d = *(float*)((char*)unit->f48 + 0xc4);
        if (!(d <= c))
            return 3;
        return 1;
    }
    case 11:
        return 9;
    case 2: // approximate, not matching
        if (!(def->f245 & 0x80))
            return 0x13;
        if ((def->f245 & 0x800) && Visible(game, unit, pos) && Marked(Lookup(game, pos)))
            return 0xa;
    L43e9e0:
        if (!target || unit->moving == 0)
            goto L43eaf5;
        if (def->f245 & 0x1000) {
            if (enemy)
                return 4;
        } else if (enemy) {
            if (((Class_00489960*)unit)->FUN_00489960(target))
                return 0xb;
        }
        if (friendly) {
            if (((Class_004899b0*)unit)->FUN_004899b0(target)) {
                if (!(target->f104 == 0.0f))
                    return 6;
            }
            if (((Class_004899b0*)unit)->FUN_004899b0(target))
                return 6;
        }
        if (def->f241 & 0x800) {
            if (target->def->f241 & 0x200)
                return 0xd;
        }
        if (((Class_00489a70*)unit)->FUN_00489a90(target))
            return ((def->f241 >> 11) & 1) ? 8 : 0xc;
        if (def->f245 & 0x20) {
            if (friendly)
                goto L43eae8;
        }
    L43eaf5:
        return 0xe;

    L43eae8:
        return 5;
    default:
        goto L43f098;
    }

L43eb02:
            if (game->flag37efa == 1) {
                if (target && target->owner == game->localPlayer && (target->f110 & 0x20) &&
                    target->f104 == 0.0f && target->ffb == 0) {
                    if (target->f86 == 0)
                        return 0xf;
                    if (target->f86->f110 & 0x40000000)
                        return 0xf;
                }
                if (enemy)
                    return 0x11;
                if (friendly)
                    return 0x12;
                if (def->f245 & 0x800) {
                    if (Visible(game, unit, pos) && Marked(Lookup(game, pos)))
                        return 0x12;
                }
                if (!(def->f245 & 0x400))
                    return 0x13;
                if (!Visible(game, unit, pos))
                    return 0x13;
                if (!Marked(Lookup(game, pos)))
                    return 0x13;
                return 0x12;
            }
L43edb6:
    if (!target)
        goto L43ee4f;
    if (((Class_004899b0*)unit)->FUN_004899b0(target)) {
        if (!(target->f104 == 0.0f))
            return 6;
    }
    if (target->owner == game->localPlayer && (target->f110 & 0x20) && target->f104 == 0.0f &&
        target->ffb == 0) {
        if (target->f86 == 0 || (target->f86->f110 & 0x40000000))
            return 0xf;
    }
L43ee4f:
    if (def->f245 & 0x800) {
        if (Visible(game, unit, pos) && Marked(Lookup(game, pos)))
            return 0xa;
    }
    if (def->f245 & 0x400) {
        if (Visible(game, unit, pos) && Marked(Lookup(game, pos)))
            return 0xb;
    }
    // Original mask is `and al,0xfb` + 0x13 = 0x13-5 = 0xe, not 8.
    return (def->f245 & 0x80) ? 0xe : 0x13;

L43f098:
    return 0x13;
}
