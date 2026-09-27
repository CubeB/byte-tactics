// Decompiled by space-bunny-free. Names are provisional.
#pragma pack(push, 1)
struct Pos_4399f0 {
    unsigned short x_frac;           // +0x0
    short x;                         // +0x2
    unsigned short y_frac;           // +0x4
    short y;                         // +0x6
    unsigned short z_frac;           // +0x8
    short z;                         // +0xa
};
struct Unit_4399f0 {
    char pad_0[0x6a];
    Pos_4399f0 pos;                  // +0x6a
    char pad_76[0x92 - 0x76];
    char* field_92;                  // +0x92
};
struct Obj_4399f0 {
    char pad_0[0x16];
    Unit_4399f0* unit;               // +0x16
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
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, int color);

// Partial, 94.8 percent (5 instructions of 312 bytes differ, nothing else).
// Only the y-coordinate setup differs. The original evaluates
// yc = p.z - view->cy - (p.y >> 1) + 0x20 in source order: it loads p.x, p.z
// then p.y, then view->cx into ecx and view->cy into ebx, then does
// sub edi,ecx (x - cx), sub ebp,ebx (z - cy), add edi,0x80, sub ebp,edx.
// This compiler reassociates the same source to (p.z - (p.y >> 1)) - view->cy:
// it loads p.y first, spends it immediately, then reuses ecx for view->cy once
// cx dies. The sar edx,1, the lea and every other byte are already in place,
// so only the operand pairing of the two subs and the cy register differ.
// This is the guide's "operand order that nothing changes" case.
//
// Everything below was tried (each with check.py on a scratch copy) and all of
// it reproduces the same 94.8 percent, with or without the guide's helper,
// hoisting and header techniques:
// - source order: yc first, yc split over three statements, the 0x20 add before
//   the shift, parentheses that change the tree, one combined declaration;
// - helpers: static inline functions for the shift, for view->cx/cy, for the
//   whole expression, for both, taking the Pos by value or by reference, with
//   out-params or a 2-int struct, plus Pos/View member functions (ScreenX,
//   ScreenY, CenterX, CenterY);
// - locals: hoisting p.x/p.y/p.z, view->cx/cy, the shift result (int and
//   short), reading the Pos through a pointer or a reference;
// - types: (float)/(double) casts on the 0.89 multiply, short angle parameters
//   for FUN_004b70ef/7123, a typed struct for unit->field_92->height, an
//   int[2] view centre, an unpacked Pos;
// - context: a real function from the repo above this one (0x439cf0,
//   0x417bb0) changes nothing, and tools/headers.py reports all 128 header
//   sets give the same bytes.
// A second round, in the same 94.8 percent: interleaving the statements to
// reproduce the original's own order (sub x, sub z, add 0x80, sub y-half, add
// 0x20, one statement each), hoisting p.x/p.z/p.y as locals in x, z, y order,
// taking int& references to view->cx and view->cy, hoisting them as locals in
// both orders, and a short local for the half all give byte-identical code to
// what is here. Only making the y expression unsigned changes anything, and it
// drops to 60.8 percent, so the tree is signed and MSVC is reassociating a
// signed chain on its own rather than exploiting unsigned commutativity.
//
// The reassociation is decided by MSVC's scheduler, not by the source text:
// 0x417e00 (matched) has the identical y formula and keeps the source order,
// so the difference is the register pressure of keeping xc and yc alive across
// the loop. Untested: decompiling the preceding function 0x439740 (674 bytes)
// into this file, per the "when a match needs the function before it compiled
// first" pattern; the two small real functions tried above did not move it.

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
        FUN_004be950(surface, x1, y1, nx, ny, *(unsigned char*)(g_game + 0xdd7));
        x1 = nx;
        y1 = ny;
    }
    *out = p;
}
