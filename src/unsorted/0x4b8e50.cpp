// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Bitmap_004b8e50 {
    unsigned short width;           // +0x0
    unsigned short height;          // +0x2
    short unknown_4;                // +0x4
    short unknown_6;                // +0x6
    char unknown_8;                 // +0x8
    char locked;                    // +0x9
    char unknown_a;                 // +0xa
    char unknown_b;                 // +0xb
    int unknown_c;                  // +0xc
    unsigned char* plane0;          // +0x10
    unsigned char* plane1;          // +0x14
};

// FUNCTION: 0x4b8e50
void __stdcall FUN_004b8e50(Bitmap_004b8e50* b, int color)
{
    if (b->locked == 0) {
        memset(b->plane0, color, b->width * b->height);
        if (b->plane1 != 0) {
            memset(b->plane1, 0, b->width * b->height);
        }
    }
}
