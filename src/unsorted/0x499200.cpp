// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, edited by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-opus-5-5, edited by DeepSeek V4.1 Flash. Names are provisional.
// claude-opus-5-5 (#4267): 98.4% -> 99.5% at the exact size (1655 bytes).
// One register is left: the g_game load for the FUN_00435c00 call at 0x499775
// is `mov ecx` here and `mov edx` in the original.
// What moved it: MSVC 5 hands fresh g_game loads out in an eax -> ecx -> edx
// rotation that runs on from the then arm into the else arm, and a load into
// a named local does not advance it (the local takes the first free register,
// eax unless eax is busy). Measured on the version without the `gp` local
// (`unsigned int saved = g_game->field_2a3c;`, 94.7%): its else arm and whole
// tail sat exactly one rotation step past the original, and deleting any one
// g_game load from the then arm's first block put every register from
// 0x4997a0 to the end in place. So the FUN_00435110 call now goes through
// `Game_00499200* g = g_game;`: that load is a local (ecx, since eax still
// holds the FUN_004352b0 result), the rotation no longer advances there, and
// the else arm and tail match. The cost is that the next rotating load, the
// FUN_00435c00 one, now gets ecx instead of edx. In rotation terms the
// original looks like: 0x499763 rotates (ecx), 0x499775 does NOT rotate but
// still lands in edx, and 0x4997a0 rotates (edx). A local at 0x499775 takes
// eax (98.1%), so the missing piece is a non-rotating load that avoids eax and
// ecx there. Also measured: the `bits.b3 = 1` and `|= 4` read-modify-writes
// in the inner block and the calls do not advance the rotation, a plain store
// there (`g_game->field_391f1 = 9;`) does.
// Also kept, both byte-identical to the previous file: the `int four = 4;`
// local is the literal 4 again (the two `|= 4` copies are what keep 4 in edi),
// and the five inlined FUN_00491c80 bodies are a SetCursor(n) helper.
// Unused declarations from 0 to 594 are flat and no header set helps.
// Tried for 0x499775 without success: locals, references, const locals and
// inline-helper parameters for g_game or g_game->net at that call (98.1%), the
// same local reused for both calls (96.8%), int/bool result locals, a switch,
// `== 0 {} else`, `goto`, swapped arms (88.4%), inline helpers for the shared
// call head, each arm, the FUN_00435c00 sequence and the whole field_39249
// block, the gp local before the branch (99.2%, the load is not sunk), inline
// getters for g_game->net, unused declarations 0 to 594 and every header set
// on this file (all flat), and permuter runs of 15 minutes from the 98.4% file
// and 35 minutes from this one (about 37,000 candidates, no gain).
// Earlier notes (98.4% file): PARTIAL, 98.4% (1654 of 1655 bytes). Every branch, field offset, call target,
// stack slot, jump target and register role now agrees with the original except
// ONE instruction: at 0x4997a0 the original loads g_game with
// `mov edx, dword ptr [0x511de8]` (6 bytes) and we emit the 5-byte A1 form
// `mov eax, dword ptr [0x511de8]`; that single byte shifts every later address
// by one. What moved the number this session, from 91.7%:
// - the `|= 4` is NOT inside `if (net->FUN_00435c00(a) != 0)`. The original's
//   `or word ptr [eax + 0x2a44], di` sits at 0x49982a, the tail of the ELSE arm
//   of `if (net->FUN_00435100() == 1)`, and the inner `if` tail-merges into it
//   (`jmp 0x49982a`). Writing the statement out in full at the end of BOTH arms
//   makes MSVC place the constant in edi across the whole tail, which is what
//   finally produced `mov edi, 4` (a plain `int four = 4;` local is folded to
//   immediates and yields `push 4` instead). 91.7% to 94.7%.
// - the saved +0x2a3c value is read through a named pointer local
//   (`Game_00499200* gp = g_game; unsigned int saved = gp->field_2a3c;`).
//   That extra graph node is what puts g_game in eax for the FUN_004a9660
//   argument, in ecx for the FUN_004ab400 table entry, in edx for the
//   FUN_00435a20 argument and in eax/ecx/edx for the three tail stores, all of
//   which the plain `g_game->field_2a3c` spelling got wrong. 94.7% to 98.4%.
// Still differs, and everything tried for the last byte:
// - the base register of the +0x2a3c load. `gp = g_game` still lands in eax
//   (MSVC prefers eax for the short A1 encoding of a global). Tried and all
//   identical at 98.4%: a reference (`Game_00499200& game = *g_game`), a
//   `char*` plus offset cast, a `const` pointer, a signed `int saved`, an
//   explicit `(unsigned short)` cast, a `static inline` getter with and without
//   a pointer parameter, a split `unsigned int saved;` declaration before the
//   pointer, a `saved = 0` first statement, a second identical pointer local,
//   and the pointer local in the enclosing block instead (95.6%) or used for the
//   +0x2a3c store-back as well (95.2%). Declaring `saved` in the enclosing
//   block with the load there too also fixes every role but moves the load to
//   0x499709, nine bytes early.
// - deepseek-v4.1 tried 14 more byte-neutral spellings; every one compiles
//   byte-identically to the base above, so the pick is not reachable through the
//   read's shape: an `(unsigned int)` cast, `&*g_game`, an indexed ushort form
//   (`((unsigned short*)gp)[0x151e]`), an address-arithmetic form
//   (`gp->unknown_29a4 + 0x98`), a through-`void`/`char` cast pointer, a
//   `register` pointer, `unsigned int const four`, a named local for the
//   FUN_00435100 and FUN_00435c00 results, a named `one` for FUN_00491d70, and a
//   coalesced duplicate pointer temp. Removing the gp local instead gives 1656
//   bytes at 94.7% (it rotates the second tail block's base registers), so gp is
//   load-bearing for the tail and only its first use's register differs.
// deepseek-v4.1 session 2 (issue #2498) added ~40 more shapes, all of which
// compile byte-identically to the 98.4% base: offsetof-style constant, `& 0xffff`,
// `+ 0u`, a reference to the field, a pointer to the field, a reinterpret_cast
// ushort pointer, `&*g_game`, a typedef'd pointer type, a nested scope around
// the calls, `(void)gp->field_2a3c;` re-read (dead), a second dead pointer local,
// an explicit `__stdcall`, truthiness instead of `!= 0`, `(void (*)(void))` on the
// hook store, `&g_game->field_519[0]`, and defining gp as the first statement of
// the enclosing `if (field_39249 != 0)` block (gp is rematerialised back to the
// same code). Only shapes that ADD a graph node move anything, and every one of
// them lands in one of exactly two attractor states, both wrong:
//   A (this file): 0x4997a0 eax, 0x4997bb eax, tail block 2 (eax, ecx, edx).
//   B (no gp, or the read through a pointer-to-pointer / chain / int arithmetic):
//     0x4997a0 eax, 0x4997bb ecx, tail block 2 (edx, eax, ecx), 1656 bytes at 94.7.
// The original is A's tail with B's first load bumped one register on: (edx, eax).
// Making gp live across the two calls (so it is used again for the FUN_004a9660
// argument) is the only thing that gives 0x4997a0 a 6-byte form, but it goes to a
// callee-saved register and also displaces `mov edi, 4`, giving 1648 bytes at 95.0.
// So the last byte needs one extra *invisible* temp in the allocator's view of the
// block that ends at 0x499717, which no source shape tried so far produces.
// deepseek-v4.1 session 4 (issue #2598): 25 more shapes, all byte-neutral at 1654
// bytes / 98.4% (so the pick is not reachable from the read, the arm boundary, or
// the condition): nested FUN_004352b0 argument, pointer-to-field temp, dead field
// read, char*/void* address forms, a second dead pointer local, function-scope
// `a`/`b`/`r`/`f`/`gp`/`saved` declarations (declaring a then-arm value in the
// enclosing block does NOT reserve its register into the else arm), the then-arm
// bit set through a named pointer or a named constant, named netMode used in the
// else arm through a folded mask, `switch`/negated-condition rewrites (both change
// the emitted bytes), casts on the condition, and long/const-pointer variants.
// Score still 1655 vs 1654 bytes, one instruction: 0x4997a0 edx vs eax.
// deepseek-v4.1 session 3 (issue #2565): 10 more byte-neutral shapes, all land on the
// same 98.4% base (1654 bytes) with `mov eax, dword ptr [<addr>]` at 0x4997a0, so the
// byte is NOT reachable by spelling the read: an `unsigned short tmp` plus a
// `unsigned int saved = tmp` two-vertex form, the `register` keyword on both locals,
// `g_game + 0` as the initializer, a self-assignment `gp = gp;`, a C++ `static_cast`
// around the field, `*(&gp->field_2a3c)`, a comma expression `(gp = g_game, ...)`, a
// truthiness test on field_39249 plus a `Class_00435100&` alias for the call, and a
// `gp` assignment inside the arm (instead of an initializer) next to a named local
// for the FUN_00435100 result. Making `saved` itself an `unsigned short` changes the
// tail and is worse: 1652 bytes at 98.3%. The else block's first pick is sticky eax,
// so the original's 6-byte `mov edx` needs an allocator-visible value that this source
// never creates, not another spelling of the read.
// deepseek-v4.1 session 5 (issue #2637) tried the two shapes that could keep the
// hoisted address node AND `mov edi, 4`: (a) `unsigned short* fp = &g_game->field_2a3c;`
// after the four declaration, used for the load and the store-back, gives
// `lea edi, [ecx + 0x2a3c]` at 0x499603 (fp steals edi because it is live across
// the two calls) plus `push 4` twice, 1636 bytes at 95.2; (b) keeping the gp
// pointer live across the else arm's calls forces ebp into the prologue (push ebp,
// all stack slots shift) and drops the function to 1629 bytes at 75.0. A pointer
// live across a call is always callee-saved here, so it can never be the original's
// edx: the 0x4997a0 pick needs a short-lived value, not a live one.
// deepseek-v4.1-flash session 6 (issue #3466): `if (1 == ...)` operand-order flip is
// byte-neutral at 1654/98.4; a named `net`/`mode` pair used in both arms drops to 1608
// bytes (net kept across calls). Register-pick survey of this function suggests the
// allocator blocks eax and ecx at 0x4997a0 in the original (pick order eax, edx, ecx),
// so the 6-byte `mov edx` needs one more value live at the else arm's entry; no shape
// tried here produces it.
// deepseek-v4.1-flash session 7 (issue #4001): the address-of-global double-pointer
// shape `Game_00499200** pp = &g_game; Game_00499200* gp = *pp;` ahead of the
// field_2a3c read is byte-identical to the 98.4 percent base (1654 bytes, same
// 0x4997a0 eax), so the address-of node is not the missing live value either.
// deepseek-v4.1-flash session 8 (issue #4122): no new shape run, analysis only.
// Constraint on the missing temp: it must occupy eax AT 0x4997a0 yet be gone by
// 0x4997bb (where the original itself picks A1 eax). The two calls at 0x4997af
// and 0x4997b6 clobber eax, so the range must end before them, and the only
// instructions in between are the gp load and the xor/mov-si saved load, which
// use only edx and esi: a live temp there would have to fuse its last use into
// those bytes or be dead-but-allocated (phi-like or block-granular range end).
// The then arm's g_game loads cycle ecx (0x499729), edx (0x499740), eax
// (0x499753), ecx (0x499763), edx (0x499775), eax (0x49978f); the else arm's
// first pick in the original is edx, i.e. exactly one step on from the last
// then-arm pick, which fits a rotating register hint whose state our build
// resets before 0x4997a0. Still differs only at 0x4997a0: edx vs eax.
// deepseek-v4.1-flash session 9 (issue #4197): four more shapes, all byte-neutral at
// 1654 bytes / 98.4 percent with 0x4997a0 still `mov eax`: naming the FUN_00435100
// result and testing it as `if (mode == 1) ... else if (mode != 1)` (the folded
// else-if emits no bytes but the value is killed at the branch, so eax is not
// blocked); the same test nested inside the else block; splitting the two locals
// into declarations plus assignments; and `unsigned int saved;` declared before the
// pointer. The liveness route is closed: any value kept live into the else arm is
// either folded away by the value tracker before allocation or, when it really
// survives, forces a callee-saved register and shrinks the function. The 6-byte
// `mov edx` is not reachable from source shape here.
// DeepSeek V4.1 Flash session 10 (2026-10-02, issue #4929): re-ran the 3 minute
// permuter (1546 candidates, no gain) and then a register-pick survey on the
// then-arm tail. Enumerated all 16 placements of a dead g_game local at the four
// accesses (FUN_00435c50/FUN_004352b0/FUN_00435110/FUN_00435c00): only a local
// at the FUN_00435110 call (the current file) reaches 99.5, every other
// placement is 94.7 to 98.1. Also tried, all byte-neutral at 99.5 with
// 0x499775 still ecx: a `Class_00435c00*`/`void*`/`int` local for the net
// pointer at the FUN_00435c00 call (folds back to the direct read), the
// FUN_00435c00 result in a named local, `g2 = g`, `g = g_game` repeated or
// reassigned, a nested scope or do/while(0) around the FUN_00435110 call, and
// char*/void*/address/typedef spellings of the g local. The FUN_00435110 local
// is load-bearing for the tail (it is what gives 0x4997a0 edx) and it is also
// what forces 0x499775 to ecx; the original wants that load in edx with the
// same tail. No source shape here produces both, so it is stuck.
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
void __cdecl FUN_004578f0();

