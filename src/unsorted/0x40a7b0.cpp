// Decompiled by Claude Opus 5.5. Names are provisional.
// Rebuilds the list of candidate cells: clears the vector at +0x4d, then
// walks every map cell and adds (x, y, feature value) for each cell whose
// feature (index below 0xfffb) has a non-zero value at +0xf0 and bit 1 of
// its flags byte set. 0x40a260 later sorts these by distance.
//
// Partial (81.4%): everything lines up, including which vector helpers
// /Ob2 leaves out of line (the third size() in the inlined insert, _Ucopy,
// _Ufill, _Destroy), except that in the reallocating branch of the inlined
// vector::insert the new buffer (_S) gets ebp and the _Ucopy result (_Q)
// gets ebx, where the original has _S in ebx and _Q in ebp (so the reloads
// of x and y after it come out in the other order). Not changed by: the
// Elem construction (named local, temporary, constructor with int or short
// parameters, member order, a helper returning Elem), declaration order or
// types of x, y and w, a pointer-walking inner loop, `continue` instead of
// nested ifs, a Feature getter, a vector reference, any header set, /Gz, or
// compiling 0x40a260 and 0x40a5d0 before it in one file. Elem's constructor
// must take the value as a float parameter: that gives the original's
// fld/fstp copy of the feature value (a plain field copy uses mov).
#include <vector>

struct Point16 {
    short x;
    short y;
};

struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    Elem_0040cc40(short x, short y, float k) { pos.x = x; pos.y = y; key = k; }
    Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key) {}
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

typedef std::vector<Elem_0040cc40> ElemVec;

#pragma pack(push, 1)
struct Feature {
    char unknown_0[0xf0];
    float value;                       // +0xf0
    char unknown_f4[0xff - 0xf4];
    unsigned char flags;               // +0xff
};

struct Cell {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    char unknown_a[0xd - 0xa];
};

struct Game {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    Feature* features;                 // +0x1426f
};

class Class_0040a7b0 {
public:
    char unknown_0[0x4d];
    ElemVec cells;                     // +0x4d
    void FUN_0040a7b0();
};
#pragma pack(pop)

extern Game* g_game;

Cell* __stdcall FUN_00481550(int x, int y);

// FUNCTION: 0x40a7b0
void Class_0040a7b0::FUN_0040a7b0()
{
    cells.clear();
    int w = g_game->width;
    for (int y = 0; y < g_game->height; y++) {
        Cell* row = FUN_00481550(0, y);
        for (int x = 0; x < w; x++) {
            if (row[x].feature < 0xfffb) {
                Feature* f = &g_game->features[row[x].feature];
                if (f->value != 0.0f && (f->flags & 2))
                    cells.push_back(Elem_0040cc40(x, y, f->value));
            }
        }
    }
}
