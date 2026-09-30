// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// PARTIAL: 67.5% (1015 vs 1040 bytes). Every block is structurally right; the
// remaining diff is one register-allocation state, not missing logic.
//
// What moved it from 66.0 to 67.5: the scan-loop prologue must read
//     size -= 4;  int n = 0;  int remaining = size;  char* p = text + 4;
// in exactly that order. That declaration order is load bearing (reordering any
// pair drops straight back to 66.0, and putting `p` before `remaining` drops to
// 50.4). It is what decides which of the two loop-carried counters gets EDI and
// which gets EBP in the 0x4638f0 scan loop.
//
// Remaining diff, all one shared cause: the original materialises the ring bound
// as a register value (`mov edi, 0x200` at 0x4637dd, loop pre-header) and uses
// immediate 0 everywhere else in that loop. Because EDI then holds 0x200 rather
// than zero, the pop's emptiness test becomes `test ecx, ecx / jle` instead of a
// compare against EDI, the pop's ring pointer lands in EAX instead of ECX, the
// wrap resets become `mov [eax+4], 0` instead of `mov [eax+4], edi`, and the
// false arm of the pop lays out out of line instead of in the fall-through.
// The same zero-vs-0x200 role swap is what puts `n` in EBX/EBP and `remaining`
// in EBP/EDI through the 0x463a30 and 0x463ad0 loops.
//
// MSVC 5 will not hoist a literal 0x200 into a register here under any source
// spelling, so the original must have had a real variable. Scored and ruled out
// (all 67.5%, none produce `mov edi, 0x200`):
//   `int cap = 0x200` at block scope, at function scope, inside the while body,
//   `static const int cap`, and an `unsigned short cap`;
//   the cap expression as `(int)(sizeof(entry) / sizeof(Entry_00463790))`;
//   a function-local `Class_00463730* self = this` (register hoist, no change);
//   moving the ring-pointer local `Buffer_00463730* r = rows` to function scope
//   for blocks 2 and 3 (much worse, 44.1%);
//   the induction variable of block 1's cancellation loop in a register
//   (`for (int j = i; j > 0; j--)`, 66.4%);
//   swapping the `n` and `remaining` declarations (no change, see above);
//   reading the popped entry as `ep->b`/`ep->c` instead of `Entry e = *ep`
//   (65.3%);
//   block 2's tail push as `r->tail++` instead of the `int t` form (no change);
//   `a4`/`a5` through temporaries, `this->f14` spellings, an extra live
//   `x - f0` copy in the span clamp (no change);
//   `tools/headers.py`: 128 header sets, best 67.5, so the header choice is not
//   load bearing here.
// Re-tested by space-bunny-free, all still 67.5 or worse, so the allocation state
// is not reachable from the scan loop's declarations alone:
//   `int remaining = size;` before `int n = 0` (byte identical, so the tie is not
//   broken by declaration order), `int remaining = size - 4;` with no `size -= 4`
//   statement, all six orderings of the n/remaining/p declarations (the two with
//   p first drop to 66.0), `unsigned int remaining` (61.6), `unsigned int n` (67.1),
//   `n = n + 1` and `n += 1` for the scan loop's `n++`, splitting the `cc = c`
//   copy into two statements, `if ((remaining -= w) < 0) break;` (folds to the
//   same code, so `remaining` has no spare graph node to drop a priority step),
//   and `f18 = a5;` before `f14 = a4;` (66.7).
// Everything after the scan loop follows from the same swap: with n in EBP the
// block-2 push clobbers it, the fall-through tail needs the `mov ebp,[esp+0x24]`
// reload the skip path does not, the two tails stop being identical and stop
// tail merging, and that unmerged 16-byte copy is most of the 25 missing bytes.
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

// Character widths, one word per entry with a 4-byte stride.
extern unsigned short DAT_00512ad8[][2];

struct Entry_00463790 {
    int a;                             // +0x00
    int b;                             // +0x04
    int c;                             // +0x08
};

// 0x180c-byte ring of 0x200 12-byte entries.
struct Buffer_00463730 {
    int n;                             // +0x00
    int head;                          // +0x04
    int tail;                          // +0x08
    Entry_00463790 entry[0x200];       // +0x0c
};


static __inline Entry_00463790* Pop_00463790(Buffer_00463730* r)
{
    if (r->n <= 0)
        return 0;
    r->n--;
    int h = r->head + 1;
    r->head = h;
    if (h >= 0x200)
        r->head = 0;
    return (Entry_00463790*)((char*)r + h * 12);
}

class Class_00463730 {
public:
    int f0;                            // +0x00
    int f4;                            // +0x04
    int f8;                            // +0x08
    char* text;                        // +0x0c
    Buffer_00463730* rows;             // +0x10
    int f14;                           // +0x14
    int f18;                           // +0x18

