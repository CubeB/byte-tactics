// Decompiled by Claude Opus 5.5. Names are provisional.
// AI path search (the pathfinder object of 0x40df00, 0x40e630 and 0x40eb70):
// expands one neighbour of the node `from`, stepping in the parent cell's
// direction turned by `turn` (0..7). A new cell gets a node from the pool
// (growing it when full) and is pushed on the binary min-heap keyed on f; an
// open cell whose cost improves is updated and sifted up. `pending` means the
// heap's top is the node the caller (0x40df00) has just taken: a new node
// reuses it in place, and an improved node that displaces it frees it.
//
// Partial (82.3%). The pool/heap helpers are inline members, as the matched
// out-of-line copies at the end of the file show (0x40ef20 Remove, 0x40f000
// SiftUp, 0x40f060 SiftDown, 0x40f110 Grow, 0x40f1e0 Free). The inline
// budget reproduces the original's choices only with these helpers: SiftUp
// is inlined in Update's pending branch but called in its else branch and
// in Push, SiftDown is always called, Grow and Remove are inlined.
// Update() and the Steps() helper in case 1 are needed for that; a Cost()
// helper in case 1 gives the same pattern, one in case 0 breaks it.
// <windows.h> and <ddraw.h> fix the order of from->g and n->penalty in
// case 1's sum.
//
// Still differs: in case 0 the original stores the estimate h to its stack
// slot (shared with Estimate's `a`) straight after _allshr and uses eax for
// the cell pointer and the 16-bit steps arithmetic, loading `from` into edx
// on both sides of the steps if/else. Ours keeps h in eax until d.f, which
// shifts the registers of the whole case 0 block (flags in dl, from->g in
// ebx, turn reloaded into ecx). Rewriting h's declaration, the flag test,
// d.f's expression or the steps assignment did not change it.
#include <windows.h>
#include <ddraw.h>

struct Target_0040da70 {
    virtual int unused0(int, int);
    virtual int unused1(int, int);
    virtual int unused2(int, int);
    virtual int unused3(int, int);
    virtual int unused4(int, int);
    virtual int unused5(int, int);
    virtual int unused6(int, int);
    virtual int Func(int x, int y);
};

struct NodeData_0040da70 {
    short x;
    short y;
    int g;
    int f;
    short penalty;
    short steps;
};

struct Node_0040da70 {
    int index;
    NodeData_0040da70 data;
};

struct Cell_0040da70 {
    unsigned char flags;
    unsigned char dir;
    unsigned short node;
};

struct Grid_0040da70 {
    Cell_0040da70* cells;
    unsigned int width;
    unsigned int height;
    char unknown_c[4];
    unsigned int* dirty;

    int InBounds(unsigned int x, unsigned int y)
    {
        return x < width && y < height;
    }
};

class Class_0040f000 {
public:
    Node_0040da70* nodes;                 // +0x00
    Node_0040da70** items;                // +0x04
    int freeHead;                         // +0x08
    int used;                             // +0x0c
    int capacity;                         // +0x10
    int count;                            // +0x14
    int pending;                          // +0x18

