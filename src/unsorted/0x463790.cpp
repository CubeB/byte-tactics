// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// PARTIAL: 73.3% (1022 vs 1040 bytes). 67.5% -> 73.3% on this pass.
//
// WHAT MOVED IT: the original materialises the ring bound 0x200 in a REGISTER in
// block 1's loop pre-header (`mov edi, 0x200`, 0x4637dd) and then compares
// head+1, n and tail+1 against that register, while the same 0x200 comparisons in
// blocks 2 and 3 stay immediates. MSVC 5 folds a *local* constant into `cmp`
// immediates, so no local, static const, sizeof-based or unsigned spelling of the
// bound can produce that (verified by compiling a standalone loop with
// tools/wcl: `int cap = 512` used three times in a loop still gives
// `cmp $0x200,%ecx`).
//
// The one construct that DOES hold a literal in a register: a literal passed as
// an ARGUMENT to an inlined function. The argument is a temp, and a temp with
// several uses gets a register; the peephole never folds a temp back into a cmp.
// Confirmed on the micro-test (`mov $0x200,%esi` then three `cmp %esi,...`), and
// it reproduces the original here: both ring wraps go through
//     __inline void Wrap_00463790(int& i, int cap) { if (i >= cap) i = 0; }
// called with the LITERAL 0x200, and the two call sites must pass the FIELD
// (r->head, r->tail), not a local copy. Passing a local makes the local
// address-taken and the register disappears again.
// With the bound in a register: f8++ gets EDI as the original has it, the pop
// tests with `test ecx,ecx / jle` against no zero register, the wraps store the
// immediate 0 (`mov [eax+4],0`, 7 bytes) instead of a 3-byte register store, and
// the three `cmp ...,0x200` become 2-byte register compares. That is the whole
// 18-byte size gap and most of the register diff.
//
// Still differs, all downstream of the same allocation state:
//  * the pop's branch polarity. The original falls THROUGH into the body and puts
//    the null path out of line (`test ecx,ecx / jle 0x46380a`, with `xor eax,eax`
//    after the body). The early-return helper here jumps over the null path
//    (`jg` / `xor eax,eax` / `jmp`). The positive `if (r->n > 0) { ... } return ep;`
//    form in the helper was tried and is worse (70.0%): it adds an `xor ecx,ecx`
//    and reorders the head store, so the polarity is not free.
//  * `size -= 4` is folded into `lea ebp,[ebx-4]`, where the original keeps
//    `sub ebx,4` in the pre-header and copies with `mov edi,ebx`.
//  * the 0x4638f0 scan loop still has n and remaining the wrong way round
//    (ours n=EDI/remaining=EBP, the original n=EBP/remaining=EDI), which cascades
//    into the scratch-register picks in the 0x463a30 and 0x463ad0 loops.
//
// Ruled out on this pass, all 67.5% or worse, so nobody repeats them:
//  * `int cap = 0x200` as a local in block 1, and as a parameter of Pop called
//    with that local (a local's value is folded, a literal argument's is not);
//  * the pop written INLINE in the `if (r->n > 0) { ... }` positive form, the
//    idiom src/unsorted/0x462f30.cpp shows for the same ring (65.0%): it keeps
//    the null path in the fall-through but loses the register-resident bound;
//  * `Wrap_00463790(h, 0x200)` on a local `h` plus re-storing the field
//    afterwards (67.1%).
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
    if (r->n <= 0)
        return 0;
    r->n--;
    int h = r->head + 1;
    r->head = h;
    Entry_00463790* ep = (Entry_00463790*)((char*)r + h * 12);
    Wrap_00463790(r->head, 0x200);
    return ep;
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
