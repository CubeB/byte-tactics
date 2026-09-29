// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL. This is a 3152-byte, 14-case switch returning order codes for a
// cursor/order picker, a sibling of 0x43f0e0 (see that file for the verified
// struct layout and the inlined cell/visibility/unit-lookup helpers, reused
// here). The prologue, the friendly/enemy classification and the short cases
// (4,5,6,8,9,11,13,14) are transcribed from the disassembly; cases 1, 2 and 12
// still differ in structure and are only approximated. Expected to compile
// but not to match.
//
// Caller (0x48d3e2) loops over candidate units, calls this with
// (g_game+0x2cc3, *it, target, g_game+0x2caa) and keeps the minimum result,
// so smaller return values are "better" order candidates and 0x13 is "none".
//
// RETRY NOTES (deepseek-v4.1-flash, still 12.8%): the only instructions that
// align are the shared "pop edi/esi/ebp/ebx/ecx; ret 0x10" epilogues, so the
// whole score comes from the ~14 return sequences; every body line differs
// because the register assignment is completely different. Original keeps
// ebx = g_game, ecx = def, edi = target, esi = friendly, ebp = enemy; unit,
// mode and pos stay in their argument homes and are reloaded from
// [esp+0x1c]/[esp+0x18]/[esp+0x24] every use. It also reserves one dummy
// local with the leading `push ecx`, at [esp+0x10], and spills friendly there
// (0x43e4a7 store 0, 0x43e4e0 store 1, reloaded at 0x43e9dc just before the
// shared L43e9e0 block) because the inlined Visible helper reuses esi for pos
// (mov esi,[esp+0x24] at 0x43e66b / 0x43e8d2 / 0x43eb8b). It likewise spills
// def into the target argument home [esp+0x20] (0x43e4b3) and re-reads it
// from there (0x43e9f5, 0x43e7ed, 0x43ea7f, 0x43ead7, 0x43ea83, 0x43ec9a,
// 0x43ee4f ...). Reproducing that means the source must clobber esi inside the
// visible block and take def's address-free live range across the switch.
// Ghidra's three booleans collapse to just friendly (esi) / enemy (ebp):
// allied[target->player->index] != 0 -> friendly, else enemy (its bVar9 is
// our enemy). Case map from the 0x43f0a8 jump table: 1=0x43e505,
// 2=0x43e8bb, 3=0x43e545, 4=0x43e850, 5=0x43e80c, 6=0x43e7d3, 7=0x43e615,
// 8=0x43e5fa, 9=0x43e5de, 10=0x43f098 (default), 11=0x43e8ae,
// 12=0x43e65c, 13=0x43e797, 14=0x43e828. Cases 1/2/12 here are still only
// semantically close, not structurally equal.
// The last case-1-with-flag block (0x43eb02) and the shared L43edb6 block
// (0x43edb6) both duplicate the same owner/f110-0x20/f104/ffb/f86->f110 test;
// they are two separate emitted copies in the original.
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
    unsigned char localPlayer;         // +0x2a42
    unsigned char localPlayerBit;      // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int mapWidth;                      // +0x14233
    char unknown_14237[0x14253 - 0x14237];
    int unitCount;                     // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    char* units;                       // +0x1426f
    unsigned short* visibility;        // +0x14273
    char unknown_14277[0x37efa - 0x14277];
    int flag37efa;                     // +0x37efa
};

struct Node_0043e490 {
    char unknown_0[0x111];
    unsigned int f111;                 // +0x111
};

struct Def_0043e490 {
    char unknown_0[0x146];
    unsigned char index;               // +0x146
    char unknown_147[0x156 - 0x147];
    int f156;                          // +0x156
    char unknown_15a[0x1ee - 0x15a];
    Node_0043e490* f1ee;               // +0x1ee
    char unknown_1f2[0x241 - 0x1f2];
    unsigned int f241;                 // +0x241
    unsigned int f245;                 // +0x245
};

