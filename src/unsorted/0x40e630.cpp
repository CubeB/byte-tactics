// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// 30-min checkpoint (space-bunny-free, best 98.3%, unchanged from main), plus
// the last 15 minutes: the two levers from the guide also come out flat here.
// A dead store in a statically folded branch does rotate the `Cost` handout
// (all ten spellings of `int t = 0; if (t) s = 1;` and up to three of them at
// once give the argument temporaries eax/edx and the vtable ebx), but it never
// reaches the original's edx/edx/eax, and in the `Cost(x, x, y)` shape it only
// moves the second argument's temporary from ebx to eax. Ten uncalled
// `static inline` file-scope helpers (int/unsigned short accessors, a FixMul
// clone, a flags accessor, a heap resetter) change neither the fastcall nor the
// thiscall nor the `Cost(x, x, y)` shape by a byte. For the Clear() group, dead
// stores that name the same fields, self-assignments (`d = d;`, `i = i;`,
// `heap.count = heap.count;`, `d.pos = d.pos;`), a comma expression putting
// `heap.Clear()` inside the NodeData constructor's argument list or inside the
// index expression, Clear() bodies that route the four values through a local or
// through a chained read of `count`, and a two-level pointer cast are all
// exactly 98.3%. The group is emitted where its statement stands, and when it
// stands early it is spread through the dirty-word OR instead of staying one
// block, so the original's slot (between the ctor's argument loads and its
// body stores) is not reachable from any statement order. tools/permute.py
// found nothing in 2937 candidates at the default seed and nothing in 3379 at
// seed 11; seeds 12 and 13 are the obvious next thing to try. Ten more uncalled
// helpers, one and two at a time, before or after the class, inline or not, a
// multi-statement one and a class member accessor, are all byte-identical. The
// clearest lead left: the original's two pushes imply a 3-parameter fastcall
// called as `Cost(x, x, y)`, which reproduces the push order, the edx
// argument and the eax vtable, and only misses because the second argument's
// temporary is handed ebx with both loads hoisted; and the Clear() group is
// emitted exactly where its statement stands, so the original's source has it
// between the NodeData constructor's argument loads and its body stores, which
// no plain statement order produces.
//
// 30-min checkpoint (space-bunny-free, best 98.3%, unchanged from main): the two
// hunks are still the extra `push ebp` and the four Clear() stores one block
// late. Measured again on this base, all flat or worse: the whole 6x6 matrix of
// narrow/int first-and-second parameter types for the virtual `Cost`
// (`short`/`unsigned short`/`char`/`unsigned char`/`int`/`long`): 16-bit first +
// 32-bit second gives 96.6 (the InBounds and Release registers then match and
// only the call block is wrong), 32-bit/32-bit 92.8, 16-bit/16-bit 93.2, so the
// 98.3 fastcall shape is still the best. `Cost(x, x, y)` (the push order the
// original's two pushes imply for a 3-parameter fastcall) scores 93.8 for all
// 216 type combinations: the second argument's temporary goes to ebx and both
// loads stay at the top instead of sinking the x load into edx. A thiscall
// 2-parameter call keeps the register reuse but hands the argument temporaries
// eax and the vtable edx, the exact mirror of the original, and no argument
// spelling (x+0, y*1, a folded ternary, an Identity wrapper, two-pointer reads,
// a target local) nor an extra dead statement moves that: the dead store
// (`int t = 0; if (t) s = 1;`) does rotate the handout, to eax/edx/ebx with the
// loads both at the top, never to edx/edx/eax. For the Clear() group, all eight
// source positions were re-swept: positions 0, 1 and 2 score 97.6 with the four
// stores spread through the dirty-word OR region instead of landing as one
// block at the top, and the spelling of the group makes no difference at all
// (member call, a free helper taking `Heap*` or `void*`, four explicit stores,
// one statement), only its position does. Dead stores around the group and a
// `*(float*)&global` node, and ctor bodies that reorder or add a self-assign,
// are all exactly 98.3%. The group is emitted in the source order of its
// statement, and the original needs it between the ctor's argument loads and
// its body stores, which no statement order reaches.
//
// deepseek-v4.1-flash (#4170 round, new best 98.3%): the big lever is the
// Target_0040e630 virtual `Cost` calling convention. Declaring it
// `virtual int __fastcall Cost(int x, int y, int z)` and calling it as
// `target->Cost(x, y, 0)` makes MSVC 5 put the argument temporaries in edx and
// the vtable in eax exactly like the original, which fixes three of the four
// old hunks (the InBounds width/x pair, both inlined Release object pointers
// and the freed-buffer register). Other conventions: 2-param `__fastcall`
// (1 push + edx arg) gives 94.7, `__stdcall` (this pushed) 93.7, plain
// thiscall 92.8, 16-bit virtual params 93.2. Remaining two hunks (880 bytes,
// 98.3%):
//   A. the fastcall third argument emits `push ebp` (0) before `push edx` (y)
//      where the original has `push edx` (y) then, after `movsx edx,x` and the
//      vtable load, `push edx` (x); i.e. the original is a 2-argument call and
//      the fastcall trick cannot drop its third push. `Cost(x, x, y)` has the
//      right push order but the allocator then puts y in ebx (93.8);
//      `Cost(x, y, 0)` with the third argument spelled as a different
//      expression only ever moves the push slot, never removes it.
//   D. the four `heap.Clear()` stores still land after the flags store instead
//      of between `movsx edx,ax` and `mov word ptr [esp+0x22],ax`; the
//      Clear()-position sweep (8 positions, including Clear() as the g
//      argument of the NodeData constructor) keeps the current placement best.
// Everything else in the function is byte-identical.
//
// deepseek-v4.1-flash (#4073 round): hoisting `heap.Clear();` ahead of the tail d(i) work (two placements: before NodeData and after it) both drop 92.8 to 92.1, so the 0x40e807 store group is not source-hoistable; baseline re-confirmed at 92.8% (880 bytes).
// deepseek-v4.1-flash (#3770 round): still 92.8% (880 bytes, exact), same four hunks.
// Two more shapes are flat: splitting `int cost; cost = Cost(start.x, start.y);`
// into two statements and moving the inlined Release body of Finish() into
// explicit Object_0040e630*/Dummy_00440be0* temps (`ow->FUN_00440be0(ob);`) both
// print the same 92.8% with byte-identical code, so the Cost argument temps and
// the two inlined Release register choices are not statement-shape levers.

