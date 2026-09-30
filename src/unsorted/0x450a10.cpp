// Decompiled by DeepSeek V4.1 Flash, finished by Space Bunny Free, retried by deepseek-v4.1-flash, retried by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// #1513 retry by Codex / GPT-6.1-sol: checkall reconfirmed 87.6% (837/872 bytes).
// Prior retry variants in this file and build/scratch/450a10 still give the best result.
// PARTIAL 87.6%, 837 vs 872 bytes. Everything outside the name-copy block now
// matches, including the free-slot search (writing that loop as a while loop with
// a separate "s = i; if (!found) s = 10;" step puts the counter in ecx and the
// walking pointer in eax, as the original does).
//
// What still differs, all inside the two strcpy pairs at 0x450b7e:
//  1. MSVC tail-merges the network path's second strcpy with the COMPUTER path's
//     second strcpy (both end in an identical "mov edi, <src> .. rep movsb").
//     The original keeps them apart because the COMPUTER path writes edx
//     ("xor edx, edx", the sunk "result = 0;") just before its final rep movsb.
//     Writing the check as "int result = 0;" with
//     "if (result == 0) { strcpy; strcpy; }" and "result = 0;" in the else branch
//     does reproduce that block layout, the doubled "test edx, edx" gate and the
//     frame size, but it drops to 81.2% because the destination address then
//     spills (see 2). Not net better, so the merged form is kept here.
//  2. The inlined strcpy puts the destination address in a hoisted temp and moves
//     it into edi ("lea edx, [ebx + 0x49] .. mov edi, edx"). The original writes
//     "lea edi, [ebx + 0x49]" straight into the copy loop. Tried: naming the
//     pointer and the result differently, moving the pointer declaration, taking
//     its address in the fullName/name expressions, and putting each strcpy pair
//     in a static inline helper; every one compiles to the same hoisted temp.
//     About 30 source shapes were scored under build/scratch/450a10.
//  3. Because the COMPUTER branch is tail-merged away, the original's
//     "mov edx, ecx" length save, its "xor eax, eax" between the two network
//     copies and the "mov edx, eax" that parks the call result in edx never
//     appear here either.
//
// Retry (deepseek-v4.1-flash) confirmed the tradeoff and added:
//  - Dropping the redundant "else { result = 0; }" (the plain if/else form the
//    binary came from) makes the network path keep the call result in edx
//    ("mov edx, eax; test edx, edx; jne") exactly like the original and stops
//    the network tail merge, but the inlined strcpy then materialises a 4-byte
//    destination temp at [esp+0x14] (frame 0x4c8 instead of 0x4c4), which
//    shifts every stack offset and scores 81.2%.
//  - "result = 0;" before the COMPUTER copies scores 80.5%.
//  - Naming the two source pointers in locals scores 80.7%.
//  - tools/headers.py tried all 128 header sets on the plain form; none match,
//    best is still 81.2% with <windows.h>.
// The remaining gap is one register-allocation decision: the original evaluates
// the strcpy source (into edi) before the destination (lea edi,[ebx+..]); ours
// hoists the destination into a temp and spills it.
//
// Second pass (space-bunny-free, #1814) re-derived the two shapes the original
// could have come from and scored both, so nobody repeats them:
//  A. The semantically right shape is
//       result = FUN_004ca7c0(...); if (result == 0) { copy; copy; }
//       else-branch of param_1 == -1: copy; copy; result = 0;
//       if (result) return 1;                      (one gate, after the join)
//     It reproduces the original's flow exactly (mov edx,eax; test edx,edx;
//     jne end; ... jmp join; the doubled gate at the join, and the COMPUTER
//     path's length save in edx), but MSVC 5 hoists both destination leas
//     above the strlen and spills the first one through [esp+0x14], so the
//     frame is 0x4c8, every stack offset moves by 4 and buf lands at
//     [esp+0xd8]: 81.2% (898 bytes).
//  B. `if (result != 0) return 1;` before the two network copies (the early
//     return) also keeps the copies in one block, but then the compiler knows
//     result == 0 on that path, so the join gate at 0x450c0e disappears and
//     the network second strcpy tail merges with the COMPUTER first one:
//     87.3% (841 bytes), one byte worse than what is kept here.
//  The file therefore keeps the old 87.6% form. A spill for the strcpy
//  destination is the whole remaining gap: get the address into edi at the
//  copy and nothing else is left to fix.
// #2104 (deepseek-v4.1, 10 minute box): no gain over 87.6%. Scored the two
// fresh shapes and left the file as it was:
//  A. early return `if (result != 0) return 1;` in front of the network copies:
//     841 bytes, 87.3%. Frame stays 0x4c4 and the three leas schedule exactly
//     like the original, but MSVC then knows result == 0 at the join, so the
//     shared gate `test edx,edx` at 0x450c0e and the `jmp 0x450c0e` both
//     disappear and the network second strcpy tail-merges with the COMPUTER
//     first one. A redundant second `if (result != 0) return 1;` after the
//     copies is elided too, same 841 bytes.
//  B. plain `if (result == 0) { copies }` plus the joint gate: 898 bytes,
//     81.2%. Flow matches the original instruction for instruction
//     (mov edx,eax / test edx,edx / jne epilogue / copies / jmp join /
//     COMPUTER's sunk xor edx,edx before its last rep movsb), but the first
//     network strcpy schedules its destination lea into the repne scasb shadow
//     while edi is still the scan cursor, so the lea lands in esi, collides
//     with the source pointer, and MSVC spills it to the first free 4-byte
//     slot (0x14, ahead of `size`), which makes the frame 0x4c8 and shifts
//     every stack offset by 4.
//  Tried against B: declarations of size/packet/buf/result hoisted to the top
//     of the function (temp still gets 0x14), and `&p->fullName[0]` /
//     `&p->name[0]` destinations (identical 898 bytes). The blocked step is
//     that single scheduler/allocator decision in the first network strcpy.
//
// #2452 retry (deepseek-v4.1-flash): found a better shape, now 90.5% (870/872).
// Both paths keep the semantically right form (result live in edx across the
// network copies, one gate after the join). The four copies now go through a
// single static inline helper written as strlen + memcpy:
//
//     static inline void CopyStr_00450a10(char* d, const char* s) {
//         unsigned long n = (unsigned long)strlen(s) + 1;
//         memcpy(d, s, n);
//     }
//
// That is what gets the destination computed late: MSVC's strlen+memcpy chain
// emits `mov esi,<src>; mov edi,esi; repne scasb; ...; lea edi,[dst]` so the
// destination lea lands directly in edi (no hoisted temp, no spill), and the
// frame stays 0x4c4. Calling strcpy directly always hoists the destination lea
// one slot early (into edx when free, into esi and spilled when edx holds
// result), which is the old 87.6%/81.2% split. Tried and scored: strcpy direct
// in every destination spelling, a helper that calls strcpy, a destination
// helper returning p->fullName, (void) casts, comma forms, and a source local;
// all hoist and stay at 81.2%.
// What still differs (2 bytes, 870 vs 872): the strlen+memcpy fusion keeps the
// source in esi from the start, so each copy reads
//     mov esi,[src]; or ecx,-1; mov edi,esi; repne scasb; not ecx; mov eax,ecx;
//     lea edi,[dst]; ...
// where the original strcpy intrinsic reads
//     mov edi,[src]; or ecx,-1; repne scasb; not ecx; sub edi,ecx; mov eax,ecx;
//     mov esi,edi; lea edi,[dst]; ...
// i.e. the original saves 4 bytes (sub+mov esi) and spends 2 (mov edi,esi),
// net 2 per copy. The blocked step is getting MSVC to keep the canonical
// strcpy scan form while still evaluating the destination last.