    void FUN_0040f000(int i)
    {
        if (i != 0) {
            int parent = (i - 1) >> 1;
            Node_0040da70* node = items[i];
            Node_0040da70* p = items[parent];
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
    // Inline copy of FUN_0040f060 (not inlined here).
    void FUN_0040f060(int i)
    {
        Node_0040da70* node = items[i];
        while (true) {
            int left = i * 2 + 1;
            int right = i * 2 + 2;
            if (right < count) {
                Node_0040da70* l = items[left];
                Node_0040da70* r = items[right];
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
                Node_0040da70* l = items[left];
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
    // Inline copy of FUN_0040f110.
    void Grow(int n)
    {
        int cap = capacity;
        if (n < cap)
            n = cap + (cap >> 1) + 0x10;
        Node_0040da70* newNodes = (Node_0040da70*)operator new(n * 0x14);
        int i;
        for (i = 0; i < used; i++)
            *(newNodes + i) = nodes[i];
        operator delete(nodes);
        Node_0040da70** newItems = (Node_0040da70**)operator new(n * 4);
        for (i = 0; i < count; i++)
            newItems[i] = newNodes + (items[i] - nodes);
        operator delete(items);
        items = newItems;
        nodes = newNodes;
        capacity = n;
    }
    int Push(const NodeData_0040da70& d)
    {
        if (pending) {
            Node_0040da70* top = items[0];
            top->data = d;
            FUN_0040f060(0);
            pending = 0;
            return top - nodes;
        }
        if (count == capacity)
            Grow(-1);
        int k = count++;
        int idx = freeHead;
        if (idx == -1)
            idx = used++;
        else
            freeHead = nodes[idx].index;
        nodes[idx].data = d;
        nodes[idx].index = k;
        items[k] = &nodes[idx];
        FUN_0040f000(k);
        return idx;
    }
    void Update(int k)
    {
        if (pending) {
            Node_0040da70* top = items[0];
            FUN_0040f000(nodes[k].index);
            if (top->index != 0) {
                pending = 0;
                Remove(top - nodes);
            }
        } else {
            FUN_0040f000(nodes[k].index);
        }
    }
    // Inline copy of FUN_0040f1e0.
    void Free(int k)
    {
        nodes[k].index = freeHead;
        freeHead = k;
    }
    // Inline copy of FUN_0040ef20.
    void Remove(int k)
    {
        int idx = nodes[k].index;
        Free(k);
        count--;
        if (idx < count) {
            items[idx] = items[count];
            items[idx]->index = idx;
            FUN_0040f060(idx);
        }
    }
};

class Class_0040d7b0 {
public:
    unsigned int FUN_0040d7b0(int x, int y);
};

extern signed char DAT_004fd670[];
extern signed char DAT_004fd678[];

class Class_0040da70 : public Class_0040f000 {
public:
    Grid_0040da70 grid;                   // +0x1c
    char unknown_30[0x40 - 0x30];
    int threshold;                        // +0x40
    char unknown_44[0x50 - 0x44];
    int scale;                            // +0x50
    char unknown_54[0x60 - 0x54];
    Target_0040da70* target;              // +0x60
    char unknown_64[4];
    unsigned char turnCost[8];            // +0x68
    unsigned char stepCost[8];            // +0x70

    int Estimate(int x, int y)
    {
        int a = scale;
        return (int)(((__int64)target->Func(x, y) * (__int64)a) >> 0x10);
    }

    short Steps(NodeData_0040da70* from, int turn)
    {
        return turn ? 1 : from->steps + 1;
    }

    void FUN_0040da70(NodeData_0040da70* from, Cell_0040da70* fromCell, int turn);
};

// FUNCTION: 0x40da70
void Class_0040da70::FUN_0040da70(NodeData_0040da70* from, Cell_0040da70* fromCell, int turn)
{
    int dir = (fromCell->dir + turn) & 7;
    unsigned int x = from->x + DAT_004fd670[dir];
    unsigned int y = from->y + DAT_004fd678[dir];
    if (!grid.InBounds(x, y))
        return;
    unsigned int i = grid.width * y + x;
    Cell_0040da70* cell = &grid.cells[i];
    switch (cell->flags & 3) {
    case 0: {
        grid.dirty[i >> 8] |= 1 << ((i >> 3) & 0x1f);
        unsigned int r = ((Class_0040d7b0*)this)->FUN_0040d7b0(x, y);
        if (r < 1 && !(cell->flags & 8)) {
            cell->flags |= 3;
            return;
        }
        int h = Estimate(x, y);
        if (h <= threshold)
            cell->flags |= 5;
        else
            cell->flags |= 1;
        cell->dir = dir;
        NodeData_0040da70 d;
        d.x = x;
        d.y = y;
        if (turn)
            d.steps = 1;
        else
            d.steps = from->steps + 1;
        d.penalty = r > 1 ? 0 : 30;
        d.g = turnCost[turn] + stepCost[dir] + from->g + d.penalty;
        if (turn && from->steps < 5)
            d.g += 75;
        d.f = d.g + h;
        cell->node = Push(d);
        break;
    }
    case 1: {
        NodeData_0040da70* n = &nodes[cell->node].data;
        int g = turnCost[turn] + stepCost[dir] + from->g + n->penalty;
        if (turn && from->steps < 5)
            g += 75;
        if (g < n->g) {
            cell->dir = dir;
            n->f += g - n->g;
            n->g = g;
            n->steps = Steps(from, turn);
            Update(cell->node);
        }
        break;
    }
    }
}
