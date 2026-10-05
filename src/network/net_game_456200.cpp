// Decompiled by Opus. Names are provisional.
// Sends a 0x16-byte type 0x10 packet for the object, like 0x456290, with the
// index looked up by name in the object's name table (+0x9a) first.

struct Game {
    char unknown_0[0x2a44];
    unsigned short flags_2a44;         // +0x2a44
};

extern Game* g_game;

class Class_004b07c0 {
public:
    int FindScript(char* name);
};

struct Player_00456200 {
    int unknown_0;
    int field_4;                       // +0x4
};

#pragma pack(push, 1)
struct Object_00456200 {
    char unknown_0[0x96];
    Player_00456200* player;           // +0x96
    Class_004b07c0* names;             // +0x9a
    char unknown_9e[0xa8 - 0x9e];
    short field_a8;                    // +0xa8
};

struct Packet_00456200 {
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

// FUNCTION: 0x456200
int __stdcall SendScriptCallByName(Object_00456200* obj, char* name, char field_5,
                           int field_6, int field_a, int field_e, int field_12)
{
    Packet_00456200 packet;
    short index = obj->names->FindScript(name);
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
