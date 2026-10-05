// Decompiled by Opus. Names are provisional.
// Sends a 0x16-byte type 0x10 packet for the object (the sibling 0x456110
// sends the same packet with only the index filled in).

struct Game {
    char unknown_0[0x2a44];
    unsigned short flags_2a44;         // +0x2a44
};

extern Game* g_game;

struct Player_00456290 {
    int unknown_0;
    int field_4;                       // +0x4
};

#pragma pack(push, 1)
struct Object_00456290 {
    char unknown_0[0x96];
    Player_00456290* player;           // +0x96
    char unknown_9a[0xa8 - 0x9a];
    short field_a8;                    // +0xa8
};

struct Packet_00456290 {
    unsigned char type;                // +0x0
    short id;                          // +0x1
    short index;                       // +0x3
    char field_5;                      // +0x5
    int field_6;                       // +0x6
    int field_a;                       // +0xa
    int field_e;                       // +0xe
    int field_12;                      // +0x12
};
#pragma pack(pop)

int __stdcall BroadcastPacket(int player, void* data, int size);

// FUNCTION: 0x456290
int __stdcall SendScriptCall(Object_00456290* obj, short index, char field_5,
                           int field_6, int field_a, int field_e, int field_12)
{
    Packet_00456290 packet;
    if (!(g_game->flags_2a44 & 1)) {
        return 0;
    }
    packet.type = 0x10;
    packet.id = obj->field_a8;
    packet.index = index;
    packet.field_5 = field_5;
    packet.field_6 = field_6;
    packet.field_a = field_a;
    packet.field_e = field_e;
    packet.field_12 = field_12;
    return BroadcastPacket(obj->player->field_4, &packet, 0x16);
}
