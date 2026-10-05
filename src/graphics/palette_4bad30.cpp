// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct App_004bad30 {
    char unknown_0[0xcc];
    unsigned char* buffer;              // +0xcc
    char unknown_d0[0xf0 - 0xd0];
    unsigned short bit0_7 : 8;          // +0xf0
    unsigned short flag8 : 1;
    unsigned short bit9_15 : 7;
};
#pragma pack(pop)

extern App_004bad30* GetDisplay(void);
void __stdcall SortByBrightness(PALETTEENTRY* palette, int* sums, unsigned char* order);
unsigned char __stdcall NearestColorInBand(PALETTEENTRY* palette, int* sums, unsigned char* order,
                                     PALETTEENTRY color);

// FUNCTION: 0x4bad30
unsigned char* __stdcall BuildGrayTable(PALETTEENTRY* palette)
{
    App_004bad30* app = GetDisplay();
    if (app->flag8) {
        PALETTEENTRY color;
        unsigned char order[256];
        int sums[256];
        SortByBrightness(palette, sums, order);
        for (int i = 0; i < 0x100; i++) {
            unsigned char gray = (unsigned char)((unsigned int)(palette[i].peRed +
                palette[i].peGreen + palette[i].peBlue) / 3);
            color.peGreen = gray;
            color.peBlue = gray;
            color.peRed = gray;
            app->buffer[i] = NearestColorInBand(palette, sums, order, color);
        }
        return app->buffer;
    }
    return 0;
}
