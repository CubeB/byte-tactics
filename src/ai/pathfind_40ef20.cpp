// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Removes node `k` from the AI search's open heap: puts the node back on the
// free list, shrinks the heap by one, moves the last heap element into the
// freed slot and sifts it down the binary min-heap keyed on data.f (+0xc).
// This is the same pop 0x40eb70 writes out with calls to 0x40f1e0 and
// 0x40f060; here both helpers are inlined.
//
// What decides the match is that the inlined sift-down reads the moved node
// from the heap itself (`node = items[i]`, as 0x40f060 does). Passing the
// node in as an argument lets MSVC fold `idx * 4` into the addresses instead
// of keeping it in edx, which is the prologue difference the #12 notes put
// down to compiler state.

struct Point_0040ef20 {
    short x;
    short y;
};

struct NodeData_0040ef20 {
    Point_0040ef20 pos;                 // +0x0
    int g;                              // +0x4
    int f;                              // +0x8, the heap key
    short unknown_c;                    // +0xc
    short depth;                        // +0xe
};

struct Node_0040ef20 {
    int index;                          // +0x0 heap slot, or next free node
    NodeData_0040ef20 data;             // +0x4
};

class Class_0040ef20 {
public:
    Node_0040ef20* pool;                // +0x0
    Node_0040ef20** items;              // +0x4, the heap
    int freeHead;                       // +0x8
    int used;                           // +0xc
    int capacity;                       // +0x10
    int count;                          // +0x14

    // Inline copy of FreeNode.
    void Free(int k)
    {
        pool[k].index = freeHead;
        freeHead = k;
    }
    // Inline copy of SiftDown.
    void SiftDown(int i)
    {
        Node_0040ef20* node = items[i];
        while (true) {
            int left = i * 2 + 1;
            int right = i * 2 + 2;
            if (right < count) {
                Node_0040ef20* l = items[left];
                Node_0040ef20* r = items[right];
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
                Node_0040ef20* l = items[left];
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

    void RemoveNode(int k);
};

// FUNCTION: 0x40ef20
void Class_0040ef20::RemoveNode(int k)
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
