// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Pops the best node from an open list and expands it on the navigation grid.
// Both copies of the pop are an inlined heap-remove helper.

struct NodeData_0040df00 {
    short x;                            // +0
    short y;                            // +2
    int field_4;                        // +4
    int key;                            // +8
    int field_c;                        // +0xc
};

struct Node_0040df00 {
    int heapPos;                        // +0
    NodeData_0040df00 data;             // +4
};

struct Cell_0040df00 {
    unsigned char kind;                 // +0
    char unknown_1;                     // +1
    short index;                        // +2
};

// The pathfinder's heap sift-down and neighbour expansion, named as their own
// files name them (all of these are one class; see docs/consolidation.md).
class OpenHeap {
public:
    void SiftDown(int pos);
};

class Class_0040da70 {
public:
    void ExpandNeighbour(short* p, Cell_0040df00* cell, int dir);
};

class Class_0040df00 {
public:
    Node_0040df00* nodes;               // +0x00
    Node_0040df00** heap;               // +0x04
    int field_8;                        // +0x08
    char unknown_c[0x14 - 0x0c];
    int count;                          // +0x14
    int field_18;                       // +0x18
    Cell_0040df00* cells;               // +0x1c
    int width;                          // +0x20
    int height;                         // +0x24
    char unknown_28[0x34 - 0x28];
    int field_34;                       // +0x34
    char unknown_38[0x44 - 0x38];
    int field_44;                       // +0x44

    void Pop()
    {
        int idx = this->heap[0] - this->nodes;
        Node_0040df00* n = this->nodes + idx;
        int pos = n->heapPos;
        n->heapPos = this->field_8;
        this->field_8 = idx;
        int last = --this->count;
        if (pos < last) {
            this->heap[pos] = this->heap[last];
            this->heap[pos]->heapPos = pos;
            ((OpenHeap*)this)->SiftDown(pos);
        }
    }

    int ExpandBestNode();
};

// FUNCTION: 0x40df00
int Class_0040df00::ExpandBestNode()
{
    if (this->field_18 != 0) {
        this->field_18 = 0;
        Pop();
    }
    Node_0040df00* top = this->heap[0];
    NodeData_0040df00 local = top->data;
    if (this->field_18 == 0)
        this->field_18 = 1;
    else
        Pop();
    int y = local.y;
    int i = this->width * y + local.x;
    Cell_0040df00* c = &this->cells[i];
    if (c->kind & 4) {
        this->field_34 = *(int*)&local;
        return 1;
    }
    c->kind = 2;
    for (int d = -this->field_44; d <= this->field_44; d++)
        ((Class_0040da70*)this)->ExpandNeighbour((short*)&local, c, d & 7);
    return 0;
}
