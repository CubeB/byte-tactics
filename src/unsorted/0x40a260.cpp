// Decompiled by Claude Opus 5.5. Names are provisional.
// Picks a build cell near a world position: every candidate in `list` (a
// vector of cells with a score) within `range` cells goes into a max-heap
// keyed on minus the squared distance, then the cells are popped nearest
// first and tried with FUN_0047d2e0. The best-scoring cell (FUN_0047c770)
// wins; once one is found, candidates more than 160 beyond the first hit's
// squared distance stop the search.
//
// The bytes match with the real MSVC 5 std::vector, but three callees the
// compiler emits out of line carry placeholder names in data/symbols.csv:
// 0x40ca30 is vector<Elem_0040cc40>::capacity() (named
// Class_0040ca30::FUN_0040ca30), 0x40c5b0 is vector<Elem_0040cc40>::size()
// (Class_0040c5b0::FUN_0040c5b0), and 0x40a5b0 is Elem_0040cc40's copy
// constructor (Class_0040a5b0::FUN_0040a5b0). All three are called from the
// inlined vector::reserve and pop_heap here, where /Ob2's budget ran out.
//
// The heap functions 0x40d620 (_Make_heap) and 0x40d700 (_Pop_heap) end in
// `ret N`: the original file was compiled with __stdcall as the default, so
// they are declared explicitly __stdcall and called through what the inline
// std::make_heap and std::pop_heap would expand to. make_heap's
// `2 <= last - first` test is written out because the inline helper count
// decides where the /Ob2 budget runs out: with make_heap as one more inline
// helper, the second vector destructor calls _Destroy out of line.
#include <vector>

struct Point16 {
    short x;
    short y;
};

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key) {}
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

typedef std::vector<Elem_0040cc40> ElemVec;

#pragma pack(push, 1)
struct UnitType {
    char unknown_0[0x14a];
    Point16 origin;                    // +0x14a
};
#pragma pack(pop)

int __stdcall FUN_0047d2e0(UnitType* type, Point16 cell, int a, int b);
int FUN_0047c770(void);
void __stdcall FUN_0040d620(Elem_0040cc40* first, Elem_0040cc40* last, int*, Elem_0040cc40*);
void __stdcall FUN_0040d700(Elem_0040cc40* first, Elem_0040cc40* last, Elem_0040cc40* dest,
                            Elem_0040cc40 val, int*);

class Class_0040a7b0 {
public:
    bool FUN_0040a260(UnitType* type, Vec3* pos, ElemVec* list, int range, Point16* out);
};

static inline Point16 WorldToCell(Vec3 v, Point16 origin)
{
    Point16 c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

static inline int DistSq(const Point16& a, const Point16& b)
{
    int dy = a.y - b.y;
    int dx = a.x - b.x;
    return dx * dx + dy * dy;
}

// std::pop_heap(f, l) as the inline template expands it.
static inline void PopHeap(Elem_0040cc40* f, Elem_0040cc40* l)
{
    FUN_0040d700(f, l - 1, l - 1, Elem_0040cc40(*(l - 1)), (int*)0);
}

// The object is the same class as 0x40a7b0's (its caller 0x40bfe0 passes
// this + 0x4d, the vector 0x40a7b0 fills, as `list`); `this` is unused.
// FUNCTION: 0x40a260
bool Class_0040a7b0::FUN_0040a260(UnitType* type, Vec3* pos, ElemVec* list, int range, Point16* out)
{
    if (list->empty())
        return false;
    ElemVec heap;
    heap.reserve(list->size());
    int rangeSq = range * range;
    Point16 center = WorldToCell(*pos, type->origin);
    for (ElemVec::iterator p = list->begin(); p != list->end(); p++) {
        int d = DistSq(p->pos, center);
        if (d <= rangeSq) {
            heap.push_back(*p);
            heap.back().key = -d;
        }
    }
    if (2 <= heap.end() - heap.begin())
        FUN_0040d620(heap.begin(), heap.end(), (int*)0, (Elem_0040cc40*)0);
    int limit = -1;
    int best = 0;
    Point16 result;
    while (!heap.empty()) {
        Point16 cell = heap.front().pos;
        cell.x -= (type->origin.x - 3) / 2;
        cell.y -= (type->origin.y - 3) / 2;
        int d = DistSq(cell, center);
        if (limit >= 0 && d > limit + 160)
            break;
        if (FUN_0047d2e0(type, cell, 0, 0) && FUN_0047c770() > best) {
            result = cell;
            best = FUN_0047c770();
            if (limit == -1)
                limit = d;
        }
        PopHeap(heap.begin(), heap.end());
        heap.pop_back();
    }
    if (best == 0)
        return false;
    if (out)
        *out = result;
    return true;
}
