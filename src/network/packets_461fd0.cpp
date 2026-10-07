// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by opus, finished by GPT-6, finished by opus. Names are provisional.
// Grows both pools of a NetBuffer: a new packet-pointer array of `growbufs` more
// packets and a new entry array of `growpackets` more entries, then moves every
// entry that still belongs to a packet into the new entry array, re-queueing the
// pending ones (the inlined 0x4623e0 and 0x461f90) and relinking the in-use list.

#include <string.h>

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* ptr);

unsigned int GetTicks();
void __cdecl PacketTrace(const char* fmt, ...);

class PacketChannel;

struct Packet_00461fd0;

struct Entry_00461fd0 {
    int field_0;                    // +0x00
    int field_4;                    // +0x04
    int field_8;                    // +0x08, the size
    Packet_00461fd0* field_c;       // +0x0c, the packet that owns it
    int field_10;                   // +0x10, >= 0 while in the pending queue
    int field_14;                   // +0x14, time it left the queue
    Entry_00461fd0* field_18;       // +0x18, previous in the global list
    Entry_00461fd0* field_1c;       // +0x1c, next in the global list
};

struct Packet_00461fd0 {
    PacketChannel* pool;            // +0x00
    int start;                      // +0x04
    int count;                      // +0x08
    int field_c;                    // +0x0c
    Entry_00461fd0* field_10;       // +0x10
    char unknown_14[0x43e - 0x14];
};

// Read side of the ring buffer at +0x38 (out of line, cf. 0x462ae0).
class Class_004623b0 {
public:
    int count;                      // +0x00
    int index;                      // +0x04
    int unused;                     // +0x08
    Entry_00461fd0* buffer[0x400];  // +0x0c

    Entry_00461fd0* PopPacket();
};

// Write side of the ring buffer at +0x38 (out of line, cf. 0x462ae0).
class PacketRing {
public:
    int count;                      // +0x00
    char unknown_4[4];
    int writeIdx;                   // +0x08
    Entry_00461fd0* buf[0x400];     // +0x0c

    int PushPacket(Entry_00461fd0* value);
};

// The same ring buffer with the push inlined here (cf. 0x461f90).
class Queue_00461fd0 {
public:
    int count;                      // +0x00
    char unknown_4[4];
    int writeIdx;                   // +0x08
    Entry_00461fd0* buf[0x400];     // +0x0c

    int Push(Entry_00461fd0* value)
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
    int field_0;                    // +0x00
    int field_4;                    // +0x04
    Packet_00461fd0** packets;      // +0x08
    unsigned int field_c;           // +0x0c, the packet pool count
    char unknown_10[4];
    int field_14;                   // +0x14
    char unknown_18[4];
    int field_1c;                   // +0x1c
    int field_20;                   // +0x20
    char unknown_24[4];
    Entry_00461fd0* entries;        // +0x28
    unsigned int count;             // +0x2c, the entry pool count
    Entry_00461fd0* field_30;       // +0x30
    Entry_00461fd0* field_34;       // +0x34
    Queue_00461fd0 queue;           // +0x38

    int GrowPools(int growbufs, int growpackets);
    void DequeuePacket(Entry_00461fd0* item);
    void EnqueuePacket(Entry_00461fd0* item);
};

void PacketChannel::DequeuePacket(Entry_00461fd0* item)
{
    item->field_10 = -1;
    item->field_14 = GetTicks();
    int n = queue.count;
    while (n-- > 0) {
        Entry_00461fd0* value = ((Class_004623b0*)&queue)->PopPacket();
        if (value == item)
            break;
        ((PacketRing*)&queue)->PushPacket(value);
    }
    field_20 -= item->field_8;
}

void PacketChannel::EnqueuePacket(Entry_00461fd0* item)
{
    queue.Push(item);
    item->field_10 = 0;
    field_20 += item->field_8;
}


// FUNCTION: 0x461fd0
int PacketChannel::GrowPools(int growbufs, int growpackets)
{
    PacketTrace("current buffer pool count: %d, current packet pool count: %d\n",
                 field_c, count);
    PacketTrace("bufs to grow by: %d, packets to grow by: %d\n", growbufs, growpackets);
    PacketTrace("current buffer pool ix: %d, current packet pool ix: %d\n",
                 field_0, field_1c);
    if (field_c <= 0 || count <= 0) {
        return 1;
    }

    // operator new/delete, not new[]: new[] references ??_U / ??_V instead.
    int total = field_c + growbufs;
    Packet_00461fd0** np = (Packet_00461fd0**)operator new(total * 4);
    if (np) {
        memset(np, 0, total * 4);
        if (field_0 >= 0) {
            int i = 0;
            int ix = field_0;
            while (i < field_c) {
                ix = ix + 1;
                if (ix >= field_c) {
                    ix = 0;
                }
                np[i] = packets[ix];
                i = i + 1;
            }
        } else {
            memcpy(np, packets, field_c * 4);
        }
        field_0 = field_c - 1;
        int ok = 1;
        while (1) {
            if (field_c >= (unsigned int)total) {
                break;
            }
            Packet_00461fd0* q = (Packet_00461fd0*)operator new(0x43e);
            if (q) {
                q->pool = this;
                q->start = -1;
                q->count = 0;
                q->field_c = 0;
                q->field_10 = 0;
            } else {
                q = 0;
            }
            np[field_c] = q;
            ok = (np[field_c] != 0);
            field_c = field_c + 1;
            if (!ok) {
                break;
            }
        }
        operator delete(packets);
        packets = np;
        if (ok) {
        int totalentries = growpackets + count;
        Entry_00461fd0* ne = (Entry_00461fd0*)operator new(totalentries * 32);
        Entry_00461fd0* base;
        if (ne) {
            Entry_00461fd0* q = ne;
            for (int i = 0; i <= totalentries - 1; i++) {
                q->field_4 = 0;
                q->field_8 = 0;
                q->field_c = 0;
                q->field_10 = -1;
                q->field_14 = 0;
                q->field_18 = 0;
                q->field_1c = 0;
                q++;
            }
            base = ne;
        } else {
            base = 0;
        }
        if (base) {
        int n = 0;
        int j;
        if (field_0 >= 0) {
            field_34 = 0;
            field_30 = 0;
            for (int i = 0; i < field_c; i++) {
                Packet_00461fd0* p = packets[i];
                if (p->count > 0) {
                    Entry_00461fd0* c = p->field_10;
                    if (c != 0) {
                        p->start = n;
                        j = 0;
                        p->field_10 = &base[n];
                        while (c != 0) {
                            if (c->field_c != p) {
                                break;
                            }
                            Entry_00461fd0* e = &base[n];
                            n++;
                            *e = *c;
                            if (c->field_10 >= 0) {
                                DequeuePacket(c);
                                EnqueuePacket(e);
                            }
                            e->field_18 = field_34;
                            e->field_c = p;
                            e->field_1c = 0;
                            // Both arms store tail = e: gives e the register weight the
                            // allocation needs; the stores are merged again.
                            if (field_34) {
                                field_34->field_1c = e;
                                field_34 = e;
                            } else {
                                field_34 = e;
                            }
                            if (!field_30) {
                                field_30 = e;
                            }
                            // One j++ in the latch: keeps its temporary in esi.
                            j++;
                            c = c->field_1c;
                        }
                        p->count = j;
                    } else {
                        p->start = -1;
                        p->count = 0;
                    }
                }
            }
        }
        operator delete(entries);
        entries = base;
        count = totalentries;
        field_1c = n - 1;
        PacketTrace("current packet pool index set to: %ld\n", n - 1);
        return 1;
        }
        }
    }
    return 0;
}
