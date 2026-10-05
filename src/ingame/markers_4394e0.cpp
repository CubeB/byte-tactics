// Decompiled by DeepSeek V4.1 Flash, finished by Space Bunny Free, deepseek-v4.1-flash, GPT-6.1-sol, and Space Bunny Free. , edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Claude Opus 5.5. Names are provisional.
// MATCH. What it does: snapshots the position the caller passed in `out`,
// calls FUN_00439740 (which draws the order's icon and writes the new position
// to `out`), and when `flag` is set walks the line from the snapshot to the
// new position in 0x300000 steps, drawing frame `idx` of g_game->anims[21] at
// each step. idx starts at (frames since order->timestamp, clamped at 0) /
// max(1, anim->field_2c) % anim->count.
//
// The last piece (Claude Opus 5.5, #4994, 84.0 -> MATCH) was the order of the
// statements after the flag test: read the new position into its own local
// first (`Pos end = *out;`), then the clamped timestamp age, then the deltas
// from `end`, with `order->timestamp` read directly (no `int& ts` reference).
// The original's code after `test ebp, ebp` shows it: g_game is loaded, then
// all three of out's components, then `sub eax, [ebx+0x46]`, and only then
// the delta subtractions. With that order the old prologue rotation
// ({out, flag, order, start.x} in {esi, ebx, edi, ebp} instead of the
// original's {esi, ebp, ebx, edi}) and the dist/d.z choice for esi both fall
// into place; the same order with the `int& ts` reference scores 98.0, the
// age computed after the deltas 82.1, and between the copy and the deltas
// without `end` 79.8.
//
// Earlier findings that the match still depends on: the deltas in a Vec3 of
// plain ints whose Length() converts each component to its own double (the
// fild order, from the matched sibling 0x40beb0), the walk behind an inlined
// Trail::Run() (76.7 with the same loop written inline), and the three 16.16
// steps built in a helper returning a Pos (the products stay in memory).
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)

union Fixed_004394e0 {
    int value;                          // 16.16
    struct {
        unsigned short frac;
        short whole;
    };
};

struct Pos_004394e0 {
    Fixed_004394e0 x, y, z;
};

// The three deltas as plain ints, with the length helper of the matched
// sibling 0x40beb0. Two things there matter and both are visible here: the
// three components go through their own `double` locals, which is what keeps
// the three `fild`s in x, y, z order, and the difference is a struct the
// helper reads through memory, which is what keeps the three deltas in the
// original's esp+0x28..0x30 slots.
struct Vec3_004394e0 {
    int x, y, z;

    int Length() const
    {
        double fx = x;
        double fy = y;
        double fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
};

struct Node_004394e0 {                  // the unit order 0x439740 walks
    char unknown_0[0x46];
    int timestamp;                      // +0x46
};

struct View_004394e0 {
    char unknown_0[0x2c];
    int scroll_x;                       // +0x2c
    int scroll_y;                       // +0x30
};

struct Anim_004394e0 {
    unsigned short count;               // +0x0
    char unknown_2[0x2c - 2];
    unsigned short field_2c;            // +0x2c
};

struct Game {
    char unknown_0[0x1487f];
    Anim_004394e0* anims[22];           // +0x1487f, this function uses [21]
    char unknown_148d7[0x38a47 - 0x148d7];
    unsigned int frame;                 // +0x38a47
};

#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_00439740(void* surface, View_004394e0* view,
                            Node_004394e0* node, Pos_004394e0* out, int unused);
void __stdcall DrawFrame(void* surface, void* bmp, int x, int y);

// The three 16.16 steps of the interpolated position. The original keeps them
// in memory (the values are stored as each _allmul/_allshr pair finishes and
// the three adds with `start` happen only after all three), which is what
// building them in a helper returning a Pos reproduces.
static Pos_004394e0 offset_004394e0(int dx, int dy, int dz, int f)
{
    Pos_004394e0 d;
    d.x.value = (int)(((__int64)dx * f) >> 16);
    d.y.value = (int)(((__int64)dy * f) >> 16);
    d.z.value = (int)(((__int64)dz * f) >> 16);
    return d;
}

// The walk itself, as an inline method on a struct that holds the whole loop
// state. Per the guide, "an inlined function boundary changes the order MSVC
// evaluates things in and which registers it keeps values in"; moving the loop
// behind this boundary is what moves `order` out of edi and into ebx (the
// register the original loads it into).
struct Trail_004394e0 {
    Pos_004394e0 start;
    Vec3_004394e0 delta;
    int dist;
    int pos;
    int idx;
    void* surface;
    View_004394e0* view;
    Anim_004394e0* anim;

    void Run()
    {
        for (; pos < dist; pos += 0x300000) {
            int f = (int)(((__int64)pos << 16) / dist);
            Pos_004394e0 o = offset_004394e0(delta.x, delta.y, delta.z, f);
            Pos_004394e0 p;
            p.x.value = start.x.value + o.x.value;
            p.y.value = start.y.value + o.y.value;
            p.z.value = start.z.value + o.z.value;
            DrawFrame(surface, *(void**)((char*)anim + idx * 8 + 0x28),
                         p.x.whole - view->scroll_x + 0x80,
                         p.z.whole - (p.y.whole >> 1) - view->scroll_y + 0x20);
            idx = (idx + 1) % anim->count;
        }
    }
};

// FUNCTION: 0x4394e0
void __stdcall FUN_004394e0(void* surface, View_004394e0* view,
                            Node_004394e0* order, Pos_004394e0* out, int flag)
{
    Pos_004394e0 start = *out;
    FUN_00439740(surface, view, order, out, flag);
    if (flag == 0)
        return;

    Pos_004394e0 end = *out;
    int t = __max(g_game->frame - order->timestamp, 0);
    Vec3_004394e0 d;
    d.x = end.x.value - start.x.value;
    d.y = end.y.value - start.y.value;
    d.z = end.z.value - start.z.value;
    int dist = d.Length();
    if (dist < 0x10000)
        return;

    Anim_004394e0* anim = g_game->anims[21];
    int pos = (t % 30) * 0x300000 / 30;
    unsigned short len = anim->field_2c;
    int frames = len < 1 ? 1 : (int)len;
    unsigned int idx = (t / frames) % anim->count;

    Trail_004394e0 tr;
    tr.start = start;
    tr.delta = d;
    tr.dist = dist;
    tr.idx = idx;
    tr.surface = surface;
    tr.view = view;
    tr.pos = pos;
    tr.anim = anim;
    tr.Run();
}
