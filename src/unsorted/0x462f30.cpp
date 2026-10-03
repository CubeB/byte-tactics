// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by fledge-alpha-free, finished by Opus. Names are provisional.
// Codex GPT-6 retry for #5204 (2026-10-03): `/Gi` leaves this function
// at 77.6% with the same 1707-byte output; the existing source remains best.
// GPT-6 retry (#5254): rechecked at 77.6%; entry lifetime and copy-tail shape remain.
// #5298 Codex retry: re-confirmed 77.6%; the entry live-range split and
// cross-jumped copy tail remain the only meaningful source-level gaps.
// #5327 retry: re-confirmed 77.6%; earlier allocator, branch-layout and
// receive-loop sweeps cover the remaining source-level choices.
// Rebuilt from the disassembly (pass 13, Opus): 43.3% -> 75.7%; pass 15 (Opus): 77.6%.
// The class name is
// data/symbols.csv's Class_00462f30 (the caller 0x4534e0 uses it); Find (0x462d90) is
// called through Class_00462d30, its own file's class, and returns Entry_00462d90*.
//
// What the structure is now, and why:
//  * The tail of each entry (+0x18) is Class_00463730, the class of 0x463730/0x463790,
//    and its 0x180c-byte ring is read through small inline methods (Peek, Pop, Take,
//    GetFrame). Pop is the same code 0x463790 inlines.
//  * The first loop walks `Entry* e = &entries[i]`: with plain `entries[i].` indexing
//    the found path recomputed the address from i instead of using the loop pointer.
//  * After the 4b5 test the original keeps both arms (A, then its shared E_FAIL return,
//    then B), and the HAPINET error block comes after B. Only an else-branch keeps it
//    there: a `goto error` to a label anywhere else (end of the block, end of the
//    function, after B's returns) is moved up to just after the receive loop. With
//    `if (1) { ... } else { error: ... }` the layout is exact (77.3% with the rest of
//    this file); `if (rc == 0) ... else { error: }` is the plausible spelling, and the
//    rc test is jump-threaded away only when the receive loop is written
//    `if (rc != 0) do { ... } while (rc != 0);` (a plain `while` leaves the zero
//    constant in edi through the second half, 69.4%). The do-while duplicates the
//    NOMSG test into the latch (`je got; cmp esi, NOMSG; jne; jmp none`), where the
//    original's latch is `test esi, esi; jne B1; jmp got`.
//  * The out-of-order sequence code computes cur = *(int*)buffer in both arms of the
//    first clamp (the original loads it once per arm, ecx then edx), and the clamp
//    helpers are `int r = n -+ 1; if (r >= -1) r = -2; return r;`. With `n--` on the
//    parameter (the old spelling) region A kept the entry in edi; with the local r it
//    is spilled exactly as in the original (+8 points). A ternary helper gets the
//    allocation too but changes the second clamp's shape.
//  * After a failed 0x463790 call the original peeks the ring in both arms
//    (`if (call) { length = 0; peek } else peek`); written once, MSVC merges them.
//
//  * Pass 15: `delete buffer; ... buffer = new char[capacity];` in the receive loop and
//    `delete entry->field_14; entry->field_14 = new char[length];` in the out-of-order
//    save, instead of operator delete/new (75.7 -> 76.6 -> 77.6). As in 0x463790 the
//    operators shift the temporary rotation by one: the grow block's length/capacity
//    moves and the second receive call's `g_game + 0x14` now use the original's eax,
//    ecx, edx.
//
// Still different:
//  * entry should live in edi from `entry = 0` after the first loop through the route
//    (`test edi, edi`, `mov edi, eax` after Find, `lea esi, [edi + 0x18]`). Here the
//    net parameter takes edi after the loop and tick takes it in the route, so tick is
//    loaded once instead of twice and lands in slot 0x1c instead of 0x14. A separate
//    block-scoped `e` for region A gives the right first-loop registers but loses edi
//    to net as well; making the route test entry twice (`if (!entry) entry = Find();
//    if (!entry) goto none;`) gives entry edi but moves tick into edi in the first loop.
//  * The first loop's found path is a full copy of the memcpy/return; the original
//    jumps into the end path's copy at `mov edi, [esp + 0x28]` (cross-jumped), and the
//    `none` block carries the scheduled epilogue. `goto copy_out` shares the code but
//    puts the src/len moves after the label (66.7%; 70.2% on pass 15's file, 73.3% with
//    a shared `found:` block taking a tail pointer).
//
// Pass 15 notes (tools/c2prio.py, scratch dumps of C2's colouring steps):
//  * entry is one web from `entry = 0` to the route and is live across region A's
//    memcpy, so it is only allowed ebx/ebp and gets split. Its big piece (#5, priority
//    26) loses edi in region A to the e->field_c temporary (priority 28, the
//    original's `mov edi, [ecx + 0xc]`, so that part is right). The pieces it is then
//    split into leave the long stretch from `entry = 0` to the route with no
//    references (spill cost 0), so C2 skips it and net's piece takes edi. The original
//    must have split it into a piece with references at both ends.
//  * `if (call) length = 0; src = entry->tail.GetFrame(tick, len);` (the peek written
//    once) raises that piece to 51, above the temporary, and entry gets edi
//    everywhere including region A (68.2%). The original's two identical peek copies
//    jumping to one join look like C2 duplicating a small join block, so the source
//    may be the single GetFrame; region A would then need something else.
//  * Loop form: `if (rc != 0) { while (1) { ...; rc = receive(); if (rc == 0) break; } }`
//    gives the original's latch (`test esi, esi; jne B1; jmp B`) and keeps the error
//    block after B, but the constant 0 then takes edi through the first half (73.0%).
//    Replacing either of the loop's two zero compares by a global brings it back
//    (77.4%), and so does reading the new buffer back through a pointer
//    (`char** pp = &buffer; if (*pp == 0)`, 79.3%, not plausible, so not used). A
//    20-minute permuter run reached 79.0% the same way. `while (1)` with the break at
//    the top cross-jumps the second receive call into the first (77.4%). Measured
//    before the delete/new change: `for (;;)` with the break at the top is turned into
//    a rotated while (70.3%), and a switch on rc sorts the cases by value (TOOSMALL
//    first, 75.5%).
//
// Receives the next frame for the local player: first any frame queued in a
// player's ring whose tick is due, otherwise a saved out-of-order frame or a new
// packet from HAPINET_receivepacket (growing the buffer on DPERR_BUFFERTOOSMALL).
// Sequenced packets (first dword not -1) are checked against the sender's last
// sequence number: an out-of-order frame is saved in the entry (and
// DPERR_NOMESSAGES returned), a gap is reported through the empty 0x4568b0, and a
// saved frame is swapped in when it can be used. The frame is then queued in the
// player's ring by 0x463790 and the next due frame is returned.
#include <string.h>

