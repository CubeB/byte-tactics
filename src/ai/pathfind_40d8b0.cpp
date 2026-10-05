// Decompiled by Opus. Names are provisional.
// Marks the cell at (x, y) of an embedded grid with kind 4 and sets the dirty
// bit of the block of eight cells that holds it. The grid's methods are
// inline; calling them on the embedded member (rather than writing the body
// here) is what makes MSVC re-read the width after the bounds check.

struct Cell_0040d8b0 {
    unsigned char kind;
    char unknown_1[3];
};

struct Grid_0040d8b0 {
    Cell_0040d8b0* cells;              // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    char unknown_c[4];
    unsigned int* dirty;               // +0x10, one bit per 8 cells

    int InBounds(unsigned int x, unsigned int y)
    {
        return x < width && y < height;
    }
    void Set(unsigned int x, unsigned int y, unsigned char kind)
    {
        unsigned int i = width * y + x;
        cells[i].kind = kind;
        dirty[i >> 8] |= 1 << ((i >> 3) & 0x1f);
    }
};

class Pathfinder {
public:
    char unknown_0[0x1c];
    Grid_0040d8b0 grid;                // +0x1c

    void MarkGoalCell(unsigned int x, unsigned int y);
};

// FUNCTION: 0x40d8b0
void Pathfinder::MarkGoalCell(unsigned int x, unsigned int y)
{
    if (grid.InBounds(x, y))
        grid.Set(x, y, 4);
}
