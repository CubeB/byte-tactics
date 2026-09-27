// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Slot of Class_0044e740 (vtable 0x4fd3f8): writes the flag word, the two
// points, and (when flag bit 0 is set) the word at +0x24 to a bit stream.

#pragma pack(push, 2)

struct Vec3_0044e930 {
    int x;
    int y;
    int z;
};

class Class_00415c10 {
public:
    void FUN_00415c10(int value, int bits);
};

class Class_0044e740 {
public:
    char unknown_0[8];
    unsigned short field_8;             // +0x8
    Vec3_0044e930 target;               // +0xa
    Vec3_0044e930 other;                // +0x16
    short field_22;                     // +0x22
    unsigned short field_24;            // +0x24

    void FUN_0044e930(Class_00415c10* stream);
};
#pragma pack(pop)

// FUNCTION: 0x44e930
void Class_0044e740::FUN_0044e930(Class_00415c10* stream)
{
    stream->FUN_00415c10(field_8, 1);
    stream->FUN_00415c10(target.x, 0x20);
    stream->FUN_00415c10(target.y, 0x20);
    stream->FUN_00415c10(target.z, 0x20);
    stream->FUN_00415c10(other.x, 0x20);
    stream->FUN_00415c10(other.y, 0x20);
    stream->FUN_00415c10(other.z, 0x20);
    if ((field_8 & 1) != 0) {
        stream->FUN_00415c10(field_24, 0x10);
    }
}