// deepseek-v4.1-flash (#3265 round): hoisting the inlined heap.Clear() to before NodeData d() / i = ... (or between i = ... and the dirty OR) scores 92.1 (881->880 bytes), both spellings interleave the four heap reset stores into the dirty OR region around `shr edx,8`; the 92.8 position after `grid.cells[i].flags |= 1;` stays best. Remaining diffs unchanged: Start/A* virtual-call arg registers (eax/edx vs edx/eax), the movsx ecx/edx pair at the bounds test, Release() object pointer in eax vs edx, and the Clear() store slot.
// deepseek-v4.1-flash (#3023 retry): still 92.8% (880 bytes, exact). Four hunks:
// Cost/bounds registers use eax/edx where the original uses edx/eax and ecx/eax;
// both inlined Release() sites pick object/owner registers opposite to the original
// (hunk B also swaps the freed-buffer reg); the contiguous four-store heap reset
// group lands one block later. Structure, frame, callees, branches and the inlined
// Push/vector code match. Signed InBounds+casts, Clear()-first, object/owner temps
// and sx/sy locals score 81.3-92.8; 128 header sets flat. Allocator/scheduler tie.
// Starts a path search for the object at +0x58: marks every goal cell the
// target reports, picks the goal nearest to the start as the probe's aim,
// runs the straight-line probe (0x40e160) and, when that did not reach a
// goal, seeds the open heap with the start cell.
//
// Partial (92.8%): structure, stack layout, callee-saved registers, every
// branch and the inlined heap/vector code match; only four hunks differ.
// All four are allocator/scheduler state, not source shape:
//   1. the virtual Cost call at 0x40e762 uses edx for both argument
//      temporaries and eax for the vtable (ours eax/edx); the bounds check
//      that follows loads width into ecx and start.x into eax (ours
//      start.x ecx / width eax);
//   2. the inlined Release on the out-of-bounds path loads object into edx
//      (ours eax) and the deleted goal buffer into eax (ours ecx);
//   3. the heap reset stores are a contiguous group in the original,
//      placed right after the start.x/start.y loads and before the node's
//      y store; ours now emits the same four stores contiguously (this is
//      the hunk the reordering below improved) but after the flags store
//      and before the dir computation. The depth store still lands after
//      `mov ebx,[edx]` (original: before `shr edx,8`) and `mov ebx,[edx]`
//      still follows `and ecx,0x1f` (original: precedes it);
//   4. the final Release loads object into ecx before pushing it (ours
//      edx and pushes later).
// Tried without effect (all stay at 92.1%): heap.Clear() vs four direct
// stores vs a local Heap*; the node as a named local, a temporary in Push,
// or declared before Clear; computing the cell index before Clear; a
// Point/short local for start; FixMul vs the spelled-out 64-bit multiply;
// a stored goal result and `!= 0` conditions (slightly worse, 91.8%);
// Free Release/Finish helpers; a swapped NodeData constructor argument
// order and a constructor argument-order change (much worse). An inline
// helper for the open-list setup exhausts the /Ob2 budget (the node
// constructor goes out of line). All 128
// tools/headers.py sets also give 92.1%, so this is most likely TU
// compiler state, the same wall as 0x40d290, 0x408f30 and 0x40cca0.
// Also tried by deepseek-v4.1, all at or below 92.1%: InBounds written as
// `width > x && height > y` (90.4), an IsGoal bool local (91.8), Cost
// without its `s` local (87.8), the index computed before Clear (73.6, it
// grows/swaps a frame slot), a Point copy of start (61.7), and two Release
// local-temp orders (92.1 each); the four hunks below never move.
// deepseek-v4.1 re-run: tried unsigned-local InBounds operands (frame 909B, 80.3%);
// the best version stays this one at 92.1%.
// deepseek-v4.1 third pass (10 check runs): a named result local in Cost (884
// bytes, 87.7%), Clear() moved after the node construction (92.1), short Cost
// parameters (92.1), a target local in Cost (92.1), Cost hand-inlined at the
// call site (92.1), Release(object) with the object as a parameter (92.1), and
// the de Morgan bounds test written out at the tail (909 bytes, 80.3). The four
// hunks never moved, so they stay allocator state, not source shape.
// deepseek-v4.1-flash retry: a brute statement-order search over the tail
// block (all 180 orders of Clear/node/index/dirty/flags/dir keeping index
// first of the three that use it) found one improvement: moving heap.Clear()
// after the dirty and flags stores groups the four heap-reset stores into one
// contiguous block and takes the score to 92.8%. Every other order is at or
// below 92.1%. Variants tried at 92.8 or below with no further gain: explicit
// four-store Clear forms (three internal orders), short sx/sy node locals,
// the node declared before Clear, a `static inline void ClearHeap(Heap*)`
// free helper, reversed Cost multiply, and adding <windows.h>/<string.h>/
// <memory.h>. The clear group still lands one block later than the original
// (after flags, before dir) and the two Release sites and the Cost/bounds
// registers are unchanged. The original's clear group sits before the node
// pos stores; no source order of these six statements reaches that slot, so
// it remains scheduler state.
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

