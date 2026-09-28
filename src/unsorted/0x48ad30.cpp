// Decompiled by space-bunny-free. Names are provisional.
// The per-tick unit housekeeping loop (called from one place): clears the
// counter at g_game+0x14353, then for each of the ten 0x14b player records at
// g_game+0x1b63 walks the unit list (first +0x67, last +0x6b, stride 0x118),
// and for every unit that still has something to do (+0xa6) it ticks the
// animation counter, runs FUN_0049e1a0 for a human or computer player, stops
// the sound object, ages the two timers at +0xfa / +0xfb, clears flag bit 4
// when the unit has been unloaded, refreshes the two progress bytes +0xf6 and
// +0xf7 every 30 ticks, and then does the water damage, the shield/energy
// transfer every 8 ticks, the two per unit updates and the flag bit 14 effect.
// After the unit list it runs FUN_0048b710 when the global bit at +0x2a44 is
// set, and at the end it ages the 0x14371 counter when bit 1 of +0x14373 is
// set and FUN_004c1b80(0xf9) says no.
//
// NOT MATCHED: 73.8%, 828 bytes against 849. Everything except the outer
// loop's shape and a handful of register choices matches instruction for
// instruction. What is missing is exactly the 16 bytes of the loop's pre-test
// and the two induction variable initialisations:
//
//     xor al, al / cmp al, 0xa          <- the loop's entry guard
//     mov dword ptr [esp + 0x18], ebx    <- the counter pointer's home slot
//     mov dword ptr [ebx], 0
//     mov byte ptr [esp + 0x13], al      <- i = 0
//     mov dword ptr [esp + 0x14], 0      <- off = 0
//     jae 0x48b008                       <- into the latch, not past the loop
//
// so the original is a rotated for/while loop whose entry guard MSVC 5 kept
// (dead: the guard is never false because i starts at 0) and whose latch is
// the ordinary up-counting one, `load / load / inc al / add edx,0x14b /
// cmp al,0xa / store / store / jb`. Getting MSVC 5 to keep that guard while
// still fusing the increment into the latch is the whole remaining problem,
// and the trip count pass is what removes it: with a constant bound of 10 it
// rewrites the loop as a down counter in a fresh int register and drops the
// guard. Ruled out, each verified with a scratch score (see below):
// - `for (i = 0, off = 0; i < 10; i++, off += 0x14b)` and the same loop with
//   `off += 0x14b` as the last statement of the body: 69.5%, `mov dword ptr
//   [esp+0x18], 0xa` plus `dec eax` in the latch.
// - `while (i < 10) { ... off += 0x14b; i++; }`: 69.5%, same down counter.
// - `for (char i = 0; ...)`, `for (int i = 0; ...)`, and the body indexing
//   `g_game->players[i]` instead of walking a byte offset: 69.5%, 68.2% and
//   69.5%, all down counters. Note 0x406db0 and 0x456850 are MATCHed with
//   exactly `for (char i = 0; i < 10; i++)`: both have a `return` inside the
//   loop, and that early exit is what stops the trip count pass there. The
//   only two places in the whole exe with this loop's `xor al,al / cmp al,0xa`
//   guard are this function and 0x44fe40, which is also MATCHed and also has
//   an early `return i`. So the original almost certainly had an early exit
//   from this loop too, but its assembly has no edge from the body to the
//   epilogue other than through the latch, so it cannot be spelled in C++
//   without emitting that edge.
// - The flat body with `continue` for each of the three failed tests (the
//   latch is shared by all the exits in the original, which is what a
//   `continue` spells): 69.9%, still a down counter.
// - `i != 10`, `i <= 9`, `i = i + 1`, an `int off = 0` initialised before the
//   loop, an empty `for (; i < 10; )` third expression and `off` as the first
//   loop initialiser: every one of them is a down counter as well (checked by
//   compiling, not by scoring, since all of them lose the same 16 bytes).
// - `while (1) { if (i >= 10) break; ... }`, brief item 9's form: 70.8%, and
//   it does keep a guard, but the guard is a memory compare
//   (`cmp byte ptr [esp+0x13], 0xa`) instead of the original's register
//   compare on the value `xor al,al` has just produced.
// - The do-while that is in the file, `do { ... i++; off += 0x14b; } while
//   (i < 10);`: 71.8%, the best of these. Its latch is byte for byte the
//   original's, and swapping the two increment statements to `i++` first was
//   worth 0.5 points (71.3% to 71.8%), so the original increments the index
//   before the offset.
// - The local slot order follows from the same decision: the original homes
//   the counter pointer at +0x18 and the offset at +0x14, and the guard's
//   presence is what keeps the offset below it.
//
// Three things that were real bugs in earlier versions of this file, all
// worth points and all invisible in the pseudo-C: the unit's owner test in
// the flag bit 4 block is `!(owner->flags & 0x40000000)`, not
// `owner->flags & 0x40000000` (the original's `jne` skips the clear, so the
// clear needs the bit clear); `u->f110.bits.b4 != 0` rather than
// `u->f110.bits.b4` is what makes MSVC 5 emit the original's `test cl, 0x10`
// instead of a `shr eax, 4 / test al, 1` extraction; and loading the unit
// list's `last` (+0x6b) before its `first` (+0x67) is worth half a point
// (72.2% to 72.2% with the previous 71.8%, both scheduler tie-breaks).
//
// Also unmatched, and all downstream of the loop: the four register choices
// in the tail half (`mov eax` vs `mov edx` for the g_game reloads, `dx` vs
// `ax` for seaLevel, `dh` vs `ah` for the bit 12 test, `dl` vs `cl` for the
// progress byte swap) and the extra `mov ebx, [esp+0x18]` reload that the
// original only performs on the path through the player block. They are one
// decision: the original lets the type pointer die at the `div` and reloads
// `[esi+0x92]` for each later use, while this version keeps one copy alive
// in ecx across the progress byte swap, which is what costs it `dl` and
// pushes the following g_game load into edx. Tried and worse: the flag word
// at +0x110 as a plain int with mask tests (68.4%: the bit 14 test stops
// being a shift) and as a bitfield group starting at bit 0 instead of bit 4
// (71.2%).
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

