// Decompiled by Claude Opus 5.5, Opus, DeepSeek V4.1 Flash, space-bunny-free, deepseek-v4.1-flash, Haiku and Sonnet. Names are provisional.
// The AI's path search (AISearch): an A* over the map's cells with an open
// heap of nodes (the OpenHeap base, whose sift-up and sift-down are defined
// here too, at their place in the original file), a per-tick scheduler that
// shares the search steps among the players, and the trace back from the goal.
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include <vector>
// Only for its symbol ids: ProbeStraightPath matches only in a window of the
// symbol count.
#include <assert.h>

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* param_1);

struct Point16 {
    short x;
    short y;
    int operator!=(const Point16& o) const
    {
        return x != o.x || y != o.y;
    }
};

struct NodeData {
    Point16 pos;                       // +0x0
    int g;                             // +0x4
    int f;                             // +0x8, the heap key
    short penalty;                     // +0xc
    short steps;                       // +0xe, steps since the last turn
    NodeData() {}
    NodeData(short x, short y, int g_, int f_, short steps_)
    {
        pos.x = x;
        pos.y = y;
        g = g_;
        f = f_;
        steps = steps_;
    }
};

struct Node {
    int index;                         // +0x0 heap slot, or next free node
    NodeData data;                     // +0x4
};

struct Cell {
    unsigned char flags;               // +0 bit 0 open, bit 2 goal, bit 3 visited
    unsigned char dir;                 // +1 the step taken into this cell
    unsigned short node;               // +2 index into the node pool
};

struct Grid {
    Cell* cells;                       // +0x0
    unsigned int width;                // +0x4
    unsigned int height;               // +0x8
    int count;                         // +0xc
    unsigned int* dirty;               // +0x10, one bit per 8 cells
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
    Cell* At(int x, int y)
    {
        return &cells[width * y + x];
    }
    void ClearBlock(int i, int last)
    {
        if (dirty[i]) {
            unsigned int bits = dirty[i];
            dirty[i] = 0;
            Cell* p = &cells[i * 256];
            while (bits) {
                if (bits & 1) {
                    int c = i << 8;
                    Cell* q = p;
                    for (int k = 8; k; k--) {
                        if (!last || c < count)
                            q->flags = 0;
                        c++;
                        q++;
                    }
                }
                bits >>= 1;
                p += 8;
            }
        }
    }
    // The dead `t` and its folded branch emit nothing, but the store keeps
    // MSVC from propagating the `cells` load into the pointer add, so the load
    // lands after the `dirty[i] = 0` store as in the original.
    void ClearLast(int i)
    {
        if (dirty[i]) {
            unsigned int bits = dirty[i];
            dirty[i] = 0;
            Cell* p = cells;
            int t = 0;
            if (t)
                bits = 0;
            p += i * 256;
            while (bits) {
                if (bits & 1) {
                    int c = i << 8;
                    Cell* q = p;
                    for (int k = 8; k; k--) {
                        if (c < count)
                            q->flags = 0;
                        c++;
                        q++;
                    }
                }
                bits >>= 1;
                p += 8;
            }
        }
    }
};

struct Table_0040d880 {
    unsigned char values[8];
};

struct Pair_0040d880 {
    unsigned char a;
    unsigned char b;
};

extern const Table_0040d880 DAT_004fca10;
extern signed char DAT_004fd670[];     // dx per direction
extern signed char DAT_004fd678[];     // dy per direction

class Planner_0040eb70;

#pragma pack(push, 1)
struct Owner_0040eb70 {
    Planner_0040eb70* planner;         // +0x0
    void* owner;                       // +0x4, the movement class
};

struct Unit {                          // 0x118 bytes
    Owner_0040eb70* owner;             // +0x0
    char unknown_4[0x66 - 0x4];
    unsigned short heading;            // +0x66
    char unknown_68[0x76 - 0x68];
    Point16 pos;                       // +0x76
    char unknown_7a[0xa6 - 0x7a];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

struct Player {                        // 0x14b bytes
    int active;                        // +0x0
    char unknown_4[0x67 - 0x4];
    Unit* unitsBegin;                  // +0x67
    Unit* unitsEnd;                    // +0x6b
    char unknown_6f[0x73 - 0x6f];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short field_2a3c;         // +0x2a3c, the number of players
    char unknown_2a3e[0x14233 - 0x2a3e];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    unsigned short* visibilityMask;    // +0x14273, one bit per player
    char unknown_14277[0x1434f - 0x14277];
    unsigned short field_1434f;        // +0x1434f
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_00511a38;
extern int DAT_00511a10[10];
extern int DAT_005119e8[10];

// The movement class the searched unit belongs to, and its pass map.
class MovementClass {
public:
    char unknown_0[4];
    short originX;                     // +0x4
    short originY;                     // +0x6
    char unknown_8[0x10 - 0x8];
    unsigned int width;                // +0x10
    unsigned int height;               // +0x14
    unsigned int* cells;               // +0x18, 16 2-bit cells per dword

