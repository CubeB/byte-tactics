// Decompiled by space-bunny-free, finished by muse-spark-1.3-free. Names are provisional.
//
// Removes a player (a "drop" / disconnect path): find the player's slot, bail
// out if the slot is not a local, active player of type 1, 2 or 3, then clear
// the two per-team tables of every local player at that player's team number,
// tell the network layer, clear the slot and hand leadership to the highest
// numbered remaining local player.
//
// Best match so far: 83.6% with a top-tested while(1) clearing loop. Every
// instruction sequence matches except (a) the callee-saved register rotation
// in the first half (original has p in esi, slot byte in edi, g_game base in
// ebx; this file has p in ebx, slot in esi, base in edi), (b) the flag load
// uses edx (mov dl / and edx,1) where the original uses eax (xor eax,eax /
// mov al / and eax,1), and (c) the clearing-loop latch (top-tested cmp/jge
// plus jmp back, the original is bottom-tested add/cmp/jl). The tail
// (network call, b2 branch, best-id loop, all three FindPlayer scans) matches
// exactly, so (c) and the b2 branch shape are knock-ons of (a), not separate
// problems.
//
// Diagnosis for whoever picks this up:
// - Referencing p->field_146 inside the clearing loop (either store) DOES put
//   p in esi, slot in edi and base in ebx (scratch z01/z02/f01/f02), but the
//   extra dereference always emits a per-iteration reload (the q stores may
//   alias p, so no CSE) and the loop body register roles swap. A bare
//   (void)p in the loop, (void)p before the loop, and slot + (p - p) as index
//   all fold away with no effect, so only a real load through p moves it.
// - Moving the slot init inside the loop (so the p reference hoists) does not
//   hoist: MSVC keeps the per-iteration load and also hoists constant 3 into
//   ebx (scratch s01/s03). A Game* game = g_game local takes esi but leaves p
//   in ebx (scratch w05/b02). A data-pointer local, split inits, flag-first
//   order, p ternary, ClearSlot/IsActive12/PlayerAt/GetSlot/HasFlag inline
//   helpers, and extra Windows/C headers all leave the rotation unchanged.
// - for-form clearing loop plus the mixed index miscompiles the stride to
//   0x296 (5 iterations, semantically wrong, scratch f01/f02); while-form
//   plus mixed keeps 0x14b (scratch z01, 75.1%). The for-form with plain slot
//   indexing has the right loop shape but the wrong tail (scan block order),
//   which suggests the tail order is also a knock-on of the rotation.
// - Prepending the matched preceding function 0x452c40 changes nothing, so
//   the state is not from the previous function in the exe.
// The natural construct that references p in the loop region with zero
// emitted code (or otherwise promotes p above slot) is still unknown.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_00463c60 {
public:
    void FUN_00463c60(int param_1);
};

struct PlayerData_00452cc0 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
    char unknown_95[0x97 - 0x95];
    unsigned char flags;               // +0x97
    char unknown_98[0x9d - 0x98];
    unsigned short word_9d;            // +0x9d
};

struct Player_00452cc0 {
    int active;                        // +0x00
    unsigned int id;                   // +0x04
    char unknown_8[0xc - 8];
    int field_c;                       // +0x0c
    char unknown_10[0x27 - 0x10];
    PlayerData_00452cc0* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char allies[0xb];         // +0x108
    unsigned char field_113[0xb];      // +0x113
    char unknown_11e[0x146 - 0x11e];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_00452cc0 {
    char unknown_0[0x14];
    char unknown_14[0x1b63 - 0x14];
    Player_00452cc0 players[10];       // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short field_2a3c;         // +0x2a3c
    char unknown_2a3e[0x2a44 - 0x2a3e];
    union {
        unsigned short value;          // +0x2a44
        struct {
            unsigned short b0 : 1;
            unsigned short b1 : 1;
            unsigned short b2 : 1;
            unsigned short b3 : 1;
            unsigned short rest : 12;
        };
    } flags;
    char unknown_2a45[0x391e9 - 0x2a46];
    Class_00435100* net;               // +0x391e9
};
#pragma pack(pop)

extern Game_00452cc0* g_game;

int __stdcall FUN_0044ffd0(unsigned char index);
unsigned char __stdcall FUN_0044fe40(int id);
void __stdcall FUN_00486f10(unsigned char player);
void __stdcall FUN_0046c620(int msg);
int __stdcall FUN_004ca780(void* net, int id);

static inline unsigned char FindIndex_00452cc0(int id)
{
    for (unsigned char i = 0; i < 10; i++) {
        if (FUN_0044ffd0(i) == id)
            return i;
    }
    return 10;
}

static inline int PlayerId_00452cc0(unsigned char i)
{
    if (i != 10 && g_game->players[i].type != 0)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_00452cc0(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (PlayerId_00452cc0(i) == id)
                return i;
        }
    }
    return 10;
}

static inline int IsActive12_00452cc0(Player_00452cc0* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 1 || p->type == 2)
        return 1;
    return 0;
}

static inline int IsType1_00452cc0(Player_00452cc0* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 1)
        return 1;
    return 0;
}

static inline int IsType3_00452cc0(Player_00452cc0* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 3)
        return 1;
    return 0;
}

// FUNCTION: 0x452cc0
void __stdcall FUN_00452cc0(int id)
{
    unsigned char fi;
    if (id == -1)
        fi = 10;
    else
        fi = FindIndex_00452cc0(id);

    Player_00452cc0* p;
    if (fi == 10)
        p = 0;
    else
        p = &g_game->players[FUN_0044fe40(id)];

    if (p == 0)
        return;
    if (p->active == 0)
        return;
    if (p->type != 1 && p->type != 2 && p->type != 3)
        return;
    if (p->field_146 == 10)
        return;

    unsigned char slot = p->field_146;
    int flag = p->data->flags & 1;

    int i = 0;
    while (1) {
        if (i >= 10)
            break;
        Player_00452cc0* q = &g_game->players[i];
        if (q->active != 0 && (q->type == 1 || q->type == 2)) {
            q->field_113[slot] = 0;
            q->allies[slot] = 0;
        }
        i++;
    }

    FUN_00486f10(FindPlayerIndex_00452cc0(id));

    if (g_game->flags.b2) {
        if (p->active != 0 && (p->type == 1 || p->type == 2))
            goto after_remove;
    } else if (p->active != 0 && (p->type == 1 || p->type == 2)) {
        FUN_004ca780((char*)g_game + 0x14, p->id);
    }
    ((Class_00463c60*)p)->FUN_00463c60(0);
    p->active = 0;
    p->id = -1;
    p->field_c = 0;

after_remove:
    g_game->field_2a3c--;
    p->data->word_9d &= 0xfffb;
    memset(&p->allies, 0, 11);

    if (g_game->net->FUN_00435100() == 3)
        FUN_0046c620(3);

    if ((g_game->flags.value & 4) && flag != 0) {
        unsigned int best = 0;
        Player_00452cc0* q = g_game->players;
        int n = 10;
        do {
            if ((q->active != 0 && q->type == 3)
                || (q->active != 0 && q->type == 1)) {
                if (q->id > best)
                    best = q->id;
            }
            q++;
        } while (--n);
        Player_00452cc0* r;
        if (FindPlayerIndex_00452cc0(best) == 10)
            r = 0;
        else
            r = &g_game->players[FindPlayerIndex_00452cc0(best)];
        if (r != 0)
            r->data->flags |= 1;
    }
}