#include <string.h>
#include <windows.h>

class Class_00435100;

#pragma pack(push, 1)
struct PlayerData_00450a10 {
    char unknown_0[0x90];
    int id;                            // +0x90
    char unknown_94[0xb9 - 0x94];
};

struct Player_00450a10 {
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0x1c - 8];
    int field_1c;                      // +0x1c
    char unknown_20[0x22 - 0x20];
    unsigned char field_22;            // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerData_00450a10* data;         // +0x27
    char name[0x49 - 0x2b];            // +0x2b
    char fullName[0x73 - 0x49];        // +0x49
    unsigned char type;                // +0x73
    char unknown_74[0x13f - 0x74];
    unsigned char alliance;            // +0x13f
    char unknown_140[0x14b - 0x140];
};

struct Game_00450a10 {
    char unknown_0[0x14];
    char net[0x1b63 - 0x14];           // +0x14
    Player_00450a10 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    unsigned char* buffer;             // +0x2a38
    unsigned short count;              // +0x2a3c
    char unknown_2a3e[0x2a44 - 0x2a3e];
    unsigned short flags_2a44;         // +0x2a44
    char unknown_2a46[0x391e9 - 0x2a46];
    Class_00435100* campaign;          // +0x391e9
};

struct Packet_00450a10 {
    unsigned char type;                // +0x00
    PlayerData_00450a10 data;          // +0x01
};
#pragma pack(pop)

