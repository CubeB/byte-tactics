// Decompiled by Opus. Names are provisional.
// Queues an item in the ring buffer at +0x38 (the inlined push of
// 0x462370), clears its +0x10 field and adds its +0x8 size to the total.

struct Item_00461f90 {
    char unknown_0[8];
    int size;                          // +0x8
    char unknown_c[4];
    int field_10;                      // +0x10
};

struct Queue_00461f90 {
    int count;                         // +0x0
    char unknown_4[4];
    int writeIdx;                      // +0x8
    Item_00461f90* buf[0x400];         // +0xc

    int Push(Item_00461f90* value)
    {
        if (count < 0x400) {
            writeIdx = writeIdx + 1;
            if (writeIdx >= 0x400) {
                writeIdx = 0;
            }
            buf[writeIdx] = value;
            count = count + 1;
            return 1;
        }
        return 0;
    }
};

class PacketChannel {
public:
    char unknown_0[0x20];
    int total;                         // +0x20
    char unknown_24[0x38 - 0x24];
    Queue_00461f90 queue;              // +0x38

    void EnqueuePacket(Item_00461f90* item);
};

// FUNCTION: 0x461f90
void PacketChannel::EnqueuePacket(Item_00461f90* item)
{
    queue.Push(item);
    item->field_10 = 0;
    total += item->size;
}
