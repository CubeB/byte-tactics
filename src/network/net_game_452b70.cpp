// Decompiled by Opus. Names are provisional.
// Sends a 14-byte message (type 0x23) carrying both player ids, a byte and a
// dword from the shared packet buffer.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2a38];
    unsigned char* buffer;             // +0x2a38
};

struct Packet_00452b70 {
    unsigned char type;                // +0x0
    int from;                          // +0x1
    int to;                            // +0x5
    char value;                        // +0x9
    int extra;                         // +0xa
};
#pragma pack(pop)

class PacketManager {
public:
    int SendAllQueued(int param_1);
};

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);

// FUNCTION: 0x452b70
int __stdcall SendAlliance(int from, int to, char value, int extra)
{
    Packet_00452b70* msg = (Packet_00452b70*)g_game->buffer;
    msg->value = value;
    msg->type = 0x23;
    msg->from = from;
    msg->to = to;
    msg->extra = extra;
    int result = SendPacketToPlayer(from, to, msg, 0xe);
    if (g_usePacketManager != 0) {
        g_packetManager.SendAllQueued(1);
    }
    return result;
}
