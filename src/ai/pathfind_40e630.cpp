// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by Claude Opus 5.5, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Starts a path search for the object at +0x58: marks every goal cell the
// target reports, picks the goal nearest to the start as the probe's aim,
// runs the straight-line probe (0x40e160) and, when that did not reach a
// goal, seeds the open heap with the start cell.
//
// What made this match (99.7% before, with a 3-argument `__fastcall` Cost as
// a stand-in): every early exit is its own `Finish(); return;`, not a
// `goto finish` into one shared block. MSVC tail-merges the copies into the
// one block at the end, so the bytes are the same, but the copy in the
// IsGoal branch is generated before the Cost call, and its two temporaries
// (ecx and edx, c2prio --rotation) move the eax/ecx/edx rotation two steps.
// With that, Cost is the plain thiscall the out-of-line copy 0x40da40 shows
// (arguments in edx, vtable in eax).
#include <vector>

struct Point_0040e630 {
    short x;
    short y;
};

struct Cell_0040e630 {
    unsigned char flags;               // +0 bit 0 open, bit 2 goal, bit 3 visited
    char dir;                          // +1
    short node;                        // +2 index into the node pool
};

struct NodeData_0040e630 {
    Point_0040e630 pos;                // +0x0
    int g;                             // +0x4
    int f;                             // +0x8
    short unknown_c;                   // +0xc
    short depth;                       // +0xe

    NodeData_0040e630(short x, short y, int g_, int f_, short depth_)
    {
        pos.x = x;
        pos.y = y;
        g = g_;
        f = f_;
        depth = depth_;
    }
};

struct Node_0040e630 {
    int index;                         // +0x0 heap slot, or next free node
    NodeData_0040e630 data;            // +0x4
};

class OpenHeap {
public:
    void SiftUp(int i);
    void SiftDown(int i);
};

class Class_0040f110 {
public:
    void GrowNodes(int n);
};

struct Heap_0040e630 {
    Node_0040e630* pool;               // +0x0
    Node_0040e630** items;             // +0x4
    int freeHead;                      // +0x8
    int used;                          // +0xc
    int capacity;                      // +0x10
    int count;                         // +0x14
    int topPopped;                     // +0x18

    void Clear()
    {
        count = 0;
        freeHead = -1;
        used = 0;
        topPopped = 0;
    }
    int Push(const NodeData_0040e630& d)
    {
        if (topPopped) {
            Node_0040e630* n = items[0];
            n->data = d;
            ((OpenHeap*)this)->SiftDown(0);
            topPopped = 0;
            return n - pool;
        }
        if (count == capacity)
            ((Class_0040f110*)this)->GrowNodes(-1);
        int i = count++;
        int k = freeHead;
        if (k == -1)
            k = used++;
        else
            freeHead = pool[k].index;
        pool[k].data = d;
        pool[k].index = i;
        items[i] = &pool[k];
        ((OpenHeap*)this)->SiftUp(i);
        return k;
    }
};

class Target_0040e630 {
public:
    virtual void unused0();
    virtual void unused1();
    virtual void unused2();
    virtual void unused3();
    virtual void unused4();
    virtual int IsGoal(int x, int y);
    virtual void GetGoals(std::vector<Point_0040e630>& goals);
    virtual int Cost(int x, int y);
};

class Class_0044ced0 {
public:
    void FUN_0044ced0(int param_1);
};

class Class_0044f010 {
public:
    void FUN_0044f080(Point_0040e630* points, int count);
};

struct Unit_0040e630 {
    char unknown_0[4];
    void* field_4;                     // +0x4
};

#pragma pack(push, 2)
struct Object_0040e630 {
    Unit_0040e630* unit;               // +0x0
    char unknown_4[0x66 - 0x4];
    unsigned short heading;            // +0x66
    char unknown_68[0x76 - 0x68];
    Point_0040e630 pos;                // +0x76
};
#pragma pack(pop)

class MovementClass {
public:
    void RefreshUnitIfStale(Object_0040e630* p);
};

class Class_00440af0 {
public:
    void RefreshMovedUnits(Object_0040e630* p);
};

class Pathfinder {
public:
    void ClearDirtyCells();
};

class Class_0040e160 {
public:
    int ProbeStraightPath();
};

struct Table_0040e630 {
    unsigned char values[8];
};

