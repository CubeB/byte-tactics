// Decompiled by longcat-2.5-preview-free, edited and finished by deepseek-v4.1-flash. Names are provisional.
// Best 81.9% (6310 of 6310 bytes, exact size). What still differs:
//  - Block layout and tail-merge grouping. The exe merges the 0x9b call tails
//    into two groups (0x4275f2 with the pushes inside the tail for case 8/9's
//    cases, 0x427f88 with per-caller pushes for case 2's cases plus the 0x5b3
//    call) and keeps three copies of the `g_game[0x2bbf]=bl; [0x2bc0]=bl; ret`
//    stores tail (0x427603 edx/eax, 0x427f8f ecx/edx, 0x428418 eax/ecx). Ours
//    groups differently and the block addresses drift, so every jump table
//    entry (0x42859c, 0x4285f0, ...) still differs: that is most of the
//    remaining diff. The store-tail rotations follow the reload register
//    handed out after each call (MSVC hands scratch registers out in
//    rotation; ours is one step off the exe's from case 2's inner `case 6`
//    reload onwards, e.g. `mov edx,[g_game]` where the exe has
//    `mov eax,[g_game]`). Throwaway loads in a scratch copy did not shift it.
//  - Case 21's unit loop addressing: the exe is `cmp [ecx+esi+0x1b63],ebx`
//    (base g_game, index i) and ours `cmp [esi+ecx+0x1b63],ebx` (base i,
//    index g_game). Standalone reproductions of the loop give base=i for every
//    operand spelling tested, so the exe's base choice comes from the
//    surrounding function. This is the same unfixable SIB swap noted at
//    0x441460.
//  - Case 20's `if (bf->b1)` bit test: the exe uses `mov dl,[m]; shr dl,1;
//    test dl,1` and ours `mov cl,...` (scratch register choice).
//
// What fixed most of the score this session (50.9 -> 81.9):
//  - The live ebp and the `mov $0x10,%ebx` constant promotions all came from
//    one root cause: MSVC value-numbers equal constants, and once a constant
//    has two uses in one basic block (case 2's inner 0 had `|= 0x10` and
//    `= 0x10`) it materialises it in a register and CSEs that register into
//    every later block using the same constant. The memcmp chains then hoisted
//    0x10 into ebx, which forced the first cmpsb's zero into ebp (push ebp in
//    all 20 epilogues). Writing the flag words as 1-bit unsigned short
//    bitfields (Bits_00426e80) makes their masks codegen constants, so the
//    promotion disappears. Probe files: build/scratch/0x426e80/v5.cpp, v6.cpp.
//    Bitfield forms also give the wanted codegen: `or byte [m],K` for a set,
//    `and word [m],~K` for a clear, and `mov reg,[m]; shr; test` for a
//    standalone `if (bf)`.
//  - Case 10 and case 20 dispatch as `switch` (the exe's `dec ecx; je` /
//    `sub ecx,2; jne` chains), not if/else.
//  - The FUN_004256d0(0x55d/0x55e) and FUN_00450d80 branch polarity: the exe
//    lays the `!= 0` arm out first (`jne` to the other arm).
//  - The 0x2ba2 GUID copy is a struct assignment
//    (`*(V4i*)(g_game + 0x2ba2) = DAT_004fdaf0;` with DAT_004fdaf0 declared
//    V4i), which gives the interleaved dword load/store and the
//    `add edx,0x2ba2` base fold.
//  - The DAT_00511fb8 checks are `strlen(DAT_00511fb8) != 0` (the
//    `repne scasb` idiom), and the FUN_004abd90 message calls nest
//    FUN_004a5030 inside the argument list so the 1,1 pushes are hoisted.
//  - FUN_00451220's first parameter is unsigned char (0x451220.cpp) and
//    g_game[0x2a42] is zero-extended with `(unsigned char)` wherever it feeds
//    an int (the exe's `xor edx,edx; mov dl,[eax+0x2a42]` idiom).
//  - Case body order inside the inner switches follows the exe's emission
//    order: case 2's inner 0,6,5,7,9,8,1; case 7's inner 0,1,10,11,13,3,14;
//    case 8's inner 15,16,3,1; case 15's inner 0,13,2,3,1.
//  - `goto tail7` (a shared `g_game[0x2bbe]=7; FUN_004256d0(0x9b,...); ...`
//    block at the end of the function) merges the 0x421/0x516/0x4b1 calls the
//    way the exe's 0x427762 does, and brings the size to exactly 6310.
//    A blanket `goto tailStores` for every 0x9b tail is much worse (64.8%).
//
// Older notes (previous sessions):
//  - the four missing bodies were added; after the outer 0x10 dispatch the exe
//    emits its inner cases in the order 0,1,17,19,18,20,21,3, so 17 is the
//    FUN_00451540 body (0x427eaf), 18 is the FUN_004517b0 body (0x427f4c), 19
//    is the `|= 0x40` fall-into-18 body (0x427f25), 20 is the FUN_004c69a0 /
//    FUN_004c2470 / FUN_004c2870 / FUN_004c63a0 group plus `return` (0x428094)
//    and 21 is the 0x4e5 / 0x97-toggle / FUN_00435a20(g_game+0x2ab1) / 0xcee
//    unit loop / 0x5ea message body (0x4280b9). The body previously kept as
//    case 20 here belonged to the 0x11 switch: it is its value 3 (0x428478).
//    Outer 0x11 emits 0,1,17,3, so case 17 is the 10-iteration
//    Class_00463c60::FUN_00463c60 loop over the 0x14b players (0x428439) and
//    case 3 is that FUN_004c9f90 group (0x428478), whose tail shares 0x42857f
//    where the exe pushes ebx (0x11 in this switch, 0 in the 0x10 case-21).
//  - the mode check must read `mode != g_game[0x2bbf]` (mode on the left) so
//    MSVC emits the original `cmp bl, cl` at 0x426e98.
//  - case 16's compare chain is one `if (memcmp(a,STR1,0x10) == 0 ||
//    memcmp(a,STR2,0x10) == 0) { ... }` (single shared body), which matches the
//    original's control flow (compare1's `je` jumps straight to the shared
//    body).
//  - every sprintf(local_100, DAT_00502f9c, <line>) needs its 4th argument:
//    DAT_00502f9c is "Code segment checksum error found when switching FE
//    states. \nState change called from [line %d, file %s]", so the original
//    pushes the line and then DAT_00503004 and cleans 0x10.
//  - the outer case bodies must be laid out in the exe's order, which the jump
//    table at 0x42859c reveals: 0, 2, 1, 3, 4, 5, 7, 10, 8, 9, 11..14, 15, 20,
//    16, 17 (case 2 sits between case 0 and case 1, and 10 comes before 8).
#include <string.h>
#include <stdio.h>

