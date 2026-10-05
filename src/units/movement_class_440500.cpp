// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash. Names are provisional.
// Builds the 2-bit-per-cell passability map (field_18) for one footprint.
// Pass 1 walks the map row by row, filling one scratch row with the raw cost
// of every cell and marking 1 on the outer edge of the usable (3) area; pass 2
// reads the map back column by column and marks 1 on the inner edge. Each pass
// works on a scratch row whose two bytes in front and field_4/field_6 cells
// past the end are zeroed.
//
// <stdio.h> is load bearing: it changes nothing the function uses, but it is
// what makes MSVC put the pointer first in `v[n]` instead of the index.
// The extra braces around the two loops are needed because MSVC 5 leaks a
// for-init variable into the enclosing scope, so the two `int j` clash.

#include <stddef.h>
#include <stdio.h>

void* __cdecl operator new(size_t size);
void __cdecl operator delete(void* p);

#pragma pack(push, 1)
struct Cell_00440500 {
    unsigned short spot;               // +0x0
    char unknown_2[2];
    unsigned char field_4;
    unsigned char high;                // +0x5
    unsigned char low;                 // +0x6
    unsigned char unknown_7;
    unsigned short feature;            // +0x8
    unsigned char spotY;               // +0xa
    unsigned char spotX;               // +0xb
    unsigned char flags;               // +0xc
};

struct Game {
    char unknown_0[0x14233];
    unsigned int width;                // +0x14233
    unsigned int height;               // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00440500* cells;              // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

class Class_00440500 {
public:
    int* field_0;                      // +0x0
    short field_4;                     // +0x4, footprint x
    short field_6;                     // +0x6, footprint y
    short field_8;
    short field_a;
    unsigned char field_c;
    unsigned char field_d;
    unsigned char field_e;
    unsigned char field_f;
    unsigned int field_10;             // +0x10, width
    unsigned int field_14;             // +0x14, height
    unsigned int* field_18;            // +0x18, the 2-bit map
    int field_1c;

    void BuildPassMap();
};

int __stdcall FUN_0047de60(Class_00440500* obj, Cell_00440500* cell);

static inline void setcell(unsigned int* q, int sh, unsigned int val)
{
    *q = (*q & ~(3 << sh)) | (val << sh);
}

// FUNCTION: 0x440500
void Class_00440500::BuildPassMap()
{
    unsigned char b;
    unsigned char* p;
    unsigned char* buf;
    int n;
    unsigned char* v;
    unsigned int size;
    if (field_10 + field_4 > field_6 + field_14)
        size = field_10 + field_4;
    else
        size = field_6 + field_14;

    buf = (unsigned char*)operator new(size + 3);
    v = buf + 2;

    {
    for (int j = 0; j < field_14; j++) {
        Cell_00440500* c = &g_game->cells[j * field_10];
        for (n = 0; n < field_10; n++, c++)
            v[n] = (unsigned char)FUN_0047de60(this, c);
        v[-2] = 0;
        v[-1] = 0;
        for (n = 0; n <= field_4; n++)
            (v + n)[field_10] = 0;
        b = 0;
        p = v;
        n = 0;
        for (; n < field_10; n++, p++) {
            int e = n + field_4 - 1;
            if (v[e] <= b) {
                b = v[e];
            } else if (p[-1] <= b) {
                b = p[0];
                int k;
                for (k = n + 1; k <= e; k++)
                    if (b >= v[k]) b = v[k];
            }
            if (b == 3 && (p[-1] < 3 || v[e + 1] < 3))
                setcell(&field_18[(j >> 4) * field_10 + n], (j & 0xf) * 2, 1);
            else
                setcell(&field_18[(j >> 4) * field_10 + n], (j & 0xf) * 2, b);
        }
    }
    }

    {
    for (int j = 0; j < field_10; j++) {
        for (n = 0; n < field_14; n++)
            v[n] = (unsigned char)((field_18[(n >> 4) * field_10 + j]
                                    >> ((n & 0xf) * 2)) & 3);
        v[-2] = 0;
        v[-1] = 0;
        for (n = 0; n <= field_6; n++)
            (v + n)[field_14] = 0;
        b = 0;
        n = 0;
        for (; n < field_14; n++) {
            int e = n + field_6 - 1;
            if (v[e] <= b) {
                b = v[e];
            } else if (v[n - 1] <= b) {
                b = v[n];
                int k;
                for (k = n + 1; k <= e; k++)
                    if (b >= v[k]) b = v[k];
            }
            if (b == 3 && (v[n - 1] < 3 || v[e + 1] < 3))
                setcell(&field_18[(n >> 4) * field_10 + j], (n & 0xf) * 2, 1);
            else
                setcell(&field_18[(n >> 4) * field_10 + j], (n & 0xf) * 2, b);
        }
    }
    }

    operator delete(buf);
}
