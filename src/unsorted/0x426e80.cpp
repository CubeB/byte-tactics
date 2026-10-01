// Decompiled by longcat-2.5-preview-free, edited and finished by deepseek-v4.1-flash. Names are provisional.
// tools/permute.py pass (6170 candidates, 17 min): 86.8 -> 86.9 via
// temp_intro + include + do_while0 + zero_compare + strip_parens. Output
// tidied before adoption: dead same0 removed, tmp0/tmp1 renamed game1/game2,
// __stdcall restored, all re-checked at 86.9.
// Best 86.8% (6310 of 6310 bytes, exact size). What still differs:
//  - Outer 15's case 2 FUN_00450d80 if/else rotation is one step off ours
//    (ours: or-reload eax, 0x2bbe-reload edx; exe: eax, eax). The same +1
//    carries to the FUN_004257e0 site (ours ecx, exe edx), case 20's bit test
//    (ours cl, exe dl) and the 0x571 body (ours edx, exe eax). Because the
//    0x55e body's stores land on our edx/eax pair they merge with case 0's
//    end tail (the exe keeps three store-tail copies), which also causes the
//    missing T1 copy after the 9i3 body and the case-0-end call/jmp merge
//    diff. An isolated probe of outer 15's inner switch (build/scratch/
//    0x426e80/p15.cpp) gives eax at the 0x55d body, so the offset comes from
//    the outer cases 0..14 before it; every temp-count shift tried (extra
//    stores in cases 13/case 2/after, chained assignments, the char one=1
//    materialisation, reordered inner cases, headers) left it at edx.
//  - Case 21's loop SIB base/index swap (ours [esi+ecx+..], exe [ecx+esi+..]).
//    Same unfixable SIB swap as 0x441460.
//  - Remaining jump-table entries follow from the block layout.
//
// What moved the score this pass (81.9 -> 86.8): the register rotation is a
// global scratch-register cycle [eax, ecx, edx] and the case label ORDER in
// each inner switch decides where each case body's temps land in it.
//  - case 2's inner order 0,1,6,5,7,9,8 (case 1 the FUN_004c69a0 body second)
//    fixed T15 at case 2's inner case 6: ours eax and the arg pushes inline,
//    which also fixed the 0x421/0x4b1/0x516 tail7 grouping (82.6%).
//  - case 8's inner order 1,15,16,3 (case 1 first) fixed the 0xb/0xc sites
//    (83.0%).
//  - case 9's inner case 3 written out in full instead of break to a shared
//    tail made cases 8i3/9i3 cross-jump at their 0x4cb/0x4e5 call like the
//    exe, giving the exact 6310 size (86.8%).
#include <math.h>
#include <string.h>
#include <stdlib.h>
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
    char mode, local_100[256], t;
    unsigned char* q;

    {
        mode = g_game[0x2bc0];
        if (mode != g_game[0x2bbf]) {
            FUN_004256d0(0xa3, DAT_00503004);
            g_game[0x2bbf] = mode;
            g_game[0x2bc0] = mode;
        }
    }

    switch (((unsigned char)g_game[0x2bbe])) {
    case 0:
    {
        Obj_00426e80* p = FUN_004b6220();
        FUN_004c22d0(0);
        if (p->flag) {
            if ((*(int*)(g_game + 0x3923d))) {
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
            if (((int)(*((int*)(g_game + 0x39245)) == 0))) { FUN_00426780(DAT_0050329c); FUN_004256d0(0x3e6, DAT_00503004); } else FUN_004256d0(0x3e9, DAT_00503004);
        } else FUN_004256d0(0x3ed, DAT_00503004);
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
            if ((0 != DAT_00512c80) == 0) {
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
        case 1:
            FUN_004c69a0(*(int*)(0x37e1b + g_game));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
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
        g_game[0x2bbf] = 0; g_game[0x2bc0] = 0;
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
            FUN_004777a0(); FUN_004644d0();
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
            FUN_00460160(); FUN_004256d0(0x499, DAT_00503004); g_game[0x2bbf] = 1;
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
            FUN_004256d0(0x4b1, DAT_00503004); g_game[0x2bbe] = 7; FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        }
        break;

    case 8:
        FUN_004c1ab0();
            switch ((unsigned char)g_game[0x2bbf]) {
            case 1:
                FUN_004c69a0(*(int*)(0x37e1b + g_game));
                FUN_004c2470();
                FUN_004c2870();
                FUN_004c63a0();
                return;
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
                FUN_004c69a0(*(int*)(0x37e1b + g_game)); FUN_004c2470();
                FUN_004c2870();
                FUN_004c63a0();
                return;
            case 2:
                ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 1;
                return;
            case 3:
                FUN_004256d0(0x4e5, DAT_00503004);
                g_game[0x2bbe] = 7;
                FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                return;
            }
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
            FUN_004c69a0(*(int*)(0x37e1b + g_game));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        case 2:
            ((Bits_00426e80*)(0x2a44 + g_game))->b2 = 1;
            return;
        case 3:
            switch ((unsigned char)g_game[0x2bbe]) {
            case 0xb:
                FUN_00478240(0);
                FUN_004256d0(0x505, DAT_00503004);
                g_game[0x2bbe] = 8;
                FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0; FUN_004256d0(0x506, DAT_00503004);
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
                FUN_0041f630(); FUN_00490b30(7); FUN_0041d9f0(7);
                FUN_004c2870();
                do return; while (0);
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
            if (FUN_00457710() != 0) {
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
                FUN_004256d0(0x547, DAT_00503004); g_game[0x2bbe] = 0x14; FUN_004256d0(0x9b, DAT_00503004);
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
            if (!(memcmp(g_game + 0x39201, DAT_004fcda8, 0x10) != 0)) {
                FUN_004256d0(0x555, DAT_00503004); g_game[0x2bbe] = 0x14; FUN_004256d0(0x9b, DAT_00503004);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x556, DAT_00503004);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                FUN_004421f0();
                return;
            }
            if (0 != FUN_00450d80()) {
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
            FUN_004c69a0(*(int*)(0x37e1b + g_game));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        }
        break;

    case 20:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 1:
            if (((Bits_00426e80*)(g_game + 0x2aaf))->b1) { if (FUN_00450d80() != 0) { ((Bits_00426e80*)(g_game + 0x2a44))->b0 = 1; FUN_004256d0(0x571, DAT_00503004); g_game[0x2bbe] = 0x10; FUN_004257e0(0, 0x9b, DAT_00503004); FUN_004256d0(0x572, DAT_00503004); } else { strncpy(DAT_00511fb8, DAT_0050324c, 0xf9); FUN_00425860(0xf, 0x3bc, DAT_00503004); FUN_004257e0(0, 0x3bd, DAT_00503004); ((Bits_00426e80*)(0x2aaf + g_game))->b0 = 0; ((Bits_00426e80*)(g_game + 0x2aaf))->b1 = 0; FUN_004256d0(0x577, DAT_00503004); g_game[0x2bbe] = 0xf; FUN_004257e0(0, 0x9b, DAT_00503004); FUN_004256d0(0x578, DAT_00503004); } g_game[0x2bbf] = 0; g_game[0x2bc0] = 0; }
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
                if (memcmp((char*)p, DAT_004fcdc8, 0x10) != 0) {
                    if (memcmp(p, DAT_004fcdb8, 0x10) != 0) goto after_key;
                }
                if ((g_game[0x2aaf] & 1)) {
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
                FUN_004abd90(0x519 + g_game, DAT_00511fb8, FUN_004a5030(DAT_00511fb8) + 0x14, 1, 1);
                DAT_00511fb8[0] = 0;
                return;
            }
            break;
        case 17:
            FUN_00451540();
            if (FUN_00451220(g_game[0x2a42], 1)) FUN_00450a10(*(int*)(0x1b67 + (0x14b * (unsigned char)g_game[0x2a42] + g_game)));
            FUN_004256d0(0x5ab, DAT_00503004);
            g_game[0x2bbe] = 0x11;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 19:
            ((Pd_00426e80*)(*(int*)(0x14b * (unsigned char)g_game[0x2a42] + g_game + 0x1b8a)))->b6 = 1;
        case 18:
            if (FUN_004517b0(*(V4i*)(g_game + 0x2ba2), (unsigned char)g_game[0x2a42]) == 0) {
                FUN_004256d0(0x5b3, DAT_00503004); g_game[0x2bbf] = 0;
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
            int unit = *((int*)(g_game + 0x4e5));
            if ((*((int*)(0x4e5 + g_game)))) ((Pd_00426e80*)(*(int*)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b8a)))->ready =
                    *(unsigned int*)(unit + 4) >> 1; else ((Pd_00426e80*)(*(int*)(0x1b8a + (g_game + (unsigned char)g_game[0x2a42] * 0x14b))))->ready = 0;
            if (g_game[0x2bbf] == 0x13) ((Pd_00426e80*)(*(int*)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b8a)))->b6 = 1;
            {
                q = (unsigned char*)(g_game + 0x14b * (unsigned char)g_game[0x2a42] + 0x1b84);
                *q = (((Bits_00426e80*)(g_game + 0x2b4c))->b4 << 1) | (*((unsigned char*)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b84)) & 0xfd);
                if (((Bits_00426e80*)(g_game + 0x2b4c))->b4) {
                    ((Class_00435a20*)*(int*)(g_game + 0x391e9))->FUN_00435a20((int)(g_game + 0x2ab1));
                    {
                        int i;
                        i = 0;
                        if (1 && i < 0xcee) do {
                                                                                        char* game1;
                                                                                        game1 = g_game;
                                                                                        do {
                                                                                            if ((*(int*)(game1 + i + 0x1b63))) {
                                                                                                char* game2;
                                                                                                game2 = g_game;
                                                                                                t = game2[i + 0x1bd6];
                                                                                                t = t;
                                                                                                t = t;
                                                                                                t = t;
                                                                                                t = t;
                                                                                                t = t;
                                                                                                t = t;
                                                                                                t = t;
                                                                                                t = t;
                                                                                                t = t;
                                                                                                t = (char)t;
                                                                                                if ((t == 1 || t == 2)) FUN_00450a10(*(int*)(0x1b67 + (i + g_game)));
                                                                                            }
                                                                                        } while (0);
                                                                                        if (!((i += 0x14b), (i < 0xcee)))
                                                                                            break;
                                                                                    } while (1);
                    }
                    ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 1;
                        return;
                }
            }
            if (FUN_00428bc0() != 0) {
                sprintf(local_100, DAT_00502f9c, 0x5ea, DAT_00503004);
                FUN_004abd90(0x519 + g_game, local_100, 500, 1, 1);
            }
            g_game[0x2bbe] = 0x11;
            FUN_004257e0(0, 0x9b, DAT_00503004);
            return;
        }
        case 3:
            FUN_00450dd0();
            if (FUN_00428bc0()) {
                sprintf(local_100, DAT_00502f9c, 0x5f0, DAT_00503004);
                FUN_004abd90(0x519 + g_game, local_100, 500, 1, 1);
            }
            g_game[0x2bbe] = 0xf;
            FUN_004257e0(0, 0x9b, DAT_00503004);
            return;
        }
        break;

    case 17:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            if (((g_game[0x2a44] & 4) != 0) == 0) {
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
                if (0 != FUN_00428bc0()) {
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
                char* p = 0x1b63 + g_game;
                int i = 0;
                if (i < 10) { do {
                    if (p[0x73] == 4) ((Class_00463c60*)p)->FUN_00463c60(0);
                    p += 0x14b;
                    i = 1 + i;
                } while (i < 10); }
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
                do return; while (0);
            }
            {
                int r = FUN_00441bc0();
                if (!(((((int)(r == 0)) || r == 3) != 0) != 0)) {
                    if (FUN_00428bc0()) {
                        sprintf(local_100, DAT_00502f9c, 0x643, DAT_00503004);
                        FUN_004abd90(g_game + 0x519, local_100, 500, 1, 1);
                    }
                    g_game[0x2bbe] = 0x10;
                } else {
                    if (FUN_00428bc0()) {
                        sprintf(local_100, DAT_00502f9c, 0x640, DAT_00503004);
                            FUN_004abd90(g_game + 0x519, local_100, 500, 1, 1);
                    }
                    g_game[0x2bbe] = 0xf;
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
