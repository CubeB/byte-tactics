// Decompiled by Space Bunny Free. Names are provisional.
// Slot 8 of Class_0044f010 (vtable 0x4fd458, see 0x44f450.cpp), the write
// counterpart of the reader 0x44f5c0. It sets the stream's next bit when the
// unit's mode asks for it, writes the point count in 2 bits, then up to three
// points of 16 bits each, and finally copies the unit's mode into flag 2 while
// clearing flag 3 (the "changed" flag the readers rely on).
// The tail only compiles to the original's `and 0xf3` / `xor` pair when the
// two flag bits are written as 1-bit bitfields: assigning the mode as an int
// value gives `and/or` instead.

// The bit writer of 0x415c10.
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

struct Target_0044f4a0 {
    char unknown_0[0x2e];
    unsigned char field_2e;
};

struct Link_0044f4a0 {
    Target_0044f4a0* target;           // +0x0
};

struct Point_0044f4a0 {
    short x;
    short y;
};

class Class_0044f010 {
public:
    int field_4;                       // +0x4
    Link_0044f4a0* owner;              // +0x8
    Point_0044f4a0 points[20];         // +0xc
    int count;                         // +0x5c
    int field_60;                      // +0x60
    unsigned char active : 1;          // +0x64 bit 0
    unsigned char flag_1 : 1;          // bit 1
    unsigned char flag_2 : 1;          // bit 2
    unsigned char flag_3 : 1;          // bit 3

    virtual void FUN_0044efc0(BitWriter* stream);       // slot 8
};

// FUNCTION: 0x44f4a0
void Class_0044f010::FUN_0044efc0(BitWriter* stream)
{
    int n;
    if (active) {
        n = count < 3 ? count : 3;
    } else {
        n = 0;
    }
    if (owner->target->field_2e & 4) {
        stream->data[stream->bit] |= 1 << stream->index;
    }
    stream->index++;
    if (stream->index == 0x20) {
        stream->index = 0;
        stream->bit++;
        if (stream->bit == stream->capacity) {
            stream->GrowBuffer();
        }
        stream->data[stream->bit] = 0;
    }
    stream->WriteBits(n, 2);
    for (int i = 0; i < n; i++) {
        stream->WriteBits(points[i].x, 0x10);
        stream->WriteBits(points[i].y, 0x10);
    }
    flag_2 = (owner->target->field_2e & 4) != 0;
    flag_3 = 0;
}
