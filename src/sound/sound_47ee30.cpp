// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, continued by GPT-6.1-sol, continued by Space Bunny Free, matched by Space Bunny Free. Names are provisional.

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
void ResetSpeech()
{
    List_0047f8c0* list = DAT_0051e68c;
    if (list->count > 0) {
        int* count = &list->count;
        do {
            int i = *count - 1;
            // Entries reached through list->entries, no separate entries local.
            Entry_0047f8c0* last = &list->entries[i];
            if (last->data) {
                FUN_004d85a0(last->data);
                last->data = 0;
            }
            for (int j = i; j < *count; j++) {
                // Re-taken at the top of the body: this is what stops MSVC 5
                // folding &list->count into [ebp+0x99], and because the
                // assignment is loop invariant the lea is hoisted into the
                // shift loop's preheader, which is where the original has it.
                count = &list->count;
                list->entries[j] = list->entries[j + 1];
            }
            (*count)--;
        } while (*count > 0);
    }
    // Must go through list, not the global.
    list->field_9d = 0;
    for (int k = 0; k < 24; k++)
        DAT_005086e0[k].field_c = 0;
}
