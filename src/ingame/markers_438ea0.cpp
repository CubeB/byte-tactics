// Decompiled by space-bunny-free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, reworked by space-bunny-free, reworked by Claude Opus 5.5. Names are provisional.
// MATCH. Draws a ring of n + 1 line segments around a 16.16 map position,
// where n = radius * 2pi / 8, and the label text at the end of segment number
// index * 3 (or of the last segment when that one ended at 0, 0).
//
// What took it from 82.8% to MATCH (Claude Opus 5.5, #4991). The previous
// pass's file subtracted the loop counter instead of scroll_y in y1 and
// needed an int-returning `Higher` predicate and a y getter to place `pos` in
// edi and `rad` in ebx; all three are gone.
//  * Each ring point is `*pos - Offset(angle, rad)`, the Offset helper the
//    matched unit code uses ({-FUN_004b70ef, 0, -FUN_004b7123}, see 0x407e90
//    and 0x406300), with a Pos minus Vec3 operator. That alone gives the
//    original's x, z, y load order and edi/ebx/ebp for pos, rad and angle
//    (34.3% with the coordinates written out by hand, 63.3% with Offset).
//  * In Offset, the second call's result is named before it is negated
//    (`int z = ...; v.z = -z;`). That keeps angle in ebp through the drawing
//    block, so the correct `- sy` in y1 spills scroll_y into pos's dead
//    argument slot as the original does (63.3% to 98.8%). Writing
//    `v.z = -FUN_004b7123(...)` directly, as 0x407e90 does, gives 63.3%; naming
//    both results first gives 81.7%.
//  * The label index is compared as `i == index * 3`; MSVC hoists the
//    product into index's slot, as `index *= 3` did, but the uninitialised
//    x2 and y2 then take i's slot as their home on the n < 0 path (99.4%).
//  * y2 is assigned before x2, which orders that path's two reloads (MATCH).
//
// The fmul order still stands: `d = radius * DAT_004fd2b0`, then
// `n = (int)(d * DAT_004fd2b8)`. check.py masks both operands, so the product
// could be in the wrong order and still score, but the references would then
// point at the wrong constants.
#include <stdlib.h>

// A 16.16 fixed-point coordinate.
union Fixed_00438ea0 {
    int v;
    struct {
        unsigned short frac;
        short whole;
    } s;
};

struct Pos_00438ea0 {
    Fixed_00438ea0 x;                    // +0x0
    Fixed_00438ea0 y;                    // +0x4
    Fixed_00438ea0 z;                    // +0x8
};

struct Vec3_00438ea0 {
    int x, y, z;
};

struct View_00438ea0 {
    char unknown_0[0x2c];
    int scroll_x;                        // +0x2c
    int scroll_y;                        // +0x30
};

extern double DAT_004fd2b0;              // 6.28318530717958
extern double DAT_004fd2b8;              // 0.125

int __cdecl FUN_004b70ef(int angle, int radius);
int __cdecl FUN_004b7123(int angle, int radius);
int __stdcall GetGroundHeight(Pos_00438ea0* pos);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, int color);
void __stdcall FUN_004c14f0(void* surface, const char* text, int x, int y, int maxWidth);

// The offset of the point at this angle and distance from the centre.
static inline Vec3_00438ea0 Offset(int angle, int distance)
{
    Vec3_00438ea0 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    int z = FUN_004b7123(angle, distance);
    v.z = -z;
    return v;
}

static inline Pos_00438ea0 operator-(const Pos_00438ea0& p, const Vec3_00438ea0& v)
{
    Pos_00438ea0 r;
    r.x.v = p.x.v - v.x;
    r.y.v = p.y.v - v.y;
    r.z.v = p.z.v - v.z;
    return r;
}

// FUNCTION: 0x438ea0
void __stdcall FUN_00438ea0(void* surface, View_00438ea0* view, Pos_00438ea0* pos, int radius,
                            int color, const char* text, int index)
{
    int angle = 0;
    if (radius) {
        int lx = 0;
        int ly = 0;
        double d = radius * DAT_004fd2b0;
        int n = (int)(d * DAT_004fd2b8);
        int i = 0;
        // Uninitialised on purpose: when the ring has no segments the
        // original reloads both from their (never written) home slot.
        int x2;
        int y2;
        if (i <= n) {
            int step = 0x10000 / n;
            int rad = radius << 16;
            do {
                Pos_00438ea0 p1 = *pos - Offset(angle, rad);
                p1.y.s.whole = __max(pos->y.s.whole, GetGroundHeight(&p1));
                angle += step;
                Pos_00438ea0 p2 = *pos - Offset(angle, rad);
                p2.y.s.whole = __max(pos->y.s.whole, GetGroundHeight(&p2));
                int sx = view->scroll_x;
                int sy = view->scroll_y;
                y2 = p2.z.s.whole - (p2.y.s.whole >> 1) - sy + 0x20;
                x2 = p2.x.s.whole - sx + 0x80;
                FUN_004be950(surface, p1.x.s.whole - sx + 0x80,
                             p1.z.s.whole - (p1.y.s.whole >> 1) - sy + 0x20, x2, y2, color);
                if (i == index * 3) {
                    lx = x2;
                    ly = y2;
                }
                i++;
            } while (i <= n);
        }
        if (text) {
            if (lx == 0 && ly == 0) {
                lx = x2;
                ly = y2;
            }
            FUN_004c14f0(surface, text, lx, ly + 4, -1);
        }
    }
}
