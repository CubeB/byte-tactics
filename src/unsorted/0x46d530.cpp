// Decompiled by Opus. Names are provisional.
// Builds a 0xe-byte packet of type 0x1a and sends it, unless sending is
// disabled. The send goes through an inline helper taking a target, called
// here with no target: in "direct" mode it reads the id through that null
// pointer, which is the `mov eax, [0]` in the original.
// Sibling of FUN_0046d630 (same object, same packet type).

#pragma pack(push, 1)
struct Packet_0046d530 {               // 0xe bytes
    unsigned char type;                // +0x0
    unsigned char arg;                 // +0x1
    int field_2;                       // +0x2
    int field_6;                       // +0x6
    int field_a;                       // +0xa
};
#pragma pack(pop)

struct Target_0046d530 {
    unsigned int id;                   // +0x0
};

int FUN_0044fe00();
unsigned int FUN_00450030();
void __stdcall FUN_00451bc0(int a, unsigned int b, void* c, int d);

// Inlined copy of FUN_0046cec0.
static inline void SendPacket(unsigned int to, void* packet)
{
    *(int*)((char*)packet + 2) = 0;
    FUN_00451bc0(FUN_0044fe00(), to, packet, 0xe);
}

class Class_0046d4c0 {
public:
    char unknown_0[0x58];
    int direct;                        // +0x58
    char unknown_5c[0x64 - 0x5c];
    int disabled;                      // +0x64

    void Send(Target_0046d530* target, void* packet)
    {
        if (direct != 0) {
            SendPacket(target->id, packet);
        } else {
            SendPacket(FUN_00450030(), packet);
        }
    }
    void FUN_0046d530(unsigned char arg, int a, int b, int unused);
};

// FUNCTION: 0x46d530
void Class_0046d4c0::FUN_0046d530(unsigned char arg, int a, int b, int unused)
{
    if (disabled == 0) {
        Packet_0046d530 packet;
        packet.type = 0x1a;
        packet.arg = arg;
        packet.field_6 = a;
        packet.field_a = b;
        Send(0, &packet);
    }
}
