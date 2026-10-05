// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by claude-opus-5-5, finished by claude-opus-5-5, finished by GPT-6, finished by claude-opus-5-5. Names are provisional.
// MATCH (claude-opus-5-5, #5067, from 93.6%). Grows this->bitmap to cover the model
// and its child pieces, then copies or re-blits `bmp` (pixels, then the shade plane
// through a pixels/shade swap) into it. The last three levers:
//   * One named `origin(0, 0, 0)` is passed to both AddModelBounds calls. Built in
//     place for the first call it gives the three separate zero registers; in the
//     loop its fields are rematerialised from one `xor eax, eax`. A fresh
//     `Pos_4589c0()` (memset) in the loop stores the middle field through a
//     `mov ecx, eax` copy, and three-store constructors give three xors.
//   * The pixels/shade swaps and the dx/dy save and restore go through one inline
//     `Swap(T&, T&)`. Through the references MSVC 5 keeps each load and store in
//     source order (load dx, store dx, load dy, store dy); written out by hand it
//     hoists the second load above the first store.
// Earlier levers that still matter: the y projection goes through a `Fixed`
// temporary (one expression folds to `(z - ya) << 16` and loses the 16-bit sar);
// the outer bounds are declared `minX, maxX, minY, maxY` and the child ones
// `cminX, cminY, cmaxX, cmaxY` (frame order); `surface.bits = ...shade` is read
// before the first swap, as the original loads it before the swap stores.
#include <string.h>

struct Child_4589c0;
struct Model_4589c0;

union Fixed { int value; struct { unsigned short fraction; short whole; }; };

struct Pos_4589c0 {
    int x;
    int y;
    int z;
    Pos_4589c0(int a, int b, int c) { x = a; y = b; z = c; }
};

#pragma pack(push, 2)
struct Owner_4589c0 {
    char unknown_0[0x6a];
    int x;                          // +0x6a
    int y;                          // +0x6e
    int z;                          // +0x72
    char unknown_76[0x8a - 0x76];
    Child_4589c0* firstChild;       // +0x8a
};

struct Model_4589c0 {
    int pieceCount;                 // +0x00
    char unknown_4[0xc - 0x4];
    Owner_4589c0* owner;            // +0x0c
    void* bitmap;                   // +0x10
};

struct Child_4589c0 {
    char unknown_0[0x6a];
    int x;                          // +0x6a
    int y;                          // +0x6e
    int z;                          // +0x72
    char unknown_76[0x8e - 0x76];
    Child_4589c0* next;             // +0x8e
    char unknown_92[0x9e - 0x92];
    Model_4589c0* model;            // +0x9e
    char unknown_a2[0x110 - 0xa2];
    unsigned int flags;             // +0x110
};

#pragma pack(pop)

struct Image_4589c0 {
    unsigned short width;           // +0x00
    unsigned short height;          // +0x02
    short dx;                       // +0x04
    short dy;                       // +0x06
    unsigned char colour;           // +0x08
    unsigned char flag9;            // +0x09
    unsigned char count;            // +0x0a
    unsigned char kind;             // +0x0b
    char unknown_c[4];
    unsigned char* pixels;          // +0x10
    unsigned char* shade;           // +0x14
};

struct Src_4589c0 {
    unsigned short width;           // +0x00
    unsigned short height;          // +0x02
    unsigned short x;               // +0x04
    unsigned short y;               // +0x06
    char unknown_8[8];
    int bits;                       // +0x10
};

struct Surface_4589c0 {
    int width;                      // +0x00
    int height;                     // +0x04
    int pitch;                      // +0x08
    void* bits;                     // +0x0c
    int field_10;                   // +0x10
    int field_14;                   // +0x14
    unsigned short x;               // +0x18
    unsigned short y;               // +0x1a
    char unknown_1c[0x10];
    unsigned int flag0 : 1;         // +0x2c
    unsigned int flag1 : 1;
};

class Class_00458310 {
public:
    void AddModelBounds(int* minX, int* maxX, int* minY, int* maxY, Model_4589c0* model,
                      Pos_4589c0 pos);
};

class Class_00458d30 {
public:
    int ShadeByIntensity(Image_4589c0* image, Model_4589c0* model);
};

class Surface;

