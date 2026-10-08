// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by claude-opus-5-5. Names are provisional.
// Removes the player with the given id: clears its slot in every playing
// player's two alliance tables, tells KillPlayerUnits, drops it from the
// session (or only resets it when the 0x2a44 bit 2 mode keeps playing
// players), and when that player was the host (bit 0 of +0x97) hands the
// host bit to the type 3 or type 1 player with the highest id.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

class Player {
public:
    void SetType(int param_1);
};

#include "../map/mission.h"

struct PlayerInfo {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97, bit 0: host
    char unknown_98[0x9d - 0x98];
    unsigned short word_9d;            // +0x9d
};

struct Player_00452cc0 {
    int active;                        // +0x00
    unsigned int id;                   // +0x04
    char unknown_8[0xc - 8];
    int lobbyDataSynced;               // +0x0c
    char unknown_10[0x27 - 0x10];
    PlayerInfo* data;                  // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char allies[0xb];         // +0x108
    unsigned char alliedBy[0xb];       // +0x113
    char unknown_11e[0x146 - 0x11e];
    unsigned char index;               // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x14];
    char unknown_14[0x1b63 - 0x14];
    Player_00452cc0 players[10];       // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short numPlayers;         // +0x2a3c
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
    char unknown_2a46[0x391e9 - 0x2a46];
    Mission* net;                      // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall KillPlayerUnits(unsigned char player);
void __stdcall ReportGameEvent(int msg);
int __stdcall HAPINET_removeplayer(void* net, int id);

// The three lookups below stay defined here without FUNCTION lines, left to /Ob2.
// 0x44ffd0 (matched in its own file).
int __stdcall GetSlotDpid(unsigned char index)
{
    if (index != 10 && g_game->players[index].type != 0)
        return g_game->players[index].id;
    return -1;
}

// 0x44fe40 (matched in its own file).
unsigned char __stdcall FindSlotByDpid(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (GetSlotDpid(i) == id)
                return i;
        }
    }
    return 10;
}

// 0x44fed0 (matched in its own file).
Player_00452cc0* __stdcall FindPlayerByDpid(int id)
{
    if (FindSlotByDpid(id) == 10)
        return 0;
    return &g_game->players[FindSlotByDpid(id)];
}

static inline int IsPlaying(Player_00452cc0* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 1 || p->type == 2)
        return 1;
    return 0;
}

static inline int IsType1(Player_00452cc0* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 1)
        return 1;
    return 0;
}

static inline int IsType3(Player_00452cc0* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 3)
        return 1;
    return 0;
}

static inline void Remove(Player_00452cc0* p)
{
    ((Player*)p)->SetType(0);
    p->active = 0;
    p->id = -1;
    p->lobbyDataSynced = 0;
}

// Stays in its own file: the gathered file's earlier callers (0x451bc0 and up)
// must keep their calls to GetSlotDpid, FindSlotByDpid and FindPlayerByDpid,
// while RemovePlayer inlines them the way its original translation unit (with
// the definitions beside it) did. One file cannot do both.
// FUNCTION: 0x452cc0
void __stdcall RemovePlayer(int id)
{
    Player_00452cc0* p = FindPlayerByDpid(id);
    if (p == 0)
        return;
    if (p->active == 0)
        return;
    if (p->type != 1 && p->type != 2 && p->type != 3)
        return;
    if (p->index == 10)
        return;

    unsigned char slot = p->index;
    int f = p->data->flags;
    // The no-op |= 0 must stay: without it the zero-extension folds into the mask.
    p->data->flags |= 0;               // emits no code; needed for the match
    int host = f & 1;

    for (int i = 0; i < 10; i++) {
        Player_00452cc0* q = &g_game->players[i];
        if (IsPlaying(q)) {
            q->alliedBy[slot] = 0;
            q->allies[slot] = 0;
        }
    }

    KillPlayerUnits(FindSlotByDpid(id));

    // Remove stays an inline helper written in both arms.
    if (g_game->flags.b2) {
        if (!IsPlaying(p))
            Remove(p);
    } else {
        if (IsPlaying(p))
            HAPINET_removeplayer((char*)g_game + 0x14, p->id);
        Remove(p);
    }
    g_game->numPlayers--;
    p->data->word_9d &= 0xfffb;
    memset(&p->allies, 0, 11);

    if (g_game->net->GetGameType() == 3)
        ReportGameEvent(3);

    if ((g_game->flags.value & 4) && host != 0) {
        unsigned int best = 0;
        // Index g_game->players[j] directly: a q pointer would be rebased.
        for (int j = 0; j < 10; j++) {
            if (IsType3(&g_game->players[j]) || IsType1(&g_game->players[j])) {
                if (g_game->players[j].id > best)
                    best = g_game->players[j].id;
            }
        }
        Player_00452cc0* r = FindPlayerByDpid(best);
        if (r != 0)
            r->data->flags |= 1;
    }
}
