// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by claude-sonnet-5-5, finished by DeepSeek V4.1 Flash. Names are provisional.
#include <windows.h>

struct Point_004c7580 {
    int x;
    int y;
};

struct Quad_004c7580 {
    Point_004c7580 p[4];
};

struct Rec_004c7580 {
    int left;   // +0x00
    int right;  // +0x04
    int tx;     // +0x08
    int ty;     // +0x0c
    int trx;    // +0x10
    int try_;   // +0x14
    int pad[4];
};

struct Frame_004c7580 {
    unsigned short w;    // +0x00
    unsigned short h;    // +0x02
    char unknown_04[0xc];
    void* data;          // +0x10
};

struct Surface_004c5e70;
int __stdcall UnlockScreen(Surface_004c5e70* s);
struct Surface_004c5e70 {
    int data[12];
    void Unlock() { UnlockScreen(this); }
};

class Surface {
public:
    char unknown_0[0x1c];
    int clip[4];             // +0x1c left, top, right, bottom

    void GetClipRect(int* out);
};

int __stdcall LockScreen(Surface_004c5e70* out);
void __stdcall DrawQuadRow(int y, int* rect, void* surf, void* info);

// FUNCTION: 0x4c7580
void __stdcall DrawFrameQuad(void* surf, Frame_004c7580* bmp,
                            Quad_004c7580* dst, Quad_004c7580* src)
{
    if (bmp == 0)
        return;
    if (dst == 0)
        return;

    Surface_004c5e70 local;
    int locked;
    if (surf == 0) {
        int ok = LockScreen(&local);
        if (ok == 0)
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

    int y0, y1;
    Rec_004c7580 recs[800];
    int ymin = 999999;
    int ymax = -999999;
    int xmin = 999999;
    int xmax = -999999;
    int clip[4];
    int imin, imax;
    int i;
    Rec_004c7580* out;
    int j, k;
    int dxdy;
    int x, tx, ty, dtx, dty;

    for (i = 0; i < 4; i++) {
        int y = dst->p[i].y;
        if (y < ymin) {
            ymin = y;
            imin = i;
        }
        if (y > ymax) {
            ymax = y;
            imax = i;
        }
        if (dst->p[i].x > xmax)
            xmax = dst->p[i].x;
        int xx = dst->p[i].x;
        if (xx < xmin)
            xmin = xx;
    }

    ((Surface*)surf)->GetClipRect(clip);
    if (xmax < clip[0]) {
        if (locked) UnlockScreen(&local);
        return;
    }
    if (xmin > clip[2]) {
        if (locked) UnlockScreen(&local);
        return;
    }
    if (ymax < clip[1]) {
        if (locked) UnlockScreen(&local);
        return;
    }
    if (ymin > clip[3]) {
        if (locked) UnlockScreen(&local);
        return;
    }
    if (ymin < clip[1])
        ymin = clip[1];
    if (ymax > clip[3])
        ymax = clip[3];
    if (ymax == ymin) {
        // Method wrapper, not UnlockScreen(&local): keeps this tail from merging with check 2.
        if (locked) local.Unlock();
        return;
    }

    out = recs;
    i = imin;
    for (;;) {
        j = k = i - 1;
        if (k < 0)
            k = 3;
        y0 = dst->p[i].y;
        y1 = dst->p[k].y;
        if (y1 > clip[1] && y0 < y1) {
            int dy = y1 - y0;
            x = dst->p[i].x;
            dxdy = ((dst->p[k].x - x) << 16) / dy;
            x = (x << 16) + 0xffff;
            tx = src->p[i].x << 16;
            ty = src->p[i].y << 16;
            dtx = ((src->p[k].x << 16) - tx) / dy;
            dty = ((src->p[k].y << 16) - ty) / dy;
            if (y0 < clip[1]) {
                x += dxdy * (clip[1] - y0);
                tx += dtx * (clip[1] - y0);
                ty += dty * (clip[1] - y0);
                y0 = clip[1];
            }
            if (y1 > clip[3])
                y1 = clip[3];
            if (y0 < y1) {
                int n = y1 - y0;
                do {
                    out->left = x >> 16;
                    out->tx = tx;
                    out->ty = ty;
                    x += dxdy;
                    tx += dtx;
                    ty += dty;
                    out++;
                } while (--n);
            }
        }
        i = i - 1;
        if (i < 0)
            i = 3;
        if (i == imax)
            break;
    }

    out = recs;
    i = imin;
    for (;;) {
        j = (i + 1) & 3;
        k = j;
        y0 = dst->p[i].y;
        y1 = dst->p[k].y;
        if (y1 > clip[1] && y0 < y1) {
            int dy = y1 - y0;
            x = dst->p[i].x;
            dxdy = ((dst->p[k].x - x) << 16) / dy;
            x = (x << 16) + 0xffff;
            tx = src->p[i].x << 16;
            ty = src->p[i].y << 16;
            dtx = ((src->p[k].x << 16) - tx) / dy;
            dty = ((src->p[k].y << 16) - ty) / dy;
            if (y0 < clip[1]) {
                x += dxdy * (clip[1] - y0);
                tx += dtx * (clip[1] - y0);
                ty += dty * (clip[1] - y0);
                y0 = clip[1];
            }
            if (y1 > clip[3])
                y1 = clip[3];
            if (y0 < y1) {
                int n = y1 - y0;
                do {
                    out->right = x >> 16;
                    out->trx = tx;
                    out->try_ = ty;
                    x += dxdy;
                    tx += dtx;
                    ty += dty;
                    out++;
                } while (--n);
            }
        }
        i = (i + 1) & 3;
        if (i == imax)
            break;
    }

    out = recs;
    for (i = ymin; i < ymax; i++) {
        if (out->right - out->left > 0)
            DrawQuadRow(i, (int*)out, surf, bmp);
        out++;
    }

    if (locked) {
unlock:
        UnlockScreen(&local);
    }
}