// One queued frame of a player's ring: the tick it is due, the data and size.
struct Frame_00462f30 {
    int tick;                          // +0x0
    void* data;                        // +0x4
    int size;                          // +0x8
};

// 0x180c-byte ring of 0x200 frames.
struct Ring_00462f30 {
    int count;                         // +0x0
    int head;                          // +0x4
    int tail;                          // +0x8
    Frame_00462f30 frames[0x200];      // +0xc

    Frame_00462f30* Pop()
    {
        if (count > 0) {
            count--;
            Frame_00462f30* f = &frames[head];
            if (++head >= 0x200)
                head = 0;
            return f;
        }
        return 0;
    }
};

// The tail of a PlayerFrameInfo entry (see 0x463730 and 0x463790).
class Class_00463730 {
public:
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    char* field_c;                     // +0x0c
    Ring_00462f30* buffer;             // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18

    int FUN_00463790(char* src, unsigned int size, int tick, int a4, int a5, int a6);

    Frame_00462f30* Peek()
    {
        if (buffer == 0 || buffer->count <= 0)
            return 0;
        return &buffer->frames[buffer->head];
    }

    // Pops the frame at the head if it is due (or if there is no tick).
    void* Take(Frame_00462f30* f, int tick, int& size)
    {
        if (f != 0) {
            int d = f->tick - tick;
            if (tick == 0 || d <= 0 || d > 0x1e) {
                f = buffer->Pop();
                size = f->size;
                return f->data;
            }
        }
        return 0;
    }

    void* GetFrame(int tick, int& size)
    {
        size = 0;
        return Take(Peek(), tick, size);
    }
};

struct Entry_00462d90 {
    int field_0;                       // +0x00 the id
    int field_4;                       // +0x04
    int field_8;                       // +0x08 last sequence number, -1 for none
    int field_c;                       // +0x0c saved frame length
    int field_10;                      // +0x10 saved frame capacity
    char* field_14;                    // +0x14 saved frame
    Class_00463730 tail;               // +0x18
};

