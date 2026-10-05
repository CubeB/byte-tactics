// Decompiled by Opus. Names are provisional.

struct Game {
    char unknown_0[0x2a44];
    unsigned short flags_2a44;         // +0x2a44
};

extern Game* g_game;

struct Vec3_00499ba0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Packet_00499ba0 {
    unsigned char type;                // +0x0
    Vec3_00499ba0 a;                   // +0x1
    Vec3_00499ba0 b;                   // +0xd
    char field_19;                     // +0x19
    char unknown_1a[0x24 - 0x1a];
};
#pragma pack(pop)

int __cdecl FUN_0044fdb0();
int __stdcall FUN_00451df0(int player, void* data, int size);

// FUNCTION: 0x499ba0
void __stdcall FUN_00499ba0(char param_1, Vec3_00499ba0* a, Vec3_00499ba0* b)
{
    Packet_00499ba0 packet;
    if (g_game->flags_2a44 & 1) {
        packet.type = 0xd;
        packet.a = *a;
        packet.b = *b;
        packet.field_19 = param_1;
        FUN_00451df0(FUN_0044fdb0(), &packet, 0x24);
    }
}