    int InBounds(unsigned int x, unsigned int y)
    {
        return x < width && y < height;
    }
    int Get(int x, int y)
    {
        return (cells[width * (y >> 4) + x] >> ((y & 0xf) << 1)) & 3;
    }
    void RefreshUnitIfStale(Unit* p);
    void RefreshMovedUnits(Unit* p);
};

// What the search is looking for.
class Target {
public:
    virtual void unused0();
    virtual void unused1();
    virtual void unused2();
    virtual void unused3();
    virtual void unused4();
    virtual int IsGoal(int x, int y);
    virtual void GetGoals(std::vector<Point16>& goals);
    virtual int Cost(int x, int y);
};

class Class_0044ced0 {
public:
    void FUN_0044ced0(int param_1);
};

class Class_0044f010 {
public:
    char unknown_0[4];
    Target* target;                    // +0x4
    void FUN_0044f080(Point16* points, int count);
};

class Planner_0040eb70 {
public:
    virtual void unused0();
    virtual void unused1();
    virtual void unused2();
    virtual void unused3();
    virtual void unused4();
    virtual void unused5();
    virtual Class_0044f010* GetPath();
};

static int FixMul(int a, int b)
{
    return (int)(((__int64)a * b) >> 0x10);
}

// The open list: a binary min-heap of node pointers over a pool of nodes,
// each node holding its heap slot (or, when free, the next free node).
class OpenHeap {
public:
    Node* pool;                        // +0x0
    Node** items;                      // +0x4
    int freeHead;                      // +0x8
    int used;                          // +0xc
    int capacity;                      // +0x10
    int count;                         // +0x14
    int topPopped;                     // +0x18

