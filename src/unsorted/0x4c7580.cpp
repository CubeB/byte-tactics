// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6. Names are provisional.
// PARTIAL 13.5%. Corrected both edge loops to test clip.top and skip nonpositive spans after clipping. Register allocation, frame and branch layout still differ.

#include <windows.h>

struct Point_004c7580 {
    int x;
    int y;
};

struct Quad_004c7580 {
    Point_004c7580 p[4];
};

struct Rec_004c7580 {
    int field_0;   // +0x00
    int field_4;   // +0x04
    int field_8;   // +0x08
    int field_c;   // +0x0c
    int field_10;  // +0x10
    int field_14;  // +0x14
    int pad[4];
};

struct Frame_004c7580 {
    unsigned short w;    // +0x00
    unsigned short h;    // +0x02
    char unknown_04[0xc];
    void* data;          // +0x10
};

struct Surface_004c5e70 {
    int data[12];
};

class Class_004c6ae0 {
public:
    char unknown_0[0x1c];
    int clip[4];             // +0x1c left, top, right, bottom

    void FUN_004c6ae0(int* out);
};

int __stdcall FUN_004c5e70(Surface_004c5e70* out);
int __stdcall FUN_004c5fa0(Surface_004c5e70* s);
void __stdcall FUN_004c7310(int y, int* rect, void* surf, void* info);

// FUNCTION: 0x4c7580
void __stdcall FUN_004c7580(void* surf, Frame_004c7580* bmp,
                            Quad_004c7580* dst, Quad_004c7580* src)
{
    if (bmp == 0)
        return;
    if (dst == 0)
        return;

    Surface_004c5e70 local;
    int locked;
    if (surf == 0) {
        if (FUN_004c5e70(&local) == 0)
            return;
        locked = 1;
        surf = &local;
    } else {
        locked = 0;
    }

    Quad_004c7580 tmp;
    if (src == 0) {
        src = &tmp;
        tmp.p[0].x = 0;
        tmp.p[0].y = 0;
        tmp.p[1].x = bmp->w - 1;
        tmp.p[1].y = 0;
        tmp.p[2].x = bmp->w - 1;
        tmp.p[2].y = bmp->h - 1;
        tmp.p[3].x = 0;
        tmp.p[3].y = bmp->h - 1;
    }

    int maxx = -999999;
    int maxy = -999999;
    int minx = 999999;
    int miny = 999999;
    int minyi;
    int maxyi;
    for (int k = 0; k < 4; k++) {
        int y = dst->p[k].y;
        if (y < miny) { miny = y; minyi = k; }
        if (y > maxy) { maxy = y; maxyi = k; }
        int x = dst->p[k].x;
        if (x > maxx) maxx = x;
        if (x < minx) minx = x;
    }

    Class_004c6ae0* srf = (Class_004c6ae0*)surf;
    int clip[4];
    srf->FUN_004c6ae0(clip);

    if (maxx < clip[0]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (minx > clip[2]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (maxy < clip[1]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (miny > clip[3]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (miny < clip[1]) { miny = clip[1]; }
    if (maxy > clip[3]) { maxy = clip[3]; }
    if (maxy == miny) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }

    Rec_004c7580 recs[800];
    Rec_004c7580* rec = recs;

    int i = minyi;
    do {
        int previous = i - 1;
        int prev = previous;
        if (prev < 0)
            prev = 3;
        int y0 = dst->p[i].y;
        int y1 = dst->p[prev].y;
        if (y1 > clip[1] && y0 < y1) {
            int dy = y1 - y0;
            int ddx = ((dst->p[prev].x - dst->p[i].x) << 16) / dy;
            int x0 = (dst->p[i].x << 16) + 0xffff;
            int tx = src->p[i].x << 16;
            int ty = src->p[i].y << 16;
            int dtx = ((src->p[prev].x - src->p[i].x) << 16) / dy;
            int dty = ((src->p[prev].y - src->p[i].y) << 16) / dy;
            if (y0 < clip[1]) {
                int dd = clip[1] - y0;
                x0 += ddx * dd;
                tx += dtx * dd;
                ty += dty * dd;
                y0 = clip[1];
            }
            if (y1 > clip[3])
                y1 = clip[3];
            int n = y1 - y0;
            if (n > 0) do {
                rec->field_0 = x0 >> 16;
                rec->field_8 = tx;
                rec->field_c = ty;
                x0 += ddx;
                tx += dtx;
                ty += dty;
                rec++;
                n--;
            } while (n != 0);
        }
        i = previous;
        if (i < 0)
            i = 3;
    } while (i != maxyi);

    rec = recs;
    i = minyi;
    do {
        int next = (i + 1) & 3;
        int y0 = dst->p[i].y;
        int y1 = dst->p[next].y;
        if (y1 > clip[1] && y0 < y1) {
            int dy = y1 - y0;
            int ddx = ((dst->p[next].x - dst->p[i].x) << 16) / dy;
            int x0 = (dst->p[i].x << 16) + 0xffff;
            int tx = src->p[i].x << 16;
            int ty = src->p[i].y << 16;
            int dtx = ((src->p[next].x - src->p[i].x) << 16) / dy;
            int dty = ((src->p[next].y - src->p[i].y) << 16) / dy;
            if (y0 < clip[1]) {
                int dd = clip[1] - y0;
                x0 += ddx * dd;
                tx += dtx * dd;
                ty += dty * dd;
                y0 = clip[1];
            }
            if (y1 > clip[3])
                y1 = clip[3];
            int n = y1 - y0;
            if (n > 0) do {
                rec->field_4 = x0 >> 16;
                rec->field_10 = tx;
                rec->field_14 = ty;
                x0 += ddx;
                tx += dtx;
                ty += dty;
                rec++;
                n--;
            } while (n != 0);
        }
        i = next;
    } while (i != maxyi);

    Rec_004c7580* r = recs;
    for (int y = miny; y < maxy; y++) {
        if (r->field_4 - r->field_0 > 0)
            FUN_004c7310(y, (int*)r, surf, bmp);
        r++;
    }

    if (locked)
        FUN_004c5fa0(&local);
}
