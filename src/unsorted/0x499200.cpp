// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol. Names are provisional.
// Header sweeps (128 C and 768 C/C++ combinations) found no improvement.
// PARTIAL, 91.7% (1653 of 1655 bytes, instruction-text score). Branches, field
// offsets, call targets, stack slots and the whole 0x4992cd..0x4996fb state
// machine agree with the original; what is left is register roles only.
// What this session added (deepseek's notes above are superseded):
// - +0x2cc6 is NOT involved, but +0x2a44 and +0x3923b are bitfield unions now.
//   +0x2a44: `|= 8` is a byte RMW and `|= <const>` is a WORD RMW, so the field
//   is `union { unsigned short value; struct { ...b3:1... } bits; }` and the
//   two statements are `bits.b3 = 1;` and `value |= four;`.
//   +0x3923b: the original is `mov ax,word[m]; test al,0x14`, a 16-bit load
//   followed by an 8-bit test, which is what a `||` of two bitfields in a
//   16-bit storage unit gives: `bits.b2 || bits.b4`. A plain `unsigned short`
//   local folds to `test byte ptr [m],0x14` and loses the load. This was the
//   only scoring change of the session (+0.5).
// What still differs, and everything tried:
// - the constant 4 lives in edi from 0x499603 to 0x49986b in the original
//   (`mov edi,4`, `push edi` twice, `or word [m],di`); ours rematerialises it
//   as `push 4` and `or word [m],4`. Tried and all identical at 91.7%:
//   int/unsigned/short/unsigned short/char/const local; `four = four`,
//   `four += 0`, `four *= 1`, `four ^= 0`, `four2 = four`, `if (four == 4)`,
//   a dead extra `|=`/`&=` use, `static int four = 4`, an enum constant,
//   `4.0f` and `4.0` initialisers, a local struct with an `int` member built
//   by a constructor, a bitfield union local, `int four_tab[1] = {4}`, a dead
//   __inline helper to change the /Ob2 budget, and two __inline helpers
//   (`Push(n)` / `Or(n)` and one `Mode(n)`) called with the literal 4 so the
//   constant would arrive through a parameter. MSVC 5 folds every one of them
//   into an immediate, so I could not find the construct the original used.
// - revisited by deepseek-v4.1-flash: `register int four = 4;`, moving the
//   declaration to the top of the function body (live range across all the
//   frame-queue calls), splitting it into `int four; four = 4;`, and
//   `unsigned short four = 4;` all still give 91.7% with byte-identical output
//   to the version above. The edi live range at 0x499603..0x49986b is not
//   reproducible from a literal-4 local, so the original evidently built the
//   value some way MSVC 5 refuses to fold.
// - the second arm of the +0x39249 block then uses edx where we use eax for
//   g_game, and builds the FUN_00435a20 argument in ecx before pushing instead
//   of edx. Both look like knock-on effects of the missing edi live range
//   (the register allocator never had to preserve a value there), but they
//   were not separable: the score does not move when the constant is removed
//   from any single use site.
// PARTIAL, 91.2% (1651 of 1655 bytes). Every branch, field offset, call and
// stack slot agrees; what is left is register/scheduling only:
// - +0x3923b: the original is `mov ax,[mem]; mov edi,4; test al,0x14`, this
//   source folds to `test byte ptr [mem],0x14` because the value is single
//   use. A plain `unsigned short` local folds for pointer-based loads; the
//   16-bit load only survives when the field is a direct global.
// - the constant 4 stays in edi across the whole +0x3923b arm (`push edi`,
//   `or word [mem],di`); here MSVC rematerialises it as immediates.
// - register-role swaps when building the +0x519 argument and the
//   FUN_00435a20 argument (original uses eax/edx, ours ecx).
// Main-loop frame handler. Copies the 24-byte view/input block off g_game,
// feeds it to the camera update, then runs the order/selection state machine
// off the flags byte at +0x2cc6 and the mouse message stored in the block.
// Advances the frame queues and, on the network/skirmish paths, flips the
// end-of-frame hooks.
#include <stdlib.h>