struct DPNAME {
    unsigned long dwSize;
    unsigned long dwFlags;
    char* lpszShortNameA;
    char* lpszLongNameA;
};

class Class_004618a0 {
public:
    int FUN_004618a0(int param_1);
};

class Class_00435100 {
public:
    int FUN_00435100();
};

extern Game_00450a10* g_game;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

int __stdcall FUN_004ca7c0(void* net, unsigned long id, void* data, unsigned long* size);
int __stdcall FUN_00451df0(int player, void* data, int size);
void __stdcall FUN_00464290(unsigned char player, char type);
void FUN_00450530();
int FUN_004b6340();
void __stdcall FUN_0046c620(int param_1);

static inline unsigned char FindSlot_00450a10(int id)
{
    unsigned char i;
    for (i = 0; i < 10; i++) {
        int v;
        if (i == 10) {
            v = -1;
        } else {
            v = g_game->players[i].type ? g_game->players[i].id : -1;
        }
        if (v == id) {
            return i;
        }
    }
    return 10;
}

static inline void CopyStr_00450a10(char* d, const char* s)
{
    unsigned long n = (unsigned long)strlen(s) + 1;
    memcpy(d, s, n);
}

// FUNCTION: 0x450a10
int __stdcall FUN_00450a10(int param_1)
{
    unsigned char slot;
    if (param_1 == -1) {
        slot = 10;
    } else {
        slot = FindSlot_00450a10(param_1);
    }
    int flag = 0;
    if (slot != 10) {
        if (g_game->players[slot].type != 1 && g_game->players[slot].type != 2) {
            flag = 1;
        }
        if (g_game->players[slot].active != 0) {
            flag |= 1;
        }
    } else {
        int found = 0;
        int i = 0;
        while (i < 10) {
            Player_00450a10* q = &g_game->players[i];
            if (q->active == 0 && q->type != 4) {
                found = 1;
                break;
            }
            i++;
        }
        int s = i;
        if (!found) {
            s = 10;
        }
        slot = s;
        if (slot == 10) {
            flag = 1;
        } else {
            g_game->players[slot].type = 3;
        }
    }
    Player_00450a10* p = &g_game->players[slot];
    if (flag) {
        return 1;
    }
    unsigned long size;
    Packet_00450a10 packet;
    char buf[0x400];
    int result;
    if (param_1 != -1) {
        size = 0x400;
        result = FUN_004ca7c0(g_game->net, param_1, buf, &size);
        if (result == 0) {
            CopyStr_00450a10(p->fullName, ((DPNAME*)buf)->lpszShortNameA);
            CopyStr_00450a10(p->name, ((DPNAME*)buf)->lpszLongNameA);
        }
    } else {
        CopyStr_00450a10(p->fullName, "COMPUTER");
        CopyStr_00450a10(p->name, "COMPUTER");
        result = 0;
    }
    if (result) {
        return 1;
    }
    FUN_00464290(slot, g_game->players[slot].type);
    p->field_22 = 0;
    p->id = param_1;
    p->field_1c = FUN_004b6340();
    g_game->count++;
    if (g_game->flags_2a44 & 1) {
        for (int i = 0; i < 10; i++) {
            Player_00450a10* q = &g_game->players[i];
            if (q->active != 0 && (q->type == 1 || q->type == 2)) {
                packet.data = *q->data;
                packet.data.id = q->id;
                packet.type = 0x20;
                FUN_00451df0(q->id, &packet, sizeof(packet));
                if (q->active != 0 && (q->type == 1 || q->type == 2)) {
                    unsigned char* msg = g_game->buffer;
                    msg[0] = 0x24;
                    *(int*)(msg + 1) = q->id;
                    msg[5] = q->alliance;
                    FUN_00451df0(q->id, msg, 6);
                    if (DAT_00506dbc != 0) {
                        DAT_00513000.FUN_004618a0(1);
                    }
                }
            }
        }
        FUN_00450530();
        DAT_00513000.FUN_004618a0(1);
    }
    if (g_game->campaign->FUN_00435100() == 3 && g_game->count > 1) {
        FUN_0046c620(2);
    }
    return 1;
}
