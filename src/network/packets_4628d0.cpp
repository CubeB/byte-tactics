// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5, retried by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
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
#include <string.h>

void __cdecl PacketTrace(const char* fmt, ...);

class PacketBuffer;

struct Packet_004628d0 {
    char unknown_0[4];
    int offset;                        // +4
    int size;                          // +8
    PacketBuffer* owner;               // +0xc
    char unknown_10[8];                // +0x10
    int value;                         // +0x18
    Packet_004628d0* next;             // +0x1c
};

class PacketBuffer {
public:
    char unknown_0[4];                 // +0
    int firstIndex;                    // +4
    int count;                         // +8
    int length;                        // +0xc
    Packet_004628d0* first;            // +0x10
    char buffer[0x416];                // +0x14

    int AppendPacket(Packet_004628d0* p, int index, const void* data,
                     unsigned int size, int value);
};

// FUNCTION: 0x4628d0
int PacketBuffer::AppendPacket(Packet_004628d0* p, int index, const void* data,
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
            firstIndex = index;
            PacketTrace("set first packet ix to: %ld\n", index);
            first = p;
        }
        PacketTrace("assigned packet count this buf: %ld\n", count);
        return 1;
    }
    PacketTrace("out of space in buffer!\n");
    return 0;
}