#pragma pack(push, 1)
struct Game_00462f30 {
    char unknown_0[0x38a47];
    int tick;                          // +0x38a47
};
#pragma pack(pop)

extern Game_00462f30* g_game;

class Class_0044f9c0 {
public:
    int FUN_0044f9c0(void* net, char* data, int* size);
};
extern Class_0044f9c0 DAT_005129f8;

void __stdcall FUN_004568b0(int a, int b, int c);
void __cdecl FUN_00461170(const char* fmt, ...);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
char* __stdcall FUN_004c9530(int error);

// The sequence numbers next to n, held below -1 (-1 or more becomes -2).
static int Prev_00462f30(int n)
{
    int r = n - 1;
    if (r >= -1)
        r = -2;
    return r;
}

static int Next_00462f30(int n)
{
    int r = n + 1;
    if (r >= -1)
        r = -2;
    return r;
}

class Class_00462d30 {
public:
    Entry_00462d90* FUN_00462d90(long id);
};

class Class_00462f30 {
public:
    void* vtable;                      // +0x00
    int field_4;                       // +0x04
    void* owner;                       // +0x08
    int field_c;                       // +0x0c current frame's sender
    int field_10;                      // +0x10
    Entry_00462d90* field_14;          // +0x14 entry whose saved frame is in use
    char* buffer;                      // +0x18
    char* spare;                       // +0x1c
    Entry_00462d90 entries[10];        // +0x20
    int capacity;                      // +0x228
    int length;                        // +0x22c
    int field_230;                     // +0x230 spare buffer's length
    int field_234;                     // +0x234
    int field_238;                     // +0x238

    int FUN_00462f30(void* net, unsigned char* data, int* size);
};

