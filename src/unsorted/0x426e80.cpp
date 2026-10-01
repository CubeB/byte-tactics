// Decompiled by longcat-2.5-preview-free, edited by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// Still differs (50.2%). What changed since the previous note:
//  - the mode check must read `mode != g_game[0x2bbf]` (mode on the left) so
//    MSVC emits the original `cmp bl, cl` at 0x426e98; the reversed spelling
//    cost one mismatched line, 50.1 -> 50.2.
//  - the case-0 0xf0 flag test: `((p[0xf0] >> 1) & 1)` folds to
//    `test byte ptr [esi+0xf0], 2` no matter the spelling, including via a
//    named `unsigned char fl = p[0xf0];` local (byte-neutral, reverted), so
//    the original `mov dl, [esi+0xf0]; shr dl, 1; test dl, 1` needs the value
//    kept live for another use the source does not model yet.
//  - the outer case bodies must be laid out in the exe's order, which the jump
//    table at 0x42859c reveals: 0, 2, 1, 3, 4, 5, 7, 10, 8, 9, 11..14, 15, 20, 16,
//    17 (case 2 sits between case 0 and case 1, and 10 comes before 8).
//    Reordering the C cases that way gained 0.9%.
//  - every sprintf(local_100, DAT_00502f9c, <line>) was missing its 4th argument:
//    DAT_00502f9c is "Code segment checksum error found when switching FE states.
//    \nState change called from [line %d, file %s]", so the original pushes the
//    line and then DAT_00503004 and cleans 0x10.
//  - the inner switch values are still wrong in places. The exe's byte index
//    tables give the true case values: outer 0x10 (16) dispatches 0x2bbf through
//    byte table 0x4286e8 (22 entries; values 0,1,3,17,18,19,20,21 map to
//    0x427d44,0x427e45,0x42825c,0x427eaf,0x427f4c,0x427f25,0x428094,0x4280b9,
//    everything else to the epilogue) and outer 0x11 (17) through byte table
//    0x428714 (18 entries; values 0,1,3,17 map to 0x4282f2,0x42837f,0x428478,
//    0x428439). Ours still lacks the case 21 body of 0x10 (0x4280b9: the unit
//    +0x4e5 lookup that toggles the 0x97 word, then 0x2b4c, FUN_00435a20 on
//    g_game+0x2ab1, the 0xcee unit loop and the 0x5ea message) and the case 3 and
//    case 17 bodies of 0x11. Adding the 0x11 bodies is verified correct against
//    the disassembly (0x428478 is the FUN_004c9f90/FUN_00461020 group, 0x428439
//    the 10-iteration loop over units calling Class_00463c60::FUN_00463c60) but it
//    scores 1.3% lower in the difflib metric, so it is kept in
//    build/scratch/0x426e80/v475r.cpp instead of here.
//  - push ebp is still live in ours: the original uses only ebx/esi/edi, so the
//    extra callee-saved register is real evidence of a wrong local in one of the
//    0x2a42-scaled 0x1b8a/0x1b67 lookups.
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
extern int DAT_004fdaf0;

// GLOBAL: 0x4fdaf4
extern int DAT_004fdaf4;

// GLOBAL: 0x4fdaf8
extern int DAT_004fdaf8;