#pragma pack(push, 1)

struct View_00499200 {
    int x;                             // +0x0
    int y;                             // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    int msg;                           // +0x10
    int field_14;                      // +0x14
};

struct Struct_00499200_531 {
    int unknown_0;
    int value;                         // +0x4
};

struct BitFlags16_00499200 {
    unsigned short b0:1;
    unsigned short b1:1;
    unsigned short b2:1;
    unsigned short b3:1;
    unsigned short b4:1;
    unsigned short b5:1;
    unsigned short b6:1;
    unsigned short b7:1;
    unsigned short b8:1;
    unsigned short b9:1;
    unsigned short b10:1;
    unsigned short b11:1;
    unsigned short b12:1;
    unsigned short b13:1;
    unsigned short b14:1;
    unsigned short b15:1;
};

union Flags16_00499200 {
    unsigned short value;
    BitFlags16_00499200 bits;
};

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_00435c50 {
public:
    int FUN_00435c50();
};

class Class_004352b0 {
public:
    char* FUN_004352b0();
};

class Class_00435110 {
public:
    void FUN_00435110(void* p);
};

class Class_00435c00 {
public:
    int FUN_00435c00(int a);
};

class Class_00435a20 {
public:
    void FUN_00435a20(void* p);
};

class Class_004ce690 {
public:
    void FUN_004ce690(int a);
};

struct Game_00499200 {
    char unknown_0[0x10];
    Class_004ce690* field_10;          // +0x10
    char unknown_14[0x519 - 0x14];
    char field_519[0x18];              // +0x519
    Struct_00499200_531* field_531;    // +0x531
    char unknown_535[0x29a0 - 0x535];
    char* field_29a0;                  // +0x29a0
    char unknown_29a4[0x2a3c - 0x29a4];
    unsigned short field_2a3c;         // +0x2a3c
    char unknown_2a3e[0x2a44 - 0x2a3e];
    Flags16_00499200 field_2a44;       // +0x2a44
    char unknown_2a46[0x2c76 - 0x2a46];
    View_00499200 view;                // +0x2c76
    char unknown_2c8e[0x2c92 - 0x2c8e];
    int field_2c92;                    // +0x2c92
    int field_2c96;                    // +0x2c96
    int field_2c9a;                    // +0x2c9a
    int field_2c9e;                    // +0x2c9e
    int field_2ca2;                    // +0x2ca2
    int field_2ca6;                    // +0x2ca6
    char unknown_2caa[0x2cac - 0x2caa];
    short field_2cac;                  // +0x2cac
    char unknown_2cae[0x2cb0 - 0x2cae];
    short field_2cb0;                  // +0x2cb0
    char unknown_2cb2[0x2cb4 - 0x2cb2];
    short field_2cb4;                  // +0x2cb4
    int field_2cb6;                    // +0x2cb6
    unsigned short field_2cba;         // +0x2cba
    char unknown_2cbc[0x2cbe - 0x2cbc];
    signed char selected;              // +0x2cbe
    char unknown_2cbf[0x2cc3 - 0x2cbf];
    unsigned char orderMode;           // +0x2cc3
    char unknown_2cc4[0x2cc6 - 0x2cc4];
    unsigned char flags_2cc6;          // +0x2cc6
    char unknown_2cc7[0x2cdf - 0x2cc7];
    int field_2cdf;                    // +0x2cdf
    char unknown_2ce3[0x14357 - 0x2ce3];
    char* units;                       // +0x14357
    char unknown_1435b[0x1487f - 0x1435b];
    void* table[21];                   // +0x1487f
    char unknown_148d3[0x37e9c - 0x148d3];
    unsigned short field_37e9c;        // +0x37e9c
    char unknown_37e9e[0x37efa - 0x37e9e];
    int field_37efa;                   // +0x37efa
    char unknown_37efe[0x391e9 - 0x37efe];
    Class_00435100* net;               // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int field_391f1;                   // +0x391f1
    void (*field_391f5)(void);         // +0x391f5
    char unknown_391f9[0x3923b - 0x391f9];
    Flags16_00499200 field_3923b;      // +0x3923b
    char unknown_3923d[0x39249 - 0x3923d];
    int field_39249;                   // +0x39249
};
#pragma pack(pop)

