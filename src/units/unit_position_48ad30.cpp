// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by longcat-2.5-preview-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, checked by GPT-6., retried by Claude Opus 5.5, finished by Claude Opus 5.5. Names are provisional.
//
// MATCH (Claude Opus 5.5, #5296). What was left at 98.7% (the `off = 0` store
// scheduled early, the loop head loading off before g_game, and the
// `if (0) { g_leak = &off; }` escape that kept off in memory) all came from
// one thing: the front-end symbol id of g_game against those of the locals.
// With `extern g_game;` at file scope g_game is numbered before every local
// of the function. Declared inside the function after off, it is numbered
// after it, and then the plain source (no escape, the stores in the
// original's order `*cnt = 0; i = 0; off = 0;`) compiles to the original:
// off stays in its frame slot, g_game is the SIB base at the loop head and is
// loaded first.
//
// Checked with tools/c2prio.py --symbols and enum padding in scratch copies:
// with g_game back at file scope (id 258), an enum of 65400 or 65450 entries
// between g_game and the function also matches (off's id wraps past 65536
// to 166 or 216 in 16 bits, below g_game's 258), while 65500 (off at 266)
// and no padding both give 75.5% / 844 bytes. So the deciding comparison is
// g_game's id against off's,
// modulo 65536, as in 0x493bf0 (SIB base order of the g_game stores there).
// The other explanation is a lost header prefix that puts the 65536 wrap
// between g_game and this function (docs/c2-regalloc.md, "Symbol ids"); no
// real header set reaches that, so the function-scope extern is what is
// written here.
//
// Kept from earlier passes, still needed: the flat continue chain with
// PlayerMore(i) (a static inline testing the byte counter, which gives the
// original's unfolded `xor al,al / cmp al,0xa / jae` entry test), and
// FUN_0043b7c0, FUN_0043bad0 and the def block inside
// `if (k3 == 1 || k3 == 2)` (Claude Opus 5.5, #5106).
#pragma pack(push, 1)

class Class_00435100 {
public:
    char unknown_0[0xd4c];
    int waterDoesDamage;               // +0xd4c
    int waterDamage;                   // +0xd50
};

class Class_0043dd20;
struct Player_0048ad30;

class Class_004b0d60 {
public:
    char unknown_0[8];
    void FUN_004b0d60(int n);
};

struct Type_0048ad30 {
    char unknown_0[0x1fa];
    unsigned int f1fa;                 // +0x1fa
    char unknown_1fe[0x200 - 0x1fe];
    unsigned short f200;               // +0x200
    char unknown_202[0x241 - 0x202];
    union F241_0048ad30 {
        struct {
            unsigned int low : 12;
            unsigned int floats : 1;   // bit 12
            unsigned int rest : 19;
        } bits;
        unsigned int all;
    } f241;                            // +0x241
};

union F110_0048ad30 {
    struct {
        unsigned int low : 4;
        unsigned int b4 : 1;
        unsigned int b5 : 1;
        unsigned int mid : 8;
        unsigned int b14 : 1;
        unsigned int mid2 : 15;
        unsigned int b30 : 1;
        unsigned int top : 1;
    } bits;
    unsigned int all;
};

struct Unit {
    Class_0043dd20* def;                // +0x00
    char unknown_4[0x70 - 4];
    short f70;                         // +0x70
    char unknown_72[0x86 - 0x72];
    Unit* owner;                       // +0x86
    char unknown_8a[0x92 - 0x8a];
    Type_0048ad30* type;               // +0x92
    Player_0048ad30* player;           // +0x96
    Class_004b0d60* f9a;                // +0x9a
    char unknown_9e[0xa6 - 0x9e];
    unsigned short fa6;                // +0xa6
    char unknown_a8[0xf5 - 0xa8];
    unsigned char ff5;                 // +0xf5
    unsigned char ff6;                 // +0xf6
    unsigned char ff7;                 // +0xf7
    char unknown_f8[0xfa - 0xf8];
    unsigned char ffa;                 // +0xfa
    int ffb;                           // +0xfb
    char unknown_ff[0x104 - 0xff];
    float f104;                        // +0x104
    short f108;                        // +0x108
    char unknown_10a[0x110 - 0x10a];
    F110_0048ad30 f110;                // +0x110
    char unknown_114[0x118 - 0x114];
};

class Class_0043dd20 {
public:
    char unknown_0[0x8a];
    void FUN_0043dd20(Unit* u);
};

struct Player_0048ad30 {
    int f0;                            // +0x00
    char unknown_4[0x67 - 4];
    Unit* f67;                         // +0x67
    Unit* f6b;                         // +0x6b
    char unknown_6f[0x73 - 0x6f];
    unsigned char f73;                 // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char f146;                // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_0048ad30 {
    char unknown_0[0x1b63];
    Player_0048ad30 players[10];       // +0x1b63
    char unknown_1c3f[0x2a44 - 0x1b63 - 10 * 0x14b];
    unsigned char f2a44;               // +0x2a44
    char unknown_2a45[0x1427f - 0x2a45];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14353 - 0x14280];
    int f14353;                        // +0x14353
    char unknown_14357[0x14371 - 0x14357];
    short f14371;                      // +0x14371
    union F14373_0048ad30 {
        struct {
            unsigned int b0 : 1;
            unsigned int b1 : 1;
            unsigned int rest : 30;
        } bits;
        unsigned int all;
    } f14373;                          // +0x14373
    char unknown_14377[0x38a47 - 0x14377];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Class_00435100* mode;              // +0x391e9
};
#pragma pack(pop)