// GLOBAL: 0x4fdafc
extern int DAT_004fdafc;

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
int __stdcall FUN_00451220(int param1, int param2);
struct V4i { int a; int b; int c; int d; };
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
unsigned char* __stdcall FUN_004b6220(void);
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
void __stdcall FUN_00463c60(void* param);
void __stdcall FUN_00435a20(int param, int param2);

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
        unsigned char* p = (unsigned char*)FUN_004b6220();
        FUN_004c22d0(0);
        if (((p[0xf0] >> 1) & 1) != 0) {
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
                g_game[0x2a44] |= 1;
                g_game[0x2bee] |= 0x10;
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
            *(unsigned short*)(g_game + 0x2bee) &= 0xffef;
            FUN_004263b0();
            if (DAT_00512c80 != 0) {
                FUN_004256d0(0x40f, DAT_00503004);
                g_game[0x2bbf] = 6;
                g_game[0x2bc0] = 6;
                FUN_004c22d0(1);
                return;
            }
            FUN_004256d0(0x40d, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            FUN_004c22d0(1);
            return;
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        case 5:
            g_game[0x2a44] |= 8;
            FUN_004256d0(0x421, DAT_00503004);
            g_game[0x2bbe] = 7;
            break;
        case 6:
            FUN_00434ab0(3);
            *(unsigned short*)(g_game + 0x2a44) &= 0xfff7;
            FUN_004256d0(0x41c, DAT_00503004);
            g_game[0x2bbe] = 0xf;
            break;
        case 7:
            FUN_004256d0(0x425, DAT_00503004);
            g_game[0x2bbe] = 0;
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
        case 9:
            FUN_004256d0(0x429, DAT_00503004);
            g_game[0x2bbe] = 3;
            break;
        }
        FUN_004256d0(0x9b, DAT_00503004);
        g_game[0x2bbf] = 0;
        g_game[0x2bc0] = 0;
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
            *(unsigned short*)(g_game + 0x2a44) &= 0xfffb;
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
            *(unsigned short*)(g_game + 0x2a44) &= 0xfffb;
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
        case 3:
            FUN_004256d0(0x49d, DAT_00503004);
            g_game[0x2bbe] = 2;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 10:
            FUN_00478240(1);
            FUN_004256d0(0x485, DAT_00503004);
            g_game[0x2bbe] = 8;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            break;
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
        case 14:
            FUN_00478240(1);
            FUN_004256d0(0x4a2, DAT_00503004);
            g_game[0x2bbe] = 8;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            break;
        }
        FUN_004256d0(0x9b, DAT_00503004);
        g_game[0x2bbf] = 1;
        g_game[0x2bc0] = 1;
        return;

    case 10:
        if (g_game[0x2bbf] == 1) {
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        }
        if (g_game[0x2bbf] == 3) {
            FUN_004256d0(0x4b1, DAT_00503004);
            g_game[0x2bbe] = 7;
            break;
        }
        break;

    case 8:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        case 3:
            FUN_004256d0(0x4cb, DAT_00503004);
            g_game[0x2bbe] = 7;
            break;
        case 15:
            FUN_00434ab0(1);
            FUN_004256d0(0x4c2, DAT_00503004);
            g_game[0x2bbe] = 0xb;
            break;
        case 16:
            FUN_00434ab0(1);
            FUN_004256d0(0x4c7, DAT_00503004);
            g_game[0x2bbe] = 0xc;
            break;
        }
        FUN_004256d0(0x9b, DAT_00503004);
        g_game[0x2bbf] = 0;
        g_game[0x2bc0] = 0;
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
            g_game[0x2a44] |= 4;
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
            g_game[0x2a44] |= 4;
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
                break;
            case 0xd:
                FUN_004c2470();
                FUN_0041f630();
                FUN_00490b30(7);
                FUN_0041d9f0(7);
                FUN_004c2870();
                return;
            case 0xe:
                FUN_00490b30(2);
                break;
            }
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        }
        break;

    case 15:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            *(unsigned short*)(g_game + 0x2aaf) &= 0xfffd;
            *(unsigned short*)(g_game + 0x2aaf) &= 0xfffe;
            if (FUN_00457710()) {
                g_game[0x2a44] |= 1;
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
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
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
            if (FUN_00450d80() == 0) {
                strncpy(DAT_00511fb8, DAT_0050324c, 0xf9);
                FUN_004256d0(0x3bc, DAT_00503004);
                g_game[0x2bbe] = 0xf;
                FUN_004257e0(0, 0x3bd, DAT_00503004);
                FUN_004257e0(0, 0x3bd, DAT_00503004);
                return;
            }
            g_game[0x2a44] |= 1;
            FUN_004256d0(0x55d, DAT_00503004);
            g_game[0x2bbe] = 0x10;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_004256d0(0x55e, DAT_00503004);
            g_game[0x2bbf] = 0x11;
            g_game[0x2bc0] = 0x11;
            return;
        case 3:
            FUN_004256d0(0x564, DAT_00503004);
            g_game[0x2bbe] = 2;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
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
        }
        break;

    case 20:
        if (g_game[0x2bbf] == 1) {
            if (((g_game[0x2aaf] >> 1) & 1) != 0) {
                if (FUN_00450d80() == 0) {
                    strncpy(DAT_00511fb8, DAT_0050324c, 0xf9);
                    FUN_004256d0(0x3bc, DAT_00503004);
                    FUN_00425860(0xf, 0x3bd, DAT_00503004);
                    *(unsigned short*)(g_game + 0x2aaf) &= 0xfffe;
                    *(unsigned short*)(g_game + 0x2aaf) &= 0xfffd;
                    FUN_004256d0(0x577, DAT_00503004);
                    g_game[0x2bbe] = 0xf;
                    FUN_004257e0(0, 0x9b, DAT_00503004);
                    return;
                }
                g_game[0x2a44] |= 1;
                FUN_004256d0(0x571, DAT_00503004);
                g_game[0x2bbe] = 0x10;
                FUN_004257e0(0, 0x9b, DAT_00503004);
            }
            FUN_004256d0(0x572, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
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
            if (memcmp(g_game + 0x39201, DAT_004fcdc8, 0x10) == 0) {
                if (g_game[0x2aaf] & 1) {
                    FUN_004256d0(0x58f, DAT_00503004);
                    g_game[0x2bbf] = 0x11;
                    g_game[0x2bc0] = 0x11;
                    return;
                }
                *(int*)(g_game + 0x2ba2) = DAT_004fdaf0;
                *(int*)(g_game + 0x2ba6) = DAT_004fdaf4;
                *(int*)(g_game + 0x2baa) = DAT_004fdaf8;
                *(int*)(g_game + 0x2bae) = DAT_004fdafc;
            } else if (memcmp(g_game + 0x39201, DAT_004fcdb8, 0x10) == 0) {
                if (g_game[0x2aaf] & 1) {
                    FUN_004256d0(0x58f, DAT_00503004);
                    g_game[0x2bbf] = 0x11;
                    g_game[0x2bc0] = 0x11;
                    return;
                }
                *(int*)(g_game + 0x2ba2) = DAT_004fdaf0;
                *(int*)(g_game + 0x2ba6) = DAT_004fdaf4;
                *(int*)(g_game + 0x2baa) = DAT_004fdaf8;
                *(int*)(g_game + 0x2bae) = DAT_004fdafc;
            }
            FUN_00443cb0();
            if (DAT_00511fb8[0] != 0) {
                int len = FUN_004a5030(DAT_00511fb8);
                FUN_004abd90(g_game + 0x519, DAT_00511fb8, len + 0x14, 1, 1);
                DAT_00511fb8[0] = 0;
                return;
            }
            break;
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            if (DAT_00511fb8[0] != 0) {
                int len = FUN_004a5030(DAT_00511fb8);
                FUN_004abd90(g_game + 0x519, DAT_00511fb8, len + 0x14, 1, 1);
                DAT_00511fb8[0] = 0;
                return;
            }
            break;
        case 3:
            FUN_00450dd0();
            if (FUN_00428bc0()) {
                sprintf(local_100, DAT_00502f9c, 0x5f0, DAT_00503004);
                FUN_004abd90(g_game + 0x519, local_100, 500, 1, 1);
            }
            g_game[0x2bbe] = 0xf;
            FUN_004257e0(0, 0x9b, DAT_00503004);
            return;
        case 18:
            FUN_00451540();
            if (FUN_00451220(g_game[0x2a42], 1)) {
                FUN_00450a10(*(int*)(g_game + g_game[0x2a42] * 0x14b + 0x1b67));
            }
            FUN_004256d0(0x5ab, DAT_00503004);
            g_game[0x2bbe] = 0x11;
            FUN_004256d0(0x9b, DAT_00503004);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            return;
        case 19:
            *(char*)(*(int*)(g_game + g_game[0x2a42] * 0x14b + 0x1b8a) + 0x9b) |= 0x40;
            if (FUN_004517b0(*(V4i*)(g_game + 0x2ba2), g_game[0x2a42]) == 0) {
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

            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        case 20:
            g_game[0x2a44] |= 1;
            FUN_004c9f90((int)(g_game + 0x14));
            FUN_00461020(2, 100);
            FUN_0046c920();
            FUN_0046c620(8);
            FUN_0046c190();
            if (((g_game[0x2bee] >> 4) & 1) != 0) {
                FUN_00450e20();
                return;
            }
            if (FUN_00441bc0() == 0 || FUN_00441bc0() == 3) {
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
            FUN_004257e0(0, 0x9b, DAT_00503004);
            return;

            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            return;
        }
        break;

    case 17:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            if (g_game[0x2a44] & 4) {
                FUN_0046ca60();
                FUN_004256d0(0x605, DAT_00503004);
                g_game[0x2bbf] = 0x11;
                g_game[0x2bc0] = 0x11;
                FUN_00450f90();
                return;
            }
            FUN_00449bb0();
            FUN_0046c620(1);
            FUN_0046c620(2);
            FUN_004256d0(0x5ff, DAT_00503004);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            FUN_00450f90();
            return;
        case 1:
            FUN_0044a680();
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
            if (((g_game[0x2a44] >> 2) & 1) != 0) {
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
        }
        break;

    }
    return;
}
