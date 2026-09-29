// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL, retry round 13, 10-minute timebox. check.py: 20.1% (not MATCH).
// The opening, case 0, cases 1..5 of the outer switch, the case-2 inner switch
// and the case-0x10 checksum branch are transcribed with their real
// FUN_004256d0 line constants from the disassembly. Everything from outer
// case 6/7 (0x42731f) to case 0x14 is still a stub.
//
// Exact remaining differences (from check.py):
//   - after `cmp bl, cl` the original has `push edi`; ours has no edi use yet
//     (edi is only live in the untranscribed case 6..0x14 bodies).
//   - our outer dispatch uses esi/edx where the original keeps ecx/edx.
//   - cases 6,7,8,9,0xa,0xb..0xf,0x11..0x14 are empty, so the outer jump table
//     is 0..0x10 rather than 0..0x14 and those bodies do not match.
//
// Frame: the original allocates `sub esp, 0x100` for the single char[256]
// scratch buffer used by the FUN_00450d80 / checksum error paths (sprintf +
// FUN_004abd90), so the buffer must be live for the prologue to appear.
//
// Tail labels (from the disassembly):
//   0x427603  clear +0x2bbf/+0x2bc0 and return
//   0x4275f2  FUN_004256d0(0x9b) then 0x427603
//   0x427477  FUN_004256d0(0x49d); +0x2bbe = 2; goto 0x4275f2
//   0x4272c7  shared tail for outer cases 4/5
//   0x42789a  FUN_004256d0; +0x2bbf/+0x2bc0 = 1; return
//   0x427f88  FUN_004256d0(0x5b3); uVar9 = 0
//   0x427f8f  +0x2bbf/+0x2bc0 = uVar9; return
//   0x427f0f  FUN_004256d0; uVar9 = 0; goto 0x428418
//   0x428418  +0x2bbf/+0x2bc0 = uVar9; return
//   0x42809a  FUN_004c69a0(uVar7); FUN_004c2470(); FUN_004c2870();
//             FUN_004c63a0(); return
//   0x427cfb  same as 0x42809a
//
// Outer switch jump table 0x42859c (0x15 entries), case-2 table 0x4285f0
// (10 entries), case-7 table 0x428618 with byte index 0x428638, case-0xb
// table 0x428648 with byte index 0x42865c, case-0x10 table 0x4286c4 with
// byte index 0x4286e8, case-0x14 table 0x42868c.

#include <string.h>
#include <stdio.h>

extern char* g_game;
extern int DAT_00512c80;
extern char DAT_004fcdb8[];
extern char DAT_004fcda8[];
extern char DAT_004fcdc8[];
extern char DAT_00511fb8[];

struct Class_004b6220 {
    char unknown_0[0xf0];
    unsigned char field_f0;            // +0xf0
};

