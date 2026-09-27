// Decompiled by Opus. Names are provisional.

struct Buffer_004635b0 {
    int count;                         // +0x0
    int used;                          // +0x4
    int current;                       // +0x8
    char data[0x180c - 0xc];

    Buffer_004635b0() { count = 0; used = 0; current = -1; }
};

// The two halves of the object are members with their own inline
// constructors; that is what places the operator new argument push after
// the first six stores.
struct Head_004635b0 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14

    Head_004635b0() { field_0 = -1; field_4 = -1; field_8 = -1; field_c = 0; field_10 = -1; field_14 = 0; }
};

struct Tail_004635b0 {
    int field_0;                       // +0x18
    int field_4;                       // +0x1c
    int field_8;                       // +0x20
    int field_c;                       // +0x24
    Buffer_004635b0* buffer;           // +0x28
    int field_14;                      // +0x2c
    int field_18;                      // +0x30

    Tail_004635b0() { buffer = 0; field_0 = 0; field_4 = 0; field_8 = 0; field_c = 0; field_14 = -1; field_18 = -1; }
};

class Class_004635b0 {
public:
    Head_004635b0 head;                // +0x00
    Tail_004635b0 tail;                // +0x18

    Class_004635b0();
};

// FUNCTION: 0x4635b0
Class_004635b0::Class_004635b0()
{
    tail.buffer = new Buffer_004635b0;
}