    void SiftUp(int i);
    void SiftDown(int i);
    void Clear()
    {
        count = 0;
        freeHead = -1;
        used = 0;
        topPopped = 0;
    }
    int Size()
    {
        return count - topPopped;
    }
    void Free(int k)
    {
        pool[k].index = freeHead;
        freeHead = k;
    }
    void Remove(int k)
    {
        int idx = pool[k].index;
        Free(k);
        count--;
        if (idx < count) {
            items[idx] = items[count];
            items[idx]->index = idx;
            SiftDown(idx);
        }
    }
    void Pop()
    {
        int idx = items[0] - pool;
        Node* n = pool + idx;
        int pos = n->index;
        n->index = freeHead;
        freeHead = idx;
        int last = --count;
        if (pos < last) {
            items[pos] = items[last];
            items[pos]->index = pos;
            SiftDown(pos);
        }
    }
    void Update(int k)
    {
        if (topPopped) {
            Node* top = items[0];
            SiftUp(pool[k].index);
            if (top->index != 0) {
                topPopped = 0;
                Remove(top - pool);
            }
        } else {
            SiftUp(pool[k].index);
        }
    }
};

#pragma pack(push, 1)
class Pathfinder : public OpenHeap {
public:
    Grid grid;                         // +0x1c
    Point16 start;                     // +0x30
    Point16 found;                     // +0x34, the goal cell reached
    int goalX;                         // +0x38
    int goalY;                         // +0x3c
    int probe;                         // +0x40, the straight-line probe's cost
    int range;                         // +0x44, the turns tried per node
    int stepsPerTick;                  // +0x48
    int steps;                         // +0x4c
    int costScale;                     // +0x50
    int baseScale;                     // +0x54
    Unit* object;                      // +0x58
    Class_0044f010* path;              // +0x5c
    Target* target;                    // +0x60
    MovementClass* owner;              // +0x64
    union {
        Table_0040d880 table;          // +0x68
        unsigned char turnCost[8];
    };
    union {
        Pair_0040d880 pairs[4];        // +0x70
        unsigned char stepCost[8];
    };
    unsigned char player;              // +0x78
    Unit* cursor[10];                  // +0x79
    int budget[10];                    // +0xa1

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
    int CostFix(int x, int y)
    {
        int s = costScale;
        return FixMul(s, target->Cost(x, y));
    }
    unsigned int Passable(int x, int y)
    {
        return GetCellState(x, y);
    }
    unsigned char Visit(unsigned int x, unsigned int y, char dir)
    {
        unsigned int i = grid.width * y + x;
        grid.dirty[i >> 8] |= 1 << ((i >> 3) & 0x1f);
        Cell* c = &grid.cells[i];
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
    short Steps(NodeData* from, int turn)
    {
        return turn ? 1 : from->steps + 1;
    }
    int Push(const NodeData& d);

    int GetCellState(int x, int y);
    void InitCostTables();
    void MarkGoalCell(unsigned int x, unsigned int y);
    void ClearDirtyCells();
    __int64 Estimate(int param1, int param2);
    // In pathfind_40da70.cpp: it inlines the node pool's growth (0x40f110),
    // which StartSearch calls.
    void ExpandNeighbour(NodeData* from, Cell* fromCell, int turn);
    int ExpandBestNode();
    void TracePath();
    int ProbeStraightPath();
    void StartSearch(Target* t);
    void FUN_0040e9a0();
    void FUN_0040e9c0(int param_1);
    Pathfinder();
    ~Pathfinder();
    void RunSearches();
    // In pathfind_40ef20.cpp: it inlines the sift-down the rest call.
    void RemoveNode(int k);
    void GrowNodes(int n);
    void FreeNode(int index);
};
#pragma pack(pop)

inline int Pathfinder::Push(const NodeData& d)
{
    if (topPopped) {
        Node* n = items[0];
        n->data = d;
        SiftDown(0);
        topPopped = 0;
        return n - pool;
    }
    if (count == capacity)
        GrowNodes(-1);
    int i = count++;
    int k = freeHead;
    if (k == -1)
        k = used++;
    else
        freeHead = pool[k].index;
    pool[k].data = d;
    pool[k].index = i;
    items[i] = &pool[k];
    SiftUp(i);
    return k;
}

static int IsPlaying(unsigned char i)
{
    if (i < 10) {
        Player* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10)
            return 1;
    }
    return 0;
}

// The original calls every method here out of line; only the helpers defined
// in the classes above are inlined.
#pragma auto_inline(off)

// Looks up the 2-bit state of map cell (x, y): 0 when the cell is off the map
// or its visibility cell is off the game grid, 2 when the player's bit is not
// set in that visibility cell, else the cell's stored value.
// The map's bounds check and cell read are inline methods (the cell read
// re-reads the width after the bounds check), `g_game->width >> 1` is written
// twice rather than held in a local, and <stdlib.h> is needed: without it the
// first argument lands in eax instead of edx.
// FUNCTION: 0x40d7b0
int Pathfinder::GetCellState(int x, int y)
{
    if (!owner->InBounds(x, y))
        return 0;
    int cx = (x >> 1) + (owner->originX >> 2);
    int cy = (y >> 1) + (owner->originY >> 2);
    if (cx >= (g_game->width >> 1) || cy >= (g_game->height >> 1))
        return 0;
    if (!((1 << player) & g_game->visibilityMask[cy * (g_game->width >> 1) + cx]))
        return 2;
    return owner->Get(x, y);
}

// Resets a table of eight bytes {0, 40, 60, 80, 100, 80, 60, 40} copied from
// a constant, and four byte pairs to {16, 22}.
// FUNCTION: 0x40d880
void Pathfinder::InitCostTables()
{
    table = DAT_004fca10;
    pairs[0].a = pairs[1].a = pairs[2].a = pairs[3].a = 0x10;
    pairs[0].b = pairs[1].b = pairs[2].b = pairs[3].b = 0x16;
}

// Marks the cell at (x, y) with kind 4 and sets the dirty
// bit of the block of eight cells that holds it. The grid's methods are
// inline; calling them on the embedded member (rather than writing the body
// here) is what makes MSVC re-read the width after the bounds check.
// FUNCTION: 0x40d8b0
void Pathfinder::MarkGoalCell(unsigned int x, unsigned int y)
{
    if (grid.InBounds(x, y))
        grid.Set(x, y, 4);
}

// Clears the kind of every cell in the dirty blocks of the grid, and the dirty
// bits. The store of a dead local in Grid::ClearLast is needed: see there.
// FUNCTION: 0x40d900
void Pathfinder::ClearDirtyCells()
{
    int n = ((grid.count + 0xff) >> 8) - 1;
    int i;
    for (i = 0; i < n; i++)
        grid.ClearBlock(i, 0);
    grid.ClearLast(i);
}

// FUNCTION: 0x40da40
__int64 Pathfinder::Estimate(int param1, int param2)
{
    int a = costScale;
    return ((__int64)target->Cost(param1, param2) * (__int64)a) >> 0x10;
}

// Pops the best node from an open list and expands it on the navigation grid.
// Both copies of the pop are an inlined heap-remove helper.
// FUNCTION: 0x40df00
int Pathfinder::ExpandBestNode()
{
    if (topPopped != 0) {
        topPopped = 0;
        Pop();
    }
    Node* top = items[0];
    NodeData local = top->data;
    if (topPopped == 0)
        topPopped = 1;
    else
        Pop();
    int y = local.pos.y;
    int i = grid.width * y + local.pos.x;
    Cell* c = &grid.cells[i];
    if (c->flags & 4) {
        found = local.pos;
        return 1;
    }
    c->flags = 2;
    for (int d = -range; d <= range; d++)
        ExpandNeighbour(&local, c, d & 7);
    return 0;
}

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
// FUNCTION: 0x40e050
void Pathfinder::TracePath()
{
    Point16 cur = found;
    int dir = (char)grid.At(found.x, found.y)->dir;
    Point16 pts[64];
    Point16 out[64];
    pts[0] = cur;
    int n = 1;

    while (cur != start) {
        Cell* c = grid.At(cur.x, cur.y);
        if ((char)c->dir != dir) {
            dir = (char)c->dir;
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
        Point16 pt = pts[(n - 1 - i) & 0x3f];
        out[i].x = (pt.x * 2 + owner->originX) * 8;
        out[i].y = (pt.y * 2 + owner->originY) * 8;
    }
    path->FUN_0044f080(out, count);
}

// Bug-style path probe: walks straight towards the goal (x first, then y)
// until blocked, then follows the obstacle's outline both ways at once until
// one side reaches the x-then-y line to the goal again. Returns the lowest
// cost seen, or 0 when the walk reaches a goal cell or a zero-cost cell.
//
// Matching notes: the return to the straight walk is a `goto`; an outer
// `for (;;)` loop gives `this` a lower register priority (ebp instead of
// esi). The two cost helpers (CostFix and Cost) differ only in how the
// 64-bit multiply is spelled, which decides whether MSVC commutes the `imul`
// at each site.

// FUNCTION: 0x40e160
int Pathfinder::ProbeStraightPath()
{
    int x = start.x;
    int y = start.y;
    int best = CostFix(x, y);
    if (Passable(start.x, start.y) < 1)
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
            int c = CostFix(nx, ny);
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
            int c = Cost(nx, ny);
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
            c = Cost(nx, ny);
            if (c < best)
                best = c;
        }
}

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
// FUNCTION: 0x40e630
void Pathfinder::StartSearch(Target* t)
{
    owner = (MovementClass*)object->owner->owner;
    target = t;
    start = object->pos;
    owner->RefreshMovedUnits(object);
    ResetTable();
    ClearDirtyCells();

    std::vector<Point16> goals;
    target->GetGoals(goals);
    int bestDist = 0x7fffffff;
    for (Point16* p = goals.begin(); p != goals.end(); p++) {
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
    probe = ProbeStraightPath();
    if (probe == 0) {
        ((Class_0044ced0*)target)->FUN_0044ced0(0x100);
    } else {
        ((Class_0044ced0*)target)->FUN_0044ced0(0x200);
        if (probe >= cost) {
            Finish();
            return;
        }
    }
    Clear();
    NodeData d(start.x, start.y, 0, cost, 100);
    unsigned int i = grid.Index(start.x, start.y);
    grid.dirty[i >> 8] |= 1 << ((i >> 3) & 0x1f);
    grid.cells[i].flags |= 1;
    grid.cells[i].dir = ((object->heading + 0x1000) >> 13) & 7;
    grid.cells[i].node = Push(d);
    range = 4;
}

// FUNCTION: 0x40e9a0
void Pathfinder::FUN_0040e9a0()
{
    owner->RefreshUnitIfStale(object);
    object = 0;
    owner = 0;
}

// FUNCTION: 0x40e9c0
void Pathfinder::FUN_0040e9c0(int param_1)
{
    if (param_1 == (int)path) {
        object = 0;
        path = 0;
        target = 0;
        owner = 0;
    }
}

// AISearch constructor (201-byte object): seeds the per-player search cost
// table, sizes the map cell array from the game's map dimensions, and builds
// the "touched" bitmap.
//
// Two details the compiler forces and that look odd in C++:
//  - the grid's cells pointer is cleared with memset, not `grid.cells = 0`. With a plain
//    assignment MSVC folds the later `operator delete(grid.cells)` to a push of
//    the zero register (1 byte instead of mov+push), so the original source
//    must have gone through an opaque memory clear.
//  - The dirty bits are filled with 0xff for n - 1 bytes only, and its last dword is
//    then forced to zero and partially re-set by the loop below.
// FUNCTION: 0x40e9e0
Pathfinder::Pathfinder()
{
    pool = 0;
    items = 0;
    capacity = 0;
    count = 0;
    freeHead = -1;
    used = 0;
    topPopped = 0;
    grid.width = 0;
    grid.height = 0;
    grid.count = 0;
    memset(&grid.cells, 0, sizeof(grid.cells));

    int w, h;
    h = g_game->height;
    w = g_game->width;
    grid.width = w;
    grid.height = h;
    operator delete(grid.cells);
    grid.count = (h * w + 7) & ~7;
    grid.cells = grid.count ? new Cell[grid.count] : 0;

    unsigned int m = (grid.count + 0xff) >> 8;
    unsigned int n = m * 4;
    grid.dirty = (unsigned int*)FUN_004d83b0("AISearch touched mapentries", n);
    memset(grid.dirty, 0xff, n - 1);
    *(int*)((char*)grid.dirty + n - 4) = 0;

    for (unsigned int i = grid.count - 0x100; i < (unsigned int)grid.count; i++)
        grid.dirty[i >> 8] |= 1 << ((i >> 3) & 0x1f);

    ClearDirtyCells();

    object = 0;
    stepsPerTick = 0x535;
    steps = 0;
    player = 0;
    costScale = 0;
    baseScale = 0x18000;

    for (unsigned int k = 0; k < 10; k++) {
        DAT_005119e8[k] = baseScale;
        budget[k] = 0;
        cursor[k] = *(Unit**)((char*)g_game + 0x1a7f + 0x14b * (k + 1));
    }
}

// FUNCTION: 0x40eb30
Pathfinder::~Pathfinder()
{
    FUN_004d85a0(grid.dirty);
    operator delete(grid.cells);
    operator delete(pool);
    operator delete(items);
}

// Per-tick path search scheduler: shares this tick's step budget among the
// active players, then spends it on their path requests in turn, starting a
// new search (0x40e630) for the next unit of the current player or expanding
// the open heap of the running one until it reaches a goal or runs dry.
//
// Matching notes: the expansion loop is `while (1)` with the empty test
// inside, under an `else if (Size() != 0)` whose else is the failure path;
// that keeps the loop tested at the top and the failure block after it.
// Two oddities are kept as the original has them: the `r < 3` case and the
// final `else` both reset the scale to baseScale, and the second
// RemoveNode path can never run because the pop above already cleared
// topPopped.
// FUNCTION: 0x40eb70
void Pathfinder::RunSearches()
{
    unsigned short players = g_game->field_2a3c;
    if (players == 0)
        return;
    int share = stepsPerTick / players;
    int total = 0;
    if (++DAT_00511a38 >= 150) {
        DAT_00511a38 = 0;
        for (int i = 0; i < 10; i++) {
            int r = DAT_00511a10[i] / g_game->field_1434f;
            if (r < 1)
                DAT_005119e8[i] = baseScale * 6;
            else if (r < 2)
                DAT_005119e8[i] = baseScale * 3;
            else if (r < 3)
                DAT_005119e8[i] = baseScale;
            else
                DAT_005119e8[i] = baseScale;
            DAT_00511a10[i] = 0;
        }
    }
    for (int i = 0; i < 10; i++) {
        if (IsPlaying(i)) {
            budget[i] += share;
            total += budget[i];
        }
    }
    while (total > 0) {
        steps = 0;
        if (object == 0) {
            steps = 1;
            while (budget[player] <= 0) {
                if (++player >= 10)
                    player = 0;
            }
            Player* pl = &g_game->players[player];
            DAT_00511a10[player]++;
            Unit** c = &cursor[player];
            if (*c == pl->unitsEnd)
                *c = pl->unitsBegin;
            else
                (*c)++;
            Unit* u = cursor[player];
            if (u->field_a6 != 0 && u->owner != 0 && u->owner->owner != 0) {
                path = u->owner->planner->GetPath();
                if (path != 0) {
                    object = u;
                    steps += 100;
                    costScale = DAT_005119e8[player];
                    ((Pathfinder*)this)->StartSearch(path->target);
                }
            }
        } else if (Size() != 0) {
            while (1) {
                if (Size() == 0)
                    break;
                steps++;
                if (topPopped) {
                    topPopped = 0;
                    int k = items[0] - pool;
                    int idx = pool[k].index;
                    ((Pathfinder*)this)->FreeNode(k);
                    count--;
                    if (idx < count) {
                        items[idx] = items[count];
                        items[idx]->index = idx;
                        ((OpenHeap*)this)->SiftDown(idx);
                    }
                }
                NodeData d = items[0]->data;
                if (!topPopped)
                    topPopped = 1;
                else
                    ((Pathfinder*)this)->RemoveNode(items[0] - pool);
                Cell* cell = &grid.cells[grid.width * d.pos.y + d.pos.x];
                if (cell->flags & 4) {
                    found = d.pos;
                    ((Pathfinder*)this)->TracePath();
                    Release();
                    break;
                }
                cell->flags = 2;
                for (int k = -range; k <= range; k++)
                    ((Pathfinder*)this)->ExpandNeighbour(&d, cell, k & 7);
                range = 2;
                if (steps >= 100)
                    break;
            }
        } else {
            path->FUN_0044f080(0, 0);
            Release();
        }
        total -= steps;
        budget[player] -= steps;
    }
}

// Sift-up of a binary min-heap of node pointers; each node stores its heap index.
// FUNCTION: 0x40f000
void OpenHeap::SiftUp(int i)
{
    if (i != 0) {
        int parent = (i - 1) >> 1;
        Node* node = items[i];
        Node* p = items[parent];
        if (node->data.f < p->data.f) {
            items[i] = p;
            p->index = i;
            i = parent;
            while (i != 0) {
                parent = (i - 1) >> 1;
                p = items[parent];
                if (node->data.f >= p->data.f) break;
                items[i] = p;
                p->index = i;
                i = parent;
            }
            items[i] = node;
            node->index = i;
        }
    }
}

// Sift-down of a binary min-heap of node pointers; each node stores its heap index.
// FUNCTION: 0x40f060
void OpenHeap::SiftDown(int i)
{
    Node* node = items[i];
    while (true) {
        int left = i * 2 + 1;
        int right = i * 2 + 2;
        if (right < count) {
            Node* l = items[left];
            Node* r = items[right];
            if (r->data.f < l->data.f) {
                if (r->data.f >= node->data.f)
                    break;
                items[i] = r;
                r->index = i;
                i = right;
            } else {
                if (l->data.f >= node->data.f)
                    break;
                items[i] = l;
                l->index = i;
                i = left;
            }
        } else {
            if (left >= count)
                break;
            Node* l = items[left];
            if (l->data.f >= node->data.f)
                break;
            items[i] = l;
            items[left]->index = i;
            i = left;
        }
    }
    items[i] = node;
    node->index = i;
}

// Reallocates the element array (20-byte elements) and the parallel array of
// element pointers of the node pool, fixing up each pointer to the new block.
// FUNCTION: 0x40f110
void Pathfinder::GrowNodes(int param_1)
{
    int cap = capacity;
    if (param_1 < cap)
        param_1 = cap + (cap >> 1) + 0x10;
    Node* newe = (Node*)operator new(param_1 * 0x14);
    int i;
    for (i = 0; i < used; i++)
        *(newe + i) = pool[i];
    operator delete(pool);
    Node** newp = (Node**)operator new(param_1 * 4);
    for (i = 0; i < count; i++)
        newp[i] = newe + (items[i] - pool);
    operator delete(items);
    items = newp;
    pool = newe;
    capacity = param_1;
}

// Pushes entry `index` onto the free list threaded through the entries.
// FUNCTION: 0x40f1e0
void Pathfinder::FreeNode(int index)
{
    pool[index].index = freeHead;
    freeHead = index;
}

#pragma auto_inline(on)
