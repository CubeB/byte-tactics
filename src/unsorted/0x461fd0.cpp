// Decompiled by space-bunny-free. Names are provisional.
// Grows both pools of a NetBuffer: a new packet-pointer array of `growbufs` more
// packets and a new entry array of `growpackets` more entries, then moves every
// entry that still belongs to a packet into the new entry array.
//
// Not byte identical yet. Still differs from the original:
//  * `totalentries` (growpackets + count) lives in a register (ebp) here; the
//    original keeps it in the stack slot at [esp+0x24] and holds the new[] result
//    in eax, so ebp is still free for `ne`.
//  * the entry-init loop's entry guard is `lea edx,[total-1]` / `test` / `jl` plus
//    a copy of the counter, where the original reuses the same register for the
//    guard and the down counter (`lea edx,[esi-1] / cmp edx,edi / jl / inc edx`).
//  * the `ne == 0` phi writes the slot and jumps where the original falls through.
//  * the packet walk's locals (n, i, q, p, j, c) are still partly in registers;
//    the original has all six in stack slots because all four callee-saved
//    registers are taken (ebx=this, ebp=ne/q, esi=&queue, edi=0).
// Tried and did NOT work: making every `return 0` its own early exit (MSVC then
// inlines a full epilogue at each site, 3 extra epilogues, 38%); a signed
// `(int)field_c` compare in the rotate-copy loop (gives `jle`, the original has
// `jb`, so the pool count must be an unsigned member).

#include <string.h>

void* operator new[](unsigned int size);
void operator delete[](void* ptr);

unsigned int FUN_004b6340();
void FUN_00461170(const char* fmt, ...);

class Class_00461fd0;

struct Entry_00461fd0 {
    int field_0;                    // +0x00
    int field_4;                    // +0x04
    int field_8;                    // +0x08, the size
    int field_c;                    // +0x0c, the packet that owns it
    int field_10;                   // +0x10, next in the packet chain
    int field_14;                   // +0x14
    int field_18;                   // +0x18, previous in the global list
    int field_1c;                   // +0x1c, next in the global list
};

class Class_00462370 {
public:
    int count;                      // +0x00
    char unknown_4[4];
    int writeIdx;                   // +0x08
    Entry_00461fd0* buf[0x400];     // +0x0c

    int FUN_00462370(Entry_00461fd0* v)
    {
        if (count < 0x400) {
            writeIdx = writeIdx + 1;
            if (writeIdx >= 0x400) {
                writeIdx = 0;
            }
            buf[writeIdx] = v;
            count = count + 1;
            return 1;
        }
        return 0;
    }
};

class Class_004623b0 {
public:
    int count;
    int index;
    int unused;
    Entry_00461fd0* buffer[0x400];

    Entry_00461fd0* FUN_004623b0();
};

struct Packet_00461fd0 {
    Class_00461fd0* pool;           // +0x00
    int start;                      // +0x04
    int count;                      // +0x08
    int field_c;                    // +0x0c
    int field_10;                   // +0x10
    char unknown_14[0x43e - 0x14];
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
    Class_00462370 queue;           // +0x38

    int FUN_00461fd0(int growbufs, int growpackets);
};

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
    Packet_00461fd0** np = (Packet_00461fd0**)operator new[](total * 4);
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
            Packet_00461fd0* q = (Packet_00461fd0*)operator new[](0x43e);
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
        operator delete[](packets);
        packets = np;
        if (ok) {
            int totalentries = growpackets + count;
            Entry_00461fd0* ne = (Entry_00461fd0*)operator new[](totalentries * 32);
            if (ne) {
                int i = 0;
                while (i < totalentries) {
                    Entry_00461fd0* q = ne + i;
                    q->field_4 = 0;
                    q->field_8 = 0;
                    q->field_c = 0;
                    q->field_10 = -1;
                    q->field_14 = 0;
                    q->field_18 = 0;
                    q->field_1c = 0;
                    i = i + 1;
                }
            } else {
                ne = 0;
            }
            if (ne) {
                int n = 0;
                if (field_0 >= 0) {
                    field_34 = 0;
                    field_30 = 0;
                    int i = 0;
                    while (i < field_c) {
                        Packet_00461fd0* p = packets[i];
                        if (p->count > 0) {
                            Entry_00461fd0* c = (Entry_00461fd0*)p->field_10;
                            if (c == 0) {
                                p->start = -1;
                                p->count = 0;
                            } else {
                                p->start = 0;
                                int j = 0;
                                Entry_00461fd0* q = ne + j * 32;
                                p->field_10 = (int)q;
                                do {
                                    if (c->field_c != (int)p) {
                                        break;
                                    }
                                    n = n + 1;
                                    Entry_00461fd0* e = q;
                                    *q = *c;
                                    q = q + 1;
                                    if (c->field_10 >= 0) {
                                        c->field_10 = -1;
                                        c->field_14 = FUN_004b6340();
                                        int cnt = queue.count;
                                        while (cnt > 0) {
                                            cnt = cnt - 1;
                                            Entry_00461fd0* x =
                                                ((Class_004623b0*)&queue)->FUN_004623b0();
                                            if (x == c) {
                                                break;
                                            }
                                            queue.FUN_00462370(x);
                                        }
                                        field_20 = field_20 - c->field_8;
                                        if (queue.FUN_00462370(e)) {
                                            e->field_10 = 0;
                                            field_20 = field_20 + e->field_8;
                                        }
                                    }
                                    e->field_c = (int)p;
                                    e->field_18 = (int)field_34;
                                    e->field_1c = 0;
                                    if (field_34) {
                                        field_34->field_1c = (int)e;
                                    }
                                    field_34 = e;
                                    if (field_30 == 0) {
                                        field_30 = e;
                                    }
                                    j = j + 1;
                                    c = (Entry_00461fd0*)c->field_1c;
                                } while (c);
                                p->count = j;
                            }
                        }
                        i = i + 1;
                    }
                }
                operator delete[](entries);
                entries = ne;
                count = totalentries;
                field_1c = n - 1;
                FUN_00461170("current packet pool index set to: %ld\n", n - 1);
                return 1;
            }
        }
    }
    return 0;
}
