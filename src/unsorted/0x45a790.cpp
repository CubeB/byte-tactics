// Decompiled by space-bunny-free. Names are provisional.
// Builds the picture of a piece: FUN_0045a510 measures the piece bounding box
// of the state, the scratch image at this->field_10 is cleared to its key
// colour and the pieces are drawn into it (FUN_0045a610), the background image
// is blitted over it, and the run length compressed result becomes the state's
// sprite (FUN_004b9e60, then a one plane bitmap from the arena).
#include <string.h>

struct Image_0045a790 {
    unsigned short width;               // +0x0
    unsigned short height;              // +0x2
    unsigned short x;                   // +0x4
    unsigned short y;                   // +0x6
    unsigned char key;                  // +0x8 key colour
    char unknown_9[7];
    unsigned char* pixels;              // +0x10
    unsigned char* compressed;          // +0x14
};

struct Bitmap_00437b50 {
    short width;                        // +0x0
    short height;                       // +0x2
    short field_4;                      // +0x4
    short field_6;                      // +0x6
    char field_8;                       // +0x8
    char field_9;                       // +0x9
    char unknown_a[2];
    int field_c;                        // +0xc
    unsigned char* pixels;              // +0x10
    int field_14;                       // +0x14
};

struct State_0045a790 {
    char unknown_0[0x14];
    Bitmap_00437b50* sprite;            // +0x14 the picture built here
    char unknown_18[0x22 - 0x18];
};

class Class_00437a30 {
public:
    int unknown_0[4];
    Image_0045a790* scratch;            // +0x10 scratch image
    int FUN_00437b50(Bitmap_00437b50** handle, int w, int h);
    void FUN_0045a790(State_0045a790* obj, Image_0045a790* dest);
};

class Class_0045a510 {
public:
    void FUN_0045a510(int* w, int* h, int* x, int* y, void* obj);
};

class Class_0045a610 {
public:
    void FUN_0045a610(Image_0045a790* img, void* obj);
};

void __stdcall FUN_004b9d70(Image_0045a790* dst, Image_0045a790* src, int x, int y);
int __stdcall FUN_004b9e60(unsigned char* dest, Image_0045a790* img);

// FUNCTION: 0x45a790
void Class_00437a30::FUN_0045a790(State_0045a790* obj, Image_0045a790* dest)
{
    int w, h, x, y;
    ((Class_0045a510*)this)->FUN_0045a510(&w, &h, &x, &y, obj);
    scratch->width = (unsigned short)w;
    scratch->height = (unsigned short)h;
    scratch->x = (unsigned short)x;
    scratch->y = (unsigned short)y;
    memset(scratch->pixels, scratch->key, h * w);
    memset(scratch->compressed, 0, h * w);
    ((Class_0045a610*)this)->FUN_0045a610(scratch, obj);
    FUN_004b9d70(dest, scratch, 5, 0);
    int size = FUN_004b9e60(scratch->compressed, scratch);
    FUN_00437b50(&obj->sprite, size, 1);
    Bitmap_00437b50* bmp = obj->sprite;
    memcpy(bmp->pixels, scratch->compressed, size);
    bmp->width = scratch->width;
    bmp->height = scratch->height;
    bmp->field_4 = scratch->x;
    bmp->field_6 = scratch->y;
    bmp->field_9 = 1;
}
