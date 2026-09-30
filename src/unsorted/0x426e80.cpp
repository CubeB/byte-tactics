// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol. Names are provisional.
// Retry #1779: GPT-6.1-sol confirmed 39.4% (6242/6310 bytes) after seven worker checks; the final checker did not MATCH.
// Partial, best verified score 39.4%, not MATCH. The state bodies are transcribed,
// but the outer and nested switch layout, shared exits, and register allocation
// still differ. Original 0x4270ba selects substate 1 for zero and substate 6 for nonzero.

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
int __stdcall FUN_00451220(unsigned char param, int param2);
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
int FUN_00441bc0(void);
void __stdcall FUN_004c9790(int param);

struct Class_00435a20 { int FUN_00435a20(char* map); };
struct Class_00463c60 { void FUN_00463c60(int value); };

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
        FUN_004256d0(0x437, FRONTEND);
        g_game[0x2bbe] = 2;
        goto LAB_004275f2;
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
            if (DAT_00512c80 == 0) {
                FUN_004256d0(0x40d, FRONTEND);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                FUN_004c22d0(1);
                return;
            }
            FUN_004256d0(0x40f, FRONTEND);
            g_game[0x2bbf] = 6;
            g_game[0x2bc0] = 6;
            FUN_004c22d0(1);
            return;
        case 1:
            uVar7 = *(int*)(g_game + 0x37e1b);
            goto LAB_0042809a;
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
            return;
        }
        FUN_004256d0(0x9b, FRONTEND);
        goto LAB_00427603;
    case 3:
        FUN_00426780("5.zrb");
        FUN_004256d0(0x43c, FRONTEND);
        g_game[0x2bbe] = 2;
        goto LAB_004275f2;
    case 4:
        if (g_game[0x2bbf] == 0) {
            FUN_004256d0(0x443, FRONTEND);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
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
            FUN_004256d0(0x443, FRONTEND);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
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
    case 7:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            FUN_004777a0();
            FUN_004644d0();
            FUN_004256d0(0x47c, FRONTEND);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 1:
            uVar7 = *(int*)(g_game + 0x37e1b);
            goto LAB_0042809a;
        case 3:
            FUN_004256d0(0x49d, FRONTEND);
            g_game[0x2bbe] = 2;
            goto LAB_004275f2;
        case 10:
            FUN_00478240(1);
            FUN_004256d0(0x485, FRONTEND);
            g_game[0x2bbe] = 8;
            FUN_004256d0(0x9b, FRONTEND);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_004256d0(0x486, FRONTEND);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 0xb:
            FUN_0042f9a0();
            FUN_00434ab0(2);
            FUN_004256d0(0x493, FRONTEND);
            g_game[0x2bbe] = 9;
            goto LAB_00427f0f;
        case 0xd:
            FUN_004256d0(0x497, FRONTEND);
            g_game[0x2bbe] = 10;
            FUN_004256d0(0x9b, FRONTEND);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_00460160();
            FUN_004256d0(0x499, FRONTEND);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        case 0xe:
            FUN_00478240(1);
            FUN_004256d0(0x4a2, FRONTEND);
            g_game[0x2bbe] = 8;
            FUN_004256d0(0x9b, FRONTEND);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_004256d0(0x4a3, FRONTEND);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            return;
        default: return;
        }
    case 8:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 1: uVar7 = *(int*)(g_game + 0x37e1b); goto LAB_0042809a;
        case 3: goto LAB_004275e0;
        case 0xf:
            FUN_00434ab0(1);
            FUN_004256d0(0x4c2, FRONTEND);
            g_game[0x2bbe] = 0xb;
            goto LAB_004275f2;
        case 0x10:
            FUN_00434ab0(1);
            FUN_004256d0(0x4c7, FRONTEND);
            g_game[0x2bbe] = 0xc;
            goto LAB_004275f2;
        default: return;
        }
    case 9:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0: FUN_0047bbb0(); FUN_004256d0(0x4d9, FRONTEND); g_game[0x2bbf] = 1; g_game[0x2bc0] = 1; return;
        case 1: uVar7 = *(int*)(g_game + 0x37e1b); goto LAB_0042809a;
        case 2: g_game[0x2a44] |= 4; return;
        case 3: goto LAB_004275e0;
        }
    case 10:
        if (g_game[0x2bbf] == 1) {
            uVar7 = *(int*)(g_game + 0x37e1b);
            goto LAB_0042809a;
        }
        if (g_game[0x2bbf] != 3) return;
        FUN_004256d0(0x4b1, FRONTEND);
        g_game[0x2bbe] = 7;
        goto LAB_004275f2;
    case 0xb:
    case 0xc:
    case 0xd:
    case 0xe:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0: FUN_00478e80(); FUN_004256d0(0x4f5, FRONTEND); g_game[0x2bbf] = 1; g_game[0x2bc0] = 1; return;
        case 1: uVar7 = *(int*)(g_game + 0x37e1b); goto LAB_0042809a;
        case 2: g_game[0x2a44] |= 4; return;
        case 3:
            switch ((unsigned char)g_game[0x2bbe]) {
            case 0xb:
                FUN_00478240(0);
                FUN_004256d0(0x505, FRONTEND);
                g_game[0x2bbe] = 8;
                FUN_004256d0(0x9b, FRONTEND);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x506, FRONTEND);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                return;
            case 0xc:
                FUN_00478240(1);
                FUN_004256d0(0x50a, FRONTEND);
                g_game[0x2bbe] = 8;
                FUN_004256d0(0x9b, FRONTEND);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x50b, FRONTEND);
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
            case 0xe: FUN_00490b30(2); goto LAB_00427762;
            default: return;
            }
        }
    case 0xf:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            *(unsigned short*)(g_game + 0x2aaf) &= 0xfffd;
            *(unsigned short*)(g_game + 0x2aaf) &= 0xfffe;
            if (FUN_00457710() != 0) {
                g_game[0x2a44] |= 1;
                FUN_004256d0(0x52d, FRONTEND);
                g_game[0x2bbe] = 0x10;
                FUN_004256d0(0x9b, FRONTEND);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x52e, FRONTEND);
                g_game[0x2bbf] = 0x12;
                g_game[0x2bc0] = 0x12;
                return;
            }
            FUN_00444580();
            if (FUN_00443ff0(-1) != 0) {
                FUN_004256d0(0x534, FRONTEND);
                g_game[0x2bbf] = 2;
                g_game[0x2bc0] = 2;
                return;
            }
            goto LAB_0042789a;
        case 1: uVar7 = *(int*)(g_game + 0x37e1b); goto LAB_0042809a;
        case 2:
            if (memcmp(g_game + 0x39201, DAT_004fcdc8, 16) == 0) {
                FUN_004256d0(0x547, FRONTEND);
                g_game[0x2bbe] = 0x14;
                FUN_004256d0(0x9b, FRONTEND);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x548, FRONTEND);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                FUN_00443100();
                return;
            }
            if (memcmp(g_game + 0x39201, DAT_004fcdb8, 16) == 0) {
                FUN_004256d0(0x54e, FRONTEND);
                g_game[0x2bbe] = 0x14;
                FUN_004256d0(0x9b, FRONTEND);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x54f, FRONTEND);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                FUN_00442560();
                return;
            }
            if (memcmp(g_game + 0x39201, DAT_004fcda8, 16) == 0) {
                FUN_004256d0(0x555, FRONTEND);
                g_game[0x2bbe] = 0x14;
                FUN_004256d0(0x9b, FRONTEND);
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                FUN_004256d0(0x556, FRONTEND);
                g_game[0x2bbf] = 1;
                g_game[0x2bc0] = 1;
                FUN_004421f0();
                return;
            }
            if (FUN_00450d80() == 0) {
                strncpy(DAT_00511fb8, "An error occurred trying to use this service", 0xf9);
                FUN_004256d0(0x3bc, FRONTEND);
                g_game[0x2bbe] = 0xf;
                FUN_004257e0(0, 0x9b, FRONTEND);
                FUN_004257e0(0, 0x3bd, FRONTEND);
                return;
            }
            g_game[0x2a44] |= 1;
            FUN_004256d0(0x55d, FRONTEND);
            g_game[0x2bbe] = 0x10;
            FUN_004256d0(0x9b, FRONTEND);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_004256d0(0x55e, FRONTEND);
            uVar9 = 0;
            goto LAB_00428418;
        case 3:
            FUN_004256d0(0x564, FRONTEND);
            g_game[0x2bbe] = 2;
            goto LAB_00427f0f;
        case 0xd:
            FUN_004256d0(0x53b, FRONTEND);
            g_game[0x2bbe] = 10;
            FUN_004256d0(0x9b, FRONTEND);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
            FUN_00460160();
            goto LAB_00427915;
        }
    case 0x10:
        FUN_004c1ab0();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            FUN_004c9790(1);
            FUN_004256d0(0x587, FRONTEND);
            g_game[0x2bbf] = 1;
            g_game[0x2bc0] = 1;
            FUN_004644d0();
            if (memcmp(g_game + 0x39201, DAT_004fcdc8, 16) == 0 ||
                memcmp(g_game + 0x39201, DAT_004fcdb8, 16) == 0) {
                if (g_game[0x2aaf] & 1) {
                    FUN_004256d0(0x58f, FRONTEND);
                    uVar9 = 0x11;
                    goto LAB_00427f8f;
                }
                *(int*)(g_game + 0x2ba2) = 0;
                *(int*)(g_game + 0x2ba6) = 0;
                *(int*)(g_game + 0x2baa) = 0;
                *(int*)(g_game + 0x2bae) = 0;
            }
            FUN_00443cb0();
            goto SHOW_ERROR;
        case 1:
            FUN_004c69a0(*(int*)(g_game + 0x37e1b));
            FUN_004c2470();
            FUN_004c2870();
            FUN_004c63a0();
