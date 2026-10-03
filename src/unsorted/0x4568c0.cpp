// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by
// (line 1 continued), finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash
// deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash
// (notes only, code unchanged at 86.8), finished by claude-opus-5-5. Names are provisional.
//
// Partial, 90.9% (99.0% with jump targets ignored), 1307 of 1310 bytes.
//
// claude-opus-5-5 (#4992) rewrote the body without the earlier tricks (the
// result flag merged into the `out` pointer as `(int*)1`, the hand-expanded
// `to` lookup). With separate `int* out` (if block) and `int ok` (before the
// readiness loop) MSVC still shares [esp+0x14] between the FindOccupied
// counter, out and ok, and the frame is the original 0x34. What placed the
// registers:
//  - the k4 loop skips with `continue` (`if (field_29d0[k4] != 0) continue;`);
//    nesting the body under `if (field_29d0[k4] == 0)` keeps `ok` in ebp and
//    loses the zero register (ebx) of the readiness loop (82.0);
//  - the tail is `if (res == 0) {...} else if (ok) {...} return ok;`; two
//    separate returns put ok in edi in the tail (83.6);
//  - the k4 send sits in a do/while(0) (SEND_TO_PLAYER below). Written plainly
//    MSVC gives res ebp and the zero edi (84.6); the original has res in edi,
//    the zero in ebx, the k4 player offset in ebp and `to` in edi, which this
//    reproduces. A block, an inline helper, `for (;;) { ...; break; }` or a
//    do/while(0) around anything larger do not.
// Declaring ok volatile (scratch only) also fixes the readiness loop, which
// is how the lever was found. The k2 loop reads the shuffled candidates by
// index (`cand[j++]`); a walking `int* cp` swaps the two setup instructions.
//
// Still different: the PlayerId lookup inside SEND_TO_PLAYER addresses
// [edx+eax+K] where the original first copies g_game into the result
// register (`mov edi, edx; add edi, eax`, as the k6/k5 copies do with edx);
// this is the 3-byte shortfall. A scratch function shows the do/while(0)
// region itself causes it (the same send without it copies the base), so the
// original probably got this colouring some other way and the macro is a
// stand-in: something that keeps res in edi and the readiness zero in ebx
// without a loop region around the send is the next thing to find. Rewriting
// PlayerId (Player* local, else-return, inverted test, nested ifs), moving
// the lookup or the from scan out of the region, a block, an inline helper,
// `for (;;) { ...; break; }`, `while (1)`, `switch (0)` and `if (1)` did not
// get there.
// The macro is also fragile: with the real preceding function (the empty
// FUN_004568b0, or FUN_00456850 defined and called instead of the inline
// FindOccupied, or the real FUN_0044ffd0/FUN_0044fe00 lookups) defined above
// this one, every variant here drops to 84.6 and only a volatile ok reaches
// 87.8. The number of declarations before the function (0..31 dummy externs)
// does not matter.
#include <stdlib.h>
#include <algorithm>

#pragma pack(push, 1)
struct PlayerInfo_004568c0 {
    char unknown_0[0x97];
    unsigned short flag_97_0 : 1;
    unsigned short rest_97 : 15;
    char unknown_99[0x9b - 0x99];
    unsigned short pad_9b_a : 6;
    unsigned short flag_9b_6 : 1;
    unsigned short pad_9b_b : 7;
    unsigned short flag_9b_14 : 1;
};

class Class_00456030 {
  public:
    int field_0;
    char unknown_4[0x73 - 0x4];
    char field_73;
    int FUN_00456030();
};

class Player_004568c0 {
  public:
    int active;
    int id;
    char unknown_8[0x27 - 0x8];
    PlayerInfo_004568c0* info;
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;
    unsigned char field_147;
    char unknown_148[0x14b - 0x148];
};

struct Game_004568c0 {
    char unknown_0[0x1b63];
    Player_004568c0 players[10];
    char unknown_2851[0x29a4 - 0x2851];
    int field_29a4[10];
    char unknown_29cc[0x29d0 - 0x29cc];
    int field_29d0[10];
    char unknown_29f8[0x29fc - 0x29f8];
    int field_29fc[10];
    char unknown_2a24[0x2a28 - 0x2a24];
    int field_2a28;
    char unknown_2a2c[0x2a42 - 0x2a2c];
    unsigned char localPlayer;
};
#pragma pack(pop)

extern Game_004568c0* g_game;

int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);
int __stdcall FUN_00451df0(int id, void* packet, int size);

static inline unsigned char FindOccupied_004568c0() {
    for (unsigned char i = 0; i < 10; i++) {
        if (g_game->players[i].state != 0 && g_game->players[i].info->flag_97_0)
            return i;
    }
    return 10;
}

