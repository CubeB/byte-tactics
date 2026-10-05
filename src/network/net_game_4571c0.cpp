// Decompiled by Opus. Names are provisional.
// Sends a 17-byte packet (type 0x16, subtype 3) from player `from` to
// player `to`, carrying both players' DirectPlay ids. Slot 10 means "no
// player".

#pragma pack(push, 1)
struct Player_004571c0 {
    int unknown_0;
    int dpid;                          // +0x04
    char unknown_8[0x73 - 0x8];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_004571c0 players[10];       // +0x1b63
};

struct Packet_004571c0 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int zero;                          // +0xd
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);

static inline int PlayerDpid(unsigned char i)
{
    if (g_game->players[i].type != 0)
        return g_game->players[i].dpid;
    return -1;
}

// FUNCTION: 0x4571c0
void __stdcall SendShareMapInfo(unsigned char from, unsigned char to)
{
    if (from != 10 && to != 10) {
        Packet_004571c0 packet;
        packet.type = 0x16;
        packet.subtype = 3;
        packet.from = PlayerDpid(from);
        packet.to = PlayerDpid(to);
        packet.zero = 0;
        SendPacketToPlayer(PlayerDpid(from), PlayerDpid(to), &packet, sizeof(packet));
    }
}