SHOW_ERROR:
            if (strlen(DAT_00511fb8) != 0) {
                FUN_004abd90(g_game + 0x519, DAT_00511fb8, FUN_004a5030(DAT_00511fb8) + 20, 1, 1);
                DAT_00511fb8[0] = 0;
            }
            return;
        case 3:
            FUN_00450dd0();
            if (FUN_00428bc0() != 0) {
                sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 0x5f0, FRONTEND);
                FUN_004abd90(g_game + 0x519, buf, 500, 1, 1);
            }
            g_game[0x2bbe] = 0xf;
            FUN_004257e0(0, 0x9b, FRONTEND);
            return;
        case 0x11:
            FUN_00451540();
            if (FUN_00451220(g_game[0x2a42], 1) != 0)
                FUN_00450a10(*(int*)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b67));
            FUN_004256d0(0x5ab, FRONTEND);
            g_game[0x2bbe] = 0x11;
            goto LAB_00427f0f;
        case 0x13:
            *(unsigned char*)(*(char**)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b8a) + 0x9b) |= 0x40;
        case 0x12:
            if (FUN_004517b0(*(int*)(g_game + 0x2ba2), *(int*)(g_game + 0x2ba6), *(int*)(g_game + 0x2baa), *(int*)(g_game + 0x2bae), (unsigned char)g_game[0x2a42]) == 0)
                goto LAB_00427f88;
            if (g_game[0x2bbf] == 0x12) {
                FUN_004c69a0(*(int*)(g_game + 0x37e1b));
                FUN_004c6890(0, 0);
                FUN_004c63a0();
                FUN_00491a70();
                if (FUN_004436e0() != 0) {
                    FUN_004ab0a0((int)(g_game + 0x519));
                    FUN_004256d0(0x5c1, FRONTEND);
                    g_game[0x2bbf] = 0x14;
                    g_game[0x2bc0] = 0x14;
                    return;
                }
                FUN_004256d0(0x5c4, FRONTEND);
            } else FUN_004256d0(0x5c7, FRONTEND);
            g_game[0x2bbf] = 0x15;
            g_game[0x2bc0] = 0x15;
            return;
        case 0x15: {
            char* player = *(char**)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b8a);
            if (*(int*)(g_game + 0x4e5) == 0) {
                *(unsigned short*)(player + 0x97) &= 0xfffe;
            } else {
                unsigned short flags = *(unsigned short*)(player + 0x97);
                *(unsigned short*)(player + 0x97) = flags ^ (((*(unsigned int*)(*(char**)(g_game + 0x4e5) + 4) >> 1) ^ flags) & 1);
            }
            if (g_game[0x2bbf] == 0x13)
                *(unsigned char*)(*(char**)(g_game + (unsigned char)g_game[0x2a42] * 0x14b + 0x1b8a) + 0x9b) |= 0x40;
            unsigned int index = (unsigned char)g_game[0x2a42];
            g_game[index * 0x14b + 0x1b84] = (g_game[index * 0x14b + 0x1b84] & 0xfd) | (((unsigned char)g_game[0x2b4c] >> 4 & 1) << 1);
            if ((unsigned char)g_game[0x2b4c] >> 4 & 1) {
                ((Class_00435a20*)*(void**)(g_game + 0x391e9))->FUN_00435a20(g_game + 0x2ab1);
                int offset = 0;
                do {
                    if (*(int*)(g_game + 0x1b63 + offset) != 0 &&
                        (g_game[0x1bd6 + offset] == 1 || g_game[0x1bd6 + offset] == 2))
                        FUN_00450a10(*(int*)(g_game + 0x1b67 + offset));
                    offset += 0x14b;
                } while (offset < 0xcee);
                g_game[0x2a44] |= 4;
                return;
            }
            if (FUN_00428bc0() != 0) {
                sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 0x5ea, FRONTEND);
                FUN_004abd90(g_game + 0x519, buf, 500, 1, 1);
            }
            g_game[0x2bbe] = 0x11;
            FUN_004257e0(0, 0x9b, FRONTEND);
            return;
        }
        case 0x14: uVar7 = *(int*)(g_game + 0x37e1b); goto LAB_0042809a;
        }
        return;
    case 0x11:
        uVar9 = 0x11;
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            if ((g_game[0x2a44] & 4) != 0) {
                FUN_0046ca60();
                FUN_004256d0(0x605, FRONTEND);
                g_game[0x2bbf] = 0x11;
                g_game[0x2bc0] = 0x11;
                FUN_00450f90();
                return;
            }
            FUN_00449bb0();
            FUN_0046c620(1);
            FUN_0046c620(2);
            FUN_004256d0(0x5ff, FRONTEND);
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
            if (((unsigned char)g_game[0x2a44] >> 2 & 1) != 0) {
                FUN_0046ca60();
                FUN_004a9660((int)(g_game + 0x519));
                if (FUN_00428bc0() != 0) {
                    sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 0x613, FRONTEND);
                    FUN_004abd90(g_game + 0x519, buf, 500, 1, 1);
                }
                goto LAB_00428418;
            }
            return;
        case 3: {
            g_game[0x2a44] |= 1;
            FUN_004c9f90((int)(g_game + 0x14));
            FUN_00461020(2, 100);
            FUN_0046c920();
            FUN_0046c620(8);
            FUN_0046c190();
            if (((unsigned char)g_game[0x2bee] >> 4 & 1) != 0) {
                FUN_00450e20();
                return;
            }
            int state = FUN_00441bc0();
            if (state == 0 || state == 3) {
                if (FUN_00428bc0() != 0) {
                    sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 0x640, FRONTEND);
                    FUN_004abd90(g_game + 0x519, buf, 500, 1, 1);
                }
                g_game[0x2bbe] = 0xf;
            } else {
                if (FUN_00428bc0() != 0) {
                    sprintf(buf, "Code segment checksum error found when switching FE states.\nState change called from [line %d, file %s]", 0x643, FRONTEND);
                    FUN_004abd90(g_game + 0x519, buf, 500, 1, 1);
                }
                g_game[0x2bbe] = 0x10;
            }
            FUN_004257e0(0, 0x9b, FRONTEND);
            return;
        }
        case 0x11: {
            Class_00463c60* player = (Class_00463c60*)(g_game + 0x1b63);
            int count = 10;
            do {
                if (*((char*)player + 0x73) == 4) player->FUN_00463c60(0);
                player = (Class_00463c60*)((char*)player + 0x14b);
            } while (--count != 0);
            FUN_0046ca60();
            g_game[0x2a44] |= 4;
            return;
        }
        default: return;
        }
    case 0x14:
        if (g_game[0x2bbf] != 1) return;
        if ((g_game[0x2aaf] & 2) != 0) {
            if (FUN_00450d80() == 0) {
                strncpy(DAT_00511fb8, "An error occurred trying to use this service", 0xf9);
                FUN_00425860(0xf, 0x3bc, FRONTEND);
                FUN_004257e0(0, 0x3bd, FRONTEND);
                *(unsigned short*)(g_game + 0x2aaf) &= 0xfffe;
                *(unsigned short*)(g_game + 0x2aaf) &= 0xfffd;
                FUN_004256d0(0x577, FRONTEND);
                g_game[0x2bbe] = 0xf;
                FUN_004257e0(0, 0x9b, FRONTEND);
            } else {
                g_game[0x2a44] |= 1;
                FUN_004256d0(0x571, FRONTEND);
                g_game[0x2bbe] = 0x10;
                FUN_004257e0(0, 0x9b, FRONTEND);
            }
            FUN_004256d0(0x578, FRONTEND);
            g_game[0x2bbf] = 0;
            g_game[0x2bc0] = 0;
        }
        uVar7 = *(int*)(g_game + 0x37e1b);
        goto LAB_0042809a;
    default: return;
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

LAB_004275f2:
    FUN_004256d0(0x9b, FRONTEND);
LAB_00427603:
    g_game[0x2bbf] = 0;
    g_game[0x2bc0] = 0;
    return;

LAB_0042789a:
    FUN_004256d0(0x536, FRONTEND);
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
    FUN_004256d0(0x9b, FRONTEND);
    uVar9 = 0;
LAB_00428418:
    g_game[0x2bbf] = uVar9;
    g_game[0x2bc0] = uVar9;
    return;

LAB_004275e0:
    FUN_004256d0(0x4e5, FRONTEND);
    g_game[0x2bbe] = 7;
    goto LAB_004275f2;
LAB_00427762:
    FUN_004256d0(0x516, FRONTEND);
    g_game[0x2bbe] = 7;
    goto LAB_004275f2;
LAB_00427915:
    FUN_004256d0(0x53d, FRONTEND);
    g_game[0x2bbf] = 1;
    g_game[0x2bc0] = 1;
    return;
LAB_0042809a:
    FUN_004c69a0(uVar7);
    FUN_004c2470();
    FUN_004c2870();
    FUN_004c63a0();
    return;
}