// GLOBAL: 0x511de8
extern char* g_game;

// GLOBAL: 0x511fb8
extern char DAT_00511fb8[];

// GLOBAL: 0x512c80
extern int DAT_00512c80;

// GLOBAL: 0x503004
extern char DAT_00503004[];

// GLOBAL: 0x50329c
extern char DAT_0050329c[];

// GLOBAL: 0x503294
extern char DAT_00503294[];

// GLOBAL: 0x50328c
extern char DAT_0050328c[];

// GLOBAL: 0x503284
extern char DAT_00503284[];

// GLOBAL: 0x50327c
extern char DAT_0050327c[];

// GLOBAL: 0x50324c
extern char DAT_0050324c[];

// GLOBAL: 0x502f9c
extern char DAT_00502f9c[];

// GLOBAL: 0x4fcdc8
extern char DAT_004fcdc8[];

// GLOBAL: 0x4fcdb8
extern char DAT_004fcdb8[];

// GLOBAL: 0x4fcda8
extern char DAT_004fcda8[];

// GLOBAL: 0x4fdaf0
struct V4i { int a; int b; int c; int d; };
extern V4i DAT_004fdaf0;

// The flag words in g_game (0x2a44, 0x2aaf, 0x2bee, 0x2b4c) are 16-bit
// bitfields: setting a bit is a straight `or byte [m], mask`, clearing one is
// `and word [m], ~mask`, and a standalone `if (bit)` is a `shr/test` pair.
struct Bits_00426e80 {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short b4 : 1;
    unsigned short : 11;
};

