// Decompiled by Haiku, Sonnet, Opus, space-bunny-free, deepseek-v4.1, deepseek-v4.1-flash, mimo-v2.6-pro and opus. Names are provisional.
// One player's packet channel: a pool of 0x43e-byte buffers, a pool of
// 0x20-byte packet records that point into them, and a ring of queued
// packets that SendQueued hands to the outgoing buffer.
//
// <memory.h> is needed by AllocBuffer, <windows.h> and <ddraw.h> by AllocPacket
// (see there). <new.h> is here for its symbol ids: AllocBuffer and SendQueued
// match only in windows of the symbol count.
#include <memory.h>
#include <windows.h>
#include <ddraw.h>
#include <new.h>

void* __cdecl operator new(unsigned int size);
void __cdecl PacketTrace(const char* fmt, ...);
unsigned int GetTicks();

class PacketChannel;
class PacketBuffer;

// A packet record: where its data sits in which buffer, and its queue state.
struct Packet {
    int frame;                         // +0x00
    int offset;                        // +0x04, into the buffer's data
    int size;                          // +0x08
    PacketBuffer* owner;               // +0x0c
    int queued;                        // +0x10, >= 0 while in the queue
    int sentTime;                      // +0x14, when it left the queue
    Packet* prev;                      // +0x18
    Packet* next;                      // +0x1c
};

class PacketBuffer {
public:
    PacketChannel* pool;               // +0x00
    int start;                         // +0x04
    int count;                         // +0x08
    int field_c;                       // +0x0c
    Packet* first;                     // +0x10
    unsigned char data[0x42a];         // +0x14

    void FreePackets();
    int AppendPacket(Packet* packet, int number, void* data, unsigned int length, void* prev);
};

class Class_00462ae0 {
public:
    void RemovePacket(void* param);
};

// The ring of queued packets (0x462370 is its push, 0x4623b0 its pop).
class PacketRing {
public:
    int count;                         // +0x0
    int readIdx;                       // +0x4
    int writeIdx;                      // +0x8
    Packet* buf[0x400];                // +0xc

    Packet* Peek()
    {
        if (count > 0)
            return buf[readIdx];
        return 0;
    }
    Packet* Pop()
    {
        if (count > 0) {
            count--;
            Packet* value = buf[readIdx];
            readIdx++;
            if (readIdx >= 0x400)
                readIdx = 0;
            return value;
        }
        return 0;
    }
    int Push(Packet* value)
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

class PacketManager {
public:
    int AppendToSendBuffer(unsigned char* data, unsigned int len);
};

class NetCondenser {
public:
    void SendPacketTo(void* session, int from, int value, void* data, int size);
};

extern char* g_game;
extern int* DAT_0051e2f4;
extern unsigned int DAT_0051e2f8;
extern PacketManager g_packetManager;
extern NetCondenser g_sendCondenser;

class PacketChannel {
public:
    int field_0;                       // +0x00, the buffer last handed out
    int field_4;                       // +0x04, ticks between sends
    PacketBuffer** buffers;            // +0x08
    unsigned int field_c;              // +0x0c, the buffer count
    int field_10;                      // +0x10, the frame number
    int field_14;                      // +0x14, the player's DPID
    unsigned int field_18;             // +0x18, the minimum retain time
    int field_1c;                      // +0x1c, the packet last handed out
    unsigned int field_20;             // +0x20, queued bytes
    unsigned int field_24;             // +0x24, the next send time
    Packet* packets;                   // +0x28
    unsigned int count;                // +0x2c, the packet count
    Packet* field_30;                  // +0x30, the first packet in use
    Packet* field_34;                  // +0x34, the last packet in use
    PacketRing queue;                  // +0x38

