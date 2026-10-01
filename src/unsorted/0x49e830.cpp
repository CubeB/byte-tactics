// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and deepseek-v4.1-flash, edited by deepseek-v4.1, retried by Sonnet 5.5. Names are provisional.
// Sonnet 5.5 retry: 87.7% -> 89.2% (1366 of 1365 bytes). The globals at
// 0x51f320..0x51f522 are fields of ONE object (the App struct of 0x4b5980:
// hInstance +0, nCmdShow +4, className +8, title +0xc, menuId +0x14, the
// flags dword +0xf0, startWidth +0x1fa, startHeight +0x1fe, the video word
// +0x202), not separate externs. Declaring them as members of one struct
// (DAT_0051f320) is what makes MSVC keep the seven single-bit sets of the
// video word as the original's `or al,K` / `or ah,K` chain on one ax register
// instead of folding them into `or edx,0x3f2`. The bit-0 update is also a
// plain bitfield assignment, `video.bits.b0 = ~DAT_0051fb48;` (the byte load
// plus word xor is the bitfield-assign idiom), not b0 ^ flag. What is left is
// instruction scheduling of the stores around the or chain (the original
// interleaves them in program order) and the g_game store register rotation.
// WinMain of Total Annihilation.
// PARTIAL 87.7%, 1345 of 1365 bytes. Best kept here; scratch probes this
// session all scored lower (unsigned-char single-struct bits -> 86.0% with
// byte RMWs in cl/al, local union copy -> 78.5% with the copy spilled to a
// stack slot and frame 0x9c). To get the original's `or al,/or ah,` byte
// chain on one ax register (byte ors a full-reg merge will not fold) the two
// bytes must be coalesced into ax via a word load+store; every source form
// tried either folded to `or eax,0x3f2` (unsigned short bits) or split the
// bytes into separate byte registers (unsigned char bits). Not solved here.
// Full body, control flow and call
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
//  - deepseek-v4.1: the `mov dl, byte [0x51f522]` first access shows the
//    original b0 assignment is a byte-typed bitfield read; a union member
//    `struct { unsigned char c0 : 1; } byte0` compiles to that byte read but
//    MSVC then emits a byte store plus a separate `or word ptr [0x51f522]`
//    word RMW, so it is worse than the current word-typed b0 access.
//    Isolated msvc5-sp3 probes (16-bit bitfield struct, char-based struct,
//    union of both, `= 1`, `|= 1`, `unsigned short` local with `|=`) all
//    fold the seven bit sets into one `or eax/word ptr, 0x3f2`; that fold is
//    a backend constant merge the original somehow blocked, and no source
//    form found in this timebox prevented it.
//  - deepseek-v4.1-flash (retry): the or-chain root cause is now clear. The
//    original's per-bit ors alternate low/high byte (al, ah, al, ah, al, al,
//    al), so no two same-register ors are adjacent and MSVC5's constant-merge
//    peephole never fires. Reproducing that requires the 16-bit value resident
//    in ax (a word load) while the individual fields use byte-granular storage
//    units. `unsigned char` bitfields stop the fold (1366-1367 bytes, 86.0-86.5)
//    but land the low byte in cl and the high byte in a second byte register
//    (al or cl, never ah) and emit two byte stores instead of one word store.
//    Adding a `.value` word toggle makes it worse (85.7, 1389 bytes). Every
//    variant tried this session (vA-vI in build/scratch/0x49e830/) scored below
//    this 87.7% folded baseline. The value-in-ax plus byte-granular-fields
//    combination was not reachable from source.
//  - deepseek-v4.1: the `mov ecx,[esp+0xbc]` for nCmdShow sits after the
//    hoisted `push &DAT_0051f320` in the original but before it here, which
//    is a scheduler choice, not a frame difference ([esp+0xb0] for
//    hInstance matches).
//  - deepseek-v4.1: writing `size20 = 0x32; hKey = NULL; size14 = 0x32;`
//    (that order) fixes the two swapped stores at [esp+0x34]/[esp+0x24]
//    and raised 86.8% to 87.1%.
//  - deepseek-v4.1: both zero compares that the original does as
//    `cmp eax, ebx` only come out that way when the call result is first
//    bound to a named local: `HANDLE hSem = OpenSemaphoreA(...); if (hSem !=
//    (HANDLE)lzero)` and `int bGameOk = FUN_004b5980(...); if (bGameOk ==
//    lzero)` give `cmp eax, ebx`, while the inline forms give
//    `test eax, eax`. Those two changes took 86.8% -> 87.3% -> 87.7%.
//  - space-bunny-free (retry): four msvc5-sp3 /Fa probes on the isolated
//    block narrow the cause and close one door. (1) MSVC 5 merges consecutive
//    `field = 1` sets of one bitfield container into ONE or of the combined
//    mask (`or WORD PTR x, 0x3f2`, or `or eax,0x3f2` once the container has
//    been widened to 32 bits), and it does NOT narrow an or to byte width for
//    a 16-bit container, so the original's `or al/ah` pair cannot come from
//    `unsigned short` bitfields however they are spelled. (2) With
//    `unsigned char` bitfields MSVC emits one byte or PER CONTIGUOUS SOURCE
//    RUN, which is exactly the original's pattern of singletons, so byte
//    containers are right and the fold is right to lose. (3) DEAD END, do not
//    retry: a struct that MIXES the types, `unsigned char b0:1` followed by
//    `unsigned short b1:1...`, does not overlay the two containers, MSVC 5 puts
//    the short container at OFFSET 2 and emits `or WORD PTR x+2, 0x1f9`. There
//    is no source form that gives a byte-container read and a word-container
//    or chain in one register. (4) `unsigned short v = x; v |= 2; v |= 0x100;
//    ...` pushes v to a stack slot and then merges, so it is worse.
//  - deepseek-v4.1: what is left is the flags block above (12 lines) and the
//    g_game field-store register rotation (ecx/edx/eax/ecx in the original,
//    edx/eax/ecx/edx here, driven by the inlined strcpy below saving its
//    low-bit count in edx instead of eax); every other hunk is only a branch
//    target shifted by the 20 missing bytes.
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
extern int DAT_0051fb48;
extern char DAT_0051fb50[];
extern char DAT_005119b8[];
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

