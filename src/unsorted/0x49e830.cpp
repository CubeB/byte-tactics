// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and deepseek-v4.1-flash. Names are provisional.
// WinMain of Total Annihilation.
// PARTIAL 86.8%, 1343 of 1365 bytes. Full body, control flow and call
// sequence match. Remaining differences:
//  - DAT_0051f522 is a bitfield union and DAT_0051f410 is an `int` bitfield
//    tested in place at bit 11 (both now match their shapes).
//  - The seven single-bit sets of bits 1,4,5,6,7,8,9 fold into one
//    `or edx, 0x3f2`; the original keeps seven `or al/ah, K` ops. Declaring
//    the fields `unsigned char` stops the fold and lands within one byte of
//    the original size (1364) but puts bits 8 and 9 in a second byte
//    register (`or cl, 1` / `or cl, 2`) instead of `or ah, 1` / `or ah, 2`,
//    so the 16-bit allocation unit has to win and the fold has to lose.
//    Probed msvc5-sp3 codegen on the isolated block: a 16-bit bitfield
//    union, a bare 16-bit bitfield struct, `=1` and `|=1` forms, and a
//    `unsigned short` local with explicit `|=` all fold to a single
//    `or ecx/edx, 0x3f2`; the original's per-bit `or al/ah` chain was not
//    reproduced by any of them, so the original source used some construct
//    (or an interleaved barrier) this model could not identify in the
//    timebox. The `unsigned short` local form also adds a stack slot and
//    `push ecx`, so it is strictly worse.
//  - WIN (deepseek-v4.1-flash): the bit-0 toggle needs the 32-bit NOT form.
//    Writing `...b0 = ...b0 ^ ~DAT_0051fb48` narrows the complement to a byte
//    (`not dl`); routing it through an `int` local first
//    (`int notFlags = ~DAT_0051fb48;` then `...b0 = ...b0 ^ notFlags;`) gives
//    the original's 32-bit `not ecx` and raises 86.6% -> 86.8%.
//  - OpenSemaphoreA and FUN_004b5980 results are compared with a named zero
//    local (`cmp eax, ebx`) in the original; a named `int lzero = 0` still
//    emits `test eax, eax`, so the original must reach its zero by a route
//    this model does not reproduce yet.
//  - Everything else still differing is register naming and store
//    scheduling inside the DAT_0051f3xx block, the inlined strcpy and the
//    RegSetValueExA tail, plus the branch targets that follow from the 22
//    missing bytes above.
#include <new>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

class Class_004cee50 {
public:
    char pad[0x294];
    Class_004cee50();
};

class Class_004ce680 {
public:
    int FUN_004ce680();
};

class Class_004ce410 {
public:
    void FUN_004ce410();
};

class Class_004ce260 {
public:
    void FUN_004ce260();
};

class Class_004cd9d0 {
public:
    void FUN_004cd9d0(void (*param_1)());
};

class Class_004cedc0 {
public:
    void FUN_004cedc0(int param_1);
};

class Class_004ce7a0 {
public:
    void FUN_004ce7a0(int param_1);
};

class Class_004ce690 {
public:
    void FUN_004ce690(int param_1);
};

class Class_004cf0b0 {
public:
    void FUN_004cf0b0();
};

#pragma pack(push, 1)
struct Game_0049e830 {
    char unknown_0[1];                       // +0
    unsigned char field_1;                   // +1
    unsigned char field_2;                   // +2
    unsigned char field_3;                   // +3
    char unknown_4[0xc - 4];                 // +4
    void* field_c;                           // +0xc
    void* field_10;                          // +0x10
    char unknown_14[0x2a44 - 0x14];
    unsigned short field_2a44;               // +0x2a44
    char unknown_2a46[0x37f14 - 0x2a46];
    unsigned char field_37f14;               // +0x37f14
    unsigned char field_37f15;               // +0x37f15
    unsigned char field_37f16;               // +0x37f16
};
#pragma pack(pop)

extern Game_0049e830* g_game;

