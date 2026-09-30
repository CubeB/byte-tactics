// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol,
// finished by space-bunny-free. Names are provisional.
// Rebuilds the list of candidate cells: clears the vector at +0x4d, then
// walks every map cell and adds (x, y, feature value) for each cell whose
// feature (index below 0xfffb) has a non-zero value at +0xf0 and bit 1 of
// its flags byte set. 0x40a260 later sorts these by distance.
//
// GPT-6.1-sol retry verification: the saved source still scores 88.7% (645/644 bytes).
// The _S/_Q register swap remains the only difference; the 128-header sweep and
// prior source-form variants recorded below did not improve it.
// Partial (88.7%, 645 bytes against the original's 644): only one allocator
// decision differs. In the reallocating branch of the inlined push_back
// (vector::insert(_P, 1, _X)) the original keeps the freshly allocated
// buffer (_S) in ebx and the first _Ucopy result (_Q) in ebp; ours swaps
// them. The swap costs the extra byte because _End = _S + _N compiles to
// `lea ecx, [ebx + eax*8]` (3 bytes) in the original but
// `lea ecx, [ebp + eax*8]` (4 bytes: ebp as a base needs a disp8) in ours,
// which shifts every later address by one. Every instruction before
// `mov ebx,eax` (_S, 0x40a8e2) is byte-identical, so the choice is decided
// by the whole function's IR, not by the source order here.
//
// Not changed by: push_back of a temporary or of a named local, an explicit
// insert(end(), 1, X) or insert(end(), X), a static inline helper returning
// the Elem by value, casting the coordinates to short, a float local for the
// value, any of the 128 header sets (tools/headers.py). This is the same
// wall the guide records for out-of-line vector::insert copies (0x408f30 is
// an identical 88.7%): treat it as compiler state and move on. Elem's
// constructor must take the value as a float parameter: that gives the
// original's fld/fstp copy of the feature value (a plain field copy uses mov).
//
// space-bunny-free retry (88.7% again, 645 of 644 bytes, nothing changed): the
// only difference is which of the two dead callee-saved registers takes the
// fresh buffer (_S) and which takes the first _Ucopy result (_Q) in the
// reallocating push_back. Both are free there (the x counter is spilled at
// [esp+0x10], the y counter at [esp+0x18]), and the loop tails, the spills and
// both stack slots are byte-identical, so it is one allocator decision. Not
// reached by tools/headers.py (128 sets, all 88.7%), nor by defining the real
// neighbour 0x40a5d0 of the same class and translation unit in the same file
// ahead of this one (scored in build/scratch/0x40a7b0/v1.cpp: still 88.7%,
// same single hunk). This is the same wall as the out-of-line inserts 0x408f30,
// 0x40d020 and 0x40d290 the guide records: treat it as compiler state.
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