#pragma pack(push, 2)
struct App_0049e830 {
    int hInstance;                     // +0x00
    int nCmdShow;                      // +0x04
    int className;                     // +0x08
    int title;                         // +0x0c
    int unknown_10;                    // +0x10
    int menuId;                        // +0x14
    char unknown_18[0xe0 - 0x18];
    int field_e0;                      // +0xe0
    char unknown_e4[0xf0 - 0xe4];
    Dword_0051f410 flags;              // +0xf0
    char unknown_f4[0x1fa - 0xf4];
    int startWidth;                    // +0x1fa
    int startHeight;                   // +0x1fe
    Word_0051f522 video;               // +0x202
};
#pragma pack(pop)
extern App_0049e830 DAT_0051f320;


extern const char DAT_005097f4[];
extern const char DAT_005097e8[];
extern const char DAT_005097d0[];
extern const char DAT_005097b0[];
extern const char DAT_005097a8[];
extern const char DAT_00504ab8[];

void FUN_0049e700();
void __cdecl FUN_0049ed90();
void __cdecl FUN_004da1d0(int param_1);
void __cdecl FUN_004d8e50(void (*param_1)());
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
    HANDLE hSem = OpenSemaphoreA(0x1f0003, lzero, DAT_0050971c);
    if (hSem != (HANDLE)lzero)
        return -1;
    CreateSemaphoreA(NULL, 1, 1, DAT_0050971c);
    srand(time(0));
    if (FUN_0049ee30(lpCmdLine, DAT_0050971c) == 0)
        return 1;
    FUN_004b52e0(&DAT_0051f320);
    DAT_0051f320.startWidth = 0x280;
    int notFlags = ~DAT_0051fb48; DAT_0051f320.video.bits.b0 = notFlags;
    DAT_0051f320.video.bits.b1 = 1;
    DAT_0051f320.video.bits.b8 = 1;
    DAT_0051f320.video.bits.b4 = 1;
    DAT_0051f320.video.bits.b9 = 1;
    DAT_0051f320.video.bits.b5 = 1;
    DAT_0051f320.hInstance = (int)hInstance;
    DAT_0051f320.video.bits.b6 = 1;
    DAT_0051f320.nCmdShow = nCmdShow;
    DAT_0051f320.video.bits.b7 = 1;
    DAT_0051f320.className = (int)DAT_00509718;
    DAT_0051f320.startHeight = 0x1e0;
    DAT_0051f320.title = (int)DAT_0050971c;
    DAT_0051f320.menuId = 0;
    int bGameOk = FUN_004b5980(&DAT_0051f320);
    if (bGameOk == lzero)
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

    size20 = 0x32;
    hKey = NULL;
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
            if (DAT_0051f320.field_e0 == lzero && *(int*)g_game->field_10 != 0) {
                FUN_00490f80();
                DAT_0051fb90 = ((Class_004ce680*)g_game->field_10)->FUN_004ce680();
                ((Class_004ce410*)g_game->field_10)->FUN_004ce410();
                DAT_00509720 = 1;
            } else if (DAT_0051f320.field_e0 != lzero && *(int*)g_game->field_10 == 0
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
            if (DAT_0051f320.field_e0 == 0 && (g_game->field_2a44 & 1) == 0)
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

    if (DAT_0051f320.flags.bits.b11) {
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