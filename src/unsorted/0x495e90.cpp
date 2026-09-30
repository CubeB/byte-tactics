// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol and space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// PARTIAL 79.6%: in-game command/gadget event dispatcher, original 2292 bytes,
// ours 2292 (exact size; structure, jump tables and case order agree).
//
// What this pass changed (79.0% -> 79.6%), THE STACK FRAME:
// Both remaining frame-slot differences were ONE cause, the size of the two
// buffers in case 0xd7 and the CTRL buffer in case 0xab. Measured with the
// scratch scorer in build/scratch/0x495e90/score.py (buf size -> frame,
// CTRL buf, findData, path):
//   buf[0x20] path[0xf0]  frame 0x230  CTRL 0x18  findData 0x38  path 0x150
//   buf[0x18] path[0xf0]  frame 0x228  CTRL 0x18  findData 0x30  path 0x148
//   buf[0x10] path[0xf0]  frame 0x220  CTRL 0x10  findData 0x28  path 0x140
//   buf[8]    path[0xf0]  frame 0x224  CTRL 0x10  findData 0x2c  path 0x144
//   buf[0x10] path[0x100] frame 0x230  CTRL 0x10  findData 0x28  path 0x140
// The last row is the original, exactly. MSVC5 sizes the frame as
// (top of the highest local) MINUS 0x10, so a `char path[0xf0]` placed at
// 0x140 only yields `sub esp,0x220`; the original's path buffer is 0x100
// bytes, and `char buf[0x10]` is what lets the CTRL buffer share slot 0x10
// with case 0xf8's `data[4]`. That single pair of sizes fixes the CTRL lea
// (esp+0x18 -> 0x10), the two 0xd7 leas (0x38 -> 0x28, 0x150 -> 0x140), the
// atoi argument lea (0x51 -> 0x41) and keeps the frame at 0x230 at the same
// time. Every diff hunk in the checker that was not a `jmp 0x4965ce` ->
// `jmp 0x4965cf` target shift is gone; see build/scratch/0x495e90/itxt.py,
// which reproduces the checker's instruction-text diff offline.
//
// What the earlier passes changed (75.7% -> 78.8%):
// - Case 0xd7: hoisting `int old = g_game->field_38c53;` ABOVE the
//   `flags_37f2f.b1` guard (semantically the same, the read is unconditional)
//   flips the whole block's allocation. MSVC now keeps g_game in esi and the
//   flag byte in cl, exactly as the original does, and stops sinking the
//   field_38c53 load past the branch. The original really does hoist that load:
//   its live range starts at the block entry even though the guard is first.
//   With the load inside the guard MSVC gives g_game to ecx, the flag byte to
//   al, and emits a 6-byte `mov ecx, g_game` where the original has the 5-byte
//   moffs `mov eax, g_game`. Worth 3.1 points.
//
// What is known to be right:
// - FUN_004c1ab0 returns the event, 0 means return; FUN_004c1b80(0xf9) returns
//   the "key down" flag (kept in esi).
// - The switch is value sorted. MSVC 5 builds a 0xf0-byte index table at
//   0x496694 (index = event - 9) and a 40-entry jump table at 0x4965f4.
//   The jump table is sorted by case value; each non-adjacent case label that
//   shares a body gets its OWN slot even though the bodies fold to one address
//   (0x21/0x23/0x2a/0x60/0x7e all fold to 0x496058; 0xab|0xae..0xbb|0xbd..0xc2
//   all fold to 0x4963d8; 0x2b|0x3d fold to 0x496570; 0x2d|0x5f fold to
//   0x496512). Cases written as one chain (0x31..0x39, 0xc5..0xcd, 0xd2..0xd5,
//   0xe6..0xe9) get one slot.
// - Case bodies in memory are NOT in value order. Physical order starts at
//   0x495ed4 with case 0x1b and ends at 0x496570 with 0x2b/0x3d; writing the
//   switch in that order is what makes the jump table line up.
// - The 0x4963d8 "CTRL_%c" body is sprintf(buf, "CTRL_%c", event - 0x69) then
//   FUN_0048bf30(buf, key). ebp is the event, not a frame pointer (there is no
//   mov ebp,esp), so lea reg,[ebp-0x69] is the character for %c.
// - The `if (key == 0)` blocks are written as `if (key != 0) { then } else`,
//   with the `key != 0` arm first, so the `key == 0` arm is out of line
//   (0x496167 je 0x4961a4). Same trick for the nested `field_2cba != 0` test.
// - The 0x2d/0x5f and 0x2b/0x3d guards are a RAW `if (!(flags_3923b.raw & 2))`
//   (test byte,2), while the tail and 0xec use the `flags_3923b.b1` bitfield
//   (mov al; shr; test). The union in this file keeps both spellings.
// - The 0x496058 body is a 1-bit `!` on a `unsigned short` bitfield, which
//   yields the not/and/xor expand-in-place toggle.
// - Case 0xf8 (0x496099): writing the call as one expression
//   `FUN_00451df0(FUN_0044fdb0(), data, 3)` (no intermediate int) makes MSVC
//   push the literal 3 before the toggle, as the original does.
//
// This pass (deepseek-v4.1) tried the following, all scored with
// check.py against a scratch copy:
//   vA `int old = g_game->field_38c53;` moved INSIDE the guard (the exact
//      original instruction order: pointer load, flag byte, je, old load,
//      store 0, cmp, jne): 2288 bytes, 75.9%. Byte-identical to the original
//      except eax and ecx are swapped: ours `mov ecx,[g_game]` (6 bytes) /
//      byte temp AL / old EAX, original `mov eax,[g_game]` (5 bytes, the moffs
//      form) / byte temp CL / old ECX. So the register pair (pointer, flag
//      byte) is a pure allocation-order tie-break: when the byte temp and
//      `old` coalesce onto one register, that pair takes EAX in ours and the
//      pointer is pushed to ECX, while the original gives the pointer EAX
//      first and the pair takes CL/ECX.
//   vB = current file + `(void)key;` at the top of case 0xd7 (to keep ESI
//      busy and force EAX for the pointer): the no-op is dropped, output is
//      byte-identical to the current file (2292, 79.6%). ESI liveness is not
//      the lever.
//   vC/vD `Game_495e90* g = g_game;` before the guard + `int old` inside:
//      same 2288 / 75.9% and the same ECX/AL/EAX swap as vA.
//   An instruction-text diff of the current file shows our d7 entry emits the
//      `old` load BEFORE `shr cl,1` and the store AFTER `cmp eax,ebx`, so
//      besides the pointer register our store/compare order is also still
//      swapped; both come from the single register-pair decision above.
//
// What still differs (measured with tools/check.py, and an instruction-text
// LCS alignment of both disassemblies, see build/scratch/0x495e90/):
// - There is now exactly ONE delta left in the whole function: case 0xd7's
//   entry is 37 bytes here against 36 in the original, so the code from
//   0x496202 on, and therefore every `jmp 0x4965ce` and the shared break
//   target, sits one byte high. Everything else, including the epilogue,
//   the tail (`push ebp / call FUN_004956c0`), the 0xad block and the two
//   jump tables, is byte for byte the original.
//     original: mov eax,[g_game] / mov cl,[eax+0x37f2f] / shr cl,1 / test cl,1
//               / je / mov ecx,[eax+0x38c53] / mov [eax+0x38c53],ebx
//               / cmp ecx,ebx / jne            (36 bytes)
//     here:     mov esi,[g_game] / mov cl,[esi+0x37f2f] / shr cl,1
//               / test cl,1 / je / mov eax,[esi+0x38c53] / cmp eax,ebx
//               / mov [esi+0x38c53],ebx / jne  (37 bytes)
//   Two things are wrong at once: the pointer needs EAX for the 5-byte moffs
//   form (any other register is 6 bytes), and the store must come BEFORE the
//   compare. The store-before-compare only happens when the `int old` load is
//   inside the guard; the EAX choice only happens when the `int old` load is
//   hoisted above it. Every spelling tried couples them the wrong way:
//     `int old` hoisted (kept, 79.6%)  flag in CL as the original, pointer ESI.
//     `int old` inside the guard       original's order, but `mov ecx,[g_game]`
//                                     and the flag byte in AL, 2288 bytes.
//     local `Game_495e90* g` hoisted, `int old` hoisted   ESI / CL, wrong order.
//     local `Game_495e90* g` inside the guard             ESI / AL, 2260 bytes.
//     `int old` hoisted + a named `unsigned short keep` for the flag: MSVC
//       folds the 1-bit compare into `test dl,2`, a different shape, 2288.
//   Reading either value into a named local (technique 8) does not decouple
//   them either. The reading to try next is a construct that makes the
//   g_game load a value the allocator ranks ABOVE the bitfield byte temp but
//   still lets its web die at the branch, so it never becomes ESI.
// - An older note claimed case 0xad needed +2 bytes at its entry test. That
//   is stale: with the corrected frame the 0xad block is exact.
// - Older notes on this file:
//   (a) Case 0xad entry is `mov eax,[esp+0x20]; cmp esi,eax` where the
//       original has the folded `cmp esi, dword ptr [esp + 0x20]`. STALE.
//       Tried and did not help: a hoisted
//       `std::vector<int>::iterator e = sel.end();`, `sel.end() != it`,
//       `!(it == sel.end())`, `int*` iteration, and a `const&` to sel.
// - Frame slots now agree with the original exactly.
// - Tested with buf[8] in case 0xab: the CTRL buffer then lands at esp+0x10
//   exactly like the original, but the frame drops to 0x224 and the 0xd7/0xad
//   locals stay 4 bytes high. STALE as a dead end: buf[0x10] WITH path[0x100]
//   gets every offset right (see the top of this file). The frame and the slot
//   offsets are two separate constraints, which is why searching buffer sizes
//   on one of them alone kept looking like a dead end.
// - Case 0xec (0x4962f8) loads the guard into al where the original uses dl
//   (`test al,1` is 2 bytes, `test dl,1` is 3). A `char` bitfield base for
//   Flags_00495e90_37f2f was tried and is much worse (69.6%), so the
//   `unsigned short` base is right and the register difference is pure
//   allocator state. Note the ORIGINAL also picks al for the same test in case
//   0x5c and cl in case 0xd7, so the choice is per block, not per expression.
// - The buffer sizes 8, 0x10, 0x18, 0x1c, 0x20 were all tried for `buf` while
//   `path` stayed 0xf0, and no CTRL buffer landed below 0x18. STALE: the CTRL
//   buffer needs BOTH buf[0x10] and path[0x100], because the frame size and
//   the slot offsets are computed independently.
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#pragma pack(push, 1)

