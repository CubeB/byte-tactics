// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by opus, finished by GPT-6, finished by opus. Names are provisional.
// Grows both pools of a NetBuffer: a new packet-pointer array of `growbufs` more
// packets and a new entry array of `growpackets` more entries, then moves every
// entry that still belongs to a packet into the new entry array, re-queueing the
// pending ones (the inlined 0x4623e0 and 0x461f90) and relinking the in-use list.
//
// MATCH (#5283). The earlier 99.0% file duplicated `j++` into both arms of the
// head test, which got the global allocation right but left j's latch
// temporary in ecx instead of esi. That temporary is picked by C2's
// Pentium-pairing rename pass (FUN_0042b3e2), which walks each codegen block
// from the bottom up and hands every flagged tuple the next free register of
// eax, ecx, edx, esi, edi, ebx, ebp from a pointer reset at each block. With a
// single `j++` in the latch, the `c != 0` compare below it takes ecx first, so
// j++ gets esi as in the original. With j++ in the latch, though, c outranked e
// (516 against 496 in tools/c2prio.py) and took ebp. The tail append written
// as `if (tail) { tail->next = e; tail = e; } else { tail = e; }` gives e the
// extra weight (c 500, e 528); MSVC merges the two `tail = e` stores again, so
// the code is unchanged apart from the allocation.
//
// The allocations call `operator new` / `operator delete` (??2 / ??3, the
// names data/symbols.csv has at 0x4b4f10 / 0x4b4f20); `operator new[]`
// compiles to the same code but references ??_U / ??_V.

#include <string.h>

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* ptr);

unsigned int FUN_004b6340();
void __cdecl FUN_00461170(const char* fmt, ...);

class Class_00461fd0;

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
    Class_00461fd0* pool;           // +0x00
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

    Entry_00461fd0* FUN_004623b0();
};

// Write side of the ring buffer at +0x38 (out of line, cf. 0x462ae0).
class Class_00462370 {
public:
    int count;                      // +0x00
    char unknown_4[4];
    int writeIdx;                   // +0x08
    Entry_00461fd0* buf[0x400];     // +0x0c

    int FUN_00462370(Entry_00461fd0* value);
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

class Class_00461fd0 {
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

    int FUN_00461fd0(int growbufs, int growpackets);
    void FUN_004623e0(Entry_00461fd0* item);
    void FUN_00461f90(Entry_00461fd0* item);
};

void Class_00461fd0::FUN_004623e0(Entry_00461fd0* item)
{
    item->field_10 = -1;
    item->field_14 = FUN_004b6340();
    int n = queue.count;
    while (n-- > 0) {
        Entry_00461fd0* value = ((Class_004623b0*)&queue)->FUN_004623b0();
        if (value == item)
            break;
        ((Class_00462370*)&queue)->FUN_00462370(value);
    }
    field_20 -= item->field_8;
}

void Class_00461fd0::FUN_00461f90(Entry_00461fd0* item)
{
    queue.Push(item);
    item->field_10 = 0;
    field_20 += item->field_8;
}


// FUNCTION: 0x461fd0
int Class_00461fd0::FUN_00461fd0(int growbufs, int growpackets)
{
    FUN_00461170("current buffer pool count: %d, current packet pool count: %d\n",
                 field_c, count);
    FUN_00461170("bufs to grow by: %d, packets to grow by: %d\n", growbufs, growpackets);
    FUN_00461170("current buffer pool ix: %d, current packet pool ix: %d\n",
                 field_0, field_1c);
    if (field_c <= 0 || count <= 0) {
        return 1;
    }

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
                                FUN_004623e0(c);
                                FUN_00461f90(e);
                            }
                            e->field_18 = field_34;
                            e->field_c = p;
                            e->field_1c = 0;
                            if (field_34) {
                                field_34->field_1c = e;
                                field_34 = e;
                            } else {
                                field_34 = e;
                            }
                            if (!field_30) {
                                field_30 = e;
                            }
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
        FUN_00461170("current packet pool index set to: %ld\n", n - 1);
        return 1;
        }
        }
    }
    return 0;
}
