// Decompiled by Opus. Names are provisional.
#include <windows.h>

struct Image_458d30 {
    unsigned short width;            // +0x00
    unsigned short height;           // +0x02
    char unknown_4[4];
    unsigned char transparent;       // +0x08
    char unknown_9[7];
    unsigned char* pixels;           // +0x10
    unsigned char* shade;            // +0x14
};

// A method that ignores `this`: its one caller (0x458dd0) passes its own
// `this` through in ecx.
class Class_00458d30 {
public:
    void RecolorByShade(Image_458d30* img, unsigned char level, int above, int below, int between);
};

// Recolours every opaque pixel by its shade: below level - 4, at or above
// level, or in between. -1 leaves the pixel, -2 makes it transparent.
// FUNCTION: 0x458d30
void Class_00458d30::RecolorByShade(Image_458d30* img, unsigned char level, int above, int below, int between)
{
    unsigned char low;
    if (level < 4)
        low = 0;
    else
        low = level - 4;
    for (int i = 0; i < img->width * img->height; i++) {
        unsigned char* p = &img->pixels[i];
        if (img->pixels[i] != img->transparent) {
            int c;
            if (img->shade[i] < low)
                c = below;
            else if (img->shade[i] >= level)
                c = above;
            else
                c = between;
            switch (c) {
            case -2:
                *p = img->transparent;
                break;
            case -1:
                break;
            default:
                *p = c;
            }
        }
    }
}
