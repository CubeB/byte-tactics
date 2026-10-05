// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Sends a 17-byte packet (type 0x16, subtype 1) from player `from` to player
// `to`, carrying both players' DirectPlay ids plus a dword argument. Slot 10
// means "no player".
//
// Same player layout and packet as 0x4571c0 (type 0x16, subtype 3); see
// 0x4515d0 for the PlayerDpid helper spelling.

#pragma pack(push, 1)
struct Player_00456ee0 {
    int active;                        // +0x00
    int dpid;                          // +0x04
    char unknown_8[0x73 - 8];
    unsigned char type;                // +0x73
    char unknown_74[0x140 - 0x74];
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00456ee0 players[10];       // +0x1b63
};

struct Packet_00456ee0 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int extra;                         // +0xd
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall SendPacketToPlayer(int from, int to, void* packet, int size);

static inline int PlayerDpid(unsigned char i)
{
    if (i != 10 && g_game->players[i].type != 0)
        return g_game->players[i].dpid;
    return -1;
}

static inline int PlayerReady(Player_00456ee0* p)
{
    return p->active != 0
        && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10
        && (p->field_144 != 0 || p->field_140 == 0);
}

// FUNCTION: 0x456ee0
void __stdcall SendShareMetal(unsigned char from, unsigned char to, int param_3)
{
    Player_00456ee0* a = &g_game->players[from];
    Player_00456ee0* b = &g_game->players[to];

    if (PlayerReady(a) && PlayerReady(b)) {
        Packet_00456ee0 packet;
        packet.type = 0x16;
        packet.subtype = 1;
        packet.from = PlayerDpid(from);
        packet.to = PlayerDpid(to);
        packet.extra = param_3;
        SendPacketToPlayer(PlayerDpid(from), PlayerDpid(to), &packet, sizeof(packet));
    }
}
