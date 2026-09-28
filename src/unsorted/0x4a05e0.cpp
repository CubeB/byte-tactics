// Decompiled by GPT-5.6-Terra. Names are provisional.
// PARTIAL (53.8%): remaining mismatch is MSVC's allocation of the persistent
// entry-table and text pointers, which shifts the inner scan's registers.
#include <string.h>

int __cdecl tolower(int);

#pragma pack(push, 1)
struct Entry_004a05e0 {                 // 0x15b bytes
    unsigned char type;                 // +0x0
    char unknown_1[0x1b - 0x1];
    unsigned int flags;                 // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    union {
        short count;                    // +0xb6 (entry 0)
        char text[0x15b - 0xb6];        // +0xb6
    } u;
};

struct Data_004a05e0 {
    int unknown_0;
    Entry_004a05e0* entries;            // +0x4
};

struct Object_004a05e0 {
    char unknown_0[0x18];
    Data_004a05e0* data;                // +0x18
};
#pragma pack(pop)

// FUNCTION: 0x4a05e0
void __stdcall FUN_004a05e0(Object_004a05e0* obj, int index)
{
    Entry_004a05e0* entries;
    Entry_004a05e0* entry;
    char* text;
    int length;
    int i;
    int j;
    int letter;

    if (index == -1)
        return;

    entries = obj->data->entries;
    entry = &entries[index];
    if (entry->type == 1 && (entry->flags & 0x10000) != 0)
        return;

    if (entry->type == 5) {
        if (strlen(&entry->u.text[0x136 - 0xb6]) == 0)
            return;
    } else if (entry->type != 1) {
        return;
    }

    if (entry->type == 1) {
        if (entry->u.text[0x136 - 0xb6] != 0) {
            entry->u.text[0x13a - 0xb6] = 0;
            return;
        }
        text = entry->u.text;
        if (strlen(text) == 0)
            return;
        entry->u.text[0x13a - 0xb6] = 0;
    } else if (entry->type == 5) {
        text = entry->u.text;
        entry->u.text[0x147 - 0xb6] = 0;
    }

    length = strlen(text);
    if (length == 0)
        return;

    for (i = 0; i < length; i++) {
        if (text[i] != ' ') {
            for (j = 0; j <= entries[0].u.count; j++) {
                if (entries[j].type == 1) {
                    letter = tolower((signed char)entries[j].u.text[0x13a - 0xb6]);
                    if (letter == tolower((signed char)text[i]))
                        break;
                } else if (entries[j].type == 5) {
                    letter = tolower((signed char)entries[j].u.text[0x147 - 0xb6]);
                    if (letter == tolower((signed char)text[i]))
                        break;
                }
            }
            if (j > entries[0].u.count) {
                if (entry->type == 1) {
                    entry->u.text[0x13a - 0xb6] = text[i];
                    return;
                }
                if (entry->type == 5) {
                    entry->u.text[0x147 - 0xb6] = text[i];
                    return;
                }
                return;
            }
        }
    }
}
