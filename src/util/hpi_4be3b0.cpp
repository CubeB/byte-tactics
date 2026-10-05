// Decompiled by Opus. Names are provisional.

struct ArchiveDirectory;

#pragma pack(push, 1)
struct ArchiveEntry {
    int unknown_0;
    ArchiveDirectory* child;        // +0x4
    unsigned char flags;            // +0x8
};
#pragma pack(pop)

struct ArchiveDirectory {
    int count;
    ArchiveEntry* entries;          // +0x4
};

// FUNCTION: 0x4be3b0
void __stdcall HAPI_ClearShadowFlags(ArchiveDirectory* list)
{
    for (int i = list->count - 1; i >= 0; i--) {
        list->entries[i].flags &= ~2;
        if (list->entries[i].flags & 1)
            HAPI_ClearShadowFlags(list->entries[i].child);
    }
}