void __stdcall FUN_00437910(Unit* u);
void __stdcall FUN_0049e1a0(Unit* u);
void __stdcall FUN_0043b7c0(Unit* u);
void __stdcall FUN_0043bad0(Unit* u);
void __stdcall FUN_0048a870(Unit* u);
void __stdcall FUN_004864b0(Unit* u, int n);
void __stdcall FUN_00489bb0(int a, Unit* u, int damage, int kind, int flag);
int __stdcall FUN_0041bd10(Unit* u, Unit* u2, float f);
void __stdcall FUN_0048b710(Player_0048ad30* p);
void __stdcall FUN_0048d790(void);
int __stdcall FUN_004c1b80(int n);
void __stdcall FUN_0041c2e0(int n);

static inline int PlayerMore(unsigned char i)
{
    if (i >= 10) return 0;
    return 1;
}

// FUNCTION: 0x48ad30
void __stdcall FUN_0048ad30(void)
{
    int* cnt;
    unsigned char i;
    int off;
    // Declared here, after the locals, not at file scope: see the notes above.
    extern Game_0048ad30* g_game;
    cnt = &g_game->f14353;
    *cnt = 0;
    i = 0;
    off = 0;
    for (; i < 10; i++, off += 0x14b) {
        if (!PlayerMore(i)) continue;
        Player_0048ad30* p = (Player_0048ad30*)((char*)&g_game->players[0] + off);
        if (p->f0 == 0) continue;
        unsigned char k = p->f73;
        if (k != 1 && k != 2 && k != 3) continue;
        if (p->f146 == 0xa) continue;
        {
            Unit* last = p->f6b;
            Unit* u = p->f67;
            while (u <= last) {
                    if (u->fa6 != 0) {
                        (*cnt)++;
                        FUN_00437910(u);
                        if (p->f0 != 0) {
                            unsigned char k2 = p->f73;
                            if (k2 == 1 || k2 == 2) {
                                FUN_0049e1a0(u);
                            }
                        }
                        if (u->f9a != 0) {
                            u->f9a->FUN_004b0d60(1);
                        }
                        if (u->ffa != 0) {
                            u->ffa--;
                        }
                        if (u->ffb != 0) {
                            u->ffb--;
                        }
                        if (u->f110.bits.b4 != 0) {
                            if (!(u->f110.bits.b5) || u->f104 != 0.0f || u->ffb != 0
                                || (u->owner != 0 && !(u->owner->f110.bits.b30))) {
                                u->f110.bits.b4 = 0;
                            }
                        }
                        if (g_game->ticks % 30 == 0) {
                            int v = u->f108 * 100 / u->type->f1fa;
                            if (v < 0) {
                                v = 0;
                            }
                            if (v > 100) {
                                v = 100;
                            }
                            u->ff7 = u->ff6;
                            u->ff6 = v;
                        }
                        Player_0048ad30* pl = u->player;
                        if (pl->f0 != 0) {
                            unsigned char k3 = pl->f73;
                            if (k3 == 1 || k3 == 2) {
                                if (g_game->mode->waterDoesDamage != 0
                                    && g_game->mode->waterDamage != 0
                                    && g_game->ticks % 30 == 0 && u->f70 <= g_game->seaLevel
                                    && !u->type->f241.bits.floats) {
                                    FUN_00489bb0(0, u, g_game->mode->waterDamage, 0xb, 0);
                                }
                                if (u->type->f200 != 0 && u->f108 < u->type->f1fa
                                    && (g_game->ticks & 7) == 0) {
                                    int n = u->type->f200 * 8;
                                    FUN_0041bd10(u, u, (float)(n / 30));
                                }
                                FUN_0043b7c0(u);
                                FUN_0043bad0(u);
                                if (u->def != 0) {
                                    u->def->FUN_0043dd20(u);
                                    FUN_0048a870(u);
                                }
                            }
                        }
                        if (u->f110.bits.b14) {
                            FUN_004864b0(u, u->ff5);
                        }
                    }
                    u = (Unit*)((char*)u + 0x118);
                }
                if (g_game->f2a44 & 1) {
                    if (p->f0 != 0) {
                        unsigned char k4 = p->f73;
                        if (k4 == 1 || k4 == 2) {
                            FUN_0048b710(p);
                        }
                    }
                }
            }
        }
    if (g_game->f14373.bits.b1) {
        if (!FUN_004c1b80(0xf9)) {
            g_game->f14371--;
            if (g_game->f14371 <= 0) {
                g_game->f14371 = 0x5a;
                FUN_0048d790();
                FUN_0041c2e0(0);
            }
        }
    }
}
