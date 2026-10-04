// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by fledge-alpha-free, finished by Opus. Names are provisional.
// Rewritten (pass 14, Opus): 75.0% -> 85.2%; pass 15 (Opus): 86.6%; pass 16 (Opus,
// #5358): 88.8%; pass 17 (Opus, #5515): 89.3%. Codex / GPT-6 retries for #5204,
// #5254, #5298, #5327, #5414, #5437 and #5451 re-confirmed earlier scores only.
// Queues one received packet's commands in the ring at +0x10. If frames are
// already queued, it only re-stamps each of them with the new tick (pop, push) and
// returns 0. Otherwise it copies the packet into the buffer at +0xc, counts the
// commands after the 4-byte sequence number (a command is 2..0x2c; 0x2c carries its
// own 16-bit length, the others' lengths are in the table at 0x512ad8), skips the
// first n - 0x200 0x2c commands when there are more than 0x200 commands, and pushes
// the rest: spread over up to 30 ticks when a6 is set, all at `tick` otherwise.
//
// Pass 17 (#5515):
//  * The a6 == 0 loop's exits are `break` (to the one `return 1` after it) instead of
//    `return 1`: the back edge's `xor edx, edx` block is then placed before the loop
//    head with the entry jumping over it, as in the original (88.8 -> 89.1). It needs
//    all three exits as `break`; any one left as `return 1` keeps the old layout.
//  * The a6 loop's skip path has its own `remaining -= w; q += w;` and `continue`s to
//    the latch, the push path ends `q += w; remaining -= w;`, and the scan locals are
//    declared p, remaining, n (89.1 -> 89.3). This gives the original's two tails,
//    the skip path's `mov edx, ecx; dec edx` and the frame slots (n at +0x24,
//    remaining at +0x28: remaining now has 6 memory references, as in the original).
//
// Still different (25 original lines besides jumps), all n against remaining in the
// scan and the a6 loop:
//  * The original keeps n in ebp from the scan through the a6 loop (except the push
//    block, where ebp holds x and the Push temporaries, and n is reloaded in the push
//    tail) and remaining in edi in the scan and in memory in the a6 loop. Here
//    remaining takes ebp in both and n is in memory (eax at the latch).
//  * Why (tools/c2prio.py): when progress takes ebx (step 45) n and remaining are each
//    left only ebp and are split; remaining's piece comes out at 123 and n's at -23,
//    so remaining wins ebp. With the shared tail (pass 16's file, x13 shape) the tail
//    block holds n's latch (K = 7, w = 4, +112 for n) and n's piece wins (77 against
//    39), which gives the original's registers but one shared tail and swapped slots.
//    With two tails the latch is its own block (K = 2, +32) and remaining gains the
//    skip block (+48). Deleting remaining from the a6 loop altogether still leaves n
//    in eax, so n's piece needs references the source does not have.
//  * Tried in pass 17 without gain: all 144 orders of (n, remaining, p) x (x,
//    progress, i, q) with two tails (best 89.3%); `for (; n > 0; n--)` and
//    `while (n > 0) { ...; n--; }` (the first is identical, C2 drops the guard);
//    `continue` or `do {} while (0)` after the shared tail; the scan and the a6 loop
//    counting down `size` itself with a copy for the a6 == 0 loop (71.9-78.9%); Push
//    on tick directly (81.6-87.0%); a 15-minute permuter run from this file (only
//    moved `n++` above `p += w`, no score change).
//
// What moved it earlier:
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
//  * Pass 16: x is defined first again (x, progress, i, q) and the a6 loop's skip
//    path ends in `goto next1;` with one shared `remaining -= w; q += w;` after the
//    label (86.6 -> 88.8). The single tail drops remaining's a6-loop piece (123 with two
//    tails, c2prio) below n's, so n keeps ebp in the scan and at the a6 latch, the scan
//    gets n in ebp and remaining in edi, and the a6 loop now has the original's
//    registers: tick in esi before it, x in the tick slot (ebp only in the push block),
//    progress ebx, i edi, q esi, n ebp. The a6 == 0 loop keeps left in edi and tick in
//    ebp.
//
// Pass 16's list (for the 88.8% file; pass 17 fixed the a6 == 0 loop and the slots):
//  * The original has two copies of the a6 tail: the skip path does its own
//    `remaining -= w; q += w` and jumps to the latch (`dec ebp`), so n stays in ebp
//    through the loop head, the reader calls and the skip path, and only the push path
//    reloads it. Here the skip path jumps into the shared tail, which reloads n on
//    both paths; that also makes the push path zero-extend w twice (`mov ebp, eax; and
//    ebp, 0xffff` for the size store) and reload x for x++. Two tails in the source
//    (either statement order, `goto` or `continue`) give the original's two blocks but
//    remaining's piece then takes ebp in the a6 loop (84.8-85.1%).
//  * With the tail shared, n and remaining tie on memory references (4 each), so n
//    takes the size slot (+0x28) and remaining the src slot (+0x24); in the original
//    remaining has 6 (the two tails) and the slots are the other way round. The
//    declaration order of n, remaining and p and the scan tail's statement order do
//    not move it.
//  * The a6 == 0 loop: as before, `xor edx, edx` is on the latch path instead of at
//    the loop head. `if (rem > 0) do { ... } while (rem > 0);` scores 89.7% (the same
//    byte count as the original) but copies the loop's first test into the latch, so
//    it is not the original's shape and is not used.
//  * Push taking `unsigned int` or `unsigned short` size: no gain.
//
// Pass 15's list (for the 86.6% file):
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

    char* p = field_c + 4;
    int remaining = size;
    int n = 0;
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
                q += w;
                remaining -= w;
            } while (--n > 0);
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
