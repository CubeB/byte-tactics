// Decompiled by Opus, deepseek-v4.1-flash, GPT-6, space-bunny-free and fledge-alpha-free. Names are provisional.

#include <string.h>

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

// Bit reader, see src/network/net_stats.cpp.
class BitReader {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int ReadBits(int bits);
};

// Length of each packet command, one word per 4-byte entry.
extern unsigned short DAT_00512ad8[][2];

// One queued frame of a player's ring: the tick it is due, the data and size.
struct Frame_00463790 {
    int tick;                          // +0x0
    void* data;                        // +0x4
    int size;                          // +0x8
};

// The queued frames, a ring of 0x200.
struct FrameRing {                     // 0x180c bytes
    int count;                         // +0x0
    int head;                          // +0x4
    int tail;                          // +0x8
    Frame_00463790 frames[0x200];      // +0xc

    FrameRing() { count = 0; head = 0; tail = -1; }

    // Pop and Push stay inline members: the requeue loop's registers follow.
    Frame_00463790* Pop()
    {
        if (count > 0) {
            count--;
            Frame_00463790* f = &frames[head];
            if (++head >= 0x200)
                head = 0;
            return f;
        }
        return 0;
    }

    int Push(int tick, void* data, int size)
    {
        if (count >= 0x200)
            return 0;
        if (++tail >= 0x200)
            tail = 0;
        frames[tail].data = data;
        frames[tail].tick = tick;
        frames[tail].size = size;
        count++;
        return 1;
    }
};

class FrameQueue {
public:
    int field_0;                       // +0x00
    unsigned int field_4;              // +0x04
    int field_8;                       // +0x08
    char* field_c;                     // +0x0c
    FrameRing* buffer;                 // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18

    int Count() { return buffer ? buffer->count : 0; }

    ~FrameQueue();
    FrameQueue();
    int ResetFrames();
    int QueueFrames(char* src, unsigned int size, int tick, int a4, int a5, int a6);
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
    buffer = new FrameRing;
}

// Destructor of the class built by 0x4636b0 (the same layout as the
// FrameQueue member of PlayerFrameInfo, whose fields 0x463680 frees in the
// same order): frees the buffer at +0x10, then the block at +0xc.
// FUNCTION: 0x463710
FrameQueue::~FrameQueue()
{
    operator delete(buffer);
    operator delete(field_c);
}

// Resets the object and allocates its 0x180c-byte buffer if it has none;
// returns 1 when the buffer already existed.
// FUNCTION: 0x463730
int FrameQueue::ResetFrames()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_c = 0;
    field_14 = -1;
    field_18 = -1;
    if (!buffer) {
        buffer = new FrameRing;
        return 0;
    }
    return 1;
}

// Queues one received packet's commands in the ring at +0x10. If frames are
// already queued, it only re-stamps each of them with the new tick (pop, push) and
// returns 0. Otherwise it copies the packet into the buffer at +0xc, counts the
// commands after the 4-byte sequence number (a command is 2..0x2c; 0x2c carries its
// own 16-bit length, the others' lengths are in the table at 0x512ad8), skips the
// first n - 0x200 0x2c commands when there are more than 0x200 commands, and pushes
// the rest: spread over up to 30 ticks when a6 is set, all at `tick` otherwise.
// FUNCTION: 0x463790
int FrameQueue::QueueFrames(char* src, unsigned int size, int tick, int a4, int a5, int a6)
{
    if (size <= 0)
        return 1;
    if (Count() != 0) {
        field_8++;
        int n = buffer->count;
        while (n--) {
            Frame_00463790 f = *buffer->Pop();
            buffer->Push(tick, f.data, f.size);
        }
        return 0;
    }

    field_8 = 0;
    if (size > field_4) {
        // delete/new expressions, not operator calls: fixes the temporary rotation.
        delete field_c;
        field_c = new char[size + 0x100];
        if (field_c == 0) {
            field_4 = 0;
            return 1;
        }
        field_4 = size + 0x100;
    }
    memcpy(field_c, src, size);
    field_14 = a4;
    field_18 = a5;
    size -= 4;

    // Declared in this order: gives the original's reload of this for field_c.
    int n = 0;
    int remaining = size;
    char* p = field_c + 4;
    while (remaining > 0) {
        unsigned char c = *p;
        if (c <= 1 || c >= 0x2d)
            break;
        unsigned short w;
        if (c == 0x2c) {
            BitReader reader;
            reader.data = (unsigned int*)p;
            reader.index = 0;
            reader.bit = 0;
            reader.ReadBits(8);
            w = (unsigned short)reader.ReadBits(0x10);
        } else {
            w = DAT_00512ad8[c][0];
        }
        remaining -= w;
        if (remaining < 0)
            break;
        p += w;
        n++;
    }

    if (n > 0) {
        int left = n - 0x200;
        if (a6 != 0) {
            int span = tick - field_0;
            if (span > 0x1e)
                span = 0x1e;
            else if (span <= 0)
                span = 1;
            int spacing = 0x10;
            if (n > span)
                spacing = (n << 4) / span;
            // x is its own local, defined before progress, i and q.
            int x = tick;
            unsigned int progress = 0;
            unsigned int i = 0;
            char* q = field_c + 4;
            // Both tails end `remaining -= w; q += w; n--;` and the loop tests n > 0.
            do {
                unsigned char c = *q;
                unsigned short w;
                if (c == 0x2c) {
                    BitReader reader;
                    reader.data = (unsigned int*)q;
                    reader.index = 0;
                    reader.bit = 0;
                    reader.ReadBits(8);
                    w = (unsigned short)reader.ReadBits(0x10);
                    if (left > 0) {
                        left--;
                        remaining -= w;
                        q += w;
                        n--;
                        continue;
                    }
                } else {
                    w = DAT_00512ad8[c][0];
                }
                if (!buffer->Push(x, q, w))
                    return 1;
                i++;
                if ((i << 4) >= progress) {
                    progress += spacing;
                    x++;
                }
                remaining -= w;
                q += w;
                n--;
            } while (n > 0);
            return 1;
        }

        char* q = field_c + 4;
        int rem = size;
        // Exits are `break` to the single return 1 below.
        while (rem > 0) {
            unsigned char c = *q;
            if (c <= 1 || c >= 0x2d)
                break;
            unsigned short w;
            if (c == 0x2c) {
                BitReader reader;
                reader.data = (unsigned int*)q;
                reader.index = 0;
                reader.bit = 0;
                reader.ReadBits(8);
                w = (unsigned short)reader.ReadBits(0x10);
                if (left > 0) {
                    left--;
                    rem -= w;
                    q += w;
                    goto next2;
                }
            } else {
                w = DAT_00512ad8[c][0];
            }
            rem -= w;
            if (rem < 0)
                break;
            if (!buffer->Push(tick, q, w))
                break;
            q += w;
        next2:
            ;
        }
    }
    return 1;
}
