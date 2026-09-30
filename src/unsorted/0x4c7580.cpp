// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 64.0%, 1144 of 1183 bytes. Rewritten in the 0x4c1000 style with the edge-walk
// temporaries (y0/y1/x/dxdy/source steps) shared across both walks and the second walk using
// (i+1)&3 (that one change was 51.7 -> 64.0). Frame and call census now match; the 39-byte gap
// is MSVC cross-jumping: the original keeps 3 of the 5 unlock+return tails inline (lea eax/edx)
// and shares only 2, ours merges 4 into one. Remaining diffs are stack-slot order (clip at
// 0x34 vs 0x3c, imin/imax at 0x44/0x48 vs 0x34/0x38), x/out swapped (0x24/0x28), xmin/y1
// swapped (0x10/0x14) and the y1>clip.top reload in the loops. Note: the earlier published
// 21.5% file with a 46-line header was replaced by a later 13.5% GPT-6 pass before this one.


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

    int y0, y1;
    Rec_004c7580 recs[800];
    int ymin = 999999;
    int ymax = -999999;
    int xmin = 999999;
    int xmax = -999999;
    int imin, imax;
    int clip[4];
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
        int xx = dst->p[i].x;
        if (xx > xmax)
            xmax = xx;
        if (xx < xmin)
            xmin = xx;
    }

    ((Class_004c6ae0*)surf)->FUN_004c6ae0(clip);
    if (xmax < clip[0]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (xmin > clip[2]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (ymax < clip[1]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (ymin > clip[3]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (ymin < clip[1])
        ymin = clip[1];
    if (ymax > clip[3])
        ymax = clip[3];
    if (ymax == ymin) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }

    out = recs;
    i = imin;
    for (;;) {
        j = i - 1;
        k = j;
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
                int dd = clip[1] - y0;
                x += dxdy * dd;
                tx += dtx * dd;
                ty += dty * dd;
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
        i = j;
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
                int dd = clip[1] - y0;
                x += dxdy * dd;
                tx += dtx * dd;
                ty += dty * dd;
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
        i = j;
        if (i == imax)
            break;
    }

    out = recs;
    for (i = ymin; i < ymax; i++) {
        if (out->right - out->left > 0)
            FUN_004c7310(i, (int*)out, surf, bmp);
        out++;
    }

    if (locked)
        FUN_004c5fa0(&local);
}
