// Decompiled by space-bunny-free. Names are provisional.
// The rest of PacketReceiver is in packet_receiver.cpp; this stays apart
// because it builds the entries through their constructors, and there the
// entries have a destructor.
//
// The real constructor of PacketReceiver (vtable 0x4fd518, scalar deleting
// destructor 0x462cc0, destructor 0x462d30). 0x4611e0.cpp already matches this
// same constructor inlined into PacketManager's constructor, so the member
// declarations are copied from there: the ten entries are the array member, and
// MSVC 5 builds their ten constructions as the loop, with the first field
// written through the array index and the rest through the walking pointer.

#include <stdlib.h>

struct Buffer_00462d30 {              // what the new below allocates
    int count;                         // +0x00
    int used;                          // +0x04
    int current;                       // +0x08
    char data[0x180c - 0x0c];

    Buffer_00462d30() { count = 0; used = 0; current = -1; }
};

struct Mid_00462d30 {
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24

    void Init() { field_18 = 0; field_1c = 0; field_20 = 0; field_24 = 0; }
};

struct FrameQueue {
    Mid_00462d30 mid;                  // +0x18
    Buffer_00462d30* buffer;           // +0x28
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30

    FrameQueue()
    {
        buffer = 0;
        mid.Init();
        field_2c = -1;
        field_30 = -1;
        buffer = new Buffer_00462d30;
    }
};

struct F0_00462d30 {
    int field_0;                       // +0x00

    F0_00462d30() { field_0 = -1; }
};

struct PlayerFrameInfo : public F0_00462d30 {
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    FrameQueue tail;                   // +0x18

    PlayerFrameInfo()
        : field_4(-1), field_8(-1), field_c(0), field_10(-1), field_14(0)
    {
    }
};

class PacketReceiver {
public:
    virtual ~PacketReceiver();
    int field_4;                       // +0x04
    void* owner;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    PlayerFrameInfo entries[10];        // +0x20
    int field_228;                     // +0x228
    int field_22c;                     // +0x22c
    int field_230;                     // +0x230
    int field_234;                     // +0x234
    int field_238;                     // +0x238

    PacketReceiver(void* o);
};

// FUNCTION: 0x462c00
PacketReceiver::PacketReceiver(void* o)
    : field_4(0), owner(o), field_c(-1), field_10(-1), field_14(0), field_18(0), field_1c(0),
      field_228(0), field_22c(0), field_230(0), field_234(-1), field_238(-1)
{
}
