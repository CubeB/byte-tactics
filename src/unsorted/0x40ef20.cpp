// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
//
// PARTIAL: 74.1%. Everything from the first loop instruction to the end is
// byte-identical; the only remaining difference is register allocation in the
// prologue after the free-list push. The original keeps the decremented count
// in edi and the item array base in esi and materialises old*4 in edx:
//     mov edi, esi / mov esi, [ecx+4] / lea edx, [eax*4]
// while this version coalesces the count into esi and folds old*4 into the
// scaled address, so the same sequence is:
//     cmp eax, esi / mov edx, [ecx+4] / [edx + eax*4]
// The count/index source forms tried (--count, count-1 then count, --count
// then read, an explicit byte offset, a slot pointer) all produce the same
// folded code, so the difference looks like an inlining/allocator decision
// that needs different source phrasing.
//
// Removes entry `index`: pushes it onto the free list (threaded through the
// entry's first field), shrinks the heap by one, moves the last heap element
// into the removed slot and sifts it down the binary min-heap keyed on +0xc.

struct Entry_0040ef20 {
    int index;                          // +0x0
    char unknown_4[8];                  // +0x4
    int key;                            // +0xc
    char unknown_10[4];                 // +0x10
};

class Class_0040ef20 {
public:
    Entry_0040ef20* entries;            // +0x0
    Entry_0040ef20** items;             // +0x4
    int free_head;                      // +0x8
    char unknown_c[8];                  // +0xc
    int count;                          // +0x14

    void PushFree(int index);
    void SiftDown(int i, Entry_0040ef20* node);
    void FUN_0040ef20(int index);
};

inline void Class_0040ef20::PushFree(int index)
{
    entries[index].index = free_head;
    free_head = index;
}

inline void Class_0040ef20::SiftDown(int i, Entry_0040ef20* node)
{
    for (;;) {
        int right = i * 2 + 2;
        int left = i * 2 + 1;
        if (right < count) {
            Entry_0040ef20* l = items[left];
            Entry_0040ef20* r = items[right];
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
            Entry_0040ef20* l = items[left];
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

// FUNCTION: 0x40ef20
void Class_0040ef20::FUN_0040ef20(int index)
{
    int old = entries[index].index;
    PushFree(index);
    int n = --count;
    if (old >= n)
        return;
    items[old] = items[n];
    items[old]->index = old;
    SiftDown(old, items[old]);
}
