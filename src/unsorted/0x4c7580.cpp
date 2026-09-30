// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// (started by deepseek-v4.1-flash / GPT-6.1-sol / GPT-6)
// deepseek-v4.1-flash, second pass (900s box): no score gain. This function is the
// no-z/no-uv sibling of 0x4c8760 (MATCH-adjacent, 72.5%): same min/max scan, same
// previous/next edge walk, same out[0]/out[2]/out[3] and out[1]/out[4]/out[5]
// span layout, same final row loop. 0x4c8760 proves the phrase for the edge loops
// is `int previous=index-1; int next=previous; if(next<0) next=3;` with `int x0`
// and `int y1` hoisted; rewrites v1..v4 using it (and `int recs[800][10]` indexed
// through an `int* out`) all scored 13.2%, against 13.5% here, so the phrase is not
// the blocker. The blocker is allocation: the original keeps maxx in ebx and minx
// in ebp while this file spills both (frame 0x7d94 vs 0x7d8c, two extra scalar
// slots). Unlike 0x4c8760 this variant calls FUN_004c5e70 before the scan, so the
// zero constant must survive that call in a callee-saved register and takes ebx
// here; finding the source shape that forces zero to ebp (freeing ebx for maxx)
// and drops both spilled min/max slots is the remaining work.
// PARTIAL 13.5%. What still differs, in the order it shows up in the diff:
//  1. Frame: original emits `mov eax, 0x7d8c; call __chkstk`, ours 0x7d94, so every
//     [esp+N] below the clip is +8 and every body offset too. The chkstk constant is
//     buffer_base + 32000 - 0x10 (MSVC5 subtracts the 4 pushed regs), and the args sit
//     at C+0x14+4*i; both hold for the original and for ours, so only the scalars before
//     the 0x7d00 byte span buffer are 8 bytes too big.
//  2. Original scalar layout (post-4-push esp): y1=0x10, minx/prev=0x14 (one slot, lives
//     are disjoint), locked=0x18, maxy=0x1c, miny=0x20, rec=0x24, x0=0x28, dtx=0x2c,
//     ddx=0x30, minyi=0x34, maxyi=0x38 => clip at 0x3c, tmp quad 0x4c, local surface
//     0x6c (0x30 bytes), spans buffer 0x9c. Ours has one extra 4 byte temp below the
//     clip (ours puts `locked` at 0x20, clip 0x40) and 4 bytes of padding between clip
//     and quad, hence +4/+8.
//  3. `maxx` must stay register only (ebx) as in the original: it is never stored to
//     memory, so the scan loop shape (pointer walk with the index in ecx) matters.
//  4. The walk loops: original keeps the loop index in memory ([esp+0x14]) storing the
//     *unclamped* i-1 / (i+1)&3 and re-clamping on reload at the bottom (do/while with
//     the test after the store). Ours keeps it in a register.
//  Parameter order is right: (surf, bmp, dst, src) with `surf = &local;` written back to
//  the arg slot at [esp+0x7da0]; esi=bmp, edi=dst, ebp=zero.
// GPT-6.1-sol refinement: tested moving default-src initialization after clipping,
// taking the parameter address through a pointer slot/reference, and moving tmp/clip
// declarations ahead of the scan. The checker held at 13.5% for every source form;
// moving initialization worsened it to 10.7%. Restored the highest-scoring baseline.
// deepseek-v4.1-flash, third pass: the single upstream cause of both the +8 frame and the
// ebx/ebp swap is that our `src` parameter is cached in ebp across the whole scan and
// clip call. The original never caches it: it reads src straight from its argument slot
// [esp+0x7dac] at each use (0x4c75e2 memory compare, 0x4c77c2 for the previous loop,
// 0x4c78d7 for the next loop). With ebp free the original keeps zero in ebp and maxx in
// ebx (maxx is never stored); ours keeps zero in ebx, spills maxx and minx, and uses
// ebx as the scan scratch. Tried and failed to evict src from ebp: a local `s = src`
// (coalesced), two per-loop locals s1/s2 (coalesced), aliasing bmp into a live local,
// a live `sf = surf`, and moving both edge loops into a `static inline BuildSpans`
// (it inlines, so src's live range is unchanged). The natural construct that makes a
// written parameter memory-resident instead of promoted is still the remaining work.

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

    int clip[4];
    ((Class_004c6ae0*)surf)->FUN_004c6ae0(clip);

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
