// Decompiled by space-bunny-free. Names are provisional.
#pragma pack(push, 1)
struct Vec3_0049df10 {
    int x;
    int y;
    int z;
};

struct Proj_0049df10 {
    char unknown_0[0x1c];
    Vec3_0049df10 dir;                 // +0x1c
    char unknown_28[0x4e - 0x28];
    int field_4e;                      // +0x4e
    char unknown_52[0x69 - 0x52];
    unsigned short flags;              // +0x69
};

struct Game {
    char unknown_0[0x2a44];
    unsigned short flags_2a44;         // +0x2a44
    char unknown_2a46[0x141f3 - 0x2a46];
    int projCount;                     // +0x141f3
    Proj_0049df10* projs;              // +0x141f7
    char unknown_141fb[0x38a47 - 0x141fb];
    int field_38a47;
};

struct Unit_0049df10 {
    char unknown_0[0x10a];
    unsigned char team;                // +0x10a
};

struct Packet_0049df10 {
    unsigned char type;                // +0x0
    Vec3_0049df10 a;                   // +0x1
    Vec3_0049df10 b;                   // +0xd
    unsigned char field_19;            // +0x19
    char unknown_1a[0x24 - 0x1a];
};
#pragma pack(pop)

extern Game* g_game;

int __cdecl GetLocalDpid();
int __stdcall BroadcastPacket(int player, void* data, int size);
void __stdcall InitProjectile(Proj_0049df10*, void*, void*, int, int, void*);

// FUNCTION: 0x49df10
int __stdcall FUN_0049df10(Unit_0049df10* unit, Vec3_0049df10* a, Vec3_0049df10* b, int flag)
{
    Proj_0049df10* proj = 0;
    if (g_game->projCount < 300) {
        proj = &g_game->projs[g_game->projCount++];
        proj->flags &= ~2;
        proj->field_4e = 0;
    }
    if (!proj)
        return 0;
    InitProjectile(proj, unit, a, 0, g_game->field_38a47, 0);
    proj->dir = *b;
    if (flag) {
        // Read into a local first: only then does MSVC hoist the byte load
        // above the g_game->flags_2a44 test, into the low byte of the register
        // that held `unit`, which is what leaves al free for the `lea` and
        // pushes the +0xd store and the type store in their original places.
        unsigned char team = unit->team;
        if (g_game->flags_2a44 & 1) {
            Packet_0049df10 packet;
            packet.type = 0xd;
            packet.a = *a;
            packet.b = *b;
            packet.field_19 = team;
            BroadcastPacket(GetLocalDpid(), &packet, 0x24);
        }
    }
    return 1;
}
