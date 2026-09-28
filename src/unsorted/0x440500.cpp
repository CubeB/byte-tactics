// Decompiled by Space Bunny Free. Names are provisional.
// Fills a 2-bit-per-cell map (field_18, indexed [row >> 4] * width + col) with
// the footprint passability of this class. Pass 1 walks the map row by row and
// stores the raw cost of every cell, marking 1 on the outer edge of the usable
// (3) area; pass 2 reads the map back column by column and marks 1 on the
// inner edge of that set. Both passes work on one scratch row (or column) of
// bytes with the two bytes in front of it zeroed and the cells past the end of
// the row zeroed too, so a footprint wider than 1 sees zeros at the row ends.

#include <stddef.h>

void* operator new(size_t size);
void operator delete(void* p);

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

struct Game_00440500 {
    char unknown_0[0x14233];
    unsigned int width;                // +0x14233
    unsigned int height;               // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00440500* cells;              // +0x14287
};
#pragma pack(pop)

extern Game_00440500* g_game;

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

    void FUN_00440500();
};

int __stdcall FUN_0047de60(Class_00440500* obj, Cell_00440500* cell);


static inline void setcell(unsigned int* q, int sh, unsigned int val)
{
    *q = (*q & ~(3 << sh)) | (val << sh);
}

// FUNCTION: 0x440500
void Class_00440500::FUN_00440500()
{
    int size;
    if (field_4 + field_10 > field_6 + field_14)
        size = field_4 + field_10;
    else
        size = field_6 + field_14;
    char* buf = (char*)operator new(size + 3);
    unsigned char* v = (unsigned char*)buf + 2;

    for (int j = 0; j < field_14; j++) {
        Cell_00440500* c = &g_game->cells[j * field_10];
        int n;
        for (n = 0; n < field_10; n++, c++)
            v[n] = (unsigned char)FUN_0047de60(this, c);
        v[-2] = 0;
        v[-1] = 0;
        for (n = 0; n <= field_4; n++)
            v[n + field_10] = 0;
        unsigned char b = 0;
        unsigned char* p = v;
        for (int i = 0; i < field_10; i++) {
            int e = i + field_4 - 1;
            if (v[e] <= b) {
                b = v[e];
            } else if (p[-1] <= b) {
                b = p[0];
                for (int k = i + 1; k <= e; k++)
                    if (b >= v[k])
                        b = v[k];
            }
            if (b == 3 && (p[-1] < 3 || v[e + 1] < 3)) {
                setcell(&field_18[((i >> 4) * field_10 + j)], (i & 0xf) * 2, 1);
            } else {
                setcell(&field_18[((i >> 4) * field_10 + j)], (i & 0xf) * 2, b);
            }
            p++;
            i++;
        }
    }

    for (int jj = 0; jj < field_10; jj++) {
        int m;
        for (m = 0; m < field_14; m++)
            v[m] = (unsigned char)((field_18[((m >> 4) * field_10 + jj)]
                                    >> ((m & 0xf) * 2)) & 3);
        v[-2] = 0;
        v[-1] = 0;
        for (m = 0; m <= field_6; m++)
            v[m + field_14] = 0;
        unsigned char b = 0;
        for (int i = 0; i < field_14; i++) {
            int e = i + field_6 - 1;
            if (v[e] <= b) {
                b = v[e];
            } else if (v[i - 1] <= b) {
                b = v[i];
                for (int k = i + 1; k <= e; k++)
                    if (b >= v[k])
                        b = v[k];
            }
            if (b == 3 && (v[i - 1] < 3 || v[e + 1] < 3)) {
                setcell(&field_18[((i >> 4) * field_10 + jj)], (i & 0xf) * 2, 1);
            } else {
                setcell(&field_18[((i >> 4) * field_10 + jj)], (i & 0xf) * 2, b);
            }
        }
    }

    operator delete(buf);
}