class Class_00435100;
class Class_00438760;

struct Sub_495e90 {
    char unknown_0[0x10];
};

struct Flags_00495e90_37f06 {
    unsigned short b0 : 1;      // mask 0x0001
    unsigned short b1_6 : 6;
    unsigned short b7 : 1;      // mask 0x0080
    unsigned short b8 : 1;      // mask 0x0100
    unsigned short rest : 7;
};

struct Flags_00495e90_37ebe {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short rest : 13;
};

struct Flags_00495e90_37f2f {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short rest : 14;
};

struct Flags_00495e90_38a51 {
    unsigned short b0 : 1;
    unsigned short rest : 15;
};

union Flags_00495e90_3923b {
    unsigned short raw;
    struct {
        unsigned short b0 : 1;
        unsigned short b1 : 1;
        unsigned short rest : 14;
    };
};

struct PlayerData_495e90 {
    char unknown_0[0x9b];
    unsigned char field_9b;             // +0x9b
};

struct Player_495e90 {
    int valid;                          // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerData_495e90* data;            // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game_495e90 {
    char unknown_0[0x519];
    Sub_495e90 gui;                     // +0x519
    char unknown_529[0x531 - 0x529];
    int field_531;                      // +0x531
    char unknown_535[0x1b63 - 0x535];
    Player_495e90 players[10];          // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x2c76 - 0x2a43];
    char orders_2c76[0x2cba - 0x2c76];  // +0x2c76
    unsigned short field_2cba;          // +0x2cba
    char unknown_2cbc[0x2cc3 - 0x2cbc];
    unsigned char field_2cc3;           // +0x2cc3
    char unknown_2cc4[0x2cc6 - 0x2cc4];
    unsigned char field_2cc6;           // +0x2cc6
    char unknown_2cc7[0x14280 - 0x2cc7];
    unsigned char field_14280;          // +0x14280
    char unknown_14281[0x37e9c - 0x14281];
    unsigned short field_37e9c;         // +0x37e9c
    char unknown_37e9e[0x37ea0 - 0x37e9e];
    char field_37ea0[0x37ebe - 0x37ea0];
    Flags_00495e90_37ebe flags_37ebe;   // +0x37ebe
    char unknown_37ec0[0x37f06 - 0x37ec0];
    Flags_00495e90_37f06 flags_37f06;   // +0x37f06
    char unknown_37f08[0x37f2f - 0x37f08];
    Flags_00495e90_37f2f flags_37f2f;   // +0x37f2f
    char unknown_37f31[0x38a47 - 0x37f31];
    int field_38a47;                    // +0x38a47
    unsigned short field_38a4b;         // +0x38a4b
    char unknown_38a4d[0x38a51 - 0x38a4d];
    Flags_00495e90_38a51 flags_38a51;   // +0x38a51
    char field_38a53[0x38b53 - 0x38a53];
    char field_38b53[0x38c53 - 0x38b53];
    int field_38c53;                    // +0x38c53
    int unknown_38c57;
    int field_38c5b;                    // +0x38c5b
    char unknown_38c5f[0x391b3 - 0x38c5f];
    int field_391b3;                    // +0x391b3
    unsigned short field_391b7;         // +0x391b7
    int field_391b9;                    // +0x391b9
    unsigned short field_391bd;         // +0x391bd
    char unknown_391bf[0x391e9 - 0x391bf];
    Class_00435100* net;                // +0x391e9
    char unknown_391ed[0x3923b - 0x391ed];
    Flags_00495e90_3923b flags_3923b;   // +0x3923b
};

