// Decompiled by Opus. Names are provisional.
// Builds a 0xe-byte packet of type 0x1a and sends it, unless sending is
// disabled. The send goes through an inline helper taking a target, called
// here with no target: in "direct" mode it reads the id through that null
// pointer, which is the `mov eax, [0]` in the original.
// Sibling of SendEntryTo (same object, same packet type).

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

int GetLocalHumanDpid();
unsigned int GetHostDpid();
void __stdcall SendPacketToPlayer(int a, unsigned int b, void* c, int d);

// Inlined copy of Class_0046cec0::SendUnsequenced (a method that ignores this).
static inline void SendPacket(unsigned int to, void* packet)
{
    *(int*)((char*)packet + 2) = 0;
    SendPacketToPlayer(GetLocalHumanDpid(), to, packet, 0xe);
}

class UnitSync {
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
            SendPacket(GetHostDpid(), packet);
        }
    }
    void SendSyncMessage(unsigned char arg, int a, int b, int unused);
};

// FUNCTION: 0x46d530
void UnitSync::SendSyncMessage(unsigned char arg, int a, int b, int unused)
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
