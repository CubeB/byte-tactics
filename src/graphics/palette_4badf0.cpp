// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Obj_004badf0 {
    char unknown_0[0xc4];
    unsigned char* buffer;             // +0xc4
    char unknown_c8[0xf0 - 0xc8];
    unsigned short bit0_5 : 6;         // +0xf0
    unsigned short flag6 : 1;
    unsigned short bit7_15 : 9;
};
#pragma pack(pop)

extern Obj_004badf0* FUN_004b6220(void);

void __stdcall FUN_004ba920(PALETTEENTRY* palette, int* sums, unsigned char* order);
unsigned char __stdcall FUN_004ba9d0(PALETTEENTRY* palette, int* sums, unsigned char* order,
                                     PALETTEENTRY color);

extern double DAT_004fdbe8;

// Builds the 32 row (x 256 entries) shaded palette table into app->buffer.
// The scaling factor starts at 0.0 and grows by 0.06875 (DAT_004fdbe8 is
// -0.06875) once per row, so the first row is all black; kept as it is in the
// original.
// FUNCTION: 0x4badf0
unsigned char* __stdcall FUN_004badf0(PALETTEENTRY* palette)
{
    Obj_004badf0* app = FUN_004b6220();
    if (app->flag6) {
        PALETTEENTRY color;
        unsigned char order[256];
        int sums[256];
        FUN_004ba920(palette, sums, order);
        double factor = 0.0;
        int offset = 0;
        do {
            for (int i = 0; i < 256; i++) {
                unsigned short v;
                v = (unsigned short)(palette[i].peRed * factor);
                color.peRed = v;
                if (v > 0xff)
                    color.peRed = 0xff;
                v = (unsigned short)(palette[i].peGreen * factor);
                color.peGreen = v;
                if (v > 0xff)
                    color.peGreen = 0xff;
                v = (unsigned short)(palette[i].peBlue * factor);
                color.peBlue = v;
                if (v > 0xff)
                    color.peBlue = 0xff;
                app->buffer[offset + i] = FUN_004ba9d0(palette, sums, order, color);
            }
            factor -= DAT_004fdbe8;
            offset += 0x100;
        } while (offset < 0x2000);
        return app->buffer;
    }
    return 0;
}
