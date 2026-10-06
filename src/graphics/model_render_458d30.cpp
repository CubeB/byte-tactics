// Decompiled by Opus, GPT-6-Luna and Space Bunny Free. Names are provisional.

#include <windows.h>
#include <math.h>
// Nothing here uses <time.h>: its symbols put ShadeByIntensity in the
// symbol-id window it matches in (docs/c2-regalloc.md).
#include <time.h>

struct Image_458d30 {
    unsigned short width;            // +0x00
    unsigned short height;           // +0x02
    char unknown_4[4];
    unsigned char transparent;       // +0x08
    char unknown_9[7];
    unsigned char* pixels;           // +0x10
    unsigned char* shade;            // +0x14
};

struct Owner_458dd0 {
    char unknown_0[0xa8];
    unsigned short palette;
    char unknown_aa[0x104 - 0xaa];
    float intensity;
};

struct Model_458dd0 {
    int pieceCount;
    char unknown_4[8];
    Owner_458dd0* owner;
};

extern char* g_game;

class Class_00458fa0 {
public:
    void DrawPieceEdges(Image_458d30* image, Model_458dd0* model, int palette);
};

static inline unsigned char Scale(int v, int num, int div) { return (unsigned char)((v * num) / div); }

// A method that ignores `this`: its one caller (0x458dd0) passes its own
// `this` through in ecx.
class Class_00458d30 {
public:
    void RecolorByShade(Image_458d30* img, unsigned char level, int above, int below, int between);
    int ShadeByIntensity(Image_458d30* image, Model_458dd0* model);
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

// MATCH (449 bytes). The level argument of Class_00458d30::RecolorByShade is
// narrow (unsigned char, see the EHHH in that function's mangled name), and
// telling the compiler so is what makes the register allocation come out
// right:
//   * RecolorByShade's `level` parameter is declared `unsigned char`, so only the
//     low byte of the quotient has to be correct. That is what turns the
//     `(x * 255) / 85 - 1` of the two middle branches into a byte decrement
//     (`add edx,ecx; dec dl`) instead of a three-operand `lea`, and it is why
//     the shared sign fix-up of the last branch lands in ecx, which lets MSVC
//     merge it with the first branch's fix-up (the first branch then jumps
//     straight into the middle of the tail).
//   * the three branches with no `- 1` reach that quotient through the
//     `Scale` helper below. The inlined call boundary is what stops MSVC from
//     sinking one of the `push -1`s into the middle of the sign fix-up; with
//     the expression written inline, the fix-up is no longer a suffix of the
//     last branch and the tail merge does not happen.
// FUNCTION: 0x458dd0
int Class_00458d30::ShadeByIntensity(Image_458d30* image, Model_458dd0* model)
{
    if (image->shade == 0)
        return 0;

    Owner_458dd0* owner = model->owner;
    if (owner->intensity == 0.0f)
        return 0;

    int b = 0;
    b = owner->palette;
    int a = b;
    b ^= 9;
    a ^= 5;
    unsigned int map = *(unsigned int*)(g_game + 0x38a47);
    a += map * 0x21 / 30;
    b += map * 0x39 / 30;
    int palette1;
    int palette2;
    if (a & 0x10)
        palette1 = 0xaf - (a & 0xf);
    else
        palette1 = (a & 0xf) + 0xa0;
    if (b & 0x10)
        palette2 = 0xaf - (b & 0xf);
    else
        palette2 = (b & 0xf) + 0xa0;

    int alpha = (int)(owner->intensity * 255.0f);
    if (alpha > 0xeb) {
        RecolorByShade(image, Scale(alpha - 0xeb, 255, 20), -2, -2, palette1);
    } else if (alpha > 200) {
        RecolorByShade(image, Scale(alpha - 200, 255, 35), -2, -2, palette1);
    } else if (alpha > 0x73) {
        RecolorByShade(image, (unsigned char)(((0x73 - alpha) * 255) / 85 - 1), -2, palette1, palette2);
    } else if (alpha > 0x1e) {
        RecolorByShade(image, (unsigned char)(((0x1e - alpha) * 255) / 85 - 1), palette1, -1, palette2);
    } else {
        RecolorByShade(image, Scale(alpha, 255, 30), -1, -1, palette1);
    }

    ((Class_00458fa0*)this)->DrawPieceEdges(image, model, palette2);
    return 1;
}
