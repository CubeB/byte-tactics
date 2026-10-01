// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash. Names are provisional.
// Continued from a partial left by deepseek-v4.1-flash and GPT-6.
// PARTIAL, 84.3% (3431 of 3463 bytes, 32 bytes short): see the 0758Z note
// below for the current byte accounting. The older description of the hunks
// (kept for history) was inaccurate for the bar blocks:
// - the original emits `push edi` BEFORE the g_game load and then
//   `mov edx, [g_game]; mov cl, byte ptr [edx + 0x38d74]; mov [esp+0x18], 0x15b;
//   and ecx, 0xff; mov [esp+0x20], 0x16f; mov eax, ecx; shl eax, 3; sub eax, ecx;
//   lea ecx, [esp+0x28]`; ours hoists a `xor ecx, ecx` first, homes the temps at
//   [esp+0x10]/[esp+0x14]/[esp+0x1c] (4-byte-shifted slots) and sinks the byte
//   load below the immediate stores, which is where the 32 missing bytes live.
// - every branch past that block therefore differs by 0x20 (`jne 0x498ca3` vs
//   `jne 0x498c83`), so that hunk is the only real work item: the frame layout
//   of the setup locals and the store/load order have to move.
// Re-checked at 84.3% (3431 of 3463 bytes) in the 10-minute issue timebox of
// the next session; no variant was tried then either. New lead from the
// disassembly (applies to both bar blocks): the original's mid-block
// `push edi` at 0x498c2b (and the matching site in the 0x130/0x144 block) is
// the FIRST push of the FUN_004bf6f0 argument sequence and cdecl pushes
// right-to-left, so edi (the palette color) is the LAST-written argument and
// the call at 0x498c5e-64 pushes only the other two (`lea eax,[esp+0x14]`,
// `lea ecx,[esp+0x28]` = &rect). That means the source evaluates the
// color-valued argument before the rect stores, and the rect[1]/rect[3]
// immediates interleave with the byte load (0x15b lands between `mov cl,...`
// and `and ecx,0xff`), which is exactly the store/load order this version
// sinks below the immediates.
// 0817Z note (deepseek-v4.1-flash, 10-minute box): baseline re-verified at
// 84.3% (3431 of 3463 bytes, 22 diff hunks). One variant tried and rejected:
// writing the 0x38d6f/0x38d73 zero pair through an `int* p =
// (int*)((char*)g_game + 0x38d6f); p[0]=0; p[1]=0;` local (the original's
// `mov edx,[g_game]; add edx,0x38d6f; mov [edx],ecx; mov [edx+4],ecx`
// shape) regresses to 83.7% / 3433 bytes, because MSVC5 folds the local back
// into [reg+0x38d6f] displacement stores with the shared ebp zero and adds an
// extra `lea eax,[ecx+0x38d6f]` before the field_10 call schedule, so that
// pair needs a base the compiler cannot fold into a displacement.
// No variant was tried for the bar blocks this session.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)

struct PlayerData_00497f40 {
    char unknown_0[0x9b];
    unsigned char flags;               // +0x9b
};

