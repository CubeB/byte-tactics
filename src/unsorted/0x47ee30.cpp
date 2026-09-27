// Decompiled by space-bunny-free. Names are provisional.

void __stdcall FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
struct Entry_0047f8c0 {                // 0x11 bytes
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int* data;                         // +0xc
    char field_10;                     // +0x10
};

struct List_0047f8c0 {
    Entry_0047f8c0 entries[9];         // +0x0
    int count;                         // +0x99
    int field_9d;                      // +0x9d
};
#pragma pack(pop)

struct Table_0047ee30 {                // 0x18 bytes
    int field_0;                       // +0x0
    int unknown_4;                     // +0x4
    int unknown_8;                     // +0x8
    int field_c;                       // +0xc
    int unknown_10;                    // +0x10
    int unknown_14;                    // +0x14
};

extern List_0047f8c0* DAT_0051e68c;
extern Table_0047ee30 DAT_005086e0[24];

// FUNCTION: 0x47ee30
void FUN_0047ee30()
{
    List_0047f8c0* list = DAT_0051e68c;
    int* count = &list->count;
    while (*count > 0) {
        int i = *count - 1;
        Entry_0047f8c0* e = list->entries + i;
        if (e->data != 0) {
            FUN_004d85a0(e->data);
            e->data = 0;
        }
        for (int j = i; j < *count; j++)
            list->entries[j] = list->entries[j + 1];
        (*count)--;
    }
    list->field_9d = 0;
    for (int k = 0; k < 24; k++)
        DAT_005086e0[k].field_c = 0;
}