    int FUN_00463790(char* src, unsigned int size, int x, int a4, int a5, int a6);
};

// FUNCTION: 0x463790
int Class_00463730::FUN_00463790(char* src, unsigned int size, int x, int a4, int a5, int a6)
{
    if (size <= 0) {
        return 1;
    }
    if ((rows ? rows->n : 0) != 0) {
        f8++;
        int i = rows->n;
        while (i--) {
            Buffer_00463730* r = rows;
            Entry_00463790* ep = Pop_00463790(r);
            Entry_00463790 e = *ep;
            r = rows;
            if (r->n < 0x200) {
                int t = r->tail + 1;
                r->tail = t;
                if (t >= 0x200) {
                    r->tail = 0;
                }
                r->entry[r->tail].b = e.b;
                r->entry[r->tail].a = x;
                r->entry[r->tail].c = e.c;
                r->n++;
            }
        }
        return 0;
    }

    f8 = 0;
    if (size > f4) {
        operator delete(text);
        char* nb = (char*)operator new(size + 0x100);
        text = nb;
        if (nb == 0) {
            f4 = 0;
            return 1;
        }
        f4 = size + 0x100;
    }
    memcpy(text, src, size);
    f14 = a4;
    f18 = a5;
    size -= 4;
    int n = 0;
    int remaining = size;
    char* p = text + 4;
    while (remaining > 0) {
        unsigned char c = *p;
        unsigned char cc = c;
        if (cc <= 1) {
            break;
        }
        if (cc >= 0x2d) {
            break;
        }
        unsigned short w;
        if (cc == 0x2c) {
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
        if (remaining < 0) {
            break;
        }
        p += w;
        n++;
    }

    if (n > 0) {
        int lineLeft = n - 0x200;
        if (a6 != 0) {
            int span = x - f0;
            if (span > 0x1e) {
                span = 0x1e;
            } else if (span <= 0) {
                span = 1;
            }
            int spacing = 0x10;
            if (n > span) {
                spacing = (n << 4) / span;
            }
            int progress = 0;
            int i = 0;
            char* q = text + 4;
            do {
                unsigned char c = *q;
                unsigned char cc = c;
                unsigned short w;
                if (cc == 0x2c) {
                    Class_00415dc0 reader;
                    reader.data = (unsigned int*)q;
                    reader.index = 0;
                    reader.bit = 0;
                    reader.FUN_00415dc0(8);
                    w = (unsigned short)reader.FUN_00415dc0(0x10);
                    if (lineLeft > 0) {
                        lineLeft--;
                        remaining -= w;
                        q += w;
                        goto next2;
                    }
                } else {
                    w = DAT_00512ad8[c][0];
                }
                {
                    Buffer_00463730* r = rows;
                    if (r->n >= 0x200) {
                        return 1;
                    }
                    int t = r->tail + 1;
                    r->tail = t;
                    if (t >= 0x200) {
                        r->tail = 0;
                    }
                    r->entry[r->tail].b = (int)q;
                    r->entry[r->tail].a = x;
                    r->entry[r->tail].c = w;
                    r->n++;
                }
                i++;
                {
                    int scaled = i << 4;
                    if ((unsigned int)scaled >= (unsigned int)progress) {
                        progress += spacing;
                        x++;
                    }
                }
                remaining -= w;
                q += w;
            next2:
                ;
            } while (--n > 0);
            return 1;
        }

        char* q = text + 4;
        while (remaining > 0) {
            unsigned char c = *q;
            unsigned char cc = c;
            if (cc <= 1) {
                return 1;
            }
            if (cc >= 0x2d) {
                return 1;
            }
            unsigned short w;
            if (cc == 0x2c) {
                Class_00415dc0 reader;
                reader.data = (unsigned int*)q;
                reader.index = 0;
                reader.bit = 0;
                reader.FUN_00415dc0(8);
                w = (unsigned short)reader.FUN_00415dc0(0x10);
                if (lineLeft > 0) {
                    lineLeft--;
                    remaining -= w;
                    q += w;
                    goto next3;
                }
            } else {
                w = DAT_00512ad8[c][0];
            }
            remaining -= w;
            if (remaining < 0) {
                return 1;
            }
            {
                Buffer_00463730* r = rows;
                if (r->n >= 0x200) {
                    return 1;
                }
                int t = r->tail + 1;
                r->tail = t;
                if (t >= 0x200) {
                    r->tail = 0;
                }
                r->entry[r->tail].b = (int)q;
                r->entry[r->tail].a = x;
                r->entry[r->tail].c = w;
                r->n++;
            }
            q += w;
        next3:
            ;
        }
    }
    return 1;
}
