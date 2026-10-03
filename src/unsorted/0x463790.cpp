// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by fledge-alpha-free, finished by Opus. Names are provisional.
// Rewritten (pass 14, Opus): 75.0% -> 85.2%; pass 15 (Opus): 86.6%. Queues one
// received packet's commands in the ring at +0x10. If frames are already queued, it
// only re-stamps each of them with the new tick (pop, push) and returns 0. Otherwise it
// copies the packet into the buffer at +0xc, counts the commands after the 4-byte
// sequence number (a command is 2..0x2c; 0x2c carries its own 16-bit length, the
// others' lengths are in the table at 0x512ad8), skips the first n - 0x200 0x2c
// commands when there are more than 0x200 commands, and pushes the rest: spread over
// up to 30 ticks when a6 is set, all at `tick` otherwise.
//
// What moved it:
//  * The ring as a struct with inline Pop and Push (the pop is the same code 0x462f30
//    inlines, `Frame* f = &frames[head]; if (++head >= 0x200) head = 0;`) fixed the
//    requeue loop's registers (78%).
//  * `delete field_c; field_c = new char[size + 0x100];` instead of the operator calls
//    shifts the temporary rotation by one, which fixes the delete argument and the
//    a4/a5 loads and stores around the memcpy (+2 points).
//  * In the a6 loop's push path, `q += w;` before `remaining -= w;`, so it is not
//    tail-merged with the 0x2c skip path (+1.7).
//  * Declaring n, remaining, p in that order (the other orders are 80-85%).
//  * The tick copy in the a6 path is its own local (x), stored to the tick slot.
//  * Pass 15: in the a6 block, i, progress, q and then x defined after the spacing
//    computation (85.2 -> 86.6). This is what gives the scan its registers (n in ebp,
//    remaining in edi) and the a6 == 0 loop its registers (left in edi, tick in ebp,
//    0x200 immediate). Why, from tools/c2prio.py: with this order tick's a6 web
//    (span, x = tick) overlaps q and takes ebx, so progress takes ebp, and n,
//    remaining and left (by then allowed only ebp) each run out of registers on their
//    own and are split one by one (FUN_00439385), which gives the original's scan
//    and a6 == 0 registers.
//    With x first (pass 14's order) tick takes esi, progress ebx, and C2 splits n,
//    remaining, left, this and the constants all at once (FUN_0041ba2b ->
//    FUN_00437e67); those pieces come out remaining 123, left 44, n -23, so remaining
//    took ebp for the scan. The order of n, remaining and p acts through the candidate
//    ids instead: with p, remaining, n the a6 == 0 loop comes out right but n loses a
//    -10 against -10 tie for edi in the scan (85.0%).
//
// Still different (37 lines):
//  * The a6 loop: the original has progress in ebx, x in its slot (+0x2c, loaded into
//    ebp only for the push block) and n in ebp outside the push block (stored at the
//    latch, reloaded after the push); here tick's piece and x take ebx, progress ebp,
//    and n stays in memory there. So the original has tick in esi before the loop
//    (`mov esi, [esp + 0x2c]; mov ecx, esi`, x stored just before `xor ebx, ebx`),
//    progress ebx, and x ebp before the split. spacing then lands in +0x34 and c in
//    +0x38 (here +0x2c and +0x34, since x needs no slot).
//  * The a6 == 0 loop re-zeroes edx at the loop head (entered with `jmp` past it);
//    here the `xor edx, edx` sits on the latch path.
//  * Tried without effect on this: every order of i, progress, q, x (with or without
//    span/spacing moved), the 6 orders of n, remaining, p crossed with those, the
//    order of the skip path's three statements and of the push path's tail, x as
//    unsigned, ++x, x += 1, Push on tick directly (78.6%), a ReadLength inline helper
//    for the three 0x2c readers, w as int, unsigned or split per branch, while/for/do
//    forms of the a6 loop, sharing one variable between the requeue count and n or
//    remaining, /Gi. Pass 14 also tried extra reads of n, ++n / n += 1,
//    `(remaining -= w) < 0`, int/unsigned i and progress, the a6 path as an inline
//    method, headers and a separate counter `int k = n;` (78.6%).
#include <string.h>

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

// Bit reader, see src/unsorted/0x415dc0.cpp.
class Class_00415dc0 {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int FUN_00415dc0(int bits);
};

// Length of each packet command, one word per 4-byte entry.
extern unsigned short DAT_00512ad8[][2];

// One queued frame of a player's ring: the tick it is due, the data and size.
struct Frame_00463790 {
    int tick;                          // +0x0
    void* data;                        // +0x4
    int size;                          // +0x8
};

// 0x180c-byte ring of 0x200 frames.
struct Ring_00463790 {
    int count;                         // +0x0
    int head;                          // +0x4
    int tail;                          // +0x8
    Frame_00463790 frames[0x200];      // +0xc

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

class Class_00463730 {
public:
    int field_0;                       // +0x00
    unsigned int field_4;              // +0x04
    int field_8;                       // +0x08
    char* field_c;                     // +0x0c
    Ring_00463790* buffer;             // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18

    int Count() { return buffer ? buffer->count : 0; }

    int FUN_00463790(char* src, unsigned int size, int tick, int a4, int a5, int a6);
};

// FUNCTION: 0x463790
int Class_00463730::FUN_00463790(char* src, unsigned int size, int tick, int a4, int a5, int a6)
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

    int n = 0;
    int remaining = size;
    char* p = field_c + 4;
    while (remaining > 0) {
        unsigned char c = *p;
        if (c <= 1 || c >= 0x2d)
            break;
        unsigned short w;
        if (c == 0x2c) {
            Class_00415dc0 reader;
            reader.data = (unsigned int*)p;
            reader.index = 0;
            reader.bit = 0;
            reader.FUN_00415dc0(8);
            w = (unsigned short)reader.FUN_00415dc0(0x10);
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
            unsigned int i = 0;
            unsigned int progress = 0;
            char* q = field_c + 4;
            int x = tick;
            do {
                unsigned char c = *q;
                unsigned short w;
                if (c == 0x2c) {
                    Class_00415dc0 reader;
                    reader.data = (unsigned int*)q;
                    reader.index = 0;
                    reader.bit = 0;
                    reader.FUN_00415dc0(8);
                    w = (unsigned short)reader.FUN_00415dc0(0x10);
                    if (left > 0) {
                        left--;
                        remaining -= w;
                        q += w;
                        goto next1;
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
                q += w;
                remaining -= w;
            next1:
                ;
            } while (--n > 0);
            return 1;
        }

        char* q = field_c + 4;
        int rem = size;
        while (rem > 0) {
            unsigned char c = *q;
            if (c <= 1 || c >= 0x2d)
                return 1;
            unsigned short w;
            if (c == 0x2c) {
                Class_00415dc0 reader;
                reader.data = (unsigned int*)q;
                reader.index = 0;
                reader.bit = 0;
                reader.FUN_00415dc0(8);
                w = (unsigned short)reader.FUN_00415dc0(0x10);
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
                return 1;
            if (!buffer->Push(tick, q, w))
                return 1;
            q += w;
        next2:
            ;
        }
    }
    return 1;
}
