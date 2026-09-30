// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 73.6% (1022 vs 1040 bytes).
//
// The `mov edi, 0x200` loop-preheader hoist in block 1 is NOT from the
// literal-argument inline-helper trick: it appears whenever block 1 has three
// uses of the literal 0x200 (head wrap, r->n < 0x200, tail wrap). Writing the
// two wraps as plain `if (h >= 0x200) r->head = 0;` on the same local keeps the
// hoist and makes block 1 byte-identical to the original except for one
// register pair: ours has r in ECX and the head/index in EAX, the original has
// r in EAX and the head/index in ECX. That plain-wrap form scores 72.5% here
// only because the line diff aligns differently; it is structurally closer to
// the original than the 73.6% Wrap_00463790 form kept in this file.
//
// Also with the plain-wrap form the Pop polarity is correct:
// `test ecx,ecx / jle <null>` with the null path out of line at the end
// (`...; mov eax,edx; jmp; xor eax,eax`).
//
// Remaining differences are one register-allocator rotation:
//  * block 1 (0x4637e2) r/index swap described above.
//  * the 0x4638db scan-loop init: ours n=EDI/remaining=EBP, the original
//    n=EBP/remaining=EDI. Our `size -= 4` becomes `lea ebp,[ebx-4]` while the
//    original does `sub ebx,4` and `mov edi,ebx`. Writing `remaining = size - 4`,
//    `size = size - 4`, or swapping the declaration order does not change it.
//  * these cascade into the 0x463a30 and 0x463ad0 loops.
//
// Ruled out (all worse than 67.5%): an int-returning Wrap that also does the
// increment (loses the EDI hoist), fully inlining the pop without a helper
// (entry copy moves to EDX), the old 0x4638f0 helper idioms, and `int cap`
// locals (folded to immediates).
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


// Bounded wrap of a ring index, see the note at the top of the file.
__inline void Wrap_00463790(int& i, int cap)
{
    if (i >= cap)
        i = 0;
}

static __inline Entry_00463790* Pop_00463790(Buffer_00463730* r)
{
    if (r->n > 0) {
        r->n--;
        int h = r->head + 1;
        r->head = h;
        Entry_00463790* ep = (Entry_00463790*)((char*)r + h * 12);
        Wrap_00463790(r->head, 0x200);
        return ep;
    }
    return 0;
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
                Wrap_00463790(r->tail, 0x200);
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