#pragma pack(pop)

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_00438760 {
public:
    Class_00438760(const char* name);
    char* name;                         // +0
};

extern Game_495e90* g_game;

int FUN_004c1ab0(void);
int __stdcall FUN_004c1b80(int key);
void FUN_0048bd00(void);
void __stdcall FUN_00491d70(int param);
void __stdcall FUN_004a9660(Sub_495e90* gui);
int __stdcall FUN_0049fe60(int handle, const char* text);
void __stdcall FUN_004a6a40(Sub_495e90* gui, int handle);
int __stdcall FUN_004ab060(Sub_495e90* gui, char* name);
void FUN_00430f00(void);
void __stdcall FUN_0047f1a0(const char* name, int param);
void FUN_00494050(void);
void __stdcall FUN_0041bf10(int param);
void __stdcall FUN_0041bde0(int param);
void __stdcall FUN_0041c060(int param);
void __stdcall FUN_0048d9a0(int index, int key);
void __stdcall FUN_0041c2e0(int param);
void __stdcall FUN_00417b50(int param_1, int param_2);
void FUN_004942e0(void);
void FUN_004936f0(void);
void FUN_0048d4d0(void);
void FUN_0048bd50(void);
void FUN_0048be00(void);
void __stdcall FUN_0048bf30(const char* name, int key);
void FUN_0041c310(void);
void FUN_0048c030(void);
void __stdcall FUN_0048ca20(void* param);
void __stdcall FUN_0048d920(int param);
void __stdcall FUN_0041d3b0(int param);
void __stdcall FUN_0041d3f0(int param);
void FUN_00464000(void);
void FUN_00463c80(void);
void __stdcall FUN_00490df0(int param_1, int param_2);
void __stdcall FUN_004ab190(Sub_495e90* gui, int param);
int FUN_0044fdb0(void);
void __stdcall FUN_00451df0(int param_1, void* param_2, int param_3);
void FUN_00495010(void);
void __stdcall FUN_004956c0(int eventType);
void __stdcall FUN_00460cc0(void);
int __stdcall FUN_004bc4b0(char* path, void* findData, int param_3, int param_4);
int __stdcall FUN_004bc640(int handle, void* findData);
void __stdcall FUN_004bc8d0(int handle);
void __stdcall FUN_004bcf00(char* path);
void __stdcall FUN_00468cf0(int param_1, int param_2);
void __stdcall FUN_004cb170(char* param_1, const char* param_2);
void __stdcall FUN_0048cf30(void* a, int b, int c, void* d, int e, void* f);
int __stdcall FUN_00439e30(int unit, int arg);
void __stdcall FUN_00439f80(int unit, int arg);
void __cdecl operator delete(void* p);

