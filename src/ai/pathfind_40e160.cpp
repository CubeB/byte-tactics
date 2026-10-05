// Decompiled by Claude Opus 5.5. Names are provisional.
// Bug-style path probe: walks straight towards the goal (x first, then y)
// until blocked, then follows the obstacle's outline both ways at once until
// one side reaches the x-then-y line to the goal again. Returns the lowest
// cost seen, or 0 when the walk reaches a goal cell or a zero-cost cell.
//
// Matching notes: the return to the straight walk is a `goto`; an outer
// `for (;;)` loop gives `this` a lower register priority (ebp instead of
// esi). The two cost helpers differ only in how the 64-bit multiply is
// spelled, which decides whether MSVC commutes the `imul` at each site, and
// the headers are needed for the same reason (tools/headers.py).

#include <windows.h>
#include <stdio.h>

struct Cell_0040e160 {
    unsigned char flags;               // +0 bit 2 goal, bit 3 visited
    char dir;                          // +1
    short heapIndex;                   // +2
};

class Target_0040e160 {
public:
    virtual int unused0(int, int);
    virtual int unused1(int, int);
    virtual int unused2(int, int);
    virtual int unused3(int, int);
    virtual int unused4(int, int);
    virtual int unused5(int, int);
    virtual int unused6(int, int);
    virtual int Cost(int x, int y);
};

class Class_0040d7b0 {
public:
    unsigned int FUN_0040d7b0(int x, int y);
};

extern const char DAT_004fd670[];      // dx per direction
extern const char DAT_004fd678[];      // dy per direction

static int FixMul(int a, int b)
{
    return (int)(((__int64)a * b) >> 0x10);
}

class Class_0040e160 {
public:
    char unknown_0[0x1c];
    Cell_0040e160* cells;              // +0x1c
    unsigned int width;                // +0x20
    unsigned int height;               // +0x24
    char unknown_28[4];
    unsigned int* dirty;               // +0x2c
    short startX;                      // +0x30
    short startY;                      // +0x32
    char unknown_34[4];
    int goalX;                         // +0x38
    int goalY;                         // +0x3c
    char unknown_40[0xc];
    int steps;                         // +0x4c
    int costScale;                     // +0x50
    char unknown_54[0xc];
    Target_0040e160* target;           // +0x60

    int Cost(int x, int y)
    {
        int s = costScale;
        return FixMul(s, target->Cost(x, y));
    }
    int CostW(int x, int y)
    {
        int s = costScale;
        return (int)(((__int64)s * target->Cost(x, y)) >> 0x10);
    }
    unsigned int Passable(int x, int y)
    {
        return ((Class_0040d7b0*)this)->FUN_0040d7b0(x, y);
    }
    unsigned char Visit(unsigned int x, unsigned int y, char dir)
    {
        unsigned int i = width * y + x;
        dirty[i >> 8] |= 1 << ((i >> 3) & 0x1f);
        Cell_0040e160* c = &cells[i];
        c->dir = dir;
        return c->flags |= 8;
    }
    int OnLine(int x, int y, int nx, int ny)
    {
        int gdx = goalX - x;
        int gdy = goalY - y;
        int px = nx - x;
        int py = ny - y;
        if (gdx < 0) {
            gdx = -gdx;
            px = -px;
        }
        if (gdy < 0) {
            gdy = -gdy;
            py = -py;
        }
        if (py == 0 && px > 0 && px <= gdx)
            return 1;
        if (px == gdx && py > 0 && py <= gdy)
            return 1;
        return 0;
    }

    int Dir(int x, int y)
    {
        int d = goalX - x;
        if (d < 0)
            return 2;
        if (d > 0)
            return 6;
        return goalY - y > 0 ? 4 : 0;
    }
    int FUN_0040e160();
};

// FUNCTION: 0x40e160
int Class_0040e160::FUN_0040e160()
{
    int x = startX;
    int y = startY;
    int best = Cost(x, y);
    if (Passable(startX, startY) < 1)
        return best;
    char dir;
    int nx;
    int ny;
greedy:
        for (;;) {
            steps++;
            if (best == 0)
                return 0;
            dir = Dir(x, y);
            nx = DAT_004fd670[dir] + x;
            ny = DAT_004fd678[dir] + y;
            if (Passable(nx, ny) < 1)
                break;
            x = nx;
            y = ny;
            if (Visit(nx, ny, dir) & 4)
                return 0;
            int c = Cost(nx, ny);
            if (c < best)
                best = c;
        }

        int ax = x;
        int ay = y;
        int bx = x;
        int by = y;
        char dirA = (dir + 2) & 7;
        char dirB = dirA;
        int started = 0;
        for (;;) {
            char stop = (dirA - 3) & 7;
            dirA = (dirA - 2) & 7;
            steps++;
            nx = DAT_004fd670[dirA] + ax;
            ny = DAT_004fd678[dirA] + ay;
            while (Passable(nx, ny) < 1) {
                if (dirA == stop)
                    return best;
                dirA = (dirA + 1) & 7;
                nx = DAT_004fd670[dirA] + ax;
                ny = DAT_004fd678[dirA] + ay;
            }
            if (ax == bx && ay == by && dirA == dirB && started)
                return best;
            ax = nx;
            ay = ny;
            started = 1;
            if (Visit(ax, ay, dirA) & 4)
                return 0;
            if (OnLine(x, y, nx, ny)) {
                x = nx;
                y = ny;
                goto greedy;
            }
            int c = CostW(nx, ny);
            if (c < best)
                best = c;

            stop = (dirB + 3) & 7;
            dirB = (dirB + 2) & 7;
            nx = bx - DAT_004fd670[dirB];
            ny = by - DAT_004fd678[dirB];
            while (Passable(nx, ny) < 1) {
                if (dirB == stop)
                    return best;
                dirB = (dirB - 1) & 7;
                nx = bx - DAT_004fd670[dirB];
                ny = by - DAT_004fd678[dirB];
            }
            if (ax == bx && ay == by && dirA == dirB)
                return best;
            bx = nx;
            by = ny;
            if (Visit(bx, by, dirB) & 4)
                return 0;
            if (OnLine(x, y, nx, ny)) {
                x = nx;
                y = ny;
                goto greedy;
            }
            c = CostW(nx, ny);
            if (c < best)
                best = c;
        }
}
