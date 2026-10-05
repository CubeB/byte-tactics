// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Traces the path found by the AI search back from the goal cell it reached
// (+0x34) to the start (+0x30) by following each cell's direction byte, keeps
// the points where the direction changes (a ring of 64), and hands them to the
// path object in world coordinates, start first. Called from 0x40eb70.
//
// Three details decide the match: the loop test is an inline
// `Point::operator!=` (a plain `||` loads cur.y before cur.x), the cell is
// taken through an inline grid method returning a pointer, and its direction
// is read twice (`if (c->dir != dir) dir = c->dir;`), which keeps
// the `lea` of the cell address in the loop.

struct Point_0044f080 {
    short x;
    short y;
};

struct Point_0040e050 {
    short x;
    short y;

    int operator!=(const Point_0040e050& o) const
    {
        return x != o.x || y != o.y;
    }
};

struct Cell_0040e050 {
    unsigned char flags;        // +0x0 bit 2 goal
    char dir;                   // +0x1 step taken into this cell
    short node;                 // +0x2
};

struct Grid_0040e050 {
    Cell_0040e050* cells;       // +0x0
    int width;                  // +0x4
    int height;                 // +0x8
    int count;                  // +0xc
    unsigned int* dirty;        // +0x10

    Cell_0040e050* At(int x, int y)
    {
        return &cells[width * y + x];
    }
};

struct Map_0040e050 {
    char unknown_0[4];
    short originX;              // +0x4
    short originY;              // +0x6
};

class Class_0044f010 {
public:
    void FUN_0044f080(Point_0044f080* points, int n);
};

extern const char DAT_004fd670[];    // dx per direction
extern const char DAT_004fd678[];    // dy per direction

class Pathfinder {
public:
    char unknown_0[0x1c];
    Grid_0040e050 grid;         // +0x1c
    Point_0040e050 start;       // +0x30
    Point_0040e050 found;       // +0x34
    char unknown_38[0x5c - 0x38];
    Class_0044f010* path;       // +0x5c
    char unknown_60[4];
    Map_0040e050* map;          // +0x64 (owner in 0x40eb70, map in 0x40d7b0)

    void TracePath();
};

// FUNCTION: 0x40e050
void Pathfinder::TracePath()
{
    Point_0040e050 cur = found;
    int dir = grid.At(found.x, found.y)->dir;
    Point_0040e050 pts[64];
    Point_0044f080 out[64];
    pts[0] = cur;
    int n = 1;

    while (cur != start) {
        Cell_0040e050* c = grid.At(cur.x, cur.y);
        if (c->dir != dir) {
            dir = c->dir;
            pts[n & 0x3f] = cur;
            n++;
        }
        cur.x -= DAT_004fd670[dir];
        cur.y -= DAT_004fd678[dir];
    }
    pts[n & 0x3f] = start;
    n++;
    int count = n;
    if (count >= 0x40)
        count = 0x40;
    for (int i = 0; i < count; i++) {
        Point_0040e050 pt = pts[(n - 1 - i) & 0x3f];
        out[i].x = (pt.x * 2 + map->originX) * 8;
        out[i].y = (pt.y * 2 + map->originY) * 8;
    }
    path->FUN_0044f080(out, count);
}
