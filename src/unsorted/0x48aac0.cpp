// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Unit_0048aac0 {
    char unknown_0[0x86];
    int field_86;                      // +0x86
    int field_8a;                      // +0x8a
    char unknown_8e[0xa8 - 0x8e];
    unsigned short id;                 // +0xa8
    char unknown_aa[0x110 - 0xaa];
    unsigned int flags;                // +0x110
};

struct Packet_0048aac0 {
    unsigned char type;                // +0x0
    unsigned short unitId;             // +0x1
    unsigned short targetId;           // +0x3
    char field_5;                      // +0x5
    char field_6;                      // +0x6
};
#pragma pack(pop)

int __cdecl FUN_0044fdb0();
int __stdcall FUN_00451df0(int player, void* data, int size);
void __stdcall FUN_0048ab70(Packet_0048aac0* packet);

// FUNCTION: 0x48aac0
void __stdcall FUN_0048aac0(Unit_0048aac0* unit, Unit_0048aac0* target, char p3, char p4)
{
    Packet_0048aac0 packet;
    if ((unit->flags & 0x10000000) && !(unit->flags & 0x20000000) && unit->field_8a == 0
        && (target == 0
            || ((target->flags & 0x10000000) && target != unit && target->field_86 == 0))) {
        packet.type = 0xa;
        if (unit == 0)
            packet.unitId = 0;
        else
            packet.unitId = unit->id;
        if (target == 0)
            packet.targetId = 0;
        else
            packet.targetId = target->id;
        packet.field_5 = p3;
        packet.field_6 = p4;
        FUN_00451df0(FUN_0044fdb0(), &packet, 7);
        FUN_0048ab70(&packet);
    }
}
