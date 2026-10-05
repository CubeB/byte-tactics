// Decompiled by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// The headers are not used by the code: they set the compiler state that
// decides the order of the y-coordinate sum (see the notes below).
#include <windows.h>
#include <memory.h>

#pragma pack(push, 1)
struct Pos_4399f0 {
    unsigned short x_frac;           // +0x0
    short x;                         // +0x2
    unsigned short y_frac;           // +0x4
    short y;                         // +0x6
    unsigned short z_frac;           // +0x8
    short z;                         // +0xa
};
struct Unit {
    char pad_0[0x6a];
    Pos_4399f0 pos;                  // +0x6a
    char pad_76[0x92 - 0x76];
    char* field_92;                  // +0x92
};
struct Obj_4399f0 {
    char pad_0[0x16];
    Unit* unit;                      // +0x16
    char pad_1a[0x22 - 0x1a];
    Pos_4399f0 pos;                  // +0x22
};
struct View_4399f0 {
    char pad_0[0x2c];
    int cx;                          // +0x2c
    int cy;                          // +0x30
};
#pragma pack(pop)

extern char* g_game;
int __cdecl FUN_004b70ef(int angle, int distance);
int __cdecl FUN_004b7123(int angle, int distance);
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, int color);

// Draws an ellipse (radius `height`, 0.89 of it vertically) of 16 segments
// around an object's screen position, then copies the position to `out`.
//
// The two earlier attempts (#31, #158) stopped at 94.8 percent with the same
// source: without these headers MSVC reassociates
// `p.z - view->cy - (p.y >> 1)` into `(p.z - (p.y >> 1)) - view->cy`, loads
// p.y first and reuses ecx for view->cy. That is compiler state, not source
// shape: with N unused `extern int` declarations in front of this source and
// no headers, it matches for N = 43 to 298, and the pattern repeats every 512
// declarations (half of every period matches). `<windows.h>` + `<memory.h>`
// puts N = 0 near the middle of a matching window (it still matches with 109
// fewer or 146 more declarations). `<stdlib.h>` + `<math.h>` + `<memory.h>`
// is the most centred set (-134 to +121); `<windows.h>` alone also matches
// (-82 to +173).
// FUNCTION: 0x4399f0
void __stdcall FUN_004399f0(void* surface, View_4399f0* view, Obj_4399f0* obj,
                            Pos_4399f0* out, int unused)
{
    Pos_4399f0 p;
    int height;
    if (obj->unit != 0) {
        p = obj->unit->pos;
        height = *(short*)(obj->unit->field_92 + 0x178);
    } else {
        p = obj->pos;
        height = 0x20;
    }
    int ry = (int)(height * 0.89);
    int xc = p.x - view->cx + 0x80;
    int yc = p.z - view->cy - (p.y >> 1) + 0x20;
    int x1 = xc + height;
    int y1 = yc;
    for (int angle = 0x1000; angle <= 0x10000; angle += 0x1000) {
        int nx = FUN_004b7123(angle, height) + xc;
        int ny = FUN_004b70ef(angle, ry) + yc;
        DrawLine(surface, x1, y1, nx, ny, *(unsigned char*)(g_game + 0xdd7));
        x1 = nx;
        y1 = ny;
    }
    *out = p;
}
