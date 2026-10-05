// Decompiled by Opus. Names are provisional.

struct Game {
    char unknown_0[0x2a44];
    unsigned short flags_2a44;         // +0x2a44
};

extern Game* g_game;

struct Player_00456190 {
    int unknown_0;
    int field_4;                       // +0x4
};

#pragma pack(push, 1)
struct Object_00456190 {
    char unknown_0[0x96];
    Player_00456190* player;           // +0x96
    char unknown_9a[0xa8 - 0x9a];
    short field_a8;                    // +0xa8
};

struct Packet_00456190 {
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

int __stdcall FUN_00451df0(int player, void* data, int size);

// FUNCTION: 0x456190
int __stdcall FUN_00456190(Object_00456190* obj, short index)
{
    Packet_00456190 packet;
    if (!(g_game->flags_2a44 & 1)) {
        return 0;
    }
    packet.type = 0x10;
    packet.id = obj->field_a8;
    packet.index = index;
    packet.field_5 = 0;
    packet.field_6 = 0;
    packet.field_a = 0;
    packet.field_e = 0;
    packet.field_12 = 0;
    return FUN_00451df0(obj->player->field_4, &packet, 0x16);
}
