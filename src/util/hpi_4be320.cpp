// Decompiled by Opus. Names are provisional.
// Stays in its own file: HAPI_ClearShadowFlags (0x4be3b0, now in
// hpi_4bc4b0.cpp) would be inlined into the recursive call below, but the
// original calls it out of line.

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

struct Node_004be320 {
    char unknown_0[0x10];
    ArchiveDirectory* list;         // +0x10
};

struct Item_004be320 {
    char unknown_0[8];
    Node_004be320* node;            // +0x8
};

struct State_004be320 {
    char unknown_0[0x618];
    Item_004be320** items;          // +0x618
    int itemCount;                  // +0x61c
};

extern char DAT_005119b8[];

State_004be320* GetDisplay(void);
void __stdcall HAPI_ClearShadowFlags(ArchiveDirectory* list);
void __stdcall HAPI_MarkShadowedFiles(char* name, int a, int b);

// FUNCTION: 0x4be320
void HAPI_ResolveShadowedFiles(void)
{
    State_004be320* state = GetDisplay();
    if (state->itemCount > 0) {
        for (int i = 0; i < state->itemCount; i++) {
            ArchiveDirectory* list = state->items[i]->node->list;
            for (int j = list->count - 1; j >= 0; j--) {
                list->entries[j].flags &= ~2;
                if (list->entries[j].flags & 1)
                    HAPI_ClearShadowFlags(list->entries[j].child);
            }
        }
        HAPI_MarkShadowedFiles(DAT_005119b8, -1, 1);
    }
}
