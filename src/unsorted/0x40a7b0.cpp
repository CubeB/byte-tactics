// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol,
// finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash (3857, 2026-10-01): hoisting `Feature* feats = g_game->features` regresses to 74.8% (649 bytes); the `(f->flags & 2) && f->value != 0.0f` swap gives 88.2%. Best stays 88.7%, same ebx/ebp swap.
// deepseek-v4.1-flash retry (3312, 2026-10-01): `feature <= 0xfffa` drops to 88.2%; a `(f->flags & 2) != 0` spelling and hoisted x/y declarations stay at 88.7% with the same ebx/ebp swap.
//
// deepseek-v4.1-flash retry: still 88.7% (645 vs 644 bytes), only the _S/_Q
// register pick in the reallocating push_back differs (original _S in ebx,
// _Q in ebp; ours swapped, so `lea ecx,[ebp+eax*8]` costs one extra byte).
// This pass re-confirmed it is not reachable from this file's source: adding
// <memory>, <xutility>, <algorithm>, <memory.h> or <windows.h> (all 88.7%),
// a named Elem local, field-wise construction, (short) casts, a while/++x
// loop, an ElemVec reference or pointer alias, a `float val` local, a 2-arg
// insert, both static-inline helpers (void AddCell and Elem MakeCell) all
// scored 88.7% or worse. The only remaining difference is the ebx/ebp swap.
// Rebuilds the list of candidate cells: clears the vector at +0x4d, then
// walks every map cell and adds (x, y, feature value) for each cell whose
// feature (index below 0xfffb) has a non-zero value at +0xf0 and bit 1 of
// its flags byte set. 0x40a260 later sorts these by distance.
//
// GPT-6.1-sol retry verification: the saved source still scores 88.7% (645/644 bytes).
// GPT-6.1-sol refinement: two variants scored 67.1% and 88.2%; restored the
// best and verified it again at 88.7%. The `_S`/`_Q` allocation swap remains.
// 2026-09-30 retry check: 88.7% (645/644 bytes); the saved source remains best.
// This pass again isolated the mismatch to `_S`/`_Q` register allocation.
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
// deepseek-v4.1 retry (88.7% again, 645 of 644 bytes, code unchanged): the swap
// survived about 100 more variants, including a file-scope padding sweep of
// K = 0..1200 in steps of 16 (all 88.7%, unlike 0x475bd0 where padding flips
// the pick), direct 3-arg insert (47.7%), 2-arg insert, while/for and ++x
// spellings, nested ifs, a reference temp, an array temp, copy-init and
// field-wise construction, ctor parameter reorder, flat fields instead of
// Point16, a spelled-out allocator, moving the <vector> include down, and
// preceding dummy functions that use the same template. The only remaining
// difference is still ebx (_S) vs ebp (_Q).
// deepseek-v4.1-flash retry 2 (88.7% again, 645 of 644 bytes, code unchanged):
// fresh levers all failed to flip the pick. A file-scope sweep of N unused
// `extern int dummyK;` declarations, N = 0..2400 step 4 (two batches plus the
// prior 16-step sweep), is 88.7% everywhere, so the pick is not reachable from
// the compiler's global symbol state. A Feature reference, both condition
// orders, a nested value/flags if, an `unsigned short fi = row[x].feature`
// local, `g_game->features + index` pointer arithmetic, a row pointer declared
// outside the loop, (short) casts, insert(end(), ...), a two-step temp and a
// field-wise temp are all 88.7% or worse. Making `flags` a byte, unsigned short
// (mask 0x200) or unsigned int (mask 0x2000000) changes the emitted test and
// drops to 69%. The single `lea ecx,[ebp+eax*8]` vs `[ebx+eax*8]` byte remains;
// see the notes above, this is the archived compiler-state wall.
// deepseek-v4.1-flash retry 3 (2026-10-01): the 0x408f30 "hand-written
// namespace std vector clone" lever is dead here too. A full clone of the
// class template (from <climits> + <memory> + <xutility>, no <vector>) gives
// byte-identical 88.7% (645/644) with the same ebx/ebp swap, so the pick is
// not header state. Also no effect: bool, int and char locals for the
// two-condition test, a flags-byte local (89.4%, 646 bytes, a different diff),
// address-taking every vector member through a derived class, prepending
// <iostream>, <string>, <map>, <list>, <algorithm>, <memory>, <set> or
// <deque>, and 1..64 dead __inline helper call sites (the /Ob2 budget does
// move the code, but never to a flipped pick).
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