    PacketChannel();
    ~PacketChannel();
    PacketBuffer* AllocBuffer();
    Packet* AllocPacket(int param_1);
    int GetMinRetainMs();
    int GetPacketEntry(int param_1);
    int InitPools(int a1, unsigned int a2, int a3, int a4);
    void EnqueuePacket(Packet* item);
    // In packets_461fd0.cpp: its inlined DequeuePacket calls the ring's
    // out-of-line pop and push, where 0x4623e0 inlines them.
    int GrowPools(int growbufs, int growpackets);
    void DequeuePacket(Packet* item);
    void ResetChannel();
    int SendQueued(int force);
    int AddPacket(int param_1, void* param_2, unsigned int param_3);
};

// One `return` per outcome: that gives the original's un-rotated scan loop
// and its block order.
static inline int IsReusable(PacketBuffer* buf, int minRetain)
{
    if (buf->count == 0)
        return 1;
    Packet* p = buf->first;
    unsigned int now = GetTicks();
    int mustBeSentBefore = now - minRetain;
    PacketTrace("cur game time: %ld, min retain=%ld, mustBeSentBefore=%ld\n",
                 now, minRetain, mustBeSentBefore);
    for (; p != 0; p = p->next) {
        if (p->owner != buf)
            break;
        if (p->queued >= 0)
            return 0;
        if (p->sentTime > mustBeSentBefore && p->sentTime <= now)
            return 0;
    }
    PacketTrace("buffer eligible for reuse:  all packets have been sent more than %lu game ticks ago\n",
                 minRetain);
    return 1;
}

// FUNCTION: 0x461a70
PacketChannel::PacketChannel()
{
    field_0 = -1;
    buffers = 0;
    field_c = 0;
    field_10 = -2;
    field_14 = -1;
    field_1c = -1;
    field_20 = 0;
    field_24 = 0;
    packets = 0;
    count = 0;
    field_30 = 0;
    field_34 = 0;
    queue.count = 0;
    queue.readIdx = 0;
    queue.writeIdx = -1;
    field_18 = 0x78;
    field_4 = 6;
}

// Destructor of the class built by 0x461a70: frees each block of the pointer
// array at +0x8 (count at +0xc), the array itself, then the packets at +0x28.
// FUNCTION: 0x461ac0
PacketChannel::~PacketChannel()
{
    if (buffers) {
        for (unsigned int i = 0; i < field_c; i++) {
            delete buffers[i];
        }
        delete buffers;
    }
    delete packets;
}

// MATCH. The body is exactly the version deepseek-v4-flash left: the only
// change needed to reach 100% was `#include <memory.h>`, which changes the
// frame layout enough for the register allocator to restore
// `this` before the index on the reuse-success path (the two reloads at
// 0x461bcf/0x461bd3 that were swapped in the 99.0% version). The bodies of
// the earlier attempts (this->head = ix, a duplicated store, a temporary, an
// unsigned long index) were all attempts to fix that reload order by hand.
// FUNCTION: 0x461b10
PacketBuffer* PacketChannel::AllocBuffer()
{
    for (;;) {
        if (field_c > 0) {
            unsigned int ix = field_0;
            field_0 = ix;
            ++ix;
            if (ix >= field_c)
                ix = 0;
            PacketBuffer* buf = buffers[ix];
            if (IsReusable(buf, field_18)) {
                buf->FreePackets();
                field_0 = ix;
                return buf;
            }
            if (field_c >= 0x22) {
                buf->FreePackets();
                field_0 = ix;
                PacketTrace("force-allocated a previously-used buffer, ix=%ld\n", ix);
                return buf;
            }
        }
        if (GrowPools(0x10, 0x320) == 0)
            return 0;
    }
}

// The buffer scan is an inline helper with one `return` per outcome (as in
// 0x461b10), which gives the original's un-rotated scan loop and its block
// order. The stack slots of `this` and idx swap without the two headers.
// FUNCTION: 0x461c20
Packet* PacketChannel::AllocPacket(int param_1)
{
    unsigned int idx;
    Packet* packet;
    for (;;) {
        PacketTrace("current packet pool index: %ld\n", field_1c);
        idx = field_1c + 1;
        if (idx >= count)
            idx = 0;
        packet = &packets[idx];
        PacketBuffer* owner = packet->owner;
        if (owner == 0)
            break;
        if (IsReusable(owner, field_18)) {
            owner->FreePackets();
            break;
        }
        if (count >= 0x6a4) {
            PacketTrace("force-initializing a in-use buffer which was not eligible for reuse!\n");
            owner->FreePackets();
            break;
        }
        GrowPools(0, 0x320);
    }
    field_1c = idx;
    PacketTrace("current packet pool index set to: %ld\n", idx);
    packet->frame = param_1;
    return packet;
}

// FUNCTION: 0x461d80
int PacketChannel::GetMinRetainMs()
{
    unsigned int val = field_18;
    return (val / 30) * 1000;
}

// FUNCTION: 0x461da0
int PacketChannel::GetPacketEntry(int param_1)
{
    return param_1 * 0x20 + (int)packets;
}

// Matches (468 of 468 bytes). The previous attempt (space-bunny-free) had the
// whole function right except the final countdown loop: the original re-reads
// queue.count at the top of the loop (`mov eax,[ebx+0x38]; cmp eax,ebp; jle
// latch` at 0x461f43) and compares the field in memory at the latch
// (`cmp dword ptr [ebx+0x38],ebp; jne` at 0x461f61), while a plain
// `while (queue.count != 0) { if (queue.count > 0) ... }` keeps the loop's test
// value in a register across the back edge.
// The fix is to split the loop's condition from the entry guard: the guard
// tests queue.count directly, so its value is loaded into eax, while the do/while
// condition goes through a local `int* p = &queue.count`, so MSVC compares it in
// memory at the latch. The body then reloads queue.count at the top. Folding the
// guard and the loop into one `while` lets MSVC share the load and lose the
// 3 bytes.
//
// The three failure exits all jump to one shared epilogue at 0x461f78 in the
// original, which needs a single `return 0` in the source, hence `goto fail`.
// The loop counters are declared at function scope (uninitialised) so the
// jumps do not skip an initialiser.
//
// Two oddities kept as the original has them: the fresh pool entries get only
// dwords 1..7 of each 0x20-byte record initialised (0x461e11), and the reset
// copy at the end copies a stack template whose first dword is never written
// (0x461ee8 writes 0x4614..0x462c, the copy starts at 0x4610), so a new
// pool's first dword stays whatever operator new returned. It does not matter
// because nothing in this function reads it. Also `q->count = 0` before
// FreePackets() (0x461ecb) is redundant: that method sets count to 0 itself
// (0x4629b0.cpp).
// FUNCTION: 0x461db0
int PacketChannel::InitPools(int a1, unsigned int a2, int a3, int a4)
{
    unsigned int i;
    unsigned int j;
    int k;
    int* p;
    field_14 = a1;
    field_4 = (a2 * 30 + 999) / 1000;
    if (packets == 0) {
        if (a4 == 0)
            a4 = 100;
        Packet* p = (Packet*)operator new(a4 * 32);
        if (p) {
            // A countdown over a separate walking pointer: this spelling is
            // what gives `lea edx,[esi-1] / cmp / jl` and `inc edx` at 0x461e06.
            k = a4 - 1;
            Packet* q = p;
            while (k >= 0) {
                q->offset = 0;
                q->size = 0;
                q->owner = 0;
                q->queued = -1;
                q->sentTime = 0;
                q->prev = 0;
                q->next = 0;
                q++;
                k--;
            }
        } else {
            p = 0;
        }
        packets = p;
        if (p == 0)
            goto fail;
        count = a4;
    }
    if (buffers == 0) {
        if (a3 == 0)
            a3 = 2;
        buffers = (PacketBuffer**)operator new(a3 * 4);
        if (buffers == 0)
            goto fail;
        for (field_c = 0; field_c < a3; field_c++) {
            PacketBuffer* q = (PacketBuffer*)operator new(0x43e);
            if (q) {
                q->pool = this;
                q->start = -1;
                q->count = 0;
                q->field_c = 0;
                q->first = 0;
            } else {
                q = 0;
            }
            buffers[field_c] = q;
            if (buffers[field_c] == 0)
                goto fail;
        }
    }
    for (i = 0; i < field_c; i++) {
        // The pointer local is what puts the store in eax and the call in ecx.
        PacketBuffer* q = buffers[i];
        q->count = 0;
        buffers[i]->FreePackets();
    }
    Packet t;
    t.offset = 0;
    t.size = 0;
    t.owner = 0;
    t.queued = -1;
    t.sentTime = 0;
    t.prev = 0;
    t.next = 0;
    for (j = 0; j < count; j++)
        packets[j] = t;
    field_0 = -1;
    field_1c = -1;
    field_30 = 0;
    field_34 = 0;
    // p keeps the latch's test a memory operand, so the body's own test at
    // the top of the loop reloads queue.count (0x461f43).
    p = &queue.count;
    if (queue.count != 0) {
        do {
            if (queue.count > 0) {
                queue.count--;
                queue.readIdx++;
                if (queue.readIdx >= 0x400)
                    queue.readIdx = 0;
            }
        } while (*p != 0);
    }
    field_20 = 0;
    return 1;
fail:
    return 0;
}

// Queues an item in the ring buffer at +0x38 (the inlined push of
// 0x462370), marks it queued and adds its size to the total.
// FUNCTION: 0x461f90
void PacketChannel::EnqueuePacket(Packet* item)
{
    queue.Push(item);
    item->queued = 0;
    field_20 += item->size;
}

// FUNCTION: 0x4623e0
void PacketChannel::DequeuePacket(Packet* item)
{
    item->queued = -1;
    item->sentTime = GetTicks();
    int n = queue.count;
    while (n-- > 0) {
        Packet* value = queue.Pop();
        if (value == item)
            break;
        queue.Push(value);
    }
    field_20 -= item->size;
}

// FUNCTION: 0x462470
void PacketChannel::ResetChannel()
{
    InitPools(-1, 0xc8, 2, 0x64);
    field_0 = -1;
    field_1c = -1;
    field_24 = 0;
}

// Sends one player's queued packets: every packet whose frame matches the
// queue head's is appended to the outgoing buffer (g_packetManager, whose
// buffer and size fields are DAT_0051e2f4 and DAT_0051e2f8), the others go
// back on the queue, and the buffer is handed to g_sendCondenser with the frame
// and the player's DPID.
//
// What matched it (opus, #4310), rebuilt from the disassembly instead of the
// 81.7% file's address-escape and do/while(0) devices:
//  * A packet's data is `owner->data[offset]`: +0x4 is the offset and +0xc
//    the owning buffer (PacketBuffer, data at +0x14). Written twice as an
//    expression, it gives the original's base+offset for the log call and
//    the fresh reload after GetTicks, with the zero kept in ebx.
//  * `if (now >= nextSend || force != 0) { ...; while ((n = queue.count) != 0)
//    {...} } return 1;` gives the original's three epilogues.
//  * The send block reads DAT_0051e2f8, dpid and DAT_0051e2f4 into locals
//    before the log call, in that order: equal priorities, so the one written
//    first takes edi (`nbytes` before `id`; 97.5% the other way round).
// FUNCTION: 0x4624a0
int PacketChannel::SendQueued(int force)
{
    unsigned int now = GetTicks();
    PacketTrace("player: %ld, ticks betw sends=%lu, nextsend=%lu, gametimereal=%lu\n",
                 field_14, field_4, field_24, now);
    if (now >= field_24 || force != 0) {
        field_24 = now + field_4;
        int n;
        while ((n = queue.count) != 0) {
            int headFrame = queue.Peek()->frame;
            int sent = 0;
            PacketTrace("assigning packets to frame number: %ld\n", field_10);
            for (int i = 0; i < n; i++) {
                Packet* entry = queue.Peek();
                queue.Pop();
                if (entry->frame == headFrame) {
                    PacketTrace("extracted packet (len=%ld, type=%d, data=\"%s\")\n",
                                 entry->size, entry->owner->data[entry->offset],
                                 &entry->owner->data[entry->offset + 1]);
                    entry->queued = field_10;
                    entry->sentTime = GetTicks();
                    if (g_packetManager.AppendToSendBuffer(&entry->owner->data[entry->offset], entry->size) == 0)
                        return 0;
                    sent++;
                } else {
                    queue.Push(entry);
                }
            }
            field_20 = 0;
            if (sent > 0) {
                PacketTrace("sending %ld packets in frame: %ld\n", sent, field_10);
                *DAT_0051e2f4 = field_14 != 0 ? -1 : field_10;
                unsigned int nbytes = DAT_0051e2f8;
                int id = field_14;
                int* data = DAT_0051e2f4;
                PacketTrace("bytes to send to (DPID)(%ld): %ld\n", id, nbytes);
                g_sendCondenser.SendPacketTo(g_game + 0x14, headFrame, id, data, nbytes);
                DAT_0051e2f8 = DAT_0051e2f4 != 0 ? 4 : 0;
                field_10--;
                if (field_10 >= -1)
                    field_10 = -2;
                if (queue.count == 0)
                    return 1;
            }
        }
    }
    return 1;
}

// FUNCTION: 0x462710
int PacketChannel::AddPacket(int param_1, void* param_2, unsigned int param_3)
{
    if (field_20 >= 0x42a || queue.count == 0x400) {
        SendQueued(1);
        if (queue.count != 0)
            return 0;
    }
    Class_00462ae0* block = 0;
    int idx = field_0;
    if (idx >= 0)
        block = (Class_00462ae0*)buffers[idx];
    if (block == 0) {
        block = (Class_00462ae0*)AllocBuffer();
        if (block == 0)
            return 0;
    }
    Packet* pkt = AllocPacket(param_1);
    if (pkt == 0)
        return 0;
    int r = ((PacketBuffer*)block)->AppendPacket(pkt, field_1c, param_2, param_3, field_34);
    if (r == 0) {
        field_1c = field_1c - 1;
        block = (Class_00462ae0*)AllocBuffer();
        if (block != 0) {
            pkt = AllocPacket(param_1);
            if (pkt != 0)
                r = ((PacketBuffer*)block)->AppendPacket(pkt, field_1c, param_2, param_3, field_34);
            else
                r = 0;
        }
    }
    if (r != 0) {
        if (field_30 == 0)
            field_30 = pkt;
        if (field_34 != 0)
            field_34->next = pkt;
        field_34 = pkt;
        queue.Push(pkt);
        pkt->queued = 0;
        field_20 += pkt->size;
        return 1;
    }
    if (block != 0 && pkt->owner == (PacketBuffer*)block)
        block->RemovePacket(pkt);
    return 0;
}
