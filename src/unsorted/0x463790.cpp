// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 66.0% (1006 vs 1040 bytes). Structure and all four blocks are
// correct; the remaining diff is register allocation, not missing logic.
//
// Two changes moved it from 62.6 to 66.0: the first (cancellation) loop must be
// `int i = rows->n; while (i--) { ... }`, whose MSVC 5 codegen is the
// `mov ecx,eax / dec eax / test ecx,ecx / je` (the guide's documented
// `while (n--)` shape) where a plain `for (i=n; i!=0; i--)` gave `cmp eax,edi`.
// Then the ring pop MUST be a `static __inline` helper with an early
// `return 0;`: inlined, it reproduces the original's branchy
// `test ecx,ecx / jle / ... / jmp / xor eax,eax` pop. Written inline as an
// `ep = 0` initialiser followed by a conditional assign it is if-converted to
// `xor edx,edx` and the whole allocation shifts.
//
// Remaining diff, all one shared cause: the original materialises the ring
// bound `mov edi,0x200` in EDI and uses immediate 0 for the resets
// (`mov [eax+4],0`, `mov [eax+8],0`, `test ecx,ecx`), while ours keeps the zero
// in EDI (`cmp eax,edi`, `mov [eax+8],edi`) and uses immediate 0x200. That one
// zero-vs-0x200 role swap cascades into `f8++` landing in EBX instead of EDI,
// `n` landing in EDI, `remaining` in EBP (original: `n`=EBP, `remaining`=EDI,
// `remaining`=EBX in the tail loop), and the a6 tail loop swapping EBX/EBP/EDI.
//
// Tried and scored, none beat 66.0%: if/else or single-return form for the pop
// helper (63.5%), pointer `(char*)r + h*12` vs `&r->entry[h-1]` (same), a
// function-local `int cap = 0x200` used for the three first-loop bounds (folds
// straight back to immediates), a `static __inline` push helper (same 1006
// bytes), swapping or hoisting the `n`/`remaining` declarations to function
// scope, and computing `remaining` before memcpy (57.0%).
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
    char* p = text + 4;
    int remaining = size - 4;
    int n = 0;
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