struct Pair_0040e630 {
    unsigned char a;
    unsigned char b;
};

extern const Table_0040e630 DAT_004fca10;

struct Grid_0040e630 {
    Cell_0040e630* cells;              // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    char unknown_c[4];
    unsigned int* dirty;               // +0x10

    int InBounds(unsigned int x, unsigned int y)
    {
        return x < width && y < height;
    }
    void Set(unsigned int x, unsigned int y, unsigned char kind)
    {
        unsigned int i = width * y + x;
        cells[i].flags = kind;
        dirty[i >> 8] |= 1 << ((i >> 3) & 0x1f);
    }
    unsigned int Index(unsigned int x, unsigned int y)
    {
        return width * y + x;
    }
};

class Class_0040e630 {
public:
    Heap_0040e630 heap;                // +0x0
    Grid_0040e630 grid;                // +0x1c
    Point_0040e630 start;              // +0x30
    char unknown_34[4];
    int goalX;                         // +0x38
    int goalY;                         // +0x3c
    int probe;                         // +0x40
    int field_44;                      // +0x44
    char unknown_48[4];
    int steps;                         // +0x4c
    int costScale;                     // +0x50
    char unknown_54[4];
    Object_0040e630* object;           // +0x58
    Class_0044f010* path;              // +0x5c
    Target_0040e630* target;           // +0x60
    MovementClass* owner;              // +0x64
    Table_0040e630 table;              // +0x68
    Pair_0040e630 pairs[4];            // +0x70

    int Cost(int x, int y)
    {
        int s = costScale;
        return (int)(((__int64)s * target->Cost(x, y)) >> 0x10);
    }
    void ResetTable()
    {
        table = DAT_004fca10;
        pairs[0].a = pairs[1].a = pairs[2].a = pairs[3].a = 0x10;
        pairs[0].b = pairs[1].b = pairs[2].b = pairs[3].b = 0x16;
    }
    void MarkGoal(unsigned int x, unsigned int y)
    {
        if (grid.InBounds(x, y))
            grid.Set(x, y, 4);
    }
    void Release()
    {
        owner->RefreshUnitIfStale(object);
        object = 0;
        owner = 0;
    }
    void Finish()
    {
        path->FUN_0044f080(0, 0);
        Release();
    }

    void StartSearch(Target_0040e630* t);
};

// FUNCTION: 0x40e630
void Class_0040e630::StartSearch(Target_0040e630* t)
{
    owner = (MovementClass*)object->unit->field_4;
    target = t;
    start = object->pos;
    ((Class_00440af0*)owner)->RefreshMovedUnits(object);
    ResetTable();
    ((Pathfinder*)this)->ClearDirtyCells();

    std::vector<Point_0040e630> goals;
    target->GetGoals(goals);
    int bestDist = 0x7fffffff;
    for (Point_0040e630* p = goals.begin(); p != goals.end(); p++) {
        MarkGoal(p->x, p->y);
        int dy = start.y - p->y;
        int dx = start.x - p->x;
        int d = dx * dx + dy * dy;
        if (d < bestDist) {
            goalX = p->x;
            bestDist = d;
            goalY = p->y;
        }
    }

    if (target->IsGoal(start.x, start.y)) {
        ((Class_0044ced0*)target)->FUN_0044ced0(0x100);
        Finish();
        return;
    }
    int cost = Cost(start.x, start.y);
    if (!grid.InBounds(start.x, start.y)) {
        ((Class_0044ced0*)target)->FUN_0044ced0(0x200);
        Finish();
        return;
    }
    probe = ((Class_0040e160*)this)->ProbeStraightPath();
    if (probe == 0) {
        ((Class_0044ced0*)target)->FUN_0044ced0(0x100);
    } else {
        ((Class_0044ced0*)target)->FUN_0044ced0(0x200);
        if (probe >= cost) {
            Finish();
            return;
        }
    }
    heap.Clear();
    NodeData_0040e630 d(start.x, start.y, 0, cost, 100);
    unsigned int i = grid.Index(start.x, start.y);
    grid.dirty[i >> 8] |= 1 << ((i >> 3) & 0x1f);
    grid.cells[i].flags |= 1;
    grid.cells[i].dir = ((object->heading + 0x1000) >> 13) & 7;
    grid.cells[i].node = heap.Push(d);
    field_44 = 4;
}
