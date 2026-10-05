// Decompiled by Opus. Names are provisional.

// Reads bit fields from an array of dwords, lowest bits first.
class BitReader {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int ReadBits(int bits);

    int ReadBit()
    {
        int r = (data[index] & (1 << bit)) != 0;
        if (++bit == 32) {
            bit = 0;
            index++;
        }
        return r;
    }
};

struct Target_0044f5c0 {
    char unknown_0[0x2e];
    unsigned char flag_0 : 1;          // +0x2e bit 0
    unsigned char flag_1 : 1;          // +0x2e bit 1
    unsigned char flag_2 : 1;          // +0x2e bit 2
    unsigned char flag_rest : 5;
};

struct Owner_0044f5c0 {
    Target_0044f5c0* target;           // +0x0
};

struct Point_0044f5c0 {
    short x;                           // +0x0
    short y;                           // +0x2
};

class Class_0044f5c0 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4
    Owner_0044f5c0* owner;             // +0x8
    Point_0044f5c0 points[3];          // +0xc
    int count;                         // +0x18

    void FUN_0044f5c0(BitReader* reader);
};

// FUNCTION: 0x44f5c0
void Class_0044f5c0::FUN_0044f5c0(BitReader* reader)
{
    owner->target->flag_2 = reader->ReadBit();
    count = reader->ReadBits(2);
    for (int i = 0; i < count; i++) {
        points[i].x = reader->ReadBits(16);
        points[i].y = reader->ReadBits(16);
    }
}
