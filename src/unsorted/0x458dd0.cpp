// Decompiled by GPT-6-Luna. Names are provisional.
// Best result: 88.9%. The remaining differences are branch-local signed
// quotient corrections, two 85-divisor result adjustments, and final call
// argument loading. A shared helper-call epilogue produced a worse match.
#include <math.h>
struct Image_458dd0 {
    unsigned short width;
    unsigned short height;
    char unknown_4[4];
    unsigned char transparent;
    char unknown_9[7];
    unsigned char* pixels;
    unsigned char* shade;
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

class Class_00458d30 {
public:
    void FUN_00458d30(Image_458dd0* image, int level, int above, int below, int between);
    int FUN_00458dd0(Image_458dd0* image, Model_458dd0* model);
};

class Class_00458fa0 {
public:
    void FUN_00458fa0(Image_458dd0* image, Model_458dd0* model, int palette);
};

// FUNCTION: 0x458dd0
int Class_00458d30::FUN_00458dd0(Image_458dd0* image, Model_458dd0* model)
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
        int level = ((alpha - 0xeb) * 255) / 20;
        FUN_00458d30(image, level, -2, -2, palette1);
    } else if (alpha > 200) {
        int level = ((alpha - 200) * 255) / 35;
        FUN_00458d30(image, level, -2, -2, palette1);
    } else if (alpha > 0x73) {
        int level = ((0x73 - alpha) * 255) / 85;
        level--;
        FUN_00458d30(image, level, -2, palette1, palette2);
    } else if (alpha > 0x1e) {
        int level = ((0x1e - alpha) * 255) / 85;
        level--;
        FUN_00458d30(image, level, palette1, -1, palette2);
    } else {
        int level = (alpha * 255) / 30;
        FUN_00458d30(image, level, -1, -1, palette1);
    }

    ((Class_00458fa0*)this)->FUN_00458fa0(image, model, palette2);
    return 1;
}
