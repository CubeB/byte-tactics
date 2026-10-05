// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Builds the 0x24 byte "unit status" network message (type 0xd) and hands it
// to FUN_00451df0. Only sent while g_game's bit 0 flag is set. Sibling of
// 0x499ba0, which sends the same type and size.
//
// The byte at packet +0x1a is a 1 bit bitfield that is only ever read and
// write back by the assignment at the end, so its upper seven bits are never
// initialised here; see the note in the bug list.

#pragma pack(push, 1)
struct Vec3_00499ab0 {
    int x;
    int y;
    int z;
};

struct Def_00499ab0 {
    char unknown_0[0x10a];
    unsigned char f10a;              // +0x10a
    char unknown_10b[0x111 - 0x10b];
    unsigned int f111;               // +0x111, bits 30 and 31
};

struct Unit {
    char unknown_0[0xc];
    Def_00499ab0* def;               // +0xc
    char unknown_10[0x16 - 0x10];
    unsigned short f16;              // +0x16
    unsigned short f18;              // +0x18
    unsigned char f1a;               // +0x1a
    unsigned char f1b;               // +0x1b
};

struct Kind_00499ab0 {
    char unknown_0[4];
    int f4;                          // +0x4, the player / "who"
};

struct Obj_00499ab0 {
    char unknown_0[0x96];
    Kind_00499ab0* kind;             // +0x96
    char unknown_9a[0xa8 - 0x9a];
    unsigned short team;             // +0xa8
};

struct Packet_00499ab0 {
    unsigned char type;              // +0x0
    Vec3_00499ab0 a;                 // +0x1
    Vec3_00499ab0 b;                 // +0xd
    unsigned char f19;               // +0x19
    unsigned char flag : 1;          // +0x1a, bit 0
    unsigned short f1b;              // +0x1b
    unsigned short f1d;              // +0x1d
    unsigned short f1f;              // +0x1f
    unsigned short f21;              // +0x21
    unsigned char f23;               // +0x23
};

struct Game_00499ab0 {
    char unknown_0[0x2a44];
    unsigned short flags_2a44;       // +0x2a44
};
#pragma pack(pop)

extern Game_00499ab0* g_game;

int __stdcall FUN_00451df0(int player, void* data, int size);

// FUNCTION: 0x499ab0
void __stdcall FUN_00499ab0(Unit* unit, Obj_00499ab0* source,
                            Obj_00499ab0* target, Vec3_00499ab0* a,
                            Vec3_00499ab0* b)
{
    Packet_00499ab0 packet;
    if (g_game->flags_2a44 & 1) {
        packet.type = 0xd;
        packet.a = *a;
        packet.b = *b;
        packet.f19 = unit->def->f10a;
        packet.f23 = (unit->f1b >> 2) & 3;
        packet.f21 = !source ? 0 : source->team;
        packet.f1f = !target ? 0 : target->team;
        packet.f1b = unit->f16;
        packet.f1d = unit->f18;
        packet.flag = unit->def->f111 >> 30;
        FUN_00451df0(source->kind->f4, &packet, 0x24);
    }
}
