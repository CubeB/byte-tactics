// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Builds the 256x256 colour-blend table at obj->buffer when flag5 is set:
// diagonal entries are the palette index itself, off-diagonal entries are the
// palette index closest to the per-channel average of palette entries row and
// col.
#include <windows.h>

#pragma pack(push, 1)
struct Obj_004ba750 {
    char unknown_0[0xc0];
    unsigned int* buffer;              // +0xc0
    char unknown_c4[0xf0 - 0xc4];
    unsigned short bit0_4 : 5;
    unsigned short flag5 : 1;
    unsigned short bit6_15 : 10;
};
#pragma pack(pop)

struct RGBA {
    unsigned char r;                   // +0
    unsigned char g;                   // +1
    unsigned char b;                   // +2
    unsigned char a;                   // +3
};

extern Obj_004ba750* GetDisplay(void);
void __stdcall SortByBrightness(unsigned char* data, int* sums, unsigned char* idx);
unsigned char __stdcall NearestColorInBand(PALETTEENTRY* palette, int* band, unsigned char* order, PALETTEENTRY color);

// FUNCTION: 0x4ba750
unsigned int* __stdcall BuildAlphaTable(unsigned char* data)
{
    Obj_004ba750* obj = GetDisplay();
    if (obj->flag5) {
        int sums[256];
        unsigned char idx[256];
        SortByBrightness(data, sums, idx);
        RGBA* pal = (RGBA*)data;
        int row = 0;
        int off = 0;
        for (; off < 0x10000; off += 0x100, row++) {
            for (int col = 0; col < 0x100; col++) {
                if (row == col) {
                    ((unsigned char*)obj->buffer)[off + col] = (unsigned char)row;
                } else {
                    PALETTEENTRY color;
                    color.peRed = (pal[row].r + pal[col].r) / 2;
                    color.peGreen = (pal[row].g + pal[col].g) / 2;
                    color.peBlue = (pal[row].b + pal[col].b) / 2;
                    ((unsigned char*)obj->buffer)[off + col] = NearestColorInBand((PALETTEENTRY*)data, sums, idx, color);
                }
            }
        }
        return obj->buffer;
    }
    return 0;
}
