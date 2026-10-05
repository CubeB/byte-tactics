// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Sift-down of a binary min-heap of node pointers; each node stores its heap index.

struct HeapNode_0040f000 {
    int index;                           // +0x0
    char unknown_4[8];
    int key;                             // +0xc
};

class Class_0040f000 {
public:
    char unknown_0[4];
    HeapNode_0040f000** items;           // +0x4
    char unknown_8[0xc];
    int count;                           // +0x14

    void FUN_0040f060(int i);
};

// FUNCTION: 0x40f060
void Class_0040f000::FUN_0040f060(int i)
{
    HeapNode_0040f000* node = items[i];
    while (true) {
        int left = i * 2 + 1;
        int right = i * 2 + 2;
        if (right < count) {
            HeapNode_0040f000* l = items[left];
            HeapNode_0040f000* r = items[right];
            if (r->key < l->key) {
                if (r->key >= node->key)
                    break;
                items[i] = r;
                r->index = i;
                i = right;
            } else {
                if (l->key >= node->key)
                    break;
                items[i] = l;
                l->index = i;
                i = left;
            }
        } else {
            if (left >= count)
                break;
            HeapNode_0040f000* l = items[left];
            if (l->key >= node->key)
                break;
            items[i] = l;
            items[left]->index = i;
            i = left;
        }
    }
    items[i] = node;
    node->index = i;
}
