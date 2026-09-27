// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// BEST EFFORT, not a match (check.py ~81%). The while condition, the whole
// direction-change block, both direction-table lookups, the cur updates, the
// target store, the cap, the output loop and the call are byte-identical. Two
// differences remain, both MSVC 5 scheduling:
//   1. Prologue: the original hoists `movsx eax,[ecx+0x36]` (pos.y) before the
//      push pair and loads the whole `pos` dword into edx before `add esi,eax`
//      (then `map` into the scratch eax). Ours hoists the width load instead,
//      loads map into edx first and pos afterwards, and reads the cell byte
//      before the cur store. Same instructions, rotated.
//   2. Direction fetch in the loop: the original materializes the cell address
//      with `lea esi,[esi+ebp*4]` then `movsx esi,[esi+1]`; ours folds it into
//      `movsx esi,[esi+ebp*4+1]`. Reading a second cell field makes the
//      compiler emit the lea, so the original likely touched another field
//      that was then optimized out.
// Tried and rejected: explicit index/temp locals, pointer locals, 2D row
// pointers, char-typed temps, `#include <string.h>` (this one is required:
// without it the loop imul comes out as `movsx ebp,bx; imul ebp,[ecx+0x20]`).
#include <string.h>

// Reconstructs the path to the goal by following the per-cell direction field
// of the navigation grid, recording every change of direction, then converts
// the tile coordinates to world coordinates for the path object.

struct Point_0044f080 {
    short x;
    short y;
};

struct Point_0040e050 {
    short x;
    short y;
};

struct Cell_0040e050 {
    unsigned char flags;    // +0x0
    signed char direction;  // +0x1
    short unknown_2;        // +0x2
};

struct Origin_0040e050 {
    int unknown_0;          // +0x0
    short x;                // +0x4
    short y;                // +0x6
};

class Class_0044f010 {
public:
    void FUN_0044f080(Point_0044f080* src, int n);
};

extern signed char DAT_004fd670[];
extern signed char DAT_004fd678[];

class Class_0040e050 {
public:
    char unknown_0[0x1c];
    Cell_0040e050* map;         // +0x1c
    int width;                  // +0x20
    char unknown_24[0xc];
    Point_0040e050 target;      // +0x30
    Point_0040e050 pos;         // +0x34
    char unknown_38[0x24];
    Class_0044f010* path;       // +0x5c
    char unknown_60[4];
    Origin_0040e050* origin;    // +0x64

    void FUN_0040e050();
};

// FUNCTION: 0x40e050
void Class_0040e050::FUN_0040e050()
{
    int dir = map[width * pos.y + pos.x].direction;
    Point_0040e050 cur = pos;
    Point_0040e050 pts[64];
    Point_0044f080 out[64];
    pts[0] = cur;
    int n = 1;

    while (cur.x != target.x || cur.y != target.y) {
        int d = map[width * cur.y + cur.x].direction;
        if (d != dir) {
            dir = d;
            pts[n & 0x3f] = cur;
            n++;
        }
        cur.x -= DAT_004fd670[dir];
        cur.y -= DAT_004fd678[dir];
    }
    pts[n & 0x3f] = target;
    n++;
    int count = n;
    if (count >= 0x40)
        count = 0x40;
    for (int i = 0; i < count; i++) {
        Point_0040e050 pt = pts[(n - 1 - i) & 0x3f];
        out[i].x = (pt.x * 2 + origin->x) * 8;
        out[i].y = (pt.y * 2 + origin->y) * 8;
    }
    path->FUN_0044f080(out, count);
}
