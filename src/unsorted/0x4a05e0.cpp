// Decompiled by GPT-5.6-Terra, finished by muse-spark-1.3-free, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by claude-opus-5-5. Names are provisional.
// The two redundant type re-tests (cmp dl,dl at 0x4a064c and cmp dl,1 at 0x4a0679)
// come from separate if statements that re-test entry->type; the tolower calls
// are compared directly, with no named temporaries.
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
    Entry_004a05e0* scan;
    char* text;
    int length;
    Entry_004a05e0* entry;
    int i;
    int j;

    if (index == -1)
        return;

    entries = obj->data->entries;
    entry = entries + index;
    if (entry->type == 1) {
        if ((entry->flags & 0x10000) != 0)
            return;
    }
    if (entry->type == 5 && strlen(&entry->u.text[0x136 - 0xb6]) == 0)
        return;
    if (entry->type != 5 && entry->type != 1)
        return;

    if (entry->type == 1 && entry->u.text[0x136 - 0xb6] != 0) {
        entry->u.text[0x13a - 0xb6] = 0;
        return;
    }
    if (entry->type == 1) {
        if (strlen(entry->u.text) == 0)
            return;
        entry->u.text[0x13a - 0xb6] = 0;
        text = entry->u.text;
    } else if (entry->type == 5) {
        text = entry->u.text;
        entry->u.text[0x147 - 0xb6] = 0;
    }

    length = strlen(text);
    if (length == 0)
        return;

    for (i = 0; i < length; i++) {
        if (text[i] != ' ') {
            for (j = 0; j <= entries->u.count; j++) {
                scan = entries + j;
                if (scan->type == 1) {
                    if (tolower((signed char)scan->u.text[0x13a - 0xb6]) == tolower((signed char)text[i]))
                        break;
                } else if (scan->type == 5) {
                    if (tolower((signed char)scan->u.text[0x147 - 0xb6]) == tolower((signed char)text[i]))
                        break;
                }
            }
            if (j > entries->u.count) {
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