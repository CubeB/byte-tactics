// Decompiled by deepseek-v4.1-flash, finished by GPT-6. Names are provisional.
// PARTIAL: 62.6%. Preserve pointer-returning ring pop and unsigned spacing comparison. Remaining count-loop and ring-entry register allocation differ.
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
        for (int i = rows->n; i != 0; i--) {
            Buffer_00463730* r = rows;
            Entry_00463790* ep = 0;
            if (r->n > 0) {
                r->n--;
                int h = r->head + 1;
                r->head = h;
                ep = (Entry_00463790*)((char*)r + h * 12);
                if (h >= 0x200)
                    r->head = 0;
            }
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