// The object returned by FUN_004b6220: its +0xf0 byte holds two 1-bit flags.
struct Obj_00426e80 {
    char unknown_0[0xf0];
    unsigned short bit0 : 1;
    unsigned short flag : 1;           // +0xf0, mask 2
};

// The 0xb9-byte player data block pointed at by g_game+0x1b8a+0x14b*n.
#pragma pack(push, 1)
struct Pd_00426e80 {
    char unknown_0[0x97];
    unsigned short ready : 1;          // +0x97
    unsigned short rest_97 : 15;       // +0x97
    char unknown_99[2];                // +0x99
    unsigned short : 6;                // +0x9b
    unsigned short b6 : 1;             // +0x9b, mask 0x40
    unsigned short : 9;
};
#pragma pack(pop)

// Callees
void __stdcall FUN_004256d0(int line, char* file);
void __stdcall FUN_004257e0(char state, int line, char* file);
void __stdcall FUN_00425860(char state, int line, char* file);
int FUN_00428bc0(void);
void __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);
void __stdcall FUN_004c22d0(int param);
void __stdcall FUN_00434ab0(int param);
void __stdcall FUN_00426780(char* param);
void __stdcall FUN_00478240(int param);
void __stdcall FUN_00490b30(int param);
void __stdcall FUN_0041d9f0(int param);
int __stdcall FUN_00443ff0(int param);
int __stdcall FUN_00451220(unsigned char playerIndex, int param2);
int __stdcall FUN_004517b0(V4i v, int idx);
int __stdcall FUN_004a5030(char* param);
void __stdcall FUN_00450a10(int param);
void __stdcall FUN_004c69a0(int param);
void __stdcall FUN_004c6890(int param1, int param2);
void __stdcall FUN_004c63a0(void);
void __stdcall FUN_004b6230(int param);
void __stdcall FUN_004c9790(int param);
void __stdcall FUN_004a9660(int param);
void __stdcall FUN_004ab0a0(int param);
void __stdcall FUN_004c9f90(int param);
void __stdcall FUN_00461020(int param1, int param2);
void __stdcall FUN_004c1ab0(void);
Obj_00426e80* __stdcall FUN_004b6220(void);
void __stdcall FUN_004263b0(void);
void __stdcall FUN_00430f00(void);
int __stdcall FUN_00457710(void);
void __stdcall FUN_004644d0(void);
void __stdcall FUN_004777a0(void);
void __stdcall FUN_0042f9a0(void);
void __stdcall FUN_00478e80(void);
void __stdcall FUN_0047bbb0(void);
void __stdcall FUN_00444580(void);
void __stdcall FUN_00443100(void);
void __stdcall FUN_00442560(void);
void __stdcall FUN_004421f0(void);
void __stdcall FUN_00443cb0(void);
void __stdcall FUN_00450dd0(void);
void __stdcall FUN_00451540(void);
int __stdcall FUN_00450d80(void);
void __stdcall FUN_00450e20(void);
void __stdcall FUN_00450f90(void);
void __stdcall FUN_00460160(void);
void __stdcall FUN_0046ca60(void);
void __stdcall FUN_00449bb0(void);
void __stdcall FUN_0044a680(void);
int __stdcall FUN_00441bc0(void);
void __stdcall FUN_0046c620(int param);
void __stdcall FUN_0046c920(void);
void __stdcall FUN_0046c190(void);
void __stdcall FUN_00491a70(void);
int __stdcall FUN_004436e0(void);
void __stdcall FUN_0041f630(void);
void __stdcall FUN_004c2470(void);
void __stdcall FUN_004c2870(void);
struct Class_00463c60 { void FUN_00463c60(int param); };
struct Class_00435a20 { void FUN_00435a20(int param); };