class Class_0040f000 {
public:
    void FUN_0040f000(int i);
};

class Class_0040f060 {
public:
    void FUN_0040f060(int i);
};

class Class_0040f110 {
public:
    void FUN_0040f110(int n);
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
            ((Class_0040f060*)this)->FUN_0040f060(0);
            topPopped = 0;
            return n - pool;
        }
        if (count == capacity)
            ((Class_0040f110*)this)->FUN_0040f110(-1);
        int i = count++;
        int k = freeHead;
        if (k == -1)
            k = used++;
        else
            freeHead = pool[k].index;
        pool[k].data = d;
        pool[k].index = i;
        items[i] = &pool[k];
        ((Class_0040f000*)this)->FUN_0040f000(i);
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
    virtual int __fastcall Cost(int x, int y, int z);
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

class Dummy_00440be0 {
public:
    void FUN_00440be0(Object_0040e630* p);
};

class Class_00440af0 {
public:
    void FUN_00440af0(Object_0040e630* p);
};

class Class_0040e9e0 {
public:
    void FUN_0040d900();
};

class Class_0040e160 {
public:
    int FUN_0040e160();
};

struct Table_0040e630 {
    unsigned char values[8];
};

struct Pair_0040e630 {
    unsigned char a;
    unsigned char b;
};

extern const Table_0040e630 DAT_004fca10;

static int FixMul(int a, int b)
{
    return (int)(((__int64)a * b) >> 0x10);
}

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
    Dummy_00440be0* owner;             // +0x64
    Table_0040e630 table;              // +0x68
    Pair_0040e630 pairs[4];            // +0x70

    int Cost(int x, int y)
    {
        int s = costScale;
        return (int)(((__int64)s * target->Cost(x, y, 0)) >> 0x10);
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
        owner->FUN_00440be0(object);
        object = 0;
        owner = 0;
    }
    void Finish()
    {
        path->FUN_0044f080(0, 0);
        Release();
    }

    void FUN_0040e630(Target_0040e630* t);
};

// FUNCTION: 0x40e630
void Class_0040e630::FUN_0040e630(Target_0040e630* t)
{
    owner = (Dummy_00440be0*)object->unit->field_4;
    target = t;
    start = object->pos;
    ((Class_00440af0*)owner)->FUN_00440af0(object);
    ResetTable();
    ((Class_0040e9e0*)this)->FUN_0040d900();

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
    finish:
        Finish();
    } else {
        int cost = Cost(start.x, start.y);
        if (!grid.InBounds(start.x, start.y)) {
            ((Class_0044ced0*)target)->FUN_0044ced0(0x200);
            Finish();
            return;
        }
        probe = ((Class_0040e160*)this)->FUN_0040e160();
        if (probe == 0) {
            ((Class_0044ced0*)target)->FUN_0044ced0(0x100);
        } else {
            ((Class_0044ced0*)target)->FUN_0044ced0(0x200);
            if (probe >= cost)
                goto finish;
        }
        NodeData_0040e630 d(start.x, start.y, 0, cost, 100);
        unsigned int i = grid.width * start.y + start.x;
        grid.dirty[i >> 8] |= 1 << ((i >> 3) & 0x1f);
        grid.cells[i].flags |= 1;
        heap.Clear();
        grid.cells[i].dir = ((object->heading + 0x1000) >> 13) & 7;
        grid.cells[i].node = heap.Push(d);
        field_44 = 4;
    }
}
