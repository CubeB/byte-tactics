// Decompiled by Opus. Names are provisional.

struct Player_004560c0 {
    int unknown_0;
    int field_4;                       // +0x4
};

#pragma pack(push, 1)
struct Object_004560c0 {
    char unknown_0[0x96];
    Player_004560c0* player;           // +0x96
    char unknown_9a[0xa8 - 0x9a];
    short field_a8;                    // +0xa8
};

struct Packet_004560c0 {
    unsigned char type;                // +0x0
    short field_1;                     // +0x1
    short field_3;                     // +0x3
};
#pragma pack(pop)

int __stdcall FUN_00451df0(int player, void* data, int size);

// FUNCTION: 0x4560c0
void __stdcall FUN_004560c0(Object_004560c0* obj, Object_004560c0* target)
{
    Packet_004560c0 packet;
    packet.type = 0x12;
    packet.field_1 = target->field_a8;
    packet.field_3 = obj->field_a8;
    FUN_00451df0(obj->player->field_4, &packet, 5);
}
