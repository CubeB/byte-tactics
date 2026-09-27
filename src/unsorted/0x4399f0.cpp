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

// Partial, 88.7 percent (312 of 312 bytes, everything else matches).
// Only the y-coordinate setup differs, 5 instructions. The original computes
// yc = (p.z - view->cy) - (p.y >> 1): it loads p.x, p.z then p.y, loads
// view->cx and view->cy into ecx and ebx up front, then does sub edi,ecx
// (x - cx), sub ebp,ebx (z - cy), add edi,0x80, and only then sub ebp,edx.
// This compiler reassociates the identical source to (p.z - (p.y >> 1)) -
// view->cy: it loads p.y first, consumes it immediately, and reuses ecx for
// view->cy after ecx dies. The shift (sar edx,1) sits at the same offset in
// both, so only the sub operand pairing and the cy register differ.
//
// Tried, all reproduce the same reassociated bytes at 88.7 percent:
// hoisting view->cx and view->cy into int locals; splitting yc into
// p.z - view->cy then -= p.y >> 1 then += 0x20; wrapping each subtraction in
// its own inline View member (CenterX/CenterY taking Pos*); an int local
// holding the shift result, both alone and together with the cx/cy locals.
// tools/headers.py reports 72 header sets give byte-identical output, so no
// header choice reproduces the original ordering.
// The likely cause is compiler state from the original file's other contents:
// the preceding function is 0x439740 (674 bytes, unnamed). Defining that real
// function in this file above this one is the next thing to try, per the
// "when a match needs the function before it compiled first" pattern.

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