struct Bitmap_4589c0 {
    unsigned short width;           // +0x00
    unsigned short height;          // +0x02
    short dx;                       // +0x04
    short dy;                       // +0x06
    unsigned char colour;           // +0x08
    unsigned char flag9;            // +0x09
    unsigned char count;            // +0x0a
    unsigned char kind;             // +0x0b
    int unknown_c;                  // +0x0c
    void* field_10;                 // +0x10
};

void __stdcall SurfaceFromFrame(Surface_4589c0* dst, Src_4589c0* src);
void __stdcall DrawFrame(Surface* dst, Bitmap_4589c0* bmp, int x, int y);

class Class_00459200 {
public:
    char unknown_0[0x10];
    Image_4589c0* bitmap;           // +0x10
    void MergeIntoComposite(Image_4589c0* src, Model_4589c0* model);
};

template <class T> inline void Swap(T& a, T& b)
{
    T t = a;
    a = b;
    b = t;
}

// FUNCTION: 0x4589c0
void Class_00459200::MergeIntoComposite(Image_4589c0* bmp, Model_4589c0* model)
{
    Pos_4589c0 origin(0, 0, 0);
    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;

    ((Class_00458310*)this)->AddModelBounds(&minX, &maxX, &minY, &maxY, model, origin);
    Child_4589c0* child = model->owner->firstChild;
    while (child != 0) {
        if ((child->flags & 0x20000) == 0) {
            int cminX = 0;
            int cminY = 0;
            int cmaxX = 0;
            int cmaxY = 0;
            ((Class_00458310*)this)->AddModelBounds(&cminX, &cmaxX, &cminY, &cmaxY,
                                                  child->model, origin);
            struct Vec { Fixed x, y, z; };
            int* op = &model->owner->x;
            Vec d;
            d.x.value = child->x - op[0];
            d.y.value = child->y - op[1];
            d.z.value = child->z - op[2];
            Vec s = d;
            short ya = d.y.whole >> 1;
            Fixed yv;
            yv.value = (ya << 16) - ya + d.z.whole;
            yv.value <<= 16;
            s.y = yv;
            int xoff = s.x.whole;
            int yo = s.y.whole;
            cminX += xoff;
            cminY += yo;
            cmaxX += xoff;
            cmaxY += yo;
            if (cminX < minX) minX = cminX;
            if (cmaxX > maxX) maxX = cmaxX;
            if (cminY < minY) minY = cminY;
            if (cmaxY > maxY) maxY = cmaxY;
        }
        child = child->next;
    }
    int x0 = -bmp->dx;
    int x1 = bmp->width - bmp->dx;
    int y0 = -bmp->dy;
    int y1 = bmp->height - bmp->dy;
    if (minX < x0) x0 = minX;
    if (maxX > x1) x1 = maxX;
    if (minY < y0) y0 = minY;
    if (maxY > y1) y1 = maxY;
    int newW = x1 - x0;
    int newH = y1 - y0;
    x0 = -x0;
    y0 = -y0;
    this->bitmap->width = (unsigned short)newW;
    this->bitmap->height = (unsigned short)newH;
    this->bitmap->dx = (short)x0;
    this->bitmap->dy = (short)y0;
    this->bitmap->colour = bmp->colour;
    if (newW == bmp->width && newH == bmp->height) {
        memcpy(this->bitmap->pixels, bmp->pixels, bmp->width * bmp->height);
        memcpy(this->bitmap->shade, bmp->shade, bmp->width * bmp->height);
    } else {
        short sdx = 0;
        short sdy = 0;
        Swap(bmp->dx, sdx);
        Swap(bmp->dy, sdy);
        Surface_4589c0 surface;
        SurfaceFromFrame(&surface, (Src_4589c0*)this->bitmap);
        memset(this->bitmap->pixels, this->bitmap->colour,
               this->bitmap->height * this->bitmap->width);
        memset(this->bitmap->shade, 0, this->bitmap->height * this->bitmap->width);
        DrawFrame((Surface*)&surface, (Bitmap_4589c0*)bmp,
                     this->bitmap->dx - sdx, this->bitmap->dy - sdy);
        surface.bits = this->bitmap->shade;
        Swap(bmp->pixels, bmp->shade);
        DrawFrame((Surface*)&surface, (Bitmap_4589c0*)bmp,
                     this->bitmap->dx - sdx, this->bitmap->dy - sdy);
        Swap(bmp->pixels, bmp->shade);
        Swap(bmp->dx, sdx);
        Swap(bmp->dy, sdy);
    }
    ((Class_00458d30*)this)->ShadeByIntensity(this->bitmap, model);
}