Class_004b6220* FUN_004b6220(void);
void __stdcall FUN_004256d0(int line, char* file);
void __stdcall FUN_004257e0(char state, int line, char* file);
void __stdcall FUN_00425860(char state, int line, char* file);
void __stdcall FUN_00426780(char* name);
void __stdcall FUN_004c22d0(int param);
void FUN_00430f00(void);
void FUN_004c1ab0(void);
int FUN_00457710(void);
void __stdcall FUN_00434ab0(int param);
void FUN_004263b0(void);
void __stdcall FUN_004c69a0(int param);
void FUN_004c2470(void);
void FUN_004c2870(void);
void FUN_004c63a0(void);
void __stdcall FUN_00478240(int param);
void FUN_0042f9a0(void);
void FUN_00460160(void);
void FUN_004777a0(void);
void FUN_004644d0(void);
void __stdcall FUN_004b6230(char* name);
void __stdcall FUN_004c6890(int a, int b);
void __stdcall FUN_00490b30(int param);
void FUN_0047bbb0(void);
void FUN_00478e80(void);
void __stdcall FUN_0041d9f0(int param);
void FUN_0041f630(void);
void FUN_00444580(void);
int __stdcall FUN_00443ff0(int param);
void FUN_00443100(void);
void FUN_00442560(void);
void FUN_004421f0(void);
int FUN_00450d80(void);
void FUN_00450dd0(void);
int FUN_00428bc0(void);
int __stdcall FUN_004a5030(char* p);
void __stdcall FUN_004abd90(char* dest, char* text, int p3, int p4, int p5);
void FUN_00443cb0(void);
void FUN_00451540(void);
int __stdcall FUN_00451220(char param, int param2);
void __stdcall FUN_00450a10(int param);
int __stdcall FUN_004517b0(int a, int b, int c, int d, int e);
void FUN_00491a70(void);
int FUN_004436e0(void);
void __stdcall FUN_004ab0a0(int param);
void FUN_00450f90(void);
void FUN_00449bb0(void);
void __stdcall FUN_0046c620(int param);
void FUN_0044a680(void);
void FUN_0046ca60(void);
void __stdcall FUN_004a9660(int param);
void __stdcall FUN_004c9f90(int param);
void __stdcall FUN_00461020(int a, int b);
void FUN_0046c920(void);
void FUN_0046c190(void);
void FUN_00450e20(void);
char FUN_00441bc0(void);
void __stdcall FUN_004c9790(int param);
void __stdcall FUN_00450e20_2(int);
void __stdcall FUN_004450d80(int);

#define FRONTEND "c:\\cavedog\\wargame\\frontend.cpp"

// FUNCTION: 0x426e80
void FUN_00426e80(void)
{
    char buf[256];
    char uVar9;
    int uVar7;

    char c = g_game[0x2bc0];
    if (c != g_game[0x2bbf]) {
        FUN_004256d0(0xa3, FRONTEND);
        g_game[0x2bbf] = c;
        g_game[0x2bc0] = c;
    }

    switch ((unsigned char)g_game[0x2bbe]) {
    case 0: {
        Class_004b6220* d = FUN_004b6220();
        FUN_004c22d0(0);
        if (((d->field_f0 >> 1) & 1) != 0) {
            if (*(int*)(g_game + 0x3923d) != 0) {
                FUN_00426780("1.zrb");
                FUN_004256d0(0x3dc, FRONTEND);
                g_game[0x2bbe] = 1;
                FUN_004256d0(0x9b, FRONTEND);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                *(int*)(g_game + 0x3923d) = 0;
                FUN_00430f00();
                return;
            }
            if (*(int*)(g_game + 0x39245) == 0) {
                FUN_00426780("1.zrb");
                FUN_004256d0(0x3e6, FRONTEND);
            } else {
                FUN_004256d0(0x3e9, FRONTEND);
            }
        } else {
            FUN_004256d0(0x3ed, FRONTEND);
        }
        g_game[0x2bbe] = 2;
        FUN_004256d0(0x9b, FRONTEND);
        goto LAB_00427603;
    }
    case 1:
        FUN_00426780("2.zrb");
        goto LAB_00427477;
    case 2:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            if (FUN_00457710() != 0) {
                g_game[0x2a44] |= 1;
                g_game[0x2bee] |= 0x10;
                FUN_004256d0(0x403, FRONTEND);
                g_game[0x2bbe] = 0x10;
                FUN_004256d0(0x9b, FRONTEND);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x404, FRONTEND);
                g_game[0x2bbf] = 0x12;
                g_game[0x2bc0] = 0x12;
                FUN_004c22d0(1);
                return;
            }
            FUN_00434ab0(0);
            *(short*)(g_game + 0x2bee) &= 0xffef;
            FUN_004263b0();
            if (DAT_00512c80 != 0) {
                FUN_004256d0(0x40d, FRONTEND);
                g_game[0x2bbf] = 6;
                g_game[0x2bc0] = 6;
                FUN_004c22d0(1);
                return;
            }
            FUN_004256d0(0x40f, FRONTEND);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            FUN_004c22d0(1);
            return;
        case 5:
            g_game[0x2a44] |= 8;
            FUN_004256d0(0x421, FRONTEND);
            g_game[0x2bbe] = 7;
            break;
        case 6:
            FUN_00434ab0(3);
            *(short*)(g_game + 0x2a44) &= 0xfff7;
            FUN_004256d0(0x41c, FRONTEND);
            g_game[0x2bbe] = 0xf;
            break;
        case 7:
            FUN_004256d0(0x425, FRONTEND);
            uVar9 = 0;
            g_game[0x2bbe] = 0;
            FUN_004256d0(0x9b, FRONTEND);
            goto LAB_00427f8f;
        case 8:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c6890(0, 0);
            FUN_004c63a0();
            FUN_004b6230(0);
            return;
        case 9:
            FUN_004256d0(0x429, FRONTEND);
            g_game[0x2bbe] = 3;
            break;
        default:
            break;
        }
        goto LAB_00427f88;
    case 3:
        FUN_00426780("5.zrb");
        goto LAB_00427477;
    case 4:
        if (g_game[0x2bbf] == 0) {
            goto LAB_0042789a;
        }
        if (g_game[0x2bbf] == 1) {
            FUN_00426780("3.zrb");
            FUN_00426780("5.zrb");
            *(short*)(g_game + 0x2a44) &= 0xfffb;
            goto LAB_004272c7;
        }
        break;
    case 5:
        if (g_game[0x2bbf] == 0) {
            goto LAB_0042789a;
        }
        if (g_game[0x2bbf] == 1) {
            FUN_00426780("4.zrb");
            FUN_00426780("5.zrb");
            *(short*)(g_game + 0x2a44) &= 0xfffb;
            FUN_004256d0(0x45b, FRONTEND);
            g_game[0x2bbe] = 2;
            FUN_004256d0(0x9b, FRONTEND);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_00490b30(2);
            return;
        }
        break;
    case 0x10:
        /* only the checksum-error inner branch is transcribed, enough to make
           buf live and restore the 0x100-byte frame */
        FUN_00450dd0();
        if (FUN_00428bc0() != 0) {
            sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 0x3bc, FRONTEND);
            FUN_004abd90(g_game + 0x519, buf, 500, 1, 1);
        }
        g_game[0x2bbe] = 0xf;
        FUN_004257e0(0, 0x9b, FRONTEND);
        return;
    /* Cases 6..0x14 apart from 0x10 are not transcribed yet; stubs keep the
       outer jump table and the dispatch shape in place. */
    case 6:
    case 7:
    case 8:
    case 9:
    case 0xa:
    case 0xb:
    case 0xc:
    case 0xd:
    case 0xe:
    case 0xf:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    default:
        return;
    }

    return;

