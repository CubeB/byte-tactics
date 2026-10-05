// Decompiled by Opus. Names are provisional.

struct Game {
    char unknown_0[0x2a44];
    unsigned short flags_2a44;         // +0x2a44
};

extern Game* g_game;

class Class_004b07c0 {
public:
    int FindScript(char* name);
};

struct Player_00456110 {
    int unknown_0;
    int field_4;                       // +0x4
};

#pragma pack(push, 1)
struct Object_00456110 {
    char unknown_0[0x96];
    Player_00456110* player;           // +0x96
    Class_004b07c0* names;             // +0x9a
    char unknown_9e[0xa8 - 0x9e];
    short field_a8;                    // +0xa8
};

struct Packet_00456110 {
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

// FUNCTION: 0x456110
int __stdcall SendScriptCallNoArgsByName(Object_00456110* obj, char* name)
{
    Packet_00456110 packet;
    int index = obj->names->FindScript(name);
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
    return BroadcastPacket(obj->player->field_4, &packet, 0x16);
}
