// Decompiled by Opus. Names are provisional.
// Removes every entry whose field_8 equals id from the list at DAT_0051e68c,
// freeing its data and shifting the later entries down.
void __cdecl FUN_004d85a0(int* param_1);

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
};
#pragma pack(pop)

extern List_0047f8c0* DAT_0051e68c;

// The count is used through its own pointer: accessed as list->count, MSVC
// keeps the list pointer live instead of reusing its register for the entries.
// FUNCTION: 0x47f8c0
void __stdcall RemoveSpeechOfUnit(int id)
{
    int* count = &DAT_0051e68c->count;
    Entry_0047f8c0* entries = DAT_0051e68c->entries;
    int i = 0;
    while (i < *count) {
        if (entries[i].field_8 == id) {
            if (entries[i].data) {
                FUN_004d85a0(entries[i].data);
                entries[i].data = 0;
            }
            for (int j = i; j < *count; j++)
                entries[j] = entries[j + 1];
            (*count)--;
        } else {
            i++;
        }
    }
}