LAB_004272c7:
    FUN_004256d0(0x44a, FRONTEND);
    g_game[0x2bbe] = 2;
    FUN_004256d0(0x9b, FRONTEND);
    g_game[0x2bbf] = 0;
    g_game[0x2bc0] = 0;
    FUN_00490b30(2);
    return;

LAB_00427477:
    FUN_004256d0(0x437, FRONTEND);
    g_game[0x2bbe] = 2;
LAB_004275f2:
    FUN_004256d0(0x9b, FRONTEND);
LAB_00427603:
    g_game[0x2bbf] = 0;
    g_game[0x2bc0] = 0;
    return;

LAB_0042789a:
    FUN_004256d0(0x443, FRONTEND);
    g_game[0x2bbf] = 1;
    g_game[0x2bc0] = 1;
    return;

LAB_00427f88:
    FUN_004256d0(0x5b3, FRONTEND);
    uVar9 = 0;
LAB_00427f8f:
    g_game[0x2bbf] = uVar9;
    g_game[0x2bc0] = uVar9;
    return;

LAB_00427f0f:
    FUN_004256d0(0x516, FRONTEND);
    uVar9 = 0;
LAB_00428418:
    g_game[0x2bbf] = uVar9;
    g_game[0x2bc0] = uVar9;
    return;

LAB_0042809a:
    FUN_004c69a0(uVar7);
    FUN_004c2470();
    FUN_004c2870();
    FUN_004c63a0();
    return;
}
