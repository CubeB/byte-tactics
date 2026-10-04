// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by fledge-alpha-free, finished by Opus. Names are provisional.
// Rewritten (pass 14, Opus): 75.0% -> 85.2%; pass 15: 86.6%; pass 16 (#5358): 88.8%;
// pass 17 (#5515): 89.3%; pass 18 (Opus, #5559): MATCH.
// Queues one received packet's commands in the ring at +0x10. If frames are
// already queued, it only re-stamps each of them with the new tick (pop, push) and
// returns 0. Otherwise it copies the packet into the buffer at +0xc, counts the
// commands after the 4-byte sequence number (a command is 2..0x2c; 0x2c carries its
// own 16-bit length, the others' lengths are in the table at 0x512ad8), skips the
// first n - 0x200 0x2c commands when there are more than 0x200 commands, and pushes
// the rest: spread over up to 30 ticks when a6 is set, all at `tick` otherwise.
//
// What matched it (pass 18):
//  * The a6 loop counts n down with `n--` at the end of each of its two tails
//    (the 0x2c skip path and the push path) and tests `while (n > 0)`, instead of
//    `while (--n > 0)` in the latch. The code is the same (the original's skip path
//    jumps to the shared `dec ebp`), but C2's priorities are not: in the re-sort
//    after progress takes ebx, n's and remaining's pieces both had only ebp left, and
//    remaining's piece won (123 against -23) mostly on the push tail block (w 4,
//    K 12, +96), where n was not referenced. With `n--` in both tails n gains those
//    blocks and keeps ebp through the scan and the a6 loop, as in the original.
//  * The scan locals are declared n, remaining, p. With the tails above, this
//    order gives the original's reload of `this` into edx for the field_c read
//    (p, remaining, n reads it through ebp, 88.4%).
//  * Both tails are written `remaining -= w; q += w; n--;` (the push path's old
//    `q += w; remaining -= w;` order put the remaining temporary in ecx).
//
// What moved it earlier:
//  * The ring as a struct with inline Pop and Push (the pop is the same code 0x462f30
//    inlines) fixed the requeue loop's registers.
//  * `delete field_c; field_c = new char[size + 0x100];` instead of the operator calls
//    shifts the temporary rotation by one, which fixes the delete argument and the
//    a4/a5 loads and stores around the memcpy.
//  * The tick copy in the a6 path is its own local (x), stored to the tick slot, and
//    x, progress, i, q are defined in that order after the spacing computation.
//  * The a6 == 0 loop's exits are `break` (to the one `return 1` after it), which
//    puts its back edge's `xor edx, edx` block before the loop head.
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
            int x = tick;
            unsigned int progress = 0;
            unsigned int i = 0;
            char* q = field_c + 4;
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
                        n--;
                        continue;
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
                remaining -= w;
                q += w;
                n--;
            } while (n > 0);
            return 1;
        }

        char* q = field_c + 4;
        int rem = size;
        while (rem > 0) {
            unsigned char c = *q;
            if (c <= 1 || c >= 0x2d)
                break;
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
                break;
            if (!buffer->Push(tick, q, w))
                break;
            q += w;
        next2:
            ;
        }
    }
    return 1;
}
