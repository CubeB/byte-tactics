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

#pragma pack(push, 1)

struct PlayerData_00452960 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
};

struct Player_00452960 {
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerData_00452960* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char allies[0xb];         // +0x108
    unsigned char field_113[0xb];      // +0x113
    char unknown_11e[0x146 - 0x11e];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Packet_00452960 {
    unsigned char type;                // +0x0
    int from;                          // +0x1
    int to;                            // +0x5
    unsigned char value;               // +0x9
    int extra;                         // +0xa
};

class Mission {
public:
    int GetGameType();
};

struct Game {
    char unknown_0[0x1b63];
    Player_00452960 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    unsigned char* buffer;             // +0x2a38
    char unknown_2a3c[0x391e9 - 0x2a3c];
    Mission* net;                      // +0x391e9
};

#pragma pack(pop)

class PacketManager {
public:
    int SendAllQueued(int param_1);
};

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
static inline int IsActive12_00452960(Player_00452960* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 1 || p->type == 2)
        return 1;
    return 0;
}

static inline int IsType3_00452960(Player_00452960* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 3)
        return 1;
    return 0;
}

static inline int IsState2_00452960(Player_00452960* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 2)
        return 1;
    return 0;
}

static inline int IsState3_00452960(Player_00452960* p)
{
    if (p->active == 0)
        return 0;
    if (p->type == 3)
        return 1;
    return 0;
}

// FUNCTION: 0x452960
int __stdcall SetAlliance(int from, int to, unsigned char value, int extra)
{
    unsigned char fi;
    // -1 handled here, not in the helper: keeps the separate store of 10.
    if (from == -1)
        fi = 10;
    else
        fi = FindIndex_00452960(from);

    Player_00452960* p1;
    if (fi == 10)
        p1 = 0;
    else
        p1 = &g_game->players[FindSlotByDpid(from)];

    Player_00452960* p2;
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
        idx = p2->field_146;
        p1->allies[idx] = value;
        if (IsState2_00452960(p2)
            || (IsState3_00452960(p2) && p2->data->field_94 == 2)
            || extra != 0) {
            idx = p2->field_146;
            p1->field_113[idx] = value;
        }
        result = 1;
    }
    if (IsActive12_00452960(p2)) {
        idx = p1->field_146;
        p2->field_113[idx] = value;
        if (IsState2_00452960(p2) || extra != 0) {
            idx = p1->field_146;
            p2->allies[idx] = value;
        }
        result = 1;
    } else if (IsType3_00452960(p2)) {
        Packet_00452960* msg = (Packet_00452960*)g_game->buffer;
        msg->value = value;
        msg->type = 0x23;
        msg->from = from;
        msg->to = to;
        msg->extra = extra;
        int r = SendPacketToPlayer(from, to, msg, 0xe);
        if (g_usePacketManager != 0)
            g_packetManager.SendAllQueued(1);
        result = r;
    }
    if (g_game->net->GetGameType() == 3)
        ReportGameEvent(4);
    return result;
}
