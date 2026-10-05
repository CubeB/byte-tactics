// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Obj_004babd0 {
    char unknown_0[0xc8];
    unsigned char* buffer;             // +0xc8
    char unknown_cc[0xf0 - 0xcc];
    unsigned short bit0_6 : 7;         // +0xf0
    unsigned short flag7 : 1;
    unsigned short bit8_15 : 8;
};
#pragma pack(pop)

extern Obj_004babd0* GetDisplay(void);

void __stdcall SortByBrightness(PALETTEENTRY* palette, int* sums,
                            unsigned char* order);
unsigned char __stdcall NearestColorInBand(PALETTEENTRY* palette, int* sums,
                                     unsigned char* order, PALETTEENTRY color);

// FUNCTION: 0x4babd0
unsigned char* __stdcall BuildLightTable(PALETTEENTRY* palette)
{
    Obj_004babd0* obj = GetDisplay();
    if (obj->flag7) {
        int sums[256];
        unsigned char order[256];
        SortByBrightness(palette, sums, order);
        for (int row = 0; row < 32; row++) {
            double factor = 1.0 - row * -0.03333333333333333;
            for (int i = 0; i < 256; i++) {
                int red = (int)(palette[i].peRed * factor);
                int green = (int)(palette[i].peGreen * factor);
                int blue = (int)(palette[i].peBlue * factor);
                PALETTEENTRY color;
                if (red > 255) {
                    red = 255;
                }
                if (green > 255) {
                    green = 255;
                }
                if (blue > 255) {
                    blue = 255;
                }
                color.peRed = red;
                color.peGreen = green;
                color.peBlue = blue;
                obj->buffer[i + row * 256] =
                    NearestColorInBand(palette, sums, order, color);
            }
        }
        return obj->buffer;
    }
    return 0;
}
