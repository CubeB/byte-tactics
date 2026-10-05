// Decompiled by Opus. Names are provisional.

struct Player_00456050 {
    int unknown_0;
    int field_4;                       // +0x4
};

struct Short3_00456050 {
    short a, b, c;
};

struct Vec3_00456050 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Object_00456050 {
    char unknown_0[0x64];
    Short3_00456050 field_64;          // +0x64
    Vec3_00456050 field_6a;            // +0x6a
    char unknown_76[0x96 - 0x76];
    Player_00456050* player;           // +0x96
    char unknown_9a[0xa6 - 0x9a];
    short field_a6;                    // +0xa6
    short field_a8;                    // +0xa8
};

struct Packet_00456050 {
    unsigned char type;                // +0x0
    short field_1;                     // +0x1
    short field_3;                     // +0x3
    Vec3_00456050 field_5;             // +0x5
    Short3_00456050 field_11;          // +0x11
};
#pragma pack(pop)

int __stdcall BroadcastPacket(int player, void* data, int size);

// FUNCTION: 0x456050
void __stdcall SendNewUnit(Object_00456050* obj)
{
    Packet_00456050 packet;
    packet.type = 9;
    packet.field_1 = obj->field_a6;
    packet.field_3 = obj->field_a8;
    packet.field_5 = obj->field_6a;
    packet.field_11 = obj->field_64;
    BroadcastPacket(obj->player->field_4, &packet, 0x17);
}