extern Game_00499200* g_game;

void FUN_004197d0();
void FUN_0041c180();
void FUN_0041cd50();
void FUN_0041d0f0();
void __stdcall FUN_0041d9f0(int a);
void FUN_004257a0();
int FUN_004572a0();
void FUN_00463c80();
void FUN_0047a760();
void FUN_0048bd00();
int __stdcall FUN_0048c390(void* p);
unsigned short __stdcall FUN_0048cd80();
int __stdcall FUN_0048d220(char mode);
void FUN_00491b60();
void __stdcall FUN_00491d70(int a);
void FUN_00496790();
void __stdcall FUN_00498da0(View_00499200* p);
void __stdcall FUN_00498f70(View_00499200* p);
void __stdcall FUN_00499100(View_00499200* p);
int __stdcall FUN_0049fe60(int value, char* name);
void __stdcall FUN_004a6a40(void* obj, int index);
void __stdcall FUN_004a9660(void* a);
void __stdcall FUN_004ab400(void* a, void* b);
void __stdcall FUN_004b4fd0(void* a, int b);
int FUN_004b6340();
void FUN_004c1a40();
int __stdcall FUN_004c1b80(int a);
void FUN_00499880();
void FUN_00496bb0();
void FUN_004578f0();

// FUNCTION: 0x499200
void FUN_00499200(void)
{
    View_00499200 view = g_game->view;
    FUN_00498da0(&view);

    unsigned char flags = g_game->flags_2cc6;
    if ((flags & 2) != 0 && g_game->orderMode == 0xe) {
        FUN_004197d0();
    } else if ((flags & 2) == 0 && (flags & 1) == 0) {
        if (g_game->selected != 0x13) {
            g_game->selected = 0x13;
            FUN_004ab400(g_game->field_519, g_game->table[0x13]);
        }
    } else {
        g_game->field_2cba = FUN_0048cd80();
        int idx = FUN_0048d220(g_game->orderMode);
        if (g_game->selected != idx) {
            g_game->selected = (signed char)idx;
            FUN_004ab400(g_game->field_519, g_game->table[idx]);
        }
    }

    FUN_0041c180();
    if ((g_game->flags_2cc6 & 0x20) != 0) {
        if (FUN_004c1b80(0xf9) == 0) {
            g_game->orderMode = 1;
            g_game->flags_2cc6 &= 0xdf;
            int index = FUN_0049fe60(g_game->field_531->value, "STOP");
            if (index != -1) {
                FUN_004a6a40(g_game->field_519, index);
            }
        }
    }

    flags = g_game->flags_2cc6;
    if ((flags & 0x10) != 0) {
        if (g_game->field_37efa == 0) {
            if (view.msg == 0x205) {
                g_game->flags_2cc6 = flags & 0xef;
            } else {
                FUN_0041d0f0();
            }
        } else {
            if (view.msg == 0x202) {
                g_game->flags_2cc6 = flags & 0xef;
            } else {
                FUN_0041d0f0();
            }
        }
    } else if (g_game->field_2cdf != 0) {
        FUN_0041cd50();
    } else if (view.msg == 0x204) {
        FUN_00499100(&view);
    } else if (g_game->orderMode != 1) {
        if (view.msg == 0x201) {
            FUN_00498f70(&view);
        }
    } else if ((flags & 8) != 0) {
        if (view.msg == 0x202) {
            g_game->flags_2cc6 = flags & 0xf7;
            int dx = g_game->field_2c92 - g_game->field_2c9e;
            int dz = g_game->field_2c9a - g_game->field_2ca6;
            dx = abs(dx);
            dz = abs(dz);
            int now = FUN_004b6340();
            if (g_game->field_2cb6 + 0x19 > now && dx < 0x20 && dz < 0x20) {
                FUN_00498f70(&view);
            } else if (FUN_0048c390(&view) == 0) {
                FUN_0048bd00();
                FUN_00491d70(1);
            }
        } else {
            g_game->field_2c9e = g_game->field_2cac;
            g_game->field_2ca2 = g_game->field_2cb0;
            g_game->field_2ca6 = g_game->field_2cb4;
        }
    } else if (view.msg == 0x201) {
        if ((flags & 2) != 0) {
            g_game->flags_2cc6 = flags | 8;
            g_game->field_2cb6 = FUN_004b6340();
            g_game->field_2c92 = g_game->field_2cac;
            g_game->field_2c96 = g_game->field_2cb0;
            g_game->field_2c9a = g_game->field_2cb4;
            g_game->field_2c9e = g_game->field_2cac;
            g_game->field_2ca2 = g_game->field_2cb0;
            g_game->field_2ca6 = g_game->field_2cb4;
            if (g_game->selected != 0x13) {
                g_game->selected = 0x13;
                FUN_004ab400(g_game->field_519, g_game->table[0x13]);
            }
        } else if (g_game->field_37efa == 1) {
            if ((flags & 1) != 0) {
                g_game->flags_2cc6 = flags | 0x10;
                if (g_game->selected != 0x13) {
                    g_game->selected = 0x13;
                    FUN_004ab400(g_game->field_519, g_game->table[0x13]);
                }
            }
        } else if ((flags & 1) != 0) {
            FUN_00498f70(&view);
        }
    }

    FUN_00496790();
    {
        unsigned short unit = g_game->field_37e9c;
        if (unit != 0 && *(short*)(g_game->units + unit * 0x118 + 0xa6) == 0) {
            FUN_00491d70(0);
        }
    }

    int four = 4;
    if (g_game->field_3923b.bits.b2 || g_game->field_3923b.bits.b4) {
        if (g_game->net->FUN_00435100() != 3 ||
            (((Class_00435100*)g_game->net)->FUN_00435100() == 3 &&
             FUN_004572a0() != 0)) {
            if (g_game->selected != 0x13) {
                g_game->selected = 0x13;
                FUN_004ab400(g_game->field_519, g_game->table[0x13]);
            }
            FUN_00491d70(1);
            FUN_004a9660(g_game->field_519);
            if (g_game->net->FUN_00435100() == 3) {
                FUN_00463c80();
                FUN_00496790();
            }
            FUN_00491b60();
            FUN_004c1a40();
            g_game->field_10->FUN_004ce690(four);
            g_game->field_391f1 = 7;
            g_game->field_391f5 = FUN_00499880;
            FUN_004b4fd0(FUN_004578f0, 0);
            FUN_0041d9f0(0);
        }
    }

    if (g_game->field_39249 != 0) {
        if (g_game->net->FUN_00435100() == 1) {
            FUN_00491b60();
            FUN_00491d70(1);
            FUN_004a9660(g_game->field_519);
            FUN_004257a0();
            int a = ((Class_00435c50*)g_game->net)->FUN_00435c50();
            char* b = ((Class_004352b0*)g_game->net)->FUN_004352b0();
            ((Class_00435110*)g_game->net)->FUN_00435110(b);
            if (((Class_00435c00*)g_game->net)->FUN_00435c00(a) != 0) {
                g_game->field_2a44.bits.b3 = 1;
                g_game->field_2a44.value |= four;
            }
        } else {
            unsigned int saved = g_game->field_2a3c;
            FUN_00491b60();
            FUN_00491d70(1);
            FUN_004a9660(g_game->field_519);
            FUN_004257a0();
            if (g_game->selected != 0x14) {
                g_game->selected = 0x14;
                FUN_004ab400(g_game->field_519, g_game->table[0x14]);
            }
            g_game->field_2a3c = saved;
            ((Class_00435a20*)g_game->net)->FUN_00435a20(g_game->field_29a0 + 0x11c);
            FUN_0047a760();
        }
        g_game->field_391f1 = 2;
        g_game->field_391f5 = FUN_00496bb0;
        FUN_004b4fd0(FUN_004578f0, 0);
        g_game->field_10->FUN_004ce690(four);
    }
}
