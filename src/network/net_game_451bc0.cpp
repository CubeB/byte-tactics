// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
//
// Validates a from/to pair before a packet send: both ids must name a live
// player (from in state 1 or 2, to in state 3), then the packet is either
// accepted because the recipient is already in state 1 or 2, or handed to the
// network layer (and the local byte counters updated).
//
// What made it match: the two lookups use a helper with the `id != -1` guard
// hoisted into the caller (`if (from == -1) fi = 10; else fi = FindIndex(...)`),
// which is what keeps a separate `mov bl, 10` at the helper's loop exit while
// the sentinel case stores 10 directly, and gives each lookup its own stack
// slot (from at [esp+0x10], to at [esp+0x1c]). The third lookup is a separate
// inline helper whose getter reads the slot state directly.

#pragma pack(push, 1)
struct Player_00451bc0 {
    int active;                          // +0x00
    int id;                              // +0x04
    char unknown_8[0x22 - 8];
    unsigned char field_22;              // +0x22
    char unknown_23[0x73 - 0x23];
    unsigned char state;                 // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x14];
    char unknown_14[0x1b63 - 0x14];
    Player_00451bc0 players[10];         // +0x1b63
    char unknown_1[0x2a44 - (0x1b63 + 0x14b * 10)];
    unsigned char flags;                 // +0x2a44
};
#pragma pack(pop)

extern Game* g_game;
extern int g_usePacketManager;

class PacketManager {
public:
    int QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4);
};
extern PacketManager g_packetManager;

int __stdcall GetSlotDpid(unsigned char index);
unsigned char __stdcall FindSlotByDpid(int id);
int __stdcall HAPINET_sendpacket(void* net, unsigned long from, unsigned long to, void* data, unsigned long size);
void __stdcall CountMessage(unsigned char kind, int amount, int player);
void __stdcall CountPacket(int size, int overhead, int sent);

static inline unsigned char FindIndex_00451bc0(int id)
{
    for (unsigned char i = 0; i < 10; i++) {
        if (GetSlotDpid(i) == id)
            return i;
    }
    return 10;
}

static inline int GetPlayerId_00451bc0(unsigned char i)
{
    if (i != 10 && g_game->players[i].state != 0)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_00451bc0(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (GetPlayerId_00451bc0(i) == id)
                return i;
        }
    }
    return 10;
}

// FUNCTION: 0x451bc0
int __stdcall SendPacketToPlayer(int from, int to, unsigned char* packet, int size)
{
    unsigned char fi;
    if (from == -1)
        fi = 10;
    else
        fi = FindIndex_00451bc0(from);
    Player_00451bc0* fromPlayer;
    if (fi == 10)
        fromPlayer = 0;
    else
        fromPlayer = &g_game->players[FindSlotByDpid(from)];

    unsigned char ti;
    if (to == -1)
        ti = 10;
    else
        ti = FindIndex_00451bc0(to);
    Player_00451bc0* toPlayer;
    if (ti == 10)
        toPlayer = 0;
    else
        toPlayer = &g_game->players[FindSlotByDpid(to)];

    if ((g_game->flags & 1) && fromPlayer != 0 && fromPlayer->active != 0 &&
        (fromPlayer->state == 1 || fromPlayer->state == 2) &&
        fromPlayer->field_22 == 0 && toPlayer != 0 && toPlayer->active != 0 &&
        toPlayer->state == 3 && toPlayer->field_22 == 0) {
        Player_00451bc0* target = &g_game->players[FindPlayerIndex_00451bc0(to)];
        if (target->active == 0 || (target->state != 1 && target->state != 2)) {
            if (g_usePacketManager != 0)
                return g_packetManager.QueuePacket(from, to, packet, size);
            if (HAPINET_sendpacket((char*)g_game + 0x14, from, to, packet, size) != 0)
                return 0;
            CountMessage(packet[0], size, 1);
            CountPacket(size, 0, 1);
        }
        return 1;
    }
    return 0;
}