struct Unit_0048ad30 {
    Class_0043dd20* def;                // +0x00
    char unknown_4[0x70 - 4];
    short f70;                         // +0x70
    char unknown_72[0x86 - 0x72];
    Unit_0048ad30* owner;              // +0x86
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
    void FUN_0043dd20(Unit_0048ad30* u);
};

struct Player_0048ad30 {
    int f0;                            // +0x00
    char unknown_4[0x67 - 4];
    Unit_0048ad30* f67;                // +0x67
    Unit_0048ad30* f6b;                // +0x6b
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

extern Game_0048ad30* g_game;

void __stdcall FUN_00437910(Unit_0048ad30* u);
void __stdcall FUN_0049e1a0(Unit_0048ad30* u);
void __stdcall FUN_0043b7c0(Unit_0048ad30* u);
void __stdcall FUN_0043bad0(Unit_0048ad30* u);
void __stdcall FUN_0048a870(Unit_0048ad30* u);
void __stdcall FUN_004864b0(Unit_0048ad30* u, int n);
void __stdcall FUN_00489bb0(int a, Unit_0048ad30* u, int damage, int kind, int flag);
int __stdcall FUN_0041bd10(Unit_0048ad30* u, Unit_0048ad30* u2, float f);
void __stdcall FUN_0048b710(Player_0048ad30* p);
void __stdcall FUN_0048d790(void);
int __stdcall FUN_004c1b80(int n);
void __stdcall FUN_0041c2e0(int n);

// FUNCTION: 0x48ad30
void __stdcall FUN_0048ad30(void)
{
    int* cnt = &g_game->f14353;
    unsigned char i;
    int off;
    *cnt = 0;
    do {
        Player_0048ad30* p = (Player_0048ad30*)((char*)&g_game->players[0] + off);
        if (p->f0 != 0) {
            unsigned char k = p->f73;
            if ((k == 1 || k == 2 || k == 3) && p->f146 != 0xa) {
                Unit_0048ad30* last = p->f6b;
                Unit_0048ad30* u = p->f67;
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
                                || u->owner == 0 || !(u->owner->f110.bits.b30)) {
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
                            unsigned char t = u->ff6;
                            u->ff6 = v;
                            u->ff7 = t;
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
                                    FUN_0041bd10(u, u, (float)(n / 15));
                                }
                            }
                            FUN_0043b7c0(u);
                            FUN_0043bad0(u);
                            if (u->def != 0) {
                                u->def->FUN_0043dd20(u);
                                FUN_0048a870(u);
                            }
                        }
                        if (u->f110.bits.b14) {
                            FUN_004864b0(u, u->ff5);
                        }
                    }
                    u = (Unit_0048ad30*)((char*)u + 0x118);
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
        i++;
        off += 0x14b;
    } while (i < 10);
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
