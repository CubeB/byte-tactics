// Decompiled by space-bunny-free. Names are provisional.
// Writes one unit's state into a bit stream (the writer of 0x415c10, the read
// counterpart is 0x48b3f0). The unit's type index (+0xa6) goes out in a bit
// count taken from g_game+0x14393, and a unit with none of that is done.
// The "advance one bit" tail is the tail of WriteBits's fast path written
// out by hand, as in the matched 0x44f4a0, and it is the store through
// stream->data that makes the compiler reload the unit's link pointer and test
// it a second time. `!= 0.0f` is what MSVC 5 turns into the fcomp / C3 test
// the original uses, and writing the conditional value as `!link ? 0 : ...`
// (not `link ? ... : 0`) is what lays the zero arm out ahead of the load; the
// `& 0xffff` is redundant on an unsigned short but the original keeps it.

#pragma pack(push, 1)
class BitWriter {
public:
    int bit;                           // +0x0 current word index
    int index;                         // +0x4 bits used in the current word
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10

    void GrowBuffer();
    void WriteBits(int value, int bits);
};

struct Link_0048b200 {
    char unknown_0[0xa8];
    unsigned short field_a8;
};

struct Owner_0048b200 {
    char unknown_0[0x20];
    int field_20;
};

struct Unit {
    Owner_0048b200* owner;             // +0x0
    char unknown_4[0x64 - 4];
    unsigned short field_64;           // +0x64
    unsigned short field_66;           // +0x66
    unsigned short field_68;           // +0x68
    int field_6a;                      // +0x6a
    int field_6e;                      // +0x6e
    int field_72;                      // +0x72
    char unknown_76[0x86 - 0x76];
    Link_0048b200* link;               // +0x86
    char unknown_8a[0xa6 - 0x8a];
    unsigned short field_a6;           // +0xa6
    char unknown_a8[0xf9 - 0xa8];
    signed char field_f9;              // +0xf9
    char unknown_fa[0x104 - 0xfa];
    float field_104;                   // +0x104
    short field_108;                   // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char field_10e;           // +0x10e
    char unknown_10f[0x110 - 0x10f];
    int flags;                         // +0x110
};

struct Game {
    char unknown_0[0x14393];
    int field_14393;                   // +0x14393, bit count for the type index
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x48b200
void __stdcall WriteUnitState(BitWriter* stream, Unit* u)
{
    stream->WriteBits(u->field_a6, g_game->field_14393);
    if (u->field_a6 == 0)
        return;
    stream->WriteBits(u->field_108, 0x10);
    stream->WriteBits((u->field_104 != 0.0f) ? 1 - (int)(u->field_104 * -254.0f) : 0, 8);
    stream->WriteBits(u->field_10e, 8);
    stream->WriteBits(u->flags & 3, 2);
    if (u->link) {
        stream->data[stream->bit] |= 1 << stream->index;
        stream->index++;
        if (stream->index == 0x20) {
            stream->index = 0;
            stream->bit++;
            if (stream->bit == stream->capacity) {
                stream->GrowBuffer();
            }
            stream->data[stream->bit] = 0;
        }
        stream->WriteBits((!u->link ? 0 : u->link->field_a8) & 0xffff, 0xf);
        stream->WriteBits(u->field_f9, 8);
    } else {
        stream->index++;
        if (stream->index == 0x20) {
            stream->index = 0;
            stream->bit++;
            if (stream->bit == stream->capacity) {
                stream->GrowBuffer();
            }
            stream->data[stream->bit] = 0;
        }
        stream->WriteBits(u->field_6a, 0x20);
        stream->WriteBits(u->field_6e, 0x20);
        stream->WriteBits(u->field_72, 0x20);
        stream->WriteBits(u->field_66, 0x10);
        stream->WriteBits(u->field_68, 0x10);
        stream->WriteBits(u->field_64, 0x10);
        if (u->owner)
            stream->WriteBits(u->owner->field_20, 0x20);
    }
}
