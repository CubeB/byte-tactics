// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct App_004baf30 {
    char unknown_0[0xd0];
    unsigned char* buffer;             // +0xd0
    char unknown_d4[0xf0 - 0xd4];
    unsigned short bit0_8 : 9;         // +0xf0
    unsigned short flag9 : 1;
    unsigned short bit10_15 : 6;
};
#pragma pack(pop)

extern App_004baf30* GetDisplay(void);
void __stdcall SortByBrightness(PALETTEENTRY* palette, int* sums, unsigned char* order);
unsigned char __stdcall NearestColorInBand(PALETTEENTRY* palette, int* sums, unsigned char* order,
                                     PALETTEENTRY color);

// FUNCTION: 0x4baf30
unsigned char* __stdcall BuildBlueTable(PALETTEENTRY* palette)
{
    App_004baf30* app = GetDisplay();
    if (app->flag9) {
        PALETTEENTRY color;
        unsigned char order[256];
        int sums[256];
        SortByBrightness(palette, sums, order);
        for (int i = 0; i < 0x100; i++) {
            color.peRed = palette[i].peRed >> 1;
            color.peGreen = palette[i].peGreen >> 1;
            int b = palette[i].peBlue >> 1;
            if (b + 0x3c > 0xff)
                color.peBlue = 0xff;
            else
                color.peBlue = b + 0x32;
            app->buffer[i] = NearestColorInBand(palette, sums, order, color);
        }
        return app->buffer;
    }
    return 0;
}