static inline void SetCursor(int n)
{
    if (g_game->selected != n) {
        g_game->selected = n;
        FUN_004ab400(g_game->field_519, g_game->table[n]);
    }
}

// FUNCTION: 0x499200
void FUN_00499200(void)
{
    View_00499200 view = g_game->view;
    FUN_00498da0(&view);

    unsigned char flags = g_game->flags_2cc6;
    if ((flags & 2) != 0 && g_game->orderMode == 0xe) {
        FUN_004197d0();
    } else if ((flags & 2) == 0 && (flags & 1) == 0) {
        SetCursor(0x13);
    } else {
        g_game->field_2cba = FUN_0048cd80();
        SetCursor(FUN_0048d220(g_game->orderMode));
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
            SetCursor(0x13);
        } else if (g_game->field_37efa == 1) {
            if ((flags & 1) != 0) {
                g_game->flags_2cc6 = flags | 0x10;
                SetCursor(0x13);
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

    if (g_game->field_3923b.bits.b2 || g_game->field_3923b.bits.b4) {
        if (g_game->net->FUN_00435100() != 3 ||
            (((Class_00435100*)g_game->net)->FUN_00435100() == 3 &&
             FUN_004572a0() != 0)) {
            SetCursor(0x13);
            FUN_00491d70(1);
            FUN_004a9660(g_game->field_519);
            if (g_game->net->FUN_00435100() == 3) {
                FUN_00463c80();
                FUN_00496790();
            }
            FUN_00491b60();
            FUN_004c1a40();
            g_game->field_10->FUN_004ce690(4);
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
            Game_00499200* g = g_game;
            ((Class_00435110*)g->net)->FUN_00435110(b);
            if (((Class_00435c00*)g_game->net)->FUN_00435c00(a) != 0) {
                g_game->field_2a44.bits.b3 = 1;
                g_game->field_2a44.value |= 4;
            }
        } else {
            unsigned int saved = g_game->field_2a3c;
            FUN_00491b60();
            FUN_00491d70(1);
            FUN_004a9660(g_game->field_519);
            FUN_004257a0();
            SetCursor(0x14);
            g_game->field_2a3c = saved;
            ((Class_00435a20*)g_game->net)->FUN_00435a20(g_game->field_29a0 + 0x11c);
            FUN_0047a760();
            g_game->field_2a44.value |= 4;
        }
        g_game->field_391f1 = 2;
        g_game->field_391f5 = FUN_00496bb0;
        FUN_004b4fd0(FUN_004578f0, 0);
        g_game->field_10->FUN_004ce690(4);
    }
}