struct PlayerInfo_00497f40 {
    char unknown_0[0x27];
    PlayerData_00497f40* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char control;             // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00497f40 {
    char unknown_0[0xc];
    int field_c;                       // +0xc
    int field_10;                      // +0x10
    char unknown_10[0x519 - 0x14];
    int field_519;                     // +0x519
    int field_51d;                     // +0x51d
    char unknown_521[0x531 - 0x521];
    int field_531;                     // +0x531
    char unknown_535[0x589 - 0x535];
    int field_589;                     // +0x589
    char unknown_58d[0xdcb - 0x58d];
    unsigned char palette[16];         // +0xdcb
    char unknown_ddb[0x11eb - 0xddb];
    int field_11eb;                    // +0x11eb
    char unknown_11ef[0x29a4 - 0x11ef];
    int players_29a4[10];              // +0x29a4
    int players_29cc[10];              // +0x29cc
    char unknown_29f4[0x2cbe - 0x29f4];
    unsigned char field_2cbe;          // +0x2cbe
    char unknown_2cbf[0x148cf - 0x2cbf];
    int field_148cf;                   // +0x148cf
    char unknown_148d3[0x37e1b - 0x148d3];
    int field_37e1b;                   // +0x37e1b
    int field_37e1f;                   // +0x37e1f
    int field_37e23;                   // +0x37e23
    int field_37e27;                   // +0x37e27
    int field_37e2b;                   // +0x37e2b
    int field_37e2f;                   // +0x37e2f
    int field_37e33;                   // +0x37e33
    int field_37e37;                   // +0x37e37
    int field_37e3b;                   // +0x37e3b
    char unknown_37e3f[0x37f1b - 0x37e3f];
    int field_37f1b;                   // +0x37f1b
    int field_37f1f;                   // +0x37f1f
    char unknown_37f23[0x38a37 - 0x37f23];
    unsigned int field_38a37;          // +0x38a37
    int field_38a3b;                   // +0x38a3b
    char pad_38a3f[0x38a47 - 0x38a3f];
    int field_38a47;                   // +0x38a47
    char pad_38a4b[0x38a4f - 0x38a4b];
    short field_38a4f;                 // +0x38a4f
    char unknown_38a51[0x38d6f - 0x38a51];
    unsigned char progress[6];         // +0x38d6f
    volatile unsigned short flags38d75;         // +0x38d75
    char unknown_38d77[0x391e9 - 0x38d77];
    int field_391e9;                   // +0x391e9
    char pad_391ed[0x391f1 - 0x391ed];
    int field_391f1;                   // +0x391f1
    void (*field_391f5)();             // +0x391f5
    int field_391f9;                   // +0x391f9
    char unknown_391fd[0x39239 - 0x391fd];
    short field_39239;                 // +0x39239
};

#pragma pack(pop)

extern Game_00497f40* g_game;

// Embedded surface at game offset 0x143a7.
#define SURFACE_143a7 ((void*)((char*)g_game + 0x143a7))

extern "C" int DAT_0051f2c8;
extern "C" int DAT_0051e810;
extern "C" int DAT_0051f2cc;
extern "C" short DAT_0051f2d0;
extern "C" int DAT_0051e814;
extern "C" short DAT_0051e818;
extern "C" int DAT_0051e6c8;
extern "C" int DAT_0051e6cc;
extern "C" int DAT_0051f308;
extern "C" int DAT_00506dbc;
extern "C" char DAT_00513000;
extern "C" int DAT_00504990;
extern "C" unsigned char DAT_0051e820, DAT_0051e821, DAT_0051e822;
extern "C" unsigned char DAT_0051e823, DAT_0051e824, DAT_0051e825;

void FUN_00497c70();
void FUN_00499200();
void __cdecl FUN_004609a0(int);
void __cdecl FUN_004257a0();
void __cdecl FUN_00428730();
void __cdecl FUN_00430f00();
void __cdecl FUN_00453d40();
void __cdecl FUN_00456de0();
void __cdecl FUN_0045b640();
void __cdecl FUN_00467d70();
void __cdecl FUN_0047f750();
void __cdecl FUN_00496790();
void __cdecl FUN_004c2870();
void __stdcall FUN_004c5fa0(void*);
void __cdecl FUN_004c62c0();
void __cdecl FUN_004c63a0();
int __cdecl FUN_004ce800();
void __cdecl FUN_004d85a0(void*);
void __stdcall FUN_004288d0(char*, int, int, int);
void __stdcall FUN_004290f0(void*, char*, char*, char*);
void __stdcall FUN_00453320(unsigned int, int);
void __stdcall FUN_00497ce0(void*);
void __stdcall FUN_0049fa70(void*);
int __stdcall FUN_004a5030(char*);
void __stdcall FUN_004a50e0(void*, char*, int, int, int, int);
void __stdcall FUN_004a9660(void*);
void __stdcall FUN_004ab400(void*, void*);
void __stdcall FUN_004ac7d0(void*, void*, void*);
void __stdcall FUN_004b4fd0(void (__cdecl *)(int), int);
void __stdcall FUN_004b5940(int, int);
void __stdcall FUN_004b6290(char*);
int __stdcall FUN_004b6b20(void (*)(void), int, int);
void __stdcall FUN_004b6b50(int);
void* __stdcall FUN_004b7f30(void*, int);
void __stdcall FUN_004b7f90(void*, void*, int, int);
void* __stdcall FUN_004b8d40(int, char*);
void __stdcall FUN_004ba200(void*, int, int);
void* __stdcall FUN_004bbe50(void*, unsigned int*);
void __stdcall FUN_004bf6f0(void*, void*, unsigned char);
void __stdcall FUN_004c13a0(int, int);
void __stdcall FUN_004c1420(int);
char* __stdcall FUN_004c5740(char*);
int __stdcall FUN_004c5e70(void*);
void __stdcall FUN_004c61f0(int);
void __stdcall FUN_004c69a0(void*);
void* __stdcall FUN_004c69f0(char*, int, int);
void __stdcall FUN_004c6b70(void*, void*, int, int);
void __stdcall FUN_004c9790(int);
class Class_004cdb40 { public: void FUN_004cdb40(); };
class Class_004ce690 { public: void FUN_004ce690(int); };
class Class_004ce800 { public: int FUN_004ce800(); };

