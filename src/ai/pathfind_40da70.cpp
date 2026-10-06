// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6. Names are provisional.
// The rest of Pathfinder is in pathfinder.cpp; this stays apart because it
// inlines the node pool's growth (0x40f110), which StartSearch calls.
//
// Cache the heuristic scale before the virtual call, then use a separate fixed-point
// multiply helper. Keeping these helper boundaries reproduces the original inlining.
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

class OpenHeap {
public:
    Node_0040da70* nodes;                 // +0x00
    Node_0040da70** items;                // +0x04
    int freeHead;                         // +0x08
    int used;                             // +0x0c
    int capacity;                         // +0x10
    int count;                            // +0x14
    int pending;                          // +0x18

    void SiftUp(int i)
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
    // Inline copy of SiftDown (not inlined here).
    void SiftDown(int i)
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
    // Inline copy of GrowNodes.
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
        nodes = newNodes;
        items = newItems;
        capacity = n;
    }
    int Push(const NodeData_0040da70& d)
    {
        if (pending) {
            Node_0040da70* top = items[0];
            top->data = d;
            SiftDown(0);
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
        SiftUp(k);
        return idx;
    }
    void Update(int k)
    {
        if (pending) {
            Node_0040da70* top = items[0];
            SiftUp(nodes[k].index);
            if (top->index != 0) {
                pending = 0;
                Remove(top - nodes);
            }
        } else {
            SiftUp(nodes[k].index);
        }
    }
    // Inline copy of FreeNode.
    void Free(int k)
    {
        nodes[k].index = freeHead;
        freeHead = k;
    }
    // Inline copy of RemoveNode.
    void Remove(int k)
    {
        int idx = nodes[k].index;
        Free(k);
        count--;
        if (idx < count) {
            items[idx] = items[count];
            items[idx]->index = idx;
            SiftDown(idx);
        }
    }
};

extern signed char DAT_004fd670[];
extern signed char DAT_004fd678[];

class Pathfinder : public OpenHeap {
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

    static int Scale(int a, int b)
    {
        __int64 v = (__int64)a * b;
        return (int)(v >> 16);
    }
    int Estimate(int x, int y)
    {
        int factor = scale;
        return Scale(target->Func(x, y), factor);
    }

    short Steps(NodeData_0040da70* from, int turn)
    {
        return turn ? 1 : from->steps + 1;
    }

    void ExpandNeighbour(NodeData_0040da70* from, Cell_0040da70* fromCell, int turn);
    unsigned int GetCellState(int x, int y);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void MarkGoalCell(unsigned int, unsigned int);
};

// FUNCTION: 0x40da70
void Pathfinder::ExpandNeighbour(NodeData_0040da70* from, Cell_0040da70* fromCell, int turn)
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
        unsigned int r = ((Pathfinder*)this)->GetCellState(x, y);
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
