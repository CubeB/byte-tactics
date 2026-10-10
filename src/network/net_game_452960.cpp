// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Sets (or clears) the alliance between the two players named by `from` and
// `to`. Each player has two eleven-byte per-team tables at +0x108 and +0x113,
// a team number at +0x146 and a state byte at +0x73; `extra` forces the second
// table to be written as well. When the other player cannot be reached locally
// (state 3) a 14-byte packet 0x23 is sent instead, and the game-mode object at
// g_game+0x391e9 is asked for a value of 3 afterwards.
// Needed although no CRT function is called: sets the base/index order of the
// byte stores.
#include <stdio.h>

#include "player.h"

#pragma pack(push, 1)

#include "player_info.h"

struct AllyFlagsPacket {
    unsigned char type;                // +0x0
    int fromNetId;                     // +0x1
    int toNetId;                       // +0x5
    unsigned char allied;              // +0x9
    int force;                         // +0xa
};

#include "../map/mission.h"

struct Game {
    char unknown_0[0x1b63];
    Player players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    unsigned char* recvPacketPtr;      // +0x2a38
    char unknown_2a3c[0x391e9 - 0x2a3c];
    Mission* mapInfo;                  // +0x391e9
};

#pragma pack(pop)

#include "packet_manager.h"

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

int __stdcall GetSlotDpid(unsigned char index);
unsigned char __stdcall FindSlotByDpid(int id);
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);
void __stdcall ReportGameEvent(int param_1);

static inline unsigned char FindIndex_00452960(int id)
{
    for (unsigned char i = 0; i < 10; i++) {
        if (GetSlotDpid(i) == id)
            return i;
    }
    return 10;
}

// The state helpers keep one return per outcome and each re-tests active.
static inline int IsActive12_00452960(Player* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 1 || p->type == 2)
        return 1;
    return 0;
}

static inline int IsType3_00452960(Player* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 3)
        return 1;
    return 0;
}

static inline int IsState2_00452960(Player* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 2)
        return 1;
    return 0;
}

static inline int IsState3_00452960(Player* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 3)
        return 1;
    return 0;
}

// Stays in its own file: the alliance table stores' SIB base/index order
// follows the original translation unit's symbol ids, which the gathered
// file changes.
// FUNCTION: 0x452960
int __stdcall SetAlliance(int from, int to, unsigned char value, int extra)
{
    unsigned char fi;
    // -1 handled here, not in the helper: keeps the separate store of 10.
    if (from == -1)
        fi = 10;
    else
        fi = FindIndex_00452960(from);

    Player* p1;
    if (fi == 10)
        p1 = 0;
    else
        p1 = &g_game->players[FindSlotByDpid(from)];

    Player* p2;
    if (FindSlotByDpid(to) == 10)
        p2 = 0;
    else
        p2 = &g_game->players[FindSlotByDpid(to)];

    int result = 0;
    if (p1 == 0 || p2 == 0)
        return 0;

    // The reference is what makes MSVC keep this byte in its stack slot and
    // reload it with the `and 0xff` widening the original has.
    unsigned char idx_;
    unsigned char& idx = idx_;
    if (IsActive12_00452960(p1)) {
        idx = p2->index;
        p1->allied[idx] = value;
        if (IsState2_00452960(p2)
            || (IsState3_00452960(p2) && p2->info->kind == 2)
            || extra != 0) {
            idx = p2->index;
            p1->alliedBy[idx] = value;
        }
        result = 1;
    }
    if (IsActive12_00452960(p2)) {
        idx = p1->index;
        p2->alliedBy[idx] = value;
        if (IsState2_00452960(p2) || extra != 0) {
            idx = p1->index;
            p2->allied[idx] = value;
        }
        result = 1;
    } else if (IsType3_00452960(p2)) {
        AllyFlagsPacket* msg = (AllyFlagsPacket*)g_game->recvPacketPtr;
        msg->allied = value;
        msg->type = 0x23;
        msg->fromNetId = from;
        msg->toNetId = to;
        msg->force = extra;
        int r = SendPacketToPlayer(from, to, msg, 0xe);
        if (g_usePacketManager != 0)
            g_packetManager.SendAllQueued(1);
        result = r;
    }
    if (g_game->mapInfo->GetGameType() == 3)
        ReportGameEvent(4);
    return result;
}
