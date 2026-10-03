// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by opus, finished by GPT-6. Names are provisional.
// Grows both pools of a NetBuffer: a new packet-pointer array of `growbufs` more
// packets and a new entry array of `growpackets` more entries, then moves every
// entry that still belongs to a packet into the new entry array.
//
// PARTIAL, 99.0% (919 of 919 bytes, size exact). Pass by opus (issue 5023), up
// from 86.8%. Every frame slot and register now matches the original (base 0x10,
// n 0x14, i 0x18, q 0x1c, p 0x20, totalentries 0x24, j in the dead growbufs slot
// 0x2c, c in the dead growpackets slot 0x30 with an EAX copy, e in EBP). Duplicating
// `j++` in both arms of the `field_30` test puts it in the original's latch while
// keeping the register allocation. The remaining diff is the latch temporary:
// this uses ECX where the original uses ESI.
//
// What got it from 86.8 to 98.4, all semantically correct (the old file kept a
// redundant `if (c == 0) break;` at the loop top):
//  1. The walk is a plain `while (c != 0) { if (c->field_c != (int)p) break; ...}`
//     inside `if (c != 0) { walk } else { p->start = -1; p->count = 0; }`. MSVC
//     drops the first `c != 0` test (known from the `if`), rotates the loop and
//     builds the original's two-entry head (`jmp` past the c/q reload block).
//  2. `n++;` BEFORE `Entry* e = q++;` (the original loads n first, 0x4621eb).
//  3. Duplicating the `j++` in both arms of the `field_30` test moves it to the
//     latch without changing EBP's assignment to `e`; one increment before the
//     test gives 98.4%.
//  4. `int j = 0;` in the walk arm, right after `p->start = n;` (store at 0x4621c9).
//  5. `p->field_10 = (int)(base + n); Entry* q = (Entry*)p->field_10;`: reading
//     q back from the field is what gives `shl esi,5 / lea ecx,[esi+ebp]` (n dies
//     at the shift) instead of `mov ecx,esi / shl ecx,5 / add ecx,ebp`.
//  6. Tail order `count = totalentries;` before `field_1c = n - 1;`.
//
// The allocation (e vs c for EBP) sits on a knife edge. Measured on the
// j++-in-latch form (69.5%): the inline Push written with a branchless ternary
// `writeIdx = (writeIdx + 1 >= 0x400) ? 0 : writeIdx + 1;` puts j++ AND the
// allocation right (92.1%) but emits setge/dec/and where the original has the
// branch, so the original's Push really has the `if`. Not levers on that form
// (all 69.5%, byte-identical allocation): helpers for 0x4623e0/0x461f90
// (RemovePending/AddPending inlined), a list-append helper, `&queue` without
// the `qp` local, a `for` rotation loop, memcpy for the copy, every order of
// the three `e->field_*` stores, `field_30 = field_34`, a `last` local, the n++
// position, the e/c declaration scope, every for-loop spelling of j++, /Gi.
// Also 69.5 or worse on that form: `do { } while (c != 0)`, `for (;;)` with the
// null test at the bottom, the advance in the loop condition, the per-entry
// body or the whole walk as an inline helper, unsigned/long/short j or n, and
// every branchy spelling of the Push wrap (`if (++writeIdx >= 0x400)`, `> 0x3ff`,
// an empty then-arm, a goto, a `w` local). Diagnostics (wrong code, only to see
// what holds the choice): deleting the `x == c` compare in the rotation loop,
// the `field_20 -= c->field_8` or the `*e = *c` copy each hands EBP back to
// `e`; deleting an `e` use or adding one late in the body does not. So `c`
// wins on the weight of its uses inside the queue block, and the original's
// source must weigh them differently while keeping j++ in the latch.
// The 0x4623e0 and 0x461f90 helpers are byte-identical inlined, so either
// spelling is fine. Also tried on the latch form: the queue's Push/Pop
// defined inline (MSVC then inlines the rotation loop's Pop and Push too, 955
// bytes, while the original calls 0x4623b0/0x462370 there and inlines only the
// last Push) and `new Packet_00461fd0(this)` with an inline constructor (same
// code, 98.1% on the j++-early form).
//
// The allocations call `operator new` / `operator delete` (??2 / ??3, the
// names data/symbols.csv has at 0x4b4f10 / 0x4b4f20); the earlier file's
// `operator new[]` compiled to the same code but references ??_U / ??_V, which
// would fail the reference check once the bytes match.

#include <string.h>

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* ptr);

unsigned int FUN_004b6340();
void __cdecl FUN_00461170(const char* fmt, ...);

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
                    Entry_00461fd0* c = (Entry_00461fd0*)p->field_10;
                    if (c != 0) {
                        p->start = n;
                        j = 0;
                        p->field_10 = (int)(base + n);
                        Entry_00461fd0* q = (Entry_00461fd0*)p->field_10;
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
                                    Entry_00461fd0* x = ((Class_004623b0*)qp)->FUN_004623b0();
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
                            e->field_18 = (int)field_34;
                            e->field_c = (int)p;
                            e->field_1c = 0;
                            if (field_34) {
                                field_34->field_1c = (int)e;
                            }
                            field_34 = e;
                            if (!field_30) {
                                field_30 = e;
                                j++;
                            } else {
                                ++j;
                            }
                            c = (Entry_00461fd0*)c->field_1c;
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
