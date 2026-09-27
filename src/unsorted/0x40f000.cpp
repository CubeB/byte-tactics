// Decompiled by Opus. Names are provisional.
// Sift-up of a binary min-heap of node pointers; each node stores its heap index.

struct HeapNode_0040f000 {
    int index;                           // +0x0
    char unknown_4[8];
    int key;                             // +0xc
};

class Class_0040f000 {
public:
    int count;                           // +0x0
    HeapNode_0040f000** items;           // +0x4
    void FUN_0040f000(int i);
};

// FUNCTION: 0x40f000
void Class_0040f000::FUN_0040f000(int i)
{
    if (i != 0) {
        int parent = (i - 1) >> 1;
        HeapNode_0040f000* node = items[i];
        HeapNode_0040f000* p = items[parent];
        if (node->key < p->key) {
            items[i] = p;
            p->index = i;
            i = parent;
            while (i != 0) {
                parent = (i - 1) >> 1;
                p = items[parent];
                if (node->key >= p->key) break;
                items[i] = p;
                p->index = i;
                i = parent;
            }
            items[i] = node;
            node->index = i;
        }
    }
}
