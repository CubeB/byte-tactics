// Decompiled by DeepSeek V4.1 Flash, Claude Opus 5.5, retried by deepseek-v4.1-flash, space-bunny-free, Opus and Haiku. Names are provisional.

#include <string.h>

void __cdecl PacketTrace(const char* fmt, ...);

class PacketBuffer;

// One packet of a buffer (0x20 bytes).
struct Packet {
    char unknown_0[4];
    int offset;                        // +0x4
    int size;                          // +0x8
    PacketBuffer* owner;               // +0xc
    int queued;                        // +0x10
    int sentTime;                      // +0x14
    int value;                         // +0x18
    Packet* next;                      // +0x1c
};

struct PacketPool {
    char unknown_0[0x28];
    Packet* packets;                   // +0x28
    unsigned int size;                 // +0x2c
};

class Class_00462ae0 {
public:
    void RemovePacket(void* param);
};

unsigned int GetTicks();

class PacketBuffer {
public:
    PacketPool* pool;                  // +0x0
    int start;                         // +0x4, the index of its first packet
    int count;                         // +0x8
    int length;                        // +0xc
    Packet* first;                     // +0x10
    char buffer[0x416];                // +0x14

    int AppendPacket(Packet* p, int index, const void* data,
                     unsigned int size, int value);
    void FreePackets();
    void FUN_00462a40();
    int IsReusable(int minRetain);
};

// MATCH (deepseek-v4.1-flash, retry after the 96.6% baseline). The single
// missing instruction was the post-copy member reload `mov eax,[ebx+0xc]` for
// `p->offset`. The fix is the guide's "a store MSVC deletes can still move
// registers" pattern (0x461b10): after the copy, read the member into a local
// and store it straight back before using the local.
//
//     int t = length;
//     length = t;          // eliminated, but keeps `t` a memory reload
//     p->offset = t;       // emits mov eax,[ebx+0xc]; mov [esi+4],eax
//
// Without `length = t;` MSVC forwards the value loaded for the size check into
// the offset store (216 bytes, 96.6%). With the store written as
// `p->offset = length;` directly, the reload appears but the allocator hoists
// the stack arg 5 load into ecx above the offset store and rotates the
// count/length update (219 bytes, 90.7%). The dead store keeps the reload
// while leaving the rest of the allocation untouched.
//
// Appends `size` bytes at `data` to the buffer's inline storage (at +0x14),
// fills in the output packet `p`, and bumps the buffer's packet count.
// `value` is the previously queued packet (0x462710 passes its tail), stored
// in the packet's prev field at +0x18.
// FUNCTION: 0x4628d0
int PacketBuffer::AppendPacket(Packet* p, int index, const void* data,
                                 unsigned int size, int value)
{
    PacketTrace("adding packet %ld (data=\"%s\")\n", index,
                 (const char*)data + 1);
    PacketTrace("current buffer length: %ld, toadd=%ld, max=%ld\n", length,
                 size, 0x42a);
    if (length + size <= 0x42a) {
        memcpy(buffer + length, data, size);
        int t = length;
        length = t;
        p->offset = t;
        p->owner = this;
        p->size = size;
        p->value = value;
        p->next = 0;
        length += size;
        if (count++ == 0) {
            start = index;
            PacketTrace("set first packet ix to: %ld\n", index);
            first = p;
        }
        PacketTrace("assigned packet count this buf: %ld\n", count);
        return 1;
    }
    PacketTrace("out of space in buffer!\n");
    return 0;
}

// Frees the `count` packets assigned from index `start` (wrapping at the
// pool size), removing each from its owning queue, then resets the range.
// FUNCTION: 0x4629b0
void PacketBuffer::FreePackets()
{
    int n = count;
    if (n > 0) {
        unsigned int size = pool->size;
        PacketTrace("freeing packets assigned. range: %ld for %ld\n", start, n);
        unsigned int i = start;
        Packet* packets = pool->packets;
        while (n--) {
            PacketTrace("initialize freeing packet %ld\n", i);
            Packet* p = &packets[i];
            i++;
            ((Class_00462ae0*)p->owner)->RemovePacket(p);
            p->owner = 0;
            if (i >= size)
                i = 0;
        }
    }
    length = 0;
    count = 0;
    start = -1;
    first = 0;
}

// FUNCTION: 0x462a40
void PacketBuffer::FUN_00462a40()
{
    if (count != 0) {
        Packet* p = first;
        while (p != 0 && p->owner == this) {
            p = p->next;
        }
    }
}

// Returns 1 when the buffer can be reused: none of its packets is still
// queued, and none was sent within the last `minRetain` game ticks.
// FUNCTION: 0x462a60
int PacketBuffer::IsReusable(int minRetain)
{
    if (count) {
        Packet* p = first;
        unsigned int now = GetTicks();
        int mustBeSentBefore = now - minRetain;
        PacketTrace("cur game time: %ld, min retain=%ld, mustBeSentBefore=%ld\n",
                     now, minRetain, mustBeSentBefore);
        for (; p && p->owner == this; p = p->next) {
            if (p->queued >= 0)
                return 0;
            if (p->sentTime > mustBeSentBefore && p->sentTime <= now)
                return 0;
        }
        PacketTrace("buffer eligible for reuse:  all packets have been sent more than %lu game ticks ago\n",
                     minRetain);
    }
    return 1;
}
