// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Bit reader, see src/network/bit_reader.cpp.
class BitReader {
public:
    int ReadBits(int bits);
};

// Unit reference link: vtable, owner, next, value (0x10 bytes).
class Class_004895c0 {
public:
    virtual void Unknown_0();
    void* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc

    Class_004895c0(void* o, int v);
    void SetUnit(void* o);
};

struct Owner_0044e080;

extern void* DAT_004fd2f8[];
extern void* DAT_004fd3b8[];

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x118];
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
};
#pragma pack(pop)

extern Game* g_game;

#pragma pack(push, 2)
class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4

    Class_0044ce20(int param_1)
    {
        vtable = DAT_004fd2f8;
        field_4 = param_1;
    }
};

class Class_0044e080 : public Class_0044ce20 {
public:
    unsigned short flags;              // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    short field_e;                     // +0xe
    short field_10;                    // +0x10
    Owner_0044e080* owner;             // +0x12
    Class_004895c0 ref;                // +0x16
    int pos_x;                         // +0x26
    int pos_y;                         // +0x2a
    int pos_z;                         // +0x2e

    Class_0044e080(Owner_0044e080* owner_, BitReader* reader);
};
#pragma pack(pop)

// FUNCTION: 0x44e080
Class_0044e080::Class_0044e080(Owner_0044e080* owner_, BitReader* reader)
    : Class_0044ce20(0), owner(owner_), ref(0, 0)
{
    vtable = DAT_004fd3b8;
    flags = reader->ReadBits(8);
    if (flags & 1) {
        field_10 = reader->ReadBits(0x10);
        unsigned short index = reader->ReadBits(0x10);
        ref.SetUnit(index == 0 ? 0 : (void*)&g_game->units[index]);
    }
    if (flags & 0x10)
        field_a = reader->ReadBits(0x10);
    else
        field_a = 0;
    if (flags & 8)
        field_c = reader->ReadBits(0x10);
    else
        field_c = 0;
    if (flags & 0x40)
        field_e = reader->ReadBits(0x10);
    else
        field_e = 0;
    if (flags & 0x20) {
        pos_x = reader->ReadBits(0x20);
        pos_y = reader->ReadBits(0x20);
        pos_z = reader->ReadBits(0x20);
    }
}