int __cdecl FUN_004b6700();
int __cdecl FUN_004b6710();
int __cdecl FUN_004c13f0();
int __cdecl FUN_004c1450();
int __cdecl FUN_004568c0();
unsigned int __cdecl FUN_004b6340();
char* __cdecl FUN_0049f580();

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_00435c30 {
public:
    char* FUN_00435c30();
};

class Class_004618a0 {
public:
    void FUN_004618a0(int);
};

// PARTIAL 84.0% (3431 vs 3463 bytes), frame exactly 0x234. The address of
// every local now matches the original: rect at +0x10 (its slot widened to 20
// bytes by the dead tail loop, which stores players[i].control at byte +0x13
// and is kept because rect's address escapes), scr.gadget at +0x24 with a
// 24-byte hole after it, namebuf[100] at +0x60, buf[128] at +0xc4, aux[256] at
// +0x144. The x87 temps land at +0x54/+0x58 as in the original.
// Still differs, all schedule/allocator state rather than missing code:
//   (a) the player loop now uses eax for the array offset like the original,
//       but its two SIBs come out [edi+esi+0x1b63] and [eax+esi] where the
//       original has [esi+edi+0x1b63] and [esi+eax] (base g_game, index the
//       offset); both sum orders in source give the same reversed SIB.
//   (b) the DAT_0051f2c8..DAT_0051e818 block uses ebp as its single zero and
//       hoists the g_game load; the original uses eax/ecx/edx and reloads
//       g_game in the middle of the stores.
//   (c) every bar block schedules the rect constants, the progress byte load
//       and `push edi` (color) in a different order, and folds `>>n & 1` into
//       `test byte ptr [..], N` where the original materialises
//       `mov dl,[..]; shr dl,N; test dl,1`.
//   (d) the window-resize, strncpy/wsprintf and FUN_004b7f30 argument
//       schedules swap edx/ecx with no source-level lever found.
// Tried and rejected: parenthesising the offset sums, unsigned arrayOffset,
// explicit `unsigned char*` casts and locals for the flag byte, reordering the
// zero stores / zeroPair pointer / flags RMW between the stores (all neutral),
// temps for the FUN_004b7f90 rect args (neutral), declaring flags38d75 as a
// 4-bit bitfield (drops to 50.2%).
// 2565 note: the operand order of the offset sums does NOT flip the SIB (both
// `(char*)g_game + off` and `off + (char*)g_game` assemble to [off-reg +
// g_game-reg]); the byte local cannot be squeezed in either: declared after
// rect, last (after namebuf), or as a 20-byte rect struct with `char pad[3];
// unsigned char prog;`, the frame still grows by 4 (73.1 / 72.4 / 72.7), so
// the original's `and ecx,0xff` bar blocks stay unmatched.
// 2637 pass note: micro-tests with the real compiler show the per-bar
// `mov cl,[m]; and ecx,0xff` widening needs an unsigned char value that sits
// in a register before being widened; plain loads, casts, `& 0xff` and
// int/byte locals all give `xor ecx,ecx; mov cl,[m]`, and the two-use byte
// local that does produce it always keeps a stack slot (`mov [esp+X],cl;
// mov ecx,[esp+X]; and ecx,0xff`). Any added byte local costs the exact
// 0x234 frame (measured: 3431 -> 3502 bytes, 73.1%). Flash reads already
// match with the byte-pointer form; writing `(DAT_0051e6c8 >> n) & 0xff`
// instead is byte-identical to the current file (diff unchanged, so the
// original's `mov ecx,[0x51e6c8]; and ecx,0xff` is already reproduced).
// 2498 note: the original's per-bar `mov cl,[progress]; and ecx,0xff` (12 bytes)
// instead of our `xor ecx,ecx; mov cl,[progress]` (8 bytes) is reproduced by
// reading the progress byte through a byte-typed LOCAL (prog = progress[i];
// rect[2] = prog*7/2+0xcd): `and ecx,0xff` then appears and the bar block
// schedules correctly. But any extra local shifts the frame by 4 (gadget moves
// +0x24 -> +0x28), which costs far more than it wins (84.0 -> 80.6), so the
// byte must live inside the existing 20-byte rect slot or the Screen hole.
// GPT-6.1-sol retry pass: kept the measured 84.0% source. The source-level
// RECT-slot alias experiment for progress bytes regressed to 66.7%. Residual
// differences are SIB operand order, zero-store register scheduling, flag-test
// folding, and local/register scheduling around resize and bar drawing.
// 0705Z note (deepseek-v4.1-flash): one real source-level bug found and fixed:
// the map-name FUN_004a50e0 call is `..., -1, 0)` (not `0, -1`): the original
// pushes the zero (ebp) first and -1 second, i.e. param5 = -1, param6 = 0, the
// same (-1, 0) every other FUN_004a50e0 call in the exe uses. That alone took
// the file from 84.0 to 84.3% (3463 vs 3431 bytes). The remaining diff is the
// six bar blocks (byte-progress `and ecx,0xff` form plus rect store order), the
// SIB base/index order ([esi+edi+0x1b63] vs [edi+esi+0x1b63], confirmed again
// here: both source sum orders emit the same reversed SIB) and a long tail of
// eax/ecx/edx register-name swaps in otherwise identical instruction sequences.
// Retried the byte `prog` local (replacing the `flash` local's slot, inlining
// the flash global at the six FUN_004a50e0 sites to keep the frame): the
// allocator moves prog to +0x10 and shifts rect to +0x14, 3428 bytes, 69.6%.
// 0758Z note (deepseek-v4.1-flash, second pass): the 32-byte shortfall is now
// fully accounted for, and none of it is a control-flow difference:
//   (1) 24 bytes: the six bar blocks. The `and ecx,0xff` the older notes blamed
//       on a progress byte is really the flash read (0x49879d: mov ecx,[0x51e6c8];
//       and ecx,0xff), which this file already reproduces. What is missing is
//       the *progress* byte load: the original has `mov cl,[eax+0x38d6f]; and
//       ecx,0xff` (12 bytes) where we get `xor ecx,ecx; mov cl,[..]` (8 bytes),
//       six times. A block-scope `unsigned char prog = g_game->progress[i];
//       rect[2] = (prog*7)/2 + 0xcd;` does emit the wanted pair but homes prog
//       (3502 bytes, 73.3%), so the register must come from something else.
//   (2) 6 bytes: the DAT_0051f2c8..DAT_0051e6cc zero block (0x4982dd) uses three
//       fresh zeros (xor eax,eax / xor ecx,ecx / xor edx,edx) grouping the
//       stores f2c8+cc+d0+e820+e824 (eax), e810+814+818 (ecx), e6c8+e6cc (edx);
//       ours stores every one of them through the long-lived ebp zero.
//   (3) 6 bytes: the two flag tests (0x498342, 0x498499) load `mov dl,[..];
//       shr dl,1; test dl,1`, i.e. a byte-sized bitfield read; ours folds the
//       shift-and into `test byte ptr [..], 2`. A byte bitfield view of
//       g_game+0x38d75 for those two tests drops the file to 50.6%, because the
//       volatile word writes (`mov cx,[..]; or ecx,1; mov [..],cx`) must keep
//       their register form.
//   (4) -5 bytes: the 0x38d6f pair (0x4985?? in the original) is a pointer
//       local: `xor ecx,ecx; mov edx,[g]; add edx,0x38d6f; mov [edx],ecx;
//       mov [edx+4],ecx` (19 bytes); ours reloads g_game twice and stores
//       through [ecx+0x38d6f]/[edx+0x38d73] (24 bytes).
// The rest of the diff (0x49838c, 0x49851f, the wsprintf/SetWindowPos schedules
// and all six bar blocks) is eax/ecx/edx naming in otherwise identical streams.
// FUNCTION: 0x497f40
void FUN_00497f40(void)
{
    struct Surface { int width, height; char fields[16]; };
    struct Screen_00497f40 { Surface gadget; char hole[24]; };
    Screen_00497f40 scr;
    char buf[128];
    char aux[256];
    void* surfaceHandle;
    int i;
    int playersOffset;
    int arrayOffset;
    unsigned int color;
    int flash;
    int bar;
    int field_391e9;
    unsigned int stamp;
    int rect[4];
    PlayerInfo_00497f40* pi;
    char namebuf[100];

    if ((g_game->flags38d75 & 1) == 0) {
        while (g_game->field_531 != 0) {
            FUN_004a9660(&g_game->field_519);
        }
        FUN_0049fa70(&g_game->field_519);
        if (g_game->field_2cbe != 0x14) {
            g_game->field_2cbe = 0x14;
            FUN_004ab400(&g_game->field_519, (void*)g_game->field_148cf);
        }
        FUN_004c1420(g_game->field_391f9);
        FUN_004ba200(SURFACE_143a7, 0, 0x100);
        if (((Class_00435100*)g_game->field_391e9)->FUN_00435100() != 2) {
            FUN_00430f00();
        }
        while (g_game->field_531 != 0) {
            FUN_004a9660(&g_game->field_519);
        }
        FUN_004257a0();
        g_game->field_37e1f = 0x280;
        g_game->field_37e23 = 0x1e0;
        if (FUN_004b6700() != 0x280 || FUN_004b6710() != 0x1e0) {
            FUN_004d85a0((void*)g_game->field_37e1b);
            g_game->field_37e1b = 0;
            FUN_004c61f0(0);
            FUN_004c62c0();
            SetWindowPos(*(HWND*)(g_game->field_c + 0x40), 0, 0, 0, 0x280, 0x1e0, 4);
            FUN_004b5940(0x280, 0x1e0);
            g_game->field_37e1b = (int)FUN_004c69f0("OFFSCREEN", g_game->field_37e1f, g_game->field_37e23);
            FUN_004c61f0(g_game->field_37e1b);
            FUN_004c69a0((void*)g_game->field_37e1b);
        }
        FUN_004290f0(aux, "palettes", "guipal", "PAL");
        surfaceHandle = FUN_004bbe50((unsigned int*)aux, 0);
        FUN_004ac7d0(&g_game->field_519, SURFACE_143a7, surfaceHandle);
        FUN_004d85a0(surfaceHandle);
        g_game->field_38a37 = FUN_004b6340();
        g_game->field_38a3b = 0;
        g_game->field_38a47 = 0;
        g_game->field_38a4f = 0;
        g_game->field_39239 = (short)0xffff;
        g_game->field_37e1f = g_game->field_37f1b;
        g_game->field_37e23 = g_game->field_37f1f;
        g_game->field_37e27 = 0x80;
        g_game->field_37e2b = 0x20;
        g_game->field_37e2f = g_game->field_37e1f - 1;
        g_game->field_37e33 = g_game->field_37e23 - 0x21;
        g_game->field_37e37 = g_game->field_37e2f - g_game->field_37e27 + 1;
        g_game->field_37e3b = g_game->field_37e33 - g_game->field_37e2b + 1;
        FUN_004288d0("loadgame2bg", 0, 0, 0);
        memset((char*)g_game + 0x29a4, 0, 0x23 * 4);
        playersOffset = 0;
        arrayOffset = 0x29a4;
        do {
            pi = (PlayerInfo_00497f40*)((char*)g_game + playersOffset + 0x1b63);
            if (*(int*)pi == 0 || (pi->control != 1 && pi->control != 2)) {
                *(int*)((char*)g_game + arrayOffset) = 0;
            } else {
                *(int*)((char*)g_game + arrayOffset) = 1;
            }
            bar = (*(int*)pi != 0 && (pi->data->flags & 0x40) != 0) ? 1 : 0;
            arrayOffset += 4;
            playersOffset += 0x14b;
            *(int*)((char*)g_game + arrayOffset + 0x28) = bar;
        } while (arrayOffset < 0x29cc);
        if (!FUN_004b6b20(FUN_00497c70, 0, 0)) {
            FUN_004b6290("Unable to start the loading thread!");
        }
        DAT_0051f2c8 = 0;
        DAT_0051e810 = 0;
        DAT_0051f2cc = 0;
        DAT_0051f2d0 = 0;
        *(int*)&DAT_0051e820 = 0;
        *(short*)&DAT_0051e824 = 0;
        DAT_0051e814 = 0;
        DAT_0051e6c8 = 0;
        DAT_0051e818 = 0;
        *(short*)&DAT_0051e6cc = 0;
        g_game->flags38d75 |= 1;
        FUN_0045b640();
    }
    if (((unsigned char)g_game->flags38d75 >> 1 & 1) != 0) {
        FUN_0047f750();
        FUN_004257a0();
        FUN_00428730();
        if (FUN_004b6700() != g_game->field_37f1b || FUN_004b6710() != g_game->field_37f1f) {
            FUN_004d85a0((void*)g_game->field_37e1b);
            g_game->field_37e1b = 0;
            FUN_004c61f0(0);
            FUN_004c62c0();
            SetWindowPos(*(HWND*)(g_game->field_c + 0x40), 0, 0, 0, g_game->field_37f1b,
                         g_game->field_37f1f, 4);
            FUN_004b5940(g_game->field_37f1b, g_game->field_37f1f);
            g_game->field_37e1b = (int)FUN_004c69f0("OFFSCREEN", g_game->field_37e1f, g_game->field_37e23);
            FUN_004c61f0(g_game->field_37e1b);
        }
        FUN_00467d70();
        FUN_00496790();
        FUN_004c2870();
        g_game->field_391f1 = 6;
        g_game->field_391f5 = FUN_00499200;
        FUN_004b4fd0(FUN_004609a0, 0);
        g_game->field_589 = 0;
        *(int*)((char*)g_game + 0x38d6f) = 0;
        *(int*)((char*)g_game + 0x38d73) = 0;
        ((Class_004ce690*)g_game->field_10)->FUN_004ce690(0);
        if (!((Class_004ce800*)g_game->field_10)->FUN_004ce800()) {
            ((Class_004cdb40*)g_game->field_10)->FUN_004cdb40();
        }
        {
            unsigned char* pb = (unsigned char*)g_game + 0x1bd6;
            i = 10;
            do {
                if (*(int*)(pb - 0x73) != 0) *((unsigned char*)&rect + 19) = *pb;
                pb += 0x14b;
                i--;
            } while (i != 0);
        }
        return;
    }
    playersOffset = 0;
    do {
        pi = (PlayerInfo_00497f40*)((char*)g_game + playersOffset + 0x1b63);
        if (*(int*)pi != 0 && (pi->control == 1 || pi->control == 2) &&
            (FUN_00453320(*(unsigned int*)((char*)pi + 4), 0), DAT_00506dbc != 0)) {
            ((Class_004618a0*)&DAT_00513000)->FUN_004618a0(1);
        }
        playersOffset += 0x14b;
    } while (playersOffset < 0xcee);
    FUN_00453d40();
    if (((unsigned char)g_game->flags38d75 >> 2 & 1) != 0 && FUN_004568c0() != 0) {
        g_game->flags38d75 &= ~4;
        g_game->flags38d75 |= 8;
        FUN_004c9790(0);
    }
    if (DAT_00506dbc != 0) {
        ((Class_004618a0*)&DAT_00513000)->FUN_004618a0(1);
    }
    FUN_004c69a0((void*)g_game->field_37e1b);
    if (FUN_004c5e70(&scr.gadget) != 0) {
        color = g_game->palette[15];
        stamp = FUN_004b6340();
        if (DAT_0051f308 < (int)stamp) {
            DAT_0051f308 = FUN_004b6340();
            for (i = 0; i < 6; i++) {
                if (((char*)&DAT_0051e6c8)[i] != 0) {
                    ((char*)&DAT_0051e6c8)[i] -= 2;
                }
            }
        }
        FUN_004c1420(g_game->field_391f9);
        FUN_004c6b70(&scr.gadget, (void*)g_game->field_11eb, 0, 0);
        if (((Class_00435100*)g_game->field_391e9)->FUN_00435100() != 1) {
            FUN_004c13a0(color, 0xfe);
            strncpy(namebuf, ((Class_00435c30*)g_game->field_391e9)->FUN_00435c30(), 100);
            namebuf[99] = 0;
            if (FUN_0049f580() != 0 && _strcmpi((const char*)FUN_0049f580(), "english") != 0) {
                _strlwr(namebuf);
            }
            wsprintfA(buf, "%s: %s", (char*)FUN_004c5740("Map"), (char*)FUN_004c5740(namebuf));
            field_391e9 = FUN_004a5030(buf);
            {
                int x = scr.gadget.width / 2 - field_391e9 / 2;
                FUN_004a50e0(&scr.gadget, buf, x,
                             (int)((double)scr.gadget.height - (double)FUN_004c1450() * 1.5), -1, 0);
            }
        }
        {
            void* light = FUN_004b8d40(g_game->field_51d, "LIGHTBAR");
            void* lightbar = FUN_004b7f30(light, 0);
            *((short*)lightbar + 3) = 0;
            *((short*)lightbar + 2) = 0;
            color = g_game->palette[g_game->progress[0] < 100 ? 12 : 10];
            FUN_004c13a0(color, FUN_004c13f0());
            if(g_game->progress[0] == 100 && DAT_0051e820 != 100) {
                ((unsigned char*)&DAT_0051e6c8)[0] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6c8)[0];
            DAT_0051e820 = g_game->progress[0];
            FUN_004a50e0(&scr.gadget, (char*)FUN_004c5740("Textures"), 0x5a, 0x87, -1, flash);
            rect[0] = 0xcd;
            rect[1] = 0x87;
            rect[3] = 0x9b;
            rect[2] = ((int)g_game->progress[0] * 7) / 2 + 0xcd;
            FUN_004bf6f0(&scr.gadget, rect, color);
            FUN_004b7f90(&scr.gadget, lightbar, rect[0], rect[1]);
            color = g_game->palette[g_game->progress[1] < 100 ? 12 : 10];
            FUN_004c13a0(color, FUN_004c13f0());
            if(g_game->progress[1] == 100 && DAT_0051e821 != 100) {
                ((unsigned char*)&DAT_0051e6c8)[1] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6c8)[1];
            DAT_0051e821 = g_game->progress[1];
            FUN_004a50e0(&scr.gadget, (char*)FUN_004c5740("Terrain"), 0x5a, 0xb1, -1, flash);
            rect[0] = 0xcd;
            rect[1] = 0xb1;
            rect[3] = 0xc5;
            rect[2] = ((int)g_game->progress[1] * 7) / 2 + 0xcd;
            FUN_004bf6f0(&scr.gadget, rect, color);
            FUN_004b7f90(&scr.gadget, lightbar, rect[0], rect[1]);
            color = g_game->palette[g_game->progress[2] < 100 ? 12 : 10];
            FUN_004c13a0(color, FUN_004c13f0());
            if(g_game->progress[2] == 100 && DAT_0051e822 != 100) {
                ((unsigned char*)&DAT_0051e6c8)[2] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6c8)[2];
            DAT_0051e822 = g_game->progress[2];
            FUN_004a50e0(&scr.gadget, (char*)FUN_004c5740("Units"), 0x5a, 0xda, -1, flash);
            rect[0] = 0xcd;
            rect[1] = 0xda;
            rect[3] = 0xee;
            rect[2] = ((int)g_game->progress[2] * 7) / 2 + 0xcd;
            FUN_004bf6f0(&scr.gadget, rect, color);
            FUN_004b7f90(&scr.gadget, lightbar, rect[0], rect[1]);
            color = g_game->palette[g_game->progress[3] < 100 ? 12 : 10];
            FUN_004c13a0(color, FUN_004c13f0());
            if(g_game->progress[3] == 100 && DAT_0051e823 != 100) {
                ((unsigned char*)&DAT_0051e6c8)[3] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6c8)[3];
            DAT_0051e823 = g_game->progress[3];
            FUN_004a50e0(&scr.gadget, (char*)FUN_004c5740("Animation"), 0x5a, 0x106, -1, flash);
            rect[0] = 0xcd;
            rect[1] = 0x106;
            rect[3] = 0x11a;
            rect[2] = ((int)g_game->progress[3] * 7) / 2 + 0xcd;
            FUN_004bf6f0(&scr.gadget, rect, color);
            FUN_004b7f90(&scr.gadget, lightbar, rect[0], rect[1]);
            color = g_game->palette[g_game->progress[4] < 100 ? 12 : 10];
            FUN_004c13a0(color, FUN_004c13f0());
            if(g_game->progress[4] == 100 && DAT_0051e824 != 100) {
                ((unsigned char*)&DAT_0051e6cc)[0] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6cc)[0];
            DAT_0051e824 = g_game->progress[4];
            FUN_004a50e0(&scr.gadget, (char*)FUN_004c5740("3D Data"), 0x5a, 0x130, -1, flash);
            rect[0] = 0xcd;
            rect[1] = 0x130;
            rect[3] = 0x144;
            rect[2] = ((int)g_game->progress[4] * 7) / 2 + 0xcd;
            FUN_004bf6f0(&scr.gadget, rect, color);
            FUN_004b7f90(&scr.gadget, lightbar, rect[0], rect[1]);
            FUN_004b7f90(&scr.gadget, lightbar, rect[0] + *((short*)lightbar + 2),
                         rect[1] + *((short*)lightbar + 3));
            color = g_game->palette[g_game->progress[5] < 100 ? 12 : 10];
            FUN_004c13a0(color, FUN_004c13f0());
            if(g_game->progress[5] == 100 && DAT_0051e825 != 100) {
                ((unsigned char*)&DAT_0051e6cc)[1] = 0x1e;
            }
            flash = ((unsigned char*)&DAT_0051e6cc)[1];
            DAT_0051e825 = g_game->progress[5];
            FUN_004a50e0(&scr.gadget, (char*)FUN_004c5740("Explosions"), 0x5a, 0x15b, -1, flash);
            rect[0] = 0xcd;
            rect[1] = 0x15b;
            rect[3] = 0x16f;
            rect[2] = ((int)g_game->progress[5] * 7) / 2 + 0xcd;
            FUN_004bf6f0(&scr.gadget, rect, color);
            FUN_004b7f90(&scr.gadget, lightbar, rect[0], rect[1]);
        }
        if (((Class_00435100*)g_game->field_391e9)->FUN_00435100() == 3) {
            FUN_00497ce0(&scr.gadget);
            FUN_00456de0();
        }
        FUN_004c5fa0(&scr.gadget);
        FUN_004c63a0();
    }
    FUN_004b6b50(200);
}
