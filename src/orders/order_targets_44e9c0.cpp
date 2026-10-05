// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

// Bit reader, see src/network/net_stats_415dc0.cpp.
class BitReader {
public:
    int ReadBits(int bits);
};

struct Owner_0044e9c0;

extern void* DAT_004fd3f8[];

#pragma pack(push, 2)
class Class_0044e9c0 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4
    unsigned short flags;              // +0x8
    int field_a;                       // +0xa
    int field_e;                       // +0xe
    int field_12;                      // +0x12
    int field_16;                      // +0x16
    int field_1a;                      // +0x1a
    int field_1e;                      // +0x1e
    short field_22;                    // +0x22
    unsigned short field_24;           // +0x24
    short pad_26;                      // +0x26
    Owner_0044e9c0* owner;             // +0x28

    Class_0044e9c0(Owner_0044e9c0* owner, BitReader* reader);
};
#pragma pack(pop)

// FUNCTION: 0x44e9c0
Class_0044e9c0::Class_0044e9c0(Owner_0044e9c0* owner, BitReader* reader)
{
    field_4 = 0;
    this->owner = owner;
    vtable = DAT_004fd3f8;
    flags = reader->ReadBits(1);
    field_a = reader->ReadBits(0x20);
    field_e = reader->ReadBits(0x20);
    field_12 = reader->ReadBits(0x20);
    field_16 = reader->ReadBits(0x20);
    field_1a = reader->ReadBits(0x20);
    field_1e = reader->ReadBits(0x20);
    if (flags & 1) {
        field_24 = reader->ReadBits(0x10);
    }
}