static inline int PlayerId_004568c0(unsigned char pi) {
    if (pi != 10 && g_game->players[pi].state != 0)
        return g_game->players[pi].id;
    return -1;
}

static inline int FirstJoinedId_004568c0() {
    for (int j = 0; j < 10; j++) {
        if (g_game->players[j].state == 1)
            return g_game->players[j].id;
    }
    return -1;
}

static inline int IsConnected_004568c0(Player_004568c0* p) {
    return p->active != 0 && (p->state == 1 || p->state == 2);
}

// A statement macro in the usual do/while(0) form. The loop emits no code,
// but it is load-bearing for register allocation (see the notes above): the
// same call written out plainly scores 84.6 with res in ebp instead of edi.
#define SEND_TO_PLAYER(k, packet)                                                   \
    do {                                                                            \
        FUN_00451bc0(FirstJoinedId_004568c0(), PlayerId_004568c0(k), (packet), 2); \
    } while (0)

// FUNCTION: 0x4568c0
int FUN_004568c0() {
    unsigned char idx = FindOccupied_004568c0();
    int res = ((Class_00456030*)&g_game->players[idx])->FUN_00456030();
    if (res != 0 && g_game->field_2a28 == 0) {
        int* out = g_game->field_29fc;
        if (g_game->players[g_game->localPlayer].info->flag_9b_14) {
            int n = 0;
            for (int k0 = 0; k0 < 10; k0++) {
                Player_004568c0* q = &g_game->players[k0];
                if (q->active != 0 && (q->state == 1 || q->state == 2 || q->state == 3) &&
                    q->field_146 != 10 && (q->info->flag_9b_6) == 0)
                    out[k0] = n++;
                else
                    out[k0] = -1;
            }
        } else {
            int cand[10];
            for (int z = 0; z < 10; z++)
                cand[z] = -1;
            int n = 0;
            for (int k1 = 0; k1 < 10; k1++) {
                Player_004568c0* p = &g_game->players[k1];
                if (p->active != 0 && (p->state == 1 || p->state == 2 || p->state == 3) &&
                    p->field_146 != 10 && (p->info->flag_9b_6) == 0) {
                    cand[n] = n;
                    n++;
                }
            }
            if (n > 2 || (int)((__int64)rand() * 2 / 0x8000) != 0)
                std::random_shuffle(cand, cand + n);
            int j = 0;
            for (int k2 = 0; k2 < 10; k2++) {
                Player_004568c0* q2 = &g_game->players[k2];
                if (q2->active != 0 && (q2->state == 1 || q2->state == 2 || q2->state == 3) &&
                    q2->field_146 != 10) {
                    if (g_game->players[k2].active != 0 && (q2->info->flag_9b_6))
                        out[k2] = -1;
                    else
                        out[k2] = cand[j++];
                } else {
                    out[k2] = -1;
                }
            }
        }
        g_game->field_2a28 = 1;
    }
    int ok = 1;
    for (int k3 = 0; k3 < 10; k3++) {
        Player_004568c0* q = &g_game->players[k3];
        if (q->active != 0 && q->state == 3) {
            if (res != 0) {
                if (g_game->field_29a4[k3] == 0 || g_game->field_29d0[k3] == 0) {
                    ok = 0;
                    break;
                }
            }
            if (res == 0) {
                if (g_game->field_29a4[k3] == 0) {
                    ok = 0;
                    break;
                }
            }
        }
    }
    if (res != 0) {
        for (int k4 = 0; k4 < 10; k4++) {
            if (g_game->field_29d0[k4] != 0)
                continue;
            unsigned char packet[2];
            packet[0] = 0x1e;
            packet[1] = (unsigned char)g_game->field_29fc[k4];
            if (g_game->players[k4].active != 0) {
                if (g_game->players[k4].state == 3) {
                    SEND_TO_PLAYER(k4, packet);
                } else if (IsConnected_004568c0(&g_game->players[k4])) {
                    g_game->players[k4].field_147 = packet[1];
                    g_game->field_29d0[k4] = 1;
                }
            }
        }
    }
    unsigned char pkt = 0x15;
    if (res == 0) {
        for (int k6 = 0; k6 < 10; k6++) {
            if (IsConnected_004568c0(&g_game->players[k6]))
                FUN_00451df0(PlayerId_004568c0(k6), &pkt, 1);
        }
    } else if (ok) {
        for (int k5 = 0; k5 < 10; k5++) {
            if (IsConnected_004568c0(&g_game->players[k5]))
                FUN_00451df0(PlayerId_004568c0(k5), &pkt, 1);
        }
    }
    return ok;
}
