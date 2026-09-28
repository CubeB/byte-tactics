// Decompiled by DeepSeek V4.1 Flash, finished by Space Bunny Free. Names are provisional.
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
            strcpy(p->fullName, ((DPNAME*)buf)->lpszShortNameA);
            strcpy(p->name, ((DPNAME*)buf)->lpszLongNameA);
        }
        else {
            result = 0;
        }
    } else {
        strcpy(p->fullName, "COMPUTER");
        strcpy(p->name, "COMPUTER");
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
