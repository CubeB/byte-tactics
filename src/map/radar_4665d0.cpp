// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
// Rescales a radar picture. The picture is first copied into a temp bitmap of
// its own size, then p is given the new size (x, y) and used as the surface to
// draw on, and the copy is blitted into it, scaled to fit (w - 0x20) by
// (h - 0x80) and centred: when the box is wider than tall the height is the
// scaled one and it is centred, otherwise the width is.

struct Pic_4665d0 {
    unsigned short w;                   // +0x0
    unsigned short h;                   // +0x2
    unsigned short x;                   // +0x4
    unsigned short y;                   // +0x6
};

struct Point_4665d0 {
    int x;
    int y;
};

struct Quad_4665d0 {
    Point_4665d0 p[4];
};

struct Surface_4665d0 {
    int width;                          // +0x0
    int height;                         // +0x4
    int pitch;                          // +0x8
    int bits;                           // +0xc
    int field_10;                       // +0x10
    int field_14;                       // +0x14
    unsigned short x;                   // +0x18
    unsigned short y;                   // +0x1a
    char unknown_1c[0x10];
    unsigned int flag0 : 1;             // +0x2c
    unsigned int flag1 : 1;
};

void* __stdcall AllocFrame(char* name, int width, int height);
void __stdcall SurfaceFromFrame(Surface_4665d0* surface, void* pic);
void __stdcall DrawFrame(Surface_4665d0* surface, short* frame, int x, int y);
void __stdcall FillSurface(Surface_4665d0* surface, int mode);
void __stdcall DrawFrameQuad(Surface_4665d0* surface, void* pic, Quad_4665d0* dst, Quad_4665d0* src);
void __cdecl FUN_004d85a0(void* pic);

// FUNCTION: 0x4665d0
void __stdcall ResizeRadarPicture(Pic_4665d0* pic, int x, int y, int w, int h)
{
    if (pic == 0) {
        return;
    }
    int dwx;
    int dhy;
    int sw;
    int sh;
    Quad_4665d0 src;
    Quad_4665d0 dst;
    Surface_4665d0 surface;
    int dw = w - 0x20;
    int dh = h - 0x80;
    int ox;
    int oy;
    if (dw >= dh) {
        dwx = x;
        dhy = dh * y / dw;
        sw = pic->w;
        sh = dh * pic->h / dw;
        ox = 0;
        oy = (y - dhy) / 2;
    } else {
        dwx = dw * x / dh;
        dhy = y;
        sw = dw * pic->w / dh;
        sh = pic->h;
        ox = (x - dwx) / 2;
        oy = 0;
    }
    void* temp = AllocFrame("TEMP RADAR PIC", pic->w, pic->h);
    SurfaceFromFrame(&surface, temp);
    DrawFrame(&surface, (short*)pic, 0, 0);
    pic->w = x;
    pic->h = y;
    SurfaceFromFrame(&surface, pic);
    FillSurface(&surface, 0);

    src.p[0].x = 0;
    src.p[0].y = 0;
    src.p[1].x = sw - 1;
    src.p[1].y = 0;
    src.p[2].x = sw - 1;
    src.p[2].y = sh - 1;
    src.p[3].x = 0;
    src.p[3].y = sh - 1;

    dst.p[0].x = ox;
    dst.p[0].y = oy;
    dst.p[1].x = ox + dwx;
    dst.p[1].y = oy;
    dst.p[2].x = ox + dwx;
    dst.p[2].y = oy + dhy;
    dst.p[3].x = ox;
    dst.p[3].y = oy + dhy;

    DrawFrameQuad(&surface, temp, &dst, &src);
    FUN_004d85a0(temp);
}
