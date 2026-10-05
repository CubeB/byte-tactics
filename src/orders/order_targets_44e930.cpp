// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Slot of Class_0044e740 (vtable 0x4fd3f8): writes the flag word, the two
// points, and (when flag bit 0 is set) the word at +0x24 to a bit stream.

#pragma pack(push, 2)

struct Vec3_0044e930 {
    int x;
    int y;
    int z;
};

class BitWriter {
public:
    void WriteBits(int value, int bits);
};

class Class_0044e740 {
public:
    char unknown_0[8];
    unsigned short field_8;             // +0x8
    Vec3_0044e930 target;               // +0xa
    Vec3_0044e930 other;                // +0x16
    short field_22;                     // +0x22
    unsigned short field_24;            // +0x24

    void FUN_0044e930(BitWriter* stream);
};
#pragma pack(pop)

// FUNCTION: 0x44e930
void Class_0044e740::FUN_0044e930(BitWriter* stream)
{
    stream->WriteBits(field_8, 1);
    stream->WriteBits(target.x, 0x20);
    stream->WriteBits(target.y, 0x20);
    stream->WriteBits(target.z, 0x20);
    stream->WriteBits(other.x, 0x20);
    stream->WriteBits(other.y, 0x20);
    stream->WriteBits(other.z, 0x20);
    if ((field_8 & 1) != 0) {
        stream->WriteBits(field_24, 0x10);
    }
}