// FUNCTION: 0x462f30
int Class_00462f30::FUN_00462f30(void* net, unsigned char* data, int* size)
{
    int tick = g_game->tick;
    Entry_00462d90* entry;
    unsigned int i;
    void* src;
    int len;
    int rc;

    for (i = 0; i < 10; i++) {
        Entry_00462d90* e = &entries[i];
        if (e->field_0 == -1)
            break;
        src = e->tail.GetFrame(tick, len);
        if (src != 0) {
            *(int*)((char*)net + 0x4b5) = e->tail.field_14;
            *(int*)((char*)net + 0x4b9) = e->tail.field_18;
            memcpy(data, src, len);
            *size = len;
            return 0;
        }
    }

    entry = 0;
    if (length == 0) {
        int n = 0;
        if (field_14 != 0) {
            if (spare != 0) {
                buffer = spare;
                if (field_230 > 0) {
                    length = field_230;
                    field_c = field_234;
                    field_10 = field_238;
                    n = length - 4;
                    if (n > 0)
                        field_14->field_8 = *(int*)buffer;
                } else {
                    length = 0;
                }
                spare = 0;
                field_14->field_c = 0;
                field_14 = 0;
            } else if (field_14->field_c > 0) {
                spare = buffer;
                field_234 = field_c;
                field_230 = 0;
                field_238 = field_10;
                buffer = field_14->field_14;
                field_14->field_8 = *(int*)buffer;
                length = field_14->field_c;
                field_c = field_14->field_0;
                field_10 = field_14->field_4;
                n = length - 4;
                field_14->field_c = 0;
            } else {
                field_14 = 0;
            }
        }
        if (n == 0) {
            length = capacity;
            rc = DAT_005129f8.FUN_0044f9c0((char*)g_game + 0x14, buffer, &length);
            if (rc != 0) {
                do {
                    if (rc == (int)0x887700be)      // DPERR_NOMESSAGES
                        goto none;
                    if (rc != (int)0x8877001e)      // DPERR_BUFFERTOOSMALL
                        goto error;
                    delete buffer;
                    capacity = length;
                    length = 0;
                    buffer = new char[capacity];
                    if (buffer == 0) {
                        capacity = 0;
                        return (int)0x8007000e;
                    }
                    length = capacity;
                    rc = DAT_005129f8.FUN_0044f9c0((char*)g_game + 0x14, buffer, &length);
                } while (rc != 0);
            }
            if (rc == 0) {
                field_c = *(int*)((char*)net + 0x4b5);
                field_10 = *(int*)((char*)net + 0x4b9);
                if (*(int*)((char*)net + 0x4b5) != 0) {
                    if ((unsigned int)length >= sizeof(int)) {
                        if (length == sizeof(int))
                            return (int)0x80004005;
                        if (*(int*)buffer != -1) {
                            entry = ((Class_00462d30*)this)->FUN_00462d90(field_c);
                            if (entry != 0) {
                                if (entry->field_8 != -1) {
                                    int prev = entry->field_8 - 1;
                                    int cur;
                                    if (prev >= -1) {
                                        prev = -2;
                                        cur = *(int*)buffer;
                                    } else {
                                        cur = *(int*)buffer;
                                    }
                                    int d = prev - cur;
                                    int flag = entry->field_c > 0;
                                    if (d > 0) {
                                        if (entry->field_c <= 0) {
                                            // Out of order: save it in the entry.
                                            if (length > entry->field_10) {
                                                delete entry->field_14;
                                                entry->field_14 = new char[length];
                                                if (entry->field_14 == 0) {
                                                    FUN_00461170("no memory for allocating saved receive frame\n");
                                                    entry->field_10 = -1;
                                                    return (int)0x8007000e;
                                                }
                                                entry->field_10 = length;
                                            }
                                            memcpy(entry->field_14, buffer, length);
                                            entry->field_4 = field_10;
                                            entry->field_0 = field_c;
                                            entry->field_c = length;
                                            goto none;
                                        }
                                        prev = Prev_00462f30(entry->field_8);
                                        if (*(int*)entry->field_14 <= cur) {
                                            if (prev != cur)
                                                FUN_004568b0(field_c, prev, Next_00462f30(cur));
                                            int p2 = Prev_00462f30(*(int*)buffer);
                                            if (p2 != *(int*)entry->field_14)
                                                FUN_004568b0(field_c, p2, Next_00462f30(*(int*)entry->field_14));
                                            flag = 0;
                                            field_14 = entry;
                                        } else {
                                            if (prev != *(int*)entry->field_14)
                                                FUN_004568b0(field_c, prev, Next_00462f30(*(int*)entry->field_14));
                                            int p2 = Prev_00462f30(*(int*)entry->field_14);
                                            if (p2 != *(int*)buffer)
                                                FUN_004568b0(field_c, p2, Next_00462f30(*(int*)buffer));
                                        }
                                    }
                                    if (flag && entry->field_c > 0) {
                                        // Swap the saved frame in, keep this one as the spare.
                                        spare = buffer;
                                        field_230 = length;
                                        field_234 = field_c;
                                        field_238 = field_10;
                                        buffer = entry->field_14;
                                        length = entry->field_c;
                                        field_c = entry->field_0;
                                        field_10 = entry->field_4;
                                        field_14 = entry;
                                        entry->field_c = 0;
                                    }
                                }
                                entry->field_8 = *(int*)buffer;
                            } else {
                                return (int)0x80004005;
                            }
                        }
                    } else {
                        return (int)0x80004005;
                    }
                } else {
                    // From sender 0 (a system message): hand it out directly.
                    if (*size >= length) {
                        memcpy(data, buffer, length);
                        *size = length;
                        length = 0;
                        return 0;
                    } else {
                        *size = length;
                        return (int)0x8877001e;
                    }
                }
            } else {
            error:
                FUN_00461170("HAPINET_receivepacket failed (%s)\n", FUN_004c9530(rc));
                length = 0;
                return rc;
            }
        }
    }

    if (entry == 0) {
        entry = ((Class_00462d30*)this)->FUN_00462d90(field_c);
        if (entry == 0)
            goto none;
    }
    {
        Frame_00462f30* f;
        if (entry->tail.FUN_00463790(buffer, length, tick, field_c, field_10, field_14 == 0)) {
            length = 0;
            len = 0;
            f = entry->tail.Peek();
        } else {
            len = 0;
            f = entry->tail.Peek();
        }
        src = entry->tail.Take(f, tick, len);
    }
    if (src != 0) {
        *(int*)((char*)net + 0x4b5) = entry->tail.field_14;
        *(int*)((char*)net + 0x4b9) = entry->tail.field_18;
        memcpy(data, src, len);
        *size = len;
        return 0;
    }

none:
    length = 0;
    return (int)0x887700be;
}