// FUNCTION: 0x495e90
void FUN_00495e90(void)
{
    int event = FUN_004c1ab0();
    if (event == 0)
        return;

    int key = FUN_004c1b80(0xf9);

    switch (event) {
    case 0x1b:
        if (g_game->flags_37ebe.b0) {
            g_game->flags_37ebe.b0 = 0;
            int r = FUN_004ab060(&g_game->gui, g_game->field_37ea0);
            if (r == 0) {
                g_game->field_37e9c = 0;
                FUN_004a9660(&g_game->gui);
            }
        } else {
            if (g_game->field_2cc3 != 1) {
                g_game->field_2cc3 = 1;
                g_game->field_2cc6 = g_game->field_2cc6 & 0xdf;
                int handle = FUN_0049fe60(*(int*)(g_game->field_531 + 4), "STOP");
                if (handle != -1)
                    FUN_004a6a40(&g_game->gui, handle);
            } else {
                FUN_0048bd00();
                FUN_00491d70(1);
            }
        }
        break;

    case 0xc5:
    case 0xc6:
    case 0xc7:
    case 0xc8:
    case 0xc9:
    case 0xca:
    case 0xcb:
    case 0xcc:
    case 0xcd:
        FUN_0048d920(event - 0xc4);
        FUN_0047f1a0("CreateSquad", 0);
        break;

    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
        if (g_game->flags_37f06.b8) {
            if (FUN_004c1b80(0xfb) != 0) {
                FUN_0041c060(event - 0x31);
            } else {
                FUN_0048d9a0(event - 0x30, key);
                FUN_0047f1a0("SelectSquad", 0);
            }
        } else {
            if (FUN_004c1b80(0xfb) != 0) {
                FUN_0048d9a0(event - 0x30, key);
                FUN_0047f1a0("SelectSquad", 0);
            } else
                FUN_0041c060(event - 0x31);
        }
        break;

    case 0xd2:
    case 0xd3:
    case 0xd4:
    case 0xd5:
        FUN_0047f1a0("SelectSquad", 0);
        FUN_0041d3b0(event - 0xd2);
        break;

    case 0xe6:
    case 0xe7:
    case 0xe8:
    case 0xe9:
        FUN_0047f1a0("SelectSquad", 0);
        FUN_0041d3f0(event - 0xe6);
        break;

    case 0x21:
    case 0x23:
    case 0x2a:
    case 0x60:
    case 0x7e:
        g_game->flags_37f06.b0 = !g_game->flags_37f06.b0;
        FUN_00430f00();
        break;

    case 0x2c:
        FUN_0041bf10(1);
        break;

    case 0x2e:
        FUN_0041bde0(1);
        break;

    case 0xf8: {
        unsigned char data[4];
        data[0] = 0x19;
        data[1] = 0;
        g_game->flags_38a51.b0 = !g_game->flags_38a51.b0;
        data[2] = (unsigned char)(g_game->flags_38a51.b0);
        FUN_00451df0(FUN_0044fdb0(), data, 3);
        break;
    }

    case 0xe2:
        if (key != 0) {
            if (g_game->field_2cba != 0) {
                g_game->field_391b3 = 1;
                g_game->field_391b7 = g_game->field_2cba;
            } else {
                g_game->field_391b3 = 0;
            }
        } else {
            FUN_004942e0();
        }
        break;

    case 9:
        if (g_game->net->FUN_00435100() == 3) {
            if (!g_game->flags_37ebe.b2)
                FUN_00495010();
            break;
        }
        // fall through
    case 0xe3:
        if (key != 0) {
            if (g_game->field_2cba != 0) {
                g_game->field_391b9 = 1;
                g_game->field_391bd = g_game->field_2cba;
            } else {
                g_game->field_391b9 = 0;
            }
        } else {
            if (!g_game->flags_37ebe.b0) {
                FUN_00460cc0();
                g_game->flags_37ebe.b0 = 1;
            }
        }
        break;

    case 0xe4:
        FUN_00464000();
        break;

    case 0xd7: {
        int old = g_game->field_38c53;
        if (g_game->flags_37f2f.b1) {
            g_game->field_38c53 = 0;
            if (old == 0) {
                char path[0x100];
                char findData[0x118];
                sprintf(path, "%s\\MOVIE*", g_game->field_38a53);
                int h = FUN_004bc4b0(path, findData, -1, 1);
                if (h >= 0) {
                    do {
                        int n = atoi(&findData[0x19]);
                        if (n > g_game->field_38c53)
                            g_game->field_38c53 = n;
                    } while (FUN_004bc640(h, findData) == 0);
                    FUN_004bc8d0(h);
                }
                g_game->field_38c53++;
                sprintf(g_game->field_38b53, "%s\\MOVIE%03i",
                        g_game->field_38a53, g_game->field_38c53);
                FUN_004bcf00(g_game->field_38b53);
                FUN_00468cf0(0, 1);
                FUN_004cb170(g_game->field_38b53, "FRAM");
                g_game->field_38c5b = g_game->field_38a47;
            }
        }
        break;
    }

    case 0xec:
        if (g_game->flags_37f2f.b1) {
            g_game->flags_3923b.b1 = !g_game->flags_3923b.b1;
            if (g_game->flags_3923b.b1) {
                FUN_004ab190(&g_game->gui, 0);
            } else {
                g_game->flags_3923b.b0 = 0;
                g_game->field_14280 = 0;
                FUN_004ab190(&g_game->gui, 1);
            }
        }
        break;

    case 0x5c:
        if (g_game->flags_37f2f.b1)
            FUN_00417b50(0, -1);
        break;

    case 0xe5:
        g_game->flags_37f06.b7 = !g_game->flags_37f06.b7;
        break;

    case 0xed:
        FUN_00463c80();
        break;

    case 0xaa:
        FUN_0048bd50();
        break;

    case 0xab:
    case 0xae:
    case 0xaf:
    case 0xb0:
    case 0xb1:
    case 0xb2:
    case 0xb3:
    case 0xb4:
    case 0xb5:
    case 0xb6:
    case 0xb7:
    case 0xb8:
    case 0xb9:
    case 0xba:
    case 0xbb:
    case 0xbd:
    case 0xbe:
    case 0xbf:
    case 0xc0:
    case 0xc1:
    case 0xc2: {
        char buf[0x10];
        sprintf(buf, "CTRL_%c", event - 0x69);
        FUN_0048bf30(buf, key);
        break;
    }

    case 0xac:
        FUN_0048bf30("CTRL_C", key);
        FUN_0041c310();
        break;

    case 0xc3:
        FUN_0048be00();
        break;

    case 0xad: {
        std::vector<int> sel;
        FUN_0048ca20(&sel);
        int found = 0;
        Class_00438760 order("SELFDESTRUCT");
        for (std::vector<int>::iterator it = sel.begin(); it != sel.end(); ++it) {
            int r = FUN_00439e30(*it, (int)order.name);
            if (r != 0) {
                found = 1;
                FUN_00439f80(*it, r);
            }
        }
        if (found == 0)
            FUN_0048cf30(g_game->orders_2c76, 0, (int)order.name, 0, 0, 0);
        break;
    }

    case 0x68:
        if (g_game->net->FUN_00435100() == 3)
            FUN_004936f0();
        break;

    case 0x6e:
        FUN_0048d4d0();
        break;

    case 0xbc:
        FUN_0048c030();
        break;

    case 0x74:
        FUN_0041c2e0(0);
        break;

    case 0x54:
        FUN_0041c2e0(1);
        break;

    case 0xd:
        FUN_0047f1a0("SmallButton", 0);
        FUN_00494050();
        break;

    case 0x2d:
    case 0x5f:
        if (!(g_game->flags_3923b.raw & 2)) {
            Player_495e90* pl = &g_game->players[g_game->localPlayer];
            if (pl->valid != 0 && (pl->data->field_9b & 0x40) != 0)
                break;
            if (g_game->field_38a4b <= 1)
                break;
            FUN_00490df0(g_game->field_38a4b - 1, 1);
        }
        break;

    case 0x2b:
    case 0x3d:
        if (!(g_game->flags_3923b.raw & 2)) {
            Player_495e90* pl = &g_game->players[g_game->localPlayer];
            if (pl->valid != 0 && (pl->data->field_9b & 0x40) != 0)
                break;
            if (g_game->field_38a4b >= 0x14)
                break;
            FUN_00490df0(g_game->field_38a4b + 1, 1);
        }
        break;

    default:
        break;
    }

    if (g_game->flags_3923b.b1)
        FUN_004956c0(event);
}
