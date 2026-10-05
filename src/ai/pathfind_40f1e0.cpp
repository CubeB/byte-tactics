// Decompiled by Opus. Names are provisional.
// Pushes entry `index` onto the free list threaded through the entries.

struct Entry_0040f1e0 {
    int next;                          // +0x0
    char unknown_4[0x10];
};

class Class_0040f1e0 {
public:
    Entry_0040f1e0* entries;           // +0x0
    int field_4;                       // +0x4
    int free_head;                     // +0x8

    void FreeNode(int index);
};

// FUNCTION: 0x40f1e0
void Class_0040f1e0::FreeNode(int index)
{
    entries[index].next = free_head;
    free_head = index;
}
