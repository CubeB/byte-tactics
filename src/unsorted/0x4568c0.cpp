// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
//
// PARTIAL, 76.2% (up from 59.1%). The frame is byte exact (0x34 locals, with
// idx/out/ret/k4 sharing the dword at +0x14 as the original has them), the
// player probe, both target-order branches, the candidate collection loop, the
// shuffle inline and the k3 done-scan loop all match in shape. What still
// differs:
//  - the k4 loop keeps its index in a stack slot and uses an element pointer
//    plus a field_29d0 offset where the original has three register inductions
//    (ebx index, esi field_29d0 offset, ebp players byte offset), and it hoists
//    the packet stores past the active test;
//  - `res` ends up in ebp where the original reloads it into edi, and the k3
//    loop then uses edi as its zero constant where the original uses ebx;
//  - the k3 loop's first arm ends in an unconditional jmp where the original
//    re-tests `res`, and the original's first `cmp [eax],0` is not hoisted.
#include <stdlib.h>
#include <algorithm>

#pragma pack(push, 1)
struct PlayerInfo_004568c0 {
    char unknown_0[0x97];
    unsigned short flag_97_0 : 1;            // +0x97 bit 0
    unsigned short rest_97 : 15;
    char unknown_99[0x9b - 0x99];
    unsigned short pad_9b_a : 6;
    unsigned short flag_9b_6 : 1;            // +0x9b bit 6
    unsigned short pad_9b_b : 7;
    unsigned short flag_9b_14 : 1;           // +0x9b bit 14
};

class Class_00456030 {
public:
    int field_0;                             // +0x0
    char unknown_4[0x73 - 0x4];
    char field_73;                           // +0x73
    int FUN_00456030();
};

class Player_004568c0 {
public:
    int active;                              // +0x00
    int id;                                  // +0x04
    char unknown_8[0x27 - 0x8];
    PlayerInfo_004568c0* info;               // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;                     // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;                 // +0x146
    unsigned char field_147;                 // +0x147
    char unknown_148[0x14b - 0x148];
};

struct Game_004568c0 {
    char unknown_0[0x1b63];
    Player_004568c0 players[10];             // +0x1b63
    char unknown_2851[0x29a4 - 0x2851];
    int field_29a4[10];                      // +0x29a4
    char unknown_29cc[0x29d0 - 0x29cc];
    int field_29d0[10];                      // +0x29d0
    char unknown_29f8[0x29fc - 0x29f8];
    int field_29fc[10];                      // +0x29fc
    char unknown_2a24[0x2a28 - 0x2a24];
    int field_2a28;                          // +0x2a28
    char unknown_2a2c[0x2a42 - 0x2a2c];
    unsigned char localPlayer;               // +0x2a42
};
#pragma pack(pop)

extern Game_004568c0* g_game;

int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);
int __stdcall FUN_00451df0(int id, void* packet, int size);

static inline unsigned char FindOccupied_004568c0()
{
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].state != 0 && g_game->players[i].info->flag_97_0)
            return i;
    }
    return 10;
}

static inline int PlayerId_004568c0(unsigned char pi)
{
    int id = -1;
    if (pi != 10 && g_game->players[pi].state != 0)
        id = g_game->players[pi].id;
    return id;
}

// FUNCTION: 0x4568c0
int FUN_004568c0()
{
    unsigned char idx = FindOccupied_004568c0();
    int res = ((Class_00456030*)&g_game->players[idx])->FUN_00456030();
    if (res != 0 && g_game->field_2a28 == 0) {
        int* out = g_game->field_29fc;
        if (g_game->players[g_game->localPlayer].info->flag_9b_14) {
            int n = 0;
            for (int k0 = 0; k0 < 10; k0++) {
                Player_004568c0* q = &g_game->players[k0];
                if (q->active != 0
                    && (q->state == 1 || q->state == 2 || q->state == 3)
                    && q->field_146 != 10
                    && (q->info->flag_9b_6) == 0)
                    out[k0] = n++;
                else
                    out[k0] = -1;
            }
        } else {
            int cand[10];
            for (int z = 0; z < 10; z++)
                cand[z] = -1;
            int n = 0;
            unsigned char* q = &g_game->players[0].state;
            int cnt = 10;
            do {
                Player_004568c0* p = (Player_004568c0*)(q - 0x73);
                if (p->active != 0
                    && (p->state == 1 || p->state == 2 || p->state == 3)
                    && p->field_146 != 10
                    && (p->info->flag_9b_6) == 0) {
                    cand[n] = n;
                    n++;
                }
                q += 0x14b;
            } while (--cnt);
            if (n > 2 || (int)((__int64)rand() * 2 / 0x8000) != 0)
                std::random_shuffle(cand, cand + n);
            int* cp = cand;
            for (int k2 = 0; k2 < 10; k2++) {
                Player_004568c0* q2 = &g_game->players[k2];
                if (q2->active != 0
                    && (q2->state == 1 || q2->state == 2 || q2->state == 3)
                    && q2->field_146 != 10) {
                    if (g_game->players[k2].active != 0 && (q2->info->flag_9b_6))
                        out[k2] = -1;
                    else
                        out[k2] = *cp++;
                } else {
                    out[k2] = -1;
                }
            }
        }
        g_game->field_2a28 = 1;
    }
    int ret = 1;
    for (int k3 = 0; k3 < 10; k3++) {
        Player_004568c0* q = &g_game->players[k3];
        if (q->active != 0 && q->state == 3) {
            if (res != 0) {
                if (g_game->field_29a4[k3] == 0 || g_game->field_29d0[k3] == 0) {
                    ret = 0;
                    break;
                }
            } else {
                if (g_game->field_29a4[k3] == 0) {
                    ret = 0;
                    break;
                }
            }
        }
    }
    if (res != 0) {
        for (unsigned char k4 = 0; k4 < 10; k4++) {
            Game_004568c0* g = g_game;
            if (g->field_29d0[k4] == 0) {
                unsigned char packet[2];
                packet[0] = 0x1e;
                packet[1] = (unsigned char)g->field_29fc[k4];
                Player_004568c0* q = &g->players[k4];
                if (q->active != 0) {
                    if (q->state == 3) {
                        int to = PlayerId_004568c0(k4);
                        int from = -1;
                        unsigned char* sp = &g->players[0].state;
                        for (int j = 0; j < 10; j++, sp += 0x14b) {
                            if (*sp == 1) {
                                from = g->players[j].id;
                                break;
                            }
                        }
                        FUN_00451bc0(from, to, packet, 2);
                    } else if (q->active != 0 && (q->state == 1 || q->state == 2)) {
                        q->field_147 = packet[1];
                        g->field_29d0[k4] = 1;
                    }
                }
            }
        }
    }
    unsigned char pkt = 0x15;
    if (res != 0) {
        if (ret != 0) {
            for (int k5 = 0; k5 < 10; k5++) {
                Player_004568c0* q = &g_game->players[k5];
                if (q->active != 0 && (q->state == 1 || q->state == 2))
                    FUN_00451df0(PlayerId_004568c0(k5), &pkt, 1);
            }
        }
        return ret;
    }
    for (int k6 = 0; k6 < 10; k6++) {
        Player_004568c0* q = &g_game->players[k6];
        if (q->active != 0 && (q->state == 1 || q->state == 2))
            FUN_00451df0(PlayerId_004568c0(k6), &pkt, 1);
    }
    return ret;
}
