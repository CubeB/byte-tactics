// Decompiled by space-bunny-free. Names are provisional.
// Sends a 17-byte packet (type 0x16, subtype 2) from player `from` to player
// `to` carrying both players' DirectPlay ids plus a dword, but only when both
// slots hold a real player of type 1, 2 or 3 (compare 0x4571c0, which is the
// same packet with subtype 3 and a trailing zero).

#pragma pack(push, 1)
struct Player_00457050 {
    int active;                        // +0x00
    int dpid;                          // +0x04
    char unknown_8[0x73 - 0x8];
    unsigned char type;                // +0x73
    char unknown_74[0x140 - 0x74];
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00457050 players[10];       // +0x1b63
};

struct Packet_00457050 {
    unsigned char type;                // +0x0
    int subtype;                       // +0x1
    int from;                          // +0x5
    int to;                            // +0x9
    int value;                         // +0xd
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);

static inline int PlayerDpid(unsigned char i)
{
    if (i != 10 && g_game->players[i].type != 0)
        return g_game->players[i].dpid;
    return -1;
}

// FUNCTION: 0x457050
void __stdcall FUN_00457050(unsigned char from, unsigned char to, int value)
{
    Player_00457050* first = &g_game->players[from];
    Player_00457050* second = &g_game->players[to];
    if (first->active != 0
        && (first->type == 1 || first->type == 2 || first->type == 3)
        && first->field_146 != 10
        && (first->field_144 != 0 || first->field_140 == 0)
        && second->active != 0
        && (second->type == 1 || second->type == 2 || second->type == 3)
        && second->field_146 != 10
        && (second->field_144 != 0 || second->field_140 == 0)) {
        Packet_00457050 packet;
        packet.type = 0x16;
        packet.subtype = 2;
        packet.from = PlayerDpid(from);
        packet.to = PlayerDpid(to);
        packet.value = value;
        FUN_00451bc0(PlayerDpid(from), PlayerDpid(to), &packet, sizeof(packet));
    }
}
