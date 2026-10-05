// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
//
// Sends the local player's incoming message `packet` (network layer, `size`)
// to `id` if that player is a live client. The player id search is the inlined
// FindPlayer helper: it searches twice (once to decide whether the player
// exists, once to take the address), which is what produces the two copies of
// the id loop. When the net layer has no DirectPlay interface (g_usePacketManager
// clear) it goes through HAPINET_sendpacket on g_game + 0x14 and reports the packet
// as forwarded with CountMessage/CountPacket. If instead g_game + 0x299c is
// non-zero this is a broadcast round: every in-use player in state 3 whose
// target group has not been told yet gets the packet, and the group is marked
// in DAT_00512b90. A single shared `return 1` at the end is what made the
// `flags_2a44` test compile as a forward `je`.
//
// Nothing guards the two inlined index searches: an id of -1 makes
// FindPlayerIndex return 10 and FindPlayer tests that against 10, so the -1
// case is handled, but the DAT_00512b90 indexing at +0xc is only range checked
// after the send.
#include <string.h>

#pragma pack(push, 1)
struct Player_00451df0 {
    int active;                        // +0x00
    int field_4;                       // +0x04
    char unknown_8[0xc - 8];
    int field_c;                       // +0x0c
    char unknown_10[0x22 - 0x10];
    char field_22;                     // +0x22
    char unknown_23[0x73 - 0x23];
    char state;                        // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00451df0 players[10];       // +0x1b63
    char unknown_1[0x299c - (0x1b63 + 0x14b * 10)];
    int field_299c;                    // +0x299c
    char unknown_2[0x2a44 - 0x299c - 4];
    unsigned char flags_2a44;          // +0x2a44
};
#pragma pack(pop)

class PacketChannel;

class PacketManager {
public:
    int QueueOnChannel(int param_1, PacketChannel* param_2, int param_3, int param_4);
};

extern Game* g_game;
extern int g_usePacketManager;
extern PacketChannel DAT_00513008;
extern PacketManager g_packetManager;
extern int DAT_00512b90[11];

int __stdcall GetSlotDpid(unsigned char index);
int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);
int __stdcall HAPINET_sendpacket(void* net, unsigned long from, unsigned long to, void* data, unsigned long size);
void __stdcall CountMessage(unsigned char kind, int amount, int player);
void __stdcall CountPacket(int size, int overhead, int sent);

static inline unsigned char FindPlayerIndex(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (GetSlotDpid(i) == id)
                return i;
        }
    }
    return 10;
}

static inline Player_00451df0* FindPlayer(int id)
{
    if (FindPlayerIndex(id) == 10)
        return 0;
    return &g_game->players[FindPlayerIndex(id)];
}

// FUNCTION: 0x451df0
int __stdcall BroadcastPacket(int id, unsigned char* packet, int size)
{
    Player_00451df0* p = FindPlayer(id);
    if (p == 0 || p->active == 0)
        return 0;
    if (p->state != 1 && p->state != 2)
        return 0;
    if (p->field_22 != 0)
        return 0;
    if ((g_game->flags_2a44 & 1) != 0) {
        if (g_game->field_299c == 0) {
            if (g_usePacketManager != 0)
                return g_packetManager.QueueOnChannel(id, &DAT_00513008, (int)packet, size);
            if (HAPINET_sendpacket((char*)g_game + 0x14, id, 0, packet, size) != 0)
                return 0;
            CountMessage(packet[0], size, 1);
            CountPacket(size, 0, 1);
            return 1;
        }
        memset(DAT_00512b90, 0, 0x2c);
        for (int i = 0; i < 10; i++) {
            if (g_game->players[i].active == 0)
                continue;
            if (g_game->players[i].state != 3)
                continue;
            if (DAT_00512b90[g_game->players[i].field_c] != 0)
                continue;
            SendPacketToPlayer(id, g_game->players[i].field_4, packet, size);
            int c = g_game->players[i].field_c;
            if (c >= 0 && c < 10)
                DAT_00512b90[c] = 1;
        }
    }
    return 1;
}
