// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol,
// finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash,
// finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5.
// Names are provisional.
// Rebuilds the list of candidate cells: clears the vector at +0x4d, then
// walks every map cell and adds (x, y, feature value) for each cell whose
// feature (index below 0xfffb) has a non-zero value at +0xf0 and bit 9 of
// its flags word set. 0x40a260 later sorts these by distance.
//
// The feature's flags are the 16-bit word at +0xfe, tested with 0x200, as in
// 0x422040 (the same test) and 0x423160. MSVC narrows the test to
// `test byte ptr [ecx + 0xff], 2` either way, but with the field declared as
// an `unsigned char` at +0xff (tested with 2) the reallocating push_back
// swaps its two callee-saved registers (the new buffer _S in ebp, the first
// _Ucopy result _Q in ebx), which held this file at 88.7% for many passes.
// The earlier passes are in git history.
//
// Elem's constructor takes the value as a float parameter: that gives the
// original's fld/fstp copy of the feature value (a plain field copy uses mov).
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
    char unknown_f4[0xfe - 0xf4];
    unsigned short flags;              // +0xfe
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

class PlayerAI {
public:
    char unknown_0[0x4d];
    ElemVec cells;                     // +0x4d
    void BuildFeatureCells();
};
#pragma pack(pop)

extern Game* g_game;

Cell* __stdcall GetMapCell(int x, int y);

// FUNCTION: 0x40a7b0
void PlayerAI::BuildFeatureCells()
{
    cells.clear();
    int w = g_game->width;
    for (int y = 0; y < g_game->height; y++) {
        Cell* row = GetMapCell(0, y);
        for (int x = 0; x < w; x++) {
            if (row[x].feature < 0xfffb) {
                Feature* f = &g_game->features[row[x].feature];
                if (f->value != 0.0f && (f->flags & 0x200))
                    cells.push_back(Elem_0040cc40(x, y, f->value));
            }
        }
    }
}
