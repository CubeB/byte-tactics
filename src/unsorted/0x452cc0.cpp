// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
//
// Removes the player with the given id. The id is first resolved to a slot
// through the two inlined index helpers (FUN_0044ffd0 for the "exists" test,
// FUN_0044fe40 for the address), the slot's two 11-byte per-player tables are
// cleared for every still-playing slot, the local player is re-selected with
// FUN_00486f10, and the slot is torn down (optionally sending the
// remove-player packet to the network layer at g_game+0x14).
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
    char unknown_0[0x97];
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
    unsigned char state;               // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char mapA[11];            // +0x108
    unsigned char mapB[11];            // +0x113
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
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x391e9 - 0x2a45];
    Class_00435100* field_391e9;       // +0x391e9
};
#pragma pack(pop)

extern Game_00452cc0* g_game;

int __stdcall FUN_0044ffd0(unsigned char index);
unsigned char __stdcall FUN_0044fe40(int id);
void __stdcall FUN_00486f10(unsigned char player);
int __stdcall FUN_0046c620(int msg);
int __stdcall FUN_004ca780(void* net, unsigned long id);

// Index of the first live slot whose +4 field matches, else 10.
static inline unsigned char FindIndex_00452cc0(int id)
{
    for (unsigned char i = 0; i < 10; i++) {
        if (FUN_0044ffd0(i) == id)
            return i;
    }
    return 10;
}

// +4 field of a live slot, or -1 when the slot is empty (or is the sentinel).
static inline int PlayerId_00452cc0(unsigned char i)
{
    if (i != 10 && g_game->players[i].state != 0)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_00452cc0(int id)
{
    for (unsigned char i = 0; i < 10; i++) {
        if (PlayerId_00452cc0(i) == id)
            return i;
    }
    return 10;
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
    if (p->state != 1 && p->state != 2 && p->state != 3)
        return;

    unsigned char slot = p->field_146;
    if (slot == 10)
        return;

    int flag = p->data->flags & 1;

    for (int i = 0; i < 10; i++) {
        Player_00452cc0* q = &g_game->players[i];
        if (q->active != 0 && (q->state == 1 || q->state == 2)) {
            q->mapA[slot] = 0;
            q->mapB[slot] = 0;
        }
    }

    unsigned char pi;
    if (id == -1)
        pi = 10;
    else
        pi = FindPlayerIndex_00452cc0(id);

    FUN_00486f10(pi);

    if ((g_game->flags_2a44 & 4) != 0) {
        if (p->active != 0 && (p->state == 1 || p->state == 2))
            goto decrement;
    } else if (p->active != 0 && (p->state == 1 || p->state == 2)) {
        FUN_004ca780((char*)g_game + 0x14, p->id);
    }

    ((Class_00463c60*)p)->FUN_00463c60(0);
    p->active = 0;
    p->id = -1;
    p->field_c = 0;

decrement:
    g_game->field_2a3c--;
    p->data->word_9d &= 0xfffb;
    memset(&p->mapA, 0, 11);

    if (g_game->field_391e9->FUN_00435100() == 3)
        FUN_0046c620(3);

    if ((g_game->flags_2a44 & 4) != 0 && flag != 0) {
        unsigned int max = 0;
        Player_00452cc0* q = g_game->players;
        int n = 10;
        do {
            if ((q->active != 0 && q->state == 3)
                || (q->active != 0 && q->state == 1)) {
                if (q->id > max)
                    max = q->id;
            }
            q++;
        } while (--n);
        if (FindPlayerIndex_00452cc0(max) != 10) {
            Player_00452cc0* r = &g_game->players[FindPlayerIndex_00452cc0(max)];
            if (r != 0)
                r->data->flags |= 1;
        }
    }
}
