// Decompiled by space-bunny-free, finished by muse-spark-1.3-free. Names are provisional.
// Grows both pools of a NetBuffer: a new packet-pointer array of `growbufs` more
// packets and a new entry array of `growpackets` more entries, then moves every
// entry that still belongs to a packet into the new entry array.
//
// Not byte identical yet (63.7%). Established pieces, do not undo:
//  * the queue rotate uses OUT-OF-LINE pop/push calls exactly as in 0x462ae0
//    (`int nq = qp->count; while (nq-- > 0) { x = pop(); if (x == c) break;
//    push(x); }`, MATCHED there);
//  * the final push of `e` is the INLINED Queue::Push exactly as in 0x461f90
//    (`queue.Push(e); e->field_10 = 0; field_20 += e->field_8;`, MATCHED there),
//    done unconditionally, not under `if (push)`;
//  * per packet: `p->start = n` (the running moved-count, 0 only for the first
//    packet), `Entry *q = ne + n`, `*e = *c` with `Entry *e = q++`, while-loop;
//  * the entry-init loop bound is `i <= totalentries - 1`, which is what gives
//    the original's `lea edx,[esi-1] / cmp / jl` guard (plain `<` gives cmp/jle);
//  * `totalentries` must stay a named local: recomputing `growpackets + count`
//    at all three sites flips MSVC's cached-zero register from edi to ebp and
//    drops to ~48%. The packet-grow section and log calls match.
//
// What still differs (one cascade): `totalentries` lives in ebp here but in esi
// in the original, so the init walker/counter are eax/ecx instead of ecx/edx,
// `ne` is spilled early instead of staying in eax, and the whole packet walk is
// rotated (p in eax not edx, c in ebp not eax, e spilled not in ebp, n/j slots
// swapped, plus extra xor/spill instructions).
// Tried and did NOT work: declaring the local at function top, or reusing `ok`
// as the entry count (both keep ebp, 63.7% either way); up/down/while/counting
// init-loop forms (all keep ebp); all 128 header sets (headers.py).
//
// Suspected original bug: at the packet-walk top, `p->count` is compared with
// edi (`cmp [edx+8],edi; jle`) and the freshly loaded `c` with edi
// (`cmp eax,edi; je`), but edi holds 0 only on walk entry. After any packet
// with entries is processed, edi keeps elector residue (n+1, e, or the
// queue-rotate residue), so a later packet's `count > 0` and `c == 0` tests run
// against garbage. Latent: needs at least two non-empty packets on a grow path.
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

struct Packet_00461fd0 {
    Class_00461fd0* pool;           // +0x00
    int start;                      // +0x04
    int count;                      // +0x08
    int field_c;                    // +0x0c
    int field_10;                   // +0x10
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
        } else {
            ne = 0;
        }
        if (ne) {
        int n = 0;
        if (field_0 >= 0) {
            field_34 = 0;
            field_30 = 0;
            for (int i = 0; i < field_c; i++) {
                Packet_00461fd0* p = packets[i];
                if (p->count > 0) {
                    Entry_00461fd0* c = (Entry_00461fd0*)p->field_10;
                    if (c == 0) {
                        p->start = -1;
                        p->count = 0;
                    } else {
                        p->start = n;
                        int j = 0;
                        Entry_00461fd0* q = ne + n;
                        p->field_10 = (int)q;
                        while (c != 0) {
                            if (c->field_c != (int)p) {
                                break;
                            }
                            n++;
                            Entry_00461fd0* e = q++;
                            *e = *c;
                            if (c->field_10 >= 0) {
                                c->field_10 = -1;
                                c->field_14 = FUN_004b6340();
                                Queue_00461fd0* qp = &queue;
                                int nq = qp->count;
                                while (nq-- > 0) {
                                    Entry_00461fd0* x =
                                        ((Class_004623b0*)qp)->FUN_004623b0();
                                    if (x == c) {
                                        break;
                                    }
                                    ((Class_00462370*)qp)->FUN_00462370(x);
                                }
                                field_20 -= c->field_8;
                                qp->Push(e);
                                e->field_10 = 0;
                                field_20 += e->field_8;
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
                            c = (Entry_00461fd0*)c->field_1c;
                            j++;
                        }
                        p->count = j;
                    }
                }
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