extern unsigned char DAT_0051f31c;
extern char* DAT_0050971c;
extern char* DAT_00509718;
extern int DAT_0051f320;
extern int DAT_0051f324;
extern int DAT_0051f328;
extern int DAT_0051f32c;
extern int DAT_0051f334;
extern int DAT_0051f51a;
extern int DAT_0051f51e;
extern int DAT_0051fb48;
extern char DAT_0051fb50[];
extern char DAT_005119b8[];
extern int DAT_0051f400;
extern int DAT_0051fb90;
extern int DAT_00509720;
extern DWORD DAT_0051fb94;

union Word_0051f522 {
    unsigned short value;
    struct {
        unsigned short b0 : 1;
        unsigned short b1 : 1;
        unsigned short b2 : 1;
        unsigned short b3 : 1;
        unsigned short b4 : 1;
        unsigned short b5 : 1;
        unsigned short b6 : 1;
        unsigned short b7 : 1;
        unsigned short b8 : 1;
        unsigned short b9 : 1;
        unsigned short spare : 6;
    } bits;
};
extern Word_0051f522 DAT_0051f522;

union Dword_0051f410 {
    int value;
    struct {
        unsigned b0 : 1;
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
        unsigned b8 : 1;
        unsigned b9 : 1;
        unsigned b10 : 1;
        unsigned b11 : 1;
        unsigned spare : 20;
    } bits;
};
extern Dword_0051f410 DAT_0051f410;

extern const char DAT_005097f4[];
extern const char DAT_005097e8[];
extern const char DAT_005097d0[];
extern const char DAT_005097b0[];
extern const char DAT_005097a8[];
extern const char DAT_00504ab8[];

void FUN_0049e700();
void FUN_0049ed90();
void FUN_004da1d0(int param_1);
void FUN_004d8e50(void (*param_1)());
void FUN_0041d920();
void FUN_0041d4c0();
void FUN_00428bb0();
void FUN_00491200();
void __stdcall FUN_004b52e0(void* param_1);
int __stdcall FUN_004b5980(void* param_1);
void __stdcall FUN_004b62d0(int param_1);
void __stdcall FUN_004b62c0(const char* param_1);
void __stdcall FUN_004b6110(void* param_1);
void FUN_00490f80();
void FUN_00490fe0();
void FUN_00499890();
void FUN_004c2cc0();
void FUN_004916a0();
int __stdcall FUN_0049ee30(char* param_1, char* param_2);
void __stdcall FUN_0042f980(const char* param_1, void* param_2, int* param_3);
void __stdcall FUN_0042f960(const char* param_1, void* param_2, int param_3);
void __stdcall FUN_004c54f0(const char* param_1, char* param_2);

