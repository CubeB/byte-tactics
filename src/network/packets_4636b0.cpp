// Decompiled by Opus. Names are provisional.

struct Buffer_004636b0 {
    int count;                         // +0x0
    int used;                          // +0x4
    int current;                       // +0x8
    char data[0x180c - 0xc];

    Buffer_004636b0() { count = 0; used = 0; current = -1; }
};

// Same layout as the FrameQueue member of PlayerFrameInfo, as a class of
// its own with the stores in the constructor body.
class FrameQueue {
public:
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    Buffer_004636b0* buffer;           // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18

    FrameQueue();
};

// FUNCTION: 0x4636b0
FrameQueue::FrameQueue()
{
    buffer = 0;
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_c = 0;
    field_14 = -1;
    field_18 = -1;
    buffer = new Buffer_004636b0;
}
