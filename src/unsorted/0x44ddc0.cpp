// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Slot 0 of vtable 0x4fd3e0: writes the flag word and, per flag bit, the
// short fields (or the three dwords) to a bit stream.

#pragma pack(push, 2)

class Class_00415c10 {
public:
    void FUN_00415c10(int value, int bits);
};

struct Target_0044ddc0 {
    char unknown_0[0xa8];
    unsigned short field_a8;        // +0xa8
};

class Class_0044ddc0 {
public:
    char unknown_0[8];
    unsigned short flags;           // +0x8
    short field_a;                  // +0xa
    short field_c;                  // +0xc
    short field_e;                  // +0xe
    short field_10;                 // +0x10
    char unknown_12[0x1a - 0x12];
    Target_0044ddc0* ptr;           // +0x1a
    char unknown_1e[0x26 - 0x1e];
    int field_26;                   // +0x26
    int field_2a;                   // +0x2a
    int field_2e;                   // +0x2e

    void FUN_0044ddc0(Class_00415c10* stream);
};

#pragma pack(pop)

// FUNCTION: 0x44ddc0
void Class_0044ddc0::FUN_0044ddc0(Class_00415c10* stream)
{
    stream->FUN_00415c10(flags, 8);
    if ((flags & 1) != 0) {
        stream->FUN_00415c10(field_10, 0x10);
        stream->FUN_00415c10((int)(unsigned short)(ptr == 0 ? 0 : ptr->field_a8), 0x10);
    }
    if ((flags & 0x10) != 0) {
        stream->FUN_00415c10(field_a, 0x10);
    }
    if ((flags & 8) != 0) {
        stream->FUN_00415c10(field_c, 0x10);
    }
    if ((flags & 0x40) != 0) {
        stream->FUN_00415c10(field_e, 0x10);
    }
    if ((flags & 0x20) != 0) {
        stream->FUN_00415c10(field_26, 0x20);
        stream->FUN_00415c10(field_2a, 0x20);
        stream->FUN_00415c10(field_2e, 0x20);
    }
}
