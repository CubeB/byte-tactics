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

struct Node_004be320 {
    char unknown_0[0x10];
    List_004be3b0* list;            // +0x10
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

State_004be320* FUN_004b6220(void);
void __stdcall FUN_004be3b0(List_004be3b0* list);
void __stdcall FUN_004be400(char* name, int a, int b);

// FUNCTION: 0x4be320
void FUN_004be320(void)
{
    State_004be320* state = FUN_004b6220();
    if (state->itemCount > 0) {
        for (int i = 0; i < state->itemCount; i++) {
            List_004be3b0* list = state->items[i]->node->list;
            for (int j = list->count - 1; j >= 0; j--) {
                list->entries[j].flags &= ~2;
                if (list->entries[j].flags & 1)
                    FUN_004be3b0(list->entries[j].child);
            }
        }
        FUN_004be400(DAT_005119b8, -1, 1);
    }
}