struct Player_0043e490 {
    char unknown_0[0x80];
    unsigned int width;                // +0x80
    unsigned int height;               // +0x84
    char unknown_88[0x108 - 0x88];
    char allied[1];                    // +0x108
    char unknown_109[0x146 - 0x109];
    unsigned char index;               // +0x146
};

struct Unit_0043e490 {
    int moving;                        // +0x0
    char unknown_4[0x10 - 0x4];
    Node_0043e490* f10;                // +0x10
    char unknown_14[0x48 - 0x14];
    void* f48;                         // +0x48
    char unknown_4c[0x86 - 0x4c];
    Unit_0043e490* f86;                // +0x86
    char unknown_8a[0x92 - 0x8a];
    Def_0043e490* def;                 // +0x92
    Player_0043e490* player;           // +0x96
    char unknown_9a[0xec - 0x9a];
    void* fec;                         // +0xec
    char unknown_f0[0xfb - 0xf0];
    int ffb;                           // +0xfb
    unsigned char owner;               // +0xff
    char unknown_100[0x104 - 0x100];
    float f104;                        // +0x104
    short f108;                        // +0x108
    char unknown_10a[0x110 - 0x10a];
    union {
        unsigned int f110;             // +0x110
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
    unsigned short feature;            // +0x8
    unsigned char offsetY;             // +0xa
    unsigned char offsetX;             // +0xb
    char unknown_c;
};

struct Thing_0043e490 {
    char unknown_0[0xfe];
    unsigned char ffe;                 // +0xfe
};
#pragma pack(pop)

extern Game_0043e490* g_game;
extern const float DAT_004fd2e8;       // 0.0f

Cell_0043e490* __stdcall FUN_004815a0(Pos_0043e490* pos);
int __stdcall FUN_0049aa80(Unit_0043e490* unit, void* slot, Pos_0043e490* pos, int which);
int __stdcall FUN_0049abb0(Unit_0043e490* unit, Unit_0043e490* target, int which);
class Class_00489960 { public: int FUN_00489960(Unit_0043e490* other); };
class Class_004899b0 { public: int FUN_004899b0(Unit_0043e490* other); };
class Class_00489a70 { public: int FUN_00489a90(Unit_0043e490* other); };

static inline int Vtol(Def_0043e490* def)
{
    return (def->f241 >> 11) & 1;
}

static inline int Visible(Unit_0043e490* unit, Pos_0043e490* pos)
{
    Player_0043e490* p = unit->player;
    int x = pos->x >> 5;
    int y = (pos->z - (pos->y >> 1)) >> 5;
    return (unsigned int)x < p->width && (unsigned int)y < p->height &&
           ((1 << g_game->localPlayerBit) &
            g_game->visibility[p->width * y + x]) != 0;
}

static inline Thing_0043e490* Lookup(Pos_0043e490* pos)
{
    Cell_0043e490* cell = FUN_004815a0(pos);
    if (!cell)
        return 0;
    unsigned short id = cell->feature;
    if (id >= 0xfffb) {
        if (id != 0xfffe)
            return 0;
        id = (cell - (cell->offsetY * g_game->mapWidth + cell->offsetX))->feature;
        if (id >= 0xfffb)
            return 0;
    } else if (id >= (unsigned)g_game->unitCount) {
        return 0;
    }
    return (Thing_0043e490*)(g_game->units + (id << 8));
}

static inline int Marked(Thing_0043e490* t)
{
    return t && (t->ffe & 0x80);
}

// FUNCTION: 0x43e490
int __stdcall FUN_0043e490(unsigned char mode, Unit_0043e490* unit,
                           Unit_0043e490* target, Pos_0043e490* pos)
{
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
    case 1: {                          // approximate, not matching
        if (g_game->flag37efa == 1) {
            if (target && target->owner == g_game->localPlayer &&
                (target->f110 & 0x20) && target->f104 == 0.0f && target->ffb == 0) {
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
                if (Visible(unit, pos) && Marked(Lookup(pos)))
                    return 0x12;
            }
            if (!(def->f245 & 0x400))
                return 0x13;
            if (!Visible(unit, pos))
                return 0x13;
            if (!Marked(Lookup(pos)))
                return 0x13;
            return 0x12;
        }
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
    case 2:                            // approximate, not matching
        if (!(def->f245 & 0x80))
            return 0x13;
        if ((def->f245 & 0x800) && Visible(unit, pos) && Marked(Lookup(pos)))
            return 0xa;
        goto L43e9e0;
    case 3:
        if (def->f245 & 0x10) {
            if (def->f1ee->f111 & 0x100)
                return 2;
        }
        if ((def->f245 >> 4) & 1) {
            if (unit->moving)
                return 1;
            if (!target) {
                if (FUN_0049aa80(unit, (char*)unit + 0x6a, pos, 0) == 0)
                    return 3;
                if (unit->f10->f111 & 0x20000)
                    return 3;
                return 1;
            }
            if (FUN_0049abb0(unit, target, 0) == 0)
                return 3;
            return 1;
        }
        goto L43f098;
    case 4: {
        if (!(def->f245 & 0x4000))
            return 0x13;
        float a = *(float*)((char*)unit->fec + 0x8c);
        float b = *(float*)((char*)unit->f48 + 0xc0);
        if (a < b)
            return 3;
        float c = *(float*)((char*)unit->fec + 0x98);
        float d = *(float*)((char*)unit->f48 + 0xc4);
        if (c < d)
            return 3;
        return 1;
    }
    case 5:
        return (def->f245 & 0x100) ? 0xd : 0x13;
    case 6: {
        if (!target)
            return 0x13;
        if (!((Class_00489a70*)unit)->FUN_00489a90(target))
            return 0x13;
        return ((def->f241 >> 11) & 1) ? 8 : 0xc;
    }
    case 7:
        goto L43e615;
    case 8:
        if (((Class_004899b0*)unit)->FUN_004899b0(target))
            return 6;
        return 0x13;
    case 9:
        return (def->f245 & 0x40) ? 7 : 0x13;
    case 11:
        return 9;
    case 12:
        if (def->f245 & 0x400) {
            if (Visible(unit, pos) && Marked(Lookup(pos)))
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
    case 14:
        if (def->f156 == 0)
            return 0x13;
        if (unit->moving == 0)
            return 0x13;
        return 0x10;
    default:
        goto L43f098;
    }

L43e615:
    if (!(def->f245 & 0x20))
        goto L43f098;
    if (!friendly)
        goto L43f098;
    if (def->f241 & 0x800)
        goto L43eae8;
    if (target->def->f241 & 0x800)
        goto L43f098;
    return 5;

L43e9e0:
    if (!target || unit->moving == 0)
        goto L43eaf5;
    if (target->def->f245 & 0x1000) {
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
    if (target->def->f241 & 0x800) {
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

L43edb6:
    if (!target)
        goto L43ee4f;
    if (((Class_004899b0*)unit)->FUN_004899b0(target)) {
        if (!(target->f104 == 0.0f))
            return 6;
    }
    if (target->owner == g_game->localPlayer && (target->f110 & 0x20) &&
        target->f104 == 0.0f && target->ffb == 0) {
        if (target->f86 == 0 || (target->f86->f110 & 0x40000000))
            return 0xf;
    }
L43ee4f:
    if (def->f245 & 0x800) {
        if (Visible(unit, pos) && Marked(Lookup(pos)))
            return 0xa;
    }
    if (def->f245 & 0x400) {
        if (Visible(unit, pos) && Marked(Lookup(pos)))
            return 0xb;
    }
    return (def->f245 & 0x80) ? 8 : 0x13;

L43f098:
    return 0x13;
}