// FUNCTION: 0x426e80
void __stdcall FUN_00426e80(void)
{
    char local_100[256];
    char local_10[16];

    {
        char mode = g_game[0x2bc0];
        if (mode != g_game[0x2bbf]) {
            FUN_004256d0(0xa3, DAT_00503004);
            g_game[0x2bbf] = mode;
            g_game[0x2bc0] = mode;
        }
    }

    switch ((unsigned char)g_game[0x2bbe]) {
    case 0:
    {
        Obj_00426e80* p = FUN_004b6220();
        FUN_004c22d0(0);
        if (p->flag) {
            if (*(int*)(g_game + 0x3923d) != 0) {
                FUN_00426780(DAT_0050329c);
                FUN_004256d0(0x3dc, DAT_00503004);
                g_game[0x2bbe] = 1;
                FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                *(int*)(g_game + 0x3923d) = 0;
                FUN_00430f00();
                return;
            }
            if (*(int*)(g_game + 0x39245) == 0) {
                FUN_00426780(DAT_0050329c);
                FUN_004256d0(0x3e6, DAT_00503004);
            } else {
                FUN_004256d0(0x3e9, DAT_00503004);
            }
        } else {
            FUN_004256d0(0x3ed, DAT_00503004);
        }
        g_game[0x2bbe] = 2;
        FUN_004256d0(0x9b, DAT_00503004);
        g_game[0x2bbf] = 0;
        g_game[0x2bc0] = 0;
        return;
    }

    case 2:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            if (FUN_00457710()) {
                ((Bits_00426e80*)(g_game + 0x2a44))->b0 = 1;
                ((Bits_00426e80*)(g_game + 0x2bee))->b4 = 1;
                FUN_004256d0(0x403, DAT_00503004);
                g_game[0x2bbe] = 0x10;
                FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x404, DAT_00503004);
                g_game[0x2bbf] = 0x12;
                g_game[0x2bc0] = 0x12;
                FUN_004c22d0(1);
                return;
            }
            FUN_00434ab0(0);
            ((Bits_00426e80*)(g_game + 0x2bee))->b4 = 0;
            FUN_004263b0();
            if (DAT_00512c80 == 0) {
                FUN_004256d0(0x40d, DAT_00503004);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                FUN_004c22d0(1);
                return;
            }
            FUN_004256d0(0x40f, DAT_00503004);
            g_game[0x2bbf] = 6;
            g_game[0x2bc0] = 6;
            FUN_004c22d0(1);
            return;
        case 6:
            FUN_00434ab0(3);
            ((Bits_00426e80*)(g_game + 0x2a44))->b3 = 0;
            FUN_004256d0(0x41c, DAT_00503004);
            g_game[0x2bbe] = 0xf;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 5:
            ((Bits_00426e80*)(g_game + 0x2a44))->b3 = 1;
            FUN_004256d0(0x421, DAT_00503004);
            goto tail7;
        case 7:
            FUN_004256d0(0x425, DAT_00503004);
            g_game[0x2bbe] = 0;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 9:
            FUN_004256d0(0x429, DAT_00503004);
            g_game[0x2bbe] = 3;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 8:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c6890(0, 0);
            FUN_004c63a0();
            FUN_004b6230(0);
            return;
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        }
        return;

    case 1:
        FUN_00426780(DAT_00503294);
        FUN_004256d0(0x437, DAT_00503004);
        g_game[0x2bbe] = 2;
        FUN_004256d0(0x9b, DAT_00503004);
        g_game[0x2bbf] = 0;
        g_game[0x2bc0] = 0;
        return;

    case 3:
        FUN_00426780(DAT_0050328c);
        FUN_004256d0(0x43c, DAT_00503004);
        g_game[0x2bbe] = 2;
        FUN_004256d0(0x9b, DAT_00503004);
        g_game[0x2bbf] = 0;
        g_game[0x2bc0] = 0;
        return;

    case 4:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            FUN_004256d0(0x443, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 1:
            FUN_00426780(DAT_00503284);
            FUN_00426780(DAT_0050328c);
            ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 0;
            FUN_004256d0(0x44a, DAT_00503004);
            g_game[0x2bbe] = 2;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_00490b30(2);
            return;
        }
        break;

    case 5:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            FUN_004256d0(0x454, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 1:
            FUN_00426780(DAT_0050327c);
            FUN_00426780(DAT_0050328c);
            ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 0;
            FUN_004256d0(0x45b, DAT_00503004);
            g_game[0x2bbe] = 2;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_00490b30(2);
            return;
        }
        break;

    case 7:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            FUN_004777a0();
            FUN_004644d0();
            FUN_004256d0(0x47c, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        case 10:
            FUN_00478240(1);
            FUN_004256d0(0x485, DAT_00503004);
            g_game[0x2bbe] = 8;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_004256d0(0x486, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 11:
            FUN_0042f9a0();
            FUN_00434ab0(2);
            FUN_004256d0(0x493, DAT_00503004);
            g_game[0x2bbe] = 9;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 13:
            FUN_004256d0(0x497, DAT_00503004);
            g_game[0x2bbe] = 0xa;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_00460160();
            FUN_004256d0(0x499, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 3:
            FUN_004256d0(0x49d, DAT_00503004);
            g_game[0x2bbe] = 2;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 14:
            FUN_00478240(1);
            FUN_004256d0(0x4a2, DAT_00503004);
            g_game[0x2bbe] = 8;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_004256d0(0x4a3, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        }
        break;

    case 10:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        case 3:
            FUN_004256d0(0x4b1, DAT_00503004);
            g_game[0x2bbe] = 7;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        }
        break;

    case 8:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 15:
            FUN_00434ab0(1);
            FUN_004256d0(0x4c2, DAT_00503004);
            g_game[0x2bbe] = 0xb;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 16:
            FUN_00434ab0(1);
            FUN_004256d0(0x4c7, DAT_00503004);
            g_game[0x2bbe] = 0xc;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 3:
            FUN_004256d0(0x4cb, DAT_00503004);
            g_game[0x2bbe] = 7;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        }
        return;

    case 9:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            FUN_0047bbb0();
            FUN_004256d0(0x4d9, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        case 2:
            ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 1;
            return;
        case 3:
            FUN_004256d0(0x4e5, DAT_00503004);
            g_game[0x2bbe] = 7;
            break;
        }
        FUN_004256d0(0x9b, DAT_00503004);
        g_game[0x2bbf] = 0;
        g_game[0x2bc0] = 0;
        return;

    case 11:
    case 12:
    case 13:
    case 14:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            FUN_00478e80();
            FUN_004256d0(0x4f5, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        case 2:
            ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 1;
            return;
        case 3:
            switch ((unsigned char)g_game[0x2bbe]) {
            case 0xb:
                FUN_00478240(0);
                FUN_004256d0(0x505, DAT_00503004);
                g_game[0x2bbe] = 8;
                FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x506, DAT_00503004);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                return;
            case 0xc:
                FUN_00478240(1);
                FUN_004256d0(0x50a, DAT_00503004);
                g_game[0x2bbe] = 8;
                FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x50b, DAT_00503004);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                return;
            case 0xd:
                FUN_004c2470();
                FUN_0041f630();
                FUN_00490b30(7);
                FUN_0041d9f0(7);
                FUN_004c2870();
                return;
            case 0xe:
                FUN_00490b30(2);
                FUN_004256d0(0x516, DAT_00503004);
                goto tail7;
            }
            break;
        }
        break;

    case 15:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            ((Bits_00426e80*)(g_game + 0x2aaf))->b1 = 0;
            ((Bits_00426e80*)(g_game + 0x2aaf))->b0 = 0;
            if (FUN_00457710()) {
                ((Bits_00426e80*)(g_game + 0x2a44))->b0 = 1;
                FUN_004256d0(0x52d, DAT_00503004);
                g_game[0x2bbe] = 0x10;
                FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x52e, DAT_00503004);
                g_game[0x2bbf] = 0x12;
                g_game[0x2bc0] = 0x12;
                return;
            }
            FUN_00444580();
            if (FUN_00443ff0(-1)) {
                FUN_004256d0(0x534, DAT_00503004);
                g_game[0x2bbf] = 2;
                g_game[0x2bc0] = 2;
                return;
            }
            FUN_004256d0(0x536, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 13:
            FUN_004256d0(0x53b, DAT_00503004);
            g_game[0x2bbe] = 0xa;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_00460160();
            FUN_004256d0(0x53d, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 2:
            if (memcmp(g_game + 0x39201, DAT_004fcdc8, 0x10) == 0) {
                FUN_004256d0(0x547, DAT_00503004);
                g_game[0x2bbe] = 0x14;
                FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x548, DAT_00503004);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                FUN_00443100();
                return;
            }
            if (memcmp(g_game + 0x39201, DAT_004fcdb8, 0x10) == 0) {
                FUN_004256d0(0x54e, DAT_00503004);
                g_game[0x2bbe] = 0x14;
                FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x54f, DAT_00503004);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                FUN_00442560();
                return;
            }
            if (memcmp(g_game + 0x39201, DAT_004fcda8, 0x10) == 0) {
                FUN_004256d0(0x555, DAT_00503004);
                g_game[0x2bbe] = 0x14;
                FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x556, DAT_00503004);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                FUN_004421f0();
                return;
            }
            if (FUN_00450d80() != 0) {
                ((Bits_00426e80*)(g_game + 0x2a44))->b0 = 1;
                FUN_004256d0(0x55d, DAT_00503004);
                g_game[0x2bbe] = 0x10;
                FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x55e, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                return;
            }
            strncpy(DAT_00511fb8, DAT_0050324c, 0xf9);
            FUN_004256d0(0x3bc, DAT_00503004);
            g_game[0x2bbe] = 0xf;
            FUN_004257e0(0, 0x9b, DAT_00503004);
            FUN_004257e0(0, 0x3bd, DAT_00503004);
            return;
        case 3:
            FUN_004256d0(0x564, DAT_00503004);
            g_game[0x2bbe] = 2;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        }
        break;

    case 20:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 1:
            if (((Bits_00426e80*)(g_game + 0x2aaf))->b1) {
                if (FUN_00450d80() != 0) {
                    ((Bits_00426e80*)(g_game + 0x2a44))->b0 = 1;
                    FUN_004256d0(0x571, DAT_00503004);
                    g_game[0x2bbe] = 0x10;
                    FUN_004257e0(0, 0x9b, DAT_00503004);
                    FUN_004256d0(0x572, DAT_00503004);
                } else {
                    strncpy(DAT_00511fb8, DAT_0050324c, 0xf9);
                    FUN_00425860(0xf, 0x3bc, DAT_00503004);
                    FUN_004257e0(0, 0x3bd, DAT_00503004);
                    ((Bits_00426e80*)(g_game + 0x2aaf))->b0 = 0;
                    ((Bits_00426e80*)(g_game + 0x2aaf))->b1 = 0;
                    FUN_004256d0(0x577, DAT_00503004);
                    g_game[0x2bbe] = 0xf;
                    FUN_004257e0(0, 0x9b, DAT_00503004);
                    FUN_004256d0(0x578, DAT_00503004);
                }
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
            }
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        }
        break;

    case 16:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            FUN_004c9790(1);
            FUN_004256d0(0x587, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            FUN_004644d0();
            {
                char* p = g_game + 0x39201;
                if (memcmp(p, DAT_004fcdc8, 0x10) != 0) {
                    if (memcmp(p, DAT_004fcdb8, 0x10) != 0) goto after_key;
                }
                if (g_game[0x2aaf] & 1) {
                    FUN_004256d0(0x58f, DAT_00503004);
                    g_game[0x2bbf] = 0x11;
                    g_game[0x2bc0] = 0x11;
                    return;
                }
                *(V4i*)(g_game + 0x2ba2) = DAT_004fdaf0;
            }
        after_key:
            FUN_00443cb0();
            if (strlen(DAT_00511fb8) != 0) {
                FUN_004abd90(g_game + 0x519, DAT_00511fb8, FUN_004a5030(DAT_00511fb8) + 0x14, 1, 1);
                DAT_00511fb8[0] = 0;
                return;
            }
            break;
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            if (strlen(DAT_00511fb8) != 0) {
                FUN_004abd90(g_game + 0x519, DAT_00511fb8, FUN_004a5030(DAT_00511fb8) + 0x14, 1, 1);
                DAT_00511fb8[0] = 0;
                return;
            }
            break;
        case 17:
            FUN_00451540();
            if (FUN_00451220(g_game[0x2a42], 1)) {
                FUN_00450a10(*(int*)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b67));
            }
            FUN_004256d0(0x5ab, DAT_00503004);
            g_game[0x2bbe] = 0x11;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 19:
            ((Pd_00426e80*)(*(int*)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b8a)))->b6 = 1;
        case 18:
            if (FUN_004517b0(*(V4i*)(g_game + 0x2ba2), (unsigned char)g_game[0x2a42]) == 0) {
                FUN_004256d0(0x5b3, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                return;
            }
            if (g_game[0x2bbf] == 0x12) {
                FUN_004c69a0(*(int*)(g_game + 0x37e1b));
                FUN_004c6890(0, 0);
                FUN_004c63a0();
                FUN_00491a70();
                if (FUN_004436e0() != 0) {
                    FUN_004ab0a0((int)(g_game + 0x519));
                    FUN_004256d0(0x5c1, DAT_00503004);
                    g_game[0x2bbf] = 0x14;
                    g_game[0x2bc0] = 0x14;
                    return;
                }
                FUN_004256d0(0x5c4, DAT_00503004);
                g_game[0x2bbf] = 0x15;
                g_game[0x2bc0] = 0x15;
                return;
            }
            FUN_004256d0(0x5c7, DAT_00503004);
            g_game[0x2bbf] = 0x15;
            g_game[0x2bc0] = 0x15;
            return;
        case 20:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        case 21:
        {
            int unit = *(int*)(g_game + 0x4e5);
            if (unit != 0) {
                ((Pd_00426e80*)(*(int*)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b8a)))->ready =
                    *(unsigned int*)(unit + 4) >> 1;
            } else {
                ((Pd_00426e80*)(*(int*)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b8a)))->ready = 0;
            }
            if (g_game[0x2bbf] == 0x13) {
                ((Pd_00426e80*)(*(int*)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b8a)))->b6 = 1;
            }
            {
                Bits_00426e80* f = (Bits_00426e80*)(g_game + 0x2b4c);
                unsigned char* q = (unsigned char*)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b84);
                *q = (*q & 0xfd) | (f->b4 << 1);
                if (((Bits_00426e80*)(g_game + 0x2b4c))->b4) {
                    ((Class_00435a20*)*(int*)(g_game + 0x391e9))->FUN_00435a20((int)(g_game + 0x2ab1));
                    {
                        int i;
                        for (i = 0; i < 0xcee; i += 0x14b) {
                            if (*(int*)(g_game + i + 0x1b63) != 0) {
                                char t = g_game[i + 0x1bd6];
                                if (t == 1 || t == 2) {
                                    FUN_00450a10(*(int*)(g_game + i + 0x1b67));
                                }
                            }
                        }
                    }
                    ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 1;
                    return;
                }
            }
            if (FUN_00428bc0()) {
                sprintf(local_100, DAT_00502f9c, 0x5ea, DAT_00503004);
                FUN_004abd90(g_game + 0x519, local_100, 500, 1, 1);
            }
            g_game[0x2bbe] = 0x11;
            FUN_004257e0(0, 0x9b, DAT_00503004);
            return;
        }
        case 3:
            FUN_00450dd0();
            if (FUN_00428bc0()) {
                sprintf(local_100, DAT_00502f9c, 0x5f0, DAT_00503004);
                FUN_004abd90(g_game + 0x519, local_100, 500, 1, 1);
            }
            g_game[0x2bbe] = 0xf;
            FUN_004257e0(0, 0x9b, DAT_00503004);
            return;
        }
        break;

    case 17:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            if (!(g_game[0x2a44] & 4)) {
                FUN_00449bb0();
                FUN_0046c620(1);
                FUN_0046c620(2);
                FUN_004256d0(0x5ff, DAT_00503004);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                FUN_00450f90();
                return;
            }
            FUN_0046ca60();
            FUN_004256d0(0x605, DAT_00503004);
            g_game[0x2bbf] = 0x11;
            g_game[0x2bc0] = 0x11;
            FUN_00450f90();
            return;
        case 1:
            FUN_0044a680();
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            if (((Bits_00426e80*)(g_game + 0x2a44))->b2) {
                FUN_0046ca60();
                FUN_004a9660((int)(g_game + 0x519));
                if (FUN_00428bc0()) {
                    sprintf(local_100, DAT_00502f9c, 0x613, DAT_00503004);
                    FUN_004abd90(g_game + 0x519, local_100, 500, 1, 1);
                }
                g_game[0x2bbf] = 0x11;
                g_game[0x2bc0] = 0x11;
                return;
            }
            break;
        case 17:
            {
                char* p = g_game + 0x1b63;
                int i;
                for (i = 0; i < 10; i++) {
                    if (p[0x73] == 4) {
                        ((Class_00463c60*)p)->FUN_00463c60(0);
                    }
                    p += 0x14b;
                }
            }
            FUN_0046ca60();
            ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 1;
            return;
        case 3:
            ((Bits_00426e80*)(g_game + 0x2a44))->b0 = 1;
            FUN_004c9f90((int)(g_game + 0x14));
            FUN_00461020(2, 100);
            FUN_0046c920();
            FUN_0046c620(8);
            FUN_0046c190();
            if (((Bits_00426e80*)(g_game + 0x2bee))->b4) {
                FUN_00450e20();
                return;
            }
            {
                int r = FUN_00441bc0();
                if (r == 0 || r == 3) {
                    if (FUN_00428bc0()) {
                        sprintf(local_100, DAT_00502f9c, 0x640, DAT_00503004);
                        FUN_004abd90(g_game + 0x519, local_100, 500, 1, 1);
                    }
                    g_game[0x2bbe] = 0xf;
                } else {
                    if (FUN_00428bc0()) {
                        sprintf(local_100, DAT_00502f9c, 0x643, DAT_00503004);
                        FUN_004abd90(g_game + 0x519, local_100, 500, 1, 1);
                    }
                    g_game[0x2bbe] = 0x10;
                }
            }
            FUN_004257e0(0, 0x9b, DAT_00503004);
            return;
        }
        break;

    }
    return;
tail7:
    g_game[0x2bbe] = 7;
    FUN_004256d0(0x9b, DAT_00503004);
    g_game[0x2bbf] = 0;
    g_game[0x2bc0] = 0;
    return;
}
