// Decompiled by Opus. Names are provisional.

struct List_004be3b0;

#pragma pack(push, 1)
struct Entry_004be3b0 {
    int unknown_0;
    List_004be3b0* child;           // +0x4
    unsigned char flags;            // +0x8
};
#pragma pack(pop)

struct List_004be3b0 {
    int count;
    Entry_004be3b0* entries;        // +0x4
};

// FUNCTION: 0x4be3b0
void __stdcall FUN_004be3b0(List_004be3b0* list)
{
    for (int i = list->count - 1; i >= 0; i--) {
        list->entries[i].flags &= ~2;
        if (list->entries[i].flags & 1)
            FUN_004be3b0(list->entries[i].child);
    }
}
