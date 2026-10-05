// Decompiled by Opus. Names are provisional.

unsigned int __cdecl GetTicks();

// The ring buffer of 0x462370 (push) and 0x4623b0 (pop), inlined here.
class Queue_004623e0 {
public:
    int count;                         // +0x0
    int readIdx;                       // +0x4
    int writeIdx;                      // +0x8
    int buf[0x400];                    // +0xc

    int Push(int value)
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

    int Pop()
    {
        if (count > 0) {
            count--;
            int value = buf[readIdx];
            readIdx++;
            if (readIdx >= 0x400)
                readIdx = 0;
            return value;
        }
        return 0;
    }
};

struct Item_004623e0 {
    char unknown_0[0x8];
    int size;                          // +0x8
    char unknown_c[0x10 - 0xc];
    int field_10;                      // +0x10
    unsigned int time;                 // +0x14
};

class PacketChannel {
public:
    char unknown_0[0x20];
    int total;                         // +0x20
    char unknown_24[0x38 - 0x24];
    Queue_004623e0 queue;              // +0x38

    void DequeuePacket(Item_004623e0* item);
};

// FUNCTION: 0x4623e0
void PacketChannel::DequeuePacket(Item_004623e0* item)
{
    item->field_10 = -1;
    item->time = GetTicks();
    int n = queue.count;
    while (n-- > 0) {
        int value = queue.Pop();
        if (value == (int)item)
            break;
        queue.Push(value);
    }
    total -= item->size;
}
