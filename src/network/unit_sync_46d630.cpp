// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Packet_0046d630 {               // 0xe bytes
    unsigned char type;                // +0x0
    unsigned char arg;                 // +0x1
    int field_2;                       // +0x2
    int field_6;                       // +0x6
    unsigned char field_a;             // +0xa
    unsigned char field_b;             // +0xb
    short field_c;                     // +0xc
};
#pragma pack(pop)

struct Source_0046d630 {
    int field_0;                       // +0x0
    char unknown_4[4];
    unsigned char field_8;             // +0x8
    char unknown_9;
    unsigned char field_a;             // +0xa
    char unknown_b;
    short field_c;                     // +0xc
};

struct Target_0046d630 {
    unsigned int id;                   // +0x0
    char unknown_4[0x24];
    int sent;                          // +0x28
};

int FUN_0044fe00();
unsigned int FUN_00450030();
void __stdcall FUN_00451bc0(int a, unsigned int b, void* c, int d);

// Inlined copy of Class_0046cec0::FUN_0046cec0 (a method that ignores this).
static inline void SendPacket(unsigned int to, void* packet)
{
    *(int*)((char*)packet + 2) = 0;
    FUN_00451bc0(FUN_0044fe00(), to, packet, 0xe);
}

class Class_0046d630 {
public:
    char unknown_0[0x58];
    int direct;                        // +0x58
    char unknown_5c[0x64 - 0x5c];
    int disabled;                      // +0x64
    void FUN_0046d630(Target_0046d630* target, unsigned char arg, Source_0046d630* src, int unused);
};

// FUNCTION: 0x46d630
void Class_0046d630::FUN_0046d630(Target_0046d630* target, unsigned char arg, Source_0046d630* src, int unused)
{
    if (disabled == 0) {
        Packet_0046d630 packet;
        packet.type = 0x1a;
        packet.arg = arg;
        packet.field_6 = src->field_0;
        packet.field_a = src->field_8;
        packet.field_b = src->field_a;
        packet.field_c = src->field_c;
        if (direct != 0) {
            SendPacket(target->id, &packet);
        } else {
            SendPacket(FUN_00450030(), &packet);
        }
        target->sent++;
    }
}
