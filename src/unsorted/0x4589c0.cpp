// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash, finished by GPT-6, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by claude-opus-5-5, finished by claude-opus-5-5. Names are provisional.
// claude-opus-5-5: 93.4% (from 92.7%). The child loop's clip ints are declared
// `cminX, cminY, cmaxX, cmaxY` and summed in that order (cx, cy, dx2, dy2). That puts
// them in the original's slots (cmaxX at cminX+8, cminY at +4, as the call's pointer
// order needs); tools/stackcmp.py reports every local in place. Found by sweeping all
// 24x24 declaration and use orders: summing in the original's cx, dx2, cy, dy2 order
// with these declarations gives 91.3%, and the permuter found nothing more in 8 min.
// Still differs: (a) the bounds block keeps the right slots but assigns registers
// differently (the original loads cminY into ebp and cmaxX into edi); (b) in the
// outer call's Pos_4589c0 setup, `lea ecx, [esp+0x38]` comes after the zero stores
// instead of before them; (c) two scheduling nits MSVC 5 will not give up:
// `bmp->dx = 0; bmp->dy = 0;` emits load/load/store/store instead of
// load/store/load/store (interleaving the statements by hand does not change it), and
// the last swap emits `bmp->pixels = bmp->shade` after `mov ax, word [esp+0x7c]`
// instead of before it (two temps, one temp and reversed temps all score the same).
// space-bunny-free: 92.7% (from 75.3%). Three changes: (1) the y projection is
// computed through a `Fixed` temporary, which keeps MSVC 5 from rewriting the stored
// value to `(z - ya) << 16` (it does that when the whole expression is one statement,
// because only the high half is ever read back); (2) the four outer bounds are declared
// `minX, maxX, minY, maxY`, which is the order MSVC zeroes them in and the order the
// prologue needs; (3) `surface.bits = this->bitmap->shade;` comes BEFORE the
// pixels/shade swap, so MSVC loads `this->bitmap` and `this->bitmap->shade` before the
// two stores and hoists the `lea` of `&surface` early for the second blit, as the
// original does.
// space-bunny-free: 85.9% (from 75.3%). Change: the y projection is computed through a
// `Fixed` temporary (`yv.value = (ya << 16) - ya + d.z.whole; yv.value <<= 16; s.y = yv;`).
// Written as one expression, MSVC 5 rewrites the value it stores into `(z - ya) << 16`,
// because only the high half is ever read back; through the temporary it keeps the
// full `shl 16 / sub / add / shl 16` chain and the 16-bit `sar dx,1`, which is what
// the original does.
// claude-opus-5-5: 75.3% (from 61.8%). Changes: the outer call's Pos is built by a
// three-int constructor (three separate zero registers in the prologue), the child
// offset is a plain 3-int struct copied into a second one (frame slots 0x30/0x3c now
// match), and the bounds block negates x0/y0 in place with x1 computed before y0.
// Still differs: the y projection. The original keeps ((ya << 16) - ya + z) << 16
// unfolded with a 16-bit `sar dx,1`; every spelling tried (multiply by 0xffff or
// 0x10000, separate `<<=` statements, short or unsigned intermediates, union
// round trips, an inline ToFixed) folds to (z - ya) << 16. Only a shift count held
// in a local (`int sh = 16`) keeps the 16-bit sar (68.3% at that stage), and that is a
// dummy construct, so it is not used. Remaining register and scheduling noise in the
// else branch swaps follows.
#include <string.h>

struct Child_4589c0;
struct Model_4589c0;

union Fixed { int value; struct { unsigned short fraction; short whole; }; };

struct Pos_4589c0 {
    int x;
    int y;
    int z;
    Pos_4589c0() { memset(this, 0, sizeof(*this)); }
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
    void FUN_00458310(int* minX, int* maxX, int* minY, int* maxY, Model_4589c0* model,
                      Pos_4589c0 pos);
};

class Class_00458d30 {
public:
    int FUN_00458dd0(Image_4589c0* image, Model_4589c0* model);
};

class Class_004c6ae0;

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

void __stdcall FUN_004b8a80(Surface_4589c0* dst, Src_4589c0* src);
void __stdcall FUN_004b7f90(Class_004c6ae0* dst, Bitmap_4589c0* bmp, int x, int y);

class Class_00459200 {
public:
    char unknown_0[0x10];
    Image_4589c0* bitmap;           // +0x10
    void FUN_004589c0(Image_4589c0* src, Model_4589c0* model);
};

// FUNCTION: 0x4589c0
void Class_00459200::FUN_004589c0(Image_4589c0* bmp, Model_4589c0* model)
{
    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;

    ((Class_00458310*)this)->FUN_00458310(&minX, &maxX, &minY, &maxY, model, Pos_4589c0(0, 0, 0));
    Child_4589c0* child = model->owner->firstChild;
    while (child != 0) {
        if ((child->flags & 0x20000) == 0) {
            int cminX = 0;
            int cminY = 0;
            int cmaxX = 0;
            int cmaxY = 0;
            Pos_4589c0 cpos;
            ((Class_00458310*)this)->FUN_00458310(&cminX, &cmaxX, &cminY, &cmaxY,
                                                  child->model, cpos);
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
            int cx = cminX + xoff;
            int cy = cminY + yo;
            int dx2 = cmaxX + xoff;
            int dy2 = cmaxY + yo;
            if (cx < minX) minX = cx;
            if (dx2 > maxX) maxX = dx2;
            if (cy < minY) minY = cy;
            if (dy2 > maxY) maxY = dy2;
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
        short sdx = bmp->dx;
        short sdy = bmp->dy;
        bmp->dx = 0;
        bmp->dy = 0;
        Surface_4589c0 surface;
        FUN_004b8a80(&surface, (Src_4589c0*)this->bitmap);
        memset(this->bitmap->pixels, this->bitmap->colour,
               this->bitmap->height * this->bitmap->width);
        memset(this->bitmap->shade, 0, this->bitmap->height * this->bitmap->width);
        FUN_004b7f90((Class_004c6ae0*)&surface, (Bitmap_4589c0*)bmp,
                     this->bitmap->dx - sdx, this->bitmap->dy - sdy);
        surface.bits = this->bitmap->shade;
        unsigned char* t = bmp->pixels;
        bmp->pixels = bmp->shade;
        bmp->shade = t;
        FUN_004b7f90((Class_004c6ae0*)&surface, (Bitmap_4589c0*)bmp,
                     this->bitmap->dx - sdx, this->bitmap->dy - sdy);
        t = bmp->pixels;
        bmp->pixels = bmp->shade;
        bmp->shade = t;
        bmp->dx = sdx;
        bmp->dy = sdy;
    }
    ((Class_00458d30*)this)->FUN_00458dd0(this->bitmap, model);
}