// FUNCTION: 0x49e830
int __stdcall FUN_0049e830(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                           LPSTR lpCmdLine, int nCmdShow)
{
    HKEY hKey;
    DWORD size14;
    DWORD type;
    DWORD size1c;
    DWORD size20;
    MSG msg;
    char buf40[0x34];
    char buf74[0x32];
    int lzero = 0;

    (void)hPrevInstance;

    FUN_004da1d0(8);
    FUN_004d8e50(FUN_0049e700);
    if ((DAT_0051f31c & 1) == 0) {
        DAT_0051f31c |= 1;
        atexit(FUN_0049ed90);
    }
    FUN_0041d920();
    if (OpenSemaphoreA(0x1f0003, lzero, DAT_0050971c) != (HANDLE)lzero)
        return -1;
    CreateSemaphoreA(NULL, 1, 1, DAT_0050971c);
    srand(time(0));
    if (FUN_0049ee30(lpCmdLine, DAT_0050971c) == 0)
        return 1;
    FUN_004b52e0(&DAT_0051f320);
    DAT_0051f51a = 0x280;
    int notFlags = ~DAT_0051fb48; DAT_0051f522.bits.b0 = DAT_0051f522.bits.b0 ^ notFlags;
    DAT_0051f522.bits.b1 = 1;
    DAT_0051f522.bits.b8 = 1;
    DAT_0051f522.bits.b4 = 1;
    DAT_0051f522.bits.b9 = 1;
    DAT_0051f522.bits.b5 = 1;
    DAT_0051f51e = 0x1e0;
    DAT_0051f522.bits.b6 = 1;
    DAT_0051f324 = nCmdShow;
    DAT_0051f522.bits.b7 = 1;
    DAT_0051f328 = (int)DAT_00509718;
    DAT_0051f320 = (int)hInstance;
    DAT_0051f32c = (int)DAT_0050971c;
    DAT_0051f334 = 0;
    if (FUN_004b5980(&DAT_0051f320) == lzero)
        return 0;
    FUN_004b62d0(0x1e);
    FUN_0041d4c0();
    FUN_004b62c0(DAT_005097f4);
    g_game->field_c = &DAT_0051f320;
    g_game->field_1 = 3;
    g_game->field_2 = 1;
    g_game->field_3 = 1;
    if (strlen(DAT_0051fb50) == 0) {
        size1c = 0x40;
        FUN_0042f980(DAT_005097e8, DAT_0051fb50, (int*)&size1c);
        if (DAT_0051fb50[0] == 0)
            strcpy(DAT_0051fb50, DAT_00504ab8);
    }
    FUN_004c54f0(DAT_005097d0, DAT_0051fb50);
    g_game->field_10 = new Class_004cee50;
    FUN_00428bb0();
    FUN_00491200();

    hKey = NULL;
    size20 = 0x32;
    size14 = 0x32;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, DAT_005097b0, 0, 0xf003f, &hKey) == 0) {
        if (RegQueryValueExA(hKey, NULL, NULL, &type, (LPBYTE)buf40, &size20) == 0) {
            RegSetValueExA(hKey, NULL, 0, type, (const BYTE*)DAT_005119b8, 1);
        } else {
            strcpy(buf40, DAT_005119b8);
        }
        size14 = 0x32;
        FUN_0042f980(DAT_005097a8, buf74, (int*)&size14);
        if (strlen(buf74) != 0)
            strcpy(buf40, buf74);
        else
            FUN_0042f960(DAT_005097a8, buf40, 0x32);
        RegFlushKey(hKey);
        RegCloseKey(hKey);
    }

    for (;;) {
        for (;;) {
            if (DAT_0051f400 == lzero && *(int*)g_game->field_10 != 0) {
                FUN_00490f80();
                DAT_0051fb90 = ((Class_004ce680*)g_game->field_10)->FUN_004ce680();
                ((Class_004ce410*)g_game->field_10)->FUN_004ce410();
                DAT_00509720 = 1;
            } else if (DAT_0051f400 != lzero && *(int*)g_game->field_10 == 0
                       && DAT_00509720 != 0) {
                ((Class_004ce260*)g_game->field_10)->FUN_004ce260();
                ((Class_004cd9d0*)g_game->field_10)->FUN_004cd9d0(FUN_00490fe0);
                ((Class_004cedc0*)g_game->field_10)->FUN_004cedc0(g_game->field_37f14 & 1);
                ((Class_004ce7a0*)g_game->field_10)->FUN_004ce7a0(g_game->field_37f16);
                ((Class_004ce690*)g_game->field_10)->FUN_004ce690(DAT_0051fb90);
                FUN_00490fe0();
                DAT_00509720 = 0;
            }
            if (PeekMessageA(&msg, NULL, 0, 0, 0) != 0)
                break;
            if (DAT_0051f400 == 0 && (g_game->field_2a44 & 1) == 0)
                break;
            FUN_00499890();
            {
                DWORD tick = GetTickCount();
                if ((int)(tick - DAT_0051fb94) >= 100) {
                    ((Class_004cf0b0*)g_game->field_10)->FUN_004cf0b0();
                    DAT_0051fb94 = tick;
                }
            }
        }
        if (GetMessageA(&msg, NULL, 0, 0) == 0)
            break;
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    if (DAT_0051f410.bits.b11) {
        FUN_004c2cc0();
        FUN_004916a0();
    }
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, DAT_005097b0, 0, 0xf003f, &hKey) == 0) {
        RegSetValueExA(hKey, NULL, 0, type, (const BYTE*)buf40,
                       (DWORD)strlen(buf40) + 1);
        RegFlushKey(hKey);
        RegCloseKey(hKey);
        strcpy(buf40, DAT_005119b8);
        FUN_0042f960(DAT_005097a8, buf40, 0x32);
    }
    FUN_004b6110(&DAT_0051f320);
    return msg.wParam;
